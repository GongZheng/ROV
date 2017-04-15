package com.scnu.pi.pivlcplayer;

import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class Main3Activity extends AppCompatActivity  {
    private EditText editText;
    private String Pi_IP;
    private Handler handler;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        editText = (EditText) findViewById(R.id.Pi_ipaddress);
        Pi_IP = editText.getText().toString();



    }




    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn_player:
                startActivity(new Intent(this,MainActivity.class));
//                String content_wifi=editText.getText().toString();
//                Intent intent_wifi = new Intent(this,PlayerActivity.class);
//                Bundle bundle_wifi = new Bundle();
//                bundle_wifi.putString("Pi_IP",content_wifi);
//                intent_wifi.putExtras(bundle_wifi);
//                startActivity(intent_wifi);
                break;
            case R.id.btn_wificonfig:
                String content_player = editText.getText().toString();
                Intent intent_player = new Intent(this,WifiConfigActivity.class);
                Bundle bundle_player = new Bundle();
                bundle_player.putString("Pi_IP",content_player);
                intent_player.putExtras(bundle_player);
                startActivity(intent_player);
                break;
            case R.id.btn_GetIp:
                Thread th = new Thread(){
                    public void run(){
                        StringBuilder buf = new StringBuilder("http://" + Pi_IP + ":8080" + "/getip");
                        URL url = null;
                        HttpURLConnection conn = null;
                        BufferedReader br = null;
                        try {
                            url = new URL(buf.toString());
                            System.out.println(url.toString());
                            conn = (HttpURLConnection) url.openConnection();
                            conn.setConnectTimeout(5000);
                            conn.setRequestMethod("GET");
                            conn.setDoOutput(true);
                            conn.setUseCaches(false);
                            conn.connect();
                            br = new BufferedReader(new InputStreamReader(conn.getInputStream(),"utf-8"));
                            String linsStr =null;
                            if((linsStr = br.readLine())!=null) {
                                Pi_IP = linsStr;
                                handler.sendEmptyMessage(1);
                            }else {
                                handler.sendEmptyMessage(0);
                            }
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
                handler = new Handler(){
                    public void handleMessage(Message msg){
                        switch (msg.what){
                            case 0:
                                new AlertDialog.Builder(Main3Activity.this)
                                        .setTitle("提示")
                                        .setMessage("小车没有连上本地WIFI")
                                        .setPositiveButton("好的",null)
                                        .show();
                                break;
                            case 1:
                                editText.setText(Pi_IP);
                                new AlertDialog.Builder(Main3Activity.this)
                                        .setTitle("提示")
                                        .setMessage("WIFI配置成功，小车IP："+Pi_IP)
                                        .setPositiveButton("好的",null)
                                        .show();

                                break;
                        }
                    }
                };
                break;
        }
    }
}
