#ifndef LLDISPLAY_H
#define LLDISPLAY_H



namespace LLDisplay {


    #define FIRST_OSDPARAM OSDPARAM_SDCONFIG_TRIES
    #define LAST_OSDPARAM  OSDPARAM_MODEM_TYPE

    enum osd_parameter {
        OSDPARAM_BLE_STATUS,
        OSDPARAM_FREE_MEMORY,
        OSDPARAM_SDCONFIG_TRIES,
        OSDPARAM_MODEM_TYPE
    };

    





    void setup(int splashLogo);

    
    int leftAlignText(int strLen, int maxWidthPixels, int charPeriodPixels);

    int leftAlignTextOLED(int strLen);

    void refreshDisplay();



}


#endif

