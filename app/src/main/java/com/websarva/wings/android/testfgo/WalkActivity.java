package com.websarva.wings.android.testfgo;

import android.content.Intent;
import android.media.AudioAttributes;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;

import androidx.appcompat.app.AppCompatActivity;

public class WalkActivity extends AppCompatActivity  {
    private MediaPlayer mediaPlayer;
    private SoundPool soundPool;
    private ImageView animeImage;
    private ImageView backgroundImage;
    private int choose_sound_1; // ボタン選択効果音＿１
    private Handler handler; // 別スレッドから実行するHandlerインスタンス
    int posX; // X位置の座標
    int posY; // Y位置の座標
    int ImageViewX; // ImageViewのX幅
    int ImageViewY; // ImageViewのY幅
    boolean posFlg = true; // ImageViewの座標フラグ

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 初期設定
        setInit();
    }

    // 初期設定
    private void setInit() {

        // レイアウトファイル読み込み
        setContentView(R.layout.activity_walk);

        // アニメーション設定
        animeImage = (ImageView)findViewById(R.id.walk_character);

        // 音楽再生
        playFromMediaPlayer(R.raw.nhk);

        // Handlerインスタンス生成
        handler = new Handler();

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

        // サウンド１をロード
        choose_sound_1 = soundPool.load(this, R.raw.__koukaonn__tin, 1);

        // 仮置き
        SoundManager soundManager = new SoundManager();
        soundManager.initSound();

        // フリック入力
        View flickView = getWindow().getDecorView(); // Activity画面
        float adjustX = 150.0f;
        float adjustY = 150.0f;

        new FlickCheck(flickView, adjustX, adjustY) {

            @Override
            public void getFlick(int flickData) {
                // 本当はここに書いてはいけないのだけど、動かないから追加
                setPos();
                switch (flickData) {
                    case FlickCheck.LEFT_FLICK:
                        // 左フリック
                        posX = animeImage.getLeft();
                        animeImage.setLeft(posX - ImageViewX);
                        break;

                    case FlickCheck.RIGHT_FLICK:
                        // 右フリック
                        posX = animeImage.getLeft();
                        animeImage.setLeft(posX + ImageViewX);
                        break;

                    case FlickCheck.UP_FLICK:
                        // 上フリック
                        posY = animeImage.getTop();
                        animeImage.setTop(posY - ImageViewY);
                        break;

                    case FlickCheck.DOWN_FLICK:
                        // 下フリック
                        posY = animeImage.getTop();
                        animeImage.setTop(posY + ImageViewY);
                        break;
                }
            }
        };
    }

    // メインアクティビティに戻る処理
    private void startMainActivity() {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        // 音楽も多重起動しないようにする
        stopFromMediaPlayer();

        // この、バトルアクティビティを終了（多重起動しないようにする）
        finish();
    }

    // 音楽ファイルの停止
    public void stopFromMediaPlayer() {
        if (mediaPlayer.isPlaying()) {
            mediaPlayer.stop();
        }
    }

    // Resume時の動作
    public void onResume() {
        // 音楽再開
        super.onResume();
        if (mediaPlayer == null) {
            playFromMediaPlayer(R.raw.nhk);
        }
    }

    // 音楽ファイルの再生
    // rValueMusic : 再生する音楽のR値
    private void playFromMediaPlayer(int rValueMusic) {
        mediaPlayer = MediaPlayer.create(this, rValueMusic);

        // ループ設定
        mediaPlayer.setLooping(true);

        // メディアプレイヤー再生
        mediaPlayer.start();
    }

    // 音楽ファイルの再生
    // rValueCharacter : セットするキャラクターのR値
    private void setWalkCharacter(int rValueCharacter) {

    }

    private void setPos() {
        if(posFlg) {
            this.ImageViewX = animeImage.getWidth();
            this.ImageViewY = animeImage.getHeight();
            posFlg = false;
        }
    }
}