#include QMK_KEYBOARD_H

#if defined(PLOOPY_MSGESTURE_ENABLE) && !defined(MSGESTURE_MODE_DE)
    #include "mouse_gesture.h"
#endif //

#if defined(PLOOPY_MSGESTURE_ENABLE) && !defined(MSGESTURE_MODE_DE)
    void housekeeping_task_user (void) {
        #if defined(PLOOPY_MSGESTURE_ENABLE) && !defined(MSGESTURE_MODE_DE)
            housekeeping_task_msgesture();
        #endif //
    }
#endif //
