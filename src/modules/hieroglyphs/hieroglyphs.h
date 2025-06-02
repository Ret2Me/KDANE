#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


class Hieroglyphs : public Game {
private:
    unsigned int difficulty;
    
    // pins
    unsigned int digital_solution_input[4][4] = {RED_BUTTON, GREEN_BUTTON, BLUE_BUTTON, YELLOW_BUTTON};
    unsigned int mux_addr[4] = {3, 4, 5, 6};
    unsigned int digital_solution_input_len = sizeof(digital_solution_input) / sizeof(digital_solution_input[0]);
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
    Hieroglyphs(unsigned int difficulty);

    void tick();
    unsigned short int verify();
    void init();
};