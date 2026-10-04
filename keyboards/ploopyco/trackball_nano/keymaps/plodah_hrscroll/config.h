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

// Scroll rate is       1 /
//           ( MULTIPLIER * 10^EXPONENT )

#define POINTING_DEVICE_HIRES_SCROLL_ENABLE
#define POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER 60
#define POINTING_DEVICE_HIRES_SCROLL_EXPONENT 0

#define EECONFIG_USER_DATA_SIZE 43
#define PLOOPY_MSGESTURE_ENABLE
#define PLOOPY_DPI_DEFAULT 1 // 0 indexed
#define PLOOPY_DPI_OPTIONS { 300, 400, 500, 600, 800 }

#define BETTER_DRAGSCROLL_INVERT_V 1
#define BETTER_DRAGSCROLL_DIVISOR_V 24
#define BETTER_DRAGSCROLL_DIVISOR_H 24
#define BETTER_DRAGSCROLL_SCRLK_ENABLE LOCKSTATE_WHILE_ENABLED
#define BETTER_DRAGSCROLL_NUMLK_ENABLE LOCKSTATE_WHILE_ENABLED
#define DRAGSCROLL_STRAIGHTEN_SENSITIVITY 85
