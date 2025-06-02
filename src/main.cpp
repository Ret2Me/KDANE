/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * YWROBOT
 *Compatible with the Arduino IDE 1.0
 *Library version:1.1
*/

#pragma once
#include <Arduino.h>
#include "macros.h"
#include "modules/game.h"
#include "modules/hieroglyphs/hieroglyphs.h"
#include "modules/passwords/passwords.h"
#include "modules/game_clock/game_clock.h"
#include "modules/safe_lock/safe_lock.h"
//#include "modules/memory/memory.h"
//#include <modules/cables/cables.h>

// game queue
unsigned short int curr_game = 0;
Game* games_queue[3];
unsigned short int game_num = sizeof(games_queue) / sizeof(games_queue[0]);

//Game_clock game_timer = Game_clock(120);
//Safe_lock safe_lock = Safe_lock(0);

void setup() {

    Serial.begin(9600);
    
    randomSeed(analogRead(0));
    // // game queue
    // games_queue[0] = new Passwords(0);
    games_queue[0] = new Hieroglyphs(2);
    // games_queue[2] = new Safe_lock(0);

    // // permutate game queue
    // for (unsigned short int i=0; i < game_num; i++) {

    //     // -- could use XOR swap but it's very hard to read --
    //     // choose two random positions and save first one to tmp
    //     int first_rnd_idx = random(0, game_num); delay(10);
    //     int second_rnd_idx = random(0, game_num); delay(10);
    //     Game* tmp = games_queue[first_rnd_idx];

    //     // swap two numbers in solution array
    //     games_queue[first_rnd_idx] = games_queue[second_rnd_idx];
    //     games_queue[second_rnd_idx] = tmp;
    // }

    // // init first game
    games_queue[0]->init();
    //safe_lock.init();
    //game_timer.init();
}


void loop() {
    //safe_lock.tick();
    //game_timer.tick();
    //game_timer.verify();
    // verify game state
    unsigned int state = (*(games_queue[curr_game])).verify();
    if ((*(games_queue[curr_game])).solved) {
        if (state == 2) {
            Serial.write("Game solved!\n");
            curr_game++;
            if (curr_game >= game_num) {
                curr_game = 0;
            } else Serial.write("Next game started!\n");

            // init next game
            (*(games_queue[curr_game])).init();
        }

        if (state == 1)
            Serial.write("Inccorrect Value!\n");
    }

    // run one game tick
    (*(games_queue[curr_game])).tick();

    delay(10);
}
