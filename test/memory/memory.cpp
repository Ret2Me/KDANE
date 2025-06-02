#pragma once
#include "modules/memory/memory.h"
#include "macros.h"
#include "modules/game.h"

Memory::Memory() {};
Memory::Memory(unsigned int difficulty, 
    unsigned int digital_solution_input[],
    unsigned int digital_solution_leds[]) {


    this->difficulty=difficulty;
    this->solved = false;

    // set digital pins 
    unsigned int digital_input_size = sizeof(this->digital_solution_input) / sizeof(this->digital_solution_input[0]); 
    for (int i=0; i < digital_input_size; i++) {
        this->digital_solution_input[i] = digital_solution_input[i];
        pinMode(digital_solution_input[i], INPUT_PULLUP);
    }

    // set digital pins for LEDs
    for (int i=0; i < digital_input_size; i++) {
        this->digital_solution_leds[i] = digital_solution_leds[i];
        pinMode(digital_solution_leds[i], OUTPUT);
    }

    // red - 0 ; green - 1 ; blue - 2 ; yellow - 3 -- led pins
    // yellow - 0 ; blue - 1 ; red - 2 ; green - 3 -- button pins

    for (unsigned int i=0; i < this->solution_length; i++) {
        // choose random colors for the solution
        int random_index = random(0, this->digital_solution_len);
        delay(10);
        this->solution[i] = soluion_poss[random_index];
    }
}


void Memory::choose_color(unsigned int col){
    for(unsigned int i = 0;i<3;i++)
        analogWrite(digital_solution_leds[i],this->colors[col][i]);    
}

void Memory::tick()
{
    if (solved)
    return;
    if (!waiting){
        for (unsigned int i = 0;i<acc_pos;i++)
        {
            for(unsigned int j = 0;j<=i;j++)
            {
                choose_color(solution[j]);
                delay(1000);
                choose_color(4);
                delay(1000);
            }

        }
    }
    if (acc_pos != 1)
        waiting = true;
}

unsigned short int Memory::verify()
{
     // return:
    // 0 - no actions taken
    // 1 - incorrect solution
    // 2 - game solved

    if (this->solved)
        return 0;
-
    unsigned int acc_state;
    for (unsigned int i=0; i < digital_solution_len; i++) {
        // button clicked 
        acc_state = digitalRead(digital_solution_input[i]);
        if (last_btn_state[i] != acc_state) {
            last_btn_state[i] = acc_state;  // update acc state
            if (acc_state == 0 && solution[this->solution_state] == i) {
                // correct button presed 
                this->solution_state++;
                char buf[20];
                sprintf(buf, "State: %d", this->solution_state);
                Serial.write(buf);

                Serial.write("Correct value");

                if (this->solution_state == this->acc_pos)
                {
                    this->solution_state = 0;
                    this->acc_pos++;
                }

                if (this->solution_state == this->solution_length) {
                    this->solved = true;
                    return 2;  // game solved
                }
            } else if (acc_state == 0) {
                // incorrect button pressed 
                this->solution_state = 0;
                this->acc_pos = 1;
                return 1;  // incorrect solution
            }
        }
        
    }

    // no action performed
    return 0;   
}