#include <SoftwareSerial.h>

#define SPEED 150
#define BUFFER_SIZE 10

const int pin_rX = 12;
const int pin_tX = 13;
const int pin_speed = 3;
const int pin_leftF = 0;
const int pin_leftB = 1;
const int pin_rightF = 4;
const int pin_rightB = 2;
const int pin_speedT = 6;
const int pin_turretF = 7;
const int pin_turretB = 5;
const int pin_gunElevationF = 8;
const int pin_gunElevationB = 9;
const int pin_gunFireF = 10;
const int pin_gunFireB = 11;

SoftwareSerial BTserial(pin_rX, pin_tX); // RX | TX

String message = "";
String commandBuffer[BUFFER_SIZE];
int bufferCount;

void setup() { 
  //Serial.begin(9600);
  BTserial.begin(9600);
  
  pinMode(pin_speed, OUTPUT);
  pinMode(pin_leftF, OUTPUT);
  pinMode(pin_leftB, OUTPUT);
  pinMode(pin_rightF, OUTPUT);
  pinMode(pin_rightB, OUTPUT);
  pinMode(pin_speedT, OUTPUT);
  pinMode(pin_turretF, OUTPUT);
  pinMode(pin_turretB, OUTPUT);
  pinMode(pin_gunElevationF, OUTPUT);
  pinMode(pin_gunElevationB, OUTPUT);
  pinMode(pin_gunFireF, OUTPUT);
  pinMode(pin_gunFireB, OUTPUT);
  
  analogWrite(pin_speed, SPEED);
  digitalWrite(pin_leftF, LOW);
  digitalWrite(pin_leftB, LOW);
  digitalWrite(pin_rightF, LOW);
  digitalWrite(pin_rightB, LOW);
  analogWrite(pin_speedT, SPEED);
  digitalWrite(pin_turretF, LOW);
  digitalWrite(pin_turretB, LOW);
  digitalWrite(pin_gunElevationF, LOW);
  digitalWrite(pin_gunElevationB, LOW);
  digitalWrite(pin_gunFireF, LOW);
  digitalWrite(pin_gunFireB, LOW);

  bufferCount = 0;
}
 
void loop(){
  readMessageFromBt();
  execActionBt();
}

void execActionBt() {
  if(!BTserial.available()) {
    if(message!="") {
      //Serial.println(message);
      
      if(message == "a") {
        // left front: a
        digitalWrite(pin_leftF, HIGH);
        digitalWrite(pin_leftB, LOW);
        message = "";  
      } else if(message == "b") {
        // right front: b
        digitalWrite(pin_rightF, HIGH);
        digitalWrite(pin_rightB, LOW);
        message = "";  
      } else if(message == "c") {
        // left back: c
        digitalWrite(pin_leftF, LOW);
        digitalWrite(pin_leftB, HIGH);
        message = "";        
      } else if(message == "d") { 
        // right back: d       
        digitalWrite(pin_rightF, LOW);
        digitalWrite(pin_rightB, HIGH);
        message = "";
      } else if(message == "e") {
        // right stop: e
        digitalWrite(pin_rightF, LOW);
        digitalWrite(pin_rightB, LOW);
        message = "";
      } else if(message == "f") {
        // left stop: f
        digitalWrite(pin_leftF, LOW);
        digitalWrite(pin_leftB, LOW);         
        message = "";
      } else if(message == "g") {        
        // gun start fire: g
        digitalWrite(pin_gunFireF, HIGH);
        digitalWrite(pin_gunFireB, LOW);
        message = "";
      } else if(message == "h") {        
        // gun stop fire: h
        digitalWrite(pin_gunFireF, LOW);
        digitalWrite(pin_gunFireB, LOW);
        message = "";
      } else if(message == "i") {        
        // left turret go: i
        digitalWrite(pin_turretF, HIGH);
        digitalWrite(pin_turretB, LOW);
        message = "";
      } else if(message == "l") {        
        // left turret stop: l
        digitalWrite(pin_turretF, LOW);
        digitalWrite(pin_turretB, LOW);
        message = "";
      } else if(message == "m") {        
        // right turret go: m
        digitalWrite(pin_turretF, LOW);
        digitalWrite(pin_turretB, HIGH);
        message = "";
      } else if(message == "n") {        
        // right turret stop: n
        digitalWrite(pin_turretF, LOW);
        digitalWrite(pin_turretB, LOW);
        message = "";
      } else if(message == "o") {        
        // gun up go: o
        digitalWrite(pin_gunElevationF, HIGH);
        digitalWrite(pin_gunElevationB, LOW);
        message = "";
      } else if(message == "p") {        
        // gun up stop: p
        digitalWrite(pin_gunElevationF, LOW);
        digitalWrite(pin_gunElevationB, LOW);
        message = "";
      } else if(message == "q") {        
        // gun down go: q
        digitalWrite(pin_gunElevationF, LOW);
        digitalWrite(pin_gunElevationB, HIGH);
        message = "";
      } else if(message == "r") {        
        // gun down stop: r
        digitalWrite(pin_gunElevationF, LOW);
        digitalWrite(pin_gunElevationB, LOW);
        message = "";
      } /*else {

        
        String m = message.substring(3, 4);
        Serial.println(m);
        if(m == "a") {
          int v = message.substring(0, 3).toInt();
          analogWrite(motor1, v);
          Serial.println(v);
          message = "";
        } else if(m == "d") {
          int v = message.substring(0, 3).toInt();
          analogWrite(motor2, v);
          Serial.println(v);
          message = "";
        } else if(m == "e") {
          int v = message.substring(0, 3).toInt();
          analogWrite(gunElevation, v);
          Serial.println(v);
          message = "";
        }
        
      } */         
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

