package com.kenan.jni;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
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
            String spliter = "base64,";
            int index = base64image.indexOf(spliter);
            if(index < 0) {
                Log.e(AndroidSystemImpls.class.toString(), "Base64 Format not valid: " + base64image);
                return null;
            }
            String cleanImage = base64image.substring(index + spliter.length());
            byte[] encodeByte = Base64.decode(cleanImage.getBytes(), Base64.DEFAULT);
            BitmapFactory.Options options = new BitmapFactory.Options();
            Bitmap image = BitmapFactory.decodeByteArray(encodeByte, 0, encodeByte.length, options);
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

    /**
     * Read assets file
     * @param fileName
     * @return
     */
    public static JavaToCppDto Android_JavaIMPL_readFileAssets(String fileName) {
        JavaToCppDto javaToCppDto = new JavaToCppDto();
        try {
            InputStream in = sContext.getAssets().open(fileName);
            int len = in.available();
            byte[] buffer = new byte[len];

            in.read(buffer);
            in.close();

            javaToCppDto.int0 = len;
            javaToCppDto.byteArray0 = buffer;

            Log.d("Android_JavaIMPL", new String(buffer) + " len:" + len);

            return javaToCppDto;
        }
        catch(Exception ex) {
            ex.printStackTrace();
            return null;
        }
    }

    /**
     * Read storage file
     * @param fileName
     * @return
     */
    public static JavaToCppDto Android_JavaIMPL_readFileStorage(String fileName) {
        JavaToCppDto javaToCppDto = new JavaToCppDto();
        try {
            File file = new File(sContext.getFilesDir().getParent(), fileName);
            InputStream in = new FileInputStream(file);
            int len = in.available();
            byte[] buffer = new byte[len];

            in.read(buffer);
            in.close();

            javaToCppDto.int0 = len;
            javaToCppDto.byteArray0 = buffer;
            return javaToCppDto;
        }
        catch(Exception ex) {
            ex.printStackTrace();
            return null;
        }
    }

    /**
     *
     * @param fileName
     * @param content
     * @return
     */
    public static int Android_JavaIMPL_writeFileStorage_UpdateNew(String fileName, String content) {
        try {
            File file = new File(sContext.getFilesDir().getParent(), fileName);
            FileWriter writer = new FileWriter(file);
            writer.write(content);
            writer.flush();
            writer.close();
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
        return 0;
    }

    /**
     *
     * @param fileName
     * @param content
     * @return
     */
    public static int Android_JavaIMPL_writeFileStorage_AppendNew(String fileName, String content) {
        try {
            File file = new File(sContext.getFilesDir().getParent(), fileName);
            FileWriter writer = new FileWriter(file, true);
            writer.append(content);
            writer.flush();
            writer.close();
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
        return 0;
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
