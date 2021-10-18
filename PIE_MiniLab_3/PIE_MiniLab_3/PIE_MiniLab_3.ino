// Insert MotorShield Functionality to Connect Motors to the Arduino
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Assign the Right and Left Motor Objects to M3 and M4 on the MotorShield
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

// Assign Sensors to Anlog speed_control_inputs on the Arduino
const int right_sensor_pin = A0;
const int left_sensor_pin = A1;

// Set Sensor States for Activation/Deactivation
int left_sensor_state;
int right_sensor_state;

// Initial Speeds
int forward_speed = 30;
int turn_back_speed = 30;
int turn_forward_speed = 70;

// Get speed_control_input for Controlling Speeds
int speed_control_input;

// Initialization Code (Run Once)
void setup() {
  // Set up Serial library at 9600 bps
  Serial.begin(9600);

  // Test whether or not the Motorshield is detected
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {
  // if (!AFMS.begin(1000)) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  Serial.println("Motors Initialized");

  // Asks User to Choose between Fast and Slow Mode
  Serial.println("Choose Speed (fast(f) or slow(s))");
  delay(3000);
}

// Continuous Running 
void loop() {
  // When the Serial is Running, allow speed changes
  if(Serial.available()){
    // Read Input
    speed_control_input = Serial.read();
    // Fast Mode
    if (Serial.readString(speed_control_input) ==  "f") {
      Serial.println("You chose fast mode");
      forward_speed = 60;
      turn_back_speed = 40;
      turn_forward_speed = 160;
    }
    // Slow Mode
    else if (Serial.readString(speed_control_input) ==  "s") {
      Serial.println("You chose slow mode");
      forward_speed = 30;
      turn_back_speed = 30;
      turn_forward_speed = 70;
    }
  }
  // Get the state variables
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);
  
  // Set the speed to start, from 0 (off) to 100 (max speed)
  rightMotor->setSpeed(forward_speed);
  leftMotor->setSpeed(forward_speed);

  // If right sensor detects line, turn left
  if(left_sensor_state <= 200 && right_sensor_state > 200)
  {
    leftMotor->setSpeed(turn_back_speed); //30
    rightMotor->setSpeed(turn_forward_speed); //70
    rightMotor->run(BACKWARD);
    leftMotor->run(FORWARD);
  }
  
  // If left sensor detects line, turn right
  else if(left_sensor_state > 200 && right_sensor_state <= 200)
  {
    rightMotor->setSpeed(turn_back_speed);
    leftMotor->setSpeed(turn_forward_speed);
    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);
    }

  // If no sensor detects lines, keep moving forward
  else if(left_sensor_state <= 200 && right_sensor_state <= 200)
  {
    //Serial.println("going forward");
    //delay(100);

    rightMotor->setSpeed(forward_speed);
    leftMotor->setSpeed(forward_speed);
    rightMotor->run(BACKWARD);
    leftMotor->run(BACKWARD);
  }
  //If both sensors detect line, then stop
  else if(left_sensor_state > 200 && right_sensor_state > 200)
  {
    rightMotor->run(RELEASE);
    leftMotor->run(RELEASE);
  }
}
