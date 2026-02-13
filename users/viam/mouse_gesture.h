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

typedef struct ploopy_msgesture_t {
    deferred_token cooldown;
    deferred_token timeout;
    uint8_t action;
    uint8_t count;
    int16_t accum;
    bool stage;
} ploopy_msgesture_t;

ploopy_msgesture_t ploopy_msgesture_x;
ploopy_msgesture_t ploopy_msgesture_y;
uint8_t gestureCount;
