package com.rov.rovandroid;

import android.app.ProgressDialog;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private Handler mHandler = new Handler();
    private SensorManager mManager;
    private Sensor mAccelerometer;//加速度
    private Sensor mCompass;//罗盘

    private float[] accelerometerValues = new float[3];
    private float[] magneticFieldValues = new float[3];

    private float pitch, yaw, roll;

    private EditText etIpAddress;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        mAccelerometer = mManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mCompass = mManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
        etIpAddress = (EditText) findViewById(R.id.et_ip_address);
    }

    /**
     * 发送消息
     *
     * @param view
     */
    public void sendMsg(View view) {
        final ProgressDialog dialog = new ProgressDialog(this);
        dialog.show();

        new Thread(new Runnable() {
            @Override
            public void run() {
                final String urlStr = " http://" + etIpAddress.getText().toString().trim() +
                        "/setdirection?roll=" + roll + "&pitch=" + pitch + "&yaw=" + yaw;
                try {
                    URL url = new URL(urlStr);
                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                    connection.connect();
                    mHandler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(MainActivity.this, "发送成功！\n" + urlStr, Toast.LENGTH_SHORT).show();
                        }
                    }, 100);
                } catch (Exception e) {
                    mHandler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(MainActivity.this, "发送失败！\n" + urlStr, Toast.LENGTH_SHORT).show();
                        }
                    }, 100);
                    e.printStackTrace();
                }
                if (dialog != null) {
                    dialog.dismiss();
                }
            }
        }).start();
    }


    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        if (sensorEvent.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
            magneticFieldValues = sensorEvent.values;
        if (sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
            accelerometerValues = sensorEvent.values;
        calculateOrientation();
    }

    /**
     * 计算角度
     */
    private void calculateOrientation() {
        float[] values = new float[3];
        float[] R = new float[9];
        SensorManager.getRotationMatrix(R, null, accelerometerValues, magneticFieldValues);
        SensorManager.getOrientation(R, values);

        // 要经过一次数据格式的转换，转换为度
        //values[0]: azimuth, rotation around the Z axis.
        roll = (float) Math.toDegrees(values[0]);
        //values[1]: pitch, rotation around the X axis.
        pitch = (float) Math.toDegrees(values[1]);
        //values[2]: roll, rotation around the Y axis.
        yaw = (float) Math.toDegrees(values[2]);
    }


    /**
     * ignore
     *
     * @param sensor
     * @param i
     */
    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }


    /**
     * 注册监听
     */
    @Override
    protected void onResume() {
        super.onResume();
        mManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_GAME);
        mManager.registerListener(this, mCompass, SensorManager.SENSOR_DELAY_GAME);
        //更新显示数据的方法
        calculateOrientation();
    }

    /**
     * 反注册监听
     */
    @Override
    protected void onPause() {
        super.onPause();
        mManager.unregisterListener(this, mAccelerometer);
        mManager.unregisterListener(this, mCompass);
    }
}
