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
#include "repeat_key.h"

enum charybdis_keymap_layers { LAYER_BASE = 0, LAYER_BASEQWERTY, LAYER_LOWER, LAYER_RAISE, LAYER_POINTER, LAYER_ADJUST };

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

// タップダンスの定義用enum
enum { TD_POINTER_LOCK = 0 };

// ポインターレイヤーが固定されているかどうかのフラグ
static bool is_pointer_locked = false;

// Tap Dance: 1回タップでKC_LBRC、2回タップでレイヤーロック切替
void td_pointer_lock_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // 単打: 「押して離す」まで自動でやる
        tap_code16(KC_LBRC);
    } else if (state->count >= 2) {
        // ダブルタップ以上: 固定モードをトグル
        is_pointer_locked = !is_pointer_locked;

        if (is_pointer_locked) {
            // 固定ON
            layer_on(LAYER_POINTER);
        } else {
            // 固定OFF
            layer_off(LAYER_POINTER);
            auto_pointer_layer_timer = 0;
        }
    }
}



// Tap Danceアクションの登録
tap_dance_action_t tap_dance_actions[] = {
    [TD_POINTER_LOCK] = ACTION_TAP_DANCE_FN(td_pointer_lock_finished),
};

/** \brief Automatically enable sniping-mode on the pointer layer. */

#define LOWER LT(LAYER_LOWER, KC_INT5)
#define RAISE LT(LAYER_RAISE, KC_INT4)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)
#define CTL_REP MT(MOD_LCTL, KC_0)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮                ╭──────────────────────────────────────────────────────╮
        KC_TAB, KC_QUOT,    KC_Q,    KC_U,    KC_E, KC_COMM,                    KC_L,    KC_D,    KC_R,    KC_Y,    KC_P, TD(TD_POINTER_LOCK),
  // ├──────────────────────────────────────────────────────┤                ├──────────────────────────────────────────────────────┤
       KC_BSPC, KC_MINS,    KC_I,    KC_A,    KC_O,  KC_DOT,                    KC_G,    KC_T,    KC_N,    KC_S,    KC_H,    KC_F,
  // ├──────────────────────────────────────────────────────┤                ├──────────────────────────────────────────────────────┤
 OSM(MOD_LSFT),    KC_Z,    KC_X,    KC_C,    KC_V, MT(MOD_LGUI,KC_SCLN),       KC_W,    KC_K,    KC_M,    KC_J,    KC_B, MT(MOD_LALT | MOD_RALT,KC_ESC),
  // ╰──────────────────────────────────────────────────────┤                ├──────────────────────────────────────────────────────╯
                                   KC_SPC, CTL_REP,   LOWER,                      RAISE,  KC_ENT
  //                            ╰───────────────────────────╯                ╰──────────────────╯
  ),

  [LAYER_BASEQWERTY] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, TD(TD_POINTER_LOCK),
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            KC_BSPC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_RSFT,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      OSM(MOD_LSFT),  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, MT(MOD_LALT | MOD_RALT,KC_ESC), 
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                      KC_SPC, CTL_REP,   LOWER,      RAISE,  KC_ENT
    //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, KC_PAST,   KC_P7,   KC_P8,   KC_P9, KC_PSLS,    XXXXXXX, KC_HOME,   KC_UP,  KC_END, KC_PGUP, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_BSPC, KC_PMNS,   KC_P4,   KC_P5,   KC_P6, KC_PDOT,    XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,  KC_DEL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,   KC_P0,   KC_P1,   KC_P2,   KC_P3, KC_PPLS,    KC_BTN4, KC_BTN1, KC_BTN2, KC_BTN3, KC_BTN5, KC_BTN5,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
  
  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, S(KC_7), KC_RBRC, KC_BSLS, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_BSPC,S(KC_EQL),S(KC_2), S(KC_8), S(KC_9), XXXXXXX,    KC_INT1, S(KC_1), S(KC_SLSH), XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, XXXXXXX, S(KC_3),S(KC_RBRC),S(KC_BSLS), XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,    _______, S(KC_INS), C(KC_INS), KC_BTN3, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,    KC_BTN4, KC_BTN1, KC_BTN2, DRGSCRL, KC_BTN5, KC_BSPC,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_ADJUST] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
         DF(0), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   DF(1),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, LSG(KC_S), XXXXXXX,  XXXXXXX, G(KC_H), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
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

#define YBOOST_MIN 0.05f // 低速域での追加ブースト（1.05倍）
#define YBOOST_MAX 0.20f // 高速域での最大ブースト（1.20倍）

// ===============================================

static inline int8_t clip2int8(int16_t v) {
    return (v < -127) ? -127 : (v > 127) ? 127 : (int8_t)v;
}

static void apply_trackball_acceleration(report_mouse_t *m) {
    // 1. 元の値を float に変換
    float raw_x = (float)m->x;
    float raw_y = (float)m->y;

    // 2. 速度計算（ユークリッド距離）
    float speed = sqrtf(raw_x * raw_x + raw_y * raw_y);

    // 3. 非線形加速カーブの計算
    float scale = 1.0f;

    if (speed > ACCEL_V1) {
        if (speed >= ACCEL_V2) {
            scale = ACCEL_MAX_SCALE;
        } else {
            float t     = (speed - ACCEL_V1) / (ACCEL_V2 - ACCEL_V1);
            float curve = t * (0.6f + 0.4f * t); // ≒ t^1.3
            scale       = 1.0f + curve * (ACCEL_MAX_SCALE - 1.0f);
        }
    }

    // 4. 速度依存 Y ブーストの計算
    float boost_y = 1.0f;

    if (speed > 0.1f) {
        // 0〜1に正規化 (遅 → 速)
        float t = fminf(speed / ACCEL_V2, 1.0f);

        // YBOOST_MIN 〜 YBOOST_MAX に線形補間
        boost_y = 1.0f + YBOOST_MIN + (YBOOST_MAX - YBOOST_MIN) * t;
    }

    // 5. XY に適用（X は補正なし、Y のみ速度依存）
    float adj_x = raw_x * scale;
    float adj_y = raw_y * scale * boost_y;

    // 6. int8 にクリップして反映
    m->x = clip2int8((int16_t)adj_x);
    m->y = clip2int8((int16_t)adj_y);
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
    // 固定モード中はタイムアウトチェックを行わない（レイヤーを維持する）
    if (is_pointer_locked) {
        return;
    }

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
    if (is_pointer_locked && record->event.pressed) {
        switch (keycode) {
            // ▼ 以下のキーはロックを解除しない（マウス操作に必要なため）
            case KC_BTN1 ... KC_BTN5: // マウスボタン
            case DRGSCRL:             // ドラッグスクロール
            case KC_LSFT:             // Shift (Shiftクリック用)
            case KC_LCTL:             // Ctrl (Ctrlクリック用)
            case KC_LALT:             // Alt
            case KC_LGUI:             // GUI
            case OSM(MOD_LSFT):       // あなたのOSM Shift
            case TD(TD_POINTER_LOCK): // ロック解除キー自体（TapDance側で処理させるためここでは無視）
            case C(KC_C):        // コピー
            case C(KC_V):        // ペースト
            case C(KC_INS):      // コピー（別キーコード）
            case S(KC_INS):      // ペースト（別キーコード）
            case KC_ENT:        // Enter
            case KC_BSPC:     // Backspace
                break;                // 何もしない（ロック維持）

            // ▼ それ以外のキー（Q, A, Space, Enterなど）が押されたらロック解除！
            default:
                is_pointer_locked = false;    // フラグを下ろす
                layer_off(LAYER_POINTER);     // レイヤーを消す
                auto_pointer_layer_timer = 0; // 自動タイマーもリセット
                // ここで return false しない！
                // (キー入力自体はそのまま通して、文字を入力させる)
                break;
        }
    }

    if (keycode == CTL_REP) {
        // タップとして扱われた場合だけ Repeat を発火
        if (record->tap.count) {
            // press / release の両方とも Repeat Key にイベントをそのまま渡す
            repeat_key_invoke(&record->event);

            // その代わり、元の MT(MOD_LCTL, KC_0) のタップ処理(KC_0送信)は完全に殺す
            return false;
        }

        // tap ではなく hold 判定のときは、通常の Mod-Tap に任せる（Ctrl として動く）
        return true;
    }

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

bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    switch (keycode) {
        case CTL_REP:
            // Ctrl/Repeat キー自体は「最後に押されたキー」にしない
            return false;
    }
    return true; // 他のキーはすべて通常どおり Repeat 対象にする
}

// --- Key Overrides ------------------------------------------------------

const key_override_t ctrl_f_to_o = ko_make_with_layers(MOD_MASK_CTRL, // 条件：Ctrl が押されたとき
                                                       KC_F,          // from: F
                                                       LCTL(KC_O),    // to: Ctrl+O（強制）
                                                       1 << 0         // layer_mask: レイヤー 0
);

const key_override_t ctrl_o_to_f = ko_make_with_layers(MOD_MASK_CTRL, KC_O, LCTL(KC_F),
                                                       1 << 0 // layer 0 限定
);

const key_override_t *key_overrides[] = {&ctrl_f_to_o, &ctrl_o_to_f, NULL};
