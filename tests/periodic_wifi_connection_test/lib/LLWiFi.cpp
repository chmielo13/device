
/*



Warning Deprecated!!! Latest updates should be from board_main/lib/LLWiFi.cpp.
Changes from that file may be periodically copied to here.

- Ted Zhu























*/


#include <SPI.h>
#include <WiFiNINA.h>



namespace LLWiFi {

    const int NUM_KNOWN_WIFI_NETWORKS = 4;

    char * KNOWN_WIFI_NETWORKS[NUM_KNOWN_WIFI_NETWORKS][2] = {
        { "Ted Zhu iPhone", "zhu123456" },
        { "Sawi7", "22223333" },
        { "Ted Zhu WiFi", "nNrZF9rzxA" },
        { "LobsterLift 2.4", "hondros8" }
    };


    String printHelloo() {
        return "HIHIHIHIH WIFI";
    }


    char ssid[] = "Ted Zhu iPhone";        // your network SSID (name)
    char pass[] = "zhu123456";               // your network password (use for WPA, or use as key for WEP)


    int status = WL_IDLE_STATUS;     // the Wifi radio's status


    String getWifiStatus() {

        int status = WiFi.status();

        if (status == WL_NO_MODULE || status == WL_NO_SHIELD) {
            return "WL_NO_MODULE__OR__WL_NO_SHIELD";
        }

        if (status == WL_IDLE_STATUS) {
            return "WL_IDLE_STATUS";
        }

        if (status == WL_NO_SSID_AVAIL) {
            return "WL_NO_SSID_AVAIL";
        }
        if (status == WL_SCAN_COMPLETED) {
            return "WL_SCAN_COMPLETED";
        }
        if (status == WL_CONNECTED) {
            return "WL_CONNECTED";
        }
        if (status == WL_CONNECT_FAILED) {
            return "WL_CONNECT_FAILED";
        }
        if (status == WL_CONNECTION_LOST) {
            return "WL_CONNECTION_LOST";
        }
        if (status == WL_DISCONNECTED) {
            return "WL_DISCONNECTED";
        }
        if (status == WL_AP_LISTENING) {
            return "WL_AP_LISTENING";
        }
        if (status == WL_AP_CONNECTED) {
            return "WL_AP_CONNECTED";
        }
        if (status == WL_AP_FAILED) {
            return "WL_AP_FAILED";
        }

        return ("Unrecongized status: " + String(status));


    }

    void printWifiStatus() {
        Serial.println("WIFI STATUS: " + getWifiStatus());
    }

    void setup() {

        // Check for basic requirements.

        // check for the WiFi module:
        if (WiFi.status() == WL_NO_MODULE) {
            Serial.println("Communication with WiFi module failed!");
            // don't continue
            while (true);
        }

        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
            Serial.println("Please upgrade the firmware");
        }

    }


    int isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }


    /*

    Prefer client to check isConnected() before accessing connectionNetworkName() and connectionIpAddress().

     */

    const char * connectionNetworkName() {
        return WiFi.SSID();
    }

    IPAddress connectionIpAddress() {
        return WiFi.localIP();
    }


    // Returns index of found known network.
    int foundKnownNetwork() {

        int numSsid = WiFi.scanNetworks();
        for (int i = 0; i < numSsid; i++) {

            const char * ssidName = WiFi.SSID(i);

            // Go through all known networks for match.
            for (int j = 0; j < NUM_KNOWN_WIFI_NETWORKS; j++) {
                char * * pair = KNOWN_WIFI_NETWORKS[j];
                char * knownName = pair[0];

                Serial.print(
                    "Comparing found ssid \"" + String(ssidName) + "\" with known name \"" + String(knownName) + "\". Result: "
                );

                int compareResult = strcmp(ssidName, knownName);

                if (compareResult == 0) {
                    Serial.println(" <------------------- MATCH");
                    return j;
                } else {
                    Serial.println(" NOMATCH");
                }
                

            }
        }

        // If reached here, no matching network found.
        return -1;

    }

    void tryConnect() {



        const int CONNECTION_ATTEMPTS = 1;

        // attempt 3 times to connect to Wifi network:
        for (int i = 0; i < CONNECTION_ATTEMPTS; i++) {

            int foundKnownNetworkIndex = foundKnownNetwork();

            if (foundKnownNetworkIndex >= 0) {
                char * * pair = KNOWN_WIFI_NETWORKS[foundKnownNetworkIndex];
                char * knownName     = pair[0];
                char * knownPassword = pair[1];

                Serial.print("Attempting to connect to WPA SSID: ");
                Serial.println(knownName);

                // Attempt to connect to known WPA/WPA2 network:
                status = WiFi.begin(knownName, knownPassword);

            } else {
                Serial.println("No matching WPA SSID found.");
            }

            



            // printWifiStatus();

            // Serial.println();
        

            if (status == WL_CONNECTED) {
                break;
            }

        
        }
    }



}

