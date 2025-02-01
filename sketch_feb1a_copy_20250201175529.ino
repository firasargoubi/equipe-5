#include <IRremote.h>
#include <IRremoteInt.h>
#include <Servo.h>

const int RECV_PIN = 7, MOVE_PIN = 11, DIR_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

int servoPin = 3;
Servo Servo1;

int motorPin = 9;      // Broche PWM pour le moteur
int speedValue = 0; 


void controlServo(int angle){
   Servo1.attach(servoPin);
   Servo1.write(angle); 
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(MOVE_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(motorPin, OUTPUT);
  controlServo(90);
  arreterMoteur();
}

int getDirection() {
  int result = -1;
  if (irrecv.decode(&results)){
        if (results.value == 0xFF22DD) {

            result = 1;
        } 
        else if (results.value == 0xFFA857) {  result = 2;}
        else if (results.value == 0xFFC23D) {  result = 3;}
        else if (results.value == 0xFF629D) { result = 4;}
        irrecv.resume();
        return result;
  }
  return -1;
}

void augmenterVitesse(int increment) {
    speedValue += increment;
    if (speedValue > 255) speedValue = 255; 
    analogWrite(motorPin, speedValue);
}

void diminuerVitesse(int decrement) {
    speedValue -= decrement;
    if (speedValue < 0) speedValue = 0;
    analogWrite(motorPin, speedValue);
}

void arreterMoteur() {
    speedValue = 0;
    analogWrite(motorPin, 0);
}

void loop() {
  int r;

  while ((r = getDirection()) <= 0) {delay(10);};
  switch (r) {
    case 1 :
      controlServo(0);
      break;
    case 2 :
      controlServo(90);
      arreterMoteur();
      break;
    case 3 :
      controlServo(180);
      break;
    case 4 :
      Serial.print(r);
      augmenterVitesse(255);
      break;
  }

}