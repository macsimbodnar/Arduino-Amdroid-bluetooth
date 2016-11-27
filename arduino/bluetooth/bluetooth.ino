#include <SoftwareSerial.h>

const int motor1 = 11;
const int m1d1 = 8;
const int m1d2 = 7;
const int motor2 = 10;
const int m2d1 = 4;
const int m2d2 = 2;
const int rX = 12;
const int tX = 13;

SoftwareSerial BTserial(rX, tX); // RX | TX

String message = "";

int speed1 = 0;
int speed2 = 0;

void setup() {  
  Serial.begin(9600);
  BTserial.begin(9600);
  
  pinMode(motor1, OUTPUT); 
  pinMode(motor2, OUTPUT);  
  pinMode(m1d1, OUTPUT);  
  pinMode(m1d2, OUTPUT);  
  pinMode(m2d1, OUTPUT);  
  pinMode(m2d2, OUTPUT);  

  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
 
  digitalWrite(m1d1, HIGH);
  digitalWrite(m1d2, LOW);
  digitalWrite(m2d1, HIGH);
  digitalWrite(m2d2, LOW);
}
 
void loop(){
  readMessageFromBt();
  execActionBt();
}

void execActionBt() {
  if(!BTserial.available()) {
    if(message!="") {
      String m = message.substring(3, 4);
      //Serial.println(m);
      if(m == "l") {
        int v = message.substring(0, 3).toInt();
        analogWrite(motor1, v);
        //Serial.println(v);
        message = "";
      } else if(m == "r") {
        int v = message.substring(0, 3).toInt();
        analogWrite(motor2, v);
        //Serial.println(v);
        message = "";
      }
    }
  }
}

void readMessageFromBt(){
  while(BTserial.available()){
    char c = char(BTserial.read());
    if(c == '~') {
      message = "";
    } else {
      message += c;          
    }
  }
}

void sendToBt(String message) {
  BTserial.print("~" + message);
}

