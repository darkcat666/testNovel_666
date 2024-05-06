package com.websarva.wings.android.testfgo;

import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class Util extends AppCompatActivity {
    private MediaPlayer mediaPlayer;
    private SoundPool soundPool;
    private AudioManager audio;
    private int ringVolume = 0;
    private int ringMaxVolume = 0;

    public Util() {
        init();
    }

    public MediaPlayer getMediaPlayer() {
        return this.mediaPlayer;
    }

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        init();
    }

    public SoundPool getSoundPool() {
        return this.soundPool;
    }

    private void init() {
        // (！警告消す)
        // オーディオアトリビュートインスタンス生成
        AudioAttributes audioAttributes = new AudioAttributes.Builder()
                .setUsage(AudioAttributes.USAGE_GAME)
                .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION)
                .build();

        // （！警告消す）
        // サウンドプールインスタンス生成
        soundPool = new SoundPool.Builder()
                .setAudioAttributes(audioAttributes)
                // ストリーム数に応じて
                .setMaxStreams(2)
                .build();
    }

    // 音楽ファイルの再生
    // rValueMusic : 再生する音楽のR値
    public void playFromMediaPlayer(int rValueMusic) {
        mediaPlayer = MediaPlayer.create(this, rValueMusic);

        // ループ設定
        mediaPlayer.setLooping(true);

        // メディアプレイヤー再生
        mediaPlayer.start();
    }

    // 音楽ファイルの停止
    public void stopFromMediaPlayer() {
        if (mediaPlayer.isPlaying()) {
            mediaPlayer.stop();
        }
    }



}
