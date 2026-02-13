#if defined(PLOOPY_MSGESTURE_ENABLE) && defined(DEFERRED_EXEC_ENABLE)
    #include "mouse_gesture.h"
    #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
    #include "mouse_jiggler.h"
    #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
    #include "better_dragscroll.h"
    #include "ploopy_via.h"

  deferred_token msgesture_switch_cooldown = INVALID_DEFERRED_TOKEN;
  deferred_token msgesture_switch_debounce = INVALID_DEFERRED_TOKEN;

  bool msgesture_debounce = false;
  bool msgesture_cooldown = false;

  void ploopy_msGestureUpdate(void){
        #if defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS)
            msgesture_x.action = ploopyvia_config.gesture_action_h;
            msgesture_y.action = ploopyvia_config.gesture_action_v;
            gestureCount       = ploopyvia_config.gesture_count;
        #else
            msgesture_x.action = GESTURE_ACTION_DRAGSCROLL;
            #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
                msgesture_y.action = GESTURE_ACTION_MSJIGGLER;
            #else
                msgesture_y.action = GESTURE_ACTION_NOTHING;
            #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
        gestureCount = PLOOPY_MSGESTURE_WIGGLES;
        #endif
    }

    void msgesture_reset( msgesture_t gesture ){
        gesture.accum = 0;
        gesture.count = 0;
        gesture.stage = false;
        gesture.timeout = INVALID_DEFERRED_TOKEN;
    }

    void process_record_msgesture( void ){
    msgesture_switch_cooldown = INVALID_DEFERRED_TOKEN;
    msgesture_switch_debounce = INVALID_DEFERRED_TOKEN;
        msgesture_reset(msgesture_x);
        msgesture_reset(msgesture_y);
    }

    uint32_t msgesture_expire_x_timeout( uint32_t trigger_time, void *cb_arg ){
        msgesture_x.accum = 0;
        msgesture_x.count = 0;
        msgesture_x.stage = false;
        return 0;
    }

    uint32_t msgesture_expire_y_timeout( uint32_t trigger_time, void *cb_arg ){
        msgesture_y.accum = 0;
        msgesture_y.count = 0;
        msgesture_y.stage = false;
        return 0;
    }

    uint32_t msgesture_expire_debounce(uint32_t trigger_time, void *cb_arg){
        msgesture_debounce = false;
        msgesture_x.accum = 0;
        msgesture_y.accum = 0;
        return 0;
    }

    uint32_t msgesture_expire_cooldown(uint32_t trigger_time, void *cb_arg){
        msgesture_cooldown = false;
        msgesture_x.accum = 0;
        msgesture_x.count = 0;
        msgesture_y.accum = 0;
        msgesture_y.count = 0;
        return 0;
    }

    void msgesture_triggered(uint8_t action){
        msgesture_cooldown = true;
        msgesture_switch_cooldown = defer_exec(PLOOPY_MSGESTURE_COOLDOWN, msgesture_expire_cooldown, NULL);
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
        if(msgesture_debounce || msgesture_cooldown){
            return mouse_report;
        }
        if(msgesture_x.action != 0){
            msgesture_x.accum += (10 * mouse_report.x);
        }
        if(msgesture_y.action != 0){
            msgesture_y.accum += (10 * mouse_report.y);
        }

        if( abs(msgesture_x.accum) > abs(msgesture_y.accum) && abs(msgesture_x.accum) > PLOOPY_MSGESTURE_THRESHOLD ){
            msgesture_switch_debounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, msgesture_expire_debounce, NULL);
            if( (abs(msgesture_x.accum) > PLOOPY_MSGESTURE_THRESHOLD && msgesture_x.count==0 ) || (msgesture_x.accum > PLOOPY_MSGESTURE_THRESHOLD && !msgesture_x.stage) || (msgesture_x.accum < -PLOOPY_MSGESTURE_THRESHOLD && msgesture_x.stage) ) {
                msgesture_x.count++;
                dprintf("==X==> C:%d,%d  acc:%d,%d (%d,%d) \n", msgesture_x.count, msgesture_y.count, msgesture_x.accum, msgesture_y.accum, abs(msgesture_x.accum), abs(msgesture_y.accum) );

                msgesture_x.stage = msgesture_x.accum > PLOOPY_MSGESTURE_THRESHOLD;
                msgesture_debounce = true;
                cancel_deferred_exec(msgesture_x.timeout);
                msgesture_x.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, msgesture_expire_x_timeout, NULL );
            }
        }

        if( abs(msgesture_y.accum) > abs(msgesture_x.accum) && abs(msgesture_y.accum) > PLOOPY_MSGESTURE_THRESHOLD ){
            msgesture_switch_debounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, msgesture_expire_debounce, NULL);
            if( (abs(msgesture_y.accum) > PLOOPY_MSGESTURE_THRESHOLD && msgesture_y.count==0 ) || (msgesture_y.accum > PLOOPY_MSGESTURE_THRESHOLD && !msgesture_y.stage) || (msgesture_y.accum < -PLOOPY_MSGESTURE_THRESHOLD && msgesture_y.stage) ) {
                msgesture_y.count++;
                dprintf("==Y==> C:%d,%d  acc:%d,%d (%d,%d) \n", msgesture_x.count, msgesture_y.count, msgesture_x.accum, msgesture_y.accum, abs(msgesture_x.accum), abs(msgesture_y.accum) );
                msgesture_y.stage = msgesture_y.accum > PLOOPY_MSGESTURE_THRESHOLD;
                msgesture_debounce = true;
                cancel_deferred_exec(msgesture_y.timeout);
                msgesture_y.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, msgesture_expire_y_timeout, NULL );
            }
        }

        if( msgesture_x.count >= gestureCount ) {
            dprintf("X gesture act:%d\n", msgesture_x.action);
            msgesture_triggered(msgesture_x.action);
        }

        if( msgesture_y.count >= gestureCount ) {
            dprintf("Y gesture act:%d\n", msgesture_y.action);
            msgesture_triggered(msgesture_y.action);
        }
        return mouse_report;
    }
#endif
