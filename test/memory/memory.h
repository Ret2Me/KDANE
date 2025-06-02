#pragma once
#include <Arduino.h>

class Memory : public Game {
    private:
        unsigned int difficulty;
        unsigned int digital_pins_len = 4;
        unsigned int digital_pins_buttons[4];
        unsigned int digital_pins_leds[3];
        unsigned int last_btn_state[4] = {1, 1, 1, 1};
        
        // internal variables
        unsigned int acc_pos = 1;
        unsigned int internal_tick = 100;
        unsigned int internal_counter = 0;
        unsigned int permutation_rounds = 24;
        unsigned int soluion_poss[4] = {0,1,2,3};
        unsigned int colors[5][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{0,0,0}};

        bool waiting = false;
        unsigned int solution_state = 0;
        void choose_color(unsigned int col);

        unsigned int digital_pins_buttons[4] = {};
        unsigned int digital_pins_leds[3] = {};

    public:
        bool solved;
        unsigned int solution_length = 8;
        unsigned int solution[8];
        Memory();
        Memory(unsigned int difficulty);
        
        void tick();
        unsigned short int verify();
        
};