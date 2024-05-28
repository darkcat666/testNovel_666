package com.websarva.wings.android.testfgo;

import java.util.ArrayList;
import java.util.List;

public class StoryManager {
    private boolean storyFreezeFlg = false; // 選択肢を選ぶまでストーリーが進まないフラグ
    private ArrayList storyList_1 = new ArrayList();
    private ArrayList storyList_2 = new ArrayList();
    private ArrayList storyList_3 = new ArrayList();
    private ArrayList storyList_4 = new ArrayList();
    private ArrayList storyList_5 = new ArrayList();
    private ArrayList storyList_6 = new ArrayList();
    private ArrayList storyList_7 = new ArrayList();
    private ArrayList storyList_8 = new ArrayList();
    private ArrayList storyList_9 = new ArrayList();
    private ArrayList storyList_10 = new ArrayList();
    private ArrayList storyList_11 = new ArrayList();
    private ArrayList storyList_12 = new ArrayList();
    private ArrayList storyList_13 = new ArrayList();
    private ArrayList WholeStoryList = new ArrayList();
    private int storyFlagNumber = 1; // ストーリーが終わったフラグをintで管理
    private int number;// ストーリーインデクス設定（storyManager.javaのみから参照するようにする）
    private String pastMessage; // １つ前のメッセージをセット

    // コンストラクタ（初期値設定）
    public StoryManager() {
        // ストーリーホールマネージャーにすべて突っ込む
        WholeStoryList.add(storyList_1);
        WholeStoryList.add(storyList_2);
        WholeStoryList.add(storyList_3);
        WholeStoryList.add(storyList_4);
        WholeStoryList.add(storyList_5);
        WholeStoryList.add(storyList_6);
        WholeStoryList.add(storyList_7);
        WholeStoryList.add(storyList_8);
        WholeStoryList.add(storyList_9);
        WholeStoryList.add(storyList_10);
        WholeStoryList.add(storyList_11);
        WholeStoryList.add(storyList_12);
        WholeStoryList.add(storyList_13);
        this.number = 0;
    }

    // 現在のストーリーリストを取得
    public ArrayList getCurrentStoryList(int storyFlagNumber) {
        return (ArrayList) WholeStoryList.get(storyFlagNumber - 1);
    }

    // 指定したストーリーフラグを設定
    public void setStoryFlag(int number) {
        this.storyFlagNumber = number;
    }

    // ストーリーをセット
        {
            storyList_1.add("あなたは１８歳になったある日、王様に呼ばれましたqqq" + "showText" + "vvv1");
            storyList_1.add(R.drawable.__think + "changeView");
            storyList_1.add("王様のいう、示談の条件とはいったい何なのでしょうか？qqq" + "showText" + "vvv2");
            storyList_1.add(R.drawable.__sennpai + "changeView");
            storyList_1.add("「おい、てめえ。俺の●●●を舐めろ」qqq" + "showText" + "vvv3");
            storyList_1.add(R.drawable.__zun +"changeView");
            storyList_1.add("王様の●●●を舐めますか？qqq" + "showText" + "vvv4");
            storyList_1.add("はいqqqいいえ" + "viewChooseButton(2)");

            storyList_2.add(R.drawable.__gameover + "changeView");
            storyList_2.add("GAMEOVERqqq"+"showText"+"vvv0");
            storyList_2.add("GAMEOVERqqq"+"showText"+"vvv0");

            storyList_3.add("「おい、てめえ。俺の●●●を舐めろ」qqq" + "showText" + "vvv3");
            storyList_3.add(R.drawable.__zun +"changeView");
            storyList_3.add("王様の●●●を舐めますか？qqq" + "showText" + "vvv4");
            storyList_3.add("はいqqqいいえ" + "viewChooseButton(2)");

            storyList_4.add(R.raw.onyourmark + "playFromMediaPlayer");
            storyList_4.add("では、王様を殺しますか？qqq" + "showText" + "vvv5");
            storyList_4.add("はいqqqいいえ" + "viewChooseButton(2)");

            storyList_5.add("イクイクイクイクイクー、あの世まで行くーイッター！！qqq" + "showText" + "vvv6");
            storyList_5.add(R.drawable.__gameclear + "changeView");
            storyList_5.add("GＡＭＥ　ＣＬＥＡＲ！！qqq" + "showText" + "vvv7");
            storyList_5.add("GＡＭＥ　ＣＬＥＡＲ！！qqq" + "showText" + "vvv7");
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
        return this.storyFlagNumber;
    }

    // 指定番号のストーリーをゲット
    // ストーリーが変更されたら、使用元のnumber（現状MainActivityのみ・・・）をクリアして整合性を保つようにすること
    // =>　同クラスのgetStorySize()にて調整すること！！
    public Object getStory() {
        // ストーリー内容を取得
        return getCurrentStoryList(storyFlagNumber).get(this.number).toString();
    }

    // 指定番号のストーリーをゲット
    // ストーリーが変更されたら、使用元のnumber（現状MainActivityのみ・・・）をクリアして整合性を保つようにすること
    // =>　同クラスのgetStorySize()にて調整すること！！
    public Object getOldStory() {
        if (pastMessage != null) {
            return this.pastMessage.replace("qqq", "");
        } else {
            return null;
        }
    }
    // ストーリーインデクス(Number)を取得する関数
    public int getNumber() {
        return this.number;
    }

    // ストーリーインデクス(Number)を設定する関数
    public void setNumber(int number) {
        this.number = number;
    }

    // 1つ前のメッセージをセット
    public void setPastMessage(String message) {
        this.pastMessage = message;
    }

    // ストーリーフラグを設定
    public void setStoryFlagNumber(int number) {
        this.storyFlagNumber = number;
    }
}
