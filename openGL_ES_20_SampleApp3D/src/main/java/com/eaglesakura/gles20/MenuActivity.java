package com.eaglesakura.gles20;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.TextView;

import com.eaglesakura.gles20.service.GLViewService;
import com.eaglesakura.gles20.util.ES20;
import com.eaglesakura.gles20_3d.R;

public class MenuActivity extends Activity {

    static {
        // Native側の初期化を行う
        System.loadLibrary("sample");
        ES20.initializeNative();
    }

    static final String TAG = MenuActivity.class.getSimpleName();

    /**
     * Menu追加する親レイアウト
     */
    ListView menuList = null;

    CheckBox useTextureview;

    CheckBox useVsync;

    List<SampleItem> menues = new ArrayList<MenuActivity.SampleItem>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        menuList = (ListView) findViewById(R.id.list);
        useTextureview = (CheckBox) findViewById(R.id.use_textureview);
        useVsync = (CheckBox) findViewById(R.id.use_vsync);

        // Android 4.0未満はTextureViewを利用できない
        if (Build.VERSION.SDK_INT < 15) {
            useTextureview.setEnabled(false);
            useTextureview.setChecked(false);
        }

        // Android 4.1未満は垂直同期を取れない
        if (Build.VERSION.SDK_INT < 16) {
            useVsync.setEnabled(false);
            useVsync.setChecked(false);
        }

        initializeItems();
        initializeAdapter();

        // SystemLayer系を初期化
        {
            findViewById(R.id.systemlayer_start).setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    GLViewService.start(getApplicationContext(), useTextureview.isChecked(), useVsync.isChecked());
                }
            });

            findViewById(R.id.systemlayer_stop).setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    GLViewService.stop(getApplicationContext());
                }
            });
        }
    }

    /**
     * サンプル一覧をリストアップする
     */
    void initializeItems() {
        for (int chapter = 0; chapter < SampleList.getChapterNum(); ++chapter) {
            // ヘッダ設定
            {
                SampleItem item = new SampleItem();
                item.header = true;
                item.title = String.format(Locale.getDefault(), "%02d章 %s", SampleList.getBookChapterNumber(chapter),
                        SampleList.getChapterName(chapter));
                menues.add(item);
            }

            for (int sample = 0; sample < SampleList.getChapterSampleNum(chapter); ++sample) {
                SampleItem item = new SampleItem();
                item.title = String.format(Locale.getDefault(), "%02d-%02d %s", chapter + 1, sample + 1,
                        SampleList.getChapterSampleName(chapter, sample));
                item.header = false;
                item.chapterNumber = chapter;
                item.sampleNumber = sample;
                menues.add(item);
            }
        }
    }

    /**
     * ListViewに与えるAdapterとListenerを設定する
     */
    void initializeAdapter() {

        // アダプタを作成する
        menuList.setAdapter(new BaseAdapter() {

            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                SampleItem item = menues.get(position);

                if (convertView == null) {
                    if (item.header) {
                        convertView = View.inflate(MenuActivity.this, R.layout.view_menu_header, null);
                    } else {
                        convertView = View.inflate(MenuActivity.this, R.layout.view_menu_title, null);
                    }
                }

                ((TextView) convertView.findViewById(R.id.text)).setText(item.title);

                return convertView;
            }

            @Override
            public int getViewTypeCount() {
                return 2;
            }

            @Override
            public int getItemViewType(int position) {
                SampleItem item = menues.get(position);
                return item.header ? 0 : 1;
            }

            @Override
            public long getItemId(int position) {
                return position;
            }

            @Override
            public Object getItem(int position) {
                return menues.get(position);
            }

            @Override
            public int getCount() {
                return menues.size();
            }
        });

        // メニュー選択時のアクションを設定する
        menuList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> arg0, View view, int position, long id) {
                SampleItem item = menues.get(position);
                if (item.header) {
                    return;
                }

                Log.d(TAG, String.format(Locale.getDefault(), "Selected(%d - %d)", item.chapterNumber + 1,
                        item.sampleNumber + 1));

                Intent intent = new Intent(MenuActivity.this, RenderingActivity.class);
                intent.putExtra(RenderingActivity.EXTRA_CHAPTER_NUMBER, item.chapterNumber);
                intent.putExtra(RenderingActivity.EXTRA_SAMPLE_NUMBER, item.sampleNumber);
                intent.putExtra(RenderingActivity.EXTRA_MODE_TEXTUREVIEW, useTextureview.isChecked());
                intent.putExtra(RenderingActivity.EXTRA_MODE_VSYNC, useVsync.isChecked());

                startActivity(intent);

            }
        });
    }

    static class SampleItem {
        /**
         * タイトル
         */
        String title = "";

        /**
         * ヘッダ属性
         */
        boolean header = true;

        /**
         * 選択されるチャプター番号
         */
        int chapterNumber = -1;

        /**
         * 選択されるサンプル番号
         */
        int sampleNumber = -1;
    }
}
