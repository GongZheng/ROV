package com.scnu.pi.pivlcplayer;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
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
import android.widget.ListView;
import android.widget.TextView;

import com.scnu.pi.pivlcplayer.common.Const;
import com.scnu.pi.pivlcplayer.common.WifiCallback;
import com.scnu.pi.pivlcplayer.common.WifiUpdate;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements WifiUpdate, AdapterView.OnItemClickListener, AdapterView.OnItemLongClickListener {
    private ListView lvCars;
    private String ipAddress;
    private BaseAdapter adapter;
    private List<String> ipList;
    private int account;
    private ProgressDialog dialog;
    private Handler handler = new Handler() {
        public void handleMessage(Message msg) {
            if (dialog.isShowing()){
                dialog.dismiss();
            }
            switch (msg.what) {
                case 0:
                    new AlertDialog.Builder(MainActivity.this)
                            .setTitle("提示")
                            .setMessage("小车没有连上本地WIFI")
                            .setPositiveButton("好的", null)
                            .show();
                    break;
                case 1:
                    Intent intent_wifi = new Intent(MainActivity.this, PlayerActivity.class);
                    Bundle bundle_wifi = new Bundle();
                    bundle_wifi.putString("Pi_IP", ipAddress);
                    intent_wifi.putExtras(bundle_wifi);
                    startActivity(intent_wifi);
                    break;
            }
        }
    };
    private SharedPreferences preferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);
        WifiCallback.getInstant().setCallback(getClass().getName(), this);
        ipList = new ArrayList<>();
        preferences = getSharedPreferences(Const.WIFI_INFO_LIST, MODE_PRIVATE);
        account = preferences.getInt(Const.WIFI_INFO, 0);
        for (int i = 0; i < account; i++) {
            ipList.add("小车：" + (i+1));
        }
        lvCars = (ListView) findViewById(R.id.lv_cars);
        adapter = new MyAdapter();
        lvCars.setAdapter(adapter);
        lvCars.setOnItemClickListener(this);
        lvCars.setOnItemLongClickListener(this);
        dialog = new ProgressDialog(this);
        dialog.setMessage("正在检测环境....");
    }


    public void onClick(View v) {
        Intent intent = new Intent(this,SetWifiActivity.class);
        switch (v.getId()) {
            case R.id.btn_add_car:
                intent.putExtra(Const.INTENT_AP_CONNECT,false);
                break;
            case R.id.btn_connect_wifi:
                intent.putExtra(Const.INTENT_AP_CONNECT,true);
                break;
        }
        startActivity(intent);
    }

    @Override
    public void update(String ip) {
        account++;
        SharedPreferences.Editor editor = preferences.edit();
        editor.putInt(Const.WIFI_INFO, account);
        editor.commit();
        ipList.add("小车：" + account);
        adapter.notifyDataSetChanged();

    }

    private void getIp() {
        dialog.show();
        Thread th = new Thread(new Runnable() {
            @Override
            public void run() {
                StringBuilder buf = new StringBuilder("http://192.168.12.1:8080/getip");
                URL url = null;
                HttpURLConnection conn = null;
                BufferedReader br = null;
                try {
                    Thread.sleep(1000);
                    url = new URL(buf.toString());
                    conn = (HttpURLConnection) url.openConnection();
                    conn.setConnectTimeout(5000);
                    conn.setRequestMethod("GET");
                    conn.connect();
                    br = new BufferedReader(new InputStreamReader(conn.getInputStream(), "utf-8"));
                    String linsStr = null;
                    if ((linsStr = br.readLine()) != null) {
                        ipAddress = linsStr;
                        handler.sendEmptyMessage(1);
                    } else {
                        handler.sendEmptyMessage(0);
                    }

                } catch (Exception e) {
                    e.printStackTrace();
                    handler.sendEmptyMessage(0);
                } finally {
                    if (conn != null) {
                        conn.disconnect();
                    }
                }
            }
        });

        th.start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        WifiCallback.getInstant().cancelCallback(getClass().getName());
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        getIp();

    }

    @Override
    public boolean onItemLongClick(AdapterView<?> parent, View view, final int position, long id) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("是否要删除该小车");
        builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });
        builder.setPositiveButton("确定", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                account--;
                SharedPreferences.Editor editor = preferences.edit();
                editor.putInt(Const.WIFI_INFO, account);
                editor.commit();
                ipList.remove(position);
                adapter.notifyDataSetChanged();
            }
        });
        builder.show();
        return true;
    }

    class MyAdapter extends BaseAdapter {

        @Override
        public int getCount() {
            return ipList == null ? 0 : ipList.size();
        }

        @Override
        public Object getItem(int position) {
            return ipList.get(position);
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
                convertView = LayoutInflater.from(MainActivity.this).inflate(R.layout.item_car_info, null);
                holder.tvWifiName = (TextView) convertView.findViewById(R.id.tv_wifi_name);
                convertView.setTag(holder);
            } else {
                holder = (MainActivity.ViewHolder) convertView.getTag();
            }

            String name= ipList.get(position);
            holder.tvWifiName.setText(name);

            return convertView;
        }
    }

    class ViewHolder {
        TextView tvWifiName;

    }
}
