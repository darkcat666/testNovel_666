package com.websarva.wings.android.testfgo;

import android.media.AudioManager;
import android.media.SoundPool;

public class SoundManager {
    private SoundPool soundPool;

    // 効果音初期設定
    public void initSound() {
        // 効果音初期設定
        soundPool = new SoundPool(6, AudioManager.STREAM_MUSIC, 0);

        // 効果音登録
//        int sound_1 = soundPool.load(this, R.raw.hit, 1);

    }

    // 登録した変数名の効果音を再生
    // targetSound：鳴らす対象の効果音
    public void playHitSound(int targetSound) {
        soundPool.play(targetSound, 1.0f, 1.0f, 1, 0, 1.0f);
    }
}
