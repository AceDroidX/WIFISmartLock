package com.github.wangxuxin.wifismartlock;

import static java.lang.Thread.sleep;

/**
 * Created by a1274 on 2017/3/4.
 */
class MutliThread implements Runnable{
    public void run(){
        try {
            while (true){
                if(!LockActivity.canSendSocket){
                    sleep(1000);
                    LockActivity.canSendSocket=true;
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
