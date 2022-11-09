#include "lib/LLTimeDate.cpp"
#include "lib/LLDisplay.cpp"
#include "lib/LLWiFi.cpp"
#include "lib/LLServer.cpp"
#include "lib/LLSAMD21Timer.cpp"

#include <MemoryFree.h>;


// MKR WIFI 1010 Definitions
// #define BUTTON_1_PIN 0



#define NANO_33_IOT 1




#ifdef NANO_33_IOT
    #define BUTTON_1_PIN 2
#endif


// int BUTTON_1_PIN = 0;
// int BUTTON_2_PIN = 1;
// int BUTTON_3_PIN = 2;

int LED_1_PIN  = A0;
int LED_2_PIN = A1;
int LED_3_PIN = A2;
int LED_4_PIN = A3;

int RELAY_1_PIN = 3;
int RELAY_2_PIN = 4;
int RELAY_3_PIN = 5;


// Logical Pin Mappings

int STATE_1_HOLD_LED_PIN    = LED_1_PIN;
int STATE_2_INFLATE_LED_PIN = LED_2_PIN;
int STATE_3_EXHAUST_LED_PIN = LED_3_PIN;
int STATE_4_VACUUM_LED_PIN  = LED_4_PIN;

int VALVE_1_SUPPLY_RELAY_PIN  = RELAY_1_PIN;
int VALVE_2_BALLOON_RELAY_PIN = RELAY_2_PIN;
int VALVE_3_DEFLATE_RELAY_PIN = RELAY_3_PIN;


int STATE_1_HOLD    = 100;
int STATE_2_INFLATE = 200;
int STATE_3_EXHAUST = 300;
int STATE_4_VACUUM  = 400;

int current_state = STATE_1_HOLD;









// LLTimeDate::time trigger_datetime       = LLTimeDate::create_time_with_with_seconds(20, 6, 1,   14, 30, 0);
LLTimeDate::time trigger_datetime       = LLTimeDate::create_time_with_with_seconds(20, 11, 8,   9, 30, 0);


// Small Red A-0: 12 sec inflate.
// Small Orange A-1: 25 sec inflate.

int inflation_duration = 7;

const int DEFAULT_INFLATION_DURATION = 93;

LLTimeDate::time inflation_end_datetime = LLTimeDate::copy_time(trigger_datetime);



void updateInflationEndTime() {

    LLTimeDate::mutate_to_time(inflation_end_datetime, trigger_datetime);
    LLTimeDate::mutate_add_seconds(inflation_end_datetime, inflation_duration, 23);
}

void setInflationDurationSeconds(int s) {
    inflation_duration = s;
}

void addInflationDuration(int s) {
    setInflationDurationSeconds(inflation_duration + s);
}

void subInflationDuration(int s) {
    int new_inflation_duration = inflation_duration - s;

    if (new_inflation_duration < 1) {
        new_inflation_duration = 1;
    }

    setInflationDurationSeconds(new_inflation_duration);
}

int getInflationDuration() {
    return inflation_duration;
}

// const int PINtoRESET = 6;


void setup() {


    // pinMode(PINtoRESET, INPUT);    // Just to be clear, as default is INPUT. Not really needed.
    // digitalWrite(PINtoRESET, LOW);


    // digitalWrite(PINtoRESET, HIGH);
    // delay(200); 
    // pinMode(PINtoRESET, OUTPUT);  


    // put your setup code here, to run once:

    Serial.begin(9600);
    
    // Wait until Serial port is open.
    // while(!Serial);

    Serial.print(F("Hello World!"));



    rtc.begin();

    // Always assume 24-hour mode.
    rtc.set24Hour();
    // rtc.set12Hour();
    

    // Comment out when not needed. To observe the long-term accuracy of the RTC component,
    // we should not set the time every compilation.
    // rtc.autoTime();



    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
    // pinMode(BUTTON_2_PIN, INPUT_PULLUP);
    // pinMode(BUTTON_3_PIN, INPUT_PULLUP);

    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
    pinMode(LED_4_PIN, OUTPUT);

    pinMode(RELAY_1_PIN, OUTPUT);
    pinMode(RELAY_2_PIN, OUTPUT);
    pinMode(RELAY_3_PIN, OUTPUT);


  LLServer::setup();

}

    deenergize_all_valves();
    
    // trigger_datetime 

    
    // LLTimeDate::setup();

    // It was found that a 2-second delay is needed before starting the OLED display, 
    // or it won't turn on.
    delay(2000);
    LLDisplay::setup();

    // display.drawChar(0, 0, 'A', SSD1306_WHITE, SSD1306_BLACK, 1);
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.print(F("Hello World!"));
    display.clearDisplay();


    // display.display();

    // Explicitly perform the first update of inflation end time.
    updateInflationEndTime();


    // Accoustics
    Serial1.begin(9600);
    Serial1.println("Beginning my custom my serial connection!");

    // SAMD21 Timer
    samd21timer_setup();
    setOnSame21TimerInterrupt(interruptTimerLoop);

}



LLTimeDate::time current_datetime;

int leftAlignText(int strLen, int maxWidthPixels, int charPeriodPixels) {

    int strWidthPixels = strLen * charPeriodPixels;
    int remainingPixels = maxWidthPixels - strWidthPixels;

    if (remainingPixels > 0) {
        return remainingPixels;
    } else {
        return 0;
    }

}

int leftAlignTextOLED(int strLen) {
    return leftAlignText(strLen, SCREEN_WIDTH, 6);
}



long millisNextReset = -1;



void updateTimeDisplay() {


    // display.clearDisplay();


    // display.setCursor(0, 0);
    // display.setTextColor(SSD1306_WHITE);
    // display.setTextSize(1);
    




    // display.setCursor(0, 0);

    // unsigned long time = millis();

    // display.print(F("Hello World:"));

    // display.setCursor(0, 15);
    // display.print(time);


    // char       current_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
    // char       trigger_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
    // char inflation_end_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];

    // LLTimeDate::time_to_string_friendly(      current_time_string, sizeof(current_time_string),       current_datetime,       false);

    // display.setCursor(0, 30);
    // display.print(current_time_string);


    // display.display();








    // End of test.





    char       current_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
    char       trigger_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
    char inflation_end_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];


    LLTimeDate::time_to_string_friendly(      current_time_string, sizeof(current_time_string),       current_datetime,       false);
    LLTimeDate::time_to_string_friendly(      trigger_time_string, sizeof(trigger_time_string),       trigger_datetime,       false);
    LLTimeDate::time_to_string_friendly(inflation_end_time_string, sizeof(inflation_end_time_string), inflation_end_datetime, false);
    
    String current_state_string      = currentStateString();
    


    int MIN_Y_OFFSET = 9;

    display.clearDisplay();

    int lineCursorY = 0;

    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    




    display.setCursor(0, lineCursorY);
    display.print(F("NOW:"));


    display.setCursor(leftAlignTextOLED(strlen(current_time_string)), lineCursorY);
    display.print(current_time_string);


    lineCursorY += MIN_Y_OFFSET;
    lineCursorY += 3;

    display.setCursor(0, lineCursorY);
    display.print(F("TRG:"));



    display.setCursor(leftAlignTextOLED(strlen(trigger_time_string)), lineCursorY);
    display.print(trigger_time_string);

    
    lineCursorY += MIN_Y_OFFSET;
    lineCursorY += 2;

    display.drawFastHLine(0, lineCursorY, SCREEN_WIDTH, SSD1306_WHITE);
    lineCursorY += 7;




    if (millisNextReset > 0) {

        display.setCursor(0, lineCursorY);
        display.print(F("Preparing to reset..."));

    } else {


        if (LLWiFi::isConnected()) {
            display.setCursor(0, lineCursorY);
            display.print(F("IP:   "));
            display.print(LLWiFi::connectionIpAddress());
        } else {

            display.setCursor(0, lineCursorY);
            display.print(LLWiFi::myStatus());
        }

    }


    lineCursorY += MIN_Y_OFFSET;
    lineCursorY += 1;


    const int debug_wifi_status = 0;
    // const int debug_wifi_status = 1;

    if (debug_wifi_status) {
        // In lieu of printing wifi network name, print the
        // exact wifi connection status.
        display.setCursor(0, lineCursorY);
        display.print(LLWiFi::connectionWifiStatus());
    } else {
        if (LLWiFi::isConnected()) {

            display.setTextWrap(false);
            display.setCursor(0, lineCursorY);
            display.print(LLWiFi::connectionStrength());
            display.print(F("  "));
            display.print(LLWiFi::connectionNetworkName());
            display.setTextWrap(true);
        }
    }


    
    
    




    lineCursorY += MIN_Y_OFFSET;
    lineCursorY += 5;

    display.setCursor(0, lineCursorY);
    display.print(current_state_string);




    
    String freeMemString = String(freeMemory());
    display.setCursor(9*6, lineCursorY);
    display.print(freeMemString);
    display.print(F("b"));


    char inflation_duration_string[50];
    snprintf(inflation_duration_string, sizeof(inflation_duration_string), "%ds", inflation_duration);
    display.setCursor(leftAlignTextOLED(strlen(inflation_duration_string)), lineCursorY);
    display.print(inflation_duration_string);


    display.display();





    
}


String currentStateString() {

    if (current_state == STATE_1_HOLD) {
        return F("HOLD");
    } else if (current_state == STATE_2_INFLATE) {
        return F("INFLATE");
    } else if (current_state == STATE_3_EXHAUST) {
        return F("EXHAUST");
    } else if (current_state == STATE_4_VACUUM) {
        return F("VACUUM");
    } else {
        return F("UNRECONGIZED");
    }

}












int pendingServerRestart = 1;











int stateLastSecond = -1;
int lastState = -1;





int       stateWiFi_LastProgramSecond = -1;

IPAddress stateWiFi_LastIPAddress     = WiFi.localIP();
char *    stateWiFi_LastNetworkName   = NULL;


int lastServerStarted = 0;
int serverStarted     = 0;

String accousticsMessage;


// IPAddress stateWiFi_LastIPAddress     = NULL;

char * lastMyWiFiStatus = NULL;









void refreshLoop() {



  LLWiFi::loop();
  LLServer::loop();


    /*****  Act on Current State  *****/


    if (current_state == STATE_1_HOLD) {
        handle_state_1_hold();
    } else if (current_state == STATE_2_INFLATE) {
        handle_state_2_inflate();
    } else if (current_state == STATE_3_EXHAUST) {
        handle_state_3_exhaust();
    } else if (current_state == STATE_4_VACUUM) {
        handle_state_4_vacuum();
    } else {
        Serial.print(F("Unrecognized State!"));
    }


    /*****  End Act on Current State  *****/














    /*****  Compute New State from Sensors, Parameters and other Input, to update Current State.  *****/



    // Sparkfun RTC Routine, to update the lastSecond, and current_datetime parameters.

    static int8_t lastSecond = -1;
    
    rtc.update();

    if (rtc.second() != lastSecond) // If the second has changed
    {

        current_datetime = LLTimeDate::get_current_time();


        // // Check if we need to reset the arduino.
        // if (millisNextReset > 0 && millis() > millisNextReset) {
        //     resetFunc();
        // }
        
        lastSecond = rtc.second(); // Update lastSecond value
    }






    /*****  Detect for a state change, and then update the OLED display if that has happened.  *****/

    // Deprecated state change detection. Always update time display.
    // if (rtc.second() != stateLastSecond || lastState != current_state || lastMyWiFiStatus != LLWiFi::myStatus()) {
    //     // updateTimeDisplay();
    //     lastState = current_state;
    //     stateLastSecond = rtc.second();
    //     lastMyWiFiStatus = LLWiFi::myStatus();
    // }

    // // Nominal invokation.
    updateTimeDisplay();


    // Only inflate if:
    // current_time >= trigger_datetime AND
    // current_time <  inflation_end_datetime

    if ((compare_time(current_datetime, trigger_datetime)       >= 0) && 
            (compare_time(current_datetime, inflation_end_datetime) < 0)) {

                current_state = STATE_2_INFLATE;

    } else {

        current_state = STATE_1_HOLD;

    }




    if (read_button_1()) {
        Serial.print(F("BUTTON 1 DOWN"));
        current_state = STATE_2_INFLATE;
    }

    




}


void interruptTimerLoop() {
    refreshLoop();
    // Serial.println("interruptTimerLoop");
}

void loop() {



    // LLWiFi::refreshStatuses();




    // Wifi Routine, get state.

    long thisWiFiMillis = millis();
    long thisWiFiSecond = thisWiFiMillis / 1000;
    if (thisWiFiSecond != stateWiFi_LastProgramSecond) {

        // if (thisWiFiSecond % (60 * 5) == 10) {
        if (thisWiFiSecond % (15) == 10) {

            Serial.println(F("Every 60 seconds"));

            unsigned long start = micros();

            if (!LLWiFi::isConnectedQuery()) {
                LLWiFi::setStatusSearching();
                // // Nominal invokation. Testing new update frequency logic.
                // updateTimeDisplay();
                LLWiFi::tryConnect();
            } else {
                Serial.print(F("Already connected to '"));
                Serial.print(LLWiFi::connectionNetworkName());
                Serial.print(F("' at ip address "));
                Serial.print(LLWiFi::connectionIpAddress());
                Serial.println();
            }

            if (LLWiFi::isConnectedQuery()) {
                if (pendingServerRestart) {
                    Serial.println(F("Starting Server..."));
                    LLWiFi::server.begin();                           // start the web server on port 80
                    printWifiServerStatus();
                    pendingServerRestart = 0;
                }
            } else {
                // Let the server restart on next successful connection.
                pendingServerRestart = 1;
            }


            unsigned long end = micros();
            unsigned long delta = end - start;
            Serial.println(delta / 1000.0);


            LLWiFi::printWifiStatus();

            
            Serial.println();

        }


        stateWiFi_LastProgramSecond = thisWiFiSecond;

    }










    
    
    /*****  End Compute New State from Sensors, Parameters and other Input, to update Current State.  *****/













    /* Server Loop */
    // LLWiFi::refreshStatuses();
    server_loop();
    LLWiFi::refreshStatuses();


    
    


    // Accoustics
    if (Serial1.available()) {
        accousticsMessage = Serial1.readStringUntil('\n');

        // Arduino 1.0 and later, String.trim() trims string in place.
        accousticsMessage.trim();

        // mySerialConnection.print("Received message: ");
        // mySerialConnection.println(message);

        Serial.print("Received accoustics message: ");
        Serial.println(accousticsMessage);

        // Serial1.print("Rec. acc. mess.: ");
        // Serial1.println(accousticsMessage);
        

        Serial.print("Received accoustics message Length: ");
        Serial.println(accousticsMessage.length());
        if (accousticsMessage == "RELEASE") {

            char       current_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
            char       trigger_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
            char inflation_end_time_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];



            Serial1.println("Release triggered accoustically!!");
            Serial.println("Release triggered accoustically!!");
            LLTimeDate::mutate_to_now(trigger_datetime);
            updateInflationEndTime();

            
            LLTimeDate::time_to_string_friendly(      current_time_string, sizeof(current_time_string),       current_datetime,       false);
            LLTimeDate::time_to_string_friendly(      trigger_time_string, sizeof(trigger_time_string),       trigger_datetime,       false);
            LLTimeDate::time_to_string_friendly(inflation_end_time_string, sizeof(inflation_end_time_string), inflation_end_datetime, false);


            Serial1.print("Curr  Time: ");
            Serial1.println(current_time_string);
            Serial1.print("Trig Start: ");
            Serial1.println(trigger_time_string);
            Serial1.print("Trig   End: ");
            Serial1.println(inflation_end_time_string);
            
        } else if (accousticsMessage == "ACK") {
            Serial1.println("ACKNOWLEDGED BY LL DEVICE");
            Serial.println("ACKNOWLEDGED BY LL DEVICE");

        } else {
            // Serial1.println("Acc. command not recognized.");
            // Serial.println("Accoustic command not recognized.");
        }
    }


    // refreshLoop();


}

































int read_button_1() {
    return read_button_pin(BUTTON_1_PIN);
}

// int read_button_2() {
//     return read_button_pin(BUTTON_2_PIN);
// }

// int read_button_3() {
//     return read_button_pin(BUTTON_3_PIN);
// }

int read_button_pin(int button_pin) {
    return !digitalRead(button_pin);
}


void handle_state_1_hold() {
    isolate_led_on(STATE_1_HOLD_LED_PIN);
    deenergize_valve(VALVE_1_SUPPLY_RELAY_PIN);
    deenergize_valve(VALVE_2_BALLOON_RELAY_PIN);
    deenergize_valve(VALVE_3_DEFLATE_RELAY_PIN);
}

void handle_state_2_inflate() {
    isolate_led_on(STATE_2_INFLATE_LED_PIN);
    energize_valve(VALVE_1_SUPPLY_RELAY_PIN);
    deenergize_valve(VALVE_2_BALLOON_RELAY_PIN);
    deenergize_valve(VALVE_3_DEFLATE_RELAY_PIN);
}

void handle_state_3_exhaust() {
    isolate_led_on(STATE_3_EXHAUST_LED_PIN);
    deenergize_valve(VALVE_1_SUPPLY_RELAY_PIN);
    energize_valve(VALVE_2_BALLOON_RELAY_PIN);
    energize_valve(VALVE_3_DEFLATE_RELAY_PIN);
}

void handle_state_4_vacuum() {
    isolate_led_on(STATE_4_VACUUM_LED_PIN);
    deenergize_valve(VALVE_1_SUPPLY_RELAY_PIN);
    energize_valve(VALVE_2_BALLOON_RELAY_PIN);
    deenergize_valve(VALVE_3_DEFLATE_RELAY_PIN);
}


void isolate_led_on(int led_pin) {

    digitalWrite(LED_1_PIN, led_pin == LED_1_PIN);
    digitalWrite(LED_2_PIN, led_pin == LED_2_PIN);
    digitalWrite(LED_3_PIN, led_pin == LED_3_PIN);
    digitalWrite(LED_4_PIN, led_pin == LED_4_PIN);

}


void deenergize_all_valves() {

    deenergize_valve(VALVE_1_SUPPLY_RELAY_PIN);
    deenergize_valve(VALVE_2_BALLOON_RELAY_PIN);
    deenergize_valve(VALVE_3_DEFLATE_RELAY_PIN);

}

void energize_valve(int valve_pin) {
    set_valve_energization(valve_pin, HIGH);
}

void deenergize_valve(int valve_pin) {
    set_valve_energization(valve_pin, LOW);
}

void set_valve_energization(int valve_pin, int energization) {
    digitalWrite(valve_pin, !energization);
}






void printWifiServerStatus() {
    // print the SSID of the network you're attached to:
    Serial.print(F("SSID: "));
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print(F("IP Address: "));
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print(F("signal strength (RSSI):"));
    Serial.print(rssi);
    Serial.println(F(" dBm"));
    // print where to go in a browser:
    Serial.print(F("To see this page in action, open a browser to http://"));
    Serial.println(ip);
}



void server_loop() {
    LLWiFi::client = LLWiFi::server.available();

    if (LLWiFi::client) {                           // if you get a client,
        Serial.println(F("new client"));               // print a message out the serial port
        String currentLine = F("");                    // make a String to hold incoming data from the client
        int clientNeverConnected = true;
        while (LLWiFi::client.connected()) {        // loop while the client's connected
            clientNeverConnected = false;
            if (LLWiFi::client.available()) {       // if there's bytes to read from the client,
                char c = LLWiFi::client.read();     // read a byte, then
                Serial.write(c);                    // print it out the serial monitor
                if (c == '\n') {                    // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        LLWiFi::client.println(F("HTTP/1.1 200 OK"));
                        LLWiFi::client.println(F("Content-type:text/html"));
                        LLWiFi::client.println(F("Cache-Control: no-cache"));
                        LLWiFi::client.println();





                        // the content of the HTTP response follows the header:

                        // LLWiFi::client.print(




// Minify with: http://minifycode.com/html-minifier/
// From source document, find and replace:    ^|$|`    with:    "


char       current_datetime_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
char       trigger_datetime_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];
char inflation_end_datetime_string[LLTimeDate::MAX_TIME_TO_STRING_LEN];

LLTimeDate::time_to_string_friendly(      current_datetime_string, sizeof(current_datetime_string),       current_datetime,       true );
LLTimeDate::time_to_string_friendly(      trigger_datetime_string, sizeof(trigger_datetime_string),       trigger_datetime,       true );
LLTimeDate::time_to_string_friendly(inflation_end_datetime_string, sizeof(inflation_end_datetime_string), inflation_end_datetime, false);


LLWiFi::client.print(F("<!DOCTYPE html><html><head> <script>const fromServer=window.location.protocol=='http:';if(window.location.pathname!='/'&&fromServer){window.location.replace('/');} const oldScrollX=sessionStorage.getItem('scrollX');const oldScrollY=sessionStorage.getItem('scrollY');window.addEventListener('load',()=>{window.scrollTo(oldScrollX,oldScrollY);if(!fromServer){document.querySelectorAll('.shorten').forEach(el=>el.innerHTML='12 APR 22:22<span class=timesec>:22</span>');} document.addEventListener('scroll',_=>{sessionStorage.setItem('scrollX',window.pageXOffset);sessionStorage.setItem('scrollY',window.pageYOffset);});});</script> <meta name='viewport' content='width=600'><style>html{box-sizing:border-box}*,*:before,*:after{box-sizing:inherit}body{text-align:center;font-family:sans-serif;background-color:lightgray}.butt{border:2px solid gray;border-radius:10px;display:inline-block;padding:10px 20px;margin:20px 10px;text-decoration:none;background-color:lightgray;color:black;font-weight:bold;font-size:24px;vertical-align:middle}.title{font-weight:bold;font-size:28px;margin-bottom:20px}.state{margin:30px auto;border:4px solid black;padding:40px 0;border-radius:20px;width:560px;background-color:white}.state-val-holder{padding:20px 40px;border:8px solid #de5021;width:max-content;margin:30px auto;border-radius:20px;background-color:rgb(183,240,183)}.state-val{font-size:48px;font-weight:bold}.state-val .timesec{font-size:24px}.currtime-val{font-size:28px;padding-bottom:10px;font-weight:bold}.currtime-val .timesec{font-size:18px}.timesec{color:#888;font-style:italic;display:none}.currtime-val,.state-val,.infl-endtime{white-space:pre-wrap;font-family:monospace}.currtime-statebox{padding-top:20px;padding-bottom:20px}.currtime-holder{display:inline-block;vertical-align:middle;margin-right:50px}.currtime-label{text-transform:uppercase;font-size:16px;margin-bottom:10px;display:inline-block}.wordmark{font-size:64px;font-weight:bold;color:#de5021;padding:10px;margin-top:40px;margin-bottom:20px}.wordmark-smc{font-size:54px;text-transform:uppercase}.infl-endtime{font-size:18px}</style></head><body><div class='wordmark'>L<span class='wordmark-smc'>obster</span>L<span class='wordmark-smc'>ift</span></div><div class='state currtime-statebox'> <span class='currtime-holder'><div class='currtime-label'>CURRENT TIME</div><div class='currtime-val shorten'>"));
LLWiFi::client.print(current_datetime_string);
LLWiFi::client.print(F("</div> </span> <a class='butt' href='/refresh_'>Refresh</a></div><div class='state'><div class='title'>TRIGGER DATE & TIME:</div> <a class='butt' href='/t_add30_'>+ 30 min</a> <a class='butt' href='/t_add10_'>+ 10 min</a> <a class='butt' href='/t_add1_'>+ 1 min</a><div class='state-val-holder'><div class='state-val shorten'>"));
LLWiFi::client.print(trigger_datetime_string);
LLWiFi::client.print(F("</div></div> <a class='butt' href='/t_sub30_'>&minus; 30 min</a> <a class='butt' href='/t_sub10_'>&minus; 10 min</a> <a class='butt' href='/t_sub1_'>&minus; 1 min</a> <br> <br> <a class='butt' href='/t_hourFromNow_'>1 hour from now</a></div> <br><div class='state'><div class='title'>INFLATION:</div> <a class='butt' href='/i_add30_'>+ 30 sec</a> <a class='butt' href='/i_add10_'>+ 10 sec</a> <a class='butt' href='/i_add1_'>+ 1 sec</a><div class='state-val-holder'><div class='state-val shorten'>"));
LLWiFi::client.print(String(inflation_duration) );
LLWiFi::client.print(F(" seconds</div><div class='infl-endtime shorten'>"));
LLWiFi::client.print(inflation_end_datetime_string);
LLWiFi::client.print(F("</div></div> <a class='butt' href='/i_sub30_'>&minus; 30 sec</a> <a class='butt' href='/i_sub10_'>&minus; 10 sec</a> <a class='butt' href='/i_sub1_'>&minus; 1 sec</a> <br> <br> <a class='butt' href='/i_default_'>DEFAULT</a></div><br><br><br><br><br><br><br></body></html>"));



// <a class='butt' href='/c_reset_'>RESET</a><br><br><br><br>











                        // The HTTP response ends with another blank line:
                        LLWiFi::client.println();
                        // break out of the while loop:
                        break;
                    } else {    // if you got a newline, then clear currentLine:
                        currentLine = F("");
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }

                // Check to see if the client request was "GET /H" or "GET /L":

                int triggerHourLimit = 22; // To prevent overflown end trigger time crossing from 23 to 0.

                if (currentLine.endsWith(F("GET /t_add30_"))) {
                    Serial.println(F("Path: t_add30_"));
                    LLTimeDate::mutate_add_minutes(trigger_datetime, 30, triggerHourLimit);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /t_add10_"))) {
                    Serial.println(F("Path: t_add10_"));
                    LLTimeDate::mutate_add_minutes(trigger_datetime, 10, triggerHourLimit);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /t_add1_"))) {
                    Serial.println(F("Path: t_add1_"));
                    LLTimeDate::mutate_add_minutes(trigger_datetime, 1, triggerHourLimit);
                    updateInflationEndTime();
                }

                if (currentLine.endsWith(F("GET /t_hourFromNow_"))) {
                    Serial.println(F("Path: t_hourFromNow_"));

                    LLTimeDate::mutate_to_now(trigger_datetime);
                    LLTimeDate::mutate_add_minutes(trigger_datetime, 60, triggerHourLimit);
                    trigger_datetime.second = 0;
                    updateInflationEndTime();
                }


                if (currentLine.endsWith(F("GET /t_sub30_"))) {
                    Serial.println(F("Path: t_sub30_"));
                    LLTimeDate::mutate_sub_minutes(trigger_datetime, 30);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /t_sub10_"))) {
                    Serial.println(F("Path: t_sub10_"));
                    LLTimeDate::mutate_sub_minutes(trigger_datetime, 10);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /t_sub1_"))) {
                    Serial.println(F("Path: t_sub1_"));
                    LLTimeDate::mutate_sub_minutes(trigger_datetime, 1);
                    updateInflationEndTime();
                }




                if (currentLine.endsWith(F("GET /i_add30_"))) {
                    Serial.println(F("Path: i_add30_"));
                    addInflationDuration(30);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /i_add10_"))) {
                    Serial.println(F("Path: i_add10_"));
                    addInflationDuration(10);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /i_add1_"))) {
                    Serial.println(F("Path: i_add1_"));
                    addInflationDuration(1);
                    updateInflationEndTime();
                }

                if (currentLine.endsWith(F("GET /i_default_"))) {
                    Serial.println(F("Path: i_default_"));
                    setInflationDurationSeconds(DEFAULT_INFLATION_DURATION);
                    updateInflationEndTime();
                }


                if (currentLine.endsWith(F("GET /i_sub30_"))) {
                    Serial.println(F("Path: i_sub30_"));
                    subInflationDuration(30);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /i_sub10_"))) {
                    Serial.println(F("Path: i_sub10_"));
                    subInflationDuration(10);
                    updateInflationEndTime();
                }
                if (currentLine.endsWith(F("GET /i_sub1_"))) {
                    Serial.println(F("Path: i_sub1_"));
                    subInflationDuration(1);
                    updateInflationEndTime();
                }



                if (currentLine.endsWith(F("GET /c_reset_"))) {
                    Serial.println(F("Path: c_reset_"));
                    // resetFunc();
                    millisNextReset = millis() + 3000;
                }

            }
        }
        // close the connection:
        LLWiFi::client.stop();
        Serial.println(F("client disonnected"));
        if (clientNeverConnected) {
            Serial.println("ClientNeverConnected, trying to restart server...");
            // LLWiFi::server.begin();
            LLWiFi::tryRestart();

            // LLWiFi::server.begin();
            // LLWiFi.end();
            // LLWiFi::tryConnect();
        }
    }
    
    // else {
    //     // Serial.println(F("No client.")); //
    //     Prints a lot of output!!
    // }

}



