// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

#define CLOCK_PIN A0
#define BUTTON_PIN A1


RTC_DS3231 rtc;

void setup() {
    Serial.begin(9600);

    //delay(1000);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(CLOCK_PIN, OUTPUT);


    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        // abort();
    }

}

int state = 0;

#define BUTTON_PRESSED 0

void loop() {

    int input = digitalRead(BUTTON_PIN);

    Serial.print("Button: ");
    Serial.print(input);

    
    if (rtc.begin()) {
        DateTime now = rtc.now();

        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
    }


    // Serial.print("State: ");
    // Serial.print(state);

    Serial.println();

    if (input == !BUTTON_PRESSED) {

        digitalWrite(CLOCK_PIN, state);
    }

    state = !state;
}

