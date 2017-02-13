/*
 http://shop34791330.taobao.com/ //德飞莱旗舰店
 http://qixingchong.tmall.com/   //七星虫旗舰店
 http://www.doflye.net //德飞莱技术论坛
 http://www.doflye.net/forum-98-1.html //德飞莱论坛arduino版块
 舵机左右摇摆功能
 德飞莱LY-51S独立模块开发板有舵机功能块，与arduino的连接图片和说明如下：
 http://www.doflye.net/viewthread.php?tid=5311&extra=
 感谢以上提供参考 By wxx
*/
#include <Servo.h> 
#include <EEPROM.h>

#define SERIAL_BUFFER_SIZE 128
 
Servo myservo;  // 
                // 最多可以控制8路舵机
 
String comdata = "";
 int pos = 0;    // 用于存储舵机位置的变量
int zt=0;
int lockzt=0;//0锁 1没锁
unsigned long previousMillis = 0;
unsigned long keyMillis = 0;

unsigned int versionCode = 2;
unsigned long versionName = 1.1;

//初始化
void setup() 
{ 
  //Serial.begin(9600);
  Serial.begin(115200,SERIAL_8N1);
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
	if(atoi(comdata.c_str())!=0&&atoi(comdata.c_str())==EEPROM.read(24)){
		keyMillis=currentMillis;
		Serial.println("keycorrect");
	}
	if(comdata=="setkey"){
		if(EEPROM.read(23)==0){
			zt=3;
			comdata="";
			Serial.println("setkeying");
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
	if(comdata=="switch"){
		if(currentMillis-keyMillis<1000){
			if(currentMillis-previousMillis<1000){
				return;
			}
			previousMillis=currentMillis;
			if(zt==0){
				myservo.write(60);
				delay(500);
				myservo.write(70);//锁住的角度
				zt=1;
				Serial.println("zt1");
			}
			else if(zt==1){
				myservo.write(110);
				delay(500);
				myservo.write(100);//没锁的角度
				zt=0;
				Serial.println("zt0");
			}
		}
	}
	if(comdata=="reset"){
		if(currentMillis-keyMillis<1000){
			EEPROM.write(24,0);
			EEPROM.write(23,0);
			Serial.println("resetsuccess");
		}
	}
	if(comdata=="version"){
		Serial.println(versionCode+" "+versionName);
	}
	if(comdata=="test1"){//Debug命令
		Serial.println(comdata==EEPROM.read(23));
		Serial.println(comdata==EEPROM.read(24));
		Serial.println(EEPROM.read(23));
		Serial.println(EEPROM.read(24));
	}
	if(comdata=="test2"){//Debug命令
		EEPROM.write(24,0);
		EEPROM.write(23,0);
		Serial.println("ok");
	}
	if (comdata=="set"){//Debug命令
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
