#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class Safe_lock {
private:
    unsigned int difficulty;
    unsigned int previous_value = 500;
    unsigned int value;
    unsigned int range_error = 10;
    unsigned int solution_length = 5;
    unsigned int solution_state = 0;
    unsigned int solution[8];
    unsigned int last_btn_state = 1;
    unsigned int internal_tick = 100;
    unsigned int internal_counter = 0;

public: 
    bool solved = false;
    Safe_lock();
    Safe_lock(unsigned int difficulty); 
    void init();
    void tick();
    unsigned short int verify();
    unsigned int get_value(unsigned int val);
};