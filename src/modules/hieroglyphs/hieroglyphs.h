#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


class Hieroglyphs {
private:
    unsigned int difficulty;
    
    // pins
    unsigned int display_sda;
    unsigned int display_scl;
    unsigned int digital_solution_input_len = 4;
    unsigned int digital_solution_input[4];
    unsigned int last_btn_state[4] = {1, 1, 1, 1};
    
    // internal variables
    unsigned int acc_pos = 0;
    unsigned int internal_tick = 100;
    unsigned int internal_counter = 0;
    unsigned int permutation_rounds = 24;
    
    unsigned int solution_state = 0;

public:
    bool solved;
    unsigned int solution_length = 8;
    unsigned int solution[8];
    unsigned int button_order[4] = {0, 0, 0, 0};
    unsigned int button_press_time[4] = {0,0,0,0};

    Hieroglyphs();
    Hieroglyphs(unsigned int difficulty,
               unsigned int digital_solution_input[],
               unsigned int display_sda = 0,
               unsigned int display_scl = 0);
    void tick();
    unsigned short int verify();
};