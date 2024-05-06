package com.websarva.wings.android.testfgo;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.media.AudioAttributes;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.BounceInterpolator;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class BattleActivity extends AppCompatActivity {
    private MediaPlayer mediaPlayer;
    private SoundPool soundPool;
    private Button chooseButton_1;
    private Button chooseButton_2;
    private TextView textViewUp;
    private TextView textViewDown;
    private ImageView iconImage;
    private ImageView animeImage;
    private int choose_sound_1; // ボタン選択効果音＿１
    private int TEXT_SPEED_INTERVAL = 500; // 文字列のsetText()インターバル
    private Handler handler; // 別スレッドから実行するHandlerインスタンス
    private Animation anim_longer; // 長くするアニメーション用インスタンス
    private Animation anim_fuluhehhenndo; // 振動させるアニメーション用インスタンス
    private int layoutWidth; // 画面横幅
    private int layoutHeight; // 画面縦幅

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 初期設定
        setInit();

    }

    // 初期設定
    private void setInit() {

        // レイアウトファイル読み込み
        setContentView(R.layout.activity_battle);

        // アニメーション設定
        animeImage = (ImageView)findViewById(R.id.imageView_battle);

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

    // 背景画像の差し替え
    // rValueView : 差し替える画像のR値
    private void changeView(int rValueView) {
        ImageView myImage= (ImageView) findViewById(R.id.imageView_battle);
        Drawable myDrawable = getResources().getDrawable(rValueView);
        myImage.setImageDrawable(myDrawable);
    }

    // アイコン画像の差し替え
    // rValueView : 差し替えるアイコン画像のR値
    private void changeIconView(int rValueView) {
        ImageView myImage= (ImageView) findViewById(R.id.view_icon);
        Drawable myDrawable = getResources().getDrawable(rValueView);
        myImage.setImageDrawable(myDrawable);
    }

    // 選択ボタンを表示させる
    // count：1~2
    private void viewChooseButton(int count) {
        if (count == 1) {
            chooseButton_1.setVisibility(View.VISIBLE);
        } else if (count == 2) {
            chooseButton_1.setVisibility(View.VISIBLE);
            chooseButton_2.setVisibility(View.VISIBLE);
        } else {
            // 値を間違えている場合は処理をしない
        }
    }

    // 選択ボタンを全て消滅させる
    private void viewDisappearButton() {
        chooseButton_1.setVisibility(View.INVISIBLE);
        chooseButton_2.setVisibility(View.INVISIBLE);
    }

    // テキストをクリアする
    private void ClearText() {
        // テキストビュー取得
        textViewUp = findViewById(R.id.textView_upper);
        textViewDown = findViewById(R.id.textView_downer);

        // クリア
        textViewUp.setText("");
        textViewDown.setText("");
    }

    // ゆっくりとテキストを表示させる
    // targetString_1：表示させる対象文言_1行目
    // targetString_2：表示させる対象文言_2行目
    public void ShowText(String targetString_1, String targetString_2) {

        // テキストをクリアする
        ClearText();

        // テキストビュー取得
        textViewUp = findViewById(R.id.textView_upper);
        textViewDown = findViewById(R.id.textView_downer);

        // 結合テキスト
        String mergeText = targetString_1 + targetString_2;

        // テキスト出力用の変数
        String outputText = "";

        // 2行目用のカウント変数
        int kk = 0;

        // 全ての文字を表示させるまで繰り返す
        for (int k = 0; k <= mergeText.length(); k++) {
            outputText = mergeText.substring(0, k);

            // 2行目の場合はtargetString_2の引数で出力用変数を上書き
            if (k > targetString_1.length()) {
                kk++;
                outputText = targetString_2.substring(0, kk);
            }

            // 別スレッドからPOSTを実行
            // 行番号に応じてテキストを表示させる
            String finalOutputText = outputText;
            int finalK = k;
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    if (finalK <= targetString_1.length()) {
                        textViewUp.setText(finalOutputText);
                    } else if (finalK > targetString_1.length()) {
                        textViewDown.setText(finalOutputText);
                    }
                }
            }, TEXT_SPEED_INTERVAL * k);
        }

    }

    // ゴウトのアイコン用アニメーション
    private Animation goutIcon() {
        // Scale 拡大縮小
        // from幅倍率,to幅倍率,from高さ倍率,to高さ倍率,中心軸X,中心軸Y
        Animation scale = new ScaleAnimation(0.0f, 1.0f, 0.0f, 1.0f);

        // true：補間をsetに適用
        AnimationSet set = new AnimationSet(true);
        scale.setDuration(500); // 1000msかけて動く
        set.addAnimation(scale);

       // true：終了時に元の状態に戻さない
        set.setFillAfter(true);

        // 0でリピートなし
        set.setRepeatCount(0);

        return set;
    }

    // （主に猫用・・・？）
    // 画像を長くするアニメーション
    // rValueImage：長くする画僧
    private Animation longingCat() {

        // Scale 拡大縮小
        // from幅倍率,to幅倍率,from高さ倍率,to高さ倍率,中心軸X,中心軸Y
        Animation scale = new ScaleAnimation(1.0f,1.0f,0.0f,3.0f);
        scale.setDuration(3000); // 3000msかけて動く

        // アニメーションの組み合わせ
        // true：補間をsetに適用　false：補間を個々に指定
        AnimationSet set = new AnimationSet(true);

        set.addAnimation(scale);

        // true：終了時に元の状態に戻さない
        set.setFillAfter(true);

        // 0でリピートなし
        set.setRepeatCount(0);

        return set;
    }

    // （主に猫用・・・？）
    // 画像を上下運動させるアニメーション
    // rValueImage：移動させる画僧
    private Animation fuluhehhenndoCat() {
        // アニメーションの組み合わせ
        // true：補間をsetに適用　false：補間を個々に指定
        AnimationSet set = new AnimationSet(true);

        Animation trans = new TranslateAnimation(
                Animation.RELATIVE_TO_SELF,0.0f,
                Animation.RELATIVE_TO_SELF,0.0f,
                Animation.RELATIVE_TO_SELF,0.5f,
                Animation.RELATIVE_TO_SELF,-0.5f);
        trans.setDuration(1500); // 1500msかけて動く
        set.addAnimation(trans);

        // true：終了時に元の状態に戻さない
        set.setFillAfter(false);

        // 0でリピートなし
        set.setRepeatCount(0);

        return set;
    }

    // 効果音再生関数
    // targetSound：鳴らす対象のロード済み効果音
    private void playSound(int targetSound) {
        // 効果音再生
        soundPool.play(targetSound, 1.0f, 1.0f, 1, 0, 1);
    }
}