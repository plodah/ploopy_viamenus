#pragma once
#include "debug.c"
#if defined (CONSOLE_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))
    void keyboard_post_init_user(void) {
        #ifdef CONSOLE_ENABLE
            keyboard_post_init_user_debug();
        #endif // CONSOLE_ENABLE
        #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
            keyboard_post_init_user_viamenus();
        #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
    }
#endif // CONSOLE_ENABLE
