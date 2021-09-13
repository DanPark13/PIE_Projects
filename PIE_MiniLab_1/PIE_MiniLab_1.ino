/*
 * Define Button and LEDs on Arduino
 */
#define BUTTON 3
#define RED_LED 5
#define YELLOW_LED 7
#define BLUE_LED 9

/*
 * Define elements for Thermal
 */
int pot_pin = A0;
int output;
int led_value;

/*
 * current_state variables
 */
 int current_state = 0; 
 int past_state = 0;
 int button_state = 0;

void setup() {
  /*
   * Button as input
   * LEDs as output
   */
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Serial.println("Setup Completed");

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  Serial.println("All Power Off");
}

void loop() {
  // read the analog state for the thermal sensor
  output = analogRead(pot_pin);
  button_state = digitalRead(BUTTON);
  if(button_state == 0){
    delay(50);
    button_state = digitalRead(BUTTON);
    if(button_state == 1){
      current_state = past_state + 1;
      Serial.println("Pressed");
      Serial.println(button_state);
      Serial.println(current_state);
      }
    }
    else{
      delay(100);
    }
  // put your main code here, to run repeatedly:
  switch(current_state){
    case 1:
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      past_state = current_state;
      Serial.println(current_state);
      break;
    case 2:
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      delay(500);
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(500);
      past_state = current_state;
      Serial.println(current_state);
      break;
    case 3:
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      delay(100);
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(100);
      past_state = current_state;
      Serial.println(current_state);
      break;
    case 4:
      digitalWrite(RED_LED, HIGH);
      delay(250);
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      delay(250);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      delay(250);
      digitalWrite(BLUE_LED, LOW);
      past_state = current_state;
      Serial.println(current_state);
      break;
    default:
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      past_state = 0;
      Serial.println(past_state);
    break;
  }
}
