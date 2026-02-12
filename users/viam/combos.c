#if defined(COMBO_ENABLE)
  #pragma once

    const uint16_t PROGMEM pl_combo_ms12[] = {MS_BTN1, MS_BTN2, COMBO_END};
    const uint16_t PROGMEM pl_combo_ms24[] = {MS_BTN2, MS_BTN4, COMBO_END};
    const uint16_t PROGMEM pl_combo_ms24_lt1[] = {MS_BTN2 , LT(1, MS_BTN4), COMBO_END};

    #if defined(PLOOPY_COBMO_EXCESSIVE)
        const uint16_t PROGMEM pl_combo_ms15[] = {MS_BTN1, MS_BTN5, COMBO_END};
        const uint16_t PROGMEM pl_combo_ms25[] = {MS_BTN2, MS_BTN5, COMBO_END};

        const uint16_t PROGMEM pl_combo_ms15_lt1[] = {MS_BTN1 , LT(1, MS_BTN5), COMBO_END};
        const uint16_t PROGMEM pl_combo_ms15_lt2[] = {MS_BTN1 , LT(2, MS_BTN5), COMBO_END};
        const uint16_t PROGMEM pl_combo_ms25_lt1[] = {MS_BTN2 , LT(1, MS_BTN5), COMBO_END};
        const uint16_t PROGMEM pl_combo_ms25_lt2[] = {MS_BTN2 , LT(2, MS_BTN5), COMBO_END};

        const uint16_t PROGMEM pl_combo_ms1d[] = {MS_BTN1, BETTER_DRAG_SCROLL_MOMENTARY, COMBO_END};
        const uint16_t PROGMEM pl_combo_ms2d[] = {MS_BTN2, BETTER_DRAG_SCROLL_MOMENTARY, COMBO_END};
    #endif // defined(PLOOPY_COBMO_EXCESSIVE)

    #if defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)
        const uint16_t PROGMEM pl_combo_ms14[] = {MS_BTN1, MS_BTN4, COMBO_END};
        const uint16_t PROGMEM pl_combo_ms14_lt1[] = {MS_BTN1 , LT(1, MS_BTN4), COMBO_END};
        #if defined(PLOOPY_COBMO_EXCESSIVE)
            const uint16_t PROGMEM pl_combo_ms45[] = {MS_BTN4, MS_BTN5, COMBO_END};
            const uint16_t PROGMEM pl_combo_ms45_lt01[] = {MS_BTN4 , LT(1, MS_BTN5), COMBO_END};
            const uint16_t PROGMEM pl_combo_ms45_lt02[] = {MS_BTN4 , LT(2, MS_BTN5), COMBO_END};
            const uint16_t PROGMEM pl_combo_ms45_lt10[] = {LT(1, MS_BTN4) , MS_BTN5, COMBO_END};
            const uint16_t PROGMEM pl_combo_ms45_lt11[] = {LT(1, MS_BTN4) , LT(1, MS_BTN5), COMBO_END};
            const uint16_t PROGMEM pl_combo_ms45_lt12[] = {LT(1, MS_BTN4) , LT(2, MS_BTN5), COMBO_END};
        #endif // defined(PLOOPY_COBMO_EXCESSIVE)
    #endif // defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)

    combo_t key_combos[] = {
        #if defined(TAP_DANCE_ENABLE) && defined(BETTER_DRAGSCROLL_TAPDANCE)
            COMBO(pl_combo_ms12,  TD(TD_DRAGSCROLL)),
        #else // BETTER_DRAGSCROLL_TAPDANCE
            COMBO(pl_combo_ms12,  BETTER_DRAG_SCROLL_MOMENTARY),
        #endif // BETTER_DRAGSCROLL_TAPDANCE
        COMBO(pl_combo_ms24,  BETTER_DRAG_SCROLL_MOMENTARY),
        COMBO(pl_combo_ms24_lt1,  BETTER_DRAG_SCROLL_MOMENTARY),

        #if defined(PLOOPY_COBMO_EXCESSIVE)
            COMBO(pl_combo_ms15,  C(KC_C)),
            COMBO(pl_combo_ms25,  C(KC_V)),
            COMBO(pl_combo_ms15_lt1,  C(KC_C)),
            COMBO(pl_combo_ms15_lt2,  C(KC_C)),
            COMBO(pl_combo_ms25_lt1,  C(KC_V)),
            COMBO(pl_combo_ms25_lt2,  C(KC_V)),

            COMBO(pl_combo_ms1d,  BETTER_DRAG_SCROLL_TOGGLE),
            COMBO(pl_combo_ms2d,  BETTER_DRAG_SCROLL_TOGGLE),
        #endif // defined(PLOOPY_COBMO_EXCESSIVE)


        #if defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)

            COMBO(pl_combo_ms14,  CM_MSJG),
            COMBO(pl_combo_ms14_lt1,  CM_MSJG),
            #if defined(PLOOPY_COBMO_EXCESSIVE)

                COMBO(pl_combo_ms45,  CM_MSJG),
                COMBO(pl_combo_ms45_lt01,  CM_MSJG),
                COMBO(pl_combo_ms45_lt02,  CM_MSJG),
                COMBO(pl_combo_ms45_lt10,  CM_MSJG),
                COMBO(pl_combo_ms45_lt11,  CM_MSJG),
                COMBO(pl_combo_ms45_lt12,  CM_MSJG),
            #endif // defined(PLOOPY_COBMO_EXCESSIVE)
        #endif // COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
    };

    #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS) && defined(COMBO_SHOULD_TRIGGER)

        bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
            if (ploopyvia_config.combos_enabled){
                return true;
            }
            return false;
        }

    #endif // defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)

#endif // COMBO_ENABLE
