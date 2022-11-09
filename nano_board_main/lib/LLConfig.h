#ifndef LLCONFIG_H
#define LLCONFIG_H











namespace LLConfig {

    enum modem_type {
        MODEM_TYPE_TELEDYNE,
        MODEM_TYPE_SUB_SEA_SONICS,
        MODEM_TYPE_UNRECOGNIZED
    };

    enum modem_type CONFIG_MODEM_TYPE = MODEM_TYPE_TELEDYNE;

    char MODEM_TYPE_TELEDYNE_STRING[] = "TELEDYNE";
    char MODEM_TYPE_SUB_SEA_SONICS_STRING[] = "SUB_SEA_SONICS";
    char MODEM_TYPE_UNRECOGNIZED_STRING[] = "UNRECOGNIZED_MODEM_TYPE";

    char *modem_type_to_string(enum modem_type m) {
        switch (m) {
            case MODEM_TYPE_TELEDYNE:
                return MODEM_TYPE_TELEDYNE_STRING;
            case MODEM_TYPE_SUB_SEA_SONICS:
                return MODEM_TYPE_SUB_SEA_SONICS_STRING;
            default:
                return MODEM_TYPE_UNRECOGNIZED_STRING;
        }
    }

    enum modem_type string_to_modem_type(const char *s) {
        if (strcmp(s, MODEM_TYPE_TELEDYNE_STRING) == 0) {
            return MODEM_TYPE_TELEDYNE;
        } else if (strcmp(s, MODEM_TYPE_SUB_SEA_SONICS_STRING) == 0) {
            return MODEM_TYPE_SUB_SEA_SONICS;
        } else {
            return MODEM_TYPE_UNRECOGNIZED;
        }
    }

    int numSDConfigTries = 1;
    bool config_setup_successful = false;

    void setup();

}



#endif
