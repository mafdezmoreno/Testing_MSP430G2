/*
 * time.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mafm
 */

#include "time_clock.h"

Time_Clock::Time_Clock()
{
    // TODO Auto-generated constructor stub
    _year = 2021;
    _second = 59;
}

const char * Time_Clock::get_second(){
    return int_to_string(_second);
}

const char * Time_Clock::int_to_string(unsigned char value){

    std::string s = std::to_string(value);
    return s.c_str();
}

void Time_Clock::increment_second(){

    _second++;
    if (_second >= 60){
        increment_minute();
        _second = 0;
    }
}

void Time_Clock::increment_minute(){

    _minute++;
    if (_second >= 60){
        increment_hour();
        _minute = 0;
    }
}

void Time_Clock::increment_hour(){

    _hour++;
    if (_hour >= 24){
        increment_day();
        _day = 0;
    }
}

void Time_Clock::increment_day(){

    _day++;
    if (_day >= 30){
        _month++;
        _day = 0;
    }
}

