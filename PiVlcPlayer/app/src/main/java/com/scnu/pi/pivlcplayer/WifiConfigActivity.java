package com.scnu.pi.pivlcplayer;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.wifi.ScanResult;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.scnu.pi.pivlcplayer.common.WifiAdmin;
import com.scnu.pi.pivlcplayer.common.WifiCallback;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.List;

public class WifiConfigActivity extends AppCompatActivity implements Spinner.OnItemSelectedListener {

    private Spinner wifiAccount;
    private EditText wifiPassword;
    private List<String> wifiList;
    private String Account;
    private String Password;
    private ProgressDialog dialog;
    private Handler handler = new Handler() {
        public void handleMessage(Message msg) {
            if (dialog.isShowing()){
                dialog.dismiss();
            }
            switch (msg.what) {
                case 0:
                    new AlertDialog.Builder(WifiConfigActivity.this)
                            .setTitle("提示")
                            .setMessage("小车连接失败，请确认已经连上小车wifi:pi/n密码：123456789")
                            .setPositiveButton("好的", null)
                            .show();
                    break;
                case 1:
                    new AlertDialog.Builder(WifiConfigActivity.this)
                            .setTitle("提示")
                            .setMessage("WIFI配置成功,请重启小车")
                            .setPositiveButton("好的", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    finish();
                                    WifiCallback.getInstant().wifiUpdate("", MainActivity.class.getName());
                                }
                            })
                            .show();

                    break;
                case -1:
                    new AlertDialog.Builder(WifiConfigActivity.this)
                            .setTitle("提示")
                            .setMessage("WIFI配置失败")
                            .setPositiveButton("好的", null)
                            .show();
                    break;
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wificonfig);
        WifiAdmin wifiAdmin = new WifiAdmin(this);
        wifiAdmin.openWifi();
        wifiAdmin.startScan();
        wifiList = wifiAdmin.getSSIDList();
        wifiAccount = (Spinner) findViewById(R.id.sp_account);
        wifiPassword = (EditText) findViewById(R.id.et_pwd);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, wifiList);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        wifiAccount.setAdapter(adapter);
        wifiAccount.setOnItemSelectedListener(this);
        wifiAccount.setSelection(0);
        dialog  = new ProgressDialog(this);
        dialog.setMessage("正在连接...");
    }

    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_WifiSet:

                Password = wifiPassword.getText().toString();
                if (Account.length() != 0 && Password.length() != 0) {
                    dialog.show();
                    Thread th = new Thread() {
                        public void run() {
                            StringBuilder buf = new StringBuilder("http://192.168.12.1:8080/settingwifi");
                            URL url = null;
                            HttpURLConnection conn = null;
                            BufferedReader br = null;
                            try {
                                url = new URL(buf.toString());
                                conn = (HttpURLConnection) url.openConnection();
                                conn.setConnectTimeout(5000);
                                conn.setRequestMethod("POST");
                                conn.setDoOutput(true);
                                conn.setUseCaches(false);
                                StringBuffer params = new StringBuffer();
                                params.append("account").append("=").append(Account).append("&")
                                        .append("password").append("=").append(Password);
                                conn.connect();
                                byte[] bypes = params.toString().getBytes();
                                conn.getOutputStream().write(bypes);
                                conn.getOutputStream().flush();
                                conn.getOutputStream().close();
                                br = new BufferedReader(new InputStreamReader(conn.getInputStream(), "utf-8"));
                                String linsStr = null;
                                if ((linsStr = br.readLine()) != null) {
                                    if (linsStr.equals("successful")) {
                                        handler.sendEmptyMessage(1);
                                    } else {
                                        handler.sendEmptyMessage(-1);
                                    }
                                } else {
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
                } else {
                    new AlertDialog.Builder(this)
                            .setTitle("提示")
                            .setMessage("请填写完整的WIFI账号和密码信息")
                            .setPositiveButton("好的", null)
                            .show();
                }
                break;
        }
    }


    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        Account = wifiList.get(position);
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
        Account = wifiList.get(wifiAccount.getSelectedItemPosition());
    }


}



