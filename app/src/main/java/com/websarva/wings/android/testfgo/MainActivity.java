package com.websarva.wings.android.testfgo;

import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.strictmode.IntentReceiverLeakedViolation;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.ScaleAnimation;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import org.w3c.dom.Text;

import java.nio.file.Path;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static android.content.ContentValues.TAG;

public class MainActivity extends AppCompatActivity {
    private Util util = new Util(); // Utilクラスのインスタンス
    private SoundPool soundPool;
    private MediaPlayer mediaPlayer;
    private Button chooseButton_1;
    private Button chooseButton_2;
    private TextView textViewUp;
    private TextView textViewDown;
    private ImageView iconImage;
    private ImageView animeImage;
    private int choose_sound_1; // ボタン選択効果音＿１
    private int TEXT_SPEED_INTERVAL = 100; // 文字列のsetText()インターバル
    private Handler handler; // 別スレッドから実行するHandlerインスタンス
    private Animation anim; // アニメーション用インスタンス
    private CountDownLatch countDownLatch;
    private Boolean touchFlag = false; // タッチイベント用のフラグ
    private CyclicBarrier barrier; // サイクリックバリア
    private int text = 0;// 出力テキスト数
    private int number = 0; // 対象のsetText
    private StoryManager story; // ストーリーマネージャー関数
    private boolean firstFlag = true; // ストーリーを最初は読める
    private MagicManager magic = new MagicManager();// マジックマネージャのインスタンス
    private AudioManager audio;
    private int ringVolume = 0;
    private int ringMaxVolume = 0;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 初期設定
        setInit();

        // ↓↓↓ストーリー仮置き１章（消さずに保存しておくこと！！）ーーーーーーー

        changeView(R.drawable.black);

        playFromMediaPlayer(R.raw.identity);

//        startBattle();

        popStoryText();

        // ↑↑↑ストーリー仮置き１章（消さずに保存しておくこと！！）ーーーーーーー
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

    //
    private void setClickListeners() {
        // ボタンに効果音を割り当てる
        chooseButton_1.setOnClickListener(v -> {
            // ボタン選択効果音を鳴らすように設定
            playSound(choose_sound_1);

            // 選択肢を選ぶまで凍らせるフラグをfalseにして解除（共通処理）
            story.setStoryFreezeFlg(false);

            // 選択肢を消す（共通処理）
            viewDisappearButton();
        });

        // ボタンに効果音を割り当てる
        chooseButton_2.setOnClickListener(v -> {
            // ボタン選択効果音を鳴らすように設定
            playSound(choose_sound_1);

            // 選択肢を選ぶまで凍らせるフラグをfalseにして解除（共通処理）
            story.setStoryFreezeFlg(false);

            // 選択肢を消す（共通処理）
            viewDisappearButton();
        });

        // 仮タッチ時にイベントが進むようにする
        // ポップしたものが文字列の場合ー＞テキストストーリー
        // ポップしたものがR値の場合ー＞エフェクト？ストーリー
        animeImage.setOnClickListener(v -> {
            execPopStoryText();
        });

        // 仮タッチ時にイベントが進むようにする
        textViewUp.setOnClickListener(v -> {
            execPopStoryText();
        });

        // 仮タッチ時にイベントが進むようにする
        textViewDown.setOnClickListener(v -> {
            execPopStoryText();
        });
    }

    // 初期設定
    private void setInit() {
        // ストーリーマネージャーインスタンス生成
        story = new StoryManager();

        // レイアウトファイル読み込み
        setContentView(R.layout.activity_main);

        // 背景画像差し替え
        changeView(R.drawable.karafuto_shishamo);

        // Handlerインスタンス生成
        handler = new Handler();

        // サイクリックバリアインスタンスを生成
        barrier = new CyclicBarrier(1, null);

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

        // ボタン初期設定
        chooseButton_1 = (Button) findViewById(R.id.button1);
        chooseButton_2 = (Button) findViewById(R.id.button2);

        // アニメ画面設定
        animeImage = (ImageView) findViewById(R.id.imageView);
        textViewUp = (TextView) findViewById(R.id.textView_upper);
        textViewDown = (TextView) findViewById(R.id.textView_downer);

        // 画面クリック時イベント設定
        setClickListeners();

        // アイコン読み込み
        iconImage = (ImageView) findViewById(R.id.view_icon);

        // 仮置き
        SoundManager soundManager = new SoundManager();
        soundManager.initSound();
    }

    // 戦闘処理
    private void startBattle() {
        Intent intent = new Intent(this, BattleActivity.class);
        startActivity(intent);
        // 音楽も多重起動しないようにする
        stopFromMediaPlayer();

        // このメインアクティビティを終了（多重起動しないようにする）
        finish();
    }

    // 間を持たせるためにシーンの合間合間に１秒待機する関数
    // （置き場所に注意！）（未設定）
    private void waitSecond() {
        try {
            story.setStoryFreezeFlg(true);
            removeClickListeners();
            Thread.sleep(1000);
            setClickListeners();
            story.setStoryFreezeFlg(false);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    // クリックイベント一時削除処理
    private void removeClickListeners() {
        animeImage.setOnClickListener(null);
        textViewUp.setOnClickListener(null);
        textViewDown.setOnClickListener(null);
    }

    // 1秒間待機させる関数
    private void sleeping() {
        try {
            Thread.sleep(1000);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    // popStory関数を実行する中身の関数
    // popStory関数を直すときはここを直す
    private void execPopStoryText(){
        // 選択肢を選ぶまで凍らせるフラグがfalseの時にしか実行できない
        if (story.getStoryFreezeFlg() == false) {

            if (firstFlag && story.getStory(number) instanceof String) {
                // changeViewがストーリーの末尾に存在するとき（来ないと思う）
                if (((String) story.getStory(number)).indexOf("changeView") != -1) {
                    // 末尾のコマンド文字列をトリム
                    String tmpStrValue = ((String) story.getStory(number)).replace("changeView", "");
                    int tmpRValue = Integer.parseInt(tmpStrValue);

                    changeView(tmpRValue);
                    // showTextがストーリーの末尾に存在するとき
                } else if (((String) story.getStory(number)).indexOf("showText") != -1) {
                    // 末尾のコマンド文字列をトリム
                    showText(story.getStory(number).toString().replace("showText", ""));
                    // ストーリーを最後まで見ないと読めないようにする
                }
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // showTextを連続で実行しているときの連打制御用if文
            } else if (((String) story.getStory(number)).indexOf("changeView") != -1) {
                // 末尾のコマンド文字列をトリム
                String tmpStrValue = ((String) story.getStory(number)).replace("changeView", "");
                int tmpRValue = Integer.parseInt(tmpStrValue);

                changeView(tmpRValue);
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // 自分で自分を呼ぶことで自動的に次に進む
                execPopStoryText();
                // ViewChooseButtonがストーリーの末尾に存在するとき
            } else if (((String) story.getStory(number)).indexOf("viewChooseButton") != -1) {
                // 引数によって分岐
                if (((String) story.getStory(number)).indexOf("viewChooseButton(1)") != -1) {
                    viewChooseButton(1);
                } else if (((String) story.getStory(number)).indexOf("viewChooseButton(2)") != -1) {
                    viewChooseButton(2);
                }
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // テキストをクリアする
            } else if (((String) story.getStory(number)).indexOf("ClearText") != -1) {
                // 末尾のコマンド文字列をトリム
                String tmpStrValue = ((String) story.getStory(number)).replace("ClearText", "");

                // clearText()呼び出し
                ClearText();
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
            } else if ((story.getOldStory().toString().replace("qqq", "").replace("changeView", "")
                    .replace("showText", "").equals(textViewUp.getText().toString() + textViewDown.getText().toString()))) {
                // showTextがストーリーの末尾に存在するとき
                if (((String) story.getStory(number)).indexOf("showText") != -1) {
                    // 末尾のコマンド文字列をトリム
                    showText(story.getStory(number).toString().replace("showText", ""));
                }
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // 自分で自分を呼ぶことで自動的に次に進む
                execPopStoryText();
                // setChooseTextがストーリーの末尾に存在するとき
            } else if (((String) story.getStory(number)).indexOf("setChooseText") != -1) {
                // 末尾のコマンド文字列をトリム
                String tmpStrValue = ((String) story.getStory(number)).replace("setChooseText", "");

                // それぞれの引数を「qqq」またぎで分割して関数に代入
                String firstStr1 = tmpStrValue.substring(0, tmpStrValue.indexOf("qqq"));
                String firstStr2 = tmpStrValue.substring(tmpStrValue.indexOf("qqq") + 3);

                // setChooseText()呼び出し
                setChooseText(firstStr1, firstStr2);
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // 自分で自分を呼ぶことで自動的に次に進む
                execPopStoryText();
                // 音楽を変更するとき「playFromMediaPlayer」
            } else if (((String) story.getStory(number)).indexOf("playFromMediaPlayer") != -1) {
                // 音楽を消してから別のを鳴らす
                stopFromMediaPlayer();

                // 末尾のコマンド文字列をトリム
                String tmpStrValue = ((String) story.getStory(number)).replace("playFromMediaPlayer", "");
                // R値に変換
                int playTarget_R = Integer.parseInt(tmpStrValue);

                // playFromMediaPlayer()呼び出し
                playFromMediaPlayer(playTarget_R);
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // 自分で自分を呼ぶことで自動的に次に進む
                execPopStoryText();
                // テキストをクリアする
            } else if (((String) story.getStory(number)).indexOf("ClearText") != -1) {
                // 末尾のコマンド文字列をトリム
                String tmpStrValue = ((String) story.getStory(number)).replace("ClearText", "");

                // clearText()呼び出し
                ClearText();
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
            } else if (((String) story.getStory(number)).indexOf("sleeping") != -1) {
                // 末尾のコマンド文字列をトリム
                String tmpStrValue = ((String) story.getStory(number)).replace("sleeping", "");

                // sleeping()呼び出し
                sleeping();
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
                // 自分で自分を呼ぶことで自動的に次に進む
                execPopStoryText();
                // １命令待機させる「STOP」（飛ばさないようにしたいときに使う）
            } else if (((String) story.getStory(number)).indexOf("STOP") != -1) {
                firstFlag = false;
                story.setNumber(story.getNumber() + 1);
            }
        }
        Log.e("tag", story.getStory(number - 2).toString().replace("qqq", "").replace("changeView", "")
                .replace("showText", ""));
        Log.e("tag", textViewUp.getText().toString() + textViewDown.getText().toString());
    }


    // 画面タッチしたらストーリーをポップする
    private void popStoryText() {
        // ストーリー最後まで行ったら終了させるガードのif文
        if (story.getStorySize() == number) {
            Log.v("tag", "ストーリーはおしまいだよ");
            System.exit(999);
        }
        // 現在のnumberをストーリーマネージャーに聞きに行く
        number = story.getNumber();

        // 仮タッチ時にイベントが進むようにする
        // ポップしたものが文字列の場合ー＞テキストストーリー
        // ポップしたものがR値の場合ー＞エフェクト？ストーリー
        animeImage.setOnClickListener(v -> {
            execPopStoryText();
        });

        // 仮タッチ時にイベントが進むようにする
        textViewUp.setOnClickListener(v -> {
            execPopStoryText();
        });

        // 仮タッチ時にイベントが進むようにする
        textViewDown.setOnClickListener(v -> {
            execPopStoryText();
        });
    }

    // StoryManager用にFirstFlagの取得関数
    public boolean getFirstFlag() {
        return this.firstFlag;
    }

    // StoryManager用にFirstFlagの設定関数
    public void setFirstFlag(boolean flag) {
        this.firstFlag = flag;
    }

    // Resume時の動作
    public void onResume() {
        // 音楽再開
        super.onResume();
        if (mediaPlayer == null) {
            playFromMediaPlayer(R.raw.onyourmark);
        }
    }

    // 背景画像の差し替え
    // rValueView : 差し替える画像のR値
    private void changeView(int rValueView) {
        ImageView myImage = findViewById(R.id.imageView);
        Drawable myDrawable = getResources().getDrawable(rValueView);
        myImage.setImageDrawable(myDrawable);
    }

    // アイコン画像の差し替え
    // rValueView : 差し替えるアイコン画像のR値
    private void changeIconView(int rValueView) {
        ImageView myImage = findViewById(R.id.view_icon);
        Drawable myDrawable = getResources().getDrawable(rValueView);
        myImage.setImageDrawable(myDrawable);
    }

    // 選択ボタンを表示させる
    // count：1~2
    private void viewChooseButton(int count) {
        if (count == 1) {
            chooseButton_1.setVisibility(View.VISIBLE);
            // 選択肢を選ぶまで凍らせる
            story.setStoryFreezeFlg(true);
        } else if (count == 2) {
            chooseButton_1.setVisibility(View.VISIBLE);
            chooseButton_2.setVisibility(View.VISIBLE);
            // 選択肢を選ぶまで凍らせる
            story.setStoryFreezeFlg(true);
        } else {
            // 値を間違えている場合は処理をしない
        }
    }

    // 選択ボタンを全て消滅させる
    private void viewDisappearButton() {
        chooseButton_1.setVisibility(View.INVISIBLE);
        chooseButton_2.setVisibility(View.INVISIBLE);
    }

    // 選択テキストをセットする
    // str1：上の選択テキスト
    // str2：下の選択テキスト
    private void setChooseText(String str1, String str2) {
        // クリア
        chooseButton_1.setText(str1);
        chooseButton_2.setText(str2);
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
    // number: 対象のshowText（仮）
    public void showText(String targetString) {

        // テキストをクリアする
        ClearText();

        // Runnable初期化
        Runnable runnable = new Runnable() {
            @Override
            public void run() {

            }
        };

        // テキストビュー取得
        textViewUp = findViewById(R.id.textView_upper);
        textViewDown = findViewById(R.id.textView_downer);

        // 別スレッドでの処理を登録
        ExecutorService service = Executors.newFixedThreadPool(1);

        // targetString1_2生成
        String targetString_1 = "";
        String targetString_2 = "";

        targetString_1 = targetString.substring(0, targetString.indexOf("qqq"));
        targetString_2 = targetString.substring(targetString.indexOf("qqq") + 3);

        // タスク登録
        runnable = taskWaiting(targetString_1, targetString_2, barrier, runnable);

        // タスク登録
        service.submit(runnable);

    }

    // add・removeEventListener用の関数
    private void clickEventListener() {
        // ボタン選択効果音を鳴らすように設定
        playSound(choose_sound_1);
        touchFlag = true;
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

    // 効果音再生関数
    // targetSound：鳴らす対象のロード済み効果音
    private void playSound(int targetSound) {
        // 効果音再生
        // AudioManagerを取得する
        audio = (AudioManager)getSystemService(Context.AUDIO_SERVICE);

        // 現在の音量を取得する
        ringVolume = audio.getStreamVolume(AudioManager.STREAM_ALARM);

        // ストリームごとの最大音量を取得する
        ringMaxVolume = audio.getStreamMaxVolume(AudioManager.STREAM_ALARM);

        soundPool.play(targetSound, ringVolume, ringVolume, 1, 0, 1);
    }

    // （showText内で使用）
    private Runnable taskWaiting(String targetString_1, String targetString_2, CyclicBarrier barrier, Runnable runnable) {

        // 結合テキスト
        String mergeText = targetString_1 + targetString_2;

        // テキスト出力用の変数
        String outputText = "";

        // 2行目用のカウント変数
        int kk = 0;

        // 全ての文字を表示させるまで繰り返す
        for (int k = 0; k <= mergeText.length(); k++) {

            // 別スレッドからPOSTを実行
            // 行番号に応じてテキストを表示させる
            outputText = mergeText.substring(0, k);

            // 2行目の場合はtargetString_2の引数で出力用変数を上書き
            if (k > targetString_1.length()) {
                kk++;
                outputText = targetString_2.substring(0, kk);
            }

            if (k <= targetString_1.length()) {
                String finalOutputText = outputText;
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        textViewUp.setText(finalOutputText);
                        // 再描画して文字を即表示させる
                        textViewUp.invalidate();
                        while (textViewUp.getText().toString().length() == targetString_1.length()) {
                            CyclicbarrierAwait(barrier);
                            break;
                        }
                    }
                }, k * TEXT_SPEED_INTERVAL);
            } else if (k > targetString_1.length()) {
                String finalOutputText1 = outputText;
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        textViewDown.setText(finalOutputText1);
                        // 再描画して文字を即表示させる
                        textViewDown.invalidate();
                        while (true) {
                            if (textViewDown.getText().toString().length() == targetString_2.length())
                            CyclicbarrierAwait(barrier);
                            break;
                        }
                    }
                }, k * TEXT_SPEED_INTERVAL);
            }
        }
        // 仮待機
        CyclicbarrierAwait(barrier);

        return runnable;
    }

    // CyclicBarrierのawait処理
    private void CyclicbarrierAwait(CyclicBarrier barrier) {
        try {
            barrier.await();
        } catch (BrokenBarrierException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}