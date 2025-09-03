/* Copyright 2025 Plodah
 * Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 #if defined(BETTER_DRAGSCROLL)
    #include <stdbool.h>
    #include <stdio.h>
    #include "quantum.h"
    #include "better_dragscroll.h"
    #include "ploopy_via.h"

    #if !defined(SCROLL_HISTORY_SIZE)
        #if defined(RP2040)
            #define SCROLL_HISTORY_SIZE 25
        #else
            #define SCROLL_HISTORY_SIZE 10
        #endif
    #endif
    #if !defined(SCROLL_HISTORY_FREQ)
        #if defined(RP2040)
            #define SCROLL_HISTORY_FREQ 10
        #else
            #define SCROLL_HISTORY_FREQ 25
        #endif
    #endif

    int8_t drgstraight_history_x[SCROLL_HISTORY_SIZE];
    int8_t drgstraight_history_y[SCROLL_HISTORY_SIZE];
    uint16_t drgstraight_history_time[SCROLL_HISTORY_SIZE];
    uint8_t drgstraight_history_head;
    uint8_t drgstraight_history_tail;
    float drgstraight_momentum_x;
    float drgstraight_momentum_y;
    int8_t drgstraight_i;

    bool better_dragscroll_enabled_bylock = 0;
    bool better_dragscroll_enabled_bypress = 0;
    bool better_dragscroll_sniper_a_enabled = 0;
    bool better_dragscroll_sniper_b_enabled = 0;
    bool better_dragscroll_dragaction_enabled = 0;
    float dragscroll_acc_h = 0;
    float dragscroll_acc_v = 0;

    void better_dragscroll_resetacc(void){
        dragscroll_acc_h = 0;
        dragscroll_acc_v = 0;
        if(ploopyvia_config.dragscroll_straighten_sensitivity){
            drgstraight_history_tail = drgstraight_history_head;
            drgstraight_history_x[drgstraight_history_tail] = 0;
            drgstraight_history_y[drgstraight_history_tail] = 0;
        }
    }

    void better_dragscroll_toggle(bool pressed){
        dprintf("better_dragscroll_toggle\n");
        if(pressed){
            better_dragscroll_enabled_bypress ^= 1;
            if(better_dragscroll_enabled_bypress){
                better_dragscroll_resetacc();
            }
        }
    }

    void better_dragscroll_momentary(bool pressed){
        dprintf("better_dragscroll_momentary\n");
        better_dragscroll_enabled_bypress = pressed;
        if(pressed){
            better_dragscroll_resetacc();
        }
    }

    bool process_record_better_dragscroll(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {
            case BETTER_DRAG_SCROLL_MOMENTARY:
                better_dragscroll_momentary(record->event.pressed);
                return false;
            case BETTER_DRAG_SCROLL_TOGGLE:
                better_dragscroll_toggle(record->event.pressed);
                return false;
            case BETTER_DRAG_ACTION_A_MOMENTARY:
            case BETTER_DRAG_ACTION_B_MOMENTARY:
                better_dragscroll_dragaction_enabled = record->event.pressed;
                if(record->event.pressed){
                    better_dragscroll_resetacc();
                    if(keycode == BETTER_DRAG_ACTION_B_MOMENTARY){
                        better_dragscroll_dragaction_alt = true;
                    }
                    else{
                        better_dragscroll_dragaction_alt = false;
                    }
                }
                return false;
            case BETTER_DRAG_SCROLL_SNIPER_A_MOMENTARY:
                if(record->event.pressed){
                    pointing_device_set_cpi(ploopyvia_config.sniper_a_dpi);
                }
                else{
                    pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
                }
                return false;
            case BETTER_DRAG_SCROLL_SNIPER_A_TOGGLE:
                if(record->event.pressed){
                    better_dragscroll_sniper_a_enabled ^= 1;
                    if(better_dragscroll_sniper_a_enabled){
                        pointing_device_set_cpi(ploopyvia_config.sniper_a_dpi);
                    }
                    else{
                        pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
                    }
                    dprintf("sniper_a_enabled:%d dpi:%d\n", better_dragscroll_sniper_a_enabled, ploopyvia_config.sniper_a_dpi);
                }
                return false;

            case BETTER_DRAG_SCROLL_SNIPER_B_MOMENTARY:
                if(record->event.pressed){
                    pointing_device_set_cpi(ploopyvia_config.sniper_b_dpi);
                }
                else{
                    pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
                }
                return false;

            case BETTER_DRAG_SCROLL_SNIPER_B_TOGGLE:
                if(record->event.pressed){
                    better_dragscroll_sniper_b_enabled ^= 1;
                    if(better_dragscroll_sniper_b_enabled){
                        pointing_device_set_cpi(ploopyvia_config.sniper_b_dpi);
                    }
                    else{
                        pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
                    }
                    dprintf("sniper_b_enabled:%d dpi:%d\n", better_dragscroll_sniper_b_enabled, ploopyvia_config.sniper_b_dpi);
                }
                return false;

            #if !defined(BETTER_DRAGSCROLL_INDEFINITE)
                default:
                    if(record->event.pressed){
                        #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                            if(ploopyvia_config.dragscroll_end_on_keypress){
                                better_dragscroll_enabled_bypress = 0;
                            }
                        #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                            #if defined(BETTER_DRAGSCROLL_END_ON_KEYPRESS)
                                better_dragscroll_enabled_bypress = 0;
                            #endif // defined(BETTER_DRAGSCROLL_END_ON_KEYPRESS)
                        #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                    }
                    break;
            #endif // !defined(BETTER_DRAGSCROLL_INDEFINITE)
        }
        return true;
    }

    report_mouse_t pointing_device_task_better_dragscroll(report_mouse_t mouse_report) {
        #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
            if(ploopyvia_config.pointer_invert_h){
                mouse_report.x = -mouse_report.x;
            }
            if(ploopyvia_config.pointer_invert_v){
                mouse_report.y = -mouse_report.y;
            }
        #else
            #if defined(BETTER_DRAGSCROLL_POINTERINVERT_X)
                mouse_report.x = -mouse_report.x;
            #endif // BETTER_DRAGSCROLL_POINTERINVERT_X
            #if defined(BETTER_DRAGSCROLL_POINTERINVERT_Y)
                mouse_report.y = -mouse_report.y;
            #endif // BETTER_DRAGSCROLL_POINTERINVERT_Y
        #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

        if (better_dragscroll_enabled_bylock || better_dragscroll_enabled_bypress) {

            #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                if(ploopyvia_config.dragscroll_divisor_h > 0){
                    dragscroll_acc_h += (float)mouse_report.x / ((float)ploopyvia_config.dragscroll_divisor_h / 4);
                }
                if(ploopyvia_config.dragscroll_divisor_v > 0){
                    dragscroll_acc_v += (float)mouse_report.y / ((float)ploopyvia_config.dragscroll_divisor_v / 4);
                }
                #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                dragscroll_acc_h += (float)mouse_report.x / BETTER_DRAGSCROLL_DIVISOR_H;
                dragscroll_acc_v += (float)mouse_report.y / BETTER_DRAGSCROLL_DIVISOR_V;
            #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

            if(ploopyvia_config.dragscroll_straighten_sensitivity){

                // Borrowed and adapted from Obosob
                // https://github.com/obosob/qmk_firmware/blob/2b1d6e6c31ac3ddf1e023143d46acafaac1103e5/keyboards/ploopyco/madromys/keymaps/obosob/keymap.c#L84
                //
                // Obosob's version would negate only HORIZONTAL scroll where there was more prevailing VERTICAL scroll
                // This will negate either horizontal or vertical scroll depending on momentum
                // Sensitivity % is tunable to allow some disgonal scrolling through.
                //
                // Removed a bunch of variables, which were used only once. Not sure what that achieved apart from making it more difficult to read.

                // if it's been more than the sampling frequency: advance the head of
                // the buffer, initialise that frame to zero, and take a timer reading
                if (timer_elapsed(drgstraight_history_time[drgstraight_history_head]) > SCROLL_HISTORY_FREQ) {
                    drgstraight_history_head = (drgstraight_history_head + 1) % SCROLL_HISTORY_SIZE;
                    // if head has met the tail, advance the tail by one
                    if(drgstraight_history_head == drgstraight_history_tail) {
                        drgstraight_history_tail = (drgstraight_history_tail + 1) % SCROLL_HISTORY_SIZE;
                    }
                    drgstraight_history_time[drgstraight_history_head] = timer_read();
                    drgstraight_history_x[drgstraight_history_head] = 0;
                    drgstraight_history_y[drgstraight_history_head] = 0;
                }

                // add the mouse report to the sample (note that this is signed, it
                // results in the total amount the cursor would have moved from the
                // start to end of the frame
                drgstraight_history_x[drgstraight_history_head] += mouse_report.x;
                drgstraight_history_y[drgstraight_history_head] += mouse_report.y;

                // iterate over the history buffer, calculate the velocity for each time
                // step (average velocity for the sample frequency)
                drgstraight_momentum_x = 0.0;
                drgstraight_momentum_y = 0.0;
                drgstraight_i = drgstraight_history_tail;

                while ( (drgstraight_i + 1) % SCROLL_HISTORY_SIZE != (drgstraight_history_head + 1) % SCROLL_HISTORY_SIZE ) {
                    drgstraight_momentum_x += (float)abs(drgstraight_history_x[(drgstraight_i + 1) % SCROLL_HISTORY_SIZE]) / (float)abs(timer_elapsed(drgstraight_history_time[(drgstraight_i + 1) % SCROLL_HISTORY_SIZE]) - timer_elapsed(drgstraight_history_time[drgstraight_i]));
                    drgstraight_momentum_y += (float)abs(drgstraight_history_y[(drgstraight_i + 1) % SCROLL_HISTORY_SIZE]) / (float)abs(timer_elapsed(drgstraight_history_time[(drgstraight_i + 1) % SCROLL_HISTORY_SIZE]) - timer_elapsed(drgstraight_history_time[drgstraight_i]));
                    drgstraight_i = (drgstraight_i + 1) % SCROLL_HISTORY_SIZE;
                }

                // dprintf("velocity: x:%d y:%d (/10,000)\n", (int16_t)(drgstraight_momentum_x * 10000), (int16_t)(drgstraight_momentum_y * 10000));
                if ( ((float)ploopyvia_config.dragscroll_straighten_sensitivity / (float)100) * drgstraight_momentum_y > drgstraight_momentum_x ){
                    // Clear HORIZONTAL accumulation where [sensitivity %] of VERTICAL momentum exceeds HORIZONTAL momentum
                    dragscroll_acc_h = 0;
                    // dprintf("cleared horizontal accumulation \n");
                }
                else if ( ((float)ploopyvia_config.dragscroll_straighten_sensitivity / (float)100) * drgstraight_momentum_x > drgstraight_momentum_y ){
                    // Clear VERTICAL accumulation where [sensitivity %] of HORIZONTAL momentum exceeds VERTICAL momentum
                    dragscroll_acc_v = 0;
                    // dprintf("cleared vertical accumulation \n");
                }
            } // ploopyvia_config.dragscroll_straighten_sensitivity

            // Assign integer parts of accumulated scroll values to the mouse report
            #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                if(ploopyvia_config.dragscroll_invert_h) {
                    mouse_report.h = -(int8_t)dragscroll_acc_h;
                } else {
                    mouse_report.h = (int8_t)dragscroll_acc_h;
                }

                if(ploopyvia_config.dragscroll_invert_v) {
                    mouse_report.v = -(int8_t)dragscroll_acc_v;
                } else {
                    mouse_report.v = (int8_t)dragscroll_acc_v;
                }
            #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                #if defined(BETTER_DRAGSCROLL_INVERT_H)
                    mouse_report.h = -(int8_t)dragscroll_acc_h;
                #else
                    mouse_report.h = (int8_t)dragscroll_acc_h;
                #endif // BETTER_DRAGSCROLL_INVERT_V

                #ifdef BETTER_DRAGSCROLL_INVERT_V
                    mouse_report.v = -(int8_t)dragscroll_acc_v;
                #else
                    mouse_report.v = (int8_t)dragscroll_acc_v;
                #endif // BETTER_DRAGSCROLL_INVERT_V
            #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

            // Update accumulated scroll values by subtracting the integer parts
            dragscroll_acc_h -= (int8_t)dragscroll_acc_h;
            dragscroll_acc_v -= (int8_t)dragscroll_acc_v;

            // Clear the X and Y values of the mouse report
            mouse_report.x = 0;
            mouse_report.y = 0;
        }
        else if (better_dragscroll_dragaction_enabled){
            #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                if(ploopyvia_config.dragscroll_divisor_h > 0){
                    dragscroll_acc_h += (float)mouse_report.x / (((float)ploopyvia_config.dragscroll_divisor_h / 4) * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR);
                }
                if(ploopyvia_config.dragscroll_divisor_v > 0){
                    dragscroll_acc_v += (float)mouse_report.y / (((float)ploopyvia_config.dragscroll_divisor_v / 4) * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR);
                }
            #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                dragscroll_acc_h += (float)mouse_report.x / (BETTER_DRAGSCROLL_DIVISOR_H * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR);
                dragscroll_acc_v += (float)mouse_report.y / (BETTER_DRAGSCROLL_DIVISOR_V * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR);
            #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

            if( abs(dragscroll_acc_v) >= 1 || abs(dragscroll_acc_h) >= 1 )  {
                dprintf("v:%d\n", (int)dragscroll_acc_v);
                if( abs(dragscroll_acc_v) > abs(dragscroll_acc_h) ){
                    if( ( dragscroll_acc_v <= -1 && !ploopyvia_config.dragscroll_invert_v) || (dragscroll_acc_v >= 1 && ploopyvia_config.dragscroll_invert_v)) {
                        if(!better_dragscroll_dragaction_alt){
                            tap_code16(ploopyvia_config.dragscroll_dragact_a_down);
                            dprintf("a_down\n");
                        }
                        else{
                            tap_code16(ploopyvia_config.dragscroll_dragact_b_down);
                            dprintf("b_down\n");
                        }
                    }
                    else {
                        if(!better_dragscroll_dragaction_alt){
                            tap_code16(ploopyvia_config.dragscroll_dragact_a_up);
                            dprintf("a_up\n");
                        }
                        else{
                            tap_code16(ploopyvia_config.dragscroll_dragact_b_up);
                            dprintf("b_up\n");
                        }
                    }
                }
                else {
                    if( (dragscroll_acc_h <= -1 && !ploopyvia_config.dragscroll_invert_h) || (dragscroll_acc_h >= 1 && ploopyvia_config.dragscroll_invert_h)){
                        if(!better_dragscroll_dragaction_alt){
                            tap_code16(ploopyvia_config.dragscroll_dragact_a_left);
                            dprintf("a_left\n");
                        }
                        else{
                            tap_code16(ploopyvia_config.dragscroll_dragact_b_left);
                            dprintf("b_left\n");
                        }
                    }
                    else {
                        if(!better_dragscroll_dragaction_alt){
                            tap_code16(ploopyvia_config.dragscroll_dragact_a_right);
                            dprintf("a_right\n");
                        }
                        else{
                            tap_code16(ploopyvia_config.dragscroll_dragact_b_right);
                            dprintf("b_right\n");
                        }
                    }
                }
                better_dragscroll_resetacc();
            }
            mouse_report.x = 0;
            mouse_report.y = 0;
        }

        return mouse_report;
    }

    #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
    bool led_update_better_dragscroll(led_t led_state) {

        if( false
        #if (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            || ( ploopyvia_config.dragscroll_caps == 1 && led_state.caps_lock)
            || ( ploopyvia_config.dragscroll_caps == 2 && ! led_state.caps_lock )
            || ( ploopyvia_config.dragscroll_num == 1 && led_state.num_lock)
            || ( ploopyvia_config.dragscroll_num == 2 && ! led_state.num_lock )
            || ( ploopyvia_config.dragscroll_scroll == 1 && led_state.scroll_lock)
            || ( ploopyvia_config.dragscroll_scroll == 2 && ! led_state.scroll_lock )
            || ( ploopyvia_config.dragscroll_permanently )
        #else // (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE)
                || led_state.scroll_lock
            #endif // BETTER_DRAGSCROLL_SCRLK_ENABLE
            #if defined(BETTER_DRAGSCROLL_CAPLK_ENABLE)
                || led_state.caps_lock
            #endif // BETTER_DRAGSCROLL_CAPLK_ENABLE
            #if defined(BETTER_DRAGSCROLL_NUMLK_ENABLE)
                || led_state.num_lock
            #endif // BETTER_DRAGSCROLL_NUMLK_ENABLE
        #endif // (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
        ){
            better_dragscroll_enabled_bylock = true;
            better_dragscroll_resetacc();
        }
        else {
            better_dragscroll_enabled_bylock = false;
        }
        dprintf("better_dragscroll_enabled_bylock %d\n",better_dragscroll_enabled_bylock);
        return true;
    }
    #endif // defined(DRAGSCROLL_SCRLK_ENABLE) || defined(DRAGSCROLL_CAPLK_ENABLE)
#endif // defined(BETTER_DRAGSCROLL)
