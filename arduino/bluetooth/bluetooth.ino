#include <SoftwareSerial.h>
const int gun = 5;
const int gunElevation = 2;
const int gunD1 = 3;
const int gunD2 = 4;
const int motor1 = 11;
const int m1d1 = 10;
const int m1d2 = 9;
const int motor2 = 7;
const int m2d1 = 6;
const int m2d2 = 8;
const int rX = 12;
const int tX = 13;

SoftwareSerial BTserial(rX, tX); // RX | TX

String message = "";

int speed1 = 0;
int speed2 = 0;
boolean fire = false;

void setup() {  
  Serial.begin(9600);
  BTserial.begin(9600);
  
  pinMode(gun, OUTPUT); 
  pinMode(gunElevation, OUTPUT); 
  pinMode(gunD1, OUTPUT);
  pinMode(gunD2, OUTPUT);
  pinMode(motor1, OUTPUT); 
  pinMode(motor2, OUTPUT);  
  pinMode(m1d1, OUTPUT);  
  pinMode(m1d2, OUTPUT);  
  pinMode(m2d1, OUTPUT);  
  pinMode(m2d2, OUTPUT);  

  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW); 
  digitalWrite(m1d1, LOW);
  digitalWrite(m1d2, LOW);
  digitalWrite(m2d1, LOW);
  digitalWrite(m2d2, LOW);  
  digitalWrite(gun, LOW);
  digitalWrite(gunElevation, LOW);
  digitalWrite(gunD1, LOW);
  digitalWrite(gunD2, HIGH);
}
 
void loop(){
  readMessageFromBt();
  execActionBt();
}

void execActionBt() {
  if(!BTserial.available()) {
    if(message!="") {
      
      if(message == "af") {
        digitalWrite(m1d1, LOW);
        digitalWrite(m1d2, HIGH);
        digitalWrite(m2d1, LOW);
        digitalWrite(m2d2, HIGH);
        message = "";  
      } else if(message == "ab") {
        digitalWrite(m1d1, HIGH);
        digitalWrite(m1d2, LOW);
        digitalWrite(m2d1, HIGH);
        digitalWrite(m2d2, LOW);   
        message = "";  
      } else if(message == "go") {
        analogWrite(motor1, 255);
        analogWrite(motor2, 255);
        message = "";
      } else if(message == "st") {
        analogWrite(motor1, 0);
        analogWrite(motor2, 0);
        message = ""; 
      } else if(message == "lf") {
        digitalWrite(m1d1, LOW);
        digitalWrite(m1d2, HIGH);
        message = "";  
      } else if(message == "rf") {
        digitalWrite(m2d1, LOW);
        digitalWrite(m2d2, HIGH);        
        message = "";  
      } else if(message == "lb") {
        digitalWrite(m1d1, HIGH);
        digitalWrite(m1d2, LOW);
        message = "";        
      } else if(message == "rb") {
        digitalWrite(m2d1, HIGH);
        digitalWrite(m2d2, LOW);
        message = "";  
      } else if(message == "gf") {        
        if(fire == true) {
          digitalWrite(gun, LOW);
          fire = false;
        } else {
          digitalWrite(gun, HIGH);
          fire = true;
        }
        message = "";
      } else {
        
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
        } else if(m == "e") {
          int v = message.substring(0, 3).toInt();
          analogWrite(gunElevation, v);
          //Serial.println(v);
          message = "";
        }
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

