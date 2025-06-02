#pragma once
#include "macros.h"
#include "modules/game.h"
#include "modules/passwords/passwords.h"

LiquidCrystal_I2C lcd2(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display;


Passwords::Passwords() {};
Passwords::Passwords(unsigned int difficulty) {

    // create katakana charset
    for (unsigned short int i = 0; i < 16; i++)
       this->charset[i] = char(193 + i);


    this->difficulty = difficulty;
    this->solved = false;
    switch (difficulty) {
        case 0: // easy
            this->solution_length = 8;
            break;
        case 1: // medium
            this->solution_length = 12;
            break;
        case 2:
            this->solution_length = 16;
            break;
        default:
            this->solution_length = 8;
            break;
    }

    for (unsigned short int i = 0; i < this->solution_length; i++) {
       this->solution[i] = char(random(193, 193 + 16)); delay(10);
    }
}

void Passwords::init() {

    // set lcd2 screen (ToDo: check out if it is needed to use lcd22)
    lcd2.init();
    lcd2.backlight();
    for (unsigned int i = 0; i < this->solution_length; i++) {
        lcd2.setCursor(i, 0);        
        lcd2.write(this->solution[i]);
    }
}

void Passwords::tick(){


    if (solved)
        return;

    Serial.write("Cooldown: ");
    Serial.write(this->remaining_cooldown);
    Serial.write("\n");
    if (this->remaining_cooldown == 0) {  // creating small delay to avoid any noise
        char key = this->keypadObj.getKey();
        Serial.write("Key pressed: ");
        Serial.write(key);
        Serial.write("\n");

        if (key) {
            Serial.write("Key pressed: ");
            Serial.write(key);
            Serial.write("\n");

            lcd2.setCursor(cursor_pos, 1);
            lcd2.write(key);
            answer[cursor_pos] = key;
            cursor_pos += 1;
            this->remaining_cooldown = this->key_cooldown;
        }
    } else this->remaining_cooldown--;
}

unsigned short int Passwords::verify(){
    // return:
    // 0 - no actions performed
    // 1 - incorrect solution
    // 2 - game solved

    if (this->solved)
        return 0;

    if (cursor_pos == this->solution_length) {

        // check if the answer is correct
        bool succ = true;
        for (unsigned int i = 0; i < this->solution_length; i++){
            if (this->solution[i] != answer[i]) {
                succ = false;
                break;
            }
        }

        if (succ) {
            this->solved = true;
            return 2; // game solved
        }


   

        // write solution??
        lcd2.clear();
        for (unsigned int i = 0; i < this->solution_length; i++) {
            lcd2.setCursor(i, 0);        
            lcd2.write(this->solution[i]);
        }
        cursor_pos = 0;
        return 1;
    }

    return 0;   
}
