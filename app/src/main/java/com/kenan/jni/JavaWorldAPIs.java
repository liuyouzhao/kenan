package com.kenan.jni;

import com.kenan.platform.ImagePool;
import com.kenan.platform.ResourceManager;
import com.kenan.platform.sound.SoundController;

public class JavaWorldAPIs {

    public static String LoadImageResource(String file) {
        return ResourceManager.getInstance().loadImageResource(file);
    }

    public static String LoadSoundResource(String file) {
        return ResourceManager.getInstance().loadSoundResource(file);
    }

    public static int[] GetImageData(String id) {
        return ImagePool.getInstance().getImageData(id);
    }

    public static int GetImageWidth(String id) {
        return ImagePool.getInstance().getWidth(id);
    }

    public static int GetImageHeight(String id) {
        return ImagePool.getInstance().getHeight(id);
    }

    public static int PlaySound(String id) {
        SoundController.playSound(id);
        return 0;
    }

    public static int StopSound(String id) {
        return 0;
    }

    public static int PauseSound(String id) {
        return 0;
    }

    public static int ResumeSound(String id) {
        return 0;
    }

    public static int SetSoundVolume(String id, float left, float right) {
        return 0;
    }
}
