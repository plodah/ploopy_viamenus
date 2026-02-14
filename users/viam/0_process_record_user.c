#pragma once

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    #if defined(BETTER_DRAGSCROLL)
        process_record_better_dragscroll(keycode, record);
    #endif // defined(BETTER_DRAGSCROLL)
    #if defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
    process_record_msgesture();
    #endif // defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)

    switch (keycode) {

        #if defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)
            case PL_MSJG:
                // jiggler_toggle();
                process_record_mouse_jiggler(COMMUNITY_MODULE_MOUSE_JIGGLER_TOGGLE, record);
                return false;
        #endif // defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)

        #if defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE)
            case PL_TSKN:
            case PL_TSKP:
                process_record_task_switch ((keycode - (PL_TSKN - COMMUNITY_MODULE_TASK_SWITCH_NEXT)), record);
                return false;
        #endif // defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE)

        #if defined(COMMUNITY_MODULE_PMW_ROTATION_ENABLE)
            case PMW_CCW:
            case PMW_CW:
            case PMW_RST:
                process_record_pmw_rotation ((keycode - (PMW_CCW - COMMUNITY_MODULE_PMW_ROTATE_CCW)), record);
                pmw_rotation_update_via_keypress();
            return false;
        #endif // defined(COMMUNITY_MODULE_PMW_ROTATION_ENABLE)
    }
    return true;
}
