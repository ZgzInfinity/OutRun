
/*
 * Copyright (c) 2023 Ruben Rodriguez
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
 * Interface file of the module MenuGame
 */

#pragma once

#ifndef MENU_GAME_H
#define MENU_GAME_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"



/**
 * Represents the menu where the player can select to play
 * or to go to the options menu
 */
class MenuGame : public Menu {

    private:

        // Texture with the background of the menu
        sf::Texture backgroundMenu;

        // Texture of the game Icon tag
        sf::Texture gameIcon;

        // Texture of the row that select the action to do in the menu
        sf::Texture rowSelector;

        // Sprite of the background of the menu
        sf::Sprite background;

        // Sprite to store the name of the game
        sf::Sprite nameGame;

        // Sprite of the row
        sf::Sprite row;

        // Vector with all the game icon textures
        vector<sf::Texture> gameIcons;

        // Vector with all the sprites of the game icons
        vector<sf::Sprite> nameGames;

        // Options of the main menu
        const static int ELEMENTS = 2;

        // Text indicators of the menu
        sf::Text textElements[ELEMENTS];

        // Font of the texts indicators
        sf::Font fontMenu;

    public:



        /**
         * Default constructor
         */
        MenuGame();



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

#endif // MENU_GAME_H

