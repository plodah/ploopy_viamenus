/* Copyright 2025 Plodah
 * Copyright 2021 Colin Lam (Ploopy Corporation)
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

#pragma once

#include "common.h"

#define EECONFIG_USER_DATA_SIZE 55
// #define PLOOPY_MSGESTURE_ENABLE
// #define BETTER_DRAGSCROLL_TAPDANCE
// #define COMBO_SHOULD_TRIGGER
// #define TURBO_FIRE_KEYCOUNT 2

#define PLOOPY_DPI_OPTIONS { 400, 500, 600, 800, 1000 }
#define PLOOPY_DPI_DEFAULT 2 // zero indexed!

#define POINTING_DEVICE_ACCEL_ENABLE_DEF 1
#define POINTING_DEVICE_ACCEL_TAKEOFF 200
#define POINTING_DEVICE_ACCEL_GROWTH_RATE 20
#define POINTING_DEVICE_ACCEL_OFFSET 220
#define POINTING_DEVICE_ACCEL_LIMIT 25

#define TASKSWITCH_DELAY 550

#define BETTER_DRAGSCROLL_DIVISOR_H 12
#define BETTER_DRAGSCROLL_DIVISOR_V 12
#define DRAGSCROLL_STRAIGHTEN_SENSITIVITY 90

#define BETTER_DRAGSCROLL_SCRLK_ENABLE
#define BETTER_DRAGSCROLL_END_ON_KEYPRESS
#define BETTER_DRAGSCROLL_ENABLE_LAYER_A 1
#define BETTER_DRAGSCROLL_ENABLE_LAYER_B 2
#define BETTER_DRAGSCROLL_INVERT_V
