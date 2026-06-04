void keyboard_post_init_user_ploopyknob(void);
void ploopyknob_config_set_value( uint8_t *data );
void ploopyknob_config_get_value( uint8_t *data );
void ploopyknob_config_save ( void );

enum via_ploopyknob_value {
    id_ploopyknob_act_default = 1,
    id_ploopyknob_act_numlk,
    id_ploopyknob_act_capslk,
    id_ploopyknob_act_scrlk,
    id_ploopyknob_act_compose,
    id_ploopyknob_act_kana,
    id_ploopyknob_rev_default = 11,
    id_ploopyknob_rev_numlk,
    id_ploopyknob_rev_capslk,
    id_ploopyknob_rev_scrlk,
    id_ploopyknob_rev_compose,
    id_ploopyknob_rev_kana,
};

enum knob_action_types {
    KNOB_ACT_NOTHING = 0,
    KNOB_ACT_SCROLL_V,
    KNOB_ACT_SCROLL_H,
    KNOB_ACT_VOLUME,
    KNOB_ACT_ARROWKEYS_V,
    KNOB_ACT_ARROWKEYS_H,
    KNOB_ACT_WASD_V,
    KNOB_ACT_WASD_H,
    KNOB_ACT_BRIGHTNESS,
};

typedef struct PACKED {
    uint8_t     action_default;
    uint8_t     action_num_lock;
    uint8_t     action_caps_lock;
    uint8_t     action_scroll_lock;
    uint8_t     action_compose;
    uint8_t     action_kana;
    bool        reverse_default;
    bool        reverse_num_lock;
    bool        reverse_caps_lock;
    bool        reverse_scroll_lock;
    bool        reverse_compose;
    bool        reverse_kana;
} via_ploopyknob_config;
via_ploopyknob_config ploopyknob_via_config;

static const via_ploopyknob_config ploopyknob_via_config_default = {
    .action_default         = KNOB_ACT_SCROLL_V,
    .action_num_lock        = KNOB_ACT_NOTHING,
    .action_caps_lock       = KNOB_ACT_VOLUME,
    .action_scroll_lock     = KNOB_ACT_SCROLL_H,
    .action_compose         = KNOB_ACT_NOTHING,
    .action_kana            = KNOB_ACT_NOTHING,
    .reverse_default        = false,
    .reverse_num_lock       = false,
    .reverse_caps_lock      = false,
    .reverse_scroll_lock    = false,
    .reverse_compose        = false,
    .reverse_kana           = false,
};
