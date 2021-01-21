/*
  1.本工程用来进行Wemos D1(ESP8266)的WiFi搜索和连接
  2.cmdMode和guide的模式如下：
    模式c:命令行模式；
    模式s:WiFi连接，读取ssid模式；
    模式p:WiFi连接，读取password模式；
    模式w:WiFi连接，开始连接WiFi;
    模式t:获取连接状态(status)模式;
*/
#include <ESP8266WiFi.h>

String cmd;
String inputString="";
String ssid,password;

char cmdMode,guide; //cmdMode定义命令模式，guide定义指示模式
bool stringComplete=true;
bool cmdPool = false;   //定义当前是否有命令需要执行，为真则执行，为假则等待输入


void setup(){ 
    Serial.begin(115200);

    //初始化设置
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    cmdMode = 'c';  //初始模式为命令模式
    guide='c';
    delay(2000);
    Serial.println(">>Setup done!");
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

void printStar(String para){
    //打出星号
    int i;
    for(i = para.length();i>0;i--){
        Serial.print('*');
    }
    Serial.println('\n');
}

void wifiConnect(){
    int timeOut=25;
    Serial.println("#**************************#");
    Serial.println("Connecting to "+ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);

    while ((WiFi.status() != WL_CONNECTED)&&(timeOut>0)) {
        delay(600);
        Serial.print(".");
        timeOut--;
    }
    Serial.println("S");
    if (timeOut == 0){
        Serial.println("WiFi connection failed!");
    }
    else{
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    Serial.println("#**************************#\n");
}

void wifiDisconnect(){
    Serial.println("#**************************#");

    WiFi.disconnect();
    Serial.println("Disconnected from "+ssid+"!");
    ssid = "";
    password = "";

    Serial.println("#**************************#\n");
}

void wifiStatus(){
    Serial.println("#**************************#");
    if(WiFi.status()==WL_CONNECTED){
        Serial.println("Connecting to "+ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else{
        Serial.println("WiFi disconnected.");
    }
    Serial.println("#**************************#\n");
}

void loop(){
    serialEvent();
    //指示打印
    if(guide!='n'){
        switch(guide){
            case 'c':
            Serial.println(">>Please input your command.");
            guide = 'n';
            break;

            case 's':
            Serial.println(">>please input your SSID.");
            guide = 'n';
            break;

            case 'p':
            Serial.println(">>Please input your password.");
            guide = 'n';
            break;

        }
    }
    //读取命令
    if(stringComplete&&(inputString.length()!=0)){
        // Serial.println(cmdMode);
        // delay(2000);
        switch(cmdMode){

            case 'c':   //命令模式
            cmd = inputString;
            Serial.println("Command:"+cmd+'\n');
            //输入缓冲区归零
            inputString = "";
            stringComplete = false;
            cmdPool = true;
            break;

            case 's':   //ssid模式
            ssid = inputString;
            Serial.println("SSID:"+ssid+'\n');
            //输入缓冲区归零
            inputString = "";
            stringComplete = false;
            cmdPool = true;
            break;

            case 'p':   //password模式
            password = inputString;
            Serial.print("Password:");
            printStar(password);
            //输入缓冲区归零
            inputString = "";
            stringComplete = false;
            cmdPool = true;
            break;

        }
    }
    //执行命令
    if(cmdPool){
        switch(cmdMode){
            case 'c':
                if(cmd=="connect"){
                    cmd = "";
                    guide = 's';
                    cmdMode = 's';
                    cmdPool = false;
                }
                else if(cmd=="disconnect"){
                    wifiDisconnect();
                    cmd = "";
                    guide = 'c';
                    cmdMode = 'c';
                    cmdPool = false;
                }
                else if(cmd == "status"){
                    wifiStatus();
                    cmd = "";
                    guide = 'c';
                    cmdPool = false;
                }
            break;

            case 's':
                guide = 'p';
                cmdMode = 'p';
                cmdPool = false;
            break;

            case 'p':
                cmdMode = 'w';
            break;

            case 'w':
                wifiConnect();
                guide = 'c';
                cmdMode = 'c';
                cmdPool = false;
            break;
        }
    }
}