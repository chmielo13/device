

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "LLDisplay.h"

#include "LLAppState.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    -1 // Originally: 4;  Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

namespace LLDisplay {


    static const unsigned char PROGMEM logo_bmp[] =
    { B00000000, B11000000,
        B00000001, B11000000,
        B00000001, B11000000,
        B00000011, B11100000,
        B11110011, B11100000,
        B11111110, B11111000,
        B01111110, B11111111,
        B00110011, B10011111,
        B00011111, B11111100,
        B00001101, B01110000,
        B00011011, B10100000,
        B00111111, B11100000,
        B00111111, B11110000,
        B01111100, B11110000,
        B01110000, B01110000,
        B00000000, B00110000 };



    // 'logo-monochrome-6', 128x20px
    static const unsigned char PROGMEM lobsterlift_logo_bmp[] = { 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x03, 0x80, 0x1f, 0x83, 0xf8, 0x3f, 0x1f, 0xf3, 0xfc, 0xff, 0x0e, 0x00, 0xe3, 0xfc, 0xff, 0x80, 
        0x03, 0x80, 0x3f, 0xc3, 0xfc, 0x7f, 0x9f, 0xf3, 0xfc, 0xff, 0x8e, 0x00, 0xe3, 0xfc, 0xff, 0x80, 
        0x03, 0x80, 0x79, 0xe3, 0x9c, 0x73, 0x83, 0x83, 0x80, 0xe3, 0x8e, 0x00, 0xe3, 0x80, 0x1c, 0x00, 
        0x03, 0x80, 0x70, 0xe3, 0x9c, 0x78, 0x03, 0x83, 0x80, 0xe3, 0x8e, 0x00, 0xe3, 0x80, 0x1c, 0x00, 
        0x03, 0x80, 0x70, 0xe3, 0xf8, 0x7f, 0x03, 0x83, 0xf8, 0xff, 0x0e, 0x00, 0xe3, 0xf8, 0x1c, 0x00, 
        0x03, 0x80, 0x70, 0xe3, 0xfc, 0x1f, 0x83, 0x83, 0xf8, 0xfe, 0x0e, 0x00, 0xe3, 0xf8, 0x1c, 0x00, 
        0x03, 0x80, 0x70, 0xe3, 0x8e, 0x03, 0xc3, 0x83, 0x80, 0xef, 0x0e, 0x00, 0xe3, 0x80, 0x1c, 0x00, 
        0x03, 0xfe, 0x79, 0xe3, 0x8e, 0x71, 0xc3, 0x83, 0x80, 0xe7, 0x8f, 0xf8, 0xe3, 0x80, 0x1c, 0x00, 
        0x03, 0xfe, 0x3f, 0xc3, 0xfc, 0x7f, 0x83, 0x83, 0xfc, 0xe3, 0x8f, 0xf8, 0xe3, 0x80, 0x1c, 0x00, 
        0x03, 0xfe, 0x1f, 0x83, 0xfc, 0x3f, 0x03, 0x83, 0xfc, 0xe1, 0xcf, 0xf8, 0xe3, 0x80, 0x1c, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };



    // const int LOBSTERLIFT_BITMAP_DELAY = 3000;  // How long the logo should load in ms. // Nominally 20000 for logo, (shorter, 300 during fast debugging needs). 
    const int LOBSTERLIFT_BITMAP_DELAY = 50;  // How long the logo should load in ms. // Nominally 20000 for logo, (shorter, 300 during fast debugging needs). 


    void drawLobsterLiftBitmap(void) {


        int lobtsterlift_logo_width  = 128;
        int lobtsterlift_logo_height = 20;

        display.clearDisplay();
        display.drawBitmap(
            (display.width()  - lobtsterlift_logo_width ) / 2,
            (display.height() - lobtsterlift_logo_height) / 2,
            lobsterlift_logo_bmp, lobtsterlift_logo_width, lobtsterlift_logo_height, 1);
        display.display();
        delay(LOBSTERLIFT_BITMAP_DELAY);


    }



    void setup(int splashLogo) {
        Serial.begin(9600);

        // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
            Serial.println(F("SSD1306 allocation failed"));
            for(;;); // Don't proceed, loop forever
        }

        // Show initial display buffer contents on the screen --
        // the library initializes this with an Adafruit splash screen.
        // display.display();
        // delay(2000); // Pause for 2 seconds

        // Clear the buffer
        display.clearDisplay();
        display.display();

        if (splashLogo) {
            drawLobsterLiftBitmap();
        }
        
        display.clearDisplay();
        display.display();

        // animations();


        
    }

    void loop() {
    }
    
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

    int MIN_LINE_HEIGHT = 9;

    void refreshDisplay() {
        
        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);
        
        // display.print(F("Hello World!"));
        // display.println(appState.getHelloIncrement());

        int lineCursorY = 0;

        display.setCursor(0, lineCursorY);
        display.print(F("NOW:"));

        char * current_time_string = LLDateTime::currentShortTimeString();

        display.setCursor(leftAlignTextOLED(strlen(current_time_string)), lineCursorY);
        display.print(current_time_string);

        lineCursorY += MIN_LINE_HEIGHT;
        lineCursorY += 3;

        display.setCursor(0, lineCursorY);
        display.print(F("TRG:"));

        char trigger_time_string[LLDateTime::MAX_TIME_BUFFER_LENGTH]; 
        LLDateTime::shortString(appState.getTriggerTime(), trigger_time_string);

        display.setCursor(leftAlignTextOLED(strlen(trigger_time_string)), lineCursorY);
        display.print(trigger_time_string);

        lineCursorY += MIN_LINE_HEIGHT;
        lineCursorY += 2;

        display.drawFastHLine(0, lineCursorY, SCREEN_WIDTH, SSD1306_WHITE);
        lineCursorY += 7;

        // Printing the OSD Parameter

        display.setCursor(0, lineCursorY);

        printOSDParameter(&display);

        // Printing the last line.

        int lastLineYPosition = SCREEN_HEIGHT - MIN_LINE_HEIGHT;
        lineCursorY = lastLineYPosition;
        
        display.setCursor(0, lineCursorY);
        display.print(appState.getValveState());
        
        display.setCursor(8*6, lineCursorY);
        // display.print(appState.getDerivedAppState().getFreeMemoryRemainingDisplay());
        display.print(-appState.getAveragePressure());
        // display.setCursor(14*6, lineCursorY);
        display.print(" PSI");
        
        char inflation_duration_string[10];
        snprintf(inflation_duration_string, sizeof(inflation_duration_string), "%ds", appState.getInflationDuration());
        display.setCursor(leftAlignTextOLED(strlen(inflation_duration_string)), lineCursorY);
        display.print(inflation_duration_string);

        display.display();

    }


    enum osd_parameter nextOSDParameter(enum osd_parameter current) {
        enum osd_parameter next;
        
        if (current == LAST_OSDPARAM) {
            next = FIRST_OSDPARAM;
        } else {
            next = (enum osd_parameter) (((int) current) + 1);
        }

        return next;
    }



}
