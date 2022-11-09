#include "lib/LLAppState.cpp"
#include "lib/nano_timer_interrupt.cpp"
#include "lib/LLBle.cpp"
#include "lib/LLDateTime.cpp"
#include "lib/LLDisplay.cpp"
#include "lib/LLConfig.cpp"
#include <MemoryFree.h>;

#define SHOW_SPLASH_LOGO 0

// 1000000:    1 ms
//  100000:   10 ms
//   10000:  102 ms
//    1000: 1024 ms
const int sampleRate = 100000; //sample rate in milliseconds, determines how often TC5_Handler is called

const int BUTTON_PIN = 2;
const int INTERACTIVE_LED_PIN     = A0;     // Was used to button detection handling in timer interupt
const int DELAY_BLINKING_LED_PIN  = A1;     // Was used to demonstrate a utility loop 50ms blink cycle. Now used to demonstrate active exhaust signal. 
const int RED_LED_PIN = A2;                 // Now currently used to demonstrate active inflate signal.

const int RELAY_1_PIN = 3;
const int RELAY_2_PIN = 4;
const int RELAY_3_PIN = 5;
const int RELAY_4_PIN = 6;
const int ACCOUSTIC_TRIGGER_PIN = 7;

const int PRESSURE_SENSOR_ANALOG_PIN = A7;
const int WATCHDOG_OUTPUT_PIN = A3;


const int ENERGIZE_SIGNAL = HIGH;
const int DEENERGIZE_SIGNAL = LOW;



unsigned long lastInterrupt = 0;

/*

TODO: Add another bluetooth characteristic, "sync time". Make the mobile app write to sync time on each connection.

*/


void transitionWatchdogOutput() {
    // return;
    digitalWrite(WATCHDOG_OUTPUT_PIN, LOW);
    digitalWrite(WATCHDOG_OUTPUT_PIN, HIGH);
    // Delay 20 ns?

    
    // Serial.println("Transition!");
    digitalWrite(WATCHDOG_OUTPUT_PIN, LOW);
}


void onTimerInterrupt() {

    // // if (digitalRead(BUTTON_PIN) == 0) { // || ble_led_assert_on) {
    // if (digitalRead(BUTTON_PIN) == 0 || ble_led_assert_on) {
    //     digitalWrite(INTERACTIVE_LED_PIN, HIGH);
    //     if (appState.getPendingIncrement()) {
    //         appState.setHelloIncrement(appState.getHelloIncrement() + 1);
    //         appState.setPendingIncrement(0);
    //         // appState.setTriggerTime(LLDateTime::now());
    //         Serial.println(appState.getHelloIncrement());
            
    //     }
    // } else {
    //     digitalWrite(INTERACTIVE_LED_PIN, LOW);
    //     appState.setPendingIncrement(1);
    // }

    unsigned long currentTime = millis();
    int elapsed = currentTime - lastInterrupt;
    lastInterrupt = currentTime;

    // // To read the time elapsed (period) of the interrupt routine.
    // Serial.println(elapsed);

    
    transitionWatchdogOutput();


    
}

LLDateTime::LLTime now;
LLDateTime::LLTime last_log_output_time;

LLDateTime::LLTime exhaust_until_time;


void setup() {
    
    // setupState();

    Serial.begin(9600);
    // delay(2000); // Wait for Serial to initialize.
    Serial.println("Serial output initialized.");

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(INTERACTIVE_LED_PIN,     OUTPUT);
    pinMode(DELAY_BLINKING_LED_PIN,  OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    pinMode(ACCOUSTIC_TRIGGER_PIN, INPUT);

    pinMode(RELAY_1_PIN, OUTPUT);
    pinMode(RELAY_2_PIN, OUTPUT);
    pinMode(RELAY_3_PIN, OUTPUT);
    pinMode(RELAY_4_PIN, OUTPUT);

    // Initially turn the relays off.
    digitalWrite(RELAY_1_PIN, DEENERGIZE_SIGNAL);
    digitalWrite(RELAY_2_PIN, DEENERGIZE_SIGNAL);
    digitalWrite(RELAY_3_PIN, DEENERGIZE_SIGNAL);
    digitalWrite(RELAY_4_PIN, DEENERGIZE_SIGNAL);

    pinMode(PRESSURE_SENSOR_ANALOG_PIN, INPUT);

    pinMode(WATCHDOG_OUTPUT_PIN, OUTPUT);
    digitalWrite(WATCHDOG_OUTPUT_PIN, LOW);

    // Timer interrupt disabled for now...
    // timer_interrupt_setup(sampleRate, &onTimerInterrupt);

    // transitionWatchdogOutput();
    LLConfig::setup();
    setup_app_state_from_config();

    LLDisplay::setup(SHOW_SPLASH_LOGO);

    // Initial display parameters.
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);


    // transitionWatchdogOutput();    
    ble_setup();

    // transitionWatchdogOutput();
    LLDateTime::setup();

    appState.setTimeReady(true);

    // timer_interrupt_setup(sampleRate, &onTimerInterrupt);

    
    // Note: Cannot access any LLDateTime routines (that depend on rtc time), until LLDateTime::setup(); is called!
    now = LLDateTime::now();
    last_log_output_time = LLDateTime::now();

    // timer_interrupt_setup(sampleRate, &onTimerInterrupt);

    
    // transitionWatchdogOutput();
    
    // Accoustics
    Serial1.begin(9600);
    Serial1.println("Beginning my custom my serial connection!");

    
    // transitionWatchdogOutput();

    utility_setup();



}

/*
 * After reading the configuration settings from the MicroSD card,
 * update the relevant application state values.
 */
void setup_app_state_from_config() {
    if (LLConfig::config_setup_successful) {
        appState.setModemType(LLConfig::CONFIG_MODEM_TYPE);
    }
}

void energize_relay_1() {
    digitalWrite(RELAY_1_PIN, ENERGIZE_SIGNAL);
}

void deenergize_rely_1() {
    digitalWrite(RELAY_1_PIN, DEENERGIZE_SIGNAL);
}

void energize_exhaust_relay() {
    digitalWrite(RELAY_2_PIN, ENERGIZE_SIGNAL);
}

void deenergize_exhaust_relay() {
    digitalWrite(RELAY_2_PIN, DEENERGIZE_SIGNAL);
}


float read_pressure() {
    // Serial.println("Hello from 15 pressure sensor test!");
    int rawAnalogInput = analogRead(PRESSURE_SENSOR_ANALOG_PIN);
    int maxAnalogCount = 1023;

    // [0 to 1023] 


    // An 80% bandwidth corresponds to 30 PSI of value.
    // 100% bandwidth corresponds to 30PSI/0.8 = 37.5 PSI.
    // One analog count = 1 / 1024 * 37.5

    float valueOfOneAnalogCount = 37.5 / 1024;

    int relativeAnalogInput = rawAnalogInput - 512;
    float relativePressurePSI = relativeAnalogInput * valueOfOneAnalogCount;

    // Serial.print("Pressure: ");
    // Serial.print(relativePressurePSI);
    // Serial.print(" (PSI).    ");
    // Serial.print("Raw Analog Input: ");
    // Serial.print(rawAnalogInput);

    return relativePressurePSI;

}



void loop() {

    now = LLDateTime::now();


    // State output differential.
    int timeBetweenStateOutputs = 1000; // Nominally, 7000.
    if (timeDifferenceMillis(now, last_log_output_time) >= timeBetweenStateOutputs) {
        printAppState();
        last_log_output_time = now;
    }

    utility_loop();
    ble_loop();
    LLDisplay::refreshDisplay();
}

void utility_setup() {
    appState.setTriggerTime(LLDateTime::now(-1 * (1 * 60 * 60 * 1000)));
}

#define NUMBER_PRESSURE_MEASUREMENTS 20

float pressureMeasurements[NUMBER_PRESSURE_MEASUREMENTS];
int pressureMeasurementIndex = 0;

float arraySumFloat(float * arr, int len) {
    float result = 0;
    for (int i = 0; i < len; i++ ) {
        result += arr[i];
    }
    return result;
}

float measurePressure() {
    float rawPressure = read_pressure();
    pressureMeasurements[pressureMeasurementIndex] = rawPressure;
    pressureMeasurementIndex = (pressureMeasurementIndex + 1) % NUMBER_PRESSURE_MEASUREMENTS;
    return rawPressure;
}

float averagePressure() {
    float pressureSum = arraySumFloat(pressureMeasurements, NUMBER_PRESSURE_MEASUREMENTS);
    return pressureSum / NUMBER_PRESSURE_MEASUREMENTS;
}

float pressureThresholdReached() {
    float threshold = 5.0;
    float pressureSum = arraySumFloat(pressureMeasurements, NUMBER_PRESSURE_MEASUREMENTS);
    float pressureSumThreshold = threshold * NUMBER_PRESSURE_MEASUREMENTS;
    return pressureSum > pressureSumThreshold;
}

void trigger_release() {
    now = LLDateTime::now(); 
    appState.setTriggerTime(now);
}

String accousticsMessage;

LLDateTime::LLTime lastOSDParamTransition;
LLDateTime::LLTime lastSubseaTrigger;

void utility_loop() {    

    // digitalWrite(DELAY_BLINKING_LED_PIN, HIGH);
    // delay(50);

    // digitalWrite(DELAY_BLINKING_LED_PIN, LOW);
    // delay(50);

    // transitionWatchdogOutput();

    LLDateTime::LLTime now = LLDateTime::now();

    if (digitalRead(ACCOUSTIC_TRIGGER_PIN) == HIGH) {
        digitalWrite(INTERACTIVE_LED_PIN, HIGH);
    } else {
        digitalWrite(INTERACTIVE_LED_PIN, LOW);
    }

    appState.setFreeMemoryRemaining(freeMemory());

    // Check if inflation should occur.
    int64_t trigger_time_elapsed = LLDateTime::timeDifferenceMillis(now, appState.getTriggerTime()); 
    // Serial.println(trigger_time_elapsed);

    char * computed_valve_state = VALVE_STATE_HOLD;

    if (trigger_time_elapsed < 0) {
        // Trigger time is in the future. Ignore.
        // appState.setValveState(VALVE_STATE_HOLD);
        computed_valve_state = VALVE_STATE_HOLD;

    } else {
        // Trigger time is in the past. Let's see if the relay should be activated.
        if (trigger_time_elapsed <= appState.getInflationDuration() * 1000) {
            // Activate relay.
            // appState.setValveState(VALVE_STATE_INFLATE);
            computed_valve_state = VALVE_STATE_INFLATE;
        } else {
            // Trigger time expired.
            // appState.setValveState(VALVE_STATE_HOLD);
            computed_valve_state = VALVE_STATE_HOLD;
        }
    }

    // transitionWatchdogOutput();

    // Button PIN can override Valve state, if depressed.
    int buttonPinResult = digitalRead(BUTTON_PIN);
    if (buttonPinResult == LOW) {
        computed_valve_state = VALVE_STATE_INFLATE;
    }

    appState.setValveState(computed_valve_state);
    
    // Side-effects based on valve state.
    if (appState.getValveState() == VALVE_STATE_INFLATE) {
        energize_relay_1();
        digitalWrite(RED_LED_PIN, HIGH);
    } else {
        deenergize_rely_1();
        digitalWrite(RED_LED_PIN, LOW);
    }

    if (appState.getValveState() != VALVE_STATE_INFLATE) {    

        // Pressure Sensors
        float rawPressure = measurePressure();
        float avgPressure = averagePressure();
        appState.setAveragePressure(avgPressure);

        // Serial.print("Raw Pressure: ");
        // Serial.print(rawPressure);
        // Serial.print(", Average Pressure: ");
        // Serial.print(avgPressure);
        // Serial.println();

        if (avgPressure < -5.0) {
            exhaust_until_time = LLDateTime::now(500);
        }

        int64_t exhaust_seconds_left = LLDateTime::timeDifferenceMillis(exhaust_until_time, LLDateTime::now()); 
        if (exhaust_seconds_left > 0) {
            energize_exhaust_relay();
            digitalWrite(DELAY_BLINKING_LED_PIN, HIGH);
        } else {
            deenergize_exhaust_relay();
            digitalWrite(DELAY_BLINKING_LED_PIN, LOW);
        }
    }

    
    // transitionWatchdogOutput();

    // Accoustics
    if (Serial1.available()) {
        accousticsMessage = Serial1.readStringUntil('\n');

        // Arduino 1.0 and later, String.trim() trims string in place.
        accousticsMessage.trim();

        // mySerialConnection.print("Received message: ");
        // mySerialConnection.println(message);

        // Serial.print("Received accoustics message: ");
        // Serial.println(accousticsMessage);

        // Serial1.print("Rec. acc. mess.: ");
        // Serial1.println(accousticsMessage);
        

        // Serial.print("Received accoustics message Length: ");
        // Serial.println(accousticsMessage.length());
        if (accousticsMessage == "RELEASE") {

            char current_time_string[LLDateTime::MAX_TIME_BUFFER_LENGTH];
            char trigger_time_string[LLDateTime::MAX_TIME_BUFFER_LENGTH];



            Serial1.println("Release triggered accoustically!!");
            Serial.println("Release triggered accoustically!!");
            // LLTimeDate::mutate_to_now(trigger_datetime);
            // updateInflationEndTime();
            LLDateTime::LLTime now;
            LLDateTime::LLTime triggerTime;

            now = LLDateTime::now(); 
            triggerTime = appState.getTriggerTime();

            trigger_release();

            LLDateTime::shortString(now, current_time_string);
            LLDateTime::shortString(triggerTime, trigger_time_string);


            Serial1.print("Curr  Time: ");
            Serial1.println(current_time_string);
            Serial1.print("Trig Start: ");
            Serial1.println(trigger_time_string);
            Serial1.print("Inflate Dur: ");
            Serial1.println(appState.getInflationDuration());
            
        } else if (accousticsMessage == "ACK") {
            Serial1.println("ACK. BY LL DEVICE");
            Serial.println("ACK. BY LL DEVICE");

        } else {
            // Serial1.println("Acc. command not recognized.");
            // Serial.println("Accoustic command not recognized.");
        }
    }

    // Subsea Sonics Trigger Detection:
    if (appState.getModemType() == LLConfig::MODEM_TYPE_SUB_SEA_SONICS) {
        if (digitalRead(ACCOUSTIC_TRIGGER_PIN) == HIGH) {
            if (LLDateTime::isNull(lastSubseaTrigger) || LLDateTime::timeDifferenceMillis(now, lastSubseaTrigger) > 10000) {
                trigger_release();
                lastSubseaTrigger = now;
            }
        }
    }

    // Update display parameter if needed.
    if (LLDateTime::isNull(lastOSDParamTransition)) {
        lastOSDParamTransition = now;
    }
    int64_t elapsedOSDParamTransitionMillis = LLDateTime::timeDifferenceMillis(now, lastOSDParamTransition);
    if (elapsedOSDParamTransitionMillis >= 1000) { //3000
        appState.setOSDParameter(LLDisplay::nextOSDParameter(appState.getOSDParameter()));
        lastOSDParamTransition = now;
    }

}


// WiFi stuff.



