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
    _second = 55;
    _minute = 44;
    _hour = 33;
    _day = 22;

    _update_minute = true;
    _update_hour = true;
}

const char * Time_Clock::get_second(){
    return int_to_string(_second);
}
const char * Time_Clock::get_minute(){
    return int_to_string(_minute);
}
const char * Time_Clock::get_hour(){
    return int_to_string(_hour);
}

const char * Time_Clock::int_to_string(unsigned char value){

    std::string s = std::to_string(value);
    if (value <10)
        s = "0" + s;
    return s.c_str();
}

void Time_Clock::increment_second(){

    _second++;
    if (_second >= 60){
        increment_minute();
        update_minute();
        _second = 0;
    }
}

void Time_Clock::update_minute(){
    _update_minute = true;
}
bool Time_Clock::get_update_minute(){
    if (_update_minute){
        _update_minute = false;
        return true;
    }
    return false;
}
void Time_Clock::increment_minute(){
    _minute++;
    if (_minute >= 60){
        increment_hour();
        update_hour();
        _minute = 0;
    }
}

void Time_Clock::update_hour(){
    _update_hour = true;
}
bool Time_Clock::get_update_hour(){
    if (_update_hour){
        _update_hour = false;
        return true;
    }
    return false;
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

