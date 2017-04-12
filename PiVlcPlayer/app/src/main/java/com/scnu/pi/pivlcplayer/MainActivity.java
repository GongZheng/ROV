package com.scnu.pi.pivlcplayer;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button btnPlayer;
    private EditText editText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnPlayer = (Button) findViewById(R.id.btn_player);
        editText = (EditText) findViewById(R.id.Pi_ipaddress);
        btnPlayer.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn_player:
                String content=editText.getText().toString();
                Intent intent = new Intent(this,PlayerActivity.class);
                Bundle bundle = new Bundle();
                bundle.putString("Pi_IP",content);
                intent.putExtras(bundle);
                startActivity(intent);

                break;
        }
    }
}
