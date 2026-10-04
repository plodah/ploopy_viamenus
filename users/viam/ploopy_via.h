#pragma once
#include QMK_KEYBOARD_H
#if defined(BETTER_DRAGSCROLL)
    #include "better_dragscroll.h"
    #if !defined(BETTER_DRAGSCROLL_ENABLE_LAYER_A)
        #define BETTER_DRAGSCROLL_ENABLE_LAYER_A 15
    #endif // BETTER_DRAGSCROLL_ENABLE_LAYER_A
    #if !defined(BETTER_DRAGSCROLL_ENABLE_LAYER_B)
        #define BETTER_DRAGSCROLL_ENABLE_LAYER_B 15
    #endif // BETTER_DRAGSCROLL_ENABLE_LAYER_B
#endif // defined(BETTER_DRAGSCROLL)

#ifndef PLOOPY_DPI_OPTIONS
    #define PLOOPY_DPI_OPTIONS { 600, 900, 1200, 1600, 2400 }
#endif

#include "mouse_gesture.h"
#if defined(COMMUNITY_MODULE_BASIC_POINTING_ACCELERATION_ENABLE)
    #include "basic_pointing_acceleration.h"
    #if !defined(POINTING_DEVICE_ACCEL_ENABLE_DEF)
        #define POINTING_DEVICE_ACCEL_ENABLE_DEF 0
    #endif // POINTING_DEVICE_ACCEL_ENABLE_DEF
#endif // COMMUNITY_MODULE_BASIC_POINTING_ACCELERATION_ENABLE
#if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE) && defined(TASK_SWITCH_MENUS_ENABLE)
    #include "task_switch.h"
#endif // COMMUNITY_MODULE_TASK_SWITCH_ENABLE && defined(TASK_SWITCH_MENUS_ENABLE)

void keyboard_post_init_user_viamenus(void);
void ploopyvia_config_set_value( uint8_t *data );
void ploopyvia_config_get_value( uint8_t *data );
void ploopyvia_config_save ( void );
void values_load(void);
void values_save(void);

enum via_ploopystuff_value {
    id_ploopystuff_dpi_activepreset = 1,
    id_ploopystuff_msjiggler_enabled = 3,
    id_ploopystuff_pointer_invert_h = 4,
    id_ploopystuff_pointer_invert_v = 5,
    id_ploopystuff_pointer_rotation_value = 6,
    id_ploopystuff_pointer_rotation_is_ccw = 7,
    id_ploopystuff_gesture_count = 11,
    id_ploopystuff_gesture_action_h,
    id_ploopystuff_gesture_action_v,
    id_ploopystuff_combos_enabled = 14,
    id_ploopystuff_dragscroll_enable_layer_a = 19,
    id_ploopystuff_dragscroll_enable_layer_b,
    id_ploopystuff_dragscroll_invert_h = 21,
    id_ploopystuff_dragscroll_invert_v,
    id_ploopystuff_dragscroll_divisor_h,
    id_ploopystuff_dragscroll_divisor_v,
    id_ploopystuff_dragscroll_enable_caps,
    id_ploopystuff_dragscroll_enable_num,
    id_ploopystuff_dragscroll_enable_scroll,
    id_ploopystuff_dragscroll_enable_end_on_keypress,
    id_ploopystuff_dragscroll_enable_permanently,
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
    id_ploopystuff_dragscroll_dragact_b_right, // 68
    id_ploopystuff_turbo_fire_keycode_count = 70,
    id_ploopystuff_turbo_fire_rate,
    id_ploopystuff_turbo_fire_duration,
    id_ploopystuff_turbo_fire_keycode_a,
    id_ploopystuff_turbo_fire_keycode_b,
    id_ploopystuff_turbo_fire_keycode_c,
    id_ploopystuff_turbo_fire_keycode_d,
    id_ploopystuff_turbo_fire_keycode_e,
    id_ploopystuff_turbo_fire_keycode_f,
    id_ploopystuff_turbo_fire_keycode_g,
    id_ploopystuff_turbo_fire_keycode_h,
    id_ploopystuff_turbo_fire_rate_range, // 81
    id_ploopystuff_task_switch_mod = 85,
    id_ploopystuff_task_switch_rev_mod,
    id_ploopystuff_task_switch_tap_key,
    id_ploopystuff_task_switch_delay, // 88
    id_ploopystuff_task_switch_rev_tap_key,
    id_ploopystuff_task_switch_mod_b,
    id_ploopystuff_task_switch_rev_mod_b,
    id_ploopystuff_task_switch_tap_key_b,
    id_ploopystuff_task_switch_rev_tap_key_b, // 93
    id_ploopystuff_task_switch_os_detection,
    id_ploopystuff_task_switch_option_mac,
    id_ploopystuff_task_switch_option_win,
    id_ploopystuff_task_switch_option_lin,
    id_ploopystuff_task_switch_option_unk,
    id_ploopystuff_task_switch_option_man, // 99!
    id_pointing_accel_takeoff = 101,
    id_pointing_accel_growth_rate,
    id_pointing_accel_offset,
    id_pointing_accel_limit,
    id_pointing_accel_enabled, // 105
    id_ploopystuff_dummy_menuitem = 230,
    id_ploopystuff_dpi_as_slider,
    id_ploopystuff_config_size,
    id_ploopystuff_feature_combos=235,
    id_ploopystuff_feature_gestures,
    id_ploopystuff_feature_dragscroll,
    id_ploopystuff_feature_sniper,
    id_ploopystuff_feature_morse_code,
    id_ploopystuff_feature_dragscroll_straighten, // 240,
    id_ploopystuff_feature_mouse_jiggler,
    id_ploopystuff_feature_sensor_rotation,
    id_ploopystuff_feature_task_switch,
    id_ploopystuff_feature_turbo_fire,
    id_ploopystuff_feature_pointing_device_accel, // 245
    id_ploopystuff_feature_task_switch_menus,
    id_ploopystuff_feature_os_detection,
    id_ploopystuff_sensor_type = 250,
    id_ploopystuff_mcu_type,
    id_ploopystuff_detected_os,
};

enum mcu_types {
    MCU_UNKNOWN = 0,
    MCU_ATMEGA32U4,
    MCU_RP2040,
    MCU_STM32L432,
};

enum sensor_types {
    SENSOR_UNKNOWN = 0,
    SENSOR_PMW3360,
    SENSOR_ADNS5050,
    SENSOR_PAW3222,
};

enum feature_state {
    FEATURE_UNKNOWN = 0,
    FEATURE_AVAILABLE,
    FEATURE_UNAVAILABLE,
    FEATURE_UNSUPPORTED,
};

enum lock_state {
    LOCKSTATE_NONE = 0,
    LOCKSTATE_WHILE_ENABLED,
    LOCKSTATE_WHILE_DISABLED,
};

#ifndef BETTER_DRAGSCROLL_CAPLK_ENABLE
#define BETTER_DRAGSCROLL_CAPLK_ENABLE LOCKSTATE_NONE
#endif
#ifndef BETTER_DRAGSCROLL_NUMLK_ENABLE
#define BETTER_DRAGSCROLL_NUMLK_ENABLE LOCKSTATE_NONE
#endif
#ifndef BETTER_DRAGSCROLL_SCRLK_ENABLE
#define BETTER_DRAGSCROLL_SCRLK_ENABLE LOCKSTATE_NONE
#endif

typedef struct {
    // misc // 13 bytes
    bool     dpi_as_slider;
    uint16_t dpi_presets[5]; // 10 bytes!
    bool     pointer_invert_h;
    bool     pointer_invert_v;
    // sniper // 4 bytes
    uint16_t sniper_a_dpi:16;
    uint16_t sniper_b_dpi:16;

    // Dragscroll basics // 4 bytes
    bool     dragscroll_invert_h;
    bool     dragscroll_invert_v;
    uint8_t  dragscroll_divisor_h:8; // Value stored *4 to allow fraction in uint8
    uint8_t  dragscroll_divisor_v:8; // Value stored *4 to allow fraction in uint8
    // Dragscroll enablement // 7 bytes
    uint8_t  dragscroll_enable_caps:2;
    uint8_t  dragscroll_enable_num:2;
    uint8_t  dragscroll_enable_scroll:2;
    bool     dragscroll_enable_end_on_keypress;
    uint8_t  dragscroll_enable_layer_a:4;
    uint8_t  dragscroll_enable_layer_b:4;
    bool     dragscroll_enable_permanently;
    // Dragscroll DragAct // 16 bytes
    uint16_t dragscroll_dragact_a_up:16;
    uint16_t dragscroll_dragact_a_down:16;
    uint16_t dragscroll_dragact_a_left:16;
    uint16_t dragscroll_dragact_a_right:16;
    uint16_t dragscroll_dragact_b_up:16;
    uint16_t dragscroll_dragact_b_down:16;
    uint16_t dragscroll_dragact_b_left:16;
    uint16_t dragscroll_dragact_b_right:16;
    #ifdef COMBO_ENABLE // 1 byte
        bool     combos_enabled;
    #endif // COMBO_ENABLE
    #ifdef PLOOPY_MSGESTURE_ENABLE // 3 bytes
        uint8_t  gesture_count:4;
        uint8_t  gesture_action_h:4;
        uint8_t  gesture_action_v:4;
    #endif // PLOOPY_MSGESTURE_ENABLE
    #ifdef COMMUNITY_MODULE_TURBO_FIRE_ENABLE // 1 byte
        // Decides scale of rate slider in Via. Not part of module.
        uint8_t turbo_fire_rate_range:8;
    #endif // COMMUNITY_MODULE_TURBO_FIRE_ENABLE
} PACKED via_ploopystuff_config;
via_ploopystuff_config ploopyvia_config;

static const via_ploopystuff_config ploopyvia_config_default = {
    .dpi_as_slider              = false,
    .dpi_presets                = PLOOPY_DPI_OPTIONS,
    .pointer_invert_h           = false,
    .pointer_invert_v           = false,
    .sniper_a_dpi               = 100,
    .sniper_b_dpi               = 200,
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

    #if 4 * BETTER_DRAGSCROLL_DIVISOR_H > 255
        .dragscroll_divisor_h       = 255,
    #else // BETTER_DRAGSCROLL_DIVISOR_H
        .dragscroll_divisor_h       = 4 * BETTER_DRAGSCROLL_DIVISOR_H,
    #endif // BETTER_DRAGSCROLL_DIVISOR_H

    #if 4 * BETTER_DRAGSCROLL_DIVISOR_V > 255
        .dragscroll_divisor_v       = 255,
    #else // BETTER_DRAGSCROLL_DIVISOR_V
        .dragscroll_divisor_v       = 4 * BETTER_DRAGSCROLL_DIVISOR_V,
    #endif // BETTER_DRAGSCROLL_DIVISOR_V

    .dragscroll_enable_caps        = BETTER_DRAGSCROLL_CAPLK_ENABLE,
    .dragscroll_enable_num         = BETTER_DRAGSCROLL_NUMLK_ENABLE,
    .dragscroll_enable_scroll      = BETTER_DRAGSCROLL_SCRLK_ENABLE,

    #if defined(BETTER_DRAGSCROLL_END_ON_KEYPRESS)
        .dragscroll_enable_end_on_keypress = true,
    #else // BETTER_DRAGSCROLL_END_ON_KEYPRESS
        .dragscroll_enable_end_on_keypress = false,
    #endif // BETTER_DRAGSCROLL_END_ON_KEYPRESS
    .dragscroll_enable_layer_a         = BETTER_DRAGSCROLL_ENABLE_LAYER_A,
    .dragscroll_enable_layer_b         = BETTER_DRAGSCROLL_ENABLE_LAYER_B,
    .dragscroll_enable_permanently     = false,
    .dragscroll_dragact_a_up    = KC_VOLU,
    .dragscroll_dragact_a_down  = KC_VOLD,
    .dragscroll_dragact_a_left  = KC_NO,
    .dragscroll_dragact_a_right = KC_NO,
    .dragscroll_dragact_b_up    = KC_NO,
    .dragscroll_dragact_b_down  = KC_NO,
    .dragscroll_dragact_b_left  = KC_NO,
    .dragscroll_dragact_b_right = KC_NO,

    #if defined(COMBO_ENABLE)
    .combos_enabled             = false,
    #endif // COMBO_ENABLE

    #ifdef PLOOPY_MSGESTURE_ENABLE
        .gesture_count              = PLOOPY_MSGESTURE_WIGGLES,
        .gesture_action_h           = GESTURE_ACTION_NOTHING,
        .gesture_action_v           = GESTURE_ACTION_NOTHING,
    #endif // PLOOPY_MSGESTURE_ENABLE

    #ifdef COMMUNITY_MODULE_TURBO_FIRE_ENABLE
        .turbo_fire_rate_range = 1,
    #endif // COMMUNITY_MODULE_TURBO_FIRE_ENABLE
};

#if defined(PLOOPYVIA_DEBUG) && defined(CONSOLE_ENABLE)
#pragma message "PLOOPYVIA_DEBUG"
#    include <debug.h>
#    include <print.h>
#    define pv_dprintf(...)            \
        do {                           \
            dprintf("%s: ", __func__); \
            dprintf(__VA_ARGS__);      \
        } while (0)
#else
#    define pv_dprintf(...) \
        do {                \
        } while (0)
#endif

#if defined(PLOOPYVIA_DEBUG_VERBOSE) && defined(CONSOLE_ENABLE)
#pragma message "PLOOPYVIA_DEBUG_VERBOSE"
#    include <debug.h>
#    include <print.h>
#    define pvv_dprintf(...)            \
        do {                           \
            dprintf("%s: ", __func__); \
            dprintf(__VA_ARGS__);      \
        } while (0)
#else
#    define pvv_dprintf(...) \
        do {                \
        } while (0)
#endif
