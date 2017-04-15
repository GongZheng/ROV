package com.scnu.pi.pivlcplayer;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.UiThread;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;


import com.scnu.pi.pivlcplayer.common.Const;
import com.scnu.pi.pivlcplayer.common.WifiAdmin;
import com.scnu.pi.pivlcplayer.common.WifiCallback;
import com.scnu.pi.pivlcplayer.common.WifiUpdate;
import com.scnu.pi.pivlcplayer.receiver.WifiReceiver;

import java.util.List;

public class SetWifiActivity extends AppCompatActivity implements AdapterView.OnItemClickListener, WifiUpdate {
    public static final String INTENT_TYPE_TWO = "intent_type_two";
    //wifi list view
    private ListView lvWifiInfo;
    // saved wifi info set
    private List<ScanResult> mWifiList;
    //the wifi controller
    private WifiAdmin mWifiAdmin;

    //list view adapter
    private MyAdapter mAdapter;

    private WifiReceiver mReceiver;
    private IntentFilter mIntentFilter;

    private String mWifiPwd;
    private ProgressDialog progressDialog;

    private FailWifi failWifi;

    private Handler handler = new Handler();

    private boolean apConnect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_set_wifi);
        mReceiver = new WifiReceiver();

        mIntentFilter = new IntentFilter();
        mIntentFilter.addAction(WifiManager.RSSI_CHANGED_ACTION);
        mIntentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);

        lvWifiInfo = (ListView) findViewById(R.id.lv_wifi_info);
        lvWifiInfo.setOnItemClickListener(this);
        mWifiAdmin = new WifiAdmin(this);
        mWifiAdmin.openWifi();
        mWifiAdmin.startScan();
        mWifiList = mWifiAdmin.getWifiList();
        mAdapter = new MyAdapter();
        lvWifiInfo.setAdapter(mAdapter);
        WifiCallback.getInstant().setCallback(getClass().getName(), this);

        progressDialog = new ProgressDialog(this);

        failWifi = new FailWifi();

        apConnect = getIntent().getBooleanExtra(Const.INTENT_AP_CONNECT, false);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

        // mWifiAdmin.connectionConfiguration(position);
        View layout = getLayoutInflater().inflate(R.layout.dialog_wifi_info, null);
        TextView account = (TextView) layout.findViewById(R.id.iv_wifi_account);
        final EditText etPwd = (EditText) layout.findViewById(R.id.et_wifi_pwd);
        final String ssid = mWifiList.get(position).SSID;
        account.setText(ssid);

        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setView(layout);
        dialog.setTitle("信息填写");
        dialog.setPositiveButton("确定", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                String pwdStr = etPwd.getText().toString();
                WifiConfiguration configuration = mWifiAdmin.createWifiInfo(ssid, pwdStr, 3);
                mWifiAdmin.addNetWork(configuration);
                dialog.dismiss();
                progressDialog.show();
                progressDialog.setMessage("正在连接...");
                handler.postDelayed(failWifi, 12000);

            }
        });
        dialog.setNegativeButton("取消", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                mWifiPwd = null;
                dialog.dismiss();
            }
        });
        dialog.show();

    }

    public void btnClick(View view) {
        switch (view.getId()) {
            case R.id.btn_scan_wifi:
                //扫描WiFi
                mWifiAdmin.startScan();
                mWifiList = mWifiAdmin.getWifiList();
                mAdapter.notifyDataSetChanged();
                Toast.makeText(this, "更新成功！", Toast.LENGTH_SHORT).show();
                break;


        }


    }

    @Override
    public void update(String ipAddress) {
        if (progressDialog.isShowing()) {
            progressDialog.dismiss();
            handler.removeCallbacks(failWifi);
            Toast.makeText(this, "连接成功!", Toast.LENGTH_SHORT).show();
            if (!apConnect) {
                startActivity(new Intent(SetWifiActivity.this, WifiConfigActivity.class));
            }
            finish();
        }


    }

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mReceiver, mIntentFilter);
    }


    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        WifiCallback.getInstant().cancelCallback(getClass().getName());
    }

    class FailWifi implements Runnable {

        @Override
        public void run() {
            if (progressDialog.isShowing()) {
                progressDialog.dismiss();
                Toast.makeText(SetWifiActivity.this, "连接失败", Toast.LENGTH_SHORT).show();
            }

        }
    }

    /**
     * 自定义list view item
     */
    class MyAdapter extends BaseAdapter {

        @Override
        public int getCount() {
            return mWifiList == null ? 0 : mWifiList.size();
        }

        @Override
        public Object getItem(int position) {
            return mWifiList.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ViewHolder holder;
            if (convertView == null) {
                holder = new ViewHolder();
                convertView = LayoutInflater.from(SetWifiActivity.this).inflate(R.layout.item_wifi_info, null);
                holder.tvWifiLevel = (TextView) convertView.findViewById(R.id.tv_wifi_level);
                holder.tvWifiName = (TextView) convertView.findViewById(R.id.tv_wifi_name);
                convertView.setTag(holder);
            } else {
                holder = (ViewHolder) convertView.getTag();
            }

            ScanResult result = mWifiList.get(position);
            holder.tvWifiName.setText(result.SSID);
            holder.tvWifiLevel.setText(result.level + 100 + "");
            return convertView;
        }
    }

    class ViewHolder {
        TextView tvWifiName;
        TextView tvWifiLevel;
    }


}
