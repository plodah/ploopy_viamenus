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
    #if defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
      #include "dragscroll_straighten.h"
    #endif // defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)

    int8_t accumulated_h = 0;
    int8_t accumulated_v = 0;

    void better_dragscroll_init(void){
        ds_state = dragscroll_state_default;
    }

    void better_dragscroll_resetacc(void){
        accumulated_h = 0;
        accumulated_v = 0;
        #if defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
            drgstraight_reset();
        #endif // defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
    }

    void better_dragscroll_toggle(bool pressed){
        dprintf("better_dragscroll_toggle\n");
        if(pressed){
            ds_state.enabled_bypress ^= 1;
            if(ds_state.enabled_bypress){
                better_dragscroll_resetacc();
            }
        }
    }

    void better_dragscroll_momentary(bool pressed){
        dprintf("better_dragscroll_momentary\n");
        ds_state.enabled_bypress = pressed;
        if(pressed){
            better_dragscroll_resetacc();
        }
    }

    void better_dragscroll_set_dpi( void ){
        if(ds_state.sniper_b_enabled){
            pointing_device_set_cpi(ploopyvia_config.sniper_b_dpi);
        }
        else if(ds_state.sniper_a_enabled){
            pointing_device_set_cpi(ploopyvia_config.sniper_a_dpi);
        }
        else {
            pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
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
                ds_state.dragaction_enabled = record->event.pressed;
                if(record->event.pressed){
                    better_dragscroll_resetacc();
                    if(keycode == BETTER_DRAG_ACTION_B_MOMENTARY){
                        ds_state.dragaction_alt = true;
                    }
                    else{
                        ds_state.dragaction_alt = false;
                    }
                }
                return false;

            case BETTER_DRAG_SCROLL_SNIPER_A_MOMENTARY:
                ds_state.sniper_a_enabled = record->event.pressed;
                better_dragscroll_set_dpi();
                dprintf("sniper_a_enabled:%d dpi:%d\n", ds_state.sniper_a_enabled, ploopyvia_config.sniper_a_dpi);
                return false;

            case BETTER_DRAG_SCROLL_SNIPER_A_TOGGLE:
                if(record->event.pressed){
                    ds_state.sniper_a_enabled ^= 1;
                    better_dragscroll_set_dpi();
                    dprintf("sniper_a_enabled:%d dpi:%d\n", ds_state.sniper_a_enabled, ploopyvia_config.sniper_a_dpi);
                }
                return false;

            case BETTER_DRAG_SCROLL_SNIPER_B_MOMENTARY:
                ds_state.sniper_b_enabled = record->event.pressed;
                better_dragscroll_set_dpi();
                dprintf("sniper_b_enabled:%d dpi:%d\n", ds_state.sniper_b_enabled, ploopyvia_config.sniper_b_dpi);
                return false;

            case BETTER_DRAG_SCROLL_SNIPER_B_TOGGLE:
                if(record->event.pressed){
                    ds_state.sniper_b_enabled ^= 1;
                    better_dragscroll_set_dpi();
                    dprintf("sniper_b_enabled:%d dpi:%d\n", ds_state.sniper_b_enabled, ploopyvia_config.sniper_b_dpi);
                }
                return false;

            #if !defined(BETTER_DRAGSCROLL_INDEFINITE)
                default:
                    if(record->event.pressed){
                        #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                            if(ploopyvia_config.dragscroll_end_on_keypress){
                                ds_state.enabled_bypress = 0;
                            }
                        #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                            #if defined(BETTER_DRAGSCROLL_END_ON_KEYPRESS)
                                ds_state.enabled_bypress = 0;
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

        if (ds_state.enabled_bylock || ds_state.enabled_bypress) {
            #if defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
                if(ploopyvia_config.dragscroll_straighten_sensitivity){
                    mouse_report = pointing_device_task_dragscroll_straighten(mouse_report);
                    if ( drgstraight_cancel_x ){ accumulated_h = 0; }
                    if ( drgstraight_cancel_y ){ accumulated_v = 0; }
                }
            #endif // defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)

            #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                if(ploopyvia_config.dragscroll_divisor_h > 0){
                    mouse_report.h = (accumulated_h + mouse_report.x) / (ploopyvia_config.dragscroll_divisor_h / 4);
                    if(ploopyvia_config.dragscroll_invert_h){mouse_report.h = -mouse_report.h; }
                    accumulated_h = (accumulated_h + mouse_report.x) % (ploopyvia_config.dragscroll_divisor_h / 4);
                }
                if(ploopyvia_config.dragscroll_divisor_v > 0){
                    mouse_report.v = (accumulated_v + mouse_report.y) / (ploopyvia_config.dragscroll_divisor_v / 4);
                    if(ploopyvia_config.dragscroll_invert_v){mouse_report.v = -mouse_report.v; }
                    accumulated_v = (accumulated_v + mouse_report.y) % (ploopyvia_config.dragscroll_divisor_v / 4);
                }
            #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                mouse_report.h = (accumulated_h + mouse_report.x) / BETTER_DRAGSCROLL_DIVISOR_H;
                if(ploopyvia_config.dragscroll_invert_h){mouse_report.h = -mouse_report.h; }
                accumulated_h = (accumulated_h + mouse_report.x) % BETTER_DRAGSCROLL_DIVISOR_H;

                mouse_report.v = (accumulated_v + mouse_report.y) / BETTER_DRAGSCROLL_DIVISOR_V;
                if(ploopyvia_config.dragscroll_invert_v){mouse_report.v = -mouse_report.v; }
                accumulated_v = (accumulated_v + mouse_report.y) % BETTER_DRAGSCROLL_DIVISOR_V;
            #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

            // Clear the X and Y values of the mouse report
            mouse_report.x = 0;
            mouse_report.y = 0;
        }
        else if (ds_state.dragaction_enabled){


            if(
                #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                    (
                        ploopyvia_config.dragscroll_divisor_h > 0 &&
                        1 <= ((accumulated_h + mouse_report.x) /
                          (ploopyvia_config.dragscroll_divisor_h * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR))
                    ) ||
                    (
                        ploopyvia_config.dragscroll_divisor_v > 0 &&
                        1 <= ((accumulated_v + mouse_report.y) /
                          (ploopyvia_config.dragscroll_divisor_v * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR))
                    )
                #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                    (
                        1 <= ((accumulated_h + mouse_report.x) /
                          (BETTER_DRAGSCROLL_DIVISOR_H * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR))
                    ) ||
                    (
                        1 <= ((accumulated_v + mouse_report.y) /
                          (BETTER_DRAGSCROLL_DIVISOR_V * BETTER_DRAGSCROLL_DRAGACTION_DIVISOR))
                    )
                #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
            )  {
                dprintf("v:%d\n", (int)accumulated_v);
                if( abs(accumulated_v) > abs(accumulated_h) ){
                    if( ( accumulated_v <= -1 && !ploopyvia_config.dragscroll_invert_v) || (accumulated_v >= 1 && ploopyvia_config.dragscroll_invert_v)) {
                        if(!ds_state.dragaction_alt){
                            tap_code16(ploopyvia_config.dragscroll_dragact_a_down);
                            dprintf("a_down\n");
                        }
                        else{
                            tap_code16(ploopyvia_config.dragscroll_dragact_b_down);
                            dprintf("b_down\n");
                        }
                    }
                    else {
                        if(!ds_state.dragaction_alt){
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
                    if( (accumulated_h <= -1 && !ploopyvia_config.dragscroll_invert_h) || (accumulated_h >= 1 && ploopyvia_config.dragscroll_invert_h)){
                        if(!ds_state.dragaction_alt){
                            tap_code16(ploopyvia_config.dragscroll_dragact_a_left);
                            dprintf("a_left\n");
                        }
                        else{
                            tap_code16(ploopyvia_config.dragscroll_dragact_b_left);
                            dprintf("b_left\n");
                        }
                    }
                    else {
                        if(!ds_state.dragaction_alt){
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
        else{
            better_dragscroll_resetacc();
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
            || ( ploopyvia_config.dragscroll_layer_a == get_highest_layer(layer_state | default_layer_state) )
            || ( ploopyvia_config.dragscroll_layer_b == get_highest_layer(layer_state | default_layer_state) )
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
            ds_state.enabled_bylock = true;
            better_dragscroll_resetacc();
        }
        else {
            ds_state.enabled_bylock = false;
        }
        dprintf("ds_state.enabled_bylock %d\n",ds_state.enabled_bylock);
        return true;
    }
    #endif // defined(DRAGSCROLL_SCRLK_ENABLE) || defined(DRAGSCROLL_CAPLK_ENABLE)
#endif // defined(BETTER_DRAGSCROLL)
