#pragma once
#include "kc_aliases.h"

#define VIA_FIRMWARE_VERSION 2
#define VIA_INSECURE
#define PLOOPY_VIAMENUS
#define BETTER_DRAGSCROLL

// set number of layers- 8 if rp2040, else 4
#if ! defined(DYNAMIC_KEYMAP_LAYER_COUNT)
    #if defined(QMK_MCU_RP2040)
        #define DYNAMIC_KEYMAP_LAYER_COUNT 8
    #else
        #define DYNAMIC_KEYMAP_LAYER_COUNT 4
    #endif
#endif // DYNAMIC_KEYMAP_LAYER_COUNT

// Some combos only on rp2040
#if defined(QMK_MCU_RP2040)
    #if (!defined(PLOOPY_COBMO_EXCESSIVE))
        #define PLOOPY_COBMO_EXCESSIVE
    #endif // PLOOPY_COBMO_EXCESSIVE
#endif // QMK_MCU_RP2040
