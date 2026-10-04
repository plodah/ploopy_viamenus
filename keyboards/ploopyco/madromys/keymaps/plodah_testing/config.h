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

#pragma once
#include "common.h"

#define EECONFIG_USER_DATA_SIZE 45
#define PLOOPY_MSGESTURE_ENABLE
#define BETTER_DRAGSCROLL_TAPDANCE
#define COMBO_SHOULD_TRIGGER

#define BETTER_DRAGSCROLL_INVERT_V 1

#define POINTING_DEVICE_ACCEL_ENABLE_DEF 1
#define POINTING_DEVICE_ACCEL_TAKEOFF 200
#define POINTING_DEVICE_ACCEL_GROWTH_RATE 60
#define POINTING_DEVICE_ACCEL_OFFSET 220
#define POINTING_DEVICE_ACCEL_LIMIT 18

#define BETTER_DRAGSCROLL_END_ON_KEYPRESS
#define PLOOPY_DPI_OPTIONS { 400, 500, 600, 800, 1000 }
#define PLOOPY_DPI_DEFAULT 3
#define BETTER_DRAGSCROLL_DIVISOR_H 12
#define BETTER_DRAGSCROLL_DIVISOR_V 12
#define BETTER_DRAGSCROLL_SCRLK_ENABLE LOCKSTATE_WHILE_ENABLED
#define DRAGSCROLL_STRAIGHTEN_SENSITIVITY 85

#define TASK_SWITCH_MOD MOD_LCTL | MOD_LGUI
#define TASK_SWITCH_REVERSE_MOD 0
#define TASK_SWITCH_TAP KC_RGHT
#define TASK_SWITCH_TAP_R KC_LEFT
#define TASK_SWITCH_MODE_LINUX TASK_SWITCH_OPTION_LINUX
