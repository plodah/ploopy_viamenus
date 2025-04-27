#if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
    #pragma once

    // #include QMK_KEYBOARD_H
    #include "better_dragscroll.h"
    #include "via.h" // uses EEPROM address, lighting value IDs
    #include "mouse_jiggler.h"
    #include "ploopy_via.h"
    #include "mouse_gesture.h"

    via_ploopystuff_config ploopyvia_config = {
        .dpi_multiplier            = 20,
        .pointer_invert_h          = false,
        .pointer_invert_v          = false,
        .gesture_count             = PLOOPY_MSGESTURE_WIGGLES,
        .gesture_action_h          = GESTURE_ACTION_NOTHING,
        .gesture_action_v          = GESTURE_ACTION_NOTHING,
        .combos_enabled            = false,
        #if defined(BETTER_DRAGSCROLL_INVERT_H)
          .dragscroll_invert_h       = true,
        #else // BETTER_DRAGSCROLL_INVERT_H
          .dragscroll_invert_h       = false,
        #endif // BETTER_DRAGSCROLL_INVERT_H

        #if defined(BETTER_DRAGSCROLL_INVERT_V)
          .dragscroll_invert_v       = true,
        #else // BETTER_DRAGSCROLL_INVERT_V
          .dragscroll_invert_v       = false,
        #endif // BETTER_DRA

        .dragscroll_divisor_h      = 4 * BETTER_DRAGSCROLL_DIVISOR_H,
        .dragscroll_divisor_v      = 4 * BETTER_DRAGSCROLL_DIVISOR_V,

        #if defined(BETTER_DRAGSCROLL_CAPLK_ENABLE)
            .dragscroll_caps         = 1,
        #else // BETTER_DRAGSCROLL_CAPLK_ENABLE
            .dragscroll_caps       = 0,
        #endif // BETTER_DRAGSCROLL_CAPLK_ENABLE

        #if defined(BETTER_DRAGSCROLL_NUMLK_ENABLE)
            .dragscroll_num         = 1,
        #else // BETTER_DRAGSCROLL_NUMLK_ENABLE
            .dragscroll_num       = 0,
        #endif // BETTER_DRAGSCROLL_NUMLK_ENABLE

        #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE)
            .dragscroll_scroll         = 1,
        #else // BETTER_DRAGSCROLL_SCRLK_ENABLE
            .dragscroll_scroll       = 0,
        #endif // BETTER_DRAGSCROLL_SCRLK_ENABLE

        #if defined(BETTER_DRAGSCROLL_END_ON_KEYPRESS)
            .dragscroll_end_on_keypress = true,
        #else // BETTER_DRAGSCROLL_END_ON_KEYPRESS
            .dragscroll_end_on_keypress = false,
        #endif // BETTER_DRAGSCROLL_END_ON_KEYPRESS
    };

    void values_load(void)
    {
        eeprom_read_block( &ploopyvia_config, ((void*)VIA_EEPROM_CUSTOM_CONFIG_ADDR), sizeof(ploopyvia_config) );
    }

    void values_save(void)
    {
        eeprom_update_block( &ploopyvia_config, ((void*)VIA_EEPROM_CUSTOM_CONFIG_ADDR), sizeof(ploopyvia_config) );
    }

    void update_dpi(void) {
        pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
        dprintf("Set CPI %d\n", dpi_array[keyboard_config.dpi_config]);
    }

    void via_init_kb(void)
    {
        // If the EEPROM has the magic, the data is good.
        // OK to load from EEPROM
        if (via_eeprom_is_valid()) {
            values_load();
        } else	{
            values_save();
            // DO NOT set EEPROM valid here, let caller do this
        }
        update_dpi();
        ploopy_msGestureUpdate();
        led_update_better_dragscroll(host_keyboard_led_state());
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
                    values_save();
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

        switch ( *value_id )
        {
            case id_ploopystuff_dpi_preset:
                keyboard_config.dpi_config = *value_data;
                dprintf("dpi_preset: %d\n", keyboard_config.dpi_config);
                update_dpi();
                break;

            case id_ploopystuff_dpi_multiplier:
                for (int i = 0; i <  5; ++i) {
                    dpi_array[i] = dpi_array[i] / ploopyvia_config.dpi_multiplier * *value_data;
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

            case id_ploopystuff_dpi_presets:
                dpi_array[value_data[0]] = (value_data[1]*10) * (ploopyvia_config.dpi_multiplier/20) ;
                dprintf("dpi_presets[%d]: %d\n", value_data[0], value_data[1]);
                update_dpi();
                break;

        }
    }

    void ploopyvia_config_get_value( uint8_t *data )
    {
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);

        switch ( *value_id )
        {
            case id_ploopystuff_dpi_presets:
                value_data[1] = (dpi_array[value_data[0]] / 10) / (ploopyvia_config.dpi_multiplier/20)  ;
                break;

            case id_ploopystuff_dpi_preset:
                *value_data = keyboard_config.dpi_config;
                break;
            case id_ploopystuff_dpi_multiplier:
                *value_data = ploopyvia_config.dpi_multiplier;
                break;

            case id_ploopystuff_msjiggler_enabled:
                *value_data = msJigMainToken != INVALID_DEFERRED_TOKEN;
                break;

            case id_ploopystuff_pointer_invert_h:
                *value_data = ploopyvia_config.pointer_invert_h;
                break;

            case id_ploopystuff_pointer_invert_v:
                *value_data = ploopyvia_config.pointer_invert_v;
                break;

            case id_ploopystuff_gesture_count:
                *value_data = ploopyvia_config.gesture_count;
                break;

            case id_ploopystuff_gesture_action_h:
                *value_data = ploopyvia_config.gesture_action_h;
                break;

            case id_ploopystuff_gesture_action_v:
                *value_data = ploopyvia_config.gesture_action_v;
                break;

            case id_ploopystuff_combos_enabled:
                *value_data = ploopyvia_config.combos_enabled;
                break;

            case id_ploopystuff_dragscroll_invert_h:
                *value_data = ploopyvia_config.dragscroll_invert_h;
                break;

            case id_ploopystuff_dragscroll_invert_v:
                *value_data = ploopyvia_config.dragscroll_invert_v;
                break;

            case id_ploopystuff_dragscroll_divisor_h:
                *value_data = ploopyvia_config.dragscroll_divisor_h;
                break;

            case id_ploopystuff_dragscroll_divisor_v:
                *value_data = ploopyvia_config.dragscroll_divisor_v;
                break;

            case id_ploopystuff_dragscroll_caps:
                *value_data = ploopyvia_config.dragscroll_caps;
                break;

            case id_ploopystuff_dragscroll_num:
                *value_data = ploopyvia_config.dragscroll_num;
                break;

            case id_ploopystuff_dragscroll_scroll:
                *value_data = ploopyvia_config.dragscroll_scroll;
                break;

            case id_ploopystuff_dragscroll_end_on_keypress:
                *value_data = ploopyvia_config.dragscroll_end_on_keypress;
                break;
        }
    }

#endif // if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
