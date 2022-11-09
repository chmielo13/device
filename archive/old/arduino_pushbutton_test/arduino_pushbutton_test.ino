


int BUTTON_1_PIN = 8;
int BUTTON_2_PIN = 9;

int LED_PIN = 10;



int STATE_1 = 1;
int STATE_2 = 2;
int STATE_3 = 3;

int state = STATE_1;


void setup() {

  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // Use Arduino input pull up resister feature. Pulls up to HIGH (5V).
  // Therefore, other end of button terminal is connected to Arduino GND,
  // And we detect a push button when the PIN reads LOW.
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
}



void loop() {
  
  adjust_led();
  
  int button_1_down = (digitalRead(BUTTON_1_PIN) == LOW);
  int button_2_down = (digitalRead(BUTTON_2_PIN) == LOW);


  if (button_1_down) {
    state = STATE_1;
  }

  
  if (button_2_down) {
    state = STATE_2;
  }
  
  
  
}


int print_counter = 100;
void p(String message) {
  Serial.println(String(print_counter++) + ": " + message);
}




int led_blink_count = 0;

unsigned long led_toggle_time = millis();
int led_is_on = 0;
int led_is_paused = 0;

unsigned long led_blink_on_duration_millis = 300;
unsigned long led_blink_off_duration_millis = 300;

unsigned long pause_between_blink_groups_millis = 1000;

void adjust_led() {

  p("Adjusting LED");

  unsigned long time_since_toggle = millis() - led_toggle_time;

  if (led_is_on) {
    if (time_since_toggle > led_blink_on_duration_millis) {
      digitalWrite(LED_PIN, LOW);
      led_is_on = 0;
      led_toggle_time = millis();
      led_blink_count++;
      if (led_blink_count >= blink_group_number_for_state(state)) {
        led_is_paused = 1;
        led_blink_count = 0;
      }
    }
  } else {
    // LED is Off.
    if (led_is_paused) {

      if (time_since_toggle > pause_between_blink_groups_millis) {
        led_is_paused = 0;
        digitalWrite(LED_PIN, HIGH);
        led_is_on = 1;
        led_toggle_time = millis();
      }
      
    } else {
      
      if (time_since_toggle > led_blink_off_duration_millis) {
        digitalWrite(LED_PIN, HIGH);
        led_is_on = 1;
        led_toggle_time = millis();
      }
    
    }
  }
  
  
}



int blink_group_number_for_state(int given_state) {
  // For now, STATE_1 = 1, STATE_2 = 2, etc...
  return given_state;
}
