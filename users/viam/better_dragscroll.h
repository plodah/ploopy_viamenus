#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "quantum.h"

#if !defined(BETTER_DRAGSCROLL_DIVISOR_H)
    #define BETTER_DRAGSCROLL_DIVISOR_H 4
#endif

#if !defined(BETTER_DRAGSCROLL_DIVISOR_V)
    #define BETTER_DRAGSCROLL_DIVISOR_V 4
#endif

#if !defined(BETTER_DRAGSCROLL_VOLUME_DIVISOR)
    #define BETTER_DRAGSCROLL_VOLUME_DIVISOR 8
#endif

bool better_dragscroll_enabled_bylock;
bool better_dragscroll_enabled_bypress;
bool better_dragscroll_volume_enabled;
bool better_dragscroll_straighten_enable;

void better_dragscroll_toggle(bool pressed);
void better_dragscroll_momentary(bool pressed);
#if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
    bool led_update_better_dragscroll(led_t led_state);
#endif //     #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
