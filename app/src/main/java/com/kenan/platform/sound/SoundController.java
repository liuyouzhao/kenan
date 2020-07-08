package com.kenan.platform.sound;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;

import com.kenan.platform.ISoundOnLoadCallback;

import java.io.IOException;
import java.util.HashMap;

public class SoundController {
    private static SoundPool sSoundPool;
    private static int sMaxPoolSize = 256;

    private static HashMap<String, Integer> sHashSounds = new HashMap<String, Integer>();
    private static HashMap<String, String> sHashSoundFiles = new HashMap<String, String>();
    private static HashMap<Integer, Boolean> sHashReady = new HashMap<Integer, Boolean>();
    public static Context sGlogalContext;

    public static void initSoundManager(Context ctx) {
        sGlogalContext = ctx;
    }

    public static int loadSoundFromAsset(String file,
                                         boolean force,
                                         final String outerId,
                                         final ISoundOnLoadCallback iSoundOnLoadCallback) {
        if(sGlogalContext == null) {
            Log.e("SoundController", "context is not given to SoundController");
            return -1;
        }

        if(alreadyLoaded(file) && !force) {
            return -1;
        }

        AssetFileDescriptor fd = null;
        try {
            fd = sGlogalContext.getAssets().openFd(file);
        } catch (IOException e) {
            Log.e("SoundController", e.toString());
            return -1;
        }
        if(fd == null) {
            Log.e("SoundController", "sound file fd is null");
            return -1;
        }

        if(sSoundPool == null) {
            sSoundPool = new SoundPool(sMaxPoolSize, AudioManager.STREAM_MUSIC, 0);
            sSoundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
                @Override
                public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                    if(status == 0) {
                        sHashReady.put(sampleId, true);
                        iSoundOnLoadCallback.onSoundLoad(sampleId, outerId);
                        Log.e("SoundController", "sampleId: " + sampleId);
                    }
                    else {
                        Log.e("SoundController", "Failed Loaded sound sampleId: " + sampleId);
                    }
                }
            });
        }
        int soundId = sSoundPool.load(fd, 1);
        sHashSounds.put(outerId, soundId);
        sHashSoundFiles.put(file, outerId);
        Log.e("SoundController", "soundId: " + soundId);
        return soundId;
    }

    public static void unloadAllSound() {
        if(sSoundPool != null) {
            sSoundPool.release();
            sSoundPool = null;
        }
    }

    public static void playSound(String outterId) {
        int id = sHashSounds.get(outterId);
        if(sHashReady.get(id) == true)
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