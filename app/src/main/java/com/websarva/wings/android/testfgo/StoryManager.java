package com.websarva.wings.android.testfgo;

import java.util.ArrayList;

public class StoryManager {
    private boolean storyFreezeFlg = false; // 選択肢を選ぶまでストーリーが進まないフラグ
    private ArrayList storyList_1 = new ArrayList();
    private boolean storyList_1EndedFlag = false; // storyList_1が完了したかどうか判定するフラグ
    private ArrayList storyList_2 = new ArrayList();
    private boolean storyList_2EndedFlag = false; // storyList_2が完了したかどうか判定するフラグ
    private ArrayList storyList_3 = new ArrayList();
    private boolean storyList_3EndedFlag = false; // storyList_3が完了したかどうか判定するフラグ
    private ArrayList storyList_4 = new ArrayList();
    private boolean storyList_4EndedFlag = false; // storyList_4が完了したかどうか判定するフラグ
    private ArrayList storyList_5 = new ArrayList();
    private boolean storyList_5EndedFlag = false; // storyList_5が完了したかどうか判定するフラグ
    private ArrayList storyList_6 = new ArrayList();
    private boolean storyList_6EndedFlag = false; // storyList_6が完了したかどうか判定するフラグ
    private ArrayList storyList_7 = new ArrayList();
    private boolean storyList_7EndedFlag = false; // storyList_7が完了したかどうか判定するフラグ
    private ArrayList storyList_8 = new ArrayList();
    private boolean storyList_8EndedFlag = false; // storyList_8が完了したかどうか判定するフラグ
    private ArrayList storyList_9 = new ArrayList();
    private boolean storyList_9EndedFlag = false; // storyList_9が完了したかどうか判定するフラグ
    private ArrayList storyList_10 = new ArrayList();
    private boolean storyList_10EndedFlag = false; // storyList_10が完了したかどうか判定するフラグ
    private ArrayList storyList_11 = new ArrayList();
    private boolean storyList_11EndedFlag = false; // storyList_11が完了したかどうか判定するフラグ
    private ArrayList storyList_12 = new ArrayList();
    private boolean storyList_12EndedFlag = false; // storyList_12が完了したかどうか判定するフラグ
    private ArrayList storyList_13 = new ArrayList();
    private boolean storyList_13EndedFlag = false; // storyList_13が完了したかどうか判定するフラグ
////////////
    private boolean[] storyManagerArray; // storyListEndedFlagたちを管理する配列
    private int[] storyWholeManagerArray; // storyListの中の数を全て格納する配列

    private int MaxStoryCounter;// ストーリーが完了しているかの判断に使用する「ストーリーごとのストーリー数最大値」
    private int number;// ストーリーインデクス設定（storyManager.javaのみから参照するようにする）
    private int STORY_SIZE = 100; // ストーリーリストのサイズ
    private MainActivity main = new MainActivity();

    // コンストラクタ（初期値設定）
    public StoryManager() {
        // ストーリーマネージャーにすべて突っ込む
        storyManagerArray = new boolean[STORY_SIZE];
        storyManagerArray[0] = storyList_1EndedFlag;
        storyManagerArray[1] = storyList_2EndedFlag;
        storyManagerArray[2] = storyList_3EndedFlag;
        storyManagerArray[3] = storyList_4EndedFlag;
        storyManagerArray[4] = storyList_5EndedFlag;
        storyManagerArray[5] = storyList_6EndedFlag;
        storyManagerArray[6] = storyList_7EndedFlag;
        storyManagerArray[7] = storyList_8EndedFlag;
        storyManagerArray[8] = storyList_9EndedFlag;
        storyManagerArray[9] = storyList_10EndedFlag;
        storyManagerArray[10] = storyList_11EndedFlag;
        storyManagerArray[11] = storyList_12EndedFlag;
        storyManagerArray[12] = storyList_13EndedFlag;

        // ストーリーホールマネージャーにすべて突っ込む
        storyWholeManagerArray = new int[storyManagerArray.length];
        storyWholeManagerArray[0] = storyList_1.size();
        storyWholeManagerArray[1] = storyList_2.size();
        storyWholeManagerArray[2] = storyList_3.size();
        storyWholeManagerArray[3] = storyList_4.size();
        storyWholeManagerArray[4] = storyList_5.size();
        storyWholeManagerArray[5] = storyList_6.size();
        storyWholeManagerArray[6] = storyList_7.size();
        storyWholeManagerArray[7] = storyList_8.size();
        storyWholeManagerArray[8] = storyList_9.size();
        storyWholeManagerArray[9] = storyList_10.size();
        storyWholeManagerArray[10] = storyList_11.size();
        storyWholeManagerArray[11] = storyList_12.size();
        storyWholeManagerArray[12] = storyList_13.size();

        this.MaxStoryCounter = 0;
        this.number = 0;
    }

    // 現在のストーリーリストを取得
    public ArrayList getCurrentStoryList() {
        if (this.storyList_1EndedFlag == false) {
            return storyList_1;
        } else if (this.storyList_2EndedFlag == false) {
            return storyList_2;
        } else if (this.storyList_3EndedFlag == false) {
            return storyList_3;
        } else if (this.storyList_4EndedFlag == false) {
            return storyList_4;
        } else if (this.storyList_5EndedFlag == false) {
            return storyList_5;
        } else if (this.storyList_6EndedFlag == false) {
            return storyList_6;
        } else if (this.storyList_7EndedFlag == false) {
            return storyList_7;
        } else if (this.storyList_8EndedFlag == false) {
            return storyList_8;
        } else if (this.storyList_9EndedFlag == false) {
            return storyList_9;
        } else if (this.storyList_10EndedFlag == false) {
            return storyList_10;
        } else if (this.storyList_11EndedFlag == false) {
            return storyList_11;
        } else if (this.storyList_12EndedFlag == false) {
            return storyList_12;
        } else if (this.storyList_13EndedFlag == false) {
            return storyList_13;
        }
        // 仮の値
        return storyList_1;
    }

    // 指定したのストーリーフラグを設定
    public void setCurrentStoryFlag(int number) {
        if (number == 1) {
            storyList_1EndedFlag = true;
        } else if (number == 2) {
            storyList_2EndedFlag = true;
        } else if (number == 3) {
            storyList_3EndedFlag = true;
        } else if (number == 4) {
            storyList_4EndedFlag = true;
        } else if (number == 5) {
            storyList_5EndedFlag = true;
        } else if (number == 6) {
            storyList_6EndedFlag = true;
        } else if (number == 7) {
            storyList_7EndedFlag = true;
        } else if (number == 8) {
            storyList_8EndedFlag = true;
        } else if (number == 9) {
            storyList_9EndedFlag = true;
        } else if (number == 10) {
            storyList_10EndedFlag = true;
        } else if (number == 11) {
            storyList_11EndedFlag = true;
        } else if (number == 12) {
            storyList_12EndedFlag = true;
        } else if (number == 13) {
            storyList_13EndedFlag = true;
        }
    }


    // ストーリーをセット
    {
        storyList_1.add("qqq"); // 0：不使用
        storyList_1.add("誰かが言った。qqq「神が死んだ」" + "showText"); // 1：showText
        storyList_1.add("2022年は地球が最も寒かったらしい・・・qqq「どんどん酷くなっていくね」" + "showText"); // 2：showText
        storyList_1.add("だが、本当は神は死んじゃあいなかった。qqq「じゃあ、一体どこに神がいるというのか？このフヌケ野郎？」" + "showText");// 3：showText
        storyList_1.add("神は俺だ！！qqq" + "showText");// 4：showText
        storyList_1.add(R.drawable.boy_1 + "changeView"); // 5：changeView - （中学生男子画像表示）
        storyList_1.add("ドーン！！qqq" + "setChooseText");// 6：setChooseText - (ドーン！！ "")
        storyList_1.add(R.raw.yespartytime + "playFromMediaPlayer"); // 7：playFromMediaPlayer - (yespartytime)
        storyList_1.add("qqq" + "viewChooseButton(1)");// 8：viewChooseButton(1) - (←　6:ドーン！！)(storyList_2へ移動)
//////        storyList_1EndedFlag = true; // storyList_1が完了したかどうか判定するフラグ：true
    }
    {
        storyList_2.add("ClearText"); // 0：クリアテキスト
        storyList_2.add(R.drawable.bakuhatu + "changeView"); // 1：changeView - （爆発画像表示）
        storyList_2.add("STOP"); // 2：STOP - (スキップ停止)
        storyList_2.add(R.drawable.black + "changeView"); // 3：changeView - （黒幕）
        storyList_2.add("STOP"); // 4：STOP - (スキップ停止)
        storyList_2.add(R.drawable.tar_kaja_quest_title + "changeView"); // 5：changeView - （タルカジャの冒険タイトル画像表示）
        storyList_2.add("STOP"); // 6：STOP - (スキップ停止)
//////        storyList_2EndedFlag = true; // storyList_2が完了したかどうか判定するフラグ：true
    }

    // 選択肢を選ぶまでストーリーが進まないフラグを取得
    public boolean getStoryFreezeFlg() {
        return storyFreezeFlg;
    }

    // 選択肢を選ぶまでストーリーが進まないフラグを自由にセット
    public void setStoryFreezeFlg(boolean flg) {
        storyFreezeFlg = flg;
    }

    // 現在のストーリー分岐を取得
    public int getCurrentStory() {
        if (this.storyList_1EndedFlag == false) {
            return 1;
        } else if (this.storyList_2EndedFlag == false) {
            return 2;
        } else if (this.storyList_3EndedFlag == false) {
            return 3;
        } else if (this.storyList_4EndedFlag == false) {
            return 4;
        } else if (this.storyList_5EndedFlag == false) {
            return 5;
        } else if (this.storyList_6EndedFlag == false) {
            return 6;
        } else if (this.storyList_7EndedFlag == false) {
            return 7;
        } else if (this.storyList_8EndedFlag == false) {
            return 8;
        } else if (this.storyList_9EndedFlag == false) {
            return 9;
        } else if (this.storyList_10EndedFlag == false) {
            return 10;
        } else if (this.storyList_11EndedFlag == false) {
            return 11;
        } else if (this.storyList_12EndedFlag == false) {
            return 12;
        } else if (this.storyList_13EndedFlag == false) {
            return 13;
        }
        // 仮の値
        return 999;
    }

    // 自動でストーリーフラグをtrueにセット
    public void setCurrentStoryFlagAuto() {
        setCurrentStoryFlag(getCurrentStory());
        // ストーリフラグを変更したので初めから読む
        this.number = 0;
    }

    // 指定したストーリーフラグをtrueにセット
    public void setCurrentStory(int storyNumber) {
        setCurrentStoryFlag(storyNumber);
        // ストーリフラグを変更したので初めから読む
        this.number = 0;
    }

    // 指定番号のストーリーをゲット
    // ストーリーが変更されたら、使用元のnumber（現状MainActivityのみ・・・）をクリアして整合性を保つようにすること
    // =>　同クラスのgetStorySize()にて調整すること！！
    public Object getStory(int number) {
        if (storyManagerArray.length < getCurrentStory()) {
            setCurrentStoryFlagAuto();
            return "ストーリーはおしまいだよ";
        } else {
            // 対象のストーリが終了（オーバー）していないか判定（storyList_1）
            if (storyWholeManagerArray[getCurrentStory() - 1] > this.number) {
                // ストーリー内容を取得
                return getCurrentStoryList().get(this.number).toString();
            } else {
                // オーバーしていたらストーリーフラグを変更する
                setCurrentStoryFlagAuto();
            }
        }
        return 999999;
    }

    // 指定番号のストーリーをゲット
    // ストーリーが変更されたら、使用元のnumber（現状MainActivityのみ・・・）をクリアして整合性を保つようにすること
    // =>　同クラスのgetStorySize()にて調整すること！！
    public Object getOldStory() {
        if (storyManagerArray.length < getCurrentStory()) {
            setCurrentStoryFlagAuto();
            return "ストーリーはおしまいだよ";
        } else {
            // 対象のストーリが終了（オーバー）していないか判定（storyList_1）
            if (storyWholeManagerArray[getCurrentStory() - 1] > this.number) {
                // MainActivityのfirstFlagがtrueの場合は初回なので読まない
                if (main.getFirstFlag()) {
                    // ストーリー内容を取得
                    return getCurrentStoryList().get(this.number - 1).toString();
                // そうでない場合はtrueに設定する
                } else {
                    main.setFirstFlag(true);
                }
            }
        }
        return 999999;
    }

    // ストーリーの数を確認
    public int getStorySize() {
        // ストーリーが完了していたら、別のストーリーをゲットする
        if (!storyList_1EndedFlag) {
            return storyList_1.size();
        }
        if (!storyList_2EndedFlag) {
            return storyList_2.size();
        }
        return 99999;
    }

    // ストーリーカウンターのセッター
    // targetStorySize：対象のストーリーリストのサイズ。これをstoryCounterに入れて判定する。
    public void setterMaxStoryCounter(int targetStorySize) {
        this.MaxStoryCounter = targetStorySize;
    }

    // ストーリーカウンターをクリアする。
    // （次のストーリーのために、全てのストーリーカウンターをクリアしておく）
    public void clearAllStoryCounter() {
        this.MaxStoryCounter = 0;
    }

    // ストーリーインデクス(Number)を取得する関数
    public int getNumber() {
        return this.number;
    }

    // ストーリーインデクス(Number)を設定する関数
    public void setNumber(int number) {
        this.number = number;
    }
}
