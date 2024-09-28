package com.eaglesakura.gles20.data;

import java.io.InputStream;
import java.nio.Buffer;
import java.nio.ByteBuffer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.eaglesakura.gles20.app.GLApplication;

/**
 * 読み込んだ画像を保存する構造体
 */
public class RawPixelImage {

    /**
     * RGB / RGBAピクセル情報
     */
    public Buffer pixel_data = null;

    /**
     * 画像幅
     */
    public int width = 0;

    /**
     * 画像高
     */
    public int height = 0;

    /**
     * 画像フォーマット
     * GL_RGBA | GL_RGB
     */
    public int format = 0;

    public RawPixelImage() {
    }

    /**
     * 画像を読み込む
     * @param util
     * @param file_name
     * @param pixel_format GL_RGBA | GL_RGB
     * @return
     */
    public static RawPixelImage loadImage(GLApplication app, final String file_name, int pixel_format) {
        AndroidPlatformData platform = ((AndroidPlatformData) app.platform);
        InputStream stream = null;
        try {
            stream = platform.context.getAssets().open(file_name);

            Bitmap image = BitmapFactory.decodeStream(stream);
            if (image == null) {
                return null;
            }

            final int image_width = image.getWidth();
            final int image_height = image.getHeight();
            RawPixelImage result = new RawPixelImage();

            // ピクセル情報の格納先
            ByteBuffer pixelBuffer = ByteBuffer.allocateDirect(image_width * image_height * 4);
            {
                result.format = pixel_format;
                result.width = image_width;
                result.height = image_height;
                result.pixel_data = pixelBuffer;
            }

            Log.d("RawPixelImage", String.format("image size(%d x %d)", image_width, image_height));

            final int[] temp = new int[image_width];
            final byte[] pixel_temp = new byte[4];
            for (int i = 0; i < image_height; ++i) {
                // 1ラインずつ読み込む
                image.getPixels(temp, 0, image_width, 0, i, image_width, 1);
                // 結果をByteArrayへ書き込む
                for (int k = 0; k < image_width; ++k) {
                    final int pixel = temp[k];

                    pixel_temp[0] = (byte) ((pixel >> 16) & 0xFF);
                    pixel_temp[1] = (byte) ((pixel >> 8) & 0xFF);
                    pixel_temp[2] = (byte) ((pixel) & 0xFF);
                    pixel_temp[3] = (byte) ((pixel >> 24) & 0xFF);

                    pixelBuffer.put(pixel_temp);
                }
            }

            // 書き込み位置をリセットする
            pixelBuffer.position(0);

            image.recycle();
            return result;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        return null;
    }
}
