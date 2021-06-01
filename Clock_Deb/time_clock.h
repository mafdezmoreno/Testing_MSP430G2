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

        std::string get_year(){

            return int_to_string(_year);
        }

        std::string get_month(){
            return "05";
        }

        std::string get_week(){
            return "22";
        }

        std::string get_day(){
            return "31";
        }
        const char * get_hour();
        const char * get_minute();
        const char * get_second();

        bool get_update_minute();
        bool get_update_hour();
        
        // Normal operation of clock (increment time every second). To be called by the interrupt function
        void increment_second();



private:
        unsigned int _year;
        unsigned int _month;
        unsigned int _week;
        unsigned int _day;
        unsigned int _hour;
        unsigned int  _minute;
        unsigned char  _second;

        bool _update_minute;
        bool _update_hour;

        const char * int_to_string(unsigned char value);
        void increment_minute();
        void increment_hour();
        void increment_day();
        void increment_week();
        void increment_month();
        void increment_year();
        void set_time();

        void update_minute();
        void update_hour();
};

#endif /* TIME_H_ */
