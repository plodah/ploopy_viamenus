#if defined(BETTER_DRAGSCROLL)
    #include <stdbool.h>
    #include <stdio.h>
    #include "quantum.h"
    #include "better_dragscroll.h"
    #include "ploopy_via.h"

    bool better_dragscroll_enabled_bylock = 0;
    bool better_dragscroll_enabled_bypress = 0;
    float dragscroll_acc_h = 0;
    float dragscroll_acc_v = 0;

    void better_dragscroll_toggle(bool pressed){
        dprintf("better_dragscroll_toggle\n");
        if(pressed){
            better_dragscroll_enabled_bypress ^= 1;
        }
    }

    void better_dragscroll_momentary(bool pressed){
        dprintf("better_dragscroll_momentary\n");
        better_dragscroll_enabled_bypress = pressed;
    }

    bool process_record_better_dragscroll(uint16_t keycode, keyrecord_t *record) {
        // if (!process_record_better_dragscroll_kb(keycode, record)) {
        //     return false;
        // }

        switch (keycode) {
            case BETTER_DRAG_SCROLL_MOMENTARY:
                better_dragscroll_momentary(record->event.pressed);
                return false;
            case BETTER_DRAG_SCROLL_TOGGLE:
                better_dragscroll_toggle(record->event.pressed);
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
                dragscroll_acc_h += (float)mouse_report.x / ((float)ploopyvia_config.dragscroll_divisor_h / 4);
                dragscroll_acc_v += (float)mouse_report.y / ((float)ploopyvia_config.dragscroll_divisor_v / 4);
            #else // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
                dragscroll_acc_h += (float)mouse_report.x / BETTER_DRAGSCROLL_DIVISOR_H;
                dragscroll_acc_v += (float)mouse_report.y / BETTER_DRAGSCROLL_DIVISOR_V;
            #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

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
        return mouse_report;
    }

    #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
        #if defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            static bool dragscroll_initial_led_state_initialized = false;
            static led_t dragscroll_initial_led_state;

            static uint32_t led_initialization_timeout(uint32_t trigger_time, void *cb_arg) {
                led_update_better_dragscroll();
                return 0; // Don't repeat
            }
        #endif // defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))

    void led_initialize_better_dragscroll(void) {
        #if defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            static const int init_timeout_delay = 1000;

            if (!ploopyvia_config.dragscroll_on_host_value_toggled) {
                led_update_better_dragscroll();
                return;
            }

            // We may receive multiple led update events during startup, so just
            // ignore them when in this mode. And only react to actual changes.
            defer_exec(init_timeout_delay, led_initialization_timeout, NULL);
        #else
            led_update_better_dragscroll_led_state(host_keyboard_led_state());
        #endif // defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
    }

    void led_update_better_dragscroll(void) {
        led_t host_state = host_keyboard_led_state();

        #if defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            dragscroll_initial_led_state_initialized = true;
        #endif // defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))

        if (ploopyvia_config.dragscroll_on_host_value_toggled) {
            dragscroll_initial_led_state = host_state;
        }

        led_update_better_dragscroll_led_state(host_state);
    }

    void led_deinitialize_better_dragscroll(void) {
        #if defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            dragscroll_initial_led_state_initialized = false;
        #endif // defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
    }

    bool led_update_better_dragscroll_led_state(led_t led_state) {
        static led_t default_initial_value = {0};
        led_t *initial_value = &default_initial_value;

        #if defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            if (!dragscroll_initial_led_state_initialized)
                return false;

            if (ploopyvia_config.dragscroll_on_host_value_toggled) {
                initial_value = &dragscroll_initial_led_state;
            }
        #endif


        if( false
        #if (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            || (ploopyvia_config.dragscroll_caps && led_state.caps_lock != initial_value->caps_lock)
            || (ploopyvia_config.dragscroll_num && led_state.num_lock != initial_value->num_lock)
            || (ploopyvia_config.dragscroll_scroll && led_state.scroll_lock != initial_value->scroll_lock)
        #else // (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
            #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE)
                || led_state.scroll_lock != initial_value->scroll_lock
            #endif // BETTER_DRAGSCROLL_SCRLK_ENABLE
            #if defined(BETTER_DRAGSCROLL_CAPLK_ENABLE)
                || led_state.caps_lock != initial_value->caps_lock
            #endif // BETTER_DRAGSCROLL_CAPLK_ENABLE
            #if defined(BETTER_DRAGSCROLL_NUMLK_ENABLE)
                || led_state.num_lock != initial_value->num_lock
            #endif // BETTER_DRAGSCROLL_NUMLK_ENABLE
        #endif // (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
        ){
            better_dragscroll_enabled_bylock = true;
        }
        else {better_dragscroll_enabled_bylock = false;}
        dprintf("better_dragscroll_enabled_bylock %d\n",better_dragscroll_enabled_bylock);
        return true;
    }
    #endif // defined(DRAGSCROLL_SCRLK_ENABLE) || defined(DRAGSCROLL_CAPLK_ENABLE)
#endif // defined(BETTER_DRAGSCROLL)
