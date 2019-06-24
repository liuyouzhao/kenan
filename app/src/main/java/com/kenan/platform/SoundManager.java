package com.kenan.platform;

import java.io.IOException;
import java.util.HashMap;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Message;
import android.util.Log;

public class SoundManager {
    private static SoundPool sSoundPool;
    private static int sMaxPoolSize = Sound.MAX_SOUND_NUM;
    private static int sCurrentNum = 0;

    private static HashMap<String, Integer> sHashSounds = new HashMap<String, Integer>();
    private static HashMap<Integer, Boolean> sHashReady = new HashMap<Integer, Boolean>();
    public static Context sGlogalContext;

    public static void initSoundManager(Context ctx) {
        sGlogalContext = ctx;
    }

    public static int loadSoundFromAsset (String file, boolean force) {
        if(sGlogalContext == null) {
            Log.e("SoundManager", "context is not given to SoundManager");
            return -1;
        }

        if(alreadyLoaded(file) && !force) {
            return -1;
        }

        AssetFileDescriptor fd = null;
        try {
            fd = sGlogalContext.getAssets().openFd(file);
        } catch (IOException e) {
            Log.e("SoundManager", e.toString());
            return -1;
        }
        if(fd == null) {
            Log.e("SoundManager", "sound file fd is null");
            return -1;
        }

        if(sSoundPool == null) {
            sSoundPool = new SoundPool(sMaxPoolSize, AudioManager.STREAM_MUSIC, 0);
            sSoundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
                @Override
                public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                    if(status == 0) {
                        sCurrentNum ++;
                        sHashReady.put(sampleId, true);
                        Log.e("SoundManager", "sampleId: " + sampleId);
                        Message msg = Sound.resourceEventHandler.obtainMessage(Sound.MSG_ONLOAD_RESOURCE, sampleId, -1);
                        Sound.resourceEventHandler.sendMessage(msg);
                    }
                    else {
                        Log.e("SoundManager", "Failed Loaded sound sampleId: " + sampleId);
                    }
                }
            });
        }
        int soundId = sSoundPool.load(fd, 1);
        sHashSounds.put(file, soundId);
        Log.e("SoundManager", "soundId: " + soundId);
        return soundId;
    }


    public static void loadSoundFromPath (String path, boolean force) {
        if(sGlogalContext == null) {
            Log.e("SoundManager", "context is not given to SoundManager");
            return;
        }

        if(alreadyLoaded(path) && !force) {
            return;
        }

        if(sSoundPool == null) {
            sSoundPool = new SoundPool(sMaxPoolSize, AudioManager.STREAM_MUSIC, 0);
            sSoundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
                @Override
                public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                    if(status == 0) {
                        sCurrentNum ++;
                        sHashReady.put(sampleId, true);
                        Message msg = Sound.resourceEventHandler.obtainMessage(Sound.MSG_ONLOAD_RESOURCE, sampleId, -1);
                        Sound.resourceEventHandler.sendMessage(msg);
                        Log.e("SoundManager", "sampleId: " + sampleId);
                    }
                    else {
                        Log.e("SoundManager", "Failed Loaded sound sampleId: " + sampleId);
                    }
                }
            });
        }
        int soundId = sSoundPool.load(path, 1);
        sHashSounds.put(path, soundId);
        Log.e("SoundManager", "soundId: " + soundId);
    }

    public static void unloadAllSound() {
        if(sSoundPool != null) {
            sSoundPool.release();
            sSoundPool = null;
        }
    }

    public static void playSound(String file) {
        if(soundReady(file)) {
            int id = sHashSounds.get(file);
            sSoundPool.play(id, 1.0f, 0.5f, 1, 0, 1.0f);
        }
    }

    public static void playSound(int id) {
        Log.e("Harvey", "PlaySound: " + id);
        sSoundPool.play(id, 1.0f, 0.5f, 1, 0, 1.0f);
    }

    public static boolean soundReady(String file) {
        if(sHashSounds.containsKey(file)) {
            int id = sHashSounds.get(file);
            if(sHashReady.containsKey(id)) {
                return sHashReady.get(id);
            }
        }
        return false;
    }

    public static boolean alreadyLoaded(String file) {
        if(sHashSounds.containsKey(file)) {
            return true;
        }
        return false;
    }
}