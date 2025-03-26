#if defined(COMBO_ENABLE)
  #pragma once

    const uint16_t PROGMEM pl_combo_ms12[] = {KC_BTN1, KC_BTN2, COMBO_END};
    const uint16_t PROGMEM pl_combo_ms15[] = {KC_BTN1, KC_BTN5, COMBO_END};
    const uint16_t PROGMEM pl_combo_ms24[] = {KC_BTN2, KC_BTN4, COMBO_END};
    const uint16_t PROGMEM pl_combo_ms25[] = {KC_BTN2, KC_BTN5, COMBO_END};

    const uint16_t PROGMEM pl_combo_ms15_lt1[] = {KC_BTN1 , LT(1, KC_BTN5), COMBO_END};
    const uint16_t PROGMEM pl_combo_ms15_lt2[] = {KC_BTN1 , LT(2, KC_BTN5), COMBO_END};
    const uint16_t PROGMEM pl_combo_ms24_lt1[] = {KC_BTN2 , LT(1, KC_BTN4), COMBO_END};
    const uint16_t PROGMEM pl_combo_ms25_lt1[] = {KC_BTN2 , LT(1, KC_BTN5), COMBO_END};
    const uint16_t PROGMEM pl_combo_ms25_lt2[] = {KC_BTN2 , LT(2, KC_BTN5), COMBO_END};

    const uint16_t PROGMEM pl_combo_ms1d[] = {KC_BTN1, BETTER_DRAG_SCROLL_MOMENTARY, COMBO_END};
    const uint16_t PROGMEM pl_combo_ms2d[] = {KC_BTN2, BETTER_DRAG_SCROLL_MOMENTARY, COMBO_END};

    #if defined(COMMUNITY_MODULE_MSJIGGLER_ENABLE)
      const uint16_t PROGMEM pl_combo_ms14[] = {KC_BTN1, KC_BTN4, COMBO_END};
      const uint16_t PROGMEM pl_combo_ms14_lt1[] = {KC_BTN1 , LT(1, KC_BTN4), COMBO_END};
      const uint16_t PROGMEM pl_combo_ms45[] = {KC_BTN4, KC_BTN5, COMBO_END};
      const uint16_t PROGMEM pl_combo_ms45_lt01[] = {KC_BTN4 , LT(1, KC_BTN5), COMBO_END};
      const uint16_t PROGMEM pl_combo_ms45_lt02[] = {KC_BTN4 , LT(2, KC_BTN5), COMBO_END};
      const uint16_t PROGMEM pl_combo_ms45_lt10[] = {LT(1, KC_BTN4) , KC_BTN5, COMBO_END};
      const uint16_t PROGMEM pl_combo_ms45_lt11[] = {LT(1, KC_BTN4) , LT(1, KC_BTN5), COMBO_END};
      const uint16_t PROGMEM pl_combo_ms45_lt12[] = {LT(1, KC_BTN4) , LT(2, KC_BTN5), COMBO_END};
    #endif // defined(COMMUNITY_MODULE_MSJIGGLER_ENABLE)

    combo_t key_combos[] = {
      #if defined(TAP_DANCE_ENABLE) && defined(BETTER_DRAGSCROLL_TAPDANCE)
        COMBO(pl_combo_ms12,  TD(TD_DRAGSCROLL)),
      #else // BETTER_DRAGSCROLL_TAPDANCE
        COMBO(pl_combo_ms12,  BETTER_DRAG_SCROLL_MOMENTARY),
      #endif // BETTER_DRAGSCROLL_TAPDANCE
      COMBO(pl_combo_ms15,  C(KC_C)),
      COMBO(pl_combo_ms24,  BETTER_DRAG_SCROLL_MOMENTARY),
      COMBO(pl_combo_ms25,  C(KC_V)),

      COMBO(pl_combo_ms15_lt1,  C(KC_C)),
      COMBO(pl_combo_ms15_lt2,  C(KC_C)),
      COMBO(pl_combo_ms24_lt1,  BETTER_DRAG_SCROLL_MOMENTARY),
      COMBO(pl_combo_ms25_lt1,  C(KC_V)),
      COMBO(pl_combo_ms25_lt2,  C(KC_V)),

      COMBO(pl_combo_ms1d,  BETTER_DRAG_SCROLL_TOGGLE),
      COMBO(pl_combo_ms2d,  BETTER_DRAG_SCROLL_TOGGLE),

      #if defined(COMMUNITY_MODULE_MSJIGGLER_ENABLE)
        COMBO(pl_combo_ms14,  CM_MSJG),
        COMBO(pl_combo_ms14_lt1,  CM_MSJG),
        COMBO(pl_combo_ms45,  CM_MSJG),
        COMBO(pl_combo_ms45_lt01,  CM_MSJG),
        COMBO(pl_combo_ms45_lt02,  CM_MSJG),
        COMBO(pl_combo_ms45_lt10,  CM_MSJG),
        COMBO(pl_combo_ms45_lt11,  CM_MSJG),
        COMBO(pl_combo_ms45_lt12,  CM_MSJG),
      #endif // COMMUNITY_MODULE_MSJIGGLER_ENABLE
    };

#endif // COMBO_ENABLE
