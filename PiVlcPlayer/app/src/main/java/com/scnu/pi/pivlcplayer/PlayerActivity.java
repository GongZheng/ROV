package com.scnu.pi.pivlcplayer;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;


import com.scnu.pi.pivlcplayer.ui.PiVideo;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class PlayerActivity extends AppCompatActivity implements  View.OnTouchListener {
    private static String SDP_CONFIGURATION = "sdp_configuration";
    private static String KEY_IP = "ip";
    private static int PORT_ONE = 8888;
    private static int PORT_TWO = 8000;
    private PiVideo piVideo;
    private String ip = "127.0.0.1";
    private String Pi_IP;
    private boolean cancelMoving;
    private static ExecutorService executorService = Executors.newFixedThreadPool(5);
    private int option = 0;
    private int value = 0;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);
        piVideo = (PiVideo) findViewById(R.id.player_video);
        piVideo.playVideo(getVideoPath(ip, PORT_ONE), getVideoPath(ip, PORT_TWO));
        Intent intent = getIntent();
        Pi_IP = intent.getStringExtra("Pi_IP");
        Thread th = new Thread(){
            public void run(){
                StringBuilder buf = new StringBuilder("http://" + Pi_IP + ":8080" + "/startpicam?record=0");
                URL url = null;
                HttpURLConnection conn = null;
                try {
                    url = new URL(buf.toString());
                    conn = (HttpURLConnection) url.openConnection();
                    conn.setConnectTimeout(5000);
                    conn.setRequestMethod("GET");
                    conn.connect();
                    System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + buf);
                    conn.getResponseCode();
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    if (conn != null) {
                        conn.disconnect();
                    }
                }
            }
        };
        th.start();
    }

    /**
     * @param ip
     * @param port
     * @return
     */
    private String getVideoPath(String ip, int port) {
        String File_PATH = getFilesDir().getPath() + "/";
        File mWorkingPath = new File(File_PATH);
        if (!mWorkingPath.exists()) {
            mWorkingPath.mkdirs();
        }
        File outFile = new File(mWorkingPath, port + "video.sdp");
        //判断IP地址有没有改变，如果改变，生成新的配置
        SharedPreferences preferences = getSharedPreferences(SDP_CONFIGURATION, Activity.MODE_PRIVATE);
        String historyIp = preferences.getString(KEY_IP, null);
        if (!outFile.exists() || historyIp == null || !historyIp.equals(ip)) {
            SharedPreferences.Editor editor = preferences.edit();
            editor.putString(KEY_IP, ip);
            editor.commit();
            writeConfig(outFile, ip, port);
        }
        return outFile.getAbsolutePath();
    }

    /**
     * @param outFile
     * @param ip
     * @param port
     */
    private void writeConfig(File outFile, String ip, int port) {
        try {
            FileWriter fileWriter = new FileWriter(outFile);
            BufferedWriter writer = new BufferedWriter(fileWriter);
            writer.write("m=video " + port + " RTP/AVP 96 \n");
            writer.write("a=rtpmap:96 H264 \n");
            writer.write("a=framerate:15  \n");
            writer.write("c=IN IP4 " + ip + " \n");
            writer.close();
            fileWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    public void onClick(View v) {
        moving(v);
    }

    private void moving(View v) {
        option = 0;
        value = 50;
        if (v == null) {
            option = 0;
            value = 0;
        } else {
            switch (v.getId()) {
                case R.id.iv_back:
                    option = 2;

                    break;
                case R.id.iv_forward:
                    option = 1;
                    break;
                case R.id.iv_back_left:
                    option = 5;
                    break;
                case R.id.iv_back_right:
                    option = 6;
                    break;
                case R.id.iv_forward_left:
                    option = 3;
                    break;
                case R.id.iv_forward_right:
                    option = 4;
                    break;
            }
            //Toast.makeText(this,option+"",0).show();
            System.out.println(option + "");
        }
        Thread th = new Thread(){
            public void run(){setSpeed(option,value);}
        };
        th.start();
        //setSpeed(option, value);
    }

    private void setSpeed(int option, int value) {
        StringBuilder buf = new StringBuilder("http://" + Pi_IP + ":8088" + "/car");
        buf.append("?");
        buf.append("v=" + value + "&");
        buf.append("a=" + option);

        URL url = null;
        HttpURLConnection conn = null;
        try {
            url = new URL(buf.toString());
            conn = (HttpURLConnection) url.openConnection();
            conn.setConnectTimeout(5000);
            conn.setRequestMethod("GET");
            conn.connect();
            System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + buf);
            conn.getResponseCode();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (conn != null) {
                conn.disconnect();
            }
        }
    }

    @Override
    public boolean onTouch(final View v, MotionEvent event) {

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                /*cancelMoving = false;
                executorService.execute(new Runnable() {
                    @Override
                    public void run() {
                        while (!cancelMoving) {
                            try {
                                Thread.sleep(200);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                            moving(v);
                        }
                    }
                });*/
                if (v.getId()!=R.id.iv_snapshot){
                moving(v);
                }else{
                    Thread th = new Thread(){
                        public void run(){
                    StringBuilder buf = new StringBuilder("http://" + Pi_IP + ":8888" + "/snapShot");
                    URL url = null;
                    HttpURLConnection conn = null;
                    try {
                        url = new URL(buf.toString());
                        conn = (HttpURLConnection) url.openConnection();
                        conn.setConnectTimeout(5000);
                        conn.setRequestMethod("GET");
                        conn.connect();
                        System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + buf);
                        conn.getResponseCode();
                    } catch (Exception e) {
                        e.printStackTrace();
                    } finally {
                        if (conn != null) {
                            conn.disconnect();
                        }
                    }
                    }
                };
                    th.start();
                }
                break;
            case MotionEvent.ACTION_UP:
                //cancelMoving = true;
                if(v.getId()!=R.id.iv_snapshot)
                {moving(null);}
                break;
        }

        return false;
    }
}
