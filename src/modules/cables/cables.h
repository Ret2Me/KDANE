#pragma once
#include <Arduino.h>
#include <Servo.h>


class Cables{
    private:
        unsigned int speed = 5;
        unsigned int digital_solution_input;
        unsigned short int direction = 1;

        unsigned int num_cables = 4;
        unsigned int analog_pins[4];
        const char* color_nNames[4] = {"Red", "Blue", "Green", "Yellow"};
        unsigned int color_resistors[4] = {
            1000,   // Red: 1k ohm
            2200,   // Blue: 2.2k ohm
            3300,   // Green: 3.3k ohm
            4700 // Yellow: 4.7k ohm
        };
        int expected_readings[4];
        unsigned int reading_tollerance = 25;

        int current_connections[4];
        bool has_connection[4];

        unsigned int solution_state = 0;
    public:
        unsigned int servo_pos = 0;
        bool solved;
        Cables();
        Cables(unsigned int digital_solution_input,unsigned int analog_pins[]);
        void tick();
        unsigned short int verify();


};