/* Copyright 2025 Plodah
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
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
/*
    //--  ALWAYS ON  --//
    // Dragscroll basics // 4 bytes
    // Dragscroll enablement // 7 bytes
    // Dragscroll DragAct // 16 bytes
    // TOTAL: 27 Bytes

    // COMBO_ENABLE // 1 Byte
    // PLOOPY_MSGESTURE_ENABLE // 3 Byte

    // COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE // 1 Byte
    // COMMUNITY_MODULE_DRAGSCROLL_STRAIGHTEN_ENABLE // 1 Byte

    // COMMUNITY_MODULE_BASIC_POINTING_ACCELERATION_ENABLE
    //  --or-- COMMUNITY_MODULE_POINTING_DEVICE_ACCEL_ENABLE // 17 Bytes
    // COMMUNITY_MODULE_PMW_ROTATION_ENABLE // 2 Bytes
    // COMMUNITY_MODULE_TASK_SWITCH_ENABLE && TASK_SWITCH_MENUS_ENABLE // 6 Bytes
    // COMMUNITY_MODULE_TURBO_FIRE_ENABLE // 6 Bytes
*/

#define EECONFIG_USER_DATA_SIZE 64
#define PLOOPY_MSGESTURE_ENABLE
#define BETTER_DRAGSCROLL_TAPDANCE
#define COMBO_SHOULD_TRIGGER

#define PLOOPY_DPI_OPTIONS { 400, 500, 600, 800, 1000 }
#define PLOOPY_DPI_DEFAULT 2 // zero indexed!

#define POINTING_DEVICE_ACCEL_ENABLE_DEF 1
#define POINTING_DEVICE_ACCEL_TAKEOFF 2.0
#define POINTING_DEVICE_ACCEL_GROWTH_RATE 0.5
#define POINTING_DEVICE_ACCEL_OFFSET 2.20
#define POINTING_DEVICE_ACCEL_LIMIT 0.25

#define TASKSWITCH_DELAY 550

#define BETTER_DRAGSCROLL_DIVISOR_H 12
#define BETTER_DRAGSCROLL_DIVISOR_V 12
#define DRAGSCROLL_STRAIGHTEN_SENSITIVITY 90

#define BETTER_DRAGSCROLL_SCRLK_ENABLE
#define BETTER_DRAGSCROLL_END_ON_KEYPRESS
#define BETTER_DRAGSCROLL_ENABLE_LAYER_A 1
#define BETTER_DRAGSCROLL_ENABLE_LAYER_B 2
