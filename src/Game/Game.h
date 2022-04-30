
/*
 * Copyright (c) 2022 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */


/*
 * Interface file of the module Game
 */

#pragma once

#ifndef GAME_H
#define GAME_H

#include <thread>
#include <mutex>
#include <atomic>
#include "../Input/Input.h"
#include "../Audio/Audio.h"
#include "../Menus/Menu/Menu.h"
#include "../Scene/Map/Map.h"
#include "../Menus/MenuLogo/MenuLogo.h"
#include "../Menus/MenuStart/MenuStart.h"
#include "../Menus/MenuGame/MenuGame.h"
#include "../Menus/MenuGears/MenuGears.h"
#include "../Menus/MenuMusicRadio/MenuMusicRadio.h"
#include "../Menus/MenuCredits/MenuCredits.h"
#include "../Menus/MenuOptions/MenuOptions.h"
#include "../Menus/MenuSound/MenuSound.h"
#include "../Menus/MenuLoading/MenuLoading.h"
#include "../Menus/MenuControls/MenuControls.h"
#include "../Menus/MenuPause/MenuPause.h"
#include "../Menus/MenuRanking/MenuRanking.h"
#include "../Menus/MenuCarSelection/MenuCarSelection.h"
#include "../Car/TrafficCar/TrafficCar.h"
#include "../Gui/Huds/HudRound/HudRound.h"
#include "../Gui/Huds/HudBonus/HudBonus.h"
#include "../Gui/Huds/HudCheckPoint/HudCheckPoint.h"
#include "../Logger/Logger.h"




/**
 * Represents an AFD that controls
 * the status of the game all the time
 */
class Game {

    private:

        /*
         * Definition of internal intervals to control the time
         * measured by the internal clocks of the game
         */

        const sf::Time shot_delayTime = sf::seconds(1.0);

        const sf::Time shot_delayLap = sf::seconds(0.01);

        const sf::Time bonus_delay = sf::seconds(0.01);

        const sf::Time blink_delay = sf::seconds(0.5f);

        /*
         * Internal clocks of the game
         */

        sf::Clock gameClockTime;

        sf::Clock gameClockLap;

        sf::Clock bonusClock;

        sf::Clock blinkTime;

        // State of the game (AFD)
        State gameStatus;

        // Control if the player car is going to be driven in automatic or manual mode
        bool automaticMode;

        // Control if it is the first game round to be played
        bool firstLoad;

        // Control if the checkpoint hud has to be drawn in the screen
        bool blinkCheckPoint;

        // Control if the public congrats sound must be played at the end of the round
        bool spectatorsCongrats;

        // Control if the public showman sound must be played at the end of the round
        bool showmanCongrats;

        // Control when the checkpoint hud must be displayed
        bool checkPoint;

        // Control if the checkpoint hud has been already displayed
        bool checkPointDisplayed;

        // Time of the scenario
        float time;

        // Time when the checkpoint animation starts
        float checkPointInitial;

        // Time that controls how much the checkpoint hud is being displayed
        float checkPointElapsed;

        // Measure the time
        clock_t tick_timer;

        // Time to complete the scenario
        int timeToPlay;

        // Tenths of bonus
        int decsTimeBonus;

        // Seconds of bonus
        int secondsBonus;

        // Hundredths of seconds of bonus
        int decs_secondBonus;

        // Time to display the bonus hud
        int countHudBonus;

        // Score got by the player
        long long int score;

        // Total score the bonus has been added
        long long int totalScore;

        // Minutes elapsed since the player starts the scenario
        float minutes;

        // Seconds elapsed since the player starts the scenario
        float secs;

        // Hundredths of second elapsed since the player starts the scenario
        float cents_second;

        // Total minutes elapsed since the player starts the game round
        float minutesTrip;

        // Total seconds elapsed since the player starts the game round
        float secsTrip;

        // Total hundredths of second elapsed since the player starts the game round
        float cents_secondTrip;

        // Level where the player is playing
        int level;

        // Time when the checkpoint hud starts to be displayed
        int timeCheck;

        // Position of the map in the tree map hud round panel
        int treeMapPos;

        // Numeric code to select the type AI assigned to a traffic car
        int startCodeAi;

        // Pointer to the current scenario
        Map* currentMap;

        // Vector with all the traffic cars
        vector<TrafficCar*> cars;

        // Pointer to the player car
        PlayerCar* player;

        // Game in pause mode
        bool pauseMode;

        // Escape key pressed
        bool escape;

        // Game over
        bool outOfTime;

        // Player completes a game round
        bool arrival;

        // Control if it is the first game round played
        bool firstGame;

        // Control if the start key (intro is pressed in game over animation)
        bool start;

        // Final animation flag
        bool endingAnimation;

        // Control if the starting round has been played
        bool startingRound;

        // Control if the player has canceled the car selection
        bool carSelectionRefused;

        // Control if the loading biomes thread has failed
        bool failBiomesLoaded;

        // Flag measures to control the interval measures done by the internal clocks
        float elapsed1, elapsed2, elapsed3, elapsed4, bonusTime, elapsedBonusTime;

        // Vector of scenarios
        std::vector<std::vector<Biome>> biomes;

        // Pointer to the goal scenario
        Biome* goalBiome;

        // Identifier of the car selected
        int playerCarSelected;

        // Threads of the game
        thread timeCounter, biomesLoader, trafficCarLoader;

    public:



        /**
         * Thread that loads all the biomes of the game
         * @param input is the module that has all the configuration of the game
         */
        void loadBiomes(Input& input);



        /**
         * Thread that loads all the traffic cars
         * @param input is the module that has all the configuration of the game
         */
        void loadTrafficCars(Input& input);



        /**
         * Thread that controls the continuous update of the timers
         * while the player is playing a game round
         */
        void updateTime();



        /**
         * Default constructor
         */
        Game();



        /**
         * Second constructor
         * @param input is the module that has all the configuration of the game
         */
        Game(Input& input);



        /**
         * Control the actions done by the player while the game round is in course
         * @param input is the module that has all the configuration of the game
         */
        void handleEvent(Input& input, const float& time);



        /**
         * Plays the initial animation at the starting point when
         * the play round starts
         * @param input is the module that has all the configuration of the game
         */
        State startRound(Input& input);



        /**
         * Update the game round status
         * @param input is the module that has all the configuration of the game
         */
        void updateRound(Input& input);



        /**
         * Update the game during the play round status
         * @param input is the module that has all the configuration of the game
         */
        State playRound(Input& input);



        /**
         * Update the end round animation when the player
         * reaches the goal and completes the game round
         * @param input is the module that has all the configuration of the game
         */
        State endRound(Input& input);



        /**
         * Update the game over status animation when the time
         * to complete the scenario is over
         * @param input is the module that has all the configuration of the game
         */
        State gameOverRound(Input& input);



        /**
         * Run the execution of all the game
         * @param input is the module that has all the configuration of the game
         */
        void run(Input& input);
 };

 #endif // GAME_H
