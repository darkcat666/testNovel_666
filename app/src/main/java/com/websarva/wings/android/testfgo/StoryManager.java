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

//    // ストーリーをセット（１：野獣先輩）
//        {
//            storyList_1.add("あなたは１８歳になったある日、王様に呼ばれましたqqq" + "showText" + "vvv1" + "setTextColor" + "#000000");
//            storyList_1.add(R.drawable.__think + "changeView");
//            storyList_1.add("王様のいう、示談の条件とはいったい何なのでしょうか？qqq" + "showText" + "vvv2");
//            storyList_1.add(R.drawable.__sennpai + "changeView");
//            storyList_1.add("「おい、てめえ。俺の●●●を舐めろ」qqq" + "showText" + "vvv3");
//            storyList_1.add(R.drawable.__zun +"changeView");
//            storyList_1.add("王様の●●●を舐めますか？qqq" + "showText" + "vvv4");
//            storyList_1.add("なめるqqqなめない" + "viewChooseButton(2)");
//
//            storyList_2.add(R.drawable.__gameover + "changeView");
//            storyList_2.add("GAMEOVERqqq"+"showText"+"vvv0");
//            storyList_2.add("GAMEOVERqqq"+"showText"+"vvv0");
//
//            storyList_3.add("「おい、てめえ。俺の●●●を舐めろ」qqq" + "showText" + "vvv3");
//            storyList_3.add(R.drawable.__zun +"changeView");
//            storyList_3.add("王様の●●●を舐めますか？qqq" + "showText" + "vvv4");
//            storyList_3.add("なめるqqqなめない" + "viewChooseButton(2)");
//
//            storyList_4.add(R.raw.onyourmark + "playFromMediaPlayer");
//            storyList_4.add(R.drawable.__satui + "changeView");
//            storyList_4.add("では、王様を殺しますか？qqq" + "showText" + "vvv5" + "setTextColor" + "#000000");
//            storyList_4.add("ころすqqqいや、なめる" + "viewChooseButton(2)");
//
//            storyList_5.add("イクイクイクイクイクー、あの世まで行くーイッター！！qqq" + "showText" + "vvv6" + "setTextColor" + "#000000");
//            storyList_5.add(R.drawable.__gameclear + "changeView");
//            storyList_5.add("GＡＭＥ　ＣＬＥＡＲ！！qqq" + "showText" + "vvv7");
//            storyList_5.add("GＡＭＥ　ＣＬＥＡＲ！！qqq" + "showText" + "vvv7");
//        }

    // ストーリーをセット（２；ノック）
    {
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10" + "setTextColor" + "#FFFF00");
        storyList_1.add(R.raw.onyourmark + "playFromMediaPlayer");
        storyList_1.add(R.drawable.nock_11 + "changeView");
        storyList_1.add("ノックは、旅に出たqqq" + "showText" + "vvv11");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable.king_12 + "changeView");
        storyList_1.add("ノックは、レイドック王の城についたqqq" + "showText" + "vvv12");
        storyList_1.add(R.drawable.sokkin_13 + "changeView");
        storyList_1.add("側近：ささ、ノック様。こちらです。qqq" + "showText" + "vvv13");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable.see_14 + "changeView");
        storyList_1.add("ノックは視線を感じた・・・qqq" + "showText" + "vvv14");
        storyList_1.add(R.drawable.gyousi_15 + "changeView");
        storyList_1.add("なんと、ノックの股間を城の兵士が凝視している！qqq" + "showText" + "vvv15");
        storyList_1.add(R.drawable.bokki_16 + "changeView");
        storyList_1.add("ノックは、ボッキしてしまった！！qqq" + "showText" + "vvv16");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable.yokukita_17 + "changeView");
        storyList_1.add("レイドック王：さあ、よくぞ来た。ノックよ。qqq" + "showText" + "vvv17");
        storyList_1.add(R.drawable.haha_18 + "changeView");
        storyList_1.add("ノック：ははっ、王よ。qqq" + "showText" + "vvv18");
        storyList_1.add(R.drawable.mudo__19 + "changeView");
        storyList_1.add("レイドック王：ノックよ、貴様にははるか南の地に住む大鬼（ムドー）をqqq退治してもらいたい。" + "showText" + "vvv19");
        storyList_1.add(R.drawable.oosenotoori_20 + "changeView");
        storyList_1.add("ノック：ははっ、仰せの通りに。qqq" + "showText" + "vvv20");
        storyList_1.add(R.drawable.onitaiji_21 + "changeView");
        storyList_1.add("ノックは、南の地に住む大鬼の自宅に向かおうとした・・・qqq" + "showText" + "vvv21");
        storyList_1.add(R.drawable.mate_22 + "changeView");
        storyList_1.add("レイドック王：ノック、待つのだ。qqq" + "showText" + "vvv22");
        storyList_1.add(R.drawable.gimon_23 + "changeView");
        storyList_1.add("ノック：ははっ、王よ。いかがされたか？qqq" + "showText" + "vvv23");
        storyList_1.add(R.drawable.bokkinositeki__24 + "changeView");
        storyList_1.add("レイドック王：ノック、貴様・・・なぜ城の中でボッキしている？？？qqq" + "showText" + "vvv24");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable.iiwake_25 + "changeView");
        storyList_1.add("ノック：王よ、これは・・・qqq" + "showText" + "vvv25");
        storyList_1.add(R.drawable.ikari_26 + "changeView");
        storyList_1.add("レイドック王：ノックよ。qqq" + "showText" + "vvv26");
        storyList_1.add(R.drawable.koreha_27 + "changeView");
        storyList_1.add("ノック：これは・・・qqq" + "showText" + "vvv27");
        storyList_1.add(R.drawable.nokku_28 + "changeView");
        storyList_1.add("レイドック王：ノックよ。qqq" + "showText" + "vvv28");
        storyList_1.add(R.drawable.zetubou_29 + "changeView");
        storyList_1.add("ノック：レイドック王、これは・・・qqq" + "showText" + "vvv29");
        storyList_1.add(R.drawable.bananakiru_30 + "changeView");
        storyList_1.add("レイドック王：お前の・・・チンコを・・・切る。qqq" + "showText" + "vvv30");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable.hasamidekikru_31 + "changeView");
        storyList_1.add("レイドック王はおもむろにノックのチンコをハサミで切った！！qqq" + "showText" + "vvv31");
        storyList_1.add(R.drawable.onnnaninatte_32 + "changeView");
        storyList_1.add("ノックは女になってしまった！！qqq" + "showText" + "vvv32");
        storyList_1.add(R.drawable._33_reidokkuouyo + "changeView");
        storyList_1.add("ノック（女）：レイドック王よ・・・qqq" + "showText" + "vvv33");
        storyList_1.add(R.drawable._34_reidokkuou + "changeView");
        storyList_1.add("レイドック王：さあ、ノック（女）よ！qqq" + "showText" + "vvv34");
        storyList_1.add(R.drawable._35_reidokkuou + "changeView");
        storyList_1.add("ノック（女）：レイドック王・・・qqq" + "showText" + "vvv35");
        storyList_1.add(R.drawable._36_minianmaioi + "changeView");
        storyList_1.add("レイドック王：南の地に住む大鬼を、見事討ち倒して参れ！qqq" + "showText" + "vvv36");
        storyList_1.add(R.drawable._37_omaenotinok + "changeView");
        storyList_1.add("ノック（女）：レイドック王・・・お前の・・・チンコを・・・切る。qqq" + "showText" + "vvv37");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable._38_nokkuhatinnkokitta + "changeView");
        storyList_1.add("ノックは、レイドック王のチンコを切った！qqq" + "showText" + "vvv38");
        storyList_1.add(R.drawable._39_nyotaika + "changeView");
        storyList_1.add("レイドック王は女になってしまった！qqq" + "showText" + "vvv39");
        storyList_1.add(R.drawable._40 + "changeView");
        storyList_1.add("さらに、ノック（女）はレイドック王のチンコを移植した！qqq" + "showText" + "vvv40");
        storyList_1.add(R.drawable._41_nanntiaka + "changeView");
        storyList_1.add("ノックは男に戻った！！qqq" + "showText" + "vvv41");
        storyList_1.add(R.drawable._42_oooni + "changeView");
        storyList_1.add("ノック：では、レイドック王（女）。南の地に住む大鬼を退治して参ります。qqq" + "showText" + "vvv42");
        storyList_1.add(R.drawable._43_honntounowatasi + "changeView");
        storyList_1.add("レイドック王（女）：これが、本当のわたし・・・？qqq" + "showText" + "vvv43");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable._44_irasshai + "changeView");
        storyList_1.add("ムドー：ははは、ノックよ。よくぞ来たな。qqq" + "showText" + "vvv44");
        storyList_1.add(R.drawable._45_onikorosi + "changeView");
        storyList_1.add("ノック：ムドー、貴様を倒しに来た！！qqq" + "showText" + "vvv45");
        storyList_1.add(R.drawable._46_k_illmie + "changeView");
        storyList_1.add("ムドー：貴様にこの我を倒すことができるか？？qqq" + "showText" + "vvv46");
        storyList_1.add(R.drawable._47_mudo_ + "changeView");
        storyList_1.add("ノック：ムドーqqq" + "showText" + "vvv47");
        storyList_1.add(R.drawable._48_gyousu + "changeView");
        storyList_1.add("ノックは、ムドーの股間をじっと見つめた・・・qqq" + "showText" + "vvv48");
        storyList_1.add(R.drawable._49lkyoufu + "changeView");
        storyList_1.add("ムドー：ノックよ。なぜ我の股間を凝視するのか？？qqq" + "showText" + "vvv49");
        storyList_1.add(R.drawable._50_bokki + "changeView");
        storyList_1.add("ムドーは大きく勃起してしまった！！qqq" + "showText" + "vvv50");
        storyList_1.add(R.drawable._51_sennseikougeki + "changeView");
        storyList_1.add("（先制攻撃のチャンス！）qqq" + "showText" + "vvv51");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable._52_cut + "changeView");
        storyList_1.add("ノックは、ムドーのチンコを切り取った！qqq" + "showText" + "vvv52");
        storyList_1.add(R.drawable._53_itai + "changeView");
        storyList_1.add("ムドー：ぎゃああ、痛い！からだがもえるうううう！qqq" + "showText" + "vvv53");
        storyList_1.add(R.drawable._54_mudo_niosi + "changeView");
        storyList_1.add("ムドーは死んでしまった！qqq" + "showText" + "vvv54");
        storyList_1.add(R.drawable._55_itaku + "changeView");
        storyList_1.add("ノック：さあ、帰るか。その前に、レイドック城によってレイドック王にqqq褒美をもらうことにしよう。" + "showText" + "vvv55");
        storyList_1.add(R.drawable.nock_10 + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv10");
        storyList_1.add(R.drawable._56_reidokkuou + "changeView");
        storyList_1.add("ノック：レイドック王、ムドーを退治して参りました。qqq" + "showText" + "vvv56");
        storyList_1.add(R.drawable._57_yoku_yatta + "changeView");
        storyList_1.add("レイドック王（女）：ノックよ、よくやった。qqq" + "showText" + "vvv57");
        storyList_1.add(R.drawable._58_youkuyakka + "changeView");
        storyList_1.add("ノック：ははっ、ありがたきお言葉。qqq" + "showText" + "vvv58");
        storyList_1.add(R.drawable._59_yokukauyta + "changeView");
        storyList_1.add("レイドック王（女）：ノック、よくやった。qqq" + "showText" + "vvv59");
        storyList_1.add(R.drawable._60_haha + "changeView");
        storyList_1.add("ノック：ははっ。qqq" + "showText" + "vvv60");
        storyList_1.add(R.drawable._61_nokku_ + "changeView");
        storyList_1.add("レイドック王（女）：ノック・・・・・・お前の、チンコを、切るqqq" + "showText" + "vvv61");
        storyList_1.add(R.drawable._62_tinnko + "changeView");
        storyList_1.add("（ノック♪）qqq" + "showText" + "vvv62");
        storyList_1.add(R.drawable._63_onnna + "changeView");
        storyList_1.add("ノックは、レイドック王に、チンコを切られてしまった！qqq" + "showText" + "vvv63");
        storyList_1.add(R.drawable._64_ishoku + "changeView");
        storyList_1.add("ノックは女になってしまった！qqq" + "showText" + "vvv64");
        storyList_1.add(R.drawable._65_nokku__ + "changeView");
        storyList_1.add("さらに、レイドック王（女）はチンコを移植した！qqq" + "showText" + "vvv65");
        storyList_1.add(R.drawable._66_reidoku + "changeView");
        storyList_1.add("レイドック王は男に戻ることができた！！！！qqq" + "showText" + "vvv66");
        storyList_1.add(R.drawable._67_houbi + "changeView");
        storyList_1.add("ノックよ、よくやった。褒美を取らそう。qqq" + "showText" + "vvv67");
        storyList_1.add(R.drawable._68_kill + "changeView");
        storyList_1.add("ノック（女）：レイドック王、お前の、チンコを、切る！qqq" + "showText" + "vvv68");
        storyList_1.add(R.drawable._69_kurai + "changeView");
        storyList_1.add("GAME CLEAR！！qqq" + "showText" + "vvv69");


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
