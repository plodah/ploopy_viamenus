#pragma once

#include "quantum.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#if !defined(BETTER_DRAGSCROLL_DIVISOR_H)
    #define BETTER_DRAGSCROLL_DIVISOR_H 4
#endif

#if !defined(BETTER_DRAGSCROLL_DIVISOR_V)
    #define BETTER_DRAGSCROLL_DIVISOR_V 4
#endif

#if !defined(BETTER_DRAGSCROLL_DRAGACTION_DIVISOR)
    #define BETTER_DRAGSCROLL_DRAGACTION_DIVISOR 8
#endif

void better_dragscroll_toggle(bool pressed);
void better_dragscroll_momentary(bool pressed);

#if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
    bool led_update_better_dragscroll(led_t led_state);
#endif //     #if defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))

typedef struct {
    bool enabled_bylock;
    bool enabled_bypress;
    bool sniper_a_enabled;
    bool sniper_b_enabled;
    bool dragaction_enabled;
    bool dragaction_alt;
} dragscroll_state;

dragscroll_state dragscroll_state_default = {
    .enabled_bylock = 0,
    .enabled_bypress = 0,
    .sniper_a_enabled = 0,
    .sniper_b_enabled = 0,
    .dragaction_enabled = 0,
    .dragaction_alt = 0,
};

dragscroll_state ds_state;
