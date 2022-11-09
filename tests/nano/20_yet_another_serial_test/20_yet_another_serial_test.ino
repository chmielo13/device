
void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    delay(2000);
    Serial.println("Beginning my custom my serial connection! B");
    Serial1.println("Beginning my custom my serial connection! B");
}

void loop() {
    // Serial.println("Yet another serial test.");
    // delay(1000);
    if (Serial1.available()) {
        Serial.print("Received: ");
        Serial.write(Serial1.read());
        Serial.println();
    }
    if (Serial.available()) {
        int val = Serial.read();
        // Serial.print("Writing: ");
        // Serial.write(val);
        // Serial.println();
        
        Serial1.write(val);
    }
    // Serial1.println("Mytest");
}

