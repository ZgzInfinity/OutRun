
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
 * Interface file of the module MenuLogo
 */

#pragma once

#ifndef MENU_LOGO_H
#define MENU_LOGO_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Input/Input.h"



/**
 * Represents the main logo introduction of the game
 */
class MenuLogo : public Menu {

    private:

        // Background rectangle to draw the menu
        sf::RectangleShape backgroundMenu;

        // Logo game icon texture
        sf::Texture logoGame;

        // Sprite to store the game icon
        sf::Sprite iconGame;

        // Font type of the texts indicators
        sf::Font fontMenu;

        // Texts indicators to build the text animation
        sf::Text logoZgz, logoInfinityGames, logoZgzCompleted;

        // Position of the icon and texts depending on the screen size
        float offsetY, offsetY2;

    public:



        /**
         * Default constructor
         */
        MenuLogo();



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

#endif // MENU_LOGO_H

