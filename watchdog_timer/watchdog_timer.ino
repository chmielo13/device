#define EXTERNAL_WATCHDOG_RESET_REQUEST_PIN 1 // ACTIVE HIGH.
#define DIRECT_INPUT_PIN 2
#define RESET_OUT_L 3 // ACTIVE LOW.
#define RESET_OUT_H 4 // Active HIGH.

#define PERIOD_BETWEEN_ALLOWABLE_RESETS_MILLIS 5000 // How long it may take for the system to fully reset.
#define RESET_ASSERTED_PERIOD_MILLIS 300

#define MINIMUM_ALLOWED_HAZARD_STATE_PERIOD_MILLIS 5000  // How long an RS232 input data transfer may take.
#define MINIMUM_WATCHDOG_RESET_REQUEST_PERIOD_SUSTAINED_HAZARD_STATE_MILLIS 1500 // How long between reset requests from watchdog shall a sustained hazard state be noted. 

void setup() {

    // Make sure reset_H is never asserted HIGH, even during bootup. Requires external pull down resistor.
    digitalWrite(RESET_OUT_H, LOW);
    pinMode(RESET_OUT_H, INPUT);
    pinMode(RESET_OUT_H, OUTPUT);

    // Make sure reset_L is never asserted LOW, even during bootup.
    digitalWrite(RESET_OUT_L, HIGH);
    pinMode(RESET_OUT_L, INPUT_PULLUP);
    pinMode(RESET_OUT_L, OUTPUT);

    pinMode(EXTERNAL_WATCHDOG_RESET_REQUEST_PIN, INPUT);
    pinMode(DIRECT_INPUT_PIN, INPUT);
    
    deassert_reset();

    Serial.begin(9600);

    // Due to the OLED screen not always powering on successfully (and needing to manually hit the Arduino reset button)
    delay(100);
    trigger_reset();

    // Allow first startup to take some number of seconds.
    delay(PERIOD_BETWEEN_ALLOWABLE_RESETS_MILLIS);
}

unsigned long last_reset_time = 0;


int read_reset_requested() {
    int result = digitalRead(EXTERNAL_WATCHDOG_RESET_REQUEST_PIN);
    return result == HIGH;
}

void assert_reset() {
    digitalWrite(RESET_OUT_L, LOW);
    digitalWrite(RESET_OUT_H, HIGH);
}

void deassert_reset() {
    digitalWrite(RESET_OUT_L, HIGH);
    digitalWrite(RESET_OUT_H, LOW);
}

void trigger_reset() {
    assert_reset();
    delay(RESET_ASSERTED_PERIOD_MILLIS);
    deassert_reset();
    delay(PERIOD_BETWEEN_ALLOWABLE_RESETS_MILLIS);
}

unsigned long hazard_state_begin_time = 0;
unsigned long last_indicated_hazard_state_time = 0;
int hazard_state_enabled = 0;

void loop() {
    unsigned long current_time = millis();
    int reset_requested = read_reset_requested();

    if (reset_requested) {
        // Hazard state indicated by watchdog.
        if (!hazard_state_enabled) {
            hazard_state_enabled = 1;
            hazard_state_begin_time = current_time;
        }
        last_indicated_hazard_state_time = current_time;
    } else {
        // Hazard state not indicated by watchdog.
        if (hazard_state_enabled) {
            // Check if hazard state needs to be disabled.
            if (current_time - last_indicated_hazard_state_time > MINIMUM_WATCHDOG_RESET_REQUEST_PERIOD_SUSTAINED_HAZARD_STATE_MILLIS) {
                hazard_state_enabled = false;
            }
        }
    }

    

    Serial.print("CurrTime: ");
    Serial.print(current_time);

    Serial.print(" -- ");

    Serial.print("RE: ");
    Serial.print(reset_requested);

    Serial.print(" -- ");

    Serial.print("HZE: ");
    Serial.print(hazard_state_enabled);

    Serial.print(" -- ");

    Serial.print("HZBTime: ");
    Serial.print(hazard_state_begin_time);

    
    Serial.println();

    if (hazard_state_enabled) {
        if (current_time - hazard_state_begin_time > MINIMUM_ALLOWED_HAZARD_STATE_PERIOD_MILLIS) {
            trigger_reset();
        }
    }


    // Serial.print("C: ");
    // Serial.print(current_time);
    // Serial.print("; E: ");
    // Serial.print(current_time - last_reset_time);
    // Serial.print("; R: ");
    // Serial.print(reset_requested());
    // Serial.print(";    ");
    // if ((current_time - last_reset_time) <= RESET_ASSERTED_PERIOD_MILLIS) {
    //     // Should be in reset period.
    //     Serial.print("(current_time - last_reset_time) <= RESET_ASSERTED_PERIOD_MILLIS");
    //     assert_reset();
    // } else if ((current_time - last_reset_time) <= PERIOD_BETWEEN_ALLOWABLE_RESETS_MILLIS) {
    //     // Reset shall not be asserted.
    //     Serial.print("(current_time - last_reset_time) <= PERIOD_BETWEEN_ALLOWABLE_RESETS_MILLIS");
    //     deassert_reset();
    // } else {
    //     // A reset request is allowed.
    //     Serial.print("ELSE");
    //     deassert_reset();
    //     if (reset_requested()) {
    //         last_reset_time = current_time;
    //     }
    // }
    // Serial.println();
}
