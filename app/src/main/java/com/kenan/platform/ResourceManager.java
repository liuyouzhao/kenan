package com.kenan.platform;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.util.HashMap;
import java.util.Vector;


public class ResourceManager {
    static class Resource {
        public int outId;
        public int inId;
        public String file;
        public Object[] data;
    };

    private static HashMap<Integer, Resource> sIDMap = new HashMap<Integer, Resource>();
    private static HashMap<Integer, Resource> sIDMapIn = new HashMap<Integer, Resource>();

    private static Vector<Integer> sImageIDsPool = new Vector<Integer>();
    private static Vector<Integer> sSoundIDsPool = new Vector<Integer>();

    public static final int MAX_IMAGE_NUM = 128;
    public static final int MAX_SOUND_NUM = 32;

    public static void init(Context ctx) {
        SoundManager.initSoundManager(ctx);
        for(int i = 0; i < MAX_IMAGE_NUM; i ++) {
            sImageIDsPool.add(i + 10000);
        }
        for(int i = 0; i < MAX_SOUND_NUM; i ++) {
            sSoundIDsPool.add(i + 20000);
        }
    }

    public static int LoadImageResource(String file) {
        return 0;
    }

    public static int LoadSoundResource(String file) {
        Resource res = new Resource();
        res.file = file;
        Log.e("Harvey", "filename: " + file);
        int id = sSoundIDsPool.get(0);
        sSoundIDsPool.remove(0);
        res.outId = id;
        Message msg = resourceEventHandler.obtainMessage(MSG_LOAD_SOUND, res);
        resourceEventHandler.sendMessage(msg);

        Log.e("Harvey", "LoadSoundResource: " + id);
        return id;
    }

    public static int ReleaseResource(int id) {
        return 0;
    }

    public static int[] GetImageData(int id) {
        return null;
    }

    public static int GetImageWidth(int id) {
        return 0;
    }

    public static int GetImageHeight(int id) {
        return 0;
    }

    public static int PlaySound(int id) {
        Resource res = sIDMap.get(id);
        if(res == null) {
            Log.e("Harvey", "res is NULL by id: " + id);
            return -1;
        }
        Message msg = resourceEventHandler.obtainMessage(MSG_PLAY_SOUND, res);
        resourceEventHandler.sendMessage(msg);
        return 0;
    }

    public static int StopSound(int id) {
        return 0;
    }

    public static int PauseSound(int id) {
        return 0;
    }

    public static int ResumeSound(int id) {
        return 0;
    }

    public static int SetSoundVolume(int id, float left, float right) {
        return 0;
    }

    public static final int MSG_LOAD_IMAGE = 1001;
    public static final int MSG_LOAD_SOUND = 1002;
    public static final int MSG_PLAY_SOUND = 1011;

    public static final int MSG_ONLOAD_RESOURCE = 1100;


    static public class JsFrameCallback {
        public static Vector sMotionEventList = new Vector();
        public static Vector sOnLoadResourceEventList = new Vector();
    }
    public static Handler resourceEventHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch(msg.what) {
                case MSG_LOAD_IMAGE:
                    break;
                case MSG_LOAD_SOUND: {
                    Resource res = (Resource) msg.obj;
                    res.inId = SoundManager.loadSoundFromAsset(res.file, false);
                    sIDMap.put(res.outId, res);
                    sIDMapIn.put(res.inId, res);
                } break;
                case MSG_PLAY_SOUND: {
                    Resource res = (Resource) msg.obj;
                    int id = res.inId;
                    SoundManager.playSound(id);
                } break;
                case MSG_ONLOAD_RESOURCE: {
                    Log.e("Harvey", "MSG_ONLOAD_RESOURCE OnloadResource: " + msg.arg1);
                    int inId = msg.arg1;
                    Resource res = sIDMapIn.get(inId);
                    JsFrameCallback.sOnLoadResourceEventList.add(res.outId);
                } break;
            }
        }
    };
}