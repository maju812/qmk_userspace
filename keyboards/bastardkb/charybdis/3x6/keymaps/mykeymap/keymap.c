/**
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *k
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include <math.h>

enum charybdis_keymap_layers { LAYER_BASE = 0, LAYER_BASEQWERTY, LAYER_LOWER, LAYER_RAISE, LAYER_POINTER, LAYER_ADJUST };

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define CHARYBDIS_MINIMUM_DEFAULT_DPI 1600

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 900
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 1
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define LOWER LT(LAYER_LOWER, KC_INT5)
#define RAISE LT(LAYER_RAISE, KC_INT4)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_TAB, KC_QUOT,    KC_Q,    KC_U,    KC_E, KC_COMM,       KC_L,    KC_D,    KC_R,    KC_Y,    KC_P, KC_LBRC,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_BSPC, KC_MINS,    KC_I,    KC_A,    KC_O,  KC_DOT,       KC_G,    KC_T,    KC_N,    KC_S,    KC_H,    KC_F,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V, KC_SCLN,       KC_W,    KC_K,    KC_M,    KC_J,    KC_B, KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_SPC, KC_LCTL,   LOWER,      RAISE,  KC_ENT
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_BASEQWERTY] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
           KC_BSPC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_RSFT,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
           KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT, 
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                       KC_SPC, KC_LCTL,   LOWER,      RAISE,  KC_ENT
    //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, RGB_TOG,    KC_7,    KC_8,    KC_9, XXXXXXX,    KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_BSPC, KC_PMNS,    KC_4,    KC_5,    KC_6, KC_PDOT,    KC_PPLS,    KC_4,    KC_5,    KC_6, KC_PMNS, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX,    KC_0,    KC_1,    KC_2,    KC_3, KC_PPLS,    KC_PAST,    KC_1,    KC_2,    KC_3, KC_PSLS, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, XXXXXXX, _______,    XXXXXXX, _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
  
  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, KC_VOLU, KC_MUTE, KC_VOLD, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, XXXXXXX,    _______, XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_BTN1, KC_BTN2, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,    XXXXXXX, KC_BTN1, KC_BTN2, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_BTN2, KC_BTN1, KC_BTN3,    KC_BTN3, KC_BTN1
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_ADJUST] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, RGB_M_P, RGB_M_B, RGB_HUD, RGB_SAD, RGB_VAD,    KC_PSCR, KC_SCRL, KC_PAUS, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, XXXXXXX, _______,    _______, XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

};
// clang-format on

// ============ トラックボール加速設定 ============

// 加速カーブのパラメータ
#define ACCEL_V1 1.2f         // 低速域の閾値
#define ACCEL_V2 8.0f         // 高速域への到達閾値
#define ACCEL_MAX_SCALE 12.0f // 最大加速倍率

// 軸ごとの感度補正（1.0 = 補正なし）
// 親指は縦移動が苦手なので、Y軸を少しブーストすると快適になります
#define SENSITIVITY_X 1.0f
#define SENSITIVITY_Y 1.1f

// ===============================================

static inline int8_t clip2int8(int16_t v) {
    return (v < -127) ? -127 : (v > 127) ? 127 : (int8_t)v;
}

static void apply_trackball_acceleration(report_mouse_t *m) {
    // 1. まず軸ごとの基本感度を適用（親指補正）
    float raw_x = (float)m->x * SENSITIVITY_X;
    float raw_y = (float)m->y * SENSITIVITY_Y;

    // 2. 正確な移動速度（ユークリッド距離）を計算
    float speed = sqrtf(raw_x * raw_x + raw_y * raw_y);

    // 3. 加速倍率の計算
    float scale = 1.0f;

    if (speed > ACCEL_V1) {
        if (speed >= ACCEL_V2) {
            scale = ACCEL_MAX_SCALE;
        } else {
            // 0〜1 に正規化
            float t = (speed - ACCEL_V1) / (ACCEL_V2 - ACCEL_V1);

            // 擬似 t^1.3 カーブ (0.6*t + 0.4*t^2)
            float curve = t * (0.6f + 0.4f * t);

            scale = 1.0f + curve * (ACCEL_MAX_SCALE - 1.0f);
        }
    }

    // 4. 最終的な値を適用
    m->x = clip2int8((int16_t)(raw_x * scale));
    m->y = clip2int8((int16_t)(raw_y * scale));
}

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // ① まずトラックボールの移動に なめらか加速 をかける
    apply_trackball_acceleration(&mouse_report);

    // ② そのあと、従来どおり自動ポインタレイヤー判定
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
        }
        auto_pointer_layer_timer = timer_read();
    }

    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#endif // POINTING_DEVICE_ENABLE

// LOWER + RAISE で LAYER_ADJUST を有効にする tri-layer 設定
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, LAYER_LOWER, LAYER_RAISE, LAYER_ADJUST);
}

// ダブルタップ判定用のタイマー変数
static uint16_t lshift_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // --- アルティメット左Shift (OSM + Caps Word) ---
    switch (keycode) {
        case OSM(MOD_LSFT):
            if (record->event.pressed) {
                // Caps Wordが既にONになっているか確認
                if (is_caps_word_on()) {
                    caps_word_off(); // 明示的にOFFにする
                    return false;    // 重要：ここで処理を打ち切り、OSMを発動させない
                }
                // ダブルタップ判定
                if (lshift_timer && timer_elapsed(lshift_timer) < TAPPING_TERM) {
                    // 1回目のタップで有効になったOne Shot Shiftをキャンセル
                    del_oneshot_mods(MOD_MASK_SHIFT);

                    // Caps Word を発動
                    caps_word_toggle();

                    lshift_timer = 0;
                    return false; // 2回目の入力はOSMとして処理させない
                }
                // 1回目のタップ時刻を記録
                lshift_timer = timer_read();
            }
            return true; // 通常のOSM処理へ（長押しならShift、単打ならOne Shot）

        // --- 他のキーが押されたらダブルタップ判定リセット ---
        default:
            if (record->event.pressed) {
                lshift_timer = 0;
            }
            break;
    }

    // Achordion等の処理があればここに
    // if (!process_achordion(keycode, record)) { return false; }

    return true;
}

// --- キーオーバーライド定義 ---

// ko_make_with_layers(修飾キー, 元キー, 送るキー, レイヤーマスク) を使います

// 1. LAYER_BASE (レイヤー0) の時だけ、Ctrl+O → F
const key_override_t ctrl_o_to_f = ko_make_with_layers(MOD_MASK_CTRL, KC_O, KC_F,
                                                       (1 << LAYER_BASE) // レイヤー0のみ有効
);

// 2. LAYER_BASE (レイヤー0) の時だけ、Ctrl+F → O
const key_override_t ctrl_f_to_o = ko_make_with_layers(MOD_MASK_CTRL, KC_F, KC_O,
                                                       (1 << LAYER_BASE) // レイヤー0のみ有効
);

// 登録用配列
const key_override_t **key_overrides = (const key_override_t *[]){&ctrl_o_to_f, &ctrl_f_to_o, NULL};
