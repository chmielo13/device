

#include <Wire.h>
#include <SparkFunDS1307RTC.h>


#include "lib/LLWiFi.cpp"


void setup() {

    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Hello World from WPA Periodic Connect Test!");


    LLWiFi::printHelloo();



    rtc.begin();

    // Always assume 24-hour mode.
    rtc.set24Hour();
    // rtc.set12Hour();
    

}

unsigned long thisMillis;
long thisSecond;
long lastSecond = -1;



void loop() {

    thisMillis = millis();
    thisSecond = thisMillis/1000;
    // Serial.println(thisMillis);
    
    if (thisSecond != lastSecond) {

        if (thisSecond % 5 == 0) {

            Serial.println("Every 5 seconds");

            unsigned long start = micros();

            if (!LLWiFi::isConnected()) {
                LLWiFi::tryConnect();
            } else {
                Serial.print("Already connected to '");
                Serial.print(LLWiFi::connectionNetworkName());
                Serial.print("' at ip address ");
                Serial.print(LLWiFi::connectionIpAddress());
                Serial.println();
            }

            unsigned long end = micros();
            unsigned long delta = end - start;
            Serial.println(delta / 1000.0);


            LLWiFi::printWifiStatus();

            
            Serial.println();

        }


        lastSecond = thisSecond;

    }

    

}

