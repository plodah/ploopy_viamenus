#include QMK_KEYBOARD_H
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    #if defined(BETTER_DRAGSCROLL)
        process_record_better_dragscroll(keycode, record);
    #endif // defined(BETTER_DRAGSCROLL)
    process_record_dpi(keycode, record);
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

        #if defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
            case TBO_TG1:
            case TBO_MO1:
            case TBO_TG2:
            case TBO_MO2:
            case TBO_TG3:
            case TBO_MO3:
            case TBO_TG4:
            case TBO_MO4:
                process_record_turbo_fire ((keycode - (TBO_TG1 - COMMUNITY_MODULE_TURBO_A_TOGGLE)), record);
                return false;
        #endif // defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)

        #if defined(COMMUNITY_MODULE_MORSE_CODE_ENABLE)
            case MOR_MAN:
            case MOR_DIT:
            case MOR_DSH:
                process_record_turbo_fire ((keycode - (MOR_MAN - COMMUNITY_MODULE_MORSE_CODE_MANUAL)), record);
                return false;
        #endif // defined(COMMUNITY_MODULE_MORSE_CODE_ENABLE)

    }
    return true;
}
