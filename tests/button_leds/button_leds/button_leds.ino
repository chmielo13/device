

const int BUTTON_PIN = 0;
const int LED_1_PIN = A0;
const int LED_2_PIN = A1;
const int LED_3_PIN = A2;

// const int RELAY_PIN_1 = 3;
// const int RELAY_PIN_2 = 4;
// const int RELAY_PIN_3 = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);


  // pinMode(RELAY_PIN_1, OUTPUT);
  // pinMode(RELAY_PIN_2, OUTPUT);
  // pinMode(RELAY_PIN_3, OUTPUT);

  digitalWrite(LED_1_PIN, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:

  int result = digitalRead(BUTTON_PIN);
  // digitalWrite(RELAY_PIN_1, !result);
  Serial.println(result * 2);

}
