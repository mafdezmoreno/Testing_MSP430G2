/*
 * time.h
 *
 *  Created on: May 31, 2021
 *      Author: mafm
 */

#ifndef TIME_H_
#define TIME_H_

#include <string> // for string class


class Time_Clock
{
    public:
        Time_Clock();

        //get char functions
        const char * get_year();
        const char * get_month();
        const char * get_week();
        const char * get_wday();
        const char * get_nday();
        const char * get_hour();
        const char * get_minute();
        const char * get_second();
        
        //get status functions
        bool get_update_minute();
        bool get_update_hour();
        bool get_update_day();
        bool get_update_week();
        bool get_update_month_year(); //I use this for month & year
        
        // ! MODIFIER FUNCTIONS
        // To increment values

        // Normal operation of clock (increment time every second). To be called by the interrupt function
        void increment_second();

        void increment_minute();
        void increment_hour();
        void increment_day();
        void increment_week();
        void increment_month();
        void increment_year();
        void set_time();

        void decrement_hour();
private:

        // Register variables
        unsigned int _year;
        unsigned int _month;
        unsigned int _week;
        unsigned int _wday;
        unsigned int _nday;
        int _hour;
        unsigned int _minute;
        unsigned char _second;

        // status variables
        bool _update_minute;
        bool _update_hour;
        bool _update_day;
        bool _update_wday;
        bool _update_week;
        bool _update_month_year; //I use this for month & year

        // constants
        const char *wday[7] = { "LUNES", "MARTES",
                                "MIERCOLES", "JUEVES",
                                "VIERNES", "SABADO", "DOMINGO"};
 

        const char * int_to_string(unsigned char value);

        // To update status (to refesh value on lcd)
        void update_minute();
        void update_hour();
        void update_day();
        void update_week();
        void update_month_year();
};

#endif /* TIME_H_ */
