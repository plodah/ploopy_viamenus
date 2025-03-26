#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _FNA,
    _FNB,
};

enum keymap_keycodes {
    KB_DPI_CONFIG       = QK_KB_0,
    KB_DRAG_SCROLL,
    BETTER_DRAG_SCROLL_MOMENTARY,
    BETTER_DRAG_SCROLL_TOGGLE,
};

enum {
    TD_DRAGSCROLL,
};

#include "common.c"

#define P_DRGTD TD(TD_DRAGSCROLL)
#define P_DPI DPI_CONFIG
#define P_DRGMO BETTER_DRAG_SCROLL_MOMENTARY
#define P_DRGTG BETTER_DRAG_SCROLL_TOGGLE

#define P_MS4FA LT(_FNA, KC_BTN4)
#define P_MS5FB LT(_FNB, KC_BTN5)

#define P_COPY C(KC_C)
#define P_PASTE C(KC_V)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        P_COPY,  KC_BTN1, KC_BTN3, KC_BTN2, P_PASTE,
        P_MS4FA, P_MS5FB, DPI_CONFIG
    ),
    [_FNA] = LAYOUT(
        CM_MSJG, CM_TSKP, _______, CM_TSKN, _______,
        _______, _______, _______
    ),
    [_FNB] = LAYOUT(
        _______, _______, _______, _______, _______,
        _______, _______, _______
    )
};
