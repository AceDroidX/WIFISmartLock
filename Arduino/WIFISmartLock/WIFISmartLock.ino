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
 
Servo myservo;  // 
                // 最多可以控制8路舵机
 
String comdata = "";
String comdataarray[100];
 int pos = 0;    // 用于存储舵机位置的变量
int zt=0;
int lockzt=0;//0锁 1没锁
unsigned long previousMillis = 0;

unsigned int versionCode = 3;
unsigned long versionName = 2.0;
int isdebug=0;//0-false 1-true

//初始化
void setup() 
{ 
  Serial.begin(9600);
  //Serial.begin(115200,SERIAL_8N1);
  myservo.attach(9);  // 舵机控制信号引脚 
  myservo.write(90);              // 输入对应的角度值，舵机会转到此位置
	keyIsAvailable();
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
	char delims[] = " ";
	char *result = NULL;
	int i = 0;
	result = strtok( comdata.c_str(), delims );
	while( result != NULL ) {
		comdataarray[i]=result;
		i++;
		result = strtok( NULL, delims );
	}
	if(isdebug&&comdataarray[0]!=""){
		Serial.println(comdataarray[0]);
	}
	
	//////////////////////////////////////////////////////////////////////////检测
	
	if (comdataarray[0]=="islock"){
		Serial.println("issmartlock");
	}
	if(comdataarray[0]=="initialization"){
		if(EEPROM.read(23)==0){
			Serial.println("notInitialize");
		}
		if(EEPROM.read(23)==1){
			Serial.println("alreadyInitialize");
		}
	}
	if(comdataarray[0]=="status"){
		if(zt==0){
			Serial.println("lock");
		}
		if(zt==1){
			Serial.println("notlock");
		}
	}
	if(comdataarray[0]=="iskey"){
		if(EEPROM.read(23)==0){
			Serial.println("notInitialize");
		}
		if(comdataarray[1]==getKey()){
			Serial.println("keycorrect");
		}else{
			Serial.println("keywrong");
		}
	}
	/////////////////////////////////////////////////////////////////////////setkey
	if(comdataarray[0]=="setkey"){
		if(EEPROM.read(23)==0){
			for(i=1;i<=5;i++){
				if(atoi(comdataarray[i].c_str())==0){
					Serial.println("thekeycantbe0");
					cleanVar();
					return;
				}
			}
			if(keyIsAvailable){
				EEPROM.write(28,atoi(comdataarray[5].c_str()));
				EEPROM.write(27,atoi(comdataarray[4].c_str()));
				EEPROM.write(26,atoi(comdataarray[3].c_str()));
				EEPROM.write(25,atoi(comdataarray[2].c_str()));
				EEPROM.write(24,atoi(comdataarray[1].c_str()));
				EEPROM.write(23,1);
				Serial.println("setkeysuccess");
			}else{
				Serial.println("setkeyerror");
			}
			/*
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
			*/
		}else{
			Serial.println("alreadysetkey");
		}
	}
	
	////////////////////////////////////////////////////////////////////////////设置
	
	if(comdataarray[0]=="switch"){
		if(currentMillis-previousMillis<1000){
			cleanVar();
			return;
		}
		if(EEPROM.read(23)==0){
			Serial.println("notInitialize");
			cleanVar();
			return;
		}
		String pw=getKey();
		//if(atoi(comdataarray[1].c_str())!=pw){
		if(comdataarray[1]!=pw){
			Serial.println("keywrong");
			cleanVar();
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
	if(comdataarray[0]=="reset"){
		if(comdataarray[1]!=EEPROM.read(24)){
			Serial.println("keywrong");
			cleanVar();
			return;
		}
		if(EEPROM.read(23)==0){
			Serial.println("notInitialize");
			cleanVar();
			return;
		}
		EEPROM.write(24,0);
		EEPROM.write(23,0);
		Serial.println("resetsuccess");
	}
	
	//////////////////////////////////////////////////////////////////////////////debug
	
	if(isdebug&&comdataarray[0]=="version"){
		Serial.println(versionCode+" "+versionName);
	}
	if(comdataarray[0]=="test1"){//Debug命令
		Serial.println("----start---test1--------");
		Serial.println(EEPROM.read(23));
		String pw=getKey();
		Serial.println(pw);
		Serial.println("------end---test1--------");
	}
	if(comdataarray[0]=="test2"){//Debug命令
		for(i=24;i<=28;i++){
			EEPROM.write(i,0);
		}
		EEPROM.write(23,0);
		Serial.println("ok");
	}
	/*
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
	*/
	cleanVar();
}

void cleanVar(){
	comdata="";
	if(comdataarray[0]!=""){
		for(int i=0;i<=100;i++){
			comdataarray[i]="";
		}
	}
}

void resetEEPROM(){
	int i;
	for(i=24;i<=28;i++){
		EEPROM.write(i,0);
	}
	EEPROM.write(23,0);
}

int keyIsAvailable(){//0-reset 1-notreset
	int i;
	if(EEPROM.read(23)==1&&EEPROM.read(24)==0){
		resetEEPROM();
		return 0;
	}
	
	String pw=getKey();
	if(atoi(pw.c_str())<1000000000||atoi(pw.c_str())>9999999999){
		resetEEPROM();
		return 0;
	}
	return 1;
}

String getKey(){
	String pw;
	for(int i=24;i<=28;i++){
		pw+=EEPROM.read(i);
	}
	return pw;
}