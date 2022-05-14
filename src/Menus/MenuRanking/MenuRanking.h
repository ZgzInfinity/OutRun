

/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
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
 * Interface file of the module MenuRanking
 */

#pragma once

#ifndef MENU_RANKING_H
#define MENU_RANKING_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Score/Score.h"



/**
 * Represents the ranking menu where the player introduces his/her credentials
 * if he/she has got a new score record after winning a game round
 */
class MenuRanking : public Menu {

    private:

        // Texture of the background
        sf::Texture backgroundMenu;

        // Sprite of the main menu
        sf::Sprite mainMenu;

        // Internal clock to compute the seconds elapsed to end the menu
        sf::Clock rankingTime;

        // Internal clock to control the blinking effect
        sf::Clock blinkStart;

        // Black rectangle screen to create the darkness transition
        sf::RectangleShape blackShape;

        // Interval time counted by the clock
        const sf::Time ranking_delay = sf::seconds(1.f);

        // Title of the menu
        sf::Text rankingTitle;

        // Score text indicator
        sf::Text scoreIndicator;

        // Name text indicator
        sf::Text playerIndicator;

        // Record text indicator
        sf::Text recordIndicator;

        // Time text indicator to finish the menu
        sf::Text timeCounter;

        // Text that stores the score got by the player
        sf::Text scorePlayer;

        // Text that stores the name of the player
        sf::Text namePlayer;

        // Text that stores the total minutes elapsed by the player to complete the game round
        sf::Text minutesPlayer;

        // Text that stores the total seconds elapsed by the player to complete the game round
        sf::Text secondsPlayer;

        // Text that stores the total hundredths of second elapsed by the player to complete the game round
        sf::Text centsPlayer;

        // Index of a ranking position
        sf::Text index;

        // Text start indicator
        sf::Text start;

        // Time to finish the ranking menu
        int time;

        // Offset position in axis Y of the start text indicator depending on the screen resolution
        int offsetStartIndicator;

        // Offset position in axis Y of the title menu text indicator depending on the screen resolution
        int offsetRankingTitle;

        // Offset position in axis Y of the time counter text indicator depending on the screen resolution
        int offsetTimeCounter;

        // Offset position in axis Y of the score indicator depending on the screen resolution
        int offsetScoreIndicator;

        // Offset position in axis Y of the player name text indicator depending on the screen resolution
        int offsetPlayerIndicator;

        // Offset position in axis Y of the record text indicator depending on the screen resolution
        int offsetRecordIndicator;

        // Offset position in axis Y of the index text indicator depending on the screen resolution
        int  offsetIndexIndicator;

        // Flags to measure the time elapsed since the ranking menu started
        float elapsed1, elapsed2, elapsed3, elapsed4;

        // Vector of scores
        std::vector<Score> scoreRankingPlayer;

        // Control the position where the new record (if it exists has to be inserted) otherwise values -1
        int record;

        // Flag to control the blinking effect
        bool blink;

        // Letters introduced by the player (mandatory 3)
        int lettersIntroduced;

        // Name of the player
        std::string name;

        // Font of the text indicators
        sf::Font fontMenu;

        // Total score reached by the player
        int long long score;

        // Total minutes elapsed by the player to complete the game round
        int minutes;

        // Total seconds elapsed by the player to complete the game round
        int secs;

        // Total hundredths of second elapsed by the player to complete the game round
        int cents_second;

    public:



        /**
         * Default constructor
         * @param _score is the total score reached by the player after finishing a game round
         * @param _minutes are the total minutes elapsed by the player to complete the game round
         * @param _secs are the total seconds elapsed by the player to complete the game round
         * @param _cents_seconds are the total hundredths of second elapsed by the player to complete the game round
         */
        MenuRanking(const int long long _score, const int _minutes, const int _secs, const int _cents_second);



        /**
         * Load the menu with all its configuration
         * @param input is the module that has all the configuration of the game
         */
        void loadMenu(Input& input) override;



        /**
         * Detect an action of the player and executes it
         * @param input is the module that has all the configuration of the game
         */
        void handleEvent(Input& input) override;



        /**
         * Draw the menu in the screen
         * @param input is the module that has all the configuration of the game
         */
        void draw(Input& input) override;



        /**
         * Return the next status of the game after and option of the menu
         * has been selected by the player
         * @param input is the module that has all the configuration of the game
         */
        State returnMenu(Input& input) override;

};

#endif // MENU_RANKING_H

