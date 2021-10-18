#include <Adafruit_MotorShield.h>

const int rightSensor = A1;
const int leftSensor = A0;

int rightSensorDiff = 0;
int leftSensorDiff = 0;

bool turnRight = false;
bool turnLeft = false;

int motorSpeed = 40;
int rightMotorSpeed = motorSpeed;
int leftMotorSpeed = motorSpeed*1.2;
int speedMultiplier = 0.8;

int right_sensor_floor_value = 0;
int left_sensor_floor_value = 0;
int sensor_buffer = 10;

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
  rightMotor->setSpeed(30);
  rightMotor->run(BACKWARD);
  // turn on motor
  // rightMotor->run(RELEASE);
  leftMotor->setSpeed(30);
  leftMotor->run(BACKWARD);
  // turn on motor
  // leftMotor->run(RELEASE);
  right_sensor_floor_value = analogRead(rightSensor);
  left_sensor_floor_value = analogRead(leftSensor);
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

  rightSensorDiff = abs(a-b);
  leftSensorDiff = abs(x-y);

  
  Serial.print("right = ");
  Serial.print(rightSensorDiff);
  Serial.print("\t left = ");
  Serial.print(leftSensorDiff);

  // If Right Sensor Detects Line
  if (rightSensorDiff >= 5 && leftSensorDiff < 5) {
    do {
      Serial.println("\t Turning Right");
      a = analogRead(rightSensor);
      leftMotor->setSpeed(leftMotorSpeed);
      rightMotor->setSpeed(rightMotorSpeed);
      rightMotor->run(BACKWARD);
      leftMotor->run(FORWARD);
    } while (a >= right_sensor_floor_value - sensor_buffer && a <= right_sensor_floor_value + sensor_buffer);
    right_sensor_floor_value = a;
  }
  // If Left Sensor Detects Line
  else if (leftSensorDiff >= 5 && rightSensorDiff < 5) {
    do {
      Serial.println("\t Turning Left");
      a = analogRead(leftSensor);
      leftMotor->setSpeed(leftMotorSpeed);
      rightMotor->setSpeed(rightMotorSpeed+50);
      rightMotor->run(FORWARD);
      leftMotor->run(BACKWARD);
    } while (a >= left_sensor_floor_value - sensor_buffer && a <= left_sensor_floor_value + sensor_buffer);
    left_sensor_floor_value = a;
  }
  // Else Keep Going
  else {
    Serial.println("\t Going Straight");
      leftMotor->setSpeed(leftMotorSpeed);
      rightMotor->setSpeed(rightMotorSpeed);
      rightMotor->run(BACKWARD);
      leftMotor->run(BACKWARD);
  }
    rightMotor->run(BACKWARD);
    leftMotor->run(BACKWARD);
  
  /*
  if (turnRight) {
    rightMotor->setSpeed(150);
    leftMotor->setSpeed(150);
  }
  
  else if (turnLeft) {
    leftMotor->setSpeed(leftMotorSpeed);
    rightMotor->setSpeed(speedMultiplier*rightMotorSpeed);
  }
  else {
    leftMotor->setSpeed(speedMultiplier*leftMotorSpeed);
    rightMotor->setSpeed(speedMultiplier*rightMotorSpeed);
  }
  */
 
  
  
  
}
