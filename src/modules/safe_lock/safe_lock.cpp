#pragma once
#include "macros.h"
#include "modules/game.h"
#include "modules/safe_lock/safe_lock.h"

LiquidCrystal_I2C lcd4(0x3F, 16, 2);

Safe_lock::Safe_lock() {};
Safe_lock::Safe_lock(unsigned int difficulty) {
    this->difficulty = difficulty;
    this->solved = false;
    switch (difficulty) {
        case 0: // easy
            this->solution_length = 4;
            break;
        case 1: // medium
            this->solution_length = 6;
            break;
        case 2:
            this->solution_length = 8;
            break;
        default:
            this->solution_length = 4;
            break;
    }
    for (unsigned short int i = 0; i < this->solution_length; i++) {
       this->solution[i] = random(0, 300); delay(10);
    }
}

unsigned int Safe_lock::get_value(unsigned int val) {
    // return value from 0 to 300
    switch (this->solution_state) {
    case 0:
        unsigned int answer = this->previous_value - val; 
        return answer;
    
    default:
        return 500;
        break;
    }
}

void Safe_lock::init() {
    pinMode(A3, INPUT);
    pinMode(2, INPUT_PULLUP);
    lcd4.init();
    lcd4.backlight();
    lcd4.setCursor(0, 0);
    for (unsigned int i = 0; i < this->solution_length; i++) {
        lcd4.print(this->solution[i]);
        lcd4.print(" ");
    }
}

void Safe_lock::tick() {
    if (solved)
        return;
    
    this->internal_counter++;
    if (this->internal_counter % this->internal_tick != 0)
        return;
    this->internal_counter = 0;
    
    this->value = analogRead(A3);
    lcd4.setCursor(0, 1);
    lcd4.print("Value: ");
    lcd4.print(this->value);
}

unsigned short int Safe_lock::verify() {
    // return:
    // 0 - no actions performed
    // 1 - incorrect solution
    // 2 - game solved

    if (this->solved)
        return 0;

    unsigned int acc_state = digitalRead(2);
    
    if (acc_state == LOW && this->last_btn_state == HIGH) {
        this->solution_state++;
        if (this->solution_state == this->solution_length) {
            this->solved = true;
            return 2;
        }
        else {
            if (abs(this->value - get_value(this->solution[this->solution_state])) < this->range_error) {
                //correct solution
                this->solution_state++;
                return 0;
            }
            else {
                // incorrect solution
                this->solution_state = 0;
                this->solved = false;
                return 1;
            }
        }
    }
    this->last_btn_state = acc_state;
    return 0;
}