#ifndef LLDATETIME_H
#define LLDATETIME_H

#include "LLAppState.h"


// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"





/*

Dealing with Time.

LLDateTime SHALL define an opaque DateTime format, called LLTime.

All time MANIPULATION AND LOGIC must be performed THROUGH the library, 
in order to protect from 2038 (signed 32 bit integer), 2106 (unsigned 32 bit integer)
time problems.

Internally, the opaque format must be at least (may include other data attributes),
a 64-bit signed integer.

Access to the external RTClib.h library must be limited, and swappable.

We'll only use it to retrieve the current unix time, with the understanding that
its return value is an unsigned 32-bit integer, and we CAST that value to our
internal 64-bit signed integer implementation immediately, before proceeding forward.

We currently know, RTClib, whether due to its impelmentation, or DS3231 hardware or both,
fails at around the 2106 time problem point.

We'll code with the understanding that RTClib, and the RTC hardware DS3231 may be swapped out
for another library and hardware combination that does not face this problem. 

*/



/*

Internal notes on the implementation of LLTime data type:

Shall be a 64-bit signed integer representing the number of milliseconds after the unix epoch.

This representation shall be called MILLIS_UNIX_64BIT, M64.

*/







RTC_DS3231 rtc;

// char daysOfTheWeek[7][12];

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeekShort[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// Note: Understand whether the month value is zero- or 1-indexed before indexing any of these month arrays.
// Note: RTClib.h month values are 1-indexed, so subtract the returned month value by 1 before indexing!
char months[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char monthsShort[12][10] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char monthsShortCaps[12][4] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

char amPm[2][3] = { "am", "pm" };
char amPmCaps[2][3] = { "AM", "PM" };

#define FORCE_SET_TIME 0




namespace LLDateTime {

    const int MAX_REPRESENTATION_TYPE_STRING_LENGTH = 5;
    const int MAX_REPRESENTATION_TYPE_BUFFER_LENGTH = MAX_REPRESENTATION_TYPE_STRING_LENGTH + 1;


    typedef int64_t _ll_timestamp;

    typedef struct {
        _ll_timestamp millis_unix_64bit;
        char representation[MAX_REPRESENTATION_TYPE_BUFFER_LENGTH] = "NULL";  
    } LLTime;

    enum TimeZone {
        America__New_York
    };

    void setup();

    String hello();

    const size_t MAX_TIME_STRING_LENGTH = 180;
    const size_t MAX_TIME_BUFFER_LENGTH = MAX_TIME_STRING_LENGTH + 1;


    char* currentShortTimeString();
    char* currentLongTimeString();

    void shortString(LLTime time_llt, char *c, size_t buffer_len);
    void longString(LLTime time_llt, char *c, size_t buffer_len);

    
    void shortString(LLTime time_llt, char *c);
    void longString(LLTime time_llt, char *c);

    int64_t bluetoothRepresentation(LLTime time_llt);
    LLTime fromBluetoothRepresentation(int64_t);

    LLTime now();
    void setCurrentTime(LLTime time);
    LLTime fromOffset(LLTime source, int64_t millis_offset);

    bool isNull(LLTime time_llt);

}




#endif
