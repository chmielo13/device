

#include <SD.h>
#include <stdarg.h>
#include <stdio.h>
#include <ArduinoJson.h>




#include "LLConfig.h"
#include "utilities.cpp"







#define CONFIG_FILE_PATH "config"



namespace LLConfig {

        
    File myFile;


    // TODO: use stream for JSON parser to consume config string instead of long-lived buffer.
    #define CONFIG_BUFFER_SIZE 500
    char config_buffer[CONFIG_BUFFER_SIZE];
    int config_buffer_cursor = 0;


    void test_deserialization() {
        StaticJsonDocument<16> doc;

        DeserializationError error = deserializeJson(doc, config_buffer, CONFIG_BUFFER_SIZE);

        if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
        }

        const char* modem_type_string = doc["MODEM_TYPE"]; // "modem_modem_modem"

        pf("Found modem type: %s", modem_type_string);

        CONFIG_MODEM_TYPE = string_to_modem_type(modem_type_string);

        pf("CONFIG_MODEM_TYPE: %d", CONFIG_MODEM_TYPE);

        if (CONFIG_MODEM_TYPE != MODEM_TYPE_UNRECOGNIZED) {
            config_setup_successful = true;
        }


    }

    void initSDCard() {
        pinMode(10, OUTPUT);
        digitalWrite(10, HIGH);

        // while (!Serial) {
        //     ; // wait for serial port to connect. Needed for native USB port only
        // }

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

    #define MAX_CONFIG_TRIES 15

    void setup() {
        
        while (!config_setup_successful) {
            // delay(500);
            Serial.print("SD Card Config Init Attempt: ");
            Serial.println(numSDConfigTries);
            initSDCard();
            if (numSDConfigTries > MAX_CONFIG_TRIES) {
                return;
            }
            numSDConfigTries++;
        }
        
    }


}