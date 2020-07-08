package com.kenan.platform;

import android.os.Handler;
import android.os.Message;

import com.kenan.platform.sound.SoundController;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ResourceLoader implements ISoundOnLoadCallback {

    static final int DEFAULT_THREAD_POOL_SIZE = 5;
    ExecutorService executorService = Executors.newFixedThreadPool(DEFAULT_THREAD_POOL_SIZE);

    public void loadSoundResource(final String outterId, final String file) {
        SoundController.loadSoundFromAsset(file, false, outterId, this);
    }

    @Override
    public void onSoundLoad(int innerId, String outterId) {
        Resource resource = new Resource();
        resource.inId = innerId;
        resource.outId = outterId;
        Handler callbackHandler = ResourceEventHandler.getInstance();
        Message message = callbackHandler.obtainMessage(ResourceEventHandler.MSG_ONLOAD_RESOURCE, resource);
        callbackHandler.sendMessage(message);
    }

    public void loadImageResource(final String outterId, final String file, final Handler callbackHandler) {
        executorService.execute(new Runnable(){
            @Override
            public void run(){
                ImagePool.getInstance().loadImageFromAssets(file);
                Resource resource = new Resource();
                resource.outId = outterId;
                Handler callbackHandler = ResourceEventHandler.getInstance();
                Message message = callbackHandler.obtainMessage(ResourceEventHandler.MSG_ONLOAD_RESOURCE, resource);
                callbackHandler.sendMessage(message);
            }
        });
    }

    public static ResourceLoader getInstance() {
        if(resourceLoader == null) {
            resourceLoader = new ResourceLoader();
        }
        return resourceLoader;
    }

    public static ResourceLoader resourceLoader;
}
