#pragma once
#include QMK_KEYBOARD_H

#define PLOOPY_MSGESTURE_TIMEOUT 225
#define PLOOPY_MSGESTURE_DEBOUNCE 25
#define PLOOPY_MSGESTURE_COOLDOWN 800
#define PLOOPY_MSGESTURE_WIGGLES 4
#define PLOOPY_MSGESTURE_THRESHOLD 1800

#if defined(DEFERRED_EXEC_ENABLE) && (!defined(MSGESTURE_FORCE_NON_DE))
    #define MSGESTURE_MODE_DE
#endif

report_mouse_t pointing_device_task_mouse_gesture(report_mouse_t mouse_report);
void process_record_msgesture(void);
void ploopy_msGestureUpdate(void);
#if !defined(MSGESTURE_MODE_DE)
    void housekeeping_task_msgesture(void);
#endif // !defined(MSGESTURE_MODE_DE)
enum gesture_actions {
    GESTURE_ACTION_NOTHING = 0,
    GESTURE_ACTION_DRAGSCROLL,
    GESTURE_ACTION_MSJIGGLER,
    GESTURE_ACTION_KEYCODE,
};

typedef struct msgesture_t {
    #if defined(MSGESTURE_MODE_DE)
        deferred_token timeout;
    #else // MSGESTURE_MODE_DE
        uint16_t timer;
    #endif
    uint8_t action;
    uint8_t count;
    int16_t accum;
    bool stage;
} msgesture_t;
msgesture_t msgesture_x;
msgesture_t msgesture_y;
uint8_t gestureCount;
