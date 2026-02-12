bool drgstraight_cancel_x;
bool drgstraight_cancel_y;
void drgstraight_reset( void );
uint8_t drgstraight_get_sensitivity( void );
void drgstraight_set_sensitivity( uint8_t value );
void pointing_device_task_dragscroll_straighten(report_mouse_t mouse_report);

#if !defined(SCROLL_HISTORY_SIZE)
    #if defined(QMK_MCU_RP2040)
        #define SCROLL_HISTORY_SIZE 25
    #else
        #define SCROLL_HISTORY_SIZE 10
    #endif
#endif
#if !defined(SCROLL_HISTORY_FREQ)
    #if defined(QMK_MCU_RP2040)
        #define SCROLL_HISTORY_FREQ 10
    #else
        #define SCROLL_HISTORY_FREQ 25
    #endif
#endif