package com.kenan.platform;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.kenan.jni.JsFrameCallback;

import java.util.HashMap;

public class ResourceEventHandler extends Handler {
    private static HashMap<Integer, Resource> sIDMapIn = new HashMap<Integer, Resource>();

    public static final int MSG_ONLOAD_RESOURCE = 1100;


    public static ResourceEventHandler getInstance() {
        if(resourceEventHandler == null) {
            resourceEventHandler = new ResourceEventHandler();
        }
        return resourceEventHandler;
    }

    private static ResourceEventHandler resourceEventHandler;

    public void handleMessage(Message msg) {
        switch(msg.what) {
            case MSG_ONLOAD_RESOURCE: {
                Resource resource = (Resource) msg.obj;
                JsFrameCallback.sOnLoadResourceEventList.add(resource.outId);
            } break;
        }
    }
}
