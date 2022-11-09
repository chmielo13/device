#include <SD.h>
#include <stdarg.h>
#include <stdio.h>
#include <ArduinoJson.h>

#define CONFIG_FILE_PATH "config"

#define SPF_BUFFER_SIZE 255
char pf_buffer[255];
#define pf(fmt, ...) _pf(PSTR(fmt), __VA_ARGS__)

File myFile;



#define CONFIG_BUFFER_SIZE 500
char config_buffer[CONFIG_BUFFER_SIZE];
int config_buffer_cursor = 0;


void _pf(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);
    vsnprintf(pf_buffer, sizeof(pf_buffer), fmt, valist);
    va_end(valist);
    Serial.println(pf_buffer);
}

void test_deserialization() {
    StaticJsonDocument<16> doc;

    DeserializationError error = deserializeJson(doc, config_buffer, CONFIG_BUFFER_SIZE);

    if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
    }

    const char* MODEM_TYPE = doc["MODEM_TYPE"]; // "modem_modem_modem"

    pf("Found modem type: %s", MODEM_TYPE);

}
void setup()
{
    
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);

    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.print("Initializing SD card...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin 
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
    // or the SD library functions will not work. 

    if (!SD.begin(10)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    // open the file for reading:
    myFile = SD.open(CONFIG_FILE_PATH);
    if (myFile) {
        pf("Content of: %s", CONFIG_FILE_PATH);

        // test_deserialization(&myFile);

        // read from the file until there's nothing else in it:
        while (myFile.available()) {
            // Serial.write(myFile.read());
            config_buffer[config_buffer_cursor] = myFile.read();
            config_buffer_cursor++;
        }
        // close the file:
        myFile.close();
        Serial.println(config_buffer);

        test_deserialization();
        
    } else {
        // if the file didn't open, print an error:
        pf("Error opening %s.", CONFIG_FILE_PATH);
    }
}

void loop()
{
    // nothing happens after setup
}