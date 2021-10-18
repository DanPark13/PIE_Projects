#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

  int vSpeed = 110;        // MAX 255
  int turn_speed = 230;    // MAX 255 
  int turn_delay = 10;
  
//L293 Connection   
  const int motorA1      = 8;  
  const int motorA2      = 10; 
  const int motorAspeed  = 9;
  const int motorB1      = 12; 
  const int motorB2      = 13; 
  const int motorBspeed  = 11;


//Sensor Connection
  const int left_sensor_pin =A1;
  const int right_sensor_pin =A0;

  int right_sensor_off = 840;
  int left_sensor_off = 190;
  int right_sensor_on = 935;
  int left_sensor_on = 635;
  int buffer_val = 10;
  
  int left_sensor_state;
  int right_sensor_state;

  int forward_speed = 30;
  int turn_back_speed = 30;
  int turn_forward_speed = 70;

  int input;
  
void setup() {
//  pinMode(motorA1, OUTPUT);
//  pinMode(motorA2, OUTPUT);
//  pinMode(motorB1, OUTPUT);
//  pinMode(motorB2, OUTPUT);

  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  Serial.println("Motors Initialized");
  
  Serial.println("Choose Speed (fast(f) or slow(s))");
  delay(3000);
}

void loop() {
  if(Serial.available()){
    input = Serial.read();
    //Serial.println(input);
    if (input ==  49) { //readString
      Serial.println("You chose fast mode");
      forward_speed = 60;
      turn_back_speed = 40;
      turn_forward_speed = 160;
    }
    else if (input ==  50) {
      Serial.println("You chose slow mode");
      forward_speed = 30;
      turn_back_speed = 30;
      turn_forward_speed = 70;
    }
  }
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);
  //Serial.println(left_sensor_state);
  //Serial.println(right_sensor_state);
  
  // Set the speed to start, from 0 (off) to 100 (max speed)
  rightMotor->setSpeed(forward_speed);
  leftMotor->setSpeed(forward_speed);
  //rightMotor->run(BACKWARD);
  //leftMotor->run(BACKWARD);

  // If right sensor detects line
  if(left_sensor_state <= 200 && right_sensor_state > 200)
  {
    //Serial.println("turning right");
    //delay(100);

    leftMotor->setSpeed(turn_back_speed); //30
    rightMotor->setSpeed(turn_forward_speed); //70
    rightMotor->run(BACKWARD);
    leftMotor->run(FORWARD);
  
  }
  
  // If left sensor detects line
  else if(left_sensor_state > 200 && right_sensor_state <= 200)
  {
    //Serial.println("turning left");
    //delay(100);
    
    rightMotor->setSpeed(turn_back_speed);
    leftMotor->setSpeed(turn_forward_speed);
    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);
  
    //delay(turn_delay);
    }

  // If no sensor detects lines
  else if(left_sensor_state <= 200 && right_sensor_state <= 200)
  {
    //Serial.println("going forward");
    //delay(100);

    rightMotor->setSpeed(forward_speed);
    leftMotor->setSpeed(forward_speed);
    rightMotor->run(BACKWARD);
    leftMotor->run(BACKWARD);
  
    //delay(turn_delay);
    
    }
//  //If both detect
//  else if(left_sensor_state > 300 && right_sensor_state > 800)
//  { 
//    Serial.println("stop");
//    delay(100);
//    
//    rightMotor->run(RELEASE);
//    leftMotor->run(RELEASE);
//   }
}
