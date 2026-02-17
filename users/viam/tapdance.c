#if defined(TAP_DANCE_ENABLE)

  #if defined(BETTER_DRAGSCROLL_TAPDANCE)
    #include "better_dragscroll.h"
    void dragscrolltd_fin(tap_dance_state_t *state, void *user_data){
      dprintf("fin; pressed:%d count:%d \n", state->pressed, state->count);
      if(state->count >= 2){
        better_dragscroll_toggle(true);
      }
      else{
        better_dragscroll_momentary(true);
      }
    }
    void dragscrolltd_rst(tap_dance_state_t *state, void *user_data){
      if(state->count < 2){
        better_dragscroll_momentary(false);
      }
    }
  #endif // BETTER_DRAGSCROLL_TAPDANCE

  tap_dance_action_t tap_dance_actions[] = {
    #if defined(BETTER_DRAGSCROLL_TAPDANCE) || defined(BETTER_DRAGSCROLL_TAPDANCE)
      [TD_DRAGSCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dragscrolltd_fin, dragscrolltd_rst),
    #endif // BETTER_DRAGSCROLL_TAPDANCE

  };
#endif // TAP_DANCE_ENABLE
