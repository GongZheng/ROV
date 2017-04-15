package com.scnu.pi.pivlcplayer.common;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by lzj on 2017/4/12.
 */

public class WifiCallback {

    private static WifiCallback instant;


    private Map<String,WifiUpdate> map;

    private WifiCallback() {
        map = new HashMap<>();

    }

    public static WifiCallback getInstant() {
        if (instant == null) {
            instant = new WifiCallback();
        }
        return instant;
    }


    public void setCallback(String className,WifiUpdate callback) {
        map.put(className,callback);
    }


    public void wifiUpdate(String ip,String callBackName){
       for (String key:map.keySet()){
           if (key.equals(callBackName)){
               map.get(key).update(ip);
               break;
           }
       }
    }

    public void cancelCallback(String className){
       map.remove(className);
    }
}
