package com.github.wangxuxin.wifismartlock;

import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;

/**
 * Created by a1274 on 2017/2/9.
 */
public class TCPSocket {
    String echo="unknown";

    void socket(final String name, final int port, final String str){
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                //客户端请求与本机在20006端口建立TCP连接
                Log.i("wxxDeb","connect "+name+":"+port+" "+str);
                Socket client = null;
                try {
                    client = new Socket(name, port);
                    client.setSoTimeout(1000);
                    //获取Socket的输出流，用来发送数据到服务端
                    PrintStream out = new PrintStream(client.getOutputStream());
                    //获取Socket的输入流，用来接收从服务端发送过来的数据
                    BufferedReader buf =  new BufferedReader(new InputStreamReader(client.getInputStream()));
                    //发送数据到服务端
                    out.println(str);
                    //从服务器端接收数据有个时间限制（系统自设，也可以自己设置），超过了这个时间，便会抛出该异常
                    echo = buf.readLine();
                    Log.i("wxxDebug1",echo);
                    client.close();
                }catch(IOException e) {
                    System.out.println("Time out, No response");
                }
            }
        });
        thread.start();
    }
}
