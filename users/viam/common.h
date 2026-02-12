#pragma once

#define VIA_FIRMWARE_VERSION 2
#define PLOOPY_VIAMENUS
#define BETTER_DRAGSCROLL

#if ! defined(DYNAMIC_KEYMAP_LAYER_COUNT)
    #if defined(QMK_MCU_RP2040)
        #define DYNAMIC_KEYMAP_LAYER_COUNT 8
    #else
        #define DYNAMIC_KEYMAP_LAYER_COUNT 4
    #endif
#endif // DYNAMIC_KEYMAP_LAYER_COUNT

// Some keycode aliases to make the declarations look prettier.
// Kind of pointless since the point of this repo is to define in Via. :shrug:

#define P_DRGMO BETTER_DRAG_SCROLL_MOMENTARY
#define P_DRGTG BETTER_DRAG_SCROLL_TOGGLE
#define P_DRGTD TD(TD_DRAGSCROLL)

#define P_DPI DPI_CONFIG
#define P_SNMOA BETTER_DRAG_SCROLL_SNIPER_A_MOMENTARY
#define P_SNTGA BETTER_DRAG_SCROLL_SNIPER_A_TOGGLE
#define P_SNMOB BETTER_DRAG_SCROLL_SNIPER_B_MOMENTARY
#define P_SNTGB BETTER_DRAG_SCROLL_SNIPER_B_TOGGLE
#define P_DGACA BETTER_DRAG_ACTION_A_MOMENTARY
#define P_DGACB BETTER_DRAG_ACTION_B_MOMENTARY

#define P_MS4FA LT(_FNA, MS_BTN4)
#define P_MS5FB LT(_FNB, MS_BTN5)

#define P_COPY C(KC_C)
#define P_PAST C(KC_V)

