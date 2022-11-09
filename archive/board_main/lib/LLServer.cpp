
#include <SPI.h>
#include <WiFiNINA.h>



WiFiServer server(80);


namespace LLServer {

    /* Call when connected */
    void beginServer() {
        Serial.println("Starting web server...");
        server.begin();
    }

    void setup() {
    }

    void loop() {
    }

}

