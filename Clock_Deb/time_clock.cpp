/*
 * time.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mafm
 */

#include "time_clock.h"

Time_Clock::Time_Clock()
{
    
    _second = 55;
    _minute = 44;
    _hour = 23;
    _nday = 22;     //day of month
    _wday = 1;      // 1 = monday, 2 = tuesday ...
    _week = 24;
    _month = 11;
    _year = 21;

    _update_minute = true;
    _update_hour = true;
    _update_day = true;
    _update_wday = true;
    _update_week = true;
    _update_month_year = true;
}

const char * Time_Clock::int_to_string(unsigned char value){

    std::string s = std::to_string(value);
    if (value <10)
        s = "0" + s;
    return s.c_str();
}

// ! MODIFIER FUNCTIONS
// To increment values

void Time_Clock::increment_minute(){
    _minute++;
    if (_minute >= 60){
        increment_hour();
        update_hour();
        _minute = 0;
    }
}

void Time_Clock::increment_hour(){
    _hour++;
    if (_hour >= 24){
        increment_day();
        update_day();
        _hour = 0;
    }
}

void Time_Clock::decrement_hour(){
    _hour--;
    if (_hour <= 0){
        //increment_day();
        //update_day();
        _hour = 23;
    }
}

void Time_Clock::increment_day(){
    _nday++;
    _wday++;
    if (_nday > 30){
        increment_month();
        update_month_year();
        _nday = 1;
    }
    if(_wday > 7){
        increment_week();
        update_week();
        _wday = 1;
    }
}

void Time_Clock::increment_second(){
    _second++;
    if (_second >= 60){
        increment_minute();
        update_minute();
        _second = 0;
    }
}
void Time_Clock::increment_week(){
    _wday++;
}
void Time_Clock::increment_month(){
    _wday++;
}
void Time_Clock::increment_year(){
    _wday++;
}

// To update status
void Time_Clock::update_minute(){
    _update_minute = true;
}
void Time_Clock::update_hour(){
    _update_hour = true;
}
void Time_Clock::update_day(){
    _update_day = true;
}
void Time_Clock::update_week(){
    _update_week = true;
}
void Time_Clock::update_month_year(){
    _update_month_year = true;
}

// GET FUNCTIONS
// get char functions
const char * Time_Clock::get_second(){
    return int_to_string(_second);
}
const char * Time_Clock::get_minute(){
    return int_to_string(_minute);
}
const char * Time_Clock::get_hour(){
    return int_to_string(_hour);
}
const char * Time_Clock::get_nday(){
    return int_to_string(_nday);
}
const char * Time_Clock::get_wday(){
    //return int_to_string(_wday);
    return wday[_wday];
}
const char * Time_Clock::get_week(){
    return int_to_string(_week);
}
const char * Time_Clock::get_month(){
    return int_to_string(_month);
}
const char * Time_Clock::get_year(){
    return int_to_string(_year);
}

// get status functions
bool Time_Clock::get_update_minute(){
    if (_update_minute){
        _update_minute = false;
        return true;
    }
    return false;
}

bool Time_Clock::get_update_hour(){
    if (_update_hour){
        _update_hour = false;
        return true;
    }
    return false;
}

bool Time_Clock::get_update_day(){
    if (_update_day){
        _update_day = false;
        return true;
    }
    return false;
}

bool Time_Clock::get_update_week(){
    if (_update_week){
        _update_week = false;
        return true;
    }
    return false;
}

bool Time_Clock::get_update_month_year(){
    if (_update_month_year){
        _update_month_year = false;
        return true;
    }
    return false;
}