
int LED_PIN = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);
  Serial.println("ON");
  delay(300);
  digitalWrite(LED_PIN, LOW);
  Serial.println("OFF");
  delay(1000);
}
