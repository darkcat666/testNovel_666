package com.websarva.wings.android.testfgo;

public class TODO {

    // （！！！）回転したときに、音楽インスタンスが複数作られてしまう・・・1つにしろ（！！！）

/////////////////////////////////////////////
    // 魔法：攻撃エフェクトを実装する方法を調べる：animation、OpenGLes
    // 魔法：フラッシュ処理を実装する方法を調べる：プログラミング、OpenGLes???
    // マップを移動するマップチップを調べる
    // マップチップ上をキャラクターを移動させる
/////////////////////////////////////////////

    // （未）魔法の音声のメモリ解放するSoundPool
    // （未）開始してから画面連打するとゲームのインスタンスが複数起動してしまう：
    // 音楽がループしない：OK!!
    // 効果音でた：OK!!
    // 文字列ゆっくりだす → setText()をsleep()混ぜて何回も呼べばいいのか・・・:OK!!
    // 文字列ゆっくりだす２　ー＞　引数に２を指定したときに、１の分のスレッド完了を待つようにする・・・OK！！
    // 軽くストーリー量産する！！
    // tweenAnimXMLかく！
    // surfaceViewもあり（ねじめさん）
    // NDK →　C++（レオは無理）（ねじめさん）
    // （未）storyListのみでは選択ボタンに対応せず、選択ボタンではなく背景を押下したときに画面が変更される
    // （未）選択ボタンまでを1つのストーリーとして、リストに登録する！！
    // （未）そのため、選択ボタン制御と背景ボタン非活性制御を入れる！！

    // （未）できたら民みさんにも見せる！（やりたいみたいだから無料版渡すか・・・）
    // （未）ツイッターあさかさんにできたら見せる
    // （未）賽の河原要素入れる！ーみんみさん
    // （未）storyList1のnumberをmainActivityとstoryManagerで別々に持つのはおかしい・・・（本来的に１つのはず）

    // （未）（鈴木君提案）：JSONファイルを外部に配置して、そこから読み込むようにする（ストーリー）
    // （未）管理・変更しやすく
    // （未）ストーリーの進行
    // →　（未）テキストでR値を解釈するのは無理だから、現状の作りでは無理？？（考えよう）
    // （未）R値（int型）を解釈する必要があるので、どう実装するか・・・（そもそもＲ値はandroid内部に持っているため、テキスト指定はいけない　→　Stringごとに分岐してＲ値を渡すか？？（ムダに見える・・・）
    // テキストのみの処理ではOK。
    // ↓
    // （未）テキストファイルは平文なので、改ざんされたら被害がある・・・また、技術的にもコードが冗長になるためテキストファイルは現状不要と考える！
}
