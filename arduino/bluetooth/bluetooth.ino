#include <SoftwareSerial.h>

const int pinBuzzer = 3;
const int pinLed = 4;
const int rX = 12;
const int tX = 13;
String message = "";

SoftwareSerial BTserial(rX, tX); // RX | TX

void setup() {
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
  
  Serial.begin(9600);
  BTserial.begin(9600);  
}
 
void loop(){
  
  while(BTserial.available()){
    char c = char(BTserial.read());
    if(c == '~') {
      message = "";
    } else {
      message += c;
    }
  }
  
  if(!BTserial.available()) {
    if(message!="") {
      if(message == "on") {
        digitalWrite(pinLed, HIGH);
        send("Led on");
        message = "";
      } else if(message == "off") {        
        digitalWrite(pinLed, LOW);
        send("Led off");
        message = "";        
      }
    }
  }
}


void send(String message) {
  BTserial.print("~" + message);
}
