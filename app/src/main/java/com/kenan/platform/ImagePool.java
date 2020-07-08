package com.kenan.platform;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.UUID;

public class ImagePool {

    private HashMap<String, Bitmap> bitmapPool = new HashMap<String, Bitmap>();

    private Context context;

    public void init(Context ctx) {
        context = ctx;
    }

    public String loadImageFromAssets(String file) {
        AssetManager assetManager = context.getAssets();
        InputStream is = null;
        String uuid = null;
        try {
            is = assetManager.open(file);
            Bitmap  bitmap = BitmapFactory.decodeStream(is);
            uuid = UUID.randomUUID().toString();
            bitmapPool.put(uuid, bitmap);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return uuid;
    }

    public void releaseImage(String uuid) {
        if(bitmapPool.containsKey(uuid))
            bitmapPool.remove(uuid);
    }

    public int[] getImageData(String uuid) {
        if(bitmapPool.containsKey(uuid)) {
            Bitmap bitmap = bitmapPool.get(uuid);
            int []pixels = new int[bitmap.getWidth() * bitmap.getHeight()];
            bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
            return pixels;
        } else {
            return null;
        }
    }

    public int getWidth(String uuid) {
        if(bitmapPool.containsKey(uuid)) {
            Bitmap bitmap = bitmapPool.get(uuid);
            return bitmap.getWidth();
        } else {
            return 0;
        }
    }

    public int getHeight(String uuid) {
        if(bitmapPool.containsKey(uuid)) {
            Bitmap bitmap = bitmapPool.get(uuid);
            return bitmap.getHeight();
        } else {
            return 0;
        }
    }

    public static ImagePool imagePool;
    public static ImagePool getInstance() {
        if(imagePool == null) {
            imagePool = new ImagePool();
        }
        return imagePool;
    }
}
