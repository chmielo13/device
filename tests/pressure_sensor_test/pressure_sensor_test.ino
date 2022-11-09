
const int SLAVE_SELECT_1_PIN = 6;
const int SLAVE_SELECT_2_PIN = 7;

const int MISO_PIN = 10;
const int SCLK_PIN = 9;
const int MOSI_PIN = 8;

const int DIFFERENTIAL_PRESSURE_SENSOR_SLAVE_SELECT_PIN = SLAVE_SELECT_1_PIN;
const int GUAGE_PRESSURE_SENSOR_SLAVE_SELECT_PIN        = SLAVE_SELECT_2_PIN;

#include <SPI.h>

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    // Wait until Serial port is open.
    // while(!Serial);
    delay(5000);
    Serial.println("Hello from Pressure Sensor Test!");

    pinMode(DIFFERENTIAL_PRESSURE_SENSOR_SLAVE_SELECT_PIN, OUTPUT);
    pinMode(GUAGE_PRESSURE_SENSOR_SLAVE_SELECT_PIN, OUTPUT);


    // digitalWrite(DIFFERENTIAL_PRESSURE_SENSOR_SLAVE_SELECT_PIN, HIGH);
    digitalWrite(GUAGE_PRESSURE_SENSOR_SLAVE_SELECT_PIN, HIGH);

    SPI.begin();


}

void loop() {

    int guageVal = 0;
    guageVal = analogRead(A0);
    // Serial.println(guageVal);


    SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));




    digitalWrite(DIFFERENTIAL_PRESSURE_SENSOR_SLAVE_SELECT_PIN, LOW);

    // Read pressure

    byte byte1 = SPI.transfer(0x00);
    byte byte2 = SPI.transfer(0x00);
    byte byte3 = SPI.transfer(0x00);
    byte byte4 = SPI.transfer(0x00);

    // Filters
    byte byte1StatusFilter = B11000000;
    byte byte1DataFilter   = B00111111;
    byte byte2DataFilter   = B11111111;
    byte byte3DataFilter   = B11111111;
    byte byte4DataFilter   = B11100000;

    int status = byte1 & byte1StatusFilter;
    status >>= 6;

    int bridgeData = (byte1 & byte1DataFilter);
    bridgeData <<= 8;
    bridgeData += (byte2 & byte2DataFilter);

    // Serial.println("Byte 1-4: " + String(byte1) + ", " + String(byte2) + ", " + String(byte3) + ", " + String(byte4));
    Serial.println("Status: " + String(status) + ",   Differential Data: " + String(bridgeData) + ",     Guage Data: " + String(guageVal));

    digitalWrite(DIFFERENTIAL_PRESSURE_SENSOR_SLAVE_SELECT_PIN, HIGH);

    SPI.endTransaction();

}
