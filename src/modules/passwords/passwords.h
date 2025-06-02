#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>



class Passwords : public Game {
private:
    unsigned int difficulty;

    // pins
    unsigned int numRows = 4;
    unsigned int numCols = 4;

    // internal variables
    unsigned int acc_pos;
    unsigned int solution_state = 0;
    unsigned int cursor_pos = 0;
    char answer[9] = "NNNNNNNN";

    unsigned int remaining_cooldown = 0;
    unsigned long key_cooldown = 10; // 1000 milliseconds = 1 second

public:
    // ToDo:
    // poziomy trudnosci 
    // japonskie znaczki
    // losowe haslo
    // konstruktor -> init()

    bool solved = false;
    unsigned int solution_length = 8;
    char solution[9] = "XXXXXXXX";
    char charset[16];

    // caharcters used in the keypad
    // to be fixed
    char keys[4][4] = {
        {char(193), char(197), char(196), char(203)},
        {char(200), char(195), char(198), char(202)},
        {char(208), char(201), char(207), char(204)},
        {char(199), char(206), char(194), char(205)}
    };

    byte colPins[4] = {COLUMN_BTN_ARRAY}; 
    byte rowPins[4] = {ROW_BTN_ARRAY};
    Keypad keypadObj = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

    Passwords();
    Passwords(unsigned int difficulty);

    void init();
    void tick();
    unsigned short int verify();
};
