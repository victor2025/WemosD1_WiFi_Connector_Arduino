/*
  本工程用来进行Wemos D1(ESP8266)的WiFi搜索和连接
*/
#include <ESP8266WiFi.h>
const char* host = "djxmmx.net";
const uint16_t port = 17;
const char* temp = "none";
String ssid;
String password;
String inputString;
String cmd="";
bool stringComplete=false;
bool guide=true;
bool cmdMode=true;
char sORp='s';

//首先初始化ssid和密码

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); //首先断开网络连接
  delay(100);
  Serial.println("Setup done");
}

void scan() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
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
  Serial.println("");

  // Wait a bit before scanning again
  //delay(5000);
}

void connect_WiFi() {
  //  char ssid_now=ssid;
  //  char password_now=password;
  Serial.print("Connecting to ");
  Serial.println(ssid.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());
  int i = 20;
  while ((WiFi.status() != WL_CONNECTED) && (i > 0)) {
    delay(800);
    Serial.print(".");
    i = i - 1;
  }
  if (i > 0) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("WiFi connection failed.");
    Serial.println("Please try again!");
  }
}

void connect_host() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  delay(300000); // execute once every 5 minutes, don't flood remote service
}

void input(int flag) {
  if (flag == 1) {
    Serial.println("Please input your command:");
    delay(500);
  }
  else if (flag == 2) {
    Serial.println("Please input ssid:");
    delay(500);
  }
  else if (flag == 3) {
    Serial.println("Please input password:");
    delay(500);
  }
}

String serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    //Serial.println("available");
    //Serial.print(Serial.available());
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    guide = true;//输入完成后需要进一步提示
    return inputString;
  }
}

void command() {
  if (cmd.equals("connect")) {
    Serial.println(cmd);
    if(sORp=='s'){
      if(guide){
        input(2);
      }
      if(stringComplete){
        ssid = inputString;
        sORp = 'p';
        guide = true;
        stringComplete = false;
      }
    }
    else if(sORp == 'p'){
      if(guide){
        input(3);
      }
      if(stringComplete){
        password = inputString;
        sORp = 's';
        guide = true;
        stringComplete = false;
      }
    }
    if(guide){
      connect_WiFi();
      guide = false;
      cmdMode = true;
    }
    
  }
  else if (cmd.equals("host")) {
    connect_host();
  }
  else if (cmd.length()>0){
    Serial.println("Wrong command!");
    cmd = "";
    cmdMode = true;  
  }
}

void loop() {
  serialEvent();
  if(cmdMode){
    if(guide){
      input(1);
      delay(200);
      guide = false;
    }
    if (stringComplete){
      cmd = inputString;
      Serial.println(cmd);
      guide = true;  
      stringComplete = false;
      cmdMode = false;
      Serial.println(cmd=="connect");
    }
  }
  delay(1000);
  command();  
}
