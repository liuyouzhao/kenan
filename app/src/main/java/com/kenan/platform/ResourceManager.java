package com.kenan.platform;

import android.content.Context;

import com.kenan.platform.sound.SoundController;

import java.util.UUID;


public class ResourceManager {

    private ResourceEventHandler resourceEventHandler;

    public void init(Context ctx) {
        SoundController.initSoundManager(ctx);
        ImagePool.getInstance().init(ctx);
        resourceEventHandler = ResourceEventHandler.getInstance();
    }

    public String loadImageResource(String file) {
        String uuid = UUID.randomUUID().toString();
        ResourceLoader.getInstance().loadImageResource(uuid, file, resourceEventHandler);
        return uuid;
    }

    public String loadSoundResource(String file) {
        String uuid = UUID.randomUUID().toString();
        ResourceLoader.getInstance().loadSoundResource(uuid, file);
        return uuid;
    }

    /**
     * For singleton
     */
    private static ResourceManager resourceManager;
    public static ResourceManager getInstance() {
        if(resourceManager == null) {
            resourceManager = new ResourceManager();
        }
        return resourceManager;
    }
}