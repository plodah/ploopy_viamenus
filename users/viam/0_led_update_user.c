#if defined(BETTER_DRAGSCROLL)
    #include "better_dragscroll.h"
#endif // defined(BETTER_DRAGSCROLL)
#if ( defined(BETTER_DRAGSCROLL) && (defined(BETTER_DRAGSCROLL_SCRLK_ENABLE) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))) || defined(COMMUNITY_MODULE_BETTER_DRAGSCROLL_ENABLED))
    bool led_update_user(led_t led_state) {
      if(led_update_better_dragscroll(led_state)){
        return true;
      }
      return false;
    }
#endif //
