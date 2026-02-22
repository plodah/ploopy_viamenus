#if defined(PLOOPY_MSGESTURE_ENABLE)
    #include "mouse_gesture.h"
    #ifdef COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
        #include "mouse_jiggler.h"
    #endif // def COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE
    #include "better_dragscroll.h"
    #include "ploopy_via.h"

    #if defined(MSGESTURE_MODE_DE)
        deferred_token msgesture_switch_cooldown = INVALID_DEFERRED_TOKEN;
        deferred_token msgesture_switch_debounce = INVALID_DEFERRED_TOKEN;
    #else
        uint16_t cooldown_timer;
        uint16_t debounce_timer;
    #endif // defined(MSGESTURE_MODE_DE)

    bool cooldown_active = false;
    bool debounce_active = false;

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

    void msgesture_reset_x(void){
        msgesture_x.accum = 0;
        msgesture_x.count = 0;
        msgesture_x.stage = false;
        #if defined(MSGESTURE_MODE_DE)
            msgesture_x.timeout = INVALID_DEFERRED_TOKEN;
        #else // MSGESTURE_MODE_DE
            msgesture_x.timer = 0;
        #endif
    }

    void msgesture_reset_y(void){
        msgesture_y.accum = 0;
        msgesture_y.count = 0;
        msgesture_y.stage = false;
        #if defined(MSGESTURE_MODE_DE)
            msgesture_y.timeout = INVALID_DEFERRED_TOKEN;
        #else // MSGESTURE_MODE_DE
            msgesture_y.timer = 0;
        #endif
    }

    void process_record_msgesture( void ){
        #if defined(MSGESTURE_MODE_DE)
            msgesture_switch_cooldown = INVALID_DEFERRED_TOKEN;
            msgesture_switch_debounce = INVALID_DEFERRED_TOKEN;
        #else
            cooldown_timer = 0;
            debounce_timer = 0;
        #endif // MSGESTURE_MODE_DE
        msgesture_reset_x();
        msgesture_reset_y();
    }

    void expire_debounce (void){
        dprintf("gesture debounce expired\n");
        debounce_active = false;
        msgesture_x.accum = 0;
        msgesture_y.accum = 0;
    }

    void expire_cooldown (void){
        dprintf("gesture cooldown expired\n");
        cooldown_active = false;
        msgesture_x.accum = 0;
        msgesture_x.count = 0;
        msgesture_y.accum = 0;
        msgesture_y.count = 0;
    }

    #if defined(MSGESTURE_MODE_DE)
        uint32_t expire_timeout_x_de( uint32_t trigger_time, void *cb_arg ){
            msgesture_reset_x();
            return 0;
        }
        uint32_t expire_timeout_y_de( uint32_t trigger_time, void *cb_arg ){
            msgesture_reset_y();
            return 0;
        }

        uint32_t expire_debounce_de(uint32_t trigger_time, void *cb_arg){
            expire_debounce();
            return 0;
        }

        uint32_t expire_cooldown_de(uint32_t trigger_time, void *cb_arg){
            expire_cooldown();
            return 0;
        }
    #else // MSGESTURE_MODE_DE
        void housekeeping_task_msgesture(void) {
            if (cooldown_active && timer_elapsed(cooldown_timer) > PLOOPY_MSGESTURE_COOLDOWN) {
                dprintf("expire_cooldown\n");
                expire_cooldown();
            }
            if (debounce_active && timer_elapsed(debounce_timer) > PLOOPY_MSGESTURE_DEBOUNCE) {
                dprintf("expire_debounce\n");
                expire_debounce();
            }
            if (msgesture_x.count >= 1 && timer_elapsed(msgesture_x.timer) > PLOOPY_MSGESTURE_TIMEOUT) {
                dprintf("expire_msgesture_x\n");
                msgesture_reset_x();
            }
            if (msgesture_y.count >= 1 && timer_elapsed(msgesture_y.timer) > PLOOPY_MSGESTURE_TIMEOUT) {
                dprintf("expire_msgesture_y\n");
                msgesture_reset_y();
            }
        }
    #endif // MSGESTURE_MODE_DE

    void start_cooldown( void ){
        #if defined(MSGESTURE_MODE_DE)
            msgesture_switch_cooldown = defer_exec(PLOOPY_MSGESTURE_COOLDOWN, expire_cooldown_de, NULL);
        #else
            cooldown_timer = timer_read();
        #endif // MSGESTURE_MODE_DE
        cooldown_active = true;
    }

    void start_debounce( void ){
        #if defined(MSGESTURE_MODE_DE)
            msgesture_switch_debounce = defer_exec(PLOOPY_MSGESTURE_DEBOUNCE, expire_debounce_de, NULL);
        #else
            debounce_timer = timer_read();
        #endif // MSGESTURE_MODE_DE
        debounce_active = true;
    }

    void msgesture_triggered(uint8_t action){
        start_cooldown();
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
        if(debounce_active || cooldown_active){
            return mouse_report;
        }
        msgesture_x.accum += (10 * mouse_report.x);
        msgesture_y.accum += (10 * mouse_report.y);

        if(
            abs(msgesture_x.accum) > abs(msgesture_y.accum)
            && abs(msgesture_x.accum) > PLOOPY_MSGESTURE_THRESHOLD
        ){
            start_debounce();
            if( (
                    abs(msgesture_x.accum) > PLOOPY_MSGESTURE_THRESHOLD \
                    && msgesture_x.count==0
                ) ||
                (
                    msgesture_x.accum > PLOOPY_MSGESTURE_THRESHOLD
                    && !msgesture_x.stage
                ) ||
                (
                    msgesture_x.accum < -PLOOPY_MSGESTURE_THRESHOLD
                    && msgesture_x.stage
                )
            ) {
                msgesture_x.count++;
                dprintf("==X==> C:%d,%d  acc:%d,%d (%d,%d) \n",
                    msgesture_x.count, msgesture_y.count,
                    msgesture_x.accum, msgesture_y.accum,
                    abs(msgesture_x.accum), abs(msgesture_y.accum)
                );

                msgesture_x.stage = msgesture_x.accum > PLOOPY_MSGESTURE_THRESHOLD;
                debounce_active = true;
                #if defined(MSGESTURE_MODE_DE)
                    dprintf("MsGesture X start timeout\n");
                    cancel_deferred_exec(msgesture_x.timeout);
                    msgesture_x.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, expire_timeout_x_de, NULL );
                    /*
                    // This is logically better but takes up 80 bytes. I hate this.
                    if(msgesture_x.timeout != INVALID_DEFERRED_TOKEN){
                        dprintf("MsGesture X extend\n");
                        extend_deferred_exec(msgesture_x.timeout, PLOOPY_MSGESTURE_TIMEOUT);
                    }
                    else{
                        dprintf("MsGesture X new\n");
                        msgesture_x.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, expire_timeout_x_de, NULL );
                    }
                    */
                #else // MSGESTURE_MODE_DE
                    msgesture_x.timer = timer_read();
                #endif // MSGESTURE_MODE_DE
            }
        }

        if(
            abs(msgesture_y.accum) > abs(msgesture_x.accum)
            && abs(msgesture_y.accum) > PLOOPY_MSGESTURE_THRESHOLD
        ){
            start_debounce();
            if(
                (
                    abs(msgesture_y.accum) > PLOOPY_MSGESTURE_THRESHOLD
                    && msgesture_y.count==0
                ) ||
                (
                    msgesture_y.accum > PLOOPY_MSGESTURE_THRESHOLD
                    && !msgesture_y.stage
                ) ||
                (
                    msgesture_y.accum < -PLOOPY_MSGESTURE_THRESHOLD
                    && msgesture_y.stage
                )
            ) {
                msgesture_y.count++;
                dprintf(
                    "==Y==> C:%d,%d  acc:%d,%d (%d,%d) \n",
                    msgesture_x.count, msgesture_y.count,
                    msgesture_x.accum, msgesture_y.accum,
                    abs(msgesture_x.accum), abs(msgesture_y.accum)
                );
                msgesture_y.stage = msgesture_y.accum > PLOOPY_MSGESTURE_THRESHOLD;
                debounce_active = true;

                #if defined(MSGESTURE_MODE_DE)
                    dprintf("MsGesture Y start timeout\n");
                    cancel_deferred_exec(msgesture_y.timeout);
                    msgesture_y.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, expire_timeout_y_de, NULL );
                    /*
                    // This is logically better but takes up 80 bytes. I hate this.
                    if(msgesture_y.timeout != INVALID_DEFERRED_TOKEN){
                        dprintf("MsGesture Y extend\n");
                        extend_deferred_exec(msgesture_y.timeout, PLOOPY_MSGESTURE_TIMEOUT);
                    }
                    else{
                        dprintf("MsGesture Y new\n");
                        msgesture_y.timeout = defer_exec( PLOOPY_MSGESTURE_TIMEOUT, expire_timeout_y_de, NULL );
                    }
                    */
                #else // MSGESTURE_MODE_DE
                    msgesture_y.timer = timer_read();
                #endif // MSGESTURE_MODE_DE


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
