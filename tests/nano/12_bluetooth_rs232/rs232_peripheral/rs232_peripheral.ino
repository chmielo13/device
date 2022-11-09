#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //232_TX,232_RX

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }


    Serial.println("Arduino Serial Test!");

    // set the data rate for the SoftwareSerial port
    mySerial.begin(9600);
    mySerial.println("Hello, world?");
}

unsigned long lastLogTime;

void loop() // run over and over
{
    // Emit to the log so that we can know that the log
    // is live rather than dead.
    unsigned long currentTime = millis();
    if (currentTime - lastLogTime >= 3000) {
        Serial.println(currentTime);
        lastLogTime = currentTime;
    }

    if (mySerial.available())
    Serial.write(mySerial.read());
    if (Serial.available())
    mySerial.write(Serial.read());
}


// void setup() {
//     Serial.begin(9600);
//     Serial1.begin(9600);
// }

// void loop() {
//     Serial.println("Hello Nano IOT RS232 test!!");
//     Serial1.println("SSSERIAL Nano IOT RS232 test!!");
//     delay(1000);
// }