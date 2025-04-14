#include QMK_KEYBOARD_H

enum layers {
    _BASE,
};

enum keymap_keycodes {
    KB_DPI_CONFIG       = QK_KB_0,
    KB_DRAG_SCROLL,
    BETTER_DRAG_SCROLL_MOMENTARY,
    BETTER_DRAG_SCROLL_TOGGLE,
    PL_MSJG,
    PL_TSKN,
    PL_TSKP,
};

#include "common.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_BTN1
    )
};

void suspend_power_down_user(void) {
    // Switch off sensor + LED making trackball unable to wake host
    adns5050_power_down();

    #if (defined(BETTER_DRAGSCROLL) && defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) && (defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE)) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)))
        led_deinitialize_better_dragscroll();
    #endif
}

void suspend_wakeup_init_user(void) {
    adns5050_init();

    #if (defined(BETTER_DRAGSCROLL) && defined(BETTER_DRAGSCROLL_ON_HOST_VALUE_TOGGLED) && (defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE)) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)))
        led_initialize_better_dragscroll();
    #endif
}
