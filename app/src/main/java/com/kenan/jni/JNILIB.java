package com.kenan.jni;

import android.util.Log;

public class JNILIB {
    public static boolean hasInited = false;
    public static void over(){
        if(hasInited){
                Log.e("over","over");
                ShutDown();
        }
        hasInited = false;
    }

    public static void begin(int width, int height, String s, String dataDir){
        System.loadLibrary("kenan");
        if(!hasInited){
                //SetBasePath("/sdcard/");
                init(width, height , s, dataDir);
        }
        hasInited = true;
    }

    public static native void init(int width, int height, String s, String dataDir);
    public static native void deinit();
    public static native void OnFrame();
    public static native String ShutDown();
    public static native void TouchStart(int x, int y);
    public static native void TouchMove(int x, int y);
    public static native void TouchEnd(int x,int y);
    public static native void SetArguments(String s);
    public static native void Zoom(float dis);
    public static native void SetBasePath(String path);
    public static native void OnResume();
    public static native void OnLoadResource(int id);
    public static native int taskLoop(String taskId, String file, boolean isRw);
}

