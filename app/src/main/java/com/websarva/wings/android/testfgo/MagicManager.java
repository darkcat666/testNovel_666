package com.websarva.wings.android.testfgo;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.WindowMetrics;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.sql.Time;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

// 魔法管理クラス
public class MagicManager extends AppCompatActivity {
    private int choose_sound_1; // SE1
    private int choose_sound_2; // SE2
    private int choose_sound_3; // SE3
    private int choose_sound_4; // SE4
    private int choose_sound_5; // SE5
    private int choose_sound_6; // SE6
    private int choose_sound_7; // SE7
    private int choose_sound_8; // SE8
    private AudioManager audio;
    private int ringVolume = 0;
    private int ringMaxVolume = 0;
    private ImageView backImage;
    private int ScreenWidth = 0;
    private int ScreenHeight = 0;
    private MainActivity main; // MainActivityクラスの関数を呼び出すためのインスタンス
    private SoundPool soundPool;
    private Util util = new Util(); // Utilクラスの関数を呼び出すためのインスタンス
    private Handler handler = new Handler();

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 初期化処理
        init();

        // マーロニ様召喚
        summonMaroni_sama();
    }

    private void init() {
        // レイアウトファイル読み込み
        setContentView(R.layout.magic_layout_full_display);

        WindowMetrics windowMetrics = this.getWindowManager().getCurrentWindowMetrics();

        ScreenWidth = windowMetrics.getBounds().width();
        ScreenHeight = windowMetrics.getBounds().height();

        // 背景画面設定
        backImage = (ImageView) findViewById(R.id.fullscreenBackImageView);

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
                .setMaxStreams(7)
                .build();

        // インスタンス生成
        main = new MainActivity();

        // サウンドプールロード
//        // 【ニコニコアタック】
//        loadSounds_nikonikoattack();

        // 【マーロニ様召喚】
        loadSounds_summonMaroniSama();
    }

    private void loadSounds_nikonikoattack() {
        // サウンド１をロード
        choose_sound_1 = soundPool.load(this, R.raw.no_way, 1);
        // サウンド２をロード
        choose_sound_2 = soundPool.load(this, R.raw.stub_sound, 1);
        // サウンド３をロード
        choose_sound_3 = soundPool.load(this, R.raw.saa_danzaino_tokida, 1);
        soundPool.setOnLoadCompleteListener((soundPool, nikonikoId, status) -> {
            // ロード完了を待つ
        });
    }

    private void loadSounds_summonMaroniSama() {
        // サウンド１をロード
        choose_sound_1 = soundPool.load(this, R.raw.maroni_sama, 1);
        // サウンド２をロード
        choose_sound_2 = soundPool.load(this, R.raw.pleasestop_maroni_sama, 1);
        // サウンド３をロード
        choose_sound_3 = soundPool.load(this, R.raw.maou_se_voice_dog01, 1);
        // サウンド４をロード
        choose_sound_4 = soundPool.load(this, R.raw.maou_se_voice_monster02, 1);
        // サウンド５をロード
        choose_sound_5 = soundPool.load(this, R.raw.pleasestop_maroni_sama, 1);
        // サウンド６をロード
        choose_sound_6 = soundPool.load(this, R.raw.pleasestop_maroni_sama, 1);
        // サウンド７をロード
        choose_sound_7 = soundPool.load(this, R.raw.pleasestop_maroni_sama, 1);
        // サウンド８をロード
        choose_sound_8 = soundPool.load(this, R.raw.pleasestop_maroni_sama, 1);
        soundPool.setOnLoadCompleteListener((soundPool, summonMaroniSamaId, status) -> {
            // ロード完了を待つ
        });
    }

    // 効果音再生関数
    // targetSound：鳴らす対象のロード済み効果音
    // count：再生回数
    public int playSound(int targetSound, int count) {
        // 効果音再生
        // AudioManagerを取得する
        audio = (AudioManager)getSystemService(Context.AUDIO_SERVICE);

        // 現在の音量を取得する
        ringVolume = audio.getStreamVolume(AudioManager.STREAM_ALARM);

        // ストリームごとの最大音量を取得する
        ringMaxVolume = audio.getStreamMaxVolume(AudioManager.STREAM_ALARM);

        // soundPoolは失敗したら0を返す
        int fail = 0;

        fail = soundPool.play(targetSound, ringVolume, ringVolume, 1, count, 1f);

        return fail;
    }

    // 失敗しないサウンド再生
    // chooseSound：選択したロードサウンド
    // count：再生回数
    public void nomissPlaySound(int chooseSound, int count) {
        // サウンドロード

        //再生テスト
        //少し待ち時間を入れる
        int streamID = 0;
        do {
            //少し待ち時間を入れる
            try {
                //ボリュームをゼロにして再生して戻り値をチェック
                streamID = soundPool.play(chooseSound, 0.0f, 0.0f, 1, 0, 1.0f);
                Thread.sleep(10);
            } catch (InterruptedException e) {
            }
        } while(streamID == 0);
        // 再生
        playSound(chooseSound, count);
    }

    // １：ニコニコアタック
    // rValueImage：移動させる画像
    public void nikoniko_attack() {
        // アニメーションの組み合わせ
        // true：補間をsetに適用　false：補間を個々に指定
        AnimationSet set1 = new AnimationSet(false);
        AnimationSet set2 = new AnimationSet(false);
        AnimationSet set3 = new AnimationSet(false);
        AnimationSet set4 = new AnimationSet(false);
        AnimationSet set5 = new AnimationSet(false);
        AnimationSet set6 = new AnimationSet(false);
        AnimationSet set7 = new AnimationSet(false);

        Animation trans_1 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_1.setDuration(1500); // 1500msかけて動く

        trans_1.setRepeatMode(Animation.RESTART);
        trans_1.setRepeatCount(Animation.INFINITE);
        trans_1.setFillAfter(true);
        Animation trans_2 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_2.setDuration(1500); // 1500msかけて動く

        trans_2.setRepeatMode(Animation.RESTART);
        trans_2.setRepeatCount(Animation.INFINITE);
        trans_2.setFillAfter(true);
        Animation trans_3 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_3.setDuration(1500); // 1500msかけて動く

        trans_3.setRepeatMode(Animation.RESTART);
        trans_3.setRepeatCount(Animation.INFINITE);
        trans_3.setFillAfter(true);
        Animation trans_4 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_4.setDuration(1500); // 1500msかけて動く

        trans_4.setRepeatMode(Animation.RESTART);
        trans_4.setRepeatCount(Animation.INFINITE);
        trans_4.setFillAfter(true);
        Animation trans_5 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_5.setDuration(1500); // 1500msかけて動く

        trans_5.setRepeatMode(Animation.RESTART);
        trans_5.setRepeatCount(Animation.INFINITE);
        trans_5.setFillAfter(true);
        Animation trans_6 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_6.setDuration(1500); // 1500msかけて動く

        trans_6.setRepeatMode(Animation.RESTART);
        trans_6.setRepeatCount(Animation.INFINITE);
        trans_6.setFillAfter(true);
        Animation trans_7 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        trans_7.setDuration(1500); // 1500msかけて動く

        trans_7.setRepeatMode(Animation.RESTART);
        trans_7.setRepeatCount(Animation.INFINITE);
        trans_7.setFillAfter(true);

        // ニコニコアタックの回転処理
        ScaleAnimation scaleAnimation = new ScaleAnimation(
                1.0f, 0.0f, 1.0f,0.0f,
                Animation.ABSOLUTE,
                ScreenWidth / 2, Animation.ABSOLUTE, ScreenHeight / 2);
        // animation時間 msec
        scaleAnimation.setDuration(1500);

        // 無限繰り返し設定（合成する際には設定を合わせること！！）
        scaleAnimation.setRepeatMode(Animation.RESTART);
        scaleAnimation.setRepeatCount(Animation.INFINITE);

        // animationが終わったそのまま表示にしない
        scaleAnimation.setFillAfter(true);

        // それぞれの顔設定
        ImageView anim1 = (ImageView) findViewById(R.id.miniView1);
        ImageView anim2 = (ImageView) findViewById(R.id.miniView2);
        ImageView anim3 = (ImageView) findViewById(R.id.miniView3);
        ImageView anim4 = (ImageView) findViewById(R.id.miniView4);
        ImageView anim5 = (ImageView) findViewById(R.id.miniView5);
        ImageView anim6 = (ImageView) findViewById(R.id.miniView6);
        ImageView anim7 = (ImageView) findViewById(R.id.miniView7);

        // アニメーションセット追加
        set1.addAnimation(trans_1);
        set1.addAnimation(scaleAnimation);
        set2.addAnimation(trans_2);
        set2.addAnimation(scaleAnimation);
        set3.addAnimation(trans_3);
        set3.addAnimation(scaleAnimation);
        set4.addAnimation(trans_4);
        set4.addAnimation(scaleAnimation);
        set5.addAnimation(trans_5);
        set5.addAnimation(scaleAnimation);
        set6.addAnimation(trans_6);
        set6.addAnimation(scaleAnimation);
        set7.addAnimation(trans_7);
        set7.addAnimation(scaleAnimation);

//        // 別スレッドでニコニコアタックの顔変更（スロットマシン風に）
//        AsyncChangeAnimation changeAnimation = new AsyncChangeAnimation(this);
//        changeAnimation.execute();

        // アニメーション開始
        anim1.startAnimation(set1);
        anim2.startAnimation(set2);
        anim3.startAnimation(set3);
        anim4.startAnimation(set4);
        anim5.startAnimation(set5);
        anim6.startAnimation(set6);
        anim7.startAnimation(set7);

        // ニコニコアタックの顔セット
        changeViewMagic(R.id.miniView1, randomChangeFace());
        changeViewMagic(R.id.miniView2, randomChangeFace());
        changeViewMagic(R.id.miniView3, randomChangeFace());
        changeViewMagic(R.id.miniView4, randomChangeFace());
        changeViewMagic(R.id.miniView5, randomChangeFace());
        changeViewMagic(R.id.miniView6, randomChangeFace());
        changeViewMagic(R.id.miniView7, randomChangeFace());

        // サウンド再生
        Timer timerSound_stub = new Timer();
        delayNoMissPlaySound(800, choose_sound_2, 0, true, timerSound_stub);

        // 白フラッシュ処理
        Timer timerFlash1 = new Timer();
        Timer timerFlash2 = new Timer();
        flashWhite(0, 500, timerFlash1, timerFlash2);

        // サウンド再生
        Timer timerSound = new Timer();
        delayNoMissPlaySound(0,  choose_sound_3, 0, true, timerSound);
        flashWhite(1000, 1500, timerFlash1, timerFlash2);

        Timer timerSound_yamero = new Timer();
        delayNoMissPlaySound(4000,  choose_sound_1, 0, true, timerSound_yamero);
        flashWhite(2000, 2500, timerFlash1, timerFlash2);
        flashWhite(3000, 3500, timerFlash1, timerFlash2);
        flashWhite(4000, 4500, timerFlash1, timerFlash2);
        flashWhite(5000, 5500, timerFlash1, timerFlash2);
    }


    // ２：マーロニ様召喚
    // rValueImage：移動させる画像
    public void summonMaroni_sama() {
//        // アニメーションの組み合わせ
//        // true：補間をsetに適用　false：補間を個々に指定
//        AnimationSet set1 = new AnimationSet(false);
//        AnimationSet set2 = new AnimationSet(false);
//        AnimationSet set3 = new AnimationSet(false);
//
//        // マーロニ様の召喚処理
//        TranslateAnimation trans_1 = new TranslateAnimation(
//                Animation.ABSOLUTE, ScreenWidth / 2 + 13f,
//                Animation.ABSOLUTE, ScreenWidth / 2 + 13f,
//                Animation.ABSOLUTE, ScreenHeight / 2 + 7f,
//                Animation.ABSOLUTE, ScreenHeight / 2 + 7f);
//        // animationが終わったそのまま表示にする
//        trans_1.setFillAfter(false);
//        // animation時間 msec
//        trans_1.setDuration(3000);
//        // マーロニ様の召喚処理
//        TranslateAnimation trans_2 = new TranslateAnimation(
//                Animation.ABSOLUTE, ScreenWidth / 3 + 10f,
//                Animation.ABSOLUTE, ScreenWidth / 3 + 10f,
//                Animation.ABSOLUTE, ScreenHeight / 3 + 10f,
//                Animation.ABSOLUTE, ScreenHeight / 3 + 10f);
//        // animationが終わったそのまま表示にする
//        trans_2.setFillAfter(false);
//        // animation時間 msec
//        trans_2.setDuration(3000);
//        // マーロニ様の召喚処理
//        TranslateAnimation trans_3 = new TranslateAnimation(
//                Animation.ABSOLUTE, ScreenWidth / 4 + 7f,
//                Animation.ABSOLUTE, ScreenWidth / 4 + 7f,
//                Animation.ABSOLUTE, ScreenHeight / 2 + 13f,
//                Animation.ABSOLUTE, ScreenHeight / 2 + 13f);
//        // animationが終わったそのまま表示にする
//        trans_3.setFillAfter(false);
//        // animation時間 msec
//        trans_3.setDuration(3000);
//
//        ScaleAnimation scaleAnimation = new ScaleAnimation(
//                1.0f, 1.0f, 1.0f,1.0f,
//                Animation.ABSOLUTE,
//                ScreenWidth * 4, Animation.ABSOLUTE, ScreenHeight * 4);
//        // animation時間 msec
//        scaleAnimation.setFillAfter(false);
//        scaleAnimation.setDuration(3000);
//
//        // それぞれの顔設定
//        // アニメーションの用意
//        ImageView anim1 = (ImageView) findViewById(R.id.miniView1);
//        ImageView anim2 = (ImageView) findViewById(R.id.miniView2);
//        ImageView anim3 = (ImageView) findViewById(R.id.miniView3);
//
//        // マーロニ様魔法陣
//        changeViewMagic(R.id.miniView1, R.drawable.summon_maroni_samatitle);
//        changeViewMagic(R.id.miniView2, R.drawable.summon_maroni_samatitle);
//        changeViewMagic(R.id.miniView3, R.drawable.summon_maroni_samatitle);
        Timer timerFlash1 = new Timer();
        Timer timerFlash2 = new Timer();
//        flashWhite(0, 500, timerFlash1, timerFlash2);
//        // アニメーションセット追加
//        set1.addAnimation(scaleAnimation);
//        set1.addAnimation(trans_1);
//        set2.addAnimation(scaleAnimation);
//        set2.addAnimation(trans_2);
//        set3.addAnimation(scaleAnimation);
//        set3.addAnimation(trans_3);
        flashMaroniSama(8000, 1000, timerFlash1, timerFlash2);
//        // アニメーション開始
//        anim1.startAnimation(set1);
//        anim2.startAnimation(set2);
//        anim3.startAnimation(set3);

        // 白フラッシュ処理
        flashWhite(1500, 2000, timerFlash1, timerFlash2);

//        // マーロニ様召喚
//        changeViewMagic(R.id.miniView1, R.drawable.touka);
//        changeViewMagic(R.id.miniView2, R.drawable.touka);
//        changeViewMagic(R.id.miniView3, R.drawable.touka);

//        // サウンド再生
//        Timer timerSound = new Timer();
//        delayNoMissPlaySound(0,  choose_sound_3, 0, true, timerSound);
//        Timer timerSound_maroni_open = new Timer();
//        delayNoMissPlaySound(4000,  choose_sound_1, 0, true, timerSound_maroni_open);
    }

    // 背景画像の差し替え
    // findViewByIdRValue : 差し替える対象のViewのR値
    // rValueView : 差し替える画像のR値
    private void changeViewMagic(int findViewByIdRValue ,int rValueView) {
        ImageView myImage = findViewById(findViewByIdRValue);
        Drawable myDrawable = getResources().getDrawable(rValueView);

        // Handlerクラスをインスタンス化し、postDelayedメソッドを呼んでいる
        handler.postDelayed(new Runnable() {
            // Runnable型のインスタンス化と定義
            @Override
            public void run() {

                // 遅らせて実行したい処理
                myImage.setImageDrawable(myDrawable);
            }
        }, 0); // 遅らせたい時間(ミリ秒) 3000ミリ秒 -> 3秒
    }

    // 遅延して音声出力
    public void delayNoMissPlaySound(long firstDelayMills, int chooseSound, int count, boolean stopOrNot, Timer timer) {
        timer.schedule(new TimerTask() {
            public void run() {
                nomissPlaySound(chooseSound, count);
                if (stopOrNot == true) {
                    // 起動したらタイマー停止
                    timer.cancel();
                }
            }
        }, firstDelayMills);
    }

    // ランダムで場所をちらつかせる処理
    private float randomerPlace() {
        Random rand = new Random();
        float val = rand.nextInt(ScreenHeight / 5);

        return val;
    }

    // 白色で背景フラッシュ処理
    private void flashWhite10Times(Timer timer1) {
        // 背景フラッシュ用アニメーション
        Animation fullBack = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        fullBack.setDuration(100); // 100msかけて動く（維持する）

        fullBack.setRepeatMode(Animation.RESTART);
        fullBack.setRepeatCount(Animation.INFINITE);
        fullBack.setFillAfter(true);

        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.active);
                fullBack.setDuration(100);
            }
        }, 0);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.whiteflash);
                fullBack.setDuration(100);
            }
        }, 100);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.active);
                fullBack.setDuration(100);
            }
        }, 200);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.whiteflash);
                fullBack.setDuration(100);
            }
        }, 300);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.active);
                fullBack.setDuration(100);
            }
        }, 400);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.whiteflash);
                fullBack.setDuration(100);
            }
        }, 500);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.active);
                fullBack.setDuration(100);
            }
        }, 600);
        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.whiteflash);
                fullBack.setDuration(100);
            }
        }, 700);
    }


    // 白色で背景フラッシュ処理
    private void flashWhite(int firstDelayMills, int secondDelayMills, Timer timer1, Timer timer2) {
        // 背景フラッシュ用アニメーション
        Animation fullBack = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenWidth / 2 + randomerPlace(),
                Animation.ABSOLUTE, ScreenHeight / 2 + randomerPlace());
        fullBack.setDuration(100); // 100msかけて動く（維持する）

        fullBack.setRepeatMode(Animation.RESTART);
        fullBack.setRepeatCount(Animation.INFINITE);
        fullBack.setFillAfter(true);

        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.active);
                fullBack.setDuration(100);
            }
        }, firstDelayMills);
        timer2.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.whiteflash);
                fullBack.setDuration(100);
            }
        }, secondDelayMills);
    }

    // 白色で背景フラッシュ処理
    private void flashMaroniSama(int firstDelayMills, int secondDelayMills, Timer timer1, Timer timer2) {

        // アニメーションの組み合わせ
        // true：補間をsetに適用　false：補間を個々に指定
        AnimationSet set1 = new AnimationSet(false);
        AnimationSet set2 = new AnimationSet(false);
        AnimationSet set3 = new AnimationSet(false);

        AnimationSet set1_1 = new AnimationSet(false);
        AnimationSet set2_1 = new AnimationSet(false);
        AnimationSet set3_1 = new AnimationSet(false);
        // マーロニ様の召喚処理
        TranslateAnimation trans_1 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + 13f,
                Animation.ABSOLUTE, ScreenWidth / 2 + 13f,
                Animation.ABSOLUTE, ScreenHeight / 2 + 7f,
                Animation.ABSOLUTE, ScreenHeight / 2 + 7f);
        // animationが終わったそのまま表示にする
        trans_1.setFillAfter(true);
        // animation時間 msec
        trans_1.setDuration(8000);
        // マーロニ様の召喚処理
        TranslateAnimation trans_2 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 3 + 10f,
                Animation.ABSOLUTE, ScreenWidth / 3 + 10f,
                Animation.ABSOLUTE, ScreenHeight / 3 + 10f,
                Animation.ABSOLUTE, ScreenHeight / 3 + 10f);
        // animationが終わったそのまま表示にする
        trans_2.setFillAfter(true);
        // animation時間 msec
        trans_2.setDuration(8000);
        // マーロニ様の召喚処理
        TranslateAnimation trans_3 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 4 + 7f,
                Animation.ABSOLUTE, ScreenWidth / 4 + 7f,
                Animation.ABSOLUTE, ScreenHeight / 2 + 13f,
                Animation.ABSOLUTE, ScreenHeight / 2 + 13f);
        // animationが終わったそのまま表示にする
        trans_3.setFillAfter(true);
        // animation時間 msec
        trans_3.setDuration(8000);

        // マーロニ様のかくかく移動処理
        TranslateAnimation trans_1_1 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 2 + 13f,
                Animation.ABSOLUTE, ScreenWidth / 2 + 130f,
                Animation.ABSOLUTE, ScreenHeight / 6 + 7f,
                Animation.ABSOLUTE, ScreenHeight / 6 + 7f);
        // animationが終わったそのまま表示にする
        trans_1_1.setFillAfter(false);
        // animation時間 msec
        trans_1_1.setDuration(50);
        trans_1_1.setRepeatMode(Animation.RESTART);
        trans_1_1.setRepeatCount(Animation.INFINITE);
        // マーロニ様の召喚処理
        TranslateAnimation trans_2_1 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 3 + 10f,
                Animation.ABSOLUTE, ScreenWidth / 3 + 70f,
                Animation.ABSOLUTE, ScreenHeight / 6 + 10f,
                Animation.ABSOLUTE, ScreenHeight / 6 + 10f);
        // animationが終わったそのまま表示にする
        trans_2_1.setFillAfter(false);
        // animation時間 msec
        trans_2_1.setDuration(50);
        trans_2_1.setRepeatMode(Animation.RESTART);
        trans_2_1.setRepeatCount(Animation.INFINITE);
        // マーロニ様の召喚処理
        TranslateAnimation trans_3_1 = new TranslateAnimation(
                Animation.ABSOLUTE, ScreenWidth / 4 + 7f,
                Animation.ABSOLUTE, ScreenWidth / 4 + 40f,
                Animation.ABSOLUTE, ScreenHeight / 4 + 13f,
                Animation.ABSOLUTE, ScreenHeight / 4 + 13f);
        // animationが終わったそのまま表示にする
        trans_3_1.setFillAfter(false);
        // animation時間 msec
        trans_3_1.setDuration(50);
        trans_3_1.setRepeatMode(Animation.RESTART);
        trans_3_1.setRepeatCount(Animation.INFINITE);

        ScaleAnimation scaleAnimation = new ScaleAnimation(
                1.0f, 1.0f, 1.0f,1.0f,
                Animation.ABSOLUTE,
                ScreenWidth * 4, Animation.ABSOLUTE, ScreenHeight * 4);
        // animation時間 msec
        scaleAnimation.setFillAfter(true);
        scaleAnimation.setDuration(8000);

        // それぞれの顔設定
        // アニメーションの用意
        ImageView anim1 = (ImageView) findViewById(R.id.miniView1);
        ImageView anim2 = (ImageView) findViewById(R.id.miniView2);
        ImageView anim3 = (ImageView) findViewById(R.id.miniView3);

        // マーロニ様魔法陣
        changeViewMagic(R.id.miniView1, R.drawable.summon_maroni_samatitle);
        changeViewMagic(R.id.miniView2, R.drawable.summon_maroni_samatitle);
        changeViewMagic(R.id.miniView3, R.drawable.summon_maroni_samatitle);
        Timer timerFlash1 = new Timer();
        Timer timerFlash2 = new Timer();
        flashWhite(0, 500, timerFlash1, timerFlash2);
        // アニメーションセット追加
        set1.addAnimation(scaleAnimation);
        set1.addAnimation(trans_1);
        set2.addAnimation(scaleAnimation);
        set2.addAnimation(trans_2);
        set3.addAnimation(scaleAnimation);
        set3.addAnimation(trans_3);

        // サウンド再生
        Timer timerSound = new Timer();
        delayNoMissPlaySound(0,  choose_sound_3, 0, true, timerSound);
        Timer timerSound_maroni_open = new Timer();
        delayNoMissPlaySound(4000,  choose_sound_1, 0, true, timerSound_maroni_open);

        // アニメーション開始
        anim1.startAnimation(set1);
        anim2.startAnimation(set2);
        anim3.startAnimation(set3);

        timer1.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.miniView1, R.drawable.maroni_sama_face);
                changeViewMagic(R.id.miniView2, R.drawable.maroni_sama_face);
                changeViewMagic(R.id.miniView3, R.drawable.maroni_sama_face);
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.whiteflash);
                set1.setDuration(1000);
                set2.setDuration(1000);
                set3.setDuration(1000);
                changeViewMagic(R.id.miniView1, R.drawable.touka);
                changeViewMagic(R.id.miniView2, R.drawable.touka);
                changeViewMagic(R.id.miniView3, R.drawable.touka);
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.black);
            }
        }, firstDelayMills);
        timer2.schedule(new TimerTask() {
            public void run() {
                changeViewMagic(R.id.miniView1, R.drawable.summon_maroni_samatitle);
                changeViewMagic(R.id.miniView2, R.drawable.summon_maroni_samatitle);
                changeViewMagic(R.id.miniView3, R.drawable.summon_maroni_samatitle);
                changeViewMagic(R.id.fullscreenBackImageView, R.drawable.summon_maroni_samatitle);
                set1.setDuration(1000);
                set2.setDuration(1000);
                set3.setDuration(1000);
                changeViewMagic(R.id.miniView1, R.drawable.maroni_sama_face);
                changeViewMagic(R.id.miniView2, R.drawable.maroni_sama_face);
                changeViewMagic(R.id.miniView3, R.drawable.maroni_sama_face);
                set1.setDuration(1000);
                set2.setDuration(1000);
                set3.setDuration(1000);
                // アニメーションセット追加
                set1_1.addAnimation(trans_1_1);
                set2_1.addAnimation(trans_2_1);
                set3_1.addAnimation(trans_3_1);
                // アニメーション開始
                anim1.startAnimation(set1_1);
                anim2.startAnimation(set2_1);
                anim3.startAnimation(set3_1);
                Timer timerSound_oyame = new Timer();
                delayNoMissPlaySound(100, choose_sound_5, 1, true, timerSound_oyame);
            }
        }, secondDelayMills);
    }


    // ニコニコアタックの顔を変更する変数
    private int randomChangeFace() {
        Random rand = new Random();
        int val = rand.nextInt(7);
        switch (val) {
            case 0:
                return R.drawable.nikoniko_attack_black;
            case 1:
                return R.drawable.nikoniko_attack_cream;
            case 2:
                return R.drawable.nikoniko_attack_green;
            case 3:
                return R.drawable.nikoniko_attack_orange;
            case 4:
                return R.drawable.nikoniko_attack_purple;
            case 5:
                return R.drawable.nikoniko_attack_red;
            case 6:
                return R.drawable.nikoniko_attack_skyblue;
            default:
                return R.drawable.nikoniko_attack_black;
        }
    }

//    class AsyncChangeAnimation extends AsyncTask<Void, Void, Void> {
//        private Context context;
//
//        public AsyncChangeAnimation(Context context) {
//            this.context = context;
//        }
//
//        @Override
//        protected void onPreExecute() {
//            // サウンド再生
//            Timer timerSound_stub = new Timer();
//            delayNoMissPlaySound(800, choose_sound_2, 0, true, timerSound_stub);
//            // 白フラッシュ処理
//            Timer timerFlash1 = new Timer();
//            Timer timerFlash2 = new Timer();
//            flashWhite(0, 1000, timerFlash1, timerFlash2);
//            // サウンド再生
//            Timer timerSound = new Timer();
//            delayNoMissPlaySound(0,  choose_sound_3, 0, true, timerSound);
//            Timer timerSound_yamero = new Timer();
//            delayNoMissPlaySound(4000,  choose_sound_1, 0, true, timerSound_yamero);
//        }
//
//        @Override
//        protected Void doInBackground(Void... voids) {
//            try {
//                for (int i = 0; i < 3; i++) {
//                    // ニコニコアタックの顔セット
//                    changeViewMagic(R.id.miniView1, randomChangeFace());
//                    changeViewMagic(R.id.miniView2, randomChangeFace());
//                    changeViewMagic(R.id.miniView3, randomChangeFace());
//                    changeViewMagic(R.id.miniView4, randomChangeFace());
//                    changeViewMagic(R.id.miniView5, randomChangeFace());
//                    changeViewMagic(R.id.miniView6, randomChangeFace());
//                    changeViewMagic(R.id.miniView7, randomChangeFace());
//                }
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//            return null;
//        }
//
//        @Override
//        protected void onPostExecute(Void aVoid) {
//            // 再生したらリリース
//            soundPool.release();
//        }
//    }
}
