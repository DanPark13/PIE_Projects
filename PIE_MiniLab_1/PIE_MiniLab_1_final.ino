// Define debounce wait time
#define MS_STATE_CHANGE_DELAY 50

// Define pin assignments
#define BUTTON 2
#define LED1 11
#define LED2 10
#define LED3 9
#define LED4 6
#define LED5 5
#define DIST_SENSOR A0

// Define LED#State ON & OFF
#define ON 1
#define OFF 0

// Initialization for reading analog values and output
uint32_t output;
uint32_t ledValue;

// Initialize vairables to keep track of button state
volatile bool changeState = false;
volatile bool buttonState = false;

// Initialize variables for keeping track of bike light state and time
uint32_t state = 0;
uint32_t t = millis();
uint32_t lastStateTime = t;
uint32_t lastBlinkTime = millis();

// Variables for ping pong simulation
uint8_t pongCounter = 1;
int8_t direction = 1;

// Variables to keep track of each LED state
uint8_t LED1State = OFF;
uint8_t LED2State = OFF;
uint8_t LED3State = OFF;
uint8_t LED4State = OFF;
uint8_t LED5State = OFF;

void LEDChange() {
  /*
   * If the LED#State variable is 1, set it to zero and vice-versa. This
   * applies to all of the LED#State variables.
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */
  LED1State = abs(LED1State - 1);
  LED2State = abs(LED2State - 1);
  LED3State = abs(LED3State - 1);
  LED4State = abs(LED4State - 1);
  LED5State = abs(LED5State - 1);
}

void LEDOff() {
  /*
   * Set all the LED#State variables to 0.
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */
  LED1State = OFF;
  LED2State = OFF;
  LED3State = OFF;
  LED4State = OFF;
  LED5State = OFF;
}

void LEDOn() {
  /*
   * Set all the LED#State variables to 1.
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */
  LED1State = ON;
  LED2State = ON;
  LED3State = ON;
  LED4State = ON;
  LED5State = ON;
}

void LEDWrite() {
  /*
   * Set the pins connected to LEDs to output a PWM signal derived from the
   * analog input ofvof the infrared distance sensor if the corresponding
   * LED#State variable is set to 1.
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */
  analogWrite(LED1, ledValue * LED1State);
  analogWrite(LED2, ledValue * LED2State);
  analogWrite(LED3, ledValue * LED3State);
  analogWrite(LED4, ledValue * LED4State);
  analogWrite(LED5, ledValue * LED5State);
}


bool checkTime(uint32_t t, uint32_t t0, uint16_t dt) {
  /*
   * Determine if a certain amount of time (milliseconds) has passed.
   * 
   * Parameters:
   *   t (uint32_t): current time (output of millis()).
   *   t0 (uint32_t): past time to use as reference.
   *   dt (uint16_t): minimum difference in time.
   * Returns:
   *   boolean: true if the difference between the current time and
   *     and past time is greater than or equal to the specified
   *     minimum difference. Otherwise false.
   */
  return ((t >= t0) && (t - t0 >= dt)) || ((t < t0) && (t + (~t0) + 1 >= dt));
}
  
void setup() {
  /*
   * This void setup starts by creating an ISR and assigning the pinModes an
   * LED and that it is an OUTPUT, it then sets all the LEDs to LOW.
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), incrementState, CHANGE);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  LEDWrite();
  Serial.println("All Power Off");
}


void loop(){
  /*
   * The void loop contains a switch case that lookes at what number (from 0-4) the
   * variable 'state' is in to determine what to display using the LEDs.
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */

  // Read the analog pin
  output = analogRead(DIST_SENSOR);

  // Get the led value associated
  ledValue = map(output, 0, 1023, 0, 255);

  // Write values stored in LED#State to digital pins wired to LEDs
  LEDWrite();

  // Get current time (milliseconds) since program start
  t = millis();

  if (changeState) {
    buttonState = !buttonState;
    if (buttonState) {
      state += 1;
    }

    // Print statements for debugging
    // Serial.print(state);
    // Serial.print(buttonState);
    // Serial.println(digitalRead(BUTTON));
    
    changeState = false;
    lastStateTime = t;
    LEDOff();
  }

  switch(state) {

    // Case 0 is all the LEDs on 
    case 0:
      LEDOff();
    break;

    // Case 1 is all the LEDs off
    case 1:
      LEDOn();
    break;

    // Case 2 is an LED ping pong simulation that repeats
    case 2:
      if (checkTime(t,lastBlinkTime,100)){
        lastBlinkTime = t;
        LEDOff();
        pongCounter += direction;

        switch(pongCounter % 6) {
          case 1:
            LED1State = ON;
            direction *= -1;
          break;

          case 2:
            LED2State = ON;
          break;

          case 3:
            LED3State = ON;
          break;

          case 4:
            LED4State = ON;
          break;

          case 5:
            LED5State = ON;
            direction *= -1;
          break;

          default:
            direction = 1;
            pongCounter = 1;
          break;
        }
      }
    break;

    // Case 3 is a slow blink of the LEDs
    case 3:
      if (checkTime(t, lastBlinkTime, 100)){
        lastBlinkTime = t;
        LEDChange();
      }
    break;

    // Case 4 is a fast blink of the LEDs
    case 4:
      if (checkTime(t, lastBlinkTime, 50)){
        lastBlinkTime = t;
        LEDChange();
      }
    break;

    default:
      state = 0;
    break;
  }
}

void incrementState() {
  /*
   * Sets the global variable, changeState, to be true if it has not been set
   * within the past few milliseconds (as specified by MS_STATE_CHANGE_DELAY).
   * 
   * Parameters:
   *   none
   * Returns:
   *   void
   */
  if (checkTime(t, lastStateTime, MS_STATE_CHANGE_DELAY)) {
    changeState = true;
  }
}
