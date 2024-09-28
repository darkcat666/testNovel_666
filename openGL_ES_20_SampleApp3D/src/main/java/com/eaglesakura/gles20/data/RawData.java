package com.eaglesakura.gles20.data;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.Buffer;
import java.nio.ByteBuffer;

import com.eaglesakura.gles20.app.GLApplication;

public class RawData {

    public RawData() {
    }

    /**
     * 先頭ポインタ
     */
    public Buffer head = null;

    /**
     * 
     */
    public int length = 0;

    /**
     * 読込ヘッダ
     */
    public Buffer read_head = null;

    /**
     * 
     * @param util
     * @param fileName
     * @return
     */
    public static RawData loadFile(GLApplication app, String fileName) {
        AndroidPlatformData platform = (AndroidPlatformData) app.platform;

        InputStream stream = null;
        try {
            stream = platform.context.getAssets().open(fileName);

            byte[] buffer = new byte[1024];
            //! 元ストリームを読み取り
            {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                int n = 0;
                while ((n = stream.read(buffer)) > 0) {
                    baos.write(buffer, 0, n);
                }

                // バッファを上書きする
                buffer = baos.toByteArray();
            }

            RawData raw = new RawData();

            // ByteBufferにコピーする
            {
                ByteBuffer temp = ByteBuffer.allocateDirect(buffer.length);
                temp.put(buffer);
                temp.position(0);

                raw.head = temp;
                raw.read_head = temp;
            }

            // 長さを書き出す
            // 本来はByteBufferが長さを持っているため必要ないが、Cとの互換性を保つ
            {
                raw.length = buffer.length;
            }
            return raw;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (stream != null) {
                    stream.close();
                }
            } catch (Exception e) {

            }
        }

        return null;
    }
}
