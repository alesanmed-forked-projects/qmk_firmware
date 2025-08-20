#include QMK_KEYBOARD_H
#include "keymap_spanish.h"

enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _QWERTY,
    _GAME,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_GAME,
    KC_PRVWD,
    KC_NXTWD,
    KC_LSTRT,
    KC_LEND,
    KC_DLINE,
    KC_TAB_N,
    KC_TAB_P,
    KC_DSK_L,
    KC_DSK_R,
    KC_SSHOT,
    KC_LOCK,
    KC_EMOJI,
    KC_OS_TOG,
    KC_ANG_PAIR
};

typedef enum { OS_WIN = 0, OS_MAC = 1, OS_LIN = 2 } os_mode_t;

typedef union {
    uint32_t raw;
    struct { uint8_t os_mode : 2; };
} user_config_t;

static user_config_t user_config;
static uint32_t oled_idle_timer = 0;
static bool oled_sleeping = false;

void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.os_mode = OS_WIN; // por defecto
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    oled_set_brightness(120);
    user_config.raw = eeconfig_read_user();
}

static void os_next_and_save(void) {
    user_config.os_mode = (user_config.os_mode + 1) % 3;
    eeconfig_update_user(user_config.raw);
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY (ES)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  º   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   Ñ  |   ´  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |Sft(  |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   -  | )Sft |
 * `--------------------------------- /      \       /     \      \-----------------------------------------'
 *       | LGUI | Alt[ |Ctl/{|Desk/<| /Enter /      \ Space \  | Desk/>| Ctl/}| Alt/]| RGUI |
 *       |      |      |     |      |/  (LT)/        \ (LT)  \ |       |      |      |      |
 *       `--------------------------'------'          '-------''----------------------------'
 *  Legend: Sft( / )Sft = Space-cadet Shift (tap: ( / ), hold: Shift)
 *          Alt[ / Ctl{ / }Ctl / ]Alt = Space-cadet pairs (tap: [ { } ], hold: Alt/Ctrl)
 *          Desk< / Desk> = OS-aware desktop left/right
 */

[_QWERTY] = LAYOUT(
  ES_MORD,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  ES_QUOT,
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, ES_NTIL,  ES_ACUT,
  SC_LSPO,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,     XXXXXXX,KC_N,    KC_M, KC_COMM,  KC_DOT, ES_MINS,  SC_RSPC,
     KC_LGUI,SC_LAPO,SC_LCPO,KC_DSK_L,LT(_LOWER, KC_ENT),     LT(_RAISE, KC_SPC),  KC_DSK_R, SC_RCPC, SC_RAPC, KC_RGUI
),
/*
 * GAME (ES)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  º   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   Ñ  |   ´  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |Shift |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  | QWRT |Shift |
 * `-------------------------------------------/       /      \   ------------------------------------------'
 *       | LGUI | LAlt | LCtl | LOWER | /Space/        \ Enter \  | RAISE | RCtl | RAlt | RGUI |
 *       |      |      |      |       |/     /          \       \ |       |      |      |      |
 *       `----------------------------------'            '-------''---------------------------'
 *  Notes: no tap-hold here (plain Shifts). Space ↔ Enter swapped for gaming.
 *         QWRT = switch back to QWERTY base (persistent).
 */
[_GAME] = LAYOUT(
  ES_MORD,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  ES_QUOT,
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, ES_NTIL,  ES_ACUT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,    XXXXXXX,KC_N,    KC_M, KC_COMM,  KC_DOT, KC_QWERTY,  KC_RSFT,
                    KC_LGUI,KC_LALT,KC_LCTL, MO(_LOWER), KC_SPC,      KC_ENT,  MO(_RAISE), KC_RCTL, KC_RALT, KC_RGUI
),
/*
 * LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  |  F10 |  F11 |  ¡   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | F12  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   `  |  +   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  !   |  @   |  #   |  $   |  %   |-------.    ,-------|  ^   |  &   |  *   |  (   |  ´   |  ç   |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |  <>  |  <   |  >   |  {   |  }   |-------|    |-------|  [   |  ]   |  ;   |  :   |  \   |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *          (<> macro types “<>” and places the caret inside)
 */
[_LOWER] = LAYOUT(
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  ES_IEXL,
  KC_F12,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    ES_GRV,  ES_PLUS,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, ES_ACUT, ES_CCED,
  _______, KC_ANG_PAIR,ES_LABK,ES_RABK,ES_LCBR,ES_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                       _______, _______, _______, _______, _______,       _______, MO(_RAISE), _______, _______, _______
),
/*
 * RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      | Ins  | Pscr | Menu |      |      |                   | PgUp | ◀Wrd|  ↑  | Wrd▶| DLine|  Del |
 * |------+------+------+------+------+------|                   |------+------+------+------+------+------|
 * |      | LAlt | LCtl | LShf |      | Caps |                   | PgDn |  ←  |  ↓  |  →  |  Del | Bspc |
 * |------+------+------+------+------+------|                   |------+------+------+------+------+------|
 * |      | Tabs-| Tabs+| Shot | Lock |      |-------.   ,-------|      | Home|      | End | Emoji|      |
 * `-----------------------------------------'       '---'       '----------------------------------------'
 *  (Shot / Lock / Emoji are OS-aware; Tabs± = prev/next tab)
 */
[_RAISE] = LAYOUT(
  _______, _______ , _______ , _______ , _______ , _______,                           _______,  _______  , _______,  _______ ,  _______ ,_______,
  _______,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, XXXXXXX,                        KC_PGUP, KC_PRVWD,   KC_UP, KC_NXTWD,KC_DLINE, KC_DEL,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______,KC_TAB_P, KC_TAB_N, KC_SSHOT, KC_LOCK, XXXXXXX,  _______,       _______,  XXXXXXX, KC_LSTRT, XXXXXXX, KC_LEND, KC_EMOJI, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/*
 * ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | BOOT |      |QWERTY|      | CGTG |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |  OS  |      |      | GAME |-------.    ,-------|      | Vol- | Mute | Vol+ |      |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |      |      |CapsW |      |      |-------|    |-------|      | Prev | Play | Next |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *  Notes: QWERTY/GAME switch base (persistent). OS = Win/macOS/GNOME. CGTG = Ctrl⇄GUI. CapsW = Caps Word.
 */
[_ADJUST] = LAYOUT(
    XXXXXXX , XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    QK_BOOT  , XXXXXXX,KC_QWERTY,  XXXXXXX  ,CG_TOGG,XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX , XXXXXXX, KC_OS_TOG, XXXXXXX,    XXXXXXX,  KC_GAME,                     XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
    XXXXXXX , XXXXXXX, XXXXXXX, CW_TOGG,    XXXXXXX,  XXXXXXX, KC_MUTE,     XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                    _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY]  = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),           ENCODER_CCW_CW(KC_TAB_P, KC_TAB_N) },
    [_LOWER]   = { ENCODER_CCW_CW(C(KC_MINS), C(KC_EQL)),      ENCODER_CCW_CW(KC_WH_D, KC_WH_U)   },
    [_RAISE]   = { ENCODER_CCW_CW(KC_DSK_L, KC_DSK_R),         ENCODER_CCW_CW(KC_PGDN, KC_PGUP)   },
    [_ADJUST]  = { ENCODER_CCW_CW(KC_BRID, KC_BRIU),           ENCODER_CCW_CW(KC_MPRV, KC_MNXT)   },
    [_GAME]    = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },

};
#endif

#ifdef OLED_ENABLE

static void oled_print_os(void) {
    oled_write_ln_P(PSTR("OS"), false);
    oled_write_P(PSTR("W"), user_config.os_mode == OS_WIN);
    oled_write_P(PSTR("/"), false);
    oled_write_P(PSTR("M"), user_config.os_mode == OS_MAC);
    oled_write_P(PSTR("/"), false);
    oled_write_ln_P(PSTR("L"), user_config.os_mode == OS_LIN);
}
static void oled_print_layer(void) {
    bool lower  = layer_state_is(_LOWER);
    bool raise  = layer_state_is(_RAISE);
    bool adjust = layer_state_is(_ADJUST);
    
    oled_write_ln_P(PSTR("Layer"), false);

    bool base_is_qwerty = (default_layer_state & (1UL << _QWERTY));
    
    oled_write_P(base_is_qwerty ? PSTR("B") : PSTR("G"), !lower && !raise && !adjust);
    oled_write_P(PSTR(" "), false);
    oled_write_ln_P(PSTR("L"), !adjust && lower);
    oled_write_P(PSTR("R"), !adjust && raise);
    oled_write_P(PSTR(" "), false);
    oled_write_ln_P(PSTR("A"), adjust);
    oled_write_ln_P(PSTR(""), false);
}
static void oled_print_flags(void) {
    led_t led = host_keyboard_led_state();
    bool cw_on = is_caps_word_on();
    oled_write_ln_P(PSTR("Flags"), false);
    oled_write_P(PSTR("C"), led.caps_lock);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("W"), cw_on);
    oled_write_P(PSTR(" "), false);

    bool game_on = (default_layer_state & (1UL << _GAME));
    oled_write_ln_P(PSTR("G"), game_on);
}

static void render_left_hud(void) {
    oled_clear();
    oled_set_cursor(0,0);
    oled_print_os();
    oled_print_layer();
    oled_print_flags();
}

static void render_right_help(void) {
    uint8_t layer = get_highest_layer(layer_state);
    oled_clear();
    oled_set_cursor(0,0);
    switch (layer) {
        case _QWERTY:
            oled_write_ln_P(PSTR("BASE"), false);
            oled_write_ln_P(PSTR(""), false);
            oled_write_ln_P(PSTR("Vol"), false);
            oled_write_ln_P(PSTR("Tabs"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("LOWER"), false);
            oled_write_ln_P(PSTR(""), false);
            oled_write_ln_P(PSTR("Zoom"), false);
            oled_write_ln_P(PSTR("Scrll"), false);
            oled_write_ln_P(PSTR("<{}[]"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("RAISE"), false);
            oled_write_ln_P(PSTR(""), false);
            oled_write_ln_P(PSTR("Desks"), false);
            oled_write_ln_P(PSTR("PgDn/PgUp"), false);
            oled_write_ln_P(PSTR("Shot Lock :)"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("ADJST"), false);
            oled_write_ln_P(PSTR(""), false);
            oled_write_ln_P(PSTR("W/M/L"), false);
            oled_write_ln_P(PSTR("Caps"), false);
            oled_write_ln_P(PSTR("CG Tg"), false);
            oled_write_ln_P(PSTR("Media"), false);
            break;
        default:
            oled_write_ln_P(PSTR("GAME"), false);
            oled_write_ln_P(PSTR("TIME"), false);
            break;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (oled_sleeping) return false;

    if (is_keyboard_master()) render_left_hud();
    else render_right_help();
    return false;
}

void matrix_scan_user(void) {
    if (is_keyboard_master()) {
        if (timer_elapsed32(oled_idle_timer) > 5000) {
            oled_off();
            oled_sleeping = true;
        }
    }
}

#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_GAME:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_GAME);
            }
            return false;
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
            }
            break;
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
        case KC_DLINE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
            }
            break;
        case KC_OS_TOG: if (record->event.pressed) os_next_and_save(); return false;
        case KC_TAB_N: if (record->event.pressed) tap_code16(C(KC_TAB)); return false;
        case KC_TAB_P: if (record->event.pressed) tap_code16(S(C(KC_TAB))); return false;

        case KC_DSK_L:
            if (record->event.pressed) {
                switch (user_config.os_mode) {
                case OS_MAC: tap_code16(C(KC_LEFT)); break;         // Ctrl+← (Spaces)
                case OS_WIN: tap_code16(G(C(KC_LEFT))); break;      // Win+Ctrl+←
                case OS_LIN: tap_code16(G(KC_PGUP)); break;         // GNOME: Super+PgUp
                }
            }
            return false;
        case KC_DSK_R:
            if (record->event.pressed) {
                switch (user_config.os_mode) {
                case OS_MAC: tap_code16(C(KC_RIGHT)); break;        // Ctrl+→
                case OS_WIN: tap_code16(G(C(KC_RIGHT))); break;     // Win+Ctrl+→
                case OS_LIN: tap_code16(G(KC_PGDN)); break;         // GNOME: Super+PgDn
                }
            }
            return false;
        case KC_SSHOT:
            if (record->event.pressed) {
                switch (user_config.os_mode) {
                case OS_MAC: tap_code16(S(G(KC_4))); break;         // Cmd+Shift+4
                case OS_WIN: tap_code16(S(G(KC_S))); break;         // Win+Shift+S
                case OS_LIN: tap_code(KC_PSCR); break;              // GNOME overlay
                }
            }
            return false;
        case KC_LOCK:
            if (record->event.pressed) {
                switch (user_config.os_mode) {
                case OS_MAC: tap_code16(C(G(KC_Q))); break;         // Ctrl+Cmd+Q
                case OS_WIN: tap_code16(G(KC_L)); break;            // Win+L
                case OS_LIN: tap_code16(G(KC_L)); break;            // Super+L
                }
            }
            return false;
        case KC_EMOJI:
            if (record->event.pressed) {
                switch (user_config.os_mode) {
                case OS_MAC: tap_code16(C(G(KC_SPC))); break;       // Ctrl+Cmd+Space
                case OS_WIN: tap_code16(G(KC_DOT)); break;          // Win+.
                case OS_LIN: tap_code16(C(KC_DOT)); break;          // GNOME (ajústalo si usas otro)
                }
            }
            return false;
        case KC_ANG_PAIR:
            if (record->event.pressed) {
                tap_code16(ES_LABK);  // <
                tap_code16(ES_RABK);  // >
                tap_code(KC_LEFT);
            }
            return false;
    }

    if (record->event.pressed) {
        oled_on();
        oled_sleeping = false;
        oled_idle_timer = timer_read32();
    }

    return true;
}

#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_UNDS: // _
        case KC_MINS: // -
        case KC_BSPC:
        case KC_1 ... KC_0: // numbers 1–0
            return true;    // keep caps word enabled
        default:
            return false;   // finish caps word
    }
}
#endif

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_LOWER, KC_ENT):
        case LT(_RAISE, KC_SPC):
            return 190;
    }
    return TAPPING_TERM;
}
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_LOWER, KC_ENT):
        case LT(_RAISE, KC_SPC):
            return true;
    }
    return false;
}