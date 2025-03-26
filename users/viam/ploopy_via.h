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
    id_ploopystuff_dragscroll_invert_h = 21,
    id_ploopystuff_dragscroll_invert_v,
    id_ploopystuff_dragscroll_caps,
    id_ploopystuff_dragscroll_num,
    id_ploopystuff_dragscroll_scroll,
    id_ploopystuff_dpi_presets = 31,
    id_ploopystuff_dpi_preset1 = 31,
    id_ploopystuff_dpi_preset2,
    id_ploopystuff_dpi_preset3,
    id_ploopystuff_dpi_preset4,
    id_ploopystuff_dpi_preset5,
};

typedef struct {
    uint8_t dpi_multiplier;
    uint8_t wiggleball_count;
    uint8_t wiggleball_action_h;
    uint8_t wiggleball_action_v;
    bool    pointer_invert_h;
    bool    pointer_invert_v;
    bool    dragscroll_invert_h;
    bool    dragscroll_invert_v;
    bool    dragscroll_caps;
    bool    dragscroll_num;
    bool    dragscroll_scroll;
} via_ploopystuff_config;

via_ploopystuff_config ploopyvia_config;
