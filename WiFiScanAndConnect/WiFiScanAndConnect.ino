/*
  本工程用来进行Wemos D1(ESP8266)的WiFi搜索和连接
*/
#include <ESP8266WiFi.h>

String cmd;
String inputString="";
char cmdMode;
bool stringComplete;


void setup(){ 
    Serial.begin(115200);

    //初始化设置
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    cmdMode = 'c';  //初始模式为命令模式
    Serial.println("Setup done!");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    //Serial.println("available");
    //Serial.print(Serial.available());
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else{
      inputString += inChar;
    }
  }
}

void loop(){
    serialEvent();
    if(stringComplete){
        // Serial.println(cmdMode);
        // delay(2000);
        switch(cmdMode){
            case 'c':
            cmd = inputString;
            stringComplete = false;
            Serial.println(cmd);
            inputString = "";
            break;
        }
    }
    
}