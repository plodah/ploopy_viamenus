#if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
    #include QMK_KEYBOARD_H
    #define VIA_DPI_STORE_RATIO 100
    #include "ploopy_via.h"

    #define COMBINE_UINT8(a, b) ((a << 8) | b)
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

    #if defined (COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
    void update_turbo_fire_kc(uint8_t index){
        set_turbo_fire_keycode(index, ploopyvia_config.turbo_fire_keycodes[index]);
    }

    void update_turbo_fire_all(void){
        for (int i = 0; i < get_turbo_fire_keycount(); i++){
            update_turbo_fire_kc(i);
        }
        set_turbo_fire_rate(ploopyvia_config.turbo_fire_rate);
        set_turbo_fire_duration(ploopyvia_config.turbo_fire_duration);
    }
    #endif // defined (COMMUNITY_MODULE_TURBO_FIRE_ENABLE)

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

    #if defined (COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)
        void update_pointer_accelaration (void){
            g_pointing_device_accel_config.enabled     = ploopyvia_config.pointer_accel_enabled;
            g_pointing_device_accel_config.growth_rate = ploopyvia_config.pointer_accel_growth_rate;
            g_pointing_device_accel_config.offset      = ploopyvia_config.pointer_accel_offset;
            g_pointing_device_accel_config.limit       = ploopyvia_config.pointer_accel_limit;
            g_pointing_device_accel_config.takeoff     = ploopyvia_config.pointer_accel_takeoff;
            dprintf("MACCEL: ena:%d tak:%.3f grw:%.3f off:%.3f lim: %.3f\n",
                g_pointing_device_accel_config.enabled,
                g_pointing_device_accel_config.takeoff,
                g_pointing_device_accel_config.growth_rate,
                g_pointing_device_accel_config.offset,
                g_pointing_device_accel_config.limit
            );
        }
    #endif // defined (COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)

    #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
    void update_task_switch( void ){
        taskswitch_set_mod(ploopyvia_config.task_switch_mod);
        taskswitch_set_rev_mod(ploopyvia_config.task_switch_rev_mod);
        taskswitch_set_tap(ploopyvia_config.task_switch_tap_key);
        taskswitch_set_delay(ploopyvia_config.task_switch_delay);
    }
    #endif // COMMUNITY_MODULE_TASK_SWITCH_ENABLE && defined(TASK_SWITCH_MENUS_ENABLE)

    void keyboard_post_init_user_viamenus(void) {
        ploopyvia_config_load();
        // Did this do anything important? Maybe find another way to check for invalid 0 value
        /*
        if(ploopyvia_config.dpi_multiplier == 0){
            eeconfig_init_user();
        }
        */
        update_dpi();
        #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
            pmw_rotation_update_via();
        #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
        #if defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
            drgstraight_set_sensitivity( ploopyvia_config.dragscroll_straighten_sensitivity );
        #endif // defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
        #if defined (COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)
            update_pointer_accelaration();
        #endif // COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE
        #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
            update_task_switch();
        #endif // defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
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

    void ploopyvia_config_set_value( uint8_t *data ) {
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);

        switch ( *value_id ) {
            case id_ploopystuff_dpi_activepreset:
                keyboard_config.dpi_config = *value_data;
                dprintf("dpi_preset: %d\n", keyboard_config.dpi_config);
                update_dpi();
                break;

            #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
                case id_ploopystuff_msjiggler_enabled:
                    if( jiggler_get_state() ^ (*value_data) ) {
                        // ^ is an XOR, buddy
                        jiggler_toggle();
                        dprintf("msjiggler_enabled\n");
                    }
                    break;
            #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE

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
            #if defined(PLOOPY_MSGESTURE_ENABLE)
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
            #endif // defined(PLOOPY_MSGESTURE_ENABLE)
            #if defined(COMBO_ENABLE)
            case id_ploopystuff_combos_enabled:
                ploopyvia_config.combos_enabled = *value_data;
                dprintf("combos_enabled:%d\n", ploopyvia_config.combos_enabled);
                break;
            #endif // defined(PLOOPY_MSGESTURE_ENABLE))

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

            case id_ploopystuff_dragscroll_enable_caps:
                ploopyvia_config.dragscroll_enable_caps = *value_data;
                dprintf("dragscroll_enable_caps: %d\n", ploopyvia_config.dragscroll_enable_caps);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_enable_num:
                ploopyvia_config.dragscroll_enable_num = *value_data;
                dprintf("dragscroll_enable_num: %d\n", ploopyvia_config.dragscroll_enable_num);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_enable_scroll:
                ploopyvia_config.dragscroll_enable_scroll = *value_data;
                dprintf("dragscroll_enable_scroll: %d\n", ploopyvia_config.dragscroll_enable_scroll);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_enable_end_on_keypress:
                ploopyvia_config.dragscroll_enable_end_on_keypress = *value_data;
                dprintf("dragscroll_enable_end_on_keypress: %d\n", ploopyvia_config.dragscroll_enable_end_on_keypress);
                break;

            case id_ploopystuff_dragscroll_enable_permanently:
                ploopyvia_config.dragscroll_enable_permanently = *value_data;
                dprintf("dragscroll_enable_permanently: %d\n", ploopyvia_config.dragscroll_enable_permanently);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_enable_layer_a:
                ploopyvia_config.dragscroll_enable_layer_a = *value_data;
                dprintf("dragscroll_enable_layer_a: %d\n", ploopyvia_config.dragscroll_enable_layer_a);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dragscroll_enable_layer_b:
                ploopyvia_config.dragscroll_enable_layer_b = *value_data;
                dprintf("dragscroll_enable_layer_b: %d\n", ploopyvia_config.dragscroll_enable_layer_b);
                led_update_better_dragscroll(host_keyboard_led_state());
                break;

            case id_ploopystuff_dpi_presets:
                ploopyvia_config.dpi_presets[value_data[0]] = (value_data[1] * VIA_DPI_STORE_RATIO );
                dprintf("dpi_presets[%d]: %d\n", value_data[0], value_data[1]);
                update_dpi();
                break;

            case id_ploopystuff_sniper_a_dpi:
                ploopyvia_config.sniper_a_dpi = ((uint8_t)*value_data * VIA_DPI_STORE_RATIO );
                dprintf("sniper_a_dpi: %d\n", ploopyvia_config.sniper_a_dpi);
                break;

            case id_ploopystuff_sniper_b_dpi:
                ploopyvia_config.sniper_b_dpi = ((uint8_t)*value_data * VIA_DPI_STORE_RATIO);
                dprintf("sniper_b_dpi: %d\n", ploopyvia_config.sniper_b_dpi);
                break;

            #if defined(COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
                case id_ploopystuff_dragscroll_straighten_sensitivity:
                    ploopyvia_config.dragscroll_straighten_sensitivity = *value_data;
                    drgstraight_set_sensitivity( ploopyvia_config.dragscroll_straighten_sensitivity );
                    dprintf("dragscroll_straighten_sensitivity: %d\n", ploopyvia_config.dragscroll_straighten_sensitivity);
                    break;
            #endif // defined(COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)

            case id_ploopystuff_dragscroll_dragact_a_up:
                ploopyvia_config.dragscroll_dragact_a_up = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_a_up: %d\n", ploopyvia_config.dragscroll_dragact_a_up);
                break;

            case id_ploopystuff_dragscroll_dragact_a_down:
                ploopyvia_config.dragscroll_dragact_a_down = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_a_down: %d\n", ploopyvia_config.dragscroll_dragact_a_down);
                break;

            case id_ploopystuff_dragscroll_dragact_a_left:
                ploopyvia_config.dragscroll_dragact_a_left = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_a_left: %d\n", ploopyvia_config.dragscroll_dragact_a_left);
                break;

            case id_ploopystuff_dragscroll_dragact_a_right:
                ploopyvia_config.dragscroll_dragact_a_right = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_a_right: %d\n", ploopyvia_config.dragscroll_dragact_a_right);
                break;

            case id_ploopystuff_dragscroll_dragact_b_up:
                ploopyvia_config.dragscroll_dragact_b_up = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_b_up: %d\n", ploopyvia_config.dragscroll_dragact_b_up);
                break;

            case id_ploopystuff_dragscroll_dragact_b_down:
                ploopyvia_config.dragscroll_dragact_b_down = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_b_down: %d\n", ploopyvia_config.dragscroll_dragact_b_down);
                break;

            case id_ploopystuff_dragscroll_dragact_b_left:
                ploopyvia_config.dragscroll_dragact_b_left = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_b_left: %d\n", ploopyvia_config.dragscroll_dragact_b_left);
                break;

            case id_ploopystuff_dragscroll_dragact_b_right:
                ploopyvia_config.dragscroll_dragact_b_right = COMBINE_UINT8(value_data[0], value_data[1]);
                dprintf("dragscroll_dragact_b_right: %d\n", ploopyvia_config.dragscroll_dragact_b_right);
                break;

            #if defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
                case id_ploopystuff_turbo_fire_rate:
                    if(ploopyvia_config.turbo_fire_rate_range >= 100 || value_data[1]>0){
                        ploopyvia_config.turbo_fire_rate = COMBINE_UINT8(value_data[0], value_data[1]);
                    }
                    else{
                        ploopyvia_config.turbo_fire_rate = value_data[0];
                    }
                    set_turbo_fire_rate(ploopyvia_config.turbo_fire_rate);
                    dprintf("turbo_fire_rate: %d\n", ploopyvia_config.turbo_fire_rate);
                    break;

                case id_ploopystuff_turbo_fire_rate_range:
                    ploopyvia_config.turbo_fire_rate_range = *value_data;
                    dprintf("turbo_fire_rate_range: %d\n", ploopyvia_config.turbo_fire_rate_range);
                    break;

                case id_ploopystuff_turbo_fire_duration:
                    ploopyvia_config.turbo_fire_duration = *value_data;
                    set_turbo_fire_duration(ploopyvia_config.turbo_fire_duration);
                    dprintf("turbo_fire_duration: %d\n", ploopyvia_config.turbo_fire_duration);
                    break;

                case id_ploopystuff_turbo_fire_keycode_a ... (id_ploopystuff_turbo_fire_keycode_a + TURBO_FIRE_KEYCOUNT - 1):
                    uint8_t temp_kcindex = *value_id - id_ploopystuff_turbo_fire_keycode_a;
                    ploopyvia_config.turbo_fire_keycodes[temp_kcindex] = COMBINE_UINT8(value_data[0], value_data[1]);
                    update_turbo_fire_kc(temp_kcindex);
                    dprintf("turbo_fire_keycodes[%d]: %d\n", temp_kcindex, ploopyvia_config.turbo_fire_keycodes[temp_kcindex]);
                    break;

            #endif // defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)

            case id_ploopystuff_dpi_as_slider:
                ploopyvia_config.dpi_as_slider = *value_data;
                dprintf("dpi_as_slider: %d\n", ploopyvia_config.dpi_as_slider);
                break;

            #if defined(COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)
            case id_pointing_device_takeoff:
                ploopyvia_config.pointer_accel_takeoff =
                    (float) COMBINE_UINT8(value_data[0], value_data[1]) / 100;
                update_pointer_accelaration();
                break;

            case id_pointing_device_growth_rate:
                ploopyvia_config.pointer_accel_growth_rate =
                    (float) COMBINE_UINT8(value_data[0], value_data[1]) / 100;
                update_pointer_accelaration();
                break;

            case id_pointing_device_offset:
                ploopyvia_config.pointer_accel_offset =
                    (float) COMBINE_UINT8(value_data[0], value_data[1]) / 100;
                update_pointer_accelaration();
                break;

            case id_pointing_device_limit:
                ploopyvia_config.pointer_accel_limit = (float) value_data[0] / 100;
                update_pointer_accelaration();
                break;

            case id_pointing_device_enabled:
                ploopyvia_config.pointer_accel_enabled = value_data[0];
                update_pointer_accelaration();
                break;
            #endif // defined(COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)

            #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
            case id_ploopystuff_task_switch_delay:
                ploopyvia_config.task_switch_delay = COMBINE_UINT8(value_data[0], value_data[1]);
                update_task_switch();
                break;
            case id_ploopystuff_task_switch_tap_key:
                ploopyvia_config.task_switch_tap_key = COMBINE_UINT8(value_data[0], value_data[1]);
                update_task_switch();
                break;
            case id_ploopystuff_task_switch_mod:
                ploopyvia_config.task_switch_mod = *value_data;
                update_task_switch();
                break;
            case id_ploopystuff_task_switch_rev_mod:
                ploopyvia_config.task_switch_rev_mod = *value_data;
                update_task_switch();
                break;
            #endif // defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE)  && defined(TASK_SWITCH_MENUS_ENABLE)
        }
    }

    void ploopyvia_config_get_value( uint8_t *data ) {
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);

        switch ( *value_id ) {
            case id_ploopystuff_dpi_activepreset:
                *value_data = keyboard_config.dpi_config;
                dprintf("dpi_preset: %d\n", keyboard_config.dpi_config);
                break;

            #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
            case id_ploopystuff_msjiggler_enabled:
                if(jiggler_get_state()){
                    dprintf("msjiggler_enabled true \n");
                    *value_data = true;
                }
                else{
                    dprintf("msjiggler_enabled false \n");
                    *value_data = false;
                }
                break;
            #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE

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

            #if defined(PLOOPY_MSGESTURE_ENABLE)
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
            #endif // defined(PLOOPY_MSGESTURE_ENABLE)

            #if defined(COMBO_ENABLE)
            case id_ploopystuff_combos_enabled:
                *value_data = ploopyvia_config.combos_enabled;
                dprintf("combos_enabled:%d\n", ploopyvia_config.combos_enabled);
                break;
            #endif // defined(PLOOPY_MSGESTURE_ENABLE))

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

            case id_ploopystuff_dragscroll_enable_caps:
                *value_data = ploopyvia_config.dragscroll_enable_caps;
                dprintf("dragscroll_enable_caps: %d\n", ploopyvia_config.dragscroll_enable_caps);
                break;

            case id_ploopystuff_dragscroll_enable_num:
                *value_data = ploopyvia_config.dragscroll_enable_num;
                dprintf("dragscroll_enable_num: %d\n", ploopyvia_config.dragscroll_enable_num);
                break;

            case id_ploopystuff_dragscroll_enable_scroll:
                *value_data = ploopyvia_config.dragscroll_enable_scroll;
                dprintf("dragscroll_enable_scroll: %d\n", ploopyvia_config.dragscroll_enable_scroll);
                break;

            case id_ploopystuff_dragscroll_enable_end_on_keypress:
                *value_data = ploopyvia_config.dragscroll_enable_end_on_keypress;
                dprintf("dragscroll_enable_end_on_keypress: %d\n", ploopyvia_config.dragscroll_enable_end_on_keypress);
                break;

            case id_ploopystuff_dragscroll_enable_permanently:
                *value_data = ploopyvia_config.dragscroll_enable_permanently;
                dprintf("dragscroll_enable_permanently: %d\n", ploopyvia_config.dragscroll_enable_permanently);
                break;

            case id_ploopystuff_dragscroll_enable_layer_a:
                *value_data = ploopyvia_config.dragscroll_enable_layer_a;
                dprintf("dragscroll_enable_layer_a: %d\n", ploopyvia_config.dragscroll_enable_layer_a);
                break;

            case id_ploopystuff_dragscroll_enable_layer_b:
                *value_data = ploopyvia_config.dragscroll_enable_layer_b;
                dprintf("dragscroll_enable_layer_b: %d\n", ploopyvia_config.dragscroll_enable_layer_b);
                break;

            case id_ploopystuff_dpi_presets:
                value_data[1] = ploopyvia_config.dpi_presets[value_data[0]] / VIA_DPI_STORE_RATIO;
                dprintf("dpi_presets[%d]: %d\n", value_data[0], ploopyvia_config.dpi_presets[value_data[0]]);
                break;

            case id_ploopystuff_sniper_a_dpi:
                *value_data = ploopyvia_config.sniper_a_dpi / VIA_DPI_STORE_RATIO;
                dprintf("sniper_a_dpi: %d\n", ploopyvia_config.sniper_a_dpi);
                break;

            case id_ploopystuff_sniper_b_dpi:
                *value_data = ploopyvia_config.sniper_b_dpi / VIA_DPI_STORE_RATIO;
                dprintf("sniper_b_dpi: %d\n", ploopyvia_config.sniper_b_dpi);
                break;

            #if defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
                case id_ploopystuff_dragscroll_straighten_sensitivity:
                    *value_data = ploopyvia_config.dragscroll_straighten_sensitivity;
                    dprintf("dragscroll_straighten_sensitivity: %d\n", ploopyvia_config.dragscroll_straighten_sensitivity);
                    break;
            #endif // defined( COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)

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

            #if defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
                case id_ploopystuff_turbo_fire_keycode_count:
                    *value_data = get_turbo_fire_keycount();
                    dprintf("turbo_fire_duration: %d\n", get_turbo_fire_keycount());
                    break;

                case id_ploopystuff_turbo_fire_rate:


                    if( ploopyvia_config.turbo_fire_rate_range >= 100 || (ploopyvia_config.turbo_fire_rate >> 8) > 0 ){
                        value_data[0] = ploopyvia_config.turbo_fire_rate >> 8;
                        value_data[1] = ploopyvia_config.turbo_fire_rate & 0xFF;
                    }
                    else{
                        *value_data = ploopyvia_config.dragscroll_enable_layer_b;
                    }

                    dprintf("turbo_fire_rate: %d\n", ploopyvia_config.turbo_fire_rate);
                    break;

                case id_ploopystuff_turbo_fire_rate_range:
                    *value_data = ploopyvia_config.turbo_fire_rate_range;
                    dprintf("turbo_fire_rate_range: %d\n", ploopyvia_config.turbo_fire_rate_range);
                    break;

                case id_ploopystuff_turbo_fire_duration:
                    *value_data = ploopyvia_config.turbo_fire_duration;
                    dprintf("turbo_fire_duration: %d\n", ploopyvia_config.turbo_fire_duration);
                    break;

                case id_ploopystuff_turbo_fire_keycode_a...(id_ploopystuff_turbo_fire_keycode_a + TURBO_FIRE_KEYCOUNT - 1):
                    uint8_t temp_kcindex = *value_id - id_ploopystuff_turbo_fire_keycode_a;
                    value_data[0] = ploopyvia_config.turbo_fire_keycodes[temp_kcindex] >> 8;
                    value_data[1] = ploopyvia_config.turbo_fire_keycodes[temp_kcindex] & 0xFF;
                    dprintf("turbo_fire_keycodes[%d]: %d\n", temp_kcindex, ploopyvia_config.turbo_fire_keycodes[temp_kcindex]);
                    break;
            #endif // defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)

            case id_ploopystuff_dpi_as_slider:
                *value_data = ploopyvia_config.dpi_as_slider;
                dprintf("dpi_as_slider: %d\n", ploopyvia_config.dpi_as_slider);
                break;

            case id_ploopystuff_feature_combos:
                #if defined(COMBO_ENABLE)
                    dprintf("feature_combos: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_combos: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMBO_ENABLE
                break;

            case id_ploopystuff_feature_gestures:
                #if defined(PLOOPY_MSGESTURE_ENABLE)
                    dprintf("feature_gestures: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_gestures: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // PLOOPY_MSGESTURE_ENABLE
                break;

            case id_ploopystuff_feature_dragscroll:
                #if defined(BETTER_DRAGSCROLL)
                    dprintf("feature_dragscroll: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_dragscroll: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // BETTER_DRAGSCROLL
                break;

            case id_ploopystuff_feature_sniper:
                #if defined(BETTER_DRAGSCROLL)
                    dprintf("feature_sniper: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_sniper: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // BETTER_DRAGSCROLL
                break;

            case id_ploopystuff_feature_dragscroll_straighten:
                #if defined(COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE)
                    dprintf("feature_dragscroll_straighten: FEATURE_AVAILABLE\n");
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_dragscroll_straighten: FEATURE_UNAVAILABLE\n");
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE
                break;

            case id_ploopystuff_feature_mouse_jiggler:
                #if defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)
                    dprintf("feature_mouse_jiggler: FEATURE_AVAILABLE\n");
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_mouse_jiggler: FEATURE_UNAVAILABLE\n");
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE
                break;

            case id_ploopystuff_feature_sensor_rotation:
                #if defined(POINTING_DEVICE_DRIVER_PMW3360)
                    #if defined(COMMUNITY_MODULE_PMW_ROTATION_ENABLE)
                        dprintf("feature_sensor_rotation: FEATURE_AVAILABLE\n");
                        *value_data = FEATURE_AVAILABLE;
                    #else
                        dprintf("feature_sensor_rotation: FEATURE_UNAVAILABLE\n");
                        *value_data = FEATURE_UNAVAILABLE;
                    #endif
                #else
                    dprintf("feature_sensor_rotation: FEATURE_UNSUPPORTED\n");
                    *value_data = FEATURE_UNSUPPORTED;
                #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
                break;

            case id_ploopystuff_feature_task_switch:
                #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE)
                    dprintf("feature_task_switch: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_task_switch: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_TASK_SWITCH_ENABLE
                break;

            case id_ploopystuff_feature_task_switch_menus:
                #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
                    dprintf("feature_task_switch_menus: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_task_switch_menus: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_TASK_SWITCH_ENABLE && defined(TASK_SWITCH_MENUS_ENABLE)
                break;

            case id_ploopystuff_feature_turbo_fire:
                #if defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
                    dprintf("feature_sensor_rotation: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_sensor_rotation: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_TURBO_FIRE_ENABLE
                break;

            case id_ploopystuff_feature_morse_code:
                #if defined(COMMUNITY_MODULE_MORSE_CODE_ENABLE)
                    dprintf("feature_morse_code: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_morse_code: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_MORSE_CODE_ENABLE
                break;

            case id_ploopystuff_feature_pointing_device_accel:
                #if defined(COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)
                    dprintf("feature_pointing_device_accel: %d\n", FEATURE_AVAILABLE);
                    *value_data = FEATURE_AVAILABLE;
                #else
                    dprintf("feature_pointing_device_accel: %d\n", FEATURE_UNAVAILABLE);
                    *value_data = FEATURE_UNAVAILABLE;
                #endif // COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE
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

            case id_ploopystuff_mcu_type:
                #if defined(QMK_MCU_RP2040)
                    dprintf("mcu_type: MCU_RP2040\n");
                    *value_data = MCU_RP2040;
                #elif defined(QMK_MCU_ATMEGA32U4)
                    dprintf("mcu_type: MCU_ATMEGA32U4\n");
                    *value_data = MCU_ATMEGA32U4;
                #elif defined(QMK_MCU_STM32L432)
                    dprintf("mcu_type: MCU_STM32L432\n");
                    *value_data = MCU_STM32L432;
                #else
                    dprintf("mcu_type: MCU_UNKNOWN");
                    *value_data = MCU_UNKNOWN;
                #endif
                break;

            #if defined(COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)
            case id_pointing_device_takeoff:
                uint16_t takeoff = ploopyvia_config.pointer_accel_takeoff * 100;
                value_data[0] = takeoff >> 8;
                value_data[1] = takeoff & 0xFF;
                break;
            case id_pointing_device_growth_rate:
                uint16_t growth_rate = ploopyvia_config.pointer_accel_growth_rate * 100;
                value_data[0] = growth_rate >> 8;
                value_data[1] = growth_rate & 0xFF;
                break;
            case id_pointing_device_offset:
                uint16_t offset = ploopyvia_config.pointer_accel_offset * 100;
                value_data[0] = offset >> 8;
                value_data[1] = offset & 0xFF;
                break;
            case id_pointing_device_limit:
                value_data[0] = ploopyvia_config.pointer_accel_limit * 100;
                break;
            case id_pointing_device_enabled:
                value_data[0] = ploopyvia_config.pointer_accel_enabled;
                break;
            #endif // defined(COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE)

            #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
            case id_ploopystuff_task_switch_delay:
                value_data[0] = ploopyvia_config.task_switch_delay >> 8;
                value_data[1] = ploopyvia_config.task_switch_delay & 0xFF;
                break;
            case id_ploopystuff_task_switch_tap_key:
                value_data[0] = ploopyvia_config.task_switch_tap_key >> 8;
                value_data[1] = ploopyvia_config.task_switch_tap_key & 0xFF;
                break;
            case id_ploopystuff_task_switch_mod:
                *value_data = ploopyvia_config.task_switch_mod;
                break;
            case id_ploopystuff_task_switch_rev_mod:
                *value_data = ploopyvia_config.task_switch_rev_mod;
                break;
            #endif // defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
        }
    }

#endif // if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
