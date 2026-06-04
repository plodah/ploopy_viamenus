#if defined(VIA_ENABLE) && defined(PLOOPY_KNOB_VIAMENUS)
    #include QMK_KEYBOARD_H
    #include "ploopy_knob_via.h"

    _Static_assert(sizeof(ploopyknob_via_config) <= EECONFIG_USER_DATA_SIZE, "config storage insufficient!");
    #define COMBINE_UINT8(a, b) ((a << 8) | b)
    void ploopyknob_config_load(void) {
        eeconfig_read_user_datablock(&ploopyknob_via_config, 0, EECONFIG_USER_DATA_SIZE);
    }

    void ploopyknob_config_save(void) {
        eeconfig_update_user_datablock(&ploopyknob_via_config, 0, EECONFIG_USER_DATA_SIZE);
    }

    void keyboard_post_init_user_ploopyknob(void){
        ploopyknob_config_load();
    }

    void eeconfig_init_user(void) {
        ploopyknob_via_config = ploopyknob_via_config_default;
        ploopyknob_config_save();
        dprintf("eeconfig_init_user\n");
    }

    void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
        // data = [ command_id, channel_id, value_id, value_data ]
        uint8_t *command_id        = &(data[0]);
        uint8_t *channel_id        = &(data[1]);
        uint8_t *value_id_and_data = &(data[2]);

        if ( *channel_id == id_custom_channel ) {
            switch ( *command_id ) {
                case id_custom_set_value:
                {
                    ploopyknob_config_set_value(value_id_and_data);
                    break;
                }
                case id_custom_get_value:
                {
                    ploopyknob_config_get_value(value_id_and_data);
                    break;
                }
                case id_custom_save:
                {
                    ploopyknob_config_save();
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

    void ploopyknob_config_set_value( uint8_t *data ){
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);
        switch ( *value_id ) {
            case id_ploopyknob_act_default:
                ploopyknob_via_config.action_default = *value_data;
                break;
            case id_ploopyknob_act_numlk:
                ploopyknob_via_config.action_num_lock = *value_data;
                break;
            case id_ploopyknob_act_capslk:
                ploopyknob_via_config.action_caps_lock = *value_data;
                break;
            case id_ploopyknob_act_scrlk:
                ploopyknob_via_config.action_scroll_lock = *value_data;
                break;
            case id_ploopyknob_act_compose:
                ploopyknob_via_config.action_compose = *value_data;
                break;
            case id_ploopyknob_act_kana:
                ploopyknob_via_config.action_kana = *value_data;
                break;
            case id_ploopyknob_rev_default:
                ploopyknob_via_config.reverse_default = *value_data;
                break;
            case id_ploopyknob_rev_numlk:
                ploopyknob_via_config.reverse_num_lock = *value_data;
                break;
            case id_ploopyknob_rev_capslk:
                ploopyknob_via_config.reverse_caps_lock = *value_data;
                break;
            case id_ploopyknob_rev_scrlk:
                ploopyknob_via_config.reverse_scroll_lock = *value_data;
                break;
            case id_ploopyknob_rev_compose:
                ploopyknob_via_config.reverse_compose = *value_data;
                break;
            case id_ploopyknob_rev_kana:
                ploopyknob_via_config.reverse_kana = *value_data;
                break;
        }
    }
    void ploopyknob_config_get_value( uint8_t *data ){
        uint8_t *value_id   = &(data[0]);
        uint8_t *value_data = &(data[1]);
        switch ( *value_id ) {
            case id_ploopyknob_act_default:
                *value_data = ploopyknob_via_config.action_default;
                break;
            case id_ploopyknob_act_numlk:
                *value_data = ploopyknob_via_config.action_num_lock;
                break;
            case id_ploopyknob_act_capslk:
                *value_data = ploopyknob_via_config.action_caps_lock;
                break;
            case id_ploopyknob_act_scrlk:
                *value_data = ploopyknob_via_config.action_scroll_lock;
                break;
            case id_ploopyknob_act_compose:
                *value_data = ploopyknob_via_config.action_compose;
                break;
            case id_ploopyknob_act_kana:
                *value_data = ploopyknob_via_config.action_kana;
                break;
            case id_ploopyknob_rev_default:
                *value_data = ploopyknob_via_config.reverse_default;
                break;
            case id_ploopyknob_rev_numlk:
                *value_data = ploopyknob_via_config.reverse_num_lock;
                break;
            case id_ploopyknob_rev_capslk:
                *value_data = ploopyknob_via_config.reverse_caps_lock;
                break;
            case id_ploopyknob_rev_scrlk:
                *value_data = ploopyknob_via_config.reverse_scroll_lock;
                break;
            case id_ploopyknob_rev_compose:
                *value_data = ploopyknob_via_config.reverse_compose;
                break;
            case id_ploopyknob_rev_kana:
                *value_data = ploopyknob_via_config.reverse_kana;
                break;
        }
    }
#endif // defined(VIA_ENABLE) && defined(PLOOPY_KNOB_VIAMENUS)
