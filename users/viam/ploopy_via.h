#pragma once
#include "mouse_gesture.h"

void ploopyvia_config_set_value( uint8_t *data );
void ploopyvia_config_get_value( uint8_t *data );
void ploopyvia_config_save ( void );
void values_load(void);
void values_save(void);

enum via_ploopystuff_value {
    id_ploopystuff_dpi_activepreset = 1,
    id_ploopystuff_dpi_multiplier,
    id_ploopystuff_msjiggler_enabled,
    id_ploopystuff_pointer_invert_h,
    id_ploopystuff_pointer_invert_v,
    #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
    id_ploopystuff_pointer_rotation_value,
    id_ploopystuff_pointer_rotation_is_ccw,
    #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
    id_ploopystuff_gesture_count = 11,
    id_ploopystuff_gesture_action_h,
    id_ploopystuff_gesture_action_v,
    id_ploopystuff_combos_enabled,
    id_ploopystuff_dragscroll_layer_a = 19,
    id_ploopystuff_dragscroll_layer_b,
    id_ploopystuff_dragscroll_invert_h = 21,
    id_ploopystuff_dragscroll_invert_v,
    id_ploopystuff_dragscroll_divisor_h,
    id_ploopystuff_dragscroll_divisor_v,
    id_ploopystuff_dragscroll_caps,
    id_ploopystuff_dragscroll_num,
    id_ploopystuff_dragscroll_scroll,
    id_ploopystuff_dragscroll_end_on_keypress,
    id_ploopystuff_dragscroll_permanently,
    id_ploopystuff_dpi_presets = 31,
    id_ploopystuff_sniper_a_dpi = 41,
    id_ploopystuff_sniper_b_dpi,
    id_ploopystuff_dragscroll_straighten_sensitivity = 51,
    id_ploopystuff_dragscroll_dragact_a_up = 61,
    id_ploopystuff_dragscroll_dragact_a_down,
    id_ploopystuff_dragscroll_dragact_a_left,
    id_ploopystuff_dragscroll_dragact_a_right,
    id_ploopystuff_dragscroll_dragact_b_up,
    id_ploopystuff_dragscroll_dragact_b_down,
    id_ploopystuff_dragscroll_dragact_b_left,
    id_ploopystuff_dragscroll_dragact_b_right,
};

typedef struct {
    uint8_t  dpi_multiplier; // Value stored *20 to allow fraction in uint8
    bool     pointer_invert_h;
    bool     pointer_invert_v;
    uint8_t  gesture_count;
    uint8_t  gesture_action_h;
    uint8_t  gesture_action_v;
    bool     combos_enabled;
    bool     dragscroll_invert_h;
    bool     dragscroll_invert_v;
    #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
    int8_t   pointer_rotation_value;
    bool     pointer_rotation_is_ccw;
    #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
    uint8_t  dragscroll_divisor_h; // Value stored *4 to allow fraction in uint8
    uint8_t  dragscroll_divisor_v; // Value stored *4 to allow fraction in uint8
    uint8_t  dragscroll_caps;
    uint8_t  dragscroll_num;
    uint8_t  dragscroll_scroll;
    bool     dragscroll_end_on_keypress;
    bool     dragscroll_permanently;
    uint8_t  dragscroll_layer_a;
    uint8_t  dragscroll_layer_b;
    uint16_t dpi_presets[5]; // 10 bytes!
    uint16_t sniper_a_dpi;
    uint16_t sniper_b_dpi;
    uint8_t  dragscroll_straighten_sensitivity;
    uint16_t dragscroll_dragact_a_up;
    uint16_t dragscroll_dragact_a_down;
    uint16_t dragscroll_dragact_a_left;
    uint16_t dragscroll_dragact_a_right;
    uint16_t dragscroll_dragact_b_up;
    uint16_t dragscroll_dragact_b_down;
    uint16_t dragscroll_dragact_b_left;
    uint16_t dragscroll_dragact_b_right;
} via_ploopystuff_config;

via_ploopystuff_config ploopyvia_config;

static via_ploopystuff_config ploopyvia_config_default = {
    .dpi_multiplier             = 20,
    .pointer_invert_h           = false,
    .pointer_invert_v           = false,
    .gesture_count              = PLOOPY_MSGESTURE_WIGGLES,
    .gesture_action_h           = GESTURE_ACTION_NOTHING,
    .gesture_action_v           = GESTURE_ACTION_NOTHING,
    .combos_enabled             = false,
    #if defined(BETTER_DRAGSCROLL_INVERT_H)
      .dragscroll_invert_h      = true,
    #else // BETTER_DRAGSCROLL_INVERT_H
      .dragscroll_invert_h      = false,
    #endif // BETTER_DRAGSCROLL_INVERT_H

    #if defined(BETTER_DRAGSCROLL_INVERT_V)
      .dragscroll_invert_v      = true,
    #else // BETTER_DRAGSCROLL_INVERT_V
      .dragscroll_invert_v      = false,
    #endif // BETTER_DRA

    .dragscroll_divisor_h       = 4 * BETTER_DRAGSCROLL_DIVISOR_H,
    .dragscroll_divisor_v       = 4 * BETTER_DRAGSCROLL_DIVISOR_V,

    #if defined(BETTER_DRAGSCROLL_CAPLK_ENABLE)
        .dragscroll_caps        = 1,
    #else // BETTER_DRAGSCROLL_CAPLK_ENABLE
        .dragscroll_caps        = 0,
    #endif // BETTER_DRAGSCROLL_CAPLK_ENABLE

    #if defined(BETTER_DRAGSCROLL_NUMLK_ENABLE)
        .dragscroll_num         = 1,
    #else // BETTER_DRAGSCROLL_NUMLK_ENABLE
        .dragscroll_num         = 0,
    #endif // BETTER_DRAGSCROLL_NUMLK_ENABLE

    #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE)
        .dragscroll_scroll      = 1,
    #else // BETTER_DRAGSCROLL_SCRLK_ENABLE
        .dragscroll_scroll      = 0,
    #endif // BETTER_DRAGSCROLL_SCRLK_ENABLE

    #if defined(BETTER_DRAGSCROLL_END_ON_KEYPRESS)
        .dragscroll_end_on_keypress = true,
    #else // BETTER_DRAGSCROLL_END_ON_KEYPRESS
        .dragscroll_end_on_keypress = false,
    #endif // BETTER_DRAGSCROLL_END_ON_KEYPRESS

    .dragscroll_permanently     = false,
    .dragscroll_layer_a         = 255,
    .dragscroll_layer_b         = 255,
    .dpi_presets                = { 600, 900, 1200, 1600, 2400 },
    .sniper_a_dpi               = 100,
    .sniper_b_dpi               = 200,
    .dragscroll_straighten_sensitivity = 0,
    .dragscroll_dragact_a_up    = KC_VOLU,
    .dragscroll_dragact_a_down  = KC_VOLD,
    .dragscroll_dragact_a_left  = KC_NO,
    .dragscroll_dragact_a_right = KC_NO,
    .dragscroll_dragact_b_up    = KC_NO,
    .dragscroll_dragact_b_down  = KC_NO,
    .dragscroll_dragact_b_left  = KC_NO,
    .dragscroll_dragact_b_right = KC_NO,
    #ifdef COMMUNITY_MODULE_PMW_ROTATION_ENABLE
    .pointer_rotation_value     = 0,
    .pointer_rotation_is_ccw    = false,
    #endif // COMMUNITY_MODULE_PMW_ROTATION_ENABLE
};
