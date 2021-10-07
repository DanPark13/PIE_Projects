/*
 * Define Button and LEDs on Arduino
 */
#define BUTTON 3
#define RED_LED 5
#define YELLOW_LED 6
#define BLUE_LED 9

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
 
  analogWrite(RED_LED, LOW);
  analogWrite(YELLOW_LED, LOW);
  analogWrite(BLUE_LED, LOW);
  Serial.println("All Power Off");
}
 
void loop() {
  output = analogRead(pot_pin);
  led_value = map(output, 0, 1023, 0, 255);

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
      analogWrite(RED_LED, led_value);
      analogWrite(YELLOW_LED, led_value);
      analogWrite(BLUE_LED, led_value);
      past_state = current_state;
      Serial.println(current_state);
      break;
    case 2:
      analogWrite(RED_LED, led_value);
      analogWrite(YELLOW_LED, led_value);
      analogWrite(BLUE_LED, led_value);
      delay(500);
      analogWrite(RED_LED, LOW);
      analogWrite(YELLOW_LED, LOW);
      analogWrite(BLUE_LED, LOW);
      delay(500);
      past_state = current_state;
      Serial.println(current_state);
      break;
    case 3:
      analogWrite(RED_LED, led_value);
      analogWrite(YELLOW_LED, led_value);
      analogWrite(BLUE_LED, led_value);
      delay(100);
      analogWrite(RED_LED, LOW);
      analogWrite(YELLOW_LED, LOW);
      analogWrite(BLUE_LED, LOW);
      delay(100);
      past_state = current_state;
      Serial.println(current_state);
      break;
    case 4:
      analogWrite(RED_LED, led_value);
      delay(250);
      analogWrite(RED_LED, LOW);
      analogWrite(YELLOW_LED, led_value);
      delay(250);
      analogWrite(YELLOW_LED, LOW);
      analogWrite(BLUE_LED, led_value);
      delay(250);
      analogWrite(BLUE_LED, LOW);
      past_state = current_state;
      Serial.println(current_state);
      break;
    default:
      analogWrite(RED_LED, LOW);
      analogWrite(YELLOW_LED, LOW);
      analogWrite(BLUE_LED, LOW);
      past_state = 0;
      Serial.println(past_state);
    break;
  }
}
