#if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
    #pragma once

    // #include QMK_KEYBOARD_H
    #include "better_dragscroll.h"
    #include "via.h"
    #include "mouse_jiggler.h"
    #include "ploopy_via.h"
    #include "mouse_gesture.h"
    #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
      #include "pmw_rotation.h"
    #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE

    void ploopyvia_config_load(void) {
        // ploopyvia_config.raw = eeconfig_read_user();
        eeconfig_read_user_datablock(&ploopyvia_config, 0, EECONFIG_USER_DATA_SIZE);
    }

    void ploopyvia_config_save(void) {
        // eeconfig_update_user(ploopyvia_config.raw);
        eeconfig_update_user_datablock(&ploopyvia_config, 0, EECONFIG_USER_DATA_SIZE);
    }

    void update_dpi(void) {
        for (int i=0; i<5; i++){
            dpi_array[i] = ploopyvia_config.dpi_presets[i];
        }
        pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
        dprintf("Set CPI %d\n", dpi_array[keyboard_config.dpi_config]);
        eeconfig_update_kb(keyboard_config.raw);
    }

    #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
        void pmw_rotation_update_via(void) {
            int8_t pmw_rotation_temp;
            if(ploopyvia_config.pointer_rotation_is_ccw){
                pmw_rotation_temp = ploopyvia_config.pointer_rotation_value;
            }
            else{
                pmw_rotation_temp = - ploopyvia_config.pointer_rotation_value;
            }
            pmw_rotation_set_config(true, pmw_rotation_temp);
            pmw_rotation_config_to_sensor();
        }
        void pmw_rotation_update_via_keypress(void) {
            int8_t pmw_rotation_temp = pmw_rotation_get_config();
            if(pmw_rotation_temp < 0){
                ploopyvia_config.pointer_rotation_value = - pmw_rotation_temp;
                ploopyvia_config.pointer_rotation_is_ccw = 0;
            }
            else{
                ploopyvia_config.pointer_rotation_value = pmw_rotation_temp;
                ploopyvia_config.pointer_rotation_is_ccw = 1;
            }
        }
    #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE


    void keyboard_post_init_user_viamenus(void) {
        ploopyvia_config_load();
        if(ploopyvia_config.dpi_multiplier == 0){
            eeconfig_init_user();
        }
        update_dpi();
        #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
            pmw_rotation_update_via();
        #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
        ploopy_msGestureUpdate();
        led_update_better_dragscroll(host_keyboard_led_state());
        dprintf("keyboard_post_init_user\n");
    }

    void eeconfig_init_user(void) {
        ploopyvia_config = ploopyvia_config_default;
        ploopyvia_config_save();
        dprintf("eeconfig_init_user\n");
    }

    void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
        // data = [ command_id, channel_id, value_id, value_data ]
        uint8_t *command_id        = &(data[0]);
        uint8_t *channel_id        = &(data[1]);
        uint8_t *value_id_and_data = &(data[2]);

        if ( *channel_id == id_custom_channel ) {
            switch ( *command_id )
            {
                case id_custom_set_value:
                {
                    ploopyvia_config_set_value(value_id_and_data);
                    break;
                }
                case id_custom_get_value:
                {
                    ploopyvia_config_get_value(value_id_and_data);
                    break;
                }
                case id_custom_save:
                {
                    ploopyvia_config_save();
                    break;
                }
                default:
                {
                    *command_id = id_unhandled;
                    break;
                }
            }
            return;
        }

        *command_id = id_unhandled;
    }

    void ploopyvia_config_set_value( uint8_t *data )
    {
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);

        switch ( *value_id ) {
            case id_ploopystuff_dpi_activepreset:
                keyboard_config.dpi_config = *value_data;
                dprintf("dpi_preset: %d\n", keyboard_config.dpi_config);
                update_dpi();
                break;

            case id_ploopystuff_dpi_multiplier:
                for (int i = 0; i <  5; ++i) {
                    ploopyvia_config.dpi_presets[i] = ploopyvia_config.dpi_presets[i] / ploopyvia_config.dpi_multiplier * *value_data;
                }
                ploopyvia_config.dpi_multiplier = *value_data;
                dprintf("dpi_multiplier: %d\n", ploopyvia_config.dpi_multiplier);
                update_dpi();
                break;

            case id_ploopystuff_msjiggler_enabled:
                if( (msJigMainToken != INVALID_DEFERRED_TOKEN) ^ (*value_data) ) {
                    // ^ is an XOR, buddy
                    jiggler_toggle();
                    dprintf("msjiggler_enabled\n");
                }
                break;

            case id_ploopystuff_pointer_invert_h:
                ploopyvia_config.pointer_invert_h = *value_data;
                dprintf("pointer_invert_h:%d\n", ploopyvia_config.pointer_invert_h);
                break;

            case id_ploopystuff_pointer_invert_v:
                ploopyvia_config.pointer_invert_v = *value_data;
                dprintf("pointer_invert_v:%d\n", ploopyvia_config.pointer_invert_v);
                break;

            #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
            case id_ploopystuff_pointer_rotation_value:
                    ploopyvia_config.pointer_rotation_value = *value_data;
                    dprintf("pointer_rotation_value:%d\n", ploopyvia_config.pointer_rotation_value);
                    pmw_rotation_update_via();
                break;

            case id_ploopystuff_pointer_rotation_is_ccw:
                    ploopyvia_config.pointer_rotation_is_ccw = *value_data;
                    dprintf("pointer_rotation_is_ccw:%d\n", ploopyvia_config.pointer_rotation_is_ccw);
                    pmw_rotation_update_via();
                break;
            #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE

            case id_ploopystuff_gesture_count:
                ploopyvia_config.gesture_count = *value_data;
                ploopy_msGestureUpdate();
                dprintf("gesture_count:%d\n", ploopyvia_config.gesture_count);
                break;

            case id_ploopystuff_gesture_action_h:
                ploopyvia_config.gesture_action_h = *value_data;
                ploopy_msGestureUpdate();
                dprintf("gesture_action_h:%d\n", ploopyvia_config.gesture_action_h);
                break;

            case id_ploopystuff_gesture_action_v:
                ploopyvia_config.gesture_action_v = *value_data;
                ploopy_msGestureUpdate();
                dprintf("gesture_action_v:%d\n", ploopyvia_config.gesture_action_v);
                break;

            case id_ploopystuff_combos_enabled:
                ploopyvia_config.combos_enabled = *value_data;
                dprintf("combos_enabled:%d\n", ploopyvia_config.combos_enabled);
                break;

            case id_ploopystuff_dragscroll_invert_h:
                ploopyvia_config.dragscroll_invert_h = *value_data;
                dprintf("dragscroll_invert_h:%d\n", ploopyvia_config.dragscroll_invert_h);
                break;

            case id_ploopystuff_dragscroll_invert_v:
                ploopyvia_config.dragscroll_invert_v = *value_data;
                dprintf("dragscroll_invert_v:%d\n", ploopyvia_config.dragscroll_invert_v);
                break;

            case id_ploopystuff_dragscroll_divisor_h:
                ploopyvia_config.dragscroll_divisor_h = *value_data;
                dprintf("dragscroll_divisor_h:%d\n", ploopyvia_config.dragscroll_divisor_h);
                break;

            case id_ploopystuff_dragscroll_divisor_v:
                ploopyvia_config.dragscroll_divisor_v = *value_data;
                dprintf("dragscroll_divisor_v:%d\n", ploopyvia_config.dragscroll_divisor_v);
                break;

            case id_ploopystuff_dragscroll_caps:
                ploopyvia_config.dragscroll_caps = *value_data;
                dprintf("dragscroll_caps: %d\n", ploopyvia_config.dragscroll_caps);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_num:
                ploopyvia_config.dragscroll_num = *value_data;
                dprintf("dragscroll_num: %d\n", ploopyvia_config.dragscroll_num);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_scroll:
                ploopyvia_config.dragscroll_scroll = *value_data;
                dprintf("dragscroll_scroll: %d\n", ploopyvia_config.dragscroll_scroll);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_end_on_keypress:
                ploopyvia_config.dragscroll_end_on_keypress = *value_data;
                dprintf("dragscroll_end_on_keypress: %d\n", ploopyvia_config.dragscroll_end_on_keypress);
                break;

            case id_ploopystuff_dragscroll_permanently:
                ploopyvia_config.dragscroll_permanently = *value_data;
                dprintf("dragscroll_permanently: %d\n", ploopyvia_config.dragscroll_permanently);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_layer_a:
                ploopyvia_config.dragscroll_layer_a = *value_data;
                dprintf("dragscroll_layer_a: %d\n", ploopyvia_config.dragscroll_layer_a);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_layer_b:
                ploopyvia_config.dragscroll_layer_b = *value_data;
                dprintf("dragscroll_layer_b: %d\n", ploopyvia_config.dragscroll_layer_b);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dpi_presets:
                ploopyvia_config.dpi_presets[value_data[0]] = (value_data[1]*100) * (ploopyvia_config.dpi_multiplier/20);
                dprintf("dpi_presets[%d]: %d\n", value_data[0], value_data[1]);
                update_dpi();
                break;

            case id_ploopystuff_sniper_a_dpi:
                ploopyvia_config.sniper_a_dpi = ((uint8_t)*value_data*100) * (ploopyvia_config.dpi_multiplier/20);
                dprintf("sniper_a_dpi: %d\n", ploopyvia_config.sniper_a_dpi);
                break;

            case id_ploopystuff_sniper_b_dpi:
                ploopyvia_config.sniper_b_dpi = ((uint8_t)*value_data*100) * (ploopyvia_config.dpi_multiplier/20);
                dprintf("sniper_b_dpi: %d\n", ploopyvia_config.sniper_b_dpi);
                break;

            case id_ploopystuff_dragscroll_straighten_sensitivity:
                ploopyvia_config.dragscroll_straighten_sensitivity = *value_data;
                dprintf("dragscroll_straighten_sensitivity: %d\n", ploopyvia_config.dragscroll_straighten_sensitivity);
                break;

            case id_ploopystuff_dragscroll_dragact_a_up:
                ploopyvia_config.dragscroll_dragact_a_up = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_a_up: %d\n", ploopyvia_config.dragscroll_dragact_a_up);
                break;

            case id_ploopystuff_dragscroll_dragact_a_down:
                ploopyvia_config.dragscroll_dragact_a_down = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_a_down: %d\n", ploopyvia_config.dragscroll_dragact_a_down);
                break;

            case id_ploopystuff_dragscroll_dragact_a_left:
                ploopyvia_config.dragscroll_dragact_a_left = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_a_left: %d\n", ploopyvia_config.dragscroll_dragact_a_left);
                break;

            case id_ploopystuff_dragscroll_dragact_a_right:
                ploopyvia_config.dragscroll_dragact_a_right = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_a_right: %d\n", ploopyvia_config.dragscroll_dragact_a_right);
                break;

            case id_ploopystuff_dragscroll_dragact_b_up:
                ploopyvia_config.dragscroll_dragact_b_up = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_b_up: %d\n", ploopyvia_config.dragscroll_dragact_b_up);
                break;

            case id_ploopystuff_dragscroll_dragact_b_down:
                ploopyvia_config.dragscroll_dragact_b_down = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_b_down: %d\n", ploopyvia_config.dragscroll_dragact_b_down);
                break;

            case id_ploopystuff_dragscroll_dragact_b_left:
                ploopyvia_config.dragscroll_dragact_b_left = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_b_left: %d\n", ploopyvia_config.dragscroll_dragact_b_left);
                break;

            case id_ploopystuff_dragscroll_dragact_b_right:
                ploopyvia_config.dragscroll_dragact_b_right = value_data[0] << 8 | value_data[1];
                dprintf("dragscroll_dragact_b_right: %d\n", ploopyvia_config.dragscroll_dragact_b_right);
                break;
            case id_ploopystuff_dpi_as_slider:
                ploopyvia_config.dpi_as_slider = *value_data;
                dprintf("dpi_as_slider: %d\n", ploopyvia_config.dpi_as_slider);
                break;
        }
    }

    void ploopyvia_config_get_value( uint8_t *data )
    {
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);

        switch ( *value_id )
        {
            case id_ploopystuff_dpi_activepreset:
                *value_data = keyboard_config.dpi_config;
                dprintf("dpi_preset: %d\n", keyboard_config.dpi_config);
                break;

            case id_ploopystuff_dpi_multiplier:
                *value_data = ploopyvia_config.dpi_multiplier;
                dprintf("dpi_multiplier: %d\n", ploopyvia_config.dpi_multiplier);
                break;

            case id_ploopystuff_msjiggler_enabled:
                if(msJigMainToken != INVALID_DEFERRED_TOKEN){
                    dprintf("msjiggler_enabled true \n");
                    *value_data = true;
                }
                else{
                    dprintf("msjiggler_enabled false \n");
                    *value_data = false;
                }
                break;

            case id_ploopystuff_pointer_invert_h:
                *value_data = ploopyvia_config.pointer_invert_h;
                dprintf("pointer_invert_h:%d\n", ploopyvia_config.pointer_invert_h);
                break;

            case id_ploopystuff_pointer_invert_v:
                *value_data = ploopyvia_config.pointer_invert_v;
                dprintf("pointer_invert_v:%d\n", ploopyvia_config.pointer_invert_v);
                break;

            #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
            case id_ploopystuff_pointer_rotation_value:
                    *value_data = ploopyvia_config.pointer_rotation_value;
                    dprintf("pointer_rotation_value:%d\n", ploopyvia_config.pointer_rotation_value);
                break;

            case id_ploopystuff_pointer_rotation_is_ccw:
                    *value_data = ploopyvia_config.pointer_rotation_is_ccw;
                    dprintf("pointer_rotation_is_ccw:%d\n", ploopyvia_config.pointer_rotation_is_ccw);
                break;
            #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE

            case id_ploopystuff_gesture_count:
                *value_data = ploopyvia_config.gesture_count;
                dprintf("gesture_count:%d\n", ploopyvia_config.gesture_count);
                break;

            case id_ploopystuff_gesture_action_h:
                *value_data = ploopyvia_config.gesture_action_h;
                dprintf("gesture_action_h:%d\n", ploopyvia_config.gesture_action_h);
                break;

            case id_ploopystuff_gesture_action_v:
                *value_data = ploopyvia_config.gesture_action_v;
                dprintf("gesture_action_v:%d\n", ploopyvia_config.gesture_action_v);
                break;

            case id_ploopystuff_combos_enabled:
                *value_data = ploopyvia_config.combos_enabled;
                dprintf("combos_enabled:%d\n", ploopyvia_config.combos_enabled);
                break;

            case id_ploopystuff_dragscroll_invert_h:
                *value_data = ploopyvia_config.dragscroll_invert_h;
                dprintf("dragscroll_invert_h:%d\n", ploopyvia_config.dragscroll_invert_h);
                break;

            case id_ploopystuff_dragscroll_invert_v:
                *value_data = ploopyvia_config.dragscroll_invert_v;
                dprintf("dragscroll_invert_v:%d\n", ploopyvia_config.dragscroll_invert_v);
                break;

            case id_ploopystuff_dragscroll_divisor_h:
                *value_data = ploopyvia_config.dragscroll_divisor_h;
                dprintf("dragscroll_divisor_h:%d\n", ploopyvia_config.dragscroll_divisor_h);
                break;

            case id_ploopystuff_dragscroll_divisor_v:
                *value_data = ploopyvia_config.dragscroll_divisor_v;
                dprintf("dragscroll_divisor_v:%d\n", ploopyvia_config.dragscroll_divisor_v);
                break;

            case id_ploopystuff_dragscroll_caps:
                *value_data = ploopyvia_config.dragscroll_caps;
                dprintf("dragscroll_caps: %d\n", ploopyvia_config.dragscroll_caps);
                break;

            case id_ploopystuff_dragscroll_num:
                *value_data = ploopyvia_config.dragscroll_num;
                dprintf("dragscroll_num: %d\n", ploopyvia_config.dragscroll_num);
                break;

            case id_ploopystuff_dragscroll_scroll:
                *value_data = ploopyvia_config.dragscroll_scroll;
                dprintf("dragscroll_scroll: %d\n", ploopyvia_config.dragscroll_scroll);
                break;

            case id_ploopystuff_dragscroll_end_on_keypress:
                *value_data = ploopyvia_config.dragscroll_end_on_keypress;
                dprintf("dragscroll_end_on_keypress: %d\n", ploopyvia_config.dragscroll_end_on_keypress);
                break;

            case id_ploopystuff_dragscroll_permanently:
                *value_data = ploopyvia_config.dragscroll_permanently;
                dprintf("dragscroll_permanently: %d\n", ploopyvia_config.dragscroll_permanently);
                break;

            case id_ploopystuff_dragscroll_layer_a:
                *value_data = ploopyvia_config.dragscroll_layer_a;
                dprintf("dragscroll_layer_a: %d\n", ploopyvia_config.dragscroll_layer_a);
                break;

            case id_ploopystuff_dragscroll_layer_b:
                *value_data = ploopyvia_config.dragscroll_layer_b;
                dprintf("dragscroll_layer_b: %d\n", ploopyvia_config.dragscroll_layer_b);
                break;

            case id_ploopystuff_dpi_presets:
                value_data[1] = (ploopyvia_config.dpi_presets[value_data[0]] / 100) / (ploopyvia_config.dpi_multiplier/20);
                dprintf("dpi_presets[%d]: %d\n", value_data[0], ploopyvia_config.dpi_presets[value_data[0]]);
                break;

            case id_ploopystuff_sniper_a_dpi:
                *value_data = (ploopyvia_config.sniper_a_dpi / 100 ) / (ploopyvia_config.dpi_multiplier/20);
                dprintf("sniper_a_dpi: %d\n", ploopyvia_config.sniper_a_dpi);
                break;

            case id_ploopystuff_sniper_b_dpi:
                *value_data = (ploopyvia_config.sniper_b_dpi / 100 ) / (ploopyvia_config.dpi_multiplier/20);
                dprintf("sniper_b_dpi: %d\n", ploopyvia_config.sniper_b_dpi);
                break;

            case id_ploopystuff_dragscroll_straighten_sensitivity:
                *value_data = ploopyvia_config.dragscroll_straighten_sensitivity;
                dprintf("dragscroll_straighten_sensitivity: %d\n", ploopyvia_config.dragscroll_straighten_sensitivity);
                break;


            case id_ploopystuff_dragscroll_dragact_a_up:
                value_data[0] = ploopyvia_config.dragscroll_dragact_a_up >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_a_up & 0xFF;
                dprintf("dragscroll_dragact_a_up: %d\n", ploopyvia_config.dragscroll_dragact_a_up);
                break;

            case id_ploopystuff_dragscroll_dragact_a_down:
                value_data[0] = ploopyvia_config.dragscroll_dragact_a_down >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_a_down & 0xFF;
                dprintf("dragscroll_dragact_a_down: %d\n", ploopyvia_config.dragscroll_dragact_a_down);
                break;

            case id_ploopystuff_dragscroll_dragact_a_left:
                value_data[0] = ploopyvia_config.dragscroll_dragact_a_left >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_a_left & 0xFF;
                dprintf("dragscroll_dragact_a_left: %d\n", ploopyvia_config.dragscroll_dragact_a_left);
                break;

            case id_ploopystuff_dragscroll_dragact_a_right:
                value_data[0] = ploopyvia_config.dragscroll_dragact_a_right >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_a_right & 0xFF;
                dprintf("dragscroll_dragact_a_right: %d\n", ploopyvia_config.dragscroll_dragact_a_right);
                break;

            case id_ploopystuff_dragscroll_dragact_b_up:
                value_data[0] = ploopyvia_config.dragscroll_dragact_b_up >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_b_up & 0xFF;
                dprintf("dragscroll_dragact_b_up: %d\n", ploopyvia_config.dragscroll_dragact_b_up);
                break;

            case id_ploopystuff_dragscroll_dragact_b_down:
                value_data[0] = ploopyvia_config.dragscroll_dragact_b_down >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_b_down & 0xFF;
                dprintf("dragscroll_dragact_b_down: %d\n", ploopyvia_config.dragscroll_dragact_b_down);
                break;

            case id_ploopystuff_dragscroll_dragact_b_left:
                value_data[0] = ploopyvia_config.dragscroll_dragact_b_left >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_b_left & 0xFF;
                dprintf("dragscroll_dragact_b_left: %d\n", ploopyvia_config.dragscroll_dragact_b_left);
                break;

            case id_ploopystuff_dragscroll_dragact_b_right:
                value_data[0] = ploopyvia_config.dragscroll_dragact_b_right >> 8;
                value_data[1] = ploopyvia_config.dragscroll_dragact_b_right & 0xFF;
                dprintf("dragscroll_dragact_b_right: %d\n", ploopyvia_config.dragscroll_dragact_b_right);
                break;

            case id_ploopystuff_dpi_as_slider:
                *value_data = ploopyvia_config.dpi_as_slider;
                dprintf("dpi_as_slider: %d\n", ploopyvia_config.dpi_as_slider);
                break;

            case id_ploopystuff_mcu_type:
                #if defined(QMK_MCU_RP2040)
                    dprintf("mcu_type: MCU_RP2040\n");
                    *value_data = MCU_RP2040;
                #elif defined(QMK_MCU_ATMEGA32U4)
                    #pragma message "MCU_ATMEGA32U4"
                    dprintf("mcu_type: MCU_ATMEGA32U4\n");
                    *value_data = MCU_ATMEGA;
                #elif defined(QMK_MCU_STM32L432)
                    dprintf("mcu_type: MCU_AVR\n");
                    *value_data = MCU_AVR;
                #else
                    dprintf("mcu_type: MCU_UNKNOWN");
                    *value_data = MCU_UNKNOWN;
                #endif
                break;

            case id_ploopystuff_sensor_type:
                #if defined(POINTING_DEVICE_DRIVER_PMW3360)
                    dprintf("sensor_type: SENSOR_PMW3360\n");
                    *value_data = SENSOR_PMW3360;
                #elif defined(POINTING_DEVICE_DRIVER_ADNS5050)
                    dprintf("sensor_type: SENSOR_ADNS5050\n");
                    *value_data = SENSOR_ADNS5050;
                #elif defined(POINTING_DEVICE_DRIVER_PAW3222)
                    dprintf("sensor_type: SENSOR_PAW3222\n");
                    *value_data = SENSOR_PAW3222;
                #else
                    dprintf("sensor_type: SENSOR_UNKNOWN\n");
                    *value_data = SENSOR_UNKNOWN;
                #endif
                break;

            case id_ploopystuff_sensor_rotation_available:
                #if defined(POINTING_DEVICE_DRIVER_PMW3360)
                    #if defined(COMMUNITY_MODULE_PMW_ROTATION_ENABLE)
                        dprintf("sensor_rotation_available: 1\n");
                        *value_data = 1;
                    #else
                        dprintf("sensor_rotation_available: 2\n");
                        *value_data = 2;
                    #endif
                #else
                    dprintf("sensor_rotation_available: 0\n");
                    *value_data = 0;
                #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
                break;
        }
    }

#endif // if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
