
String cmd,inputString;
bool stringComplete=false;
void setup() {
  Serial.begin(115200);
  delay(1000);
  cmd = "";
  Serial.println(cmd.length());
  Serial.println(cmd.length()>0);
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

void loop() {
  serialEvent();
  if(stringComplete){
    cmd = inputString;
    stringComplete=false;
    inputString = "";
  } 
  if(cmd.length()>0){
    Serial.println(cmd);
    Serial.println(cmd=="test");
    if(cmd=="test"){
      Serial.println("Hello, World!");  
      cmd = "";
    }
    else{
      Serial.println("Wrong command!");
      delay(100);
      cmd = ""; 
    }
  }
  
}
