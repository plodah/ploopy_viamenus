#if defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
  #include "mouse_gesture.h"
  #include "mouse_jiggler.h"
  #include "better_dragscroll.h"
  #include "ploopy_via.h"

  void ploopy_msGestureUpdate(void){
    #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
      PLOOPY_MSGESTURE_X.action = ploopyvia_config.wiggleball_action_h;
      PLOOPY_MSGESTURE_Y.action = ploopyvia_config.wiggleball_action_v;
      gestureCount              = ploopyvia_config.wiggleball_count;
    #else
      PLOOPY_MSGESTURE_X.action = GESTURE_ACTION_DRAGSCROLL;
      PLOOPY_MSGESTURE_Y.action = GESTURE_ACTION_MSJIGGLER;
      gestureCount = PLOOPY_MSGESTURE_WIGGLES;
    #endif
  }
  deferred_token ploopy_msGestureSwitchCooldown = INVALID_DEFERRED_TOKEN;
  deferred_token ploopy_msGestureSwitchDebounce = INVALID_DEFERRED_TOKEN;

  bool ploopy_msGestureDebounce = false;
  bool ploopy_msGestureCooldown = false;

  void ploopy_msGestureResetX(void){
    PLOOPY_MSGESTURE_X.accum = 0;
    PLOOPY_MSGESTURE_X.count = 0;
    PLOOPY_MSGESTURE_X.stage = false;
    PLOOPY_MSGESTURE_X.timeout = INVALID_DEFERRED_TOKEN;
  }

  void ploopy_msGestureResetY(void){
    PLOOPY_MSGESTURE_Y.accum = 0;
    PLOOPY_MSGESTURE_Y.count = 0;
    PLOOPY_MSGESTURE_Y.stage = false;
    PLOOPY_MSGESTURE_Y.timeout = INVALID_DEFERRED_TOKEN;
  }

  void process_record_msGesture(void){
    ploopy_msGestureSwitchCooldown = INVALID_DEFERRED_TOKEN;
    ploopy_msGestureSwitchDebounce = INVALID_DEFERRED_TOKEN;
    ploopy_msGestureResetX();
    ploopy_msGestureResetY();
  }

  //
  uint32_t PLOOPY_MSGESTURE_expireXTimeout(uint32_t trigger_time, void *cb_arg){
    PLOOPY_MSGESTURE_X.accum = 0;
    PLOOPY_MSGESTURE_X.count = 0;
    PLOOPY_MSGESTURE_X.stage = false;
    return 0;
  }

  uint32_t PLOOPY_MSGESTURE_expireYTimeout(uint32_t trigger_time, void *cb_arg){
    PLOOPY_MSGESTURE_Y.accum = 0;
    PLOOPY_MSGESTURE_Y.count = 0;
    PLOOPY_MSGESTURE_Y.stage = false;
    return 0;
  }

  uint32_t PLOOPY_MSGESTURE_expireDebounce(uint32_t trigger_time, void *cb_arg){
    ploopy_msGestureDebounce = false;
    PLOOPY_MSGESTURE_X.accum = 0;
    PLOOPY_MSGESTURE_Y.accum = 0;
    return 0;
  }

  uint32_t PLOOPY_MSGESTURE_expireCooldown(uint32_t trigger_time, void *cb_arg){
    ploopy_msGestureCooldown = false;
    PLOOPY_MSGESTURE_X.accum = 0;
    PLOOPY_MSGESTURE_X.count = 0;
    PLOOPY_MSGESTURE_Y.accum = 0;
    PLOOPY_MSGESTURE_Y.count = 0;
    return 0;
  }

  void ploopy_msGestureTriggered(uint8_t action){
    ploopy_msGestureCooldown = true;
    ploopy_msGestureSwitchCooldown = defer_exec(PLOOPY_MSGESTURE_COOLDOWN, PLOOPY_MSGESTURE_expireCooldown, NULL);
    switch(action){
        case GESTURE_ACTION_MSJIGGLER:
          jiggler_toggle();
          break;
        case GESTURE_ACTION_DRAGSCROLL:
          better_dragscroll_toggle(true);
          break;
    }
  }

  report_mouse_t pointing_device_task_mouse_gesture(report_mouse_t mouse_report) {
    if(ploopy_msGestureDebounce || ploopy_msGestureCooldown){
        return mouse_report;
    }
    PLOOPY_MSGESTURE_X.accum += (10 * mouse_report.x);
    PLOOPY_MSGESTURE_Y.accum += (10 * mouse_report.y);

    if( abs(PLOOPY_MSGESTURE_X.accum) > abs(PLOOPY_MSGESTURE_Y.accum) && abs(PLOOPY_MSGESTURE_X.accum) > PLOOPY_MSGESTURE_THRESHOLD ){
        ploopy_msGestureSwitchDebounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, PLOOPY_MSGESTURE_expireDebounce, NULL);
        if( (abs(PLOOPY_MSGESTURE_X.accum) > PLOOPY_MSGESTURE_THRESHOLD && PLOOPY_MSGESTURE_X.count==0 ) || (PLOOPY_MSGESTURE_X.accum > PLOOPY_MSGESTURE_THRESHOLD && !PLOOPY_MSGESTURE_X.stage) || (PLOOPY_MSGESTURE_X.accum < -PLOOPY_MSGESTURE_THRESHOLD && PLOOPY_MSGESTURE_X.stage) ) {
            PLOOPY_MSGESTURE_X.count++;
            dprintf("==X==> C:%d,%d  acc:%d,%d (%d,%d) \n", PLOOPY_MSGESTURE_X.count, PLOOPY_MSGESTURE_Y.count, PLOOPY_MSGESTURE_X.accum, PLOOPY_MSGESTURE_Y.accum, abs(PLOOPY_MSGESTURE_X.accum), abs(PLOOPY_MSGESTURE_Y.accum) );

            PLOOPY_MSGESTURE_X.stage = PLOOPY_MSGESTURE_X.accum > PLOOPY_MSGESTURE_THRESHOLD;
            ploopy_msGestureDebounce = true;
            cancel_deferred_exec(PLOOPY_MSGESTURE_X.timeout);
            PLOOPY_MSGESTURE_X.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, PLOOPY_MSGESTURE_expireXTimeout, NULL );
        }
    }

    if( abs(PLOOPY_MSGESTURE_Y.accum) > abs(PLOOPY_MSGESTURE_X.accum) && abs(PLOOPY_MSGESTURE_Y.accum) > PLOOPY_MSGESTURE_THRESHOLD ){
        ploopy_msGestureSwitchDebounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, PLOOPY_MSGESTURE_expireDebounce, NULL);
        if( (abs(PLOOPY_MSGESTURE_Y.accum) > PLOOPY_MSGESTURE_THRESHOLD && PLOOPY_MSGESTURE_Y.count==0 ) || (PLOOPY_MSGESTURE_Y.accum > PLOOPY_MSGESTURE_THRESHOLD && !PLOOPY_MSGESTURE_Y.stage) || (PLOOPY_MSGESTURE_Y.accum < -PLOOPY_MSGESTURE_THRESHOLD && PLOOPY_MSGESTURE_Y.stage) ) {
            PLOOPY_MSGESTURE_Y.count++;
            dprintf("==Y==> C:%d,%d  acc:%d,%d (%d,%d) \n", PLOOPY_MSGESTURE_X.count, PLOOPY_MSGESTURE_Y.count, PLOOPY_MSGESTURE_X.accum, PLOOPY_MSGESTURE_Y.accum, abs(PLOOPY_MSGESTURE_X.accum), abs(PLOOPY_MSGESTURE_Y.accum) );
            PLOOPY_MSGESTURE_Y.stage = PLOOPY_MSGESTURE_Y.accum > PLOOPY_MSGESTURE_THRESHOLD;
            ploopy_msGestureDebounce = true;
            cancel_deferred_exec(PLOOPY_MSGESTURE_Y.timeout);
            PLOOPY_MSGESTURE_Y.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, PLOOPY_MSGESTURE_expireYTimeout, NULL );
        }
    }

    if( PLOOPY_MSGESTURE_X.count >= gestureCount ) {
        dprintf("X Jiggle a:%d\n", PLOOPY_MSGESTURE_X.action);
        ploopy_msGestureTriggered(PLOOPY_MSGESTURE_X.action);
    }

    if( PLOOPY_MSGESTURE_Y.count >= gestureCount ) {
        dprintf("Y Jiggle a:%d\n", PLOOPY_MSGESTURE_Y.action);
        ploopy_msGestureTriggered(PLOOPY_MSGESTURE_Y.action);
    }
    return mouse_report;
  }
#endif
