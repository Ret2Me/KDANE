#pragma once
#include "macros.h"
#include "modules/game_clock/game_clock.h"

LiquidCrystal_I2C lcd3(0x3F, 16, 2);

Game_clock::Game_clock() {};
Game_clock::Game_clock(unsigned int game_duration) {
    this->game_duration = game_duration;
    this->game_over = false;
}

void Game_clock::init() {
    pinMode(this->rclk, OUTPUT);
    pinMode(this->srclk, OUTPUT);
    pinMode(this->ser, OUTPUT);
    this->game_time = millis();
}


void Game_clock::tick() {
    
    if (millis() - this->game_time <= 10000) {
        digitalWrite(this->rclk, LOW);
        shiftOut(this->ser, this->srclk, LSBFIRST, this->font[this->current_minute]);
        digitalWrite(this->rclk, HIGH);
    }
    else {
        this->current_minute++;
        this->game_time = millis();
    }

}

void Game_clock::verify() {
    if (this->current_minute > this->minutes_per_game) {
        this->game_over = true;
    }
    if (this->game_over) {
            lcd3.init();
            lcd3.backlight();
            lcd3.setCursor(0, 0);        
            lcd3.print("BOOOOOOOM");
        while (true){
            for(unsigned int i = 6; i < 11; i++){
                digitalWrite(this->rclk, LOW);
                shiftOut(this->ser, this->srclk, LSBFIRST, this->font[i]);
                digitalWrite(this->rclk, HIGH);
                delay(100);
            }
            for(unsigned int i = 9; i > 6; i--){
                digitalWrite(this->rclk, LOW);
                shiftOut(this->ser, this->srclk, LSBFIRST, this->font[i]);
                digitalWrite(this->rclk, HIGH);
                delay(100);
            }
        }
    }
}