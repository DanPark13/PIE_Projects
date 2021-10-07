#include <Adafruit_MotorShield.h>

const int rightSensor = A1;
const int leftSensor = A0;

int rightSensorValue = 0;
int leftSensorValue = 0;

int rightSensorCutoff = 930;
int leftSensorCutoff = 330;

int motorSpeed = 80;
int rightMotorSpeed = motorSpeed;
int leftMotorSpeed = motorSpeed*1.2;
int speedMultiplier = 0.8;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  rightMotor->setSpeed(150);
  rightMotor->run(BACKWARD);
  // turn on motor
  rightMotor->run(RELEASE);
  leftMotor->setSpeed(150);
  leftMotor->run(BACKWARD);
  // turn on motor
  leftMotor->run(RELEASE);
}


void loop() {
  // put your main code here, to run repeatedly:
  uint8_t i;
  uint16_t a,b,c,x,y,z,maxL,minL,maxR,minR;

  a = analogRead(rightSensor);
  x = analogRead(leftSensor);
  delay(100);
  b = analogRead(rightSensor);
  
  y = analogRead(leftSensor);

  rightSensorValue = abs(a-b);
  leftSensorValue = abs(x-y);
  
  Serial.print("right = ");
  Serial.print(rightSensorValue);
  Serial.print("\t left = ");
  Serial.println(leftSensorValue);
  
  rightMotor->run(BACKWARD);
  leftMotor->run(BACKWARD);
  
  if (rightSensorValue >= 5 && leftSensorValue < 5 ) {
    rightMotor->setSpeed(rightMotorSpeed);
    leftMotor->setSpeed(speedMultiplier*leftMotorSpeed);
  }
  else if (rightSensorValue < 5 && leftSensorValue >= 5) {
    leftMotor->setSpeed(leftMotorSpeed);
    rightMotor->setSpeed(speedMultiplier*rightMotorSpeed);
  }
  else if (rightSensorValue < 5 && leftSensorValue < 5){
    leftMotor->setSpeed(speedMultiplier*leftMotorSpeed);
    rightMotor->setSpeed(speedMultiplier*rightMotorSpeed);

  }
  
  
  
}
