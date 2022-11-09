#include <Wire.h>
#include <SparkFunDS1307RTC.h>



#define PRINT_USA_DATE

namespace LLTimeDate {


    const int MAX_TIME_TO_STRING_LEN = 50;


    typedef struct {
        int year;
        int month;
        int date;
        int hour;
        int minute;
        int second;
    } time;



    time copy_time(time initial_time) {

        time result;

        result.year   = initial_time.year;
        result.month  = initial_time.month;
        result.date   = initial_time.date;
        result.hour   = initial_time.hour;
        result.minute = initial_time.minute;
        result.second = initial_time.second;

        return result;

    }

    time get_current_time() {
        rtc.update();

        time result;

        result.year   = rtc.year();
        result.month  = rtc.month();
        result.date   = rtc.date();
        result.hour   = rtc.hour();
        result.minute = rtc.minute();
        result.second = rtc.second();

        return result;

    }




    // -1 if a < b.
    //  0 if a == b.
    //  1 if a > b.
    int compare_time(time a, time b) {

        if (a.year < b.year) {
            return -1;
        } else if (a.year > b.year) {
            return 1;
        }

        // a.year == b.year

        if (a.month < b.month) {
            return -1;
        } else if (a.month > b.month) {
            return 1;
        }

        // a.month == b.month

        if (a.date < b.date) {
            return -1;
        } else if (a.date > b.date) {
            return 1;
        }

        // a.date == b.date

        if (a.hour < b.hour) {
            return -1;
        } else if (a.hour > b.hour) {
            return 1;
        }

        // a.hour == b.hour

        if (a.minute < b.minute) {
            return -1;
        } else if (a.minute > b.minute) {
            return 1;
        }

        // a.minute == b.minute

        if (a.second < b.second) {
            return -1;
        } else if (a.second > b.second) {
            return 1;
        }

        // a.second == b.second

        // All components identical, return 0.

        return 0;

    }


    time create_time_with_with_seconds(int year, int month, int date, int hour, int minute, int second) {
        time result;

        result.year   = year;
        result.month  = month;
        result.date   = date;
        result.hour   = hour;
        result.minute = minute;
        result.second = second;

        return result;

    }


    int remainder(int dividend, int divisor) {

        // 181  / 60 = 3, remainder 1
        // -181 / 60 = -3, remainder -1

        // 181  - (60 * 3)  = 181-180     =  1;
        // -181 - (60 * -3) = -181 - -180 = -1;

        int int_quotient = dividend / divisor;

        int remainder = dividend - (divisor * int_quotient);

        return remainder;

    }

    time offset_time(time initial_time, int offset_seconds) {

        time result = copy_time(initial_time);

        // Naive offset time seconds implementation. Needs to be updated to be correct.
        // This only works when: 
        // 0 >= initial_time.seconds + offset_seconds < 60
        result.second += offset_seconds;

        return result;

        /*
            Example: Time is 15:02:35 

                + 163 seconds, expecting 

                35 + 163 = 198

                198 / 60   = 3 min
                198 rem 60 = 18 seconds

            result: 15:05:18


            Example: Time is 15:02:35

                - 163 seconds, expecting

                35 - 163 = -128

                -128 / 60   = -2 min
                -128 rem 60 = -8 seconds

            result: 15:00:-8

                -2 - 1  = - 3 min
                -8 + 60 = + 52 sec


            result: 15:-1:52

            






        */


        // result.second += offset_seconds;

        // int offset_minutes = result.second / 60;
        // result.second = result.second %




    }

    // Where year is 00-99.
    // Where month is 1-12.
    // Where hour is 1-23

    time create_time(int year, int month, int date, int hour, int minute) {
        return create_time_with_with_seconds(year, month, date, hour, minute, 0);
    }


    // mutate_truncate_seconds

    // Hour limit should be 23, or 22 to restrict triggering to 10pm.
    void mutate_add_minutes(time & t, int minutes, int hourLimit) {
        t.minute += minutes;
        
        int overHours = t.minute / 60;
        t.minute -= overHours * 60;
        t.hour   += overHours;

        if (t.hour > hourLimit) {
            t.hour = hourLimit;
        }

    }

    // Hour limit should be 23.
    void mutate_add_seconds(time & t, int seconds, int hourLimit) {

        t.second += seconds;

        int overMinutes = t.second / 60;
        t.second -= overMinutes * 60;
        t.minute += overMinutes;
        

        
        int overHours = t.minute / 60;
        t.minute -= overHours * 60;
        t.hour   += overHours;


        if (t.hour > hourLimit) {
            t.hour = hourLimit;
        }

    }

    void mutate_sub_minutes(time & t, int minutes) {
        t.minute -= minutes;

        int lessHours = 0;
        while (t.minute < 0) {
            lessHours += 1;
            t.minute  += 60;
        }

        t.hour -= lessHours;

        if (t.hour < 0) {
            t.hour = 0;
        }
    }

    void mutate_to_time(time & destination, time source) {


        destination.year   = source.year;
        destination.month  = source.month;
        destination.date   = source.date;
        destination.hour   = source.hour;
        destination.minute = source.minute;
        destination.second = source.second;
    
    }

    void mutate_to_now(time & t) {
        mutate_to_time(t, get_current_time());
    }


    void time_to_string(char * destination, int len, time a) {


        // String yearString;
        char yearString[len];
        
        // Handle 2-digit year.

        if (a.year <= 69) {
            // strncpy(yearString, "20",    sizeof(yearString) );
            snprintf(yearString, sizeof(yearString), "20%d", a.year);
        } else if (a.year <= 99) {
            // strncpy(yearString, "19",    sizeof(yearString) );
            snprintf(yearString, sizeof(yearString), "19%d", a.year);
        }
        
        // strncat(yearString, String(a.year), sizeof(yearString) );



        // String monthString;
        char monthString[len];

        if (a.month <= 9) {
            snprintf(monthString, sizeof(monthString), "0%d", a.month);
        } else {
            snprintf(monthString, sizeof(monthString), "%d", a.month);
        }

        // strncat(monthString, String(a.month), sizeof(monthString));



        // String dateString;
        char dateString[len];

        if (a.date <= 9) {
            snprintf(dateString, sizeof(dateString), "0%d", a.date);
        } else {
            snprintf(dateString, sizeof(dateString), "%d", a.date);
        }

        // strncat(dateString, String(a.date), sizeof(dateString));



        // String hourString;
        char hourString[len];

        if (a.hour <= 9) {
            snprintf(hourString, sizeof(hourString), " %d", a.hour);
        } else {
            snprintf(hourString, sizeof(hourString), "%d", a.hour);
        }

        // strncat(hourString, String(a.hour), sizeof(hourString));



        // String minuteString;
        char minuteString[len];

        if (a.minute <= 9) {
            snprintf(minuteString, sizeof(minuteString), "0%d", a.minute);
        } else {
            snprintf(minuteString, sizeof(minuteString), "%d", a.minute);
        }

        // strncat(minuteString, String(a.minute), sizeof(minuteString));



        // String secondString;
        char secondString[len];

        if (a.second <= 9) {
            snprintf(secondString, sizeof(secondString), "0%d", a.second);
        } else {
            snprintf(secondString, sizeof(secondString), "%d", a.second);
        }

        // strncat(secondString, String(a.second), sizeof(secondString));



        snprintf(destination, len, "%s-%s-%s   %s:%s:%s", yearString, monthString, dateString, hourString, minuteString, secondString);
        

        // return String(
        //     String(yearString)  + '-'   +
        //     String(monthString) + '-'   +
        //     String(dateString)  + "   " +

        //     String(hourString)   + ':' +
        //     String(minuteString) + ':' +
        //     String(secondString)
        // );

        // return result;

    }



    const char * MONTH_STRINGS_UPPER[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
    const char * MONTH_STRINGS_LOWER[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    const char * getMonthString(time a, bool allCaps) {
        if (allCaps) {
            return MONTH_STRINGS_UPPER[a.month - 1];
        } else {
            return MONTH_STRINGS_LOWER[a.month - 1];
        }
    }


    const char SEC_SPAN_OPEN[]  = "<span class='timesec'>";
    const char SEC_SPAN_CLOSE[] = "</span>";

    void time_to_string_friendly(char * destination, int len, time a, bool withSpans) {


        const char * monthString = getMonthString(a, true);


        // String dateString = String(a.date);



        // String hourString = String(a.hour);



        char minuteString[len];

        if (a.minute <= 9) {
            snprintf(minuteString, sizeof(minuteString), "0%d", a.minute);
        } else {
            snprintf(minuteString, sizeof(minuteString), "%d", a.minute);
        }



        char secondString[len];

        if (a.second <= 9) {
            snprintf(secondString, sizeof(secondString), "0%d", a.second);
        } else {
            snprintf(secondString, sizeof(secondString), "%d", a.second);
        }


        char secSpanOpen[len]  = "";
        char secSpanClose[len] = "";
        if (withSpans) {
            strncpy(secSpanOpen,  SEC_SPAN_OPEN,  sizeof(secSpanOpen ));
            strncpy(secSpanClose, SEC_SPAN_CLOSE, sizeof(secSpanClose));
        }


        snprintf(destination, len, "%d %s  %d:%s%s:%s%s", a.date, monthString, a.hour, minuteString, secSpanOpen, secondString, secSpanClose);

        // return String(
        //     dateString + " " + monthString + "  " +

        //     hourString   + ':' +
        //     minuteString + secSpanOpen + ':' +
        //     secondString + secSpanClose
        // );

    }


    

    void print_time(time a) {

        char result[50];
        time_to_string(result, sizeof(result), a);
        Serial.println(result);
        
    }


    String test_function() {
        return "TESTTESTEST1111";
    }


    void printTime()
    {
        Serial.print(String(rtc.hour()) + ":"); // Print hour
        if (rtc.minute() < 10)
            Serial.print('0'); // Print leading '0' for minute
        Serial.print(String(rtc.minute()) + ":"); // Print minute
        if (rtc.second() < 10)
            Serial.print('0'); // Print leading '0' for second
        Serial.print(String(rtc.second())); // Print second

        if (rtc.is12Hour()) // If we're in 12-hour mode
        {
            // Use rtc.pm() to read the AM/PM state of the hour
            if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
            else Serial.print(" AM");
        }

        Serial.print(" | ");

        // Few options for printing the day, pick one:
        Serial.print(rtc.dayStr()); // Print day string
        //Serial.print(rtc.dayC()); // Print day character
        //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
        Serial.print(" - ");
        #ifdef PRINT_USA_DATE
            Serial.print(String(rtc.month()) + "/" +   // Print month
                    String(rtc.date()) + "/");  // Print date
        #else
            Serial.print(String(rtc.date()) + "/" +    // (or) print date
                    String(rtc.month()) + "/"); // Print month
        #endif
        Serial.println(String(rtc.year()));        // Print year
    }





}




