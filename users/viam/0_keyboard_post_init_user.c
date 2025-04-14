#if defined(BETTER_DRAGSCROLL)
    #include "better_dragscroll.h"
#endif // defined(BETTER_DRAGSCROLL)
#if ( defined(BETTER_DRAGSCROLL) && ((defined(BETTER_DRAGSCROLL_SCRLK_ENABLE)) || defined(BETTER_DRAGSCROLL_CAPLK_ENABLE) || defined(BETTER_DRAGSCROLL_NUMLK_ENABLE) || (defined(VIA_ENABLE) && defined(PLOOPY_VIAMENUS))) || defined(COMMUNITY_MODULE_BETTER_DRAGSCROLL_ENABLED))
    void keyboard_post_init_user(void) {
      led_initialize_better_dragscroll();
    }
#endif //
