
/*
    LED

    This example creates a BLE peripheral with service that contains a
    characteristic to control an LED.

    The circuit:
    - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
        Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

    You can use a generic BLE central app, like LightBlue (iOS and Android) or
    nRF Connect (Android), to interact with the services and characteristics
    created in this sketch.

    This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include "LLBle.h"
#include "LLAppState.h"



// BLE Status Strings
char BLE_STATUS_UNKNOWN_STRING[] = "BLE_STATUS_UNKNOWN";
char BLE_STATUS_CENTRAL_FOUND_STRING[] = "BLE_STATUS_CENTRAL_FOUND";
char BLE_STATUS_CENTRAL_NOT_FOUND_STRING[] = "BLE_STATUS_CENTRAL_NOT_FOUND";
char BLE_STATUS_CENTRAL_CONNECTED_STRING[] = "BLE_STATUS_CENTRAL_CONNECTED";
char BLE_STATUS_CENTRAL_DISCONNECTED_STRING[] = "BLE_STATUS_CENTRAL_DISCONNECTED";
char BLE_STATUS_CENTRAL_NO_LONGER_FOUND_STRING[] = "BLE_STATUS_CENTRAL_NO_LONGER_FOUND";



char * bleStatusString(BleStatus status) {
    switch (status) {
        case BLE_STATUS_UNKNOWN:
            return BLE_STATUS_UNKNOWN_STRING;

        case BLE_STATUS_CENTRAL_FOUND:
            return BLE_STATUS_CENTRAL_FOUND_STRING;

        case BLE_STATUS_CENTRAL_NOT_FOUND:
            return BLE_STATUS_CENTRAL_NOT_FOUND_STRING;

        case BLE_STATUS_CENTRAL_CONNECTED:
            return BLE_STATUS_CENTRAL_CONNECTED_STRING;

        case BLE_STATUS_CENTRAL_DISCONNECTED:
            return BLE_STATUS_CENTRAL_DISCONNECTED_STRING;

        case BLE_STATUS_CENTRAL_NO_LONGER_FOUND:
            return BLE_STATUS_CENTRAL_NO_LONGER_FOUND_STRING;

        default:
            return "Unknown type!";
    }
};





// BLELongCharacteristic triggerTimeCharacteristicPartA("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
// BLELongCharacteristic triggerTimeCharacteristicPartB("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);
// BLELongCharacteristic triggerTimeCharacteristicVersion("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite);

const int ledPin = A0; // pin to use for the LED
const int TIME_SET_PIN = 2;
int ble_led_assert_on = false;

void ble_setup() {

    // set LED pin to output mode
    pinMode(ledPin, OUTPUT);

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");

        while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("LED Ted-Zhu-Test");
    BLE.setAdvertisedService(ledService);

    // add the characteristic to the service
    ledService.addCharacteristic(switchCharacteristic);
    ledService.addCharacteristic(triggerTimeCharacteristic);
    ledService.addCharacteristic(inflationDurationCharacteristic);

    // add service
    BLE.addService(ledService);

    // set the initial value for the characeristic:
    switchCharacteristic.writeValue(0);
    triggerTimeCharacteristic.writeValue(LLDateTime::bluetoothRepresentation(appState.getTriggerTime()));
    inflationDurationCharacteristic.writeValue(appState.getInflationDuration());

    // start advertising
    BLE.advertise();

    Serial.println("BLE LED Peripheral");
}





int ble_connected = false;
BLEDevice central;


void ble_loop() {

    // Print BLE Diagnostics.
    // BLEDevice central_diagnostic = BLE.central();
    // Serial.println(F("Bluetooth Diagnostics: "));
    // Serial.println(central_diagnostic.address());
    // Serial.print(F("Available: "));
    // Serial.println(BLE.available());

    if (!ble_connected) {
        // listen for BLE peripherals to connect:
        central = BLE.central();
        
        // if a central is connected to peripheral:
        if (central) {
            appState.setBluetoothStatus(BLE_STATUS_CENTRAL_FOUND);
            Serial.print("Connected to central: ");
            // print the central's MAC address:
            Serial.println(central.address());
            central.connected();
            ble_connected = true;
        } else {
            appState.setBluetoothStatus(BLE_STATUS_CENTRAL_NOT_FOUND);
        }

    } else {
        if (!BLE.central()) {
            Serial.println("Central no longer found.");
        }
        if (central.connected()) {
            appState.setBluetoothStatus(BLE_STATUS_CENTRAL_CONNECTED);
            // if the remote device wrote to the characteristic,
            // use the value to control the LED:
            if (switchCharacteristic.written()) {
                if (switchCharacteristic.value()) {   // any value other than 0
                    Serial.println("LED on");
                //   digitalWrite(ledPin, HIGH);         // will turn the LED on
                        ble_led_assert_on = true;
                } else {                              // a 0 value
                    Serial.println(F("LED off"));
                    digitalWrite(ledPin, LOW);          // will turn the LED off
                    ble_led_assert_on = false;
                }
            }


            if (triggerTimeCharacteristic.written()) {

                // Note: The code below assumes that the trigger time written is "5 seconds" from the current time of the mobile phone.
                // The code below has unconditional code that sets the lobsterlift device's time to 5 seconds behind the trigger time written.
                // It is assumed that on the mobile phone, only the "Trigger 5 seconds from now" feature is the only triggerTimeCharacterstic bluetooth writing feature
                // That is, there is no other manual date/time trigger time feature on the mobile app. Otherwise, say someone tried to set a manual date/time of next week;
                // the LobsterLift device time will be set to next week, but 5 seconds behind.
                
                // You'll need to remove the unconditional LLDateTime::setCurrentTime() call below should you wish to add a manual data/time trigger feature in the mobile app.

                
                Serial.print("Setting new trigger time Characteristic Value... ");
                char result[LLDateTime::MAX_TIME_BUFFER_LENGTH];
                LLDateTime::longString(LLDateTime::fromBluetoothRepresentation(triggerTimeCharacteristic.value()), result);
                Serial.println(result);

                // Deprected: if (digitalRead(TIME_SET_PIN) == LOW) {
                if (digitalRead(TIME_SET_PIN) == LOW) {
                // if (1) { // Alway reprogram time on device.
                    
                    LLDateTime::LLTime fiveSecsFromNow = LLDateTime::fromBluetoothRepresentation(triggerTimeCharacteristic.value());
                    LLDateTime::LLTime currentTime = LLDateTime::fromOffset(fiveSecsFromNow, -5000);
                    LLDateTime::setCurrentTime(currentTime);

                    // appState.setTriggerTime(LLDateTime::fromBluetoothRepresentation(triggerTimeCharacteristic.value()));
                } else {
                    appState.setTriggerTime(LLDateTime::fromBluetoothRepresentation(triggerTimeCharacteristic.value()));
                }
            }

            if (inflationDurationCharacteristic.written()) {
                appState.setInflationDuration(inflationDurationCharacteristic.value());
            }



        } 
        else {
            appState.setBluetoothStatus(BLE_STATUS_CENTRAL_DISCONNECTED);
            // when the central disconnects, print it out:
            Serial.print(F("Disconnected from central: "));
            Serial.println(central.address());
            ble_connected = false;
        }

    }
}

