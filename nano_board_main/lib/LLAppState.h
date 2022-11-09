#ifndef LLAPPSTATE_H
#define LLAPPSTATE_H

#include "LLDateTime.h"
#include "LLBle.h"
#include "LLConfig.h"
#include "LLDisplay.h"


#include <Adafruit_SSD1306.h>

void setupState();
void updateDerivedState();


void printAppState();




void updateFreeMemoryRemainingDisplay();

char VALVE_STATE_HOLD[] = "HOLD";
char VALVE_STATE_INFLATE[] = "INFLATE";


class AppState {
    private:
        class DerivedAppState {
            private: 
                // Internally stored derived state variables.
                char freeMemoryRemainingDisplay[10];
                char* bluetoothStatusDisplay;

            public:

                char* getFreeMemoryRemainingDisplay() {
                    return freeMemoryRemainingDisplay;
                }

                char* getBluetoothStatusDisplay() {
                    return bluetoothStatusDisplay;
                }

                void updateFreeMomoryRemainingDisplay(AppState* a) {
                    char * resultBuffer = freeMemoryRemainingDisplay;
                    int remaining = a->getFreeMemoryRemaining();
                    if (remaining < 1000) {
                        // No need to output comma.
                        sprintf(resultBuffer, "%db", remaining);
                    } else {
                        // Need to output comma.
                        int digitsUnder1000 = remaining % 1000;
                        int digitsAbove1000 = remaining / 1000;
                        sprintf(resultBuffer, "%d,%03db", digitsAbove1000, digitsUnder1000);
                    }
                }

                void updateBluetoothStatusDisplay(AppState* a) {
                    bluetoothStatusDisplay = bleStatusString(a->getBluetoothStatus());
                }

                void updateDerivedAppState(AppState* a) {
                    updateFreeMomoryRemainingDisplay(a);
                    updateBluetoothStatusDisplay(a);
                }
        };

        // Internally stored app state variables.
        int helloIncrement = 15;
        bool pendingIncrement = 0;
        int rtcReprogrammed = -1; // Unknown
        LLDateTime::LLTime triggerTime;
        bool triggerTimeEnabled = 1;
        int inflationDuration = 15;   // Important: This is in seconds.
        bool inflationDurationEnabled = 1;
        int freeMemoryRemaining;
        BleStatus bluetoothStatus = BLE_STATUS_UNKNOWN;
        char * valveState = VALVE_STATE_HOLD;
        bool timeReady = false;
        float averagePressure = 0;
        enum LLConfig::modem_type modemType = LLConfig::MODEM_TYPE_TELEDYNE;
        enum LLDisplay::osd_parameter osdParameter = LLDisplay::OSDPARAM_SDCONFIG_TRIES;


        DerivedAppState _derivedAppState;
        
        int _pendingUpdateDerivedState;

        void setterFinished() {
            // Set pending update derived state, to indicate that
            // the derived state is now stale due to a recent app state change.
            _pendingUpdateDerivedState = 1;
        }

        void updateDerivedAppState() {
            _derivedAppState.updateDerivedAppState(this);
        }

    public:

        // App State Constructor.
        AppState() {
            _derivedAppState = DerivedAppState();
            _pendingUpdateDerivedState = 1;
        }

        // Setters (must include to call to setterFinished() at the end of each setter method)

        void setHelloIncrement(int value) {
            helloIncrement = value;
            setterFinished();
        }

        void setPendingIncrement(bool value) {
            pendingIncrement = value;
            setterFinished();
        }

        void setRtcReprogrammed(int value) {
            rtcReprogrammed = value;
            setterFinished();
        }

        void setTriggerTime(LLDateTime::LLTime value) {
            triggerTime = value;
            triggerTimeCharacteristic.writeValue(LLDateTime::bluetoothRepresentation(value));
            setterFinished();
        }

        void setTriggerTimeEnabled(bool value) {
            triggerTimeEnabled = value;
            setterFinished();
        }

        void setInflationDuration(int value) {
            inflationDuration = value;
            inflationDurationCharacteristic.writeValue(value);
            setterFinished();
        }

        void setInflationDurationEnabled(bool value) {
            inflationDurationEnabled = value;
            setterFinished();
        }

        void setFreeMemoryRemaining(int value) {
            freeMemoryRemaining = value;
            setterFinished();
        }

        void setBluetoothStatus(BleStatus value) {
            bluetoothStatus = value;
            setterFinished();
        }

        void setValveState(char * value) {
            valveState = value;
            setterFinished();
        }

        void setTimeReady(boolean value) {
            timeReady = value;
            setterFinished();
        }

        void setAveragePressure(float value) {
            averagePressure = value;
            setterFinished();
        }

        void setModemType(enum LLConfig::modem_type value) {
            modemType = value;
            setterFinished();
        }

        void setOSDParameter(enum LLDisplay::osd_parameter value) {
            osdParameter = value;
            setterFinished();
        }
        

        // Getters

        int getHelloIncrement() {
            return helloIncrement;
        }

        bool getPendingIncrement() {
            return pendingIncrement;
        }

        int getRtcReprogrammed() {
            return rtcReprogrammed;
        }

        LLDateTime::LLTime getTriggerTime() {
            return triggerTime;
        }

        bool getTriggerTimeEnabled() {
            return triggerTimeEnabled;
        }

        int getInflationDuration() {
            return inflationDuration;
        }

        bool getInflationDurationEnabled() {
            return inflationDurationEnabled;
        }

        int getFreeMemoryRemaining() {
            return freeMemoryRemaining;
        }

        BleStatus getBluetoothStatus() {
            return bluetoothStatus;
        }

        char * getValveState() {
            return valveState;
        }

        bool getTimeReady() {
            return timeReady;
        }

        float getAveragePressure() {
            return averagePressure;
        }

        enum LLConfig::modem_type getModemType() {
            return modemType;
        }

        enum LLDisplay::osd_parameter getOSDParameter() {
            return osdParameter;
        }




        DerivedAppState getDerivedAppState() {
            if (_pendingUpdateDerivedState) {
                updateDerivedAppState();
                return _derivedAppState;
            }
        }

};


AppState appState; 

void printOSDParameter(Adafruit_SSD1306 *display);

#endif
