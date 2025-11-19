#pragma once

bool process_record_turbo_fire(uint16_t keycode, keyrecord_t *record);
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  #if defined(BETTER_DRAGSCROLL)
    process_record_better_dragscroll(keycode, record);
  #endif // defined(BETTER_DRAGSCROLL)

  #if defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
    process_record_msGesture();
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
        process_record_task_switch(COMMUNITY_MODULE_TASK_SWITCH_NEXT, record);
        return false;
      case PL_TSKP:
        process_record_task_switch(COMMUNITY_MODULE_TASK_SWITCH_PREVIOUS, record);
        return false;
    #endif // defined(COMMUNITY_MODULE_TASK_SWITCH_ENABLE)

    #if defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
      case KB_TURBO_A_TOGGLE:
        process_record_turbo_fire(COMMUNITY_MODULE_TURBO_A_TOGGLE, record);
        return false;
      case KB_TURBO_A_MOMENTARY:
        process_record_turbo_fire(COMMUNITY_MODULE_TURBO_A_MOMENTARY, record);
        return false;
    #endif // defined(COMMUNITY_MODULE_TURBO_FIRE_ENABLE)
  }
  return true;
}