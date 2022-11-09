
#include <SPI.h>
#include <WiFiNINA.h>



namespace LLWiFi {

    const int NUM_KNOWN_WIFI_NETWORKS = 4;

    char * KNOWN_WIFI_NETWORKS[NUM_KNOWN_WIFI_NETWORKS][2] = {
        { "Ted Zhu iPhone", "zhu123456" },
        { "Sawi7", "22223333" },
        { "KRKS",  "41meadow" },
        { "LobsterLift-WiFi-Router",  "ropeless" }
        // { "Bradford's iPhone 11",  "Ki1710915" },
        // { "Ted Zhu WiFi", "nNrZF9rzxA" }
        // , { "LobsterLift 2.4", "hondros8" }
    };


    String printHelloo() {
        return "HIHIHIHIH WIFI";
    }


    char ssid[] = "Ted Zhu iPhone";        // your network SSID (name)
    char pass[] = "zhu123456";               // your network password (use for WPA, or use as key for WEP)


    int status = WL_IDLE_STATUS;     // the Wifi radio's status


    WiFiServer server(80);


    // const char STR_WL_NO_MODULE__OR__WL_NO_SHIELD[] = "WL_NO_MODULE__OR__WL_NO_SHIELD";
    const char STR_WL_NO_MODULE[] = "WL_NO_MODULE";
    const char STR_WL_NO_SHIELD[] = "WL_NO_SHIELD";
    const char STR_WL_IDLE_STATUS[] = "WL_IDLE_STATUS";
    const char STR_WL_NO_SSID_AVAIL[] = "WL_NO_SSID_AVAIL";
    const char STR_WL_SCAN_COMPLETED[] = "WL_SCAN_COMPLETED";
    const char STR_WL_CONNECTED[] = "WL_CONNECTED";
    const char STR_WL_CONNECT_FAILED[] = "WL_CONNECT_FAILED";
    const char STR_WL_CONNECTION_LOST[] = "WL_CONNECTION_LOST";
    const char STR_WL_DISCONNECTED[] = "WL_DISCONNECTED";
    const char STR_WL_AP_LISTENING[] = "WL_AP_LISTENING";
    const char STR_WL_AP_CONNECTED[] = "WL_AP_CONNECTED";
    const char STR_WL_AP_FAILED[] = "WL_AP_FAILED";
    const char STR_UNRECONGIZED_STATUS[] = "UNRECONGIZED_STATUS";

    const char * getWifiStatus() {

        int status = WiFi.status();

        // if (status == WL_NO_MODULE || status == WL_NO_SHIELD) {
        //     return STR_WL_NO_MODULE__OR__WL_NO_SHIELD;
        // }

        if (status == WL_NO_MODULE) {
            return STR_WL_NO_MODULE;
        }

        if (status == WL_NO_SHIELD) {
            return STR_WL_NO_SHIELD;
        }

        if (status == WL_IDLE_STATUS) {
            return STR_WL_IDLE_STATUS;
        }

        if (status == WL_NO_SSID_AVAIL) {
            return STR_WL_NO_SSID_AVAIL;
        }
        if (status == WL_SCAN_COMPLETED) {
            return STR_WL_SCAN_COMPLETED;
        }
        if (status == WL_CONNECTED) {
            return STR_WL_CONNECTED;
        }
        if (status == WL_CONNECT_FAILED) {
            return STR_WL_CONNECT_FAILED;
        }
        if (status == WL_CONNECTION_LOST) {
            return STR_WL_CONNECTION_LOST;
        }
        if (status == WL_DISCONNECTED) {
            return STR_WL_DISCONNECTED;
        }
        if (status == WL_AP_LISTENING) {
            return STR_WL_AP_LISTENING;
        }
        if (status == WL_AP_CONNECTED) {
            return STR_WL_AP_CONNECTED;
        }
        if (status == WL_AP_FAILED) {
            return STR_WL_AP_FAILED;
        }

        return STR_UNRECONGIZED_STATUS;
        // return ("Unrecongized status: " + String(status));


    }

    void printWifiStatus() {
        Serial.print("WIFI STATUS: ");
        Serial.println(getWifiStatus());
    }






    void setup() {

        // Check for basic requirements.

        // check for the WiFi module:
        if (WiFi.status() == WL_NO_MODULE) {
            Serial.println(F("Communication with WiFi module failed!"));
            // don't continue
            while (true);
        }

        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
            Serial.println(F("Please upgrade the firmware"));
        }

    }

    bool _isConnected = 0;
    bool isConnected() {
        return _isConnected;
    }

    char MY_STATUS_CONNECTING[] = "Connecting...";
    char MY_STATUS_SEARCHING[]  = "Searching for networks...";
    // char MY_STATUS_CONNECTED[]  = "Connected";
    char MY_STATUS_CONNECTED[]  = "No connection.";
    char MY_STATUS_NO_WIFI[]    = "No WIFI";

    char * my_status = MY_STATUS_NO_WIFI;

    char * myStatus() {
        return my_status;
    }



    /*

    Prefer client to check isConnected() before accessing connectionNetworkName() and connectionIpAddress().

     */


    const char * _connectionNetworkName;
    const char * connectionNetworkName() {
        return _connectionNetworkName;
    }

    IPAddress _connectionIpAddress;
    IPAddress connectionIpAddress() {
        return _connectionIpAddress;
    }

    long _connectionStrength;
    long connectionStrength() {
        return _connectionStrength;
    }

    const char * _connectionWifiStatus;
    const char * connectionWifiStatus() {
        return _connectionWifiStatus;
    }


    void refreshStatuses() {
        _isConnected = (WiFi.status() == WL_CONNECTED);
        _connectionNetworkName = WiFi.SSID();
        _connectionIpAddress = WiFi.localIP();
        _connectionStrength = WiFi.RSSI();
        _connectionWifiStatus = getWifiStatus();
    }


    // Cannot be run inside timer interrupt (update display routine).
    bool isConnectedQuery() {
        return (WiFi.status() == WL_CONNECTED);
    }

    // Returns index of found known network.
    int foundKnownNetwork() {

        int numSsid = WiFi.scanNetworks();

        

        Serial.print(F("Found "));
        Serial.print(numSsid);
        Serial.print(F(" networks: "));
        for (int i = 0; i < numSsid; i++) {
            const char * ssidName = WiFi.SSID(i);
            Serial.print(ssidName);
            if (i < numSsid - 1) {
                Serial.print(F(", "));
            }
        }
        Serial.println();
        Serial.println();

        for (int i = 0; i < numSsid; i++) {

            const char * ssidName = WiFi.SSID(i);

            // Go through all known networks for match.
            for (int j = 0; j < NUM_KNOWN_WIFI_NETWORKS; j++) {
                char * * pair = KNOWN_WIFI_NETWORKS[j];
                char * knownName = pair[0];

                Serial.print(F("Comparing found ssid \""));
                Serial.print(ssidName);
                Serial.print(F("\" with known name \""));
                Serial.print(knownName);
                Serial.print(F("\". Result: "));

                int compareResult = strcmp(ssidName, knownName);

                if (compareResult == 0) {
                    Serial.println(F(" <------------------- MATCH"));
                    return j;
                } else {
                    Serial.println(F(" NOMATCH"));
                }
                

            }
        }

        // If reached here, no matching network found.
        return -1;

    }

    void setStatusSearching() {
        my_status = MY_STATUS_SEARCHING;
    }

    void tryConnect() {

        my_status = MY_STATUS_SEARCHING;

        const int CONNECTION_ATTEMPTS = 1;

        // attempt 3 times to connect to Wifi network:
        for (int i = 0; i < CONNECTION_ATTEMPTS; i++) {

            int foundKnownNetworkIndex = foundKnownNetwork();

            if (foundKnownNetworkIndex >= 0) {
                
                my_status = MY_STATUS_CONNECTING;

                char * * pair = KNOWN_WIFI_NETWORKS[foundKnownNetworkIndex];
                char * knownName     = pair[0];
                char * knownPassword = pair[1];

                Serial.print(F("Attempting to connect to WPA SSID: "));
                Serial.println(knownName);

                // Attempt to connect to known WPA/WPA2 network:
                status = WiFi.begin(knownName, knownPassword);

                

            } else {
                Serial.println(F("No matching WPA SSID found."));
                WiFi.end();
                my_status = MY_STATUS_NO_WIFI;
            }

            



            // printWifiStatus();

            // Serial.println();
        

            if (status == WL_CONNECTED) {
                my_status = MY_STATUS_CONNECTED;
                break;
            }

        
        }
    }

    
    long thisSecond;
    long lastSecond = -1;

    void loop() {

        unsigned long thisMillis = millis();
        thisSecond = thisMillis/1000;
        // Serial.println(thisMillis);
        
        if (thisSecond != lastSecond) {

            if (thisSecond % 5 == 0) {

                Serial.println("Every 5 seconds");

                unsigned long start = micros();

                if (!isConnected()) {
                    tryConnect();
                } else {
                    Serial.print("Already connected to '");
                    Serial.print(connectionNetworkName());
                    Serial.print("' at ip address ");
                    Serial.print(connectionIpAddress());
                    Serial.println();
                }

                unsigned long end = micros();
                unsigned long delta = end - start;
                Serial.println(delta / 1000.0);


                printWifiStatus();

                
                Serial.println();

            }


            lastSecond = thisSecond;

        }

    }

    void tryRestart() {
        WiFi.end();
        tryConnect();
    }





    WiFiClient client;


}

