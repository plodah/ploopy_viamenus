#pragma once

#define PLOOPY_VIAMENUS
#define BETTER_DRAGSCROLL

#if ! defined(DYNAMIC_KEYMAP_LAYER_COUNT)
    #if defined(RP2040)
        #define DYNAMIC_KEYMAP_LAYER_COUNT 8
    #else
        #define DYNAMIC_KEYMAP_LAYER_COUNT 8
    #endif
#endif // DYNAMIC_KEYMAP_LAYER_COUNT
