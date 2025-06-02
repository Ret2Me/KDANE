#pragma once
#include "macros.h"
#include "modules/game.h"
#include "modules/hieroglyphs/hieroglyphs.h"
#include "modules/hieroglyphs/lcd_chars.h"


LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


Hieroglyphs::Hieroglyphs() {};
Hieroglyphs::Hieroglyphs(unsigned int difficulty) {
    this->difficulty = difficulty;
    this->solved = false;

    // init solution array
    for (unsigned int i=0; i < this->solution_length; i++)
        this->solution[i] = i;
    // permutate solution
    for (unsigned int i=0; i < this->permutation_rounds; i++) {
        // choose two random positions and save first one to tmp
        int first_random_index = random(0, this->solution_length); delay(10);
        int second_random_index = random(0, this->solution_length); delay(10);
        int tmp = this->solution[first_random_index];

        // swap two numbers in solution array
        this->solution[first_random_index] = this->solution[second_random_index];
        this->solution[second_random_index] = tmp;
    }
}


void Hieroglyphs::init() {

    // set digital pins 
    unsigned int digital_input_size = 4; //sizeof(this->digital_solution_input) / sizeof(this->digital_solution_input[0]); 
    for (unsigned int i=0; i < digital_input_size; i++) {
        pinMode(this->mux_addr[i], OUTPUT);
    }


    // calculate button order
    for (unsigned int i=0; i < this->digital_solution_input_len; i++)
        this->button_order[i] = this->solution[i * 2] % 4;

    // calculatte button press time
    for (unsigned int i=0; i < this->digital_solution_input_len; i++)
        this->button_press_time[i] = (this->solution[i * 2 + 1] +1) % 8;
    
    // array with all hiergolifs
    byte* hierogilfs_ascii[8] = {
        face, yeti, person,
        amogus, invertedAce, ace,
        reverseS, omega
    };

    // Initialize the display
    lcd.init();
    lcd.backlight();

    // Init chars
    unsigned int hieroglyphs_ascii_len = sizeof(hierogilfs_ascii) / sizeof(hierogilfs_ascii[0]);
    for (unsigned int i=0; i < hieroglyphs_ascii_len; i++)    
        lcd.createChar(i, hierogilfs_ascii[i]);

    // We only need to render Hieroglyphs once for begginer level
    if (this->difficulty == 0) {
        // render Hieroglyphs
        for (unsigned int i = 0; i < this->solution_length; i++) {
            lcd.setCursor(i * 2, 1);        
            lcd.write(this->solution[i]);
        }
    }
}


void Hieroglyphs::tick() {
    if (solved)
        return;

    // hieroglyphs are reneder only once
    if (this->difficulty == 0)
        return;

    // internal counter to slow down animation
    this->internal_counter++;
    if (this->internal_counter % this->internal_tick != 0)
        return;
    this->internal_counter = 0;

    // if last chars are rendered, clear the screen 
    if (this->acc_pos == 0)
        lcd.clear();

    // render new character
    unsigned int y = (this->difficulty == 1) ? 1 : this->acc_pos % 2;  // jumping Hieroglyphs effect
    lcd.setCursor(acc_pos * 2, y);
    lcd.write(this->solution[acc_pos]);
    
    // clear the previous character (fade effect)
    y = (this->difficulty == 1) ? 1: (this->acc_pos - 1) % 2;
    if (this->acc_pos != 0) {
        lcd.setCursor((this->acc_pos - 1) * 2, y);
    }
    lcd.write(' '); // clear the character

    // increment the position counter
    this->acc_pos = (this->acc_pos == this->solution_length - 1) ? 0 : this->acc_pos + 1;
}

unsigned short int Hieroglyphs::verify() {
    // return:
    // 0 - no actions performed
    // 1 - incorrect solution
    // 2 - game solved

    if (this->solved)
        return 0;

    unsigned int acc_state;

    // check all 4 buttons
    for (unsigned int i=0; i < digital_solution_input_len; i++) {
        for (unsigned int j=0; j < 4; j++) {
            digitalWrite(this->mux_addr[j], digital_solution_input[i][j]);
        }
        pinMode(MUX_DATA, INPUT_PULLUP);

        // button clicked (+ analog to digital conversion)
        acc_state = (analogRead(MUX_DATA) > 512);
        char buf[20];
        sprintf(buf, "State: %d", acc_state);
        Serial.write(buf);
        if (last_btn_state[i] != acc_state) {
            last_btn_state[i] = acc_state;  // update acc state
            if (acc_state == 0 && button_order[this->solution_state] == i && button_press_time[this->solution_state]== this->acc_pos) {
                // correct button presed 
                this->solution_state++;

                char buf[20];
                sprintf(buf, "State: %d\n", this->solution_state);
                Serial.write(buf);

                Serial.write("Correct value");
                if (this->solution_state == this->digital_solution_input_len) {
                    this->solved = true;
                    return 2;  // game solved
                }
            } else if (acc_state == 0) {
                // incorrect button pressed 
                this->solution_state = 0;
                return 1;  // incorrect solution
            }
        }
    }

    // no actions performed
    return 0;   
}