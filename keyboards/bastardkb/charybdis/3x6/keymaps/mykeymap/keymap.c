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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
    LAYER_ADJUST, // 新しいレイヤーを追加
};

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
       KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V, KC_SCLN,       KC_W,    KC_K,    KC_M,    KC_J,    KC_B, KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_SPC, KC_LSFT,   LOWER,      RAISE,  KC_ENT
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

// ==== トラックボール加速用のヘルパー関数 ====

static inline int8_t clip2int8(int16_t v) {
    return (v < -127) ? -127 : (v > 127) ? 127 : (int8_t)v;
}

// ==== トラックボール用 なめらか加速関数 ====
static void apply_trackball_acceleration(report_mouse_t *m) {
    int16_t x = m->x;
    int16_t y = m->y;

    int16_t ax    = (x >= 0) ? x : -x;
    int16_t ay    = (y >= 0) ? y : -y;
    int16_t speed = (ax > ay) ? ax : ay; // max(|x|, |y|)

    // いま使っているパラメータをそのまま利用
    const float v1        = 1.2f;  // ここまではほぼ等倍
    const float v2        = 8.0f;  // ここまでの間で加速していく
    const float max_scale = 12.0f; // 最大倍率

    float scale = 1.0f;

    if (speed <= v1) {
        // ごく小さい動き → スナイプ用で等倍
        scale = 1.0f;
    } else if (speed >= v2) {
        // 十分速い → 常に max_scale
        scale = max_scale;
    } else {
        // 0〜1 に正規化
        float t = (float)(speed - v1) / (float)(v2 - v1); // 0〜1

        // 擬似 t^1.3 カーブ
        // e ≒ t^1.3 に近い形になるように、
        // 一次と二次の線形結合 0.6*t + 0.4*t^2 を使用
        float e = t * (0.6f + 0.4f * t);

        // 1.0〜max_scale の間でスケール
        scale = 1.0f + e * (max_scale - 1.0f);
    }

    float fx = (float)x * scale;
    float fy = (float)y * scale;

    m->x = clip2int8((int16_t)fx);
    m->y = clip2int8((int16_t)fy);
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
    // ① 左シフトキーの処理
    if (keycode == KC_LSFT) {
        if (record->event.pressed) {
            // 前回のShift入力から TAPPING_TERM (デフォルト200ms) 以内ならダブルタップと判定
            if (lshift_timer && timer_elapsed(lshift_timer) < TAPPING_TERM) {
                caps_word_toggle(); // Caps Word を切り替え
                lshift_timer = 0;   // タイマーをリセット
                return false;       // 2回目のShift入力自体はPCに送らない（誤操作防止）
            }
            // タイマーを開始（1回目のタップ時刻を記録）
            lshift_timer = timer_read();
        }
        return true; // 1回目のShift入力は通常通りPCに送る（遅延なし）
    }

    // ② 他のキーが押された場合の処理（誤爆防止）
    // Shiftを押した後に別のキー（例えば 'A'）を押した場合、
    // それは「Shift連打」ではないのでタイマーをリセットする。
    if (record->event.pressed) {
        lshift_timer = 0;
    }

    return true;
}
