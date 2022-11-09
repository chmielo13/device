void setup() {
    Serial.begin(9600);
    pinMode(A0, OUTPUT);
}

int state = 0;


void loop() {
    state = !state;
    digitalWrite(A0, state);
    Serial.print("Mosfet Testb!! ");
    Serial.print(state);
    Serial.println();
    delay(2500);


}