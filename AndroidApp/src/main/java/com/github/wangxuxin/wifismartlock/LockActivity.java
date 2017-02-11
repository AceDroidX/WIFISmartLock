package com.github.wangxuxin.wifismartlock;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class LockActivity extends AppCompatActivity {
    String lockip;
    String lockpw;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lock);

        Intent intent = getIntent();
        final String type = intent.getStringExtra("type");

        SharedPreferences locklistSP = getSharedPreferences("lock", 0);
        lockip = locklistSP.getString("ip",null);

        if("1".equals(type)){
            setContentView(R.layout.activity_set);
            Button keysetButton =(Button)findViewById(R.id.keysetButton);
            keysetButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    final TCPSocket keysetSocket = new TCPSocket();
                    final EditText keysetEdit =(EditText)findViewById(R.id.keysetEdit);
                    //1、打开Preferences，名称为setting，如果存在则打开它，否则创建新的Preferences
                    SharedPreferences isFirstOpen = getSharedPreferences("lock", 0);
                    //2、让setting处于编辑状态
                    SharedPreferences.Editor editor = isFirstOpen.edit();
                    //3、存放数据
                    editor.putString("password",keysetEdit.getText().toString());
                    //4、完成提交
                    editor.apply();
                    keysetSocket.socket(lockip,9000,"setkey");
                    new Handler().postDelayed(new Runnable(){
                        public void run() {
                            //execute the task
                            keysetSocket.socket(lockip,9000,keysetEdit.getText().toString());
                            keysetSocket.socket(lockip,9000,keysetEdit.getText().toString());
                            setContentView(R.layout.activity_lock);
                        }
                    }, 1000);
                }
            });
        }

        lockpw = locklistSP.getString("password",null);

        /*
        Button switchButton = (Button)findViewById(R.id.switchButton);
        Button resetButton = (Button)findViewById(R.id.resetButton);

        switchButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TCPSocket lockSocket = new TCPSocket();
                lockSocket.socket(lockip,9000,"key");
                lockSocket.socket(lockip,9000,lockpw);
                lockSocket.socket(lockip,9000,"switch");
            }
        });
        resetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TCPSocket lockSocket = new TCPSocket();
                lockSocket.socket(lockip,9000,"key");
                lockSocket.socket(lockip,9000,lockpw);
                lockSocket.socket(lockip,9000,"reset");
            }
        });
        */
    }

    public void switchButton(View v){
        final TCPSocket lockSocket = new TCPSocket();
        lockSocket.socket(lockip,9000,lockpw);
        new Handler().postDelayed(new Runnable(){
            public void run() {
                //execute the task
                lockSocket.socket(lockip,9000,"switch");
            }
        }, 100);
    }

    public void resetButton(View v){
        final TCPSocket lockSocket = new TCPSocket();
        lockSocket.socket(lockip,9000,lockpw);
        new Handler().postDelayed(new Runnable(){
            public void run() {
                //execute the task
                lockSocket.socket(lockip,9000,"reset");
            }
        }, 100);
    }
}
