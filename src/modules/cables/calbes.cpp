#pragma once
#include <modules/cables/cables.h>
Servo myservo;


Cables::Cables (){};
Cables::Cables (unsigned int digital_solution_input,unsigned int analog_pins[]){

    this->solved = false;
    
    myservo.attach(9);
    unsigned int digital_input_size = sizeof(this->analog_pins) / sizeof(this->analog_pins[0]); 
    for (int i=0; i < digital_input_size; i++) {
        this->analog_pins[i] = analog_pins[i];
        pinMode(analog_pins[i], INPUT);
    }
    // Calculate expected readings for each resistor
    // Based on voltage divider with Arduino's internal ~32k pulldown resistor when reading analog
    for (int i = 0; i < this->num_cables; i++) {
    // ADC reading = (resistor / (resistor + 32k)) * 1023
    // This is approximate - you may need to calibrate based on actual readings
    this->expected_readings[i] = (int)(((float)color_resistors[i] / (color_resistors[i] + 32000)) * 1023);
    
    // Initialize connection tracking arrays
        this->current_connections[i] = -1; // -1 means not connected
        this->has_connection[i] = false;
    }

};

void Cables::tick()
{
    if (solved)
        return;

    this->servo_pos += this->direction;
    if(servo_pos == 180 || servo_pos ==0)
        this->direction *= -1;

    myservo.write(this->servo_pos);
    delay(speed);

    for (int i = 0; i < this->num_cables; i++) {
    this->current_connections[i] = -1;
    this->has_connection[i] = false;
  }
  
}

unsigned short int Cables::verify()
{
    if (this->solved)
        return 0;

    for (unsigned input_channel = 0; input_channel < this->num_cables; input_channel++) {
        int analog_value = analogRead(analog_pins[input_channel]);

    // Only process if there's a significant reading
        if (analog_value > 50)
        {
            int best_match = -1;
            int smallest_difference = 1024; // Max ADC value
            
            // Find the closest matching expected reading
            for (int i = 0; i < this->num_cables; i++) {
                int difference = abs(analog_value - this->expected_readings[i]);
            
                if (difference < smallest_difference && difference < this->reading_tollerance) {
                smallest_difference = difference;
                best_match = i;
                }

                unsigned int detected_color = best_match;

                if (detected_color >= 0) {
                    // We found a connection
                    this->current_connections[input_channel] = detected_color;
                    this->has_connection[input_channel] = true;

                    if (detected_color == input_channel) {
                        solution_state++;
                    } 
                    else {
                        solution_state=0;
                        return 1;
                    }
                }
            }
        }
    }
    if (this->solution_state == this->num_cables) {
                    this->solved = true;
                    return 2;  // game solved
                }

    return 0;

}