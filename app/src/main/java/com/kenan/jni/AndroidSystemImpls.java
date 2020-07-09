package com.kenan.jni;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.provider.MediaStore;
import android.util.Base64;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;

public class AndroidSystemImpls {

    public static Context sContext;

    public static void init(Context context) {   sContext = context; }

    /**
     * Bitmap is default RGBA
     * @param base64image
     * @return int32Array = [|pixels|w|h]
     */
    public static int[] Android_JavaIMPL_base64ToPixel(String base64image) {
        try {
            String cleanImage = base64image.replace("data:image/png;base64,", "").replace("data:image/jpeg;base64,","");
            byte[] encodeByte = Base64.decode(cleanImage.getBytes(), Base64.DEFAULT);
            BitmapFactory.Options options = new BitmapFactory.Options();
            Bitmap image = BitmapFactory.decodeByteArray(encodeByte, 0, encodeByte.length, options);


            String savedImageURL = MediaStore.Images.Media.insertImage(
                    sContext.getContentResolver(),
                    image,
                    "Bird",
                    "Image of bird"
            );

            return convertReturnValue(image);
        }
        catch(Exception ex) {
            ex.printStackTrace();
            return null;
        }
    }

    /**
     * Load pixel data from Local file, RGBA
     * @param fileName
     * @return
     */
    public static int[] Android_JavaIMPL_loadImage(String fileName) {
        if(sContext == null) {
            Log.e(AndroidSystemImpls.class.getName(), "Context is null, please give context object to this class");
            return null;
        }
        Bitmap image = getBitmapFromAsset(sContext, fileName);
        return convertReturnValue(image);
    }

    private static int[] convertReturnValue(Bitmap image) {

        int[] pixels = new int[image.getWidth() * image.getHeight()];
        int[] rt = new int[image.getWidth() * image.getHeight() + 2];

        image.getPixels(pixels, 0, image.getWidth(), 0, 0, image.getWidth(), image.getHeight());

        rt[0] = image.getWidth(); rt[1] = image.getHeight();
        int i = 0;
        for( ; i < image.getWidth() * image.getHeight(); i ++) {
            rt[i + 2] = pixels[i];
        }
        return rt;
    }

    private static Bitmap getBitmapFromAsset(Context context, String filePath) {
        AssetManager assetManager = context.getAssets();

        InputStream istr;
        Bitmap bitmap = null;
        try {
            istr = assetManager.open(filePath);
            bitmap = BitmapFactory.decodeStream(istr);
        } catch (IOException e) {
            // handle exception
        }
        return bitmap;
    }

}
