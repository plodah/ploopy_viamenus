#pragma once

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  #if defined(BETTER_DRAGSCROLL)
    process_record_better_dragscroll(keycode, record);
  #endif // defined(BETTER_DRAGSCROLL)

  #if defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
    process_record_msGesture();
  #endif // defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)

  switch (keycode) {

    #if defined(KC_PL_SCRL)
      case PL_SCRL:
        tap_code(KC_SCRL);
        return false;
    #endif // defined(KC_PL_SCRL)

  }
  return true;
}
