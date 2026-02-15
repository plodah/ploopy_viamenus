#ifdef CONSOLE_ENABLE
    #include QMK_KEYBOARD_H

    void keyboard_post_init_user_debug(void) {
        debug_enable = true;
        // debug_matrix = true;
        // debug_keyboard = true;
        // debug_mouse = true;
  }
#endif // CONSOLE_ENABLE
