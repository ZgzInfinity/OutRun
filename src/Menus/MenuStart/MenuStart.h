
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
 * Interface file of the module MenuStart
 */

#pragma once

#ifndef MENU_START_H
#define MENU_START_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"



/**
 * Represents the main menu of the game
 */
class MenuStart : public Menu {

    private:

        // Background texture of the menu
        sf::Texture backgroundMenu;

        // Texture of the game Icon tag
        sf::Texture gameIcon;

        // Sprite to store the background
        sf::Sprite background;

        // Sprite to store the name of the game
        sf::Sprite nameGame;

        // Vector with all the game icon textures
        vector<sf::Texture> gameIcons;

        // Vector with all the sprites of the game icons
        vector<sf::Sprite> nameGames;

        // Options of the main menu
        const static int ELEMENTS = 7;

        // Vector with all text indicators of the menu
        sf::Text textElements[ELEMENTS];

        // Font of the text indicators
        sf::Font fontMenu;

        // Black rectangle to make the darkness transition
        sf::RectangleShape blackShape;

        // Control the blinking effect of the menu
        float elapsedBlink, elapsedHide;

        // Intern clock that controls the blinking effect
        sf::Clock blinkClcok;

        // Control if the menu components have to blink or not
        bool blink;

        // Control if it the first time that the menu is loaded
        bool firstLoad;

        // Control if the menu has been acceded in pause mode
        bool pauseMode;

        // Control it is a new game round
        bool newGame;

    public:



        /**
         * Default constructor
         */
        MenuStart();



        /**
         * Set the start menu configuration flags
         * @param _firstLoad controls if it the first load of the menu
         * @param _newGame controls if it is a new game round
         */
        void setMenuStart(const bool _firstLoad, const bool _newGame);



        /**
         * Get if it is the first time that the menu has been loaded
         */
        bool getFirstLoad() const;



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

#endif // MENU_START_H

