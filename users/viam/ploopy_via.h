#pragma once

void ploopyvia_dpi_config_set_value( uint8_t *data );
void ploopyvia_dpi_config_get_value( uint8_t *data );
void ploopyvia_dpi_config_save ( void );
void values_load(void);
void values_save(void);

enum via_ploopystuff_value {
    id_ploopystuff_dpi_preset = 1,
    id_ploopystuff_dpi_multiplier,
    id_ploopystuff_msjiggler_enabled,
    id_ploopystuff_pointer_invert_h,
    id_ploopystuff_pointer_invert_v,
    id_ploopystuff_gesture_count = 11,
    id_ploopystuff_gesture_action_h,
    id_ploopystuff_gesture_action_v,
    id_ploopystuff_combos_enabled,
    id_ploopystuff_dragscroll_invert_h = 21,
    id_ploopystuff_dragscroll_invert_v,
    id_ploopystuff_dragscroll_divisor_h,
    id_ploopystuff_dragscroll_divisor_v,
    id_ploopystuff_dragscroll_caps,
    id_ploopystuff_dragscroll_num,
    id_ploopystuff_dragscroll_scroll,
    id_ploopystuff_dragscroll_end_on_keypress,
    id_ploopystuff_dragscroll_on_host_value_toggled,
    id_ploopystuff_dpi_presets = 31,
    id_ploopystuff_dpi_preset1 = 31,
    id_ploopystuff_dpi_preset2,
    id_ploopystuff_dpi_preset3,
    id_ploopystuff_dpi_preset4,
    id_ploopystuff_dpi_preset5,
};

typedef struct {
    uint8_t dpi_multiplier; // Value stored *20 to allow fraction in uint8
    bool    pointer_invert_h;
    bool    pointer_invert_v;
    uint8_t gesture_count;
    uint8_t gesture_action_h;
    uint8_t gesture_action_v;
    bool    combos_enabled;
    bool    dragscroll_invert_h;
    bool    dragscroll_invert_v;
    uint8_t dragscroll_divisor_h; // Value stored *4 to allow fraction in uint8
    uint8_t dragscroll_divisor_v; // Value stored *4 to allow fraction in uint8
    bool    dragscroll_caps;
    bool    dragscroll_num;
    bool    dragscroll_scroll;
    bool    dragscroll_end_on_keypress;
    bool    dragscroll_on_host_value_toggled;
} via_ploopystuff_config;

via_ploopystuff_config ploopyvia_config;
