#include "LLDateTime.h"
#include "LLAppState.h"
#include <inttypes.h>

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"


namespace LLDateTime {

    TimeZone currentTimeZone() {
        return America__New_York;
    }

    _ll_timestamp _currentUtcTimestamp() {
        DateTime currentUtcDateTime = rtc.now();
        return currentUtcDateTime.unixtime() * 1000;
    }

    _ll_timestamp _localUtcOffset() {
        if (currentTimeZone() == America__New_York) {
            _ll_timestamp standardTimeOffset = - 5 * 60 * 60 * 1000;
            _ll_timestamp daylightSavingsTimeOffset = - 4 * 60 * 60 * 1000;
            return daylightSavingsTimeOffset;
        } else {
            return 0;
        }
    }

    _ll_timestamp _localTimestampToUTCTimestamp(_ll_timestamp localTimestamp) {
        return localTimestamp - _localUtcOffset();
    }
    
    _ll_timestamp _utcTimestampToLocalTimestamp(_ll_timestamp utcTimestamp) {
        return utcTimestamp + _localUtcOffset();
    }

    // "LLTime constructor".
    LLTime _lltimeFromMillisUnixTime64BitT(_ll_timestamp millis_unix_64bit) {
        LLTime result_llt;
        result_llt.millis_unix_64bit = millis_unix_64bit;
        strncpy(result_llt.representation, "M64", MAX_REPRESENTATION_TYPE_STRING_LENGTH);
        return result_llt;
    }

    LLTime now(int64_t millis_offset) {
        _ll_timestamp utcTimeStamp = rtc.now().unixtime() * ((_ll_timestamp) 1000);
        utcTimeStamp += millis_offset;
        return _lltimeFromMillisUnixTime64BitT(utcTimeStamp);
    }

    LLTime now() {
        return now(0);
    }

    // bool isNull(LLTime time) {
    //     if (strncmp(time.representation, "NULL", MAX_REPRESENTATION_TYPE_STRING_LENGTH) == 0) {
    //         return true;
    //     } else {
    //         return false;
    //     }
    // }

    void setCurrentTime(LLTime time) {
        _ll_timestamp utcTimestamp = time.millis_unix_64bit;
        DateTime utcDateTime = DateTime(utcTimestamp / 1000);
        rtc.adjust(utcDateTime);
    }

    LLTime fromOffset(LLTime source, int64_t millis_offset) {
        LLTime copy = source;
        copy.millis_unix_64bit += millis_offset;
        return copy;
    }



    void setup() {
        if (! rtc.begin()) {
            Serial.println("Couldn't find RTC");
            Serial.flush();
            abort();
        }
            
        if (rtc.lostPower() || FORCE_SET_TIME) {
            Serial.println("RTC lost power, let's set the time!");
            // When time needs to be set on a new device, or after a power loss, the
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            // This line sets the RTC with an explicit date & time, for example to set
            // January 21, 2014 at 3am you would call:
            // rtc.adjust(DateTime(2106, 1, 21, 3, 0, 0));

            _ll_timestamp localTimestamp = DateTime(F(__DATE__), F(__TIME__)).unixtime() * ((_ll_timestamp) 1000);
            _ll_timestamp utcTimestamp = _localTimestampToUTCTimestamp(localTimestamp);

            _ll_timestamp synchronization_offset = 7.1 * 1000;

            DateTime utcDateTime = DateTime((utcTimestamp + synchronization_offset) / 1000);

            rtc.adjust(utcDateTime);


            /*
            Nominal set accurate time procedure:


                1. Set #define FORCE_SET_TIME to 1.
                2. Compile and send to Arduino. (And see that time is correctly set, but a few seconds off).
                3. Compile and send to Arduino, again. (And see that time is correctly set, but just about exact to the second).
                4. Set #define FORCE_SET_TIME to 0.
                5. Compile and send to Arduino.

            At no point should power be turned off to the Arduino, since if the Arduino loads
            again while the compiled program macro FORCE_SET_TIME is set to 1, then the Arduino
            RTC will be set to an archaic time. The Arduino is not safe to power off until it's been
            loaded with compiled program macro FORCE_SET_TIME set to 0. 
            */

            appState.setRtcReprogrammed(101);
        } else {
            Serial.println('Not reprogramming the RTC component.');
            appState.setRtcReprogrammed(100);
        }

    }

    String hello() {
        return "Hi Hi Hi...";
    }

    
    char currentShortTimeStringBuffer[MAX_TIME_BUFFER_LENGTH] = "Hello";
    char currentLongTimeStringBuffer[MAX_TIME_BUFFER_LENGTH] = "Hello";

    char* currentShortTimeString() {
        shortString(LLDateTime::now(), currentShortTimeStringBuffer, MAX_TIME_BUFFER_LENGTH);
        return currentShortTimeStringBuffer;
    }

    char* currentLongTimeString() {
        longString(LLDateTime::now(), currentLongTimeStringBuffer, MAX_TIME_BUFFER_LENGTH);
        return currentLongTimeStringBuffer;
    }

    void _longStringDT(DateTime datetime, char *c) {
        // sprintf_P(c, PSTR("Year: %d, Month: %d, Day: %d, Day of Week: %s, %d:%d:%d, Unixtime: %u"), 
        //     datetime.year(), datetime.month(), datetime.day(),
        //     daysOfTheWeek[datetime.dayOfTheWeek()],
        //     datetime.hour(), datetime.minute(), datetime.second(),
        //     datetime.unixtime()
        // );
        sprintf_P(c, PSTR("%s, %d, %d; %s, %d:%02d%s (%02d sec), Unixtime: %u"), 
            months[datetime.month()-1], datetime.day(), datetime.year(), 
            daysOfTheWeek[datetime.dayOfTheWeek()],
            datetime.twelveHour(), datetime.minute(), amPm[datetime.isPM()], datetime.second(), 
            datetime.unixtime()
        );
    }

    void _shortStringDT(DateTime datetime, char *c) {
        sprintf(c, "%d %s  %d:%02d:%02d", 
            datetime.day(), monthsShortCaps[datetime.month() - 1],
            datetime.hour(), datetime.minute(), datetime.second()
        );
    }
    
    void shortString(LLTime time_llt, char *c, size_t buffer_len) {
        _ll_timestamp local_timestamp = _utcTimestampToLocalTimestamp(time_llt.millis_unix_64bit);
        DateTime local_datetime = DateTime(local_timestamp / 1000);
        _shortStringDT(local_datetime, c);
    }

    void longString(LLTime time_llt, char *c, size_t buffer_len) {

        if (isNull(time_llt)) {
            sprintf(c, "NULL");
        } else {
            size_t tag_len = snprintf_P(c, buffer_len, PSTR("LLTime (%s, %"PRId64"): "), time_llt.representation, time_llt.millis_unix_64bit);
            size_t remainint_buffer_len = buffer_len - tag_len;

            char *new_buffer = c + tag_len;

            _ll_timestamp local_timestamp = _utcTimestampToLocalTimestamp(time_llt.millis_unix_64bit);
            DateTime local_datetime = DateTime(local_timestamp / 1000);
            _longStringDT(local_datetime, new_buffer);
        }
    }

    void shortString(LLTime time_llt, char *c) {
        shortString(time_llt, c, MAX_TIME_BUFFER_LENGTH);
    }

    void longString(LLTime time_llt, char *c) {
        longString(time_llt, c, MAX_TIME_BUFFER_LENGTH);
    }

    // Returns time_a - time_b; the typical usage is that time_a should be later than time_b.
    int64_t timeDifferenceMillis(LLTime a_llt, LLTime b_llt) {
        _ll_timestamp timestamp_a = a_llt.millis_unix_64bit;
        _ll_timestamp timestamp_b = b_llt.millis_unix_64bit;

        return int64_t(timestamp_a - timestamp_b);
        
    }

    int64_t bluetoothRepresentation(LLTime time_llt) {
        return time_llt.millis_unix_64bit;
    }

    LLTime fromBluetoothRepresentation(int64_t rep) {
        return _lltimeFromMillisUnixTime64BitT(rep);
    }

    bool isNull(LLTime time_llt) {
        if (strncmp(time_llt.representation, "NULL", MAX_REPRESENTATION_TYPE_STRING_LENGTH) == 0) {
            return true;
        }
        // Also return true if the representation string is \0.
        if (time_llt.representation[0] == 0) {
            return true;
        }
        return false;
    }

}


