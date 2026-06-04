/* Copyright 2023 Colin Lam (Ploopy Corporation)
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

#include <stdbool.h>
#include QMK_KEYBOARD_H
#include "as5600.h"
#include "ploopy_knob_via.h"

// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KC_NO }}};

static uint16_t     current_position = 0;
int16_t             delta;
uint8_t             knobmode;

void reset_current_pos(void){
    current_position = as5600_get_rawangle();
}

bool led_update_user(led_t led_state) {
    dprintf("N%d,%d|C%d,%d|S%d,%d|M%d,%d|K%d,%d => ",
        led_state.num_lock, ploopyknob_via_config.action_num_lock,
        led_state.caps_lock, ploopyknob_via_config.action_num_lock,
        led_state.scroll_lock, ploopyknob_via_config.action_num_lock,
        led_state.compose, ploopyknob_via_config.action_num_lock,
        led_state.kana, ploopyknob_via_config.action_num_lock );

    //dprintf("N%d|C%d|S%d|M%d|K%d\n")
    if          ( led_state.num_lock    && ploopyknob_via_config.action_num_lock ){
        knobmode = ploopyknob_via_config.action_num_lock;
    } else if   ( led_state.caps_lock   && ploopyknob_via_config.action_caps_lock){
        knobmode = ploopyknob_via_config.action_caps_lock;
    } else if   ( led_state.scroll_lock && ploopyknob_via_config.action_scroll_lock){
        knobmode = ploopyknob_via_config.action_scroll_lock;
    } else if   ( led_state.compose     && ploopyknob_via_config.action_compose){
        knobmode = ploopyknob_via_config.action_compose;
    } else if   ( led_state.kana        && ploopyknob_via_config.action_kana){
        knobmode = ploopyknob_via_config.action_kana;
    } else {
        knobmode = ploopyknob_via_config.action_default;
    }

    dprintf("Mode: %d\n", knobmode );

    reset_current_pos();
    return false;
}

void keyboard_post_init_user(void) {
    // debug_enable   = true;
    // debug_matrix   = true;
    // debug_keyboard = true;
    // debug_mouse    = true;

    as5600_init();
    reset_current_pos();
    led_update_user(host_keyboard_led_state());
    keyboard_post_init_user_ploopyknob();
    #ifdef CONSOLE_ENABLE
    debug_enable = true;
    #endif // CONSOLE_ENABLE
}

void knob_keycode_handler(uint16_t keycode_ccw, uint16_t keycode_cw, int16_t delta){
    if ( delta >= POINTING_DEVICE_AS5600_TICK_COUNT ) {
        tap_code(keycode_ccw);
        reset_current_pos();
    } else if ( delta <= -POINTING_DEVICE_AS5600_TICK_COUNT ) {
        tap_code(keycode_cw);
        reset_current_pos();
    }
}

void housekeeping_task_user(void){
    delta = (int16_t)(as5600_get_rawangle() - current_position);

    // Wrap into [-2048, 2047] to get shortest direction
    if (delta > 2048) {
        delta -= 4096;
    } else if (delta < -2048) {
        delta += 4096;
    }
    switch( knobmode ){
        case KNOB_ACT_SCROLL_V:
        case KNOB_ACT_SCROLL_H:
            // Handled by pointing_device_task_user;
            break;
        case KNOB_ACT_VOLUME:
            //dprintf("hktask d:%d, c%d\n",delta,current_position);
            knob_keycode_handler(KC_VOLD, KC_VOLU, delta);
            break;
        case KNOB_ACT_ARROWKEYS_V:
            //dprintf("hktask d:%d, c%d\n",delta,current_position);
            knob_keycode_handler(KC_UP, KC_DOWN, delta);
            break;
        case KNOB_ACT_ARROWKEYS_H:
            //dprintf("hktask d:%d, c%d\n",delta,current_position);
            knob_keycode_handler(KC_LEFT, KC_RIGHT, delta);
            break;
        case KNOB_ACT_WASD_V:
            //dprintf("hktask d:%d, c%d\n",delta,current_position);
            knob_keycode_handler(KC_W, KC_S, delta);
            break;
        case KNOB_ACT_WASD_H:
            //dprintf("hktask d:%d, c%d\n",delta,current_position);
            knob_keycode_handler(KC_A, KC_D, delta);
            break;
        case KNOB_ACT_BRIGHTNESS:
            //dprintf("hktask d:%d, c%d\n",delta,current_position);
            knob_keycode_handler(KC_BRIGHTNESS_DOWN, KC_BRIGHTNESS_UP, delta);
            break;
    }
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if( knobmode == KNOB_ACT_SCROLL_V || knobmode == KNOB_ACT_SCROLL_H ) {
        if (detected_host_os() == OS_WINDOWS || detected_host_os() == OS_LINUX) {
            //dprintf("pdtask d:%d, c:%d\n",delta,current_position);
            // Establish a deadzone to prevent spurious inputs
            if (delta > POINTING_DEVICE_AS5600_DEADZONE || delta < -POINTING_DEVICE_AS5600_DEADZONE) {
                reset_current_pos();
                if(knobmode == KNOB_ACT_SCROLL_H){
                    mouse_report.h = -delta / POINTING_DEVICE_AS5600_SPEED_DIV;
                }
                else{
                    mouse_report.v = delta / POINTING_DEVICE_AS5600_SPEED_DIV;
                }
            }
        } else {
            // Certain operating systems, like MacOS, don't play well with the
            // high-res scrolling implementation. For more details, see:
            // https://github.com/qmk/qmk_firmware/issues/17585#issuecomment-2325248167
            if (delta >= POINTING_DEVICE_AS5600_TICK_COUNT) {
                reset_current_pos();
                if(knobmode == KNOB_ACT_SCROLL_H){
                    mouse_report.h = 1;
                }
                else{
                    mouse_report.v = 1;
                }
            } else if (delta <= -POINTING_DEVICE_AS5600_TICK_COUNT) {
                reset_current_pos();
                if(knobmode == KNOB_ACT_SCROLL_H){
                    mouse_report.h = -1;
                }
                else{
                    mouse_report.v = -1;
                }
            }
        }
    }

    return mouse_report;
}

bool pointing_device_driver_init(void) {
    return true;
}
