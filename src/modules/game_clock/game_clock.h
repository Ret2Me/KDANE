#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class Game_clock {
private:
// Pins
    unsigned int game_duration;
    static const int rclk = 6;
    static const int srclk = 9;
    static const int ser = 10;
    unsigned long game_time = 0;
    unsigned int current_minute = 0;
    uint8_t font[11] = {
        0b01111001, // 0
        0b01111000, // 1
        0b01110000, // 2
        0b01100000, // 3
        0b01000000, // 4
        0b00000000, // 5
        0b01000000, // 6
        0b00100000, // 7
        0b00010000, // 8
        0b00001000, // 9
        0b00000001  // 10
    };
    unsigned int minutes_per_game = 4;
    
public:
    bool game_over = false;
    Game_clock();
    Game_clock(unsigned int game_duration = 120);
    void init();
    void tick();
    void verify();
};

