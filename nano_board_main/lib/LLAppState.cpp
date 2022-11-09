#include "LLAppState.h"
#include "LLDateTime.h"

// Temporarily
#include "RTClib.h"
#include "LLDisplay.h"


#include <Adafruit_SSD1306.h>

char* indent = "    ";

void printStateLabel(const __FlashStringHelper * label) {
    Serial.print(indent);
    Serial.print(label);
    Serial.print(F(": "));
}


void printOSDParameter(Adafruit_SSD1306 *display) {

    if (!LLConfig::config_setup_successful) {
        display->print(F("ERROR: SDCard Config Not Read"));
    } else {

        // No error states, print normal OSD Parameter.

        enum LLDisplay::osd_parameter osdParameter = appState.getOSDParameter();

        switch (osdParameter) {
            case LLDisplay::OSDPARAM_BLE_STATUS:
                display->print(F("BLE Status: "));
                display->print(appState.getDerivedAppState().getBluetoothStatusDisplay());
                break;
            case LLDisplay::OSDPARAM_FREE_MEMORY:
                display->print(F("Free Mem: "));
                display->print(appState.getDerivedAppState().getFreeMemoryRemainingDisplay());
                break;
            case LLDisplay::OSDPARAM_MODEM_TYPE:
                display->print(F("Modem: "));
                display->print(LLConfig::modem_type_to_string(appState.getModemType()));
                break;
            case LLDisplay::OSDPARAM_SDCONFIG_TRIES:
                display->print(F("SDConfTry: "));
                display->print(LLConfig::numSDConfigTries);
                break;
            default:
                break;
        }
    }


    // display->print(F("BLE2 Status: "));
    // display->print(appState.getDerivedAppState().getBluetoothStatusDisplay());
}

void printAppState() {

    Serial.println(F("App State: "));

    printStateLabel(F("Hello"));
    Serial.println(appState.getHelloIncrement());
    
    printStateLabel(F("pendingIncrement"));
    Serial.println(appState.getPendingIncrement());
    
    printStateLabel(F("rtcReprogrammed"));
    Serial.println(appState.getRtcReprogrammed());

    printStateLabel(F("Memory remaining"));
    Serial.println(appState.getDerivedAppState().getFreeMemoryRemainingDisplay());

    printStateLabel(F("Bluetooth Status"));
    Serial.println(appState.getDerivedAppState().getBluetoothStatusDisplay());

    printStateLabel(F("Trigger Time"));
    char buffer[LLDateTime::MAX_TIME_BUFFER_LENGTH];
    LLDateTime::longString(appState.getTriggerTime(), buffer);
    Serial.println(buffer);

    printStateLabel(F("Modem Type"));
    Serial.println(LLConfig::modem_type_to_string(appState.getModemType()));


    printStateLabel(F("Size of Int"));
    Serial.println(sizeof(int));

    printStateLabel(F("Size of Long"));
    Serial.println(sizeof(long));

    printStateLabel(F("Size of Long Long"));
    Serial.println(sizeof(long long));

    printStateLabel(F("Max Long"));
    Serial.println(9223372036854775807L);
    
    printStateLabel(F("Arbitrary Long"));
    Serial.println(8213372035824775807L, BIN);

    // long longs[2];
    // LongLongCharacteristic::splitLongLong(8213372035824775807L, longs);
    
    // printStateLabel(F("Long0"));
    // Serial.println(longs[0], BIN);

    // printStateLabel(F("Long1"));
    // Serial.println(longs[1], BIN);

    // printStateLabel(F("Recomposed Longs"));
    // Serial.println(LongLongCharacteristic::composeLongLong(longs[0], longs[1]), BIN);

    
    

    printStateLabel(F("rtc.now()"));
    Serial.println(rtc.now().unixtime());
    
    printStateLabel(F("True"));
    Serial.println(true);
    
    printStateLabel(F("Short DateTime"));
    Serial.println(LLDateTime::currentShortTimeString());
    
    printStateLabel(F("Long DateTime"));
    Serial.println(LLDateTime::currentLongTimeString());
    
    printStateLabel(F("Null DateTime"));
    LLDateTime::LLTime nullDateTime;
    char timeStringBufferResult[LLDateTime::MAX_TIME_BUFFER_LENGTH];
    LLDateTime::longString(nullDateTime, timeStringBufferResult);
    Serial.println(timeStringBufferResult);


    Serial.println();
    
}

