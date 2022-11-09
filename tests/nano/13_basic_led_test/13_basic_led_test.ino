
#define LED_PIN A0

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
}

int state = 0;

void loop() {

    state = !state;

    digitalWrite(LED_PIN, state);

    Serial.print("Basic LED Test");
    Serial.print(state);
    Serial.println();
    
    
    
    delay(1000);
}
