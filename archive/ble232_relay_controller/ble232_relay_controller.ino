
#define RELAY_PIN 3
#define LED_PIN 2

int relay_energized = 0;

// void setup() {

//     pinMode(RELAY_PIN, OUTPUT);
//     digitalWrite(RELAY_PIN, relay_energized);

//     Serial.begin(9600);


// }

// void loop() {
//     Serial.println("Hello World from BLE232 Relay Controller!!");
//     delay(3000);
// }

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

BLEService relayService("180D4AB2-632A-4F38-AA61-A7DC2401FB98"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic powerCycleRequestNumberCharacteristic("D9912AD9-0C9F-48C2-A35F-724CEC06B6A2", BLERead | BLEWrite);

void update_pins_based_on_state() {
    if (relay_energized) {
        digitalWrite(RELAY_PIN, HIGH);
        digitalWrite(LED_PIN, LOW);
    } else {
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(LED_PIN, HIGH);
    }
}

void setup() {
    Serial.begin(9600);
    // while (!Serial);

    // set LED pin to output mode
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    update_pins_based_on_state();

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");

        while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("LL-BLE232-RELAY-CONTROLLER");
    BLE.setAdvertisedService(relayService);

    // add the characteristic to the service
    relayService.addCharacteristic(powerCycleRequestNumberCharacteristic);

    // add service
    BLE.addService(relayService);

    // set the initial value for the characeristic:
    powerCycleRequestNumberCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();

    Serial.println("BLE LED Peripheral");
}

int lastHandledRequest = 0;

void loop() {
    // listen for BLE peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());

        // while the central is still connected to peripheral:
        while (central.connected()) {
            // if the remote device wrote to the characteristic,
            // use the value to control the LED:
            if (powerCycleRequestNumberCharacteristic.written()) {
                    relay_energized = 1;
                    update_pins_based_on_state();
                    delay(2000);
                    relay_energized = 0;
                    update_pins_based_on_state();
                // if (powerCycleRequestNumberCharacteristic.value()) {   // any value other than 0
                //   Serial.println("LED on");
                //   digitalWrite(LED_PIN, HIGH);         // will turn the LED on
                // } else {                              // a 0 value
                //   Serial.println(F("LED off"));
                //   digitalWrite(LED_PIN, LOW);          // will turn the LED off
                // }
            }
        }

        // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    }
}
