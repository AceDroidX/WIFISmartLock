/*
 http://shop34791330.taobao.com/ //德飞莱旗舰店
 http://qixingchong.tmall.com/   //七星虫旗舰店
 http://www.doflye.net //德飞莱技术论坛
 http://www.doflye.net/forum-98-1.html //德飞莱论坛arduino版块
 舵机左右摇摆功能
 德飞莱LY-51S独立模块开发板有舵机功能块，与arduino的连接图片和说明如下：
 http://www.doflye.net/viewthread.php?tid=5311&extra=
*/
#include <Servo.h> 
#include <EEPROM.h>
 
Servo myservo;  // 
                // 最多可以控制8路舵机
 
String comdata = "";
 int pos = 0;    // 用于存储舵机位置的变量
int zt=0;
int lockzt=0;//0锁 1没锁
unsigned long previousMillis = 0;
unsigned long keyMillis = 0;

//初始化
void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(9);  // 舵机控制信号引脚 
  myservo.write(90);              // 输入对应的角度值，舵机会转到此位置
} 
 
//主循环
void loop() 
{ 
	unsigned long currentMillis = millis();
	
	while (Serial.available() > 0)  
    {
        comdata += char(Serial.read());
        delay(2);
    }
	if(EEPROM.read(23)==1&&EEPROM.read(24)==0){
		EEPROM.write(24,0);
		EEPROM.write(23,0);
	}
	if (comdata=="islock"){
		Serial.println("issmartlock");
	}
	if(comdata=="initialization"){
		if(EEPROM.read(23)==0){
			Serial.println("notInitialize");
		}
		if(EEPROM.read(23)==1){
			Serial.println("alreadyInitialize");
		}
	}
	if(comdata=="status"){
		if(zt==0){
			Serial.println("lock");
		}
		if(zt==1){
			Serial.println("notlock");
		}
	}
	if(comdata=="setkey"){
		if(EEPROM.read(23)==0){
			zt=3;
			comdata="";
			while (zt==3){
				while (Serial.available() > 0)  {
					comdata += char(Serial.read());
					delay(2);
				}
				if(comdata.length() > 0){
					EEPROM.write(24,atoi(comdata.c_str()));
					EEPROM.write(23,1);
					Serial.println("setkeysuccess");
					zt=0;
					break;
				}
			}
		}
	}
	if(atoi(comdata.c_str())!=0&&atoi(comdata.c_str())==EEPROM.read(24)){
		keyMillis=currentMillis;
		Serial.println("keycorrect");
	}
	if(comdata=="switch"){
		Serial.println(currentMillis-keyMillis);
		if(currentMillis-keyMillis<500){
			if(currentMillis-previousMillis<1000){
				return;
			}
			previousMillis=currentMillis;
			if(zt==0){
				myservo.write(60);
				zt=1;
				Serial.println("zt1");
			}
			else if(zt==1){
				myservo.write(100);
				zt=0;
				Serial.println("zt0");
			}
		}
	}
	if(comdata=="reset"){
		if(currentMillis-keyMillis<500){
			EEPROM.write(24,0);
			EEPROM.write(23,0);
			Serial.println("resetsuccess");
		}
	}
	if(comdata=="test1"){
		Serial.println(comdata==EEPROM.read(23));
		Serial.println(comdata==EEPROM.read(24));
		Serial.println(EEPROM.read(23));
		Serial.println(EEPROM.read(24));
	}
	if(comdata=="test2"){
		EEPROM.write(24,0);
		EEPROM.write(23,0);
		Serial.println("ok");
	}
	if (comdata=="set"){
		if(currentMillis-previousMillis<1000){
			return;
		}
		previousMillis=currentMillis;
		zt=3;
		comdata="";
		Serial.println("set");
		while (zt==3)  
		{
			while (Serial.available() > 0)  
			{
				comdata += char(Serial.read());
				delay(2);
			}
			if(comdata.length() > 0){
				zt=0;
				break;
			}
		}
		myservo.write(atoi(comdata.c_str()));
		Serial.println(atoi(comdata.c_str()));
	}
	comdata="";
} 
