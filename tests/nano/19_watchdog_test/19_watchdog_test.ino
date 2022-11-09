
#define CLOCK_PIN A0
#define BUTTON_PIN A1

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(CLOCK_PIN, OUTPUT);
}

int state = 0;

#define BUTTON_PRESSED 0

void loop() {

    int input = digitalRead(BUTTON_PIN);

    Serial.print("Button: ");
    Serial.print(input);

    // Serial.print("State: ");
    // Serial.print(state);

    Serial.println();

    if (input == !BUTTON_PRESSED) {

        digitalWrite(CLOCK_PIN, state);
    }

    state = !state;
}

