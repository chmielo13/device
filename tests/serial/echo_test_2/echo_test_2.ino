
// #include <SoftwareSerial.h>

// SoftwareSerial mySerialConnection(2, 3);

void setup() {

    Serial.begin(9600);

    while (!Serial);

    Serial.println("Hello World from Echo Test2!!s");

    Serial1.begin(9600);

    Serial1.println("Beginning my custom my serial connection!");

}

int IDLE_MODE  = 1;
int READ_MODE  = 2;
int WRITE_MODE = 3;

int currentMode = IDLE_MODE;

// char message[200];
String message;

void loop() {


    // printCurrentMode();

    int byte;


    // int messageIndex = 0;
    // while (mySerialConnection.available() && byte != 10) {
    //     int byte = mySerialConnection.read();
    //     currentMode = READ_MODE;
    //     mySerialConnection.write(byte);
        
    // }
    if (Serial1.available()) {
        message = Serial1.readStringUntil('\n');
        // mySerialConnection.print("Received message: ");
        // mySerialConnection.println(message);

        Serial.print("Received message: ");
        Serial.println(message);
    }

}


void printCurrentMode() {
    Serial.print("Current: ");
    if (currentMode == IDLE_MODE) {
        Serial.print("IDLE_MODE");
    } else if (currentMode == READ_MODE) {
        Serial.print("READ_MODE");
    } else if (currentMode == WRITE_MODE) {
        Serial.print("WRITE_MODE");
    } else {
        Serial.print("UNKOWN ");
        Serial.print(currentMode);
    }

    Serial.println();

}

