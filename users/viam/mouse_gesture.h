#pragma once

#define PLOOPY_MSGESTURE_TIMEOUT 225
#define PLOOPY_MSGESTURE_DEBOUNCE 25
#define PLOOPY_MSGESTURE_COOLDOWN 800
#define PLOOPY_MSGESTURE_WIGGLES 4
#define PLOOPY_MSGESTURE_THRESHOLD 1800

report_mouse_t pointing_device_task_mouse_gesture(report_mouse_t mouse_report);
void process_record_msGesture(void);
void ploopy_msGestureUpdate(void);

enum gesture_actions {
    GESTURE_ACTION_NOTHING = 0,
    GESTURE_ACTION_DRAGSCROLL,
    GESTURE_ACTION_MSJIGGLER,
};

typedef struct PLOOPY_MSGESTURE_t {
    deferred_token cooldown;
    deferred_token timeout;
    uint8_t action;
    uint8_t count;
    int16_t accum;
    bool stage;
} PLOOPY_MSGESTURE_t;

PLOOPY_MSGESTURE_t PLOOPY_MSGESTURE_X;
PLOOPY_MSGESTURE_t PLOOPY_MSGESTURE_Y;
uint8_t gestureCount;
