int8_t history_x[SCROLL_HISTORY_SIZE];
int8_t history_y[SCROLL_HISTORY_SIZE];
uint16_t history_time[SCROLL_HISTORY_SIZE];
uint8_t history_head;
uint8_t history_tail;
uint8_t sensitivity;

bool drgstraight_cancel_x;
bool drgstraight_cancel_y;
uint8_t drgstraight_get_sensitivity(){
    return sensitivity; 
}
void drgstraight_set_sensitivity(uint8_t value){
    if(value > 100){
        sensitivity=100;
    } else{ 
        sensitivity = value;
    }
}

void drgstraight_reset( void ){
    history_tail = history_head;
    history_x[history_tail] = 0;
    history_y[history_tail] = 0;
}

void pointing_device_task_dragscroll_straighten(report_mouse_t mouse_report) {
    /* Borrowed and adapted from Obosob
    *  https://github.com/obosob/qmk_firmware/blob/2b1d6e6c31ac3ddf1e023143d46acafaac1103e5/keyboards/ploopyco/madromys/keymaps/obosob/keymap.c#L84
    * 
    *  Obosob's could negates horizontal scroll when the prevailing scroll direction is vertical.
    *  This code also negates vertical scroll when the prevailing scroll direction is horizontal.
    *  Additionally, the threshold for negating the lesser scroll direction is tuneable.
    */

    // When sampling frequency elapsed
    drgstraight_cancel_x = false;
    drgstraight_cancel_y = false;
    if ( !sensitivity ){ return; }
    if (timer_elapsed(history_time[history_head]) > SCROLL_HISTORY_FREQ) {
        //advance the head of the buffer.
        history_head = (history_head + 1) % SCROLL_HISTORY_SIZE;
        // if head has met the tail, advance the tail by one
        if(history_head == history_tail) {
            history_tail = (history_tail + 1) % SCROLL_HISTORY_SIZE;
        }
        // Start timer and initialise new frame to zero.
        history_time[history_head] = timer_read();
        history_x[history_head] = 0;
        history_y[history_head] = 0;
    }

    // Add mouse report to sample.
    history_x[history_head] += mouse_report.x;
    history_y[history_head] += mouse_report.y;

    // iterate over the history buffer, calculate the velocity for each time
    // step (average velocity for the sample frequency)
    float momentum_x = 0.0;
    float momentum_y = 0.0;
    int8_t i = history_tail;
    while ( (i + 1) % SCROLL_HISTORY_SIZE != (history_head + 1) % SCROLL_HISTORY_SIZE ) {
        momentum_x += (float)abs(history_x[(i + 1) % SCROLL_HISTORY_SIZE]) / (float)abs(timer_elapsed(history_time[(i + 1) % SCROLL_HISTORY_SIZE]) - timer_elapsed(history_time[i]));
        momentum_y += (float)abs(history_y[(i + 1) % SCROLL_HISTORY_SIZE]) / (float)abs(timer_elapsed(history_time[(i + 1) % SCROLL_HISTORY_SIZE]) - timer_elapsed(history_time[i]));
        i = (i + 1) % SCROLL_HISTORY_SIZE;
    }

    // If [sensitivity %] of VERTICAL momentum exceeds HORIZONTAL momentum
    if ( ((float)sensitivity / (float)100) * momentum_y > momentum_x ){
        // Clear HORIZONTAL accumulation 
        drgstraight_cancel_x = true;
        // dprintf("cleared horizontal accumulation \n");
    }
    // If [sensitivity %] of HORIZONTAL momentum exceeds VERTICAL momentum
    else if ( ((float)sensitivity / (float)100) * momentum_x > momentum_y ){
        // Clear VERTICAL accumulation where [sensitivity %] of HORIZONTAL momentum exceeds VERTICAL momentum
        drgstraight_cancel_y = true;
        // dprintf("cleared vertical accumulation \n");
    }
}
