/* Copyright 2025 Plodah
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        LT(_FNA, MS_BTN4), MS_BTN1, MS_BTN3, MS_BTN2, LT(_FNB, MS_BTN5),
        DPI_CONFIG
    ),
    [_FNA] = LAYOUT(
        _______, _______, _______, _______, _______,
        _______
    ),
    [_FNB] = LAYOUT(
        _______, _______, _______, _______, _______,
        _______
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
    [_FNA] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_FNB] = {ENCODER_CCW_CW(_______, _______)}
};
#endif // ENCODER_MAP_ENABLE
