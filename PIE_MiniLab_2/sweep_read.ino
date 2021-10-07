/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

#include <Servo.h>

Servo horizontalservo;
Servo verticalservo;
int pos = 0;
int vertical_pos = 10;
int pos_change = 1;
long previousMillis = 0;
long interval = 5; // turn servo by one degree every 10th of second (less is faster)

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // attach the servos to ports in Arduino
  horizontalservo.attach(6);
  verticalservo.attach(9);
  // set both servos to default position
  horizontalservo.write(0);
  verticalservo.write(0);
}

// the loop routine runs over and over again forever:
void loop() {
  // Get the milliseconds passed to be track time instead of using delay
  unsigned long currentMillis = millis();
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // check every hundredth of a second
  if(currentMillis - previousMillis > interval) {
    // if the horizontal position has reached the max motion
    // move the horizontal position back to zero degrees
    if(pos >= 180){
      horizontalservo.write(0);
      verticalservo.write(vertical_pos+=5);
      delay(500);
      pos = 0;
    }
    // when the horizontal position goes back to zero
    // move the horizontal servo motion back to forward
    // and move the vertical servo forward
    else if(pos == 0){
      pos_change = 1;
    }
    // Move the horizontal position
    pos += pos_change;
    horizontalservo.write(pos);
    // End program when vertical servo has completed full range of motion
    if(vertical_pos >= 80){
      exit(1);
    }
    double every_five_digits[5];
    for(int i=0;i<5;i++){
      every_five_digits[i]=sensorToDistanceConversion(analogRead(A0));
      delay(5);
    }
    sort(every_five_digits, 5);
    // Print out serial output
    String out = (String)pos + "," + vertical_pos;
//    for (const double reading : every_five_digits) {
//      out += ",";
//      out += reading;
//    }
    out+=(String)"," + every_five_digits[2]+",";
    Serial.println(out);
    // Reset the time
    previousMillis = currentMillis;
  }
}

double sensorToDistanceConversion(double sensorReading){
  return -1.354e-05*pow(sensorReading,3)+0.01421*pow(sensorReading,2)-4.839*sensorReading+568.6;
}

void sort(double a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    double t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}
