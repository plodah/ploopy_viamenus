/* Copyright 2025 Plodah
 * Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    PL_MSJG,
    PL_TSKN,
    PL_TSKP,
    BETTER_DRAG_SCROLL_SNIPER_A_MOMENTARY,
    BETTER_DRAG_SCROLL_SNIPER_A_TOGGLE,
    BETTER_DRAG_SCROLL_SNIPER_B_MOMENTARY,
    BETTER_DRAG_SCROLL_SNIPER_B_TOGGLE,
    BETTER_DRAG_VOLUME_MOMENTARY,
};

enum {
    TD_DRAGSCROLL,
};

#include "common.c"

#define P_DRGTD TD(TD_DRAGSCROLL)
#define P_DPI DPI_CONFIG
#define P_DRGMO BETTER_DRAG_SCROLL_MOMENTARY
#define P_DRGTG BETTER_DRAG_SCROLL_TOGGLE
#define P_SNMOA BETTER_DRAG_SCROLL_SNIPER_A_MOMENTARY
#define P_SNTGA BETTER_DRAG_SCROLL_SNIPER_A_TOGGLE
#define P_SNMOB BETTER_DRAG_SCROLL_SNIPER_B_MOMENTARY
#define P_SNTGB BETTER_DRAG_SCROLL_SNIPER_B_TOGGLE
#define P_DRGVL BETTER_DRAG_VOLUME_MOMENTARY

#define P_MS4FA LT(_FNA, KC_BTN4)
#define P_MS5FB LT(_FNB, KC_BTN5)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        P_MS4FA, P_MS5FB, KC_BTN3, KC_BTN2,
        KC_BTN1,                   TD(TD_DRAGSCROLL)
    ),
    [_FNA] = LAYOUT(
        _______, PL_TSKP, PL_TSKN, _______,
        DPI_CONFIG,                P_DRGTG
    ),
    [_FNB] = LAYOUT(
        _______, _______, _______, P_SNTGA,
        PL_MSJG,                   P_DRGTG
    )
};
