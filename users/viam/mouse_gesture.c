#if defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
  #include "mouse_gesture.h"
  #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
  #include "mouse_jiggler.h"
  #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
  #include "better_dragscroll.h"
  #include "ploopy_via.h"

  deferred_token ploopy_msgesture_switch_cooldown = INVALID_DEFERRED_TOKEN;
  deferred_token ploopy_msgesture_switch_debounce = INVALID_DEFERRED_TOKEN;

  bool ploopy_msgesture_debounce = false;
  bool ploopy_msgesture_cooldown = false;

  void ploopy_msGestureUpdate(void){
    #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
        ploopy_msgesture_x.action = ploopyvia_config.gesture_action_h;
        ploopy_msgesture_y.action = ploopyvia_config.gesture_action_v;
        gestureCount              = ploopyvia_config.gesture_count;
    #else
        ploopy_msgesture_x.action = GESTURE_ACTION_DRAGSCROLL;
        #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
            ploopy_msgesture_y.action = GESTURE_ACTION_MSJIGGLER;
        #else
            ploopy_msgesture_y.action = GESTURE_ACTION_NOTHING;
        #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
      gestureCount = PLOOPY_MSGESTURE_WIGGLES;
    #endif
  }

  void ploopy_msgesture_reset_x(void){
    ploopy_msgesture_x.accum = 0;
    ploopy_msgesture_x.count = 0;
    ploopy_msgesture_x.stage = false;
    ploopy_msgesture_x.timeout = INVALID_DEFERRED_TOKEN;
  }

  void ploopy_msgesture_reset_y(void){
    ploopy_msgesture_y.accum = 0;
    ploopy_msgesture_y.count = 0;
    ploopy_msgesture_y.stage = false;
    ploopy_msgesture_y.timeout = INVALID_DEFERRED_TOKEN;
  }

  void process_record_msGesture(void){
    ploopy_msgesture_switch_cooldown = INVALID_DEFERRED_TOKEN;
    ploopy_msgesture_switch_debounce = INVALID_DEFERRED_TOKEN;
    ploopy_msgesture_reset_x();
    ploopy_msgesture_reset_y();
  }

  uint32_t ploopy_msgesture_expire_x_timeout(uint32_t trigger_time, void *cb_arg){
    ploopy_msgesture_x.accum = 0;
    ploopy_msgesture_x.count = 0;
    ploopy_msgesture_x.stage = false;
    return 0;
  }

  uint32_t ploopy_msgesture_expire_y_timeout(uint32_t trigger_time, void *cb_arg){
    ploopy_msgesture_y.accum = 0;
    ploopy_msgesture_y.count = 0;
    ploopy_msgesture_y.stage = false;
    return 0;
  }

  uint32_t ploopy_msgesture_expiredebounce(uint32_t trigger_time, void *cb_arg){
    ploopy_msgesture_debounce = false;
    ploopy_msgesture_x.accum = 0;
    ploopy_msgesture_y.accum = 0;
    return 0;
  }

  uint32_t ploopy_msgesture_expire_cooldown(uint32_t trigger_time, void *cb_arg){
    ploopy_msgesture_cooldown = false;
    ploopy_msgesture_x.accum = 0;
    ploopy_msgesture_x.count = 0;
    ploopy_msgesture_y.accum = 0;
    ploopy_msgesture_y.count = 0;
    return 0;
  }

  void ploopy_msgesture_triggered(uint8_t action){
    ploopy_msgesture_cooldown = true;
    ploopy_msgesture_switch_cooldown = defer_exec(PLOOPY_MSGESTURE_COOLDOWN, ploopy_msgesture_expire_cooldown, NULL);
        switch(action){
            case GESTURE_ACTION_MSJIGGLER:
                #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
                    jiggler_toggle();
                #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
                break;
            case GESTURE_ACTION_DRAGSCROLL:
                better_dragscroll_toggle(true);
                break;
        }
  }

  report_mouse_t pointing_device_task_mouse_gesture(report_mouse_t mouse_report) {
    if(ploopy_msgesture_debounce || ploopy_msgesture_cooldown){
        return mouse_report;
    }
    if(ploopy_msgesture_x.action != 0){
        ploopy_msgesture_x.accum += (10 * mouse_report.x);
    }
    if(ploopy_msgesture_y.action != 0){
        ploopy_msgesture_y.accum += (10 * mouse_report.y);
    }

    if( abs(ploopy_msgesture_x.accum) > abs(ploopy_msgesture_y.accum) && abs(ploopy_msgesture_x.accum) > PLOOPY_MSGESTURE_THRESHOLD ){
        ploopy_msgesture_switch_debounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, ploopy_msgesture_expiredebounce, NULL);
        if( (abs(ploopy_msgesture_x.accum) > PLOOPY_MSGESTURE_THRESHOLD && ploopy_msgesture_x.count==0 ) || (ploopy_msgesture_x.accum > PLOOPY_MSGESTURE_THRESHOLD && !ploopy_msgesture_x.stage) || (ploopy_msgesture_x.accum < -PLOOPY_MSGESTURE_THRESHOLD && ploopy_msgesture_x.stage) ) {
            ploopy_msgesture_x.count++;
            dprintf("==X==> C:%d,%d  acc:%d,%d (%d,%d) \n", ploopy_msgesture_x.count, ploopy_msgesture_y.count, ploopy_msgesture_x.accum, ploopy_msgesture_y.accum, abs(ploopy_msgesture_x.accum), abs(ploopy_msgesture_y.accum) );

            ploopy_msgesture_x.stage = ploopy_msgesture_x.accum > PLOOPY_MSGESTURE_THRESHOLD;
            ploopy_msgesture_debounce = true;
            cancel_deferred_exec(ploopy_msgesture_x.timeout);
            ploopy_msgesture_x.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, ploopy_msgesture_expire_x_timeout, NULL );
        }
    }

    if( abs(ploopy_msgesture_y.accum) > abs(ploopy_msgesture_x.accum) && abs(ploopy_msgesture_y.accum) > PLOOPY_MSGESTURE_THRESHOLD ){
        ploopy_msgesture_switch_debounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, ploopy_msgesture_expiredebounce, NULL);
        if( (abs(ploopy_msgesture_y.accum) > PLOOPY_MSGESTURE_THRESHOLD && ploopy_msgesture_y.count==0 ) || (ploopy_msgesture_y.accum > PLOOPY_MSGESTURE_THRESHOLD && !ploopy_msgesture_y.stage) || (ploopy_msgesture_y.accum < -PLOOPY_MSGESTURE_THRESHOLD && ploopy_msgesture_y.stage) ) {
            ploopy_msgesture_y.count++;
            dprintf("==Y==> C:%d,%d  acc:%d,%d (%d,%d) \n", ploopy_msgesture_x.count, ploopy_msgesture_y.count, ploopy_msgesture_x.accum, ploopy_msgesture_y.accum, abs(ploopy_msgesture_x.accum), abs(ploopy_msgesture_y.accum) );
            ploopy_msgesture_y.stage = ploopy_msgesture_y.accum > PLOOPY_MSGESTURE_THRESHOLD;
            ploopy_msgesture_debounce = true;
            cancel_deferred_exec(ploopy_msgesture_y.timeout);
            ploopy_msgesture_y.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, ploopy_msgesture_expire_y_timeout, NULL );
        }
    }

    if( ploopy_msgesture_x.count >= gestureCount ) {
        dprintf("X gesture act:%d\n", ploopy_msgesture_x.action);
        ploopy_msgesture_triggered(ploopy_msgesture_x.action);
    }

    if( ploopy_msgesture_y.count >= gestureCount ) {
        dprintf("Y gesture act:%d\n", ploopy_msgesture_y.action);
        ploopy_msgesture_triggered(ploopy_msgesture_y.action);
    }
    return mouse_report;
  }
#endif
