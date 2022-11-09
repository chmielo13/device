
const int LED_PIN = A0;

void setup() {
    pinMode(LED_PIN, OUTPUT)
}

void loop() {
    Serial.println("Hello from Nano!!");
    
    digitalWrite(LED_PIN, 0);
    delay(1000);
    digitalWrite(LED_PIN, 1);
    delay(1000);



}

