/*
  1.本工程用来进行Wemos D1(ESP8266)的WiFi搜索和连接
  2.cmdMode和guide的模式如下：
    模式c:命令行模式；
    模式s:WiFi连接，读取ssid模式；
    模式p:WiFi连接，读取password模式；
    模式w:WiFi连接，开始连接WiFi;
    模式t:获取连接状态(status)模式;
    模式A:AP模式，读取APssid；
    模式P:AP模式，读取APpassword；
    模式b:AP模式，开启AP热点；
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

String cmd;
String inputString="";
String ssid,password;
String APssid,APpassword;
String number="hey!";
int i=0;

char cmdMode,guide; //cmdMode定义命令模式，guide定义指示模式
bool stringComplete=true;
bool cmdPool = false;   //定义当前是否有命令需要执行，为真则执行，为假则等待输入
bool server_if = false;

ESP8266WebServer server(80);

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
    WiFi.disconnect();
    WiFi.begin(ssid,password);

    while ((WiFi.status() != WL_CONNECTED)&&(timeOut>0)) {
        delay(600);
        Serial.print(".");
        timeOut--;
    }
    Serial.println("");
    if (timeOut == 0){
        Serial.println("WiFi connection failed!");
        ssid = "";
        password = "";
    }
    else{
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        APssid="";
        APpassword = "";
    }
    Serial.println("#**************************#\n");
}

void wifiDisconnect(){
    Serial.println("#**************************#");

    if (WiFi.status() == WL_CONNECTED){
        WiFi.disconnect();
        Serial.println("Disconnected from "+ssid+"!");
        ssid = "";
        password = "";
        APssid = "";
        APpassword = "";
    }
    else{
        Serial.println("No WiFi has been connected!");
    }
    // Serial.println(WiFi.softAPIP());

    Serial.println("#**************************#\n");
}

void wifiStatus(){
    Serial.println("#**************************#");
    if(WiFi.status()==WL_CONNECTED){
        Serial.println("Connected to "+ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else{
        Serial.println("WiFi disconnected.");
    }
    Serial.println("#**************************#\n");
}

void wifiScan(){
    Serial.println("#**************************#");
    Serial.println("Scan start");
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("No networks found");
    } else {
        Serial.print(n);
        Serial.println(" Networks found:");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("#**************************#\n");
}

void handleRoot() {
  server.send(200, "text/html", "<meta http-equiv=\"refresh\" content=\"0.01\"><h1>"+number+"</h1>");
}

void wifiAP(){
    Serial.println("#**************************#");
    Serial.print("Configuring Acess point "+APssid+"...");

    WiFi.softAP(APssid,APpassword);

    Serial.println("");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", handleRoot);
    server.begin();

    Serial.println("HTTP server started");
    Serial.println("#**************************#\n");
}

void APdisconnect(){
    Serial.println("#**************************#");

    WiFi.softAPdisconnect();
    Serial.println("Acess point "+APssid+" has been closed!");
    ssid = "";
    password = "";
    APssid = "";
    APpassword = "";
    // if (WiFi.softAPIP()){
    //     WiFi.disconnect();
    //     Serial.println("Acess point "+APssid+"has been closed!");
    //     ssid = "";
    //     password = "";
    //     APssid = "";
    //     APpassword = "";
    // }
    // else{
    //     Serial.println("No AP has started!");
    // }
    // Serial.println(WiFi.softAPIP());

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

            case 'A':
            Serial.println(">>Please input your SSID of AP.");
            guide = 'n';
            break;

            case 'P':
            Serial.println(">>Please input your password of AP.");
            guide = 'n';
            break;
        }
    }
    //读取命令
    if(stringComplete&&(inputString.length()!=0)){
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

            case 'A':   //password模式
            APssid = inputString;
            Serial.println("APssid:"+APssid+'\n');
            //输入缓冲区归零
            inputString = "";
            stringComplete = false;
            cmdPool = true;
            break;

            case 'P':   //password模式
            APpassword = inputString;
            Serial.print("APpassword:");
            printStar(APpassword);
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
                }
                else if(cmd=="disconnect"){
                    wifiDisconnect();
                    cmd = "";
                    guide = 'c';
                    cmdMode = 'c';
                }
                else if(cmd == "status"){
                    wifiStatus();
                    cmd = "";
                    guide = 'c';
                }
                else if(cmd == "scan"){
                    wifiScan();
                    cmd = "";
                    guide = 'c';
                }
                else if(cmd == "acesspointon"){
                    cmd = "";
                    guide = 'A';
                    cmdMode = 'A';
                }
                else if(cmd == "acesspointoff"){
                    APdisconnect();
                    cmd = "";
                    guide = 'c';
                    cmdMode = 'c';
                }
                else if(cmd == "serveron"){
                    server_if = true;
                    Serial.println("Server is on!\n");
                    guide = 'c';
                    cmdMode = 'c';
                }
                else if(cmd == "serveroff"){
                    server_if = false;
                    Serial.println("Server is off!\n");
                    guide = 'c';
                    cmdMode = 'c';
                }
                else{
                    Serial.println("No such command!\n");
                    cmd = "";
                    guide = 'c';
                    cmdMode = 'c';
                }
                cmdPool = false;
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

            case 'A':
                guide = 'P';
                cmdMode = 'P';
                cmdPool = false;
            break;

            case 'P':
                cmdMode = 'b';
            break;

            case 'b':
                wifiAP();
                guide = 'c';
                cmdMode = 'c';
                cmdPool = false;
            break;
        }
    }
    //开启服务器
    i++;
    number = (String)i;
    if(server_if){
        server.handleClient();
    }
}