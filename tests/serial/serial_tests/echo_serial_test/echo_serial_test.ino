
void setup() {
    Serial.begin(9600);
    Serial.println("Awaiting...");
}


int counter = 0;
String latest;

void loop() {
    // counter += 1;
    // if (counter % 100000 == 0) {
    //     Serial.println("Hello");
    // }

    if (Serial.available()) {
        latest = Serial.readString();
        Serial.print("You sent: ");
        Serial.println(latest);
    }
}

