
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
 * Interface file of the module MenuCredits
 */

#pragma once

#ifndef MENU_CREDITS_H
#define MENU_CREDITS_H

#include <SFML/Graphics.hpp>
#include "Credit.h"
#include "../Menu/Menu.h"



/**
 * Represents the menu were are specified the credits of
 * all the material (sprites, soundtracks and sfx) used to develop the game
 */
class MenuCredits : public Menu {

    private:

        // Font of the text indicators
        sf::Font fontMenu;

        // Texture of the background
        sf::Texture backgroundMenu;

        // Texture to load the game icons
        sf::Texture gameIcon;

        // Sprite to store the background texture
        sf::Sprite mainMenu;

        // Sprite to store the game icon textures
        sf::Sprite nameGame;

        // Main panel that contains the credits information
        sf::RectangleShape mainPanel;

        // Second panel to result the credits information
        sf::RectangleShape blackShape;

        // Title of the menu
        sf::Text titleText;

        // Text that contains the technical details of the game
        sf::Text gameDetailText;

        // Text that explains the rights of the game
        sf::Text rightText;

        // Text that contains the credit information of the game
        sf::Text informationText;

        // Vector of the game icon textures
        vector<sf::Texture> gameIcons;

        // Vector of sprites of the game icons
        vector<sf::Sprite> nameGames;

        // Offset of the main panel animation move in axis X
        float offsetY;

        // Offset of the main title text indicator move in axis X
        float offsetTitleText;

        // Offset of the description text indicator move in axis X
        float offsetDescriptionMenuText;

        /*
         * Flags that control the gear intro panel animation
         */

        // Flag of the main title text animation
        bool mainTextArrived;

        // Flag of the description text animation
        bool descriptionTextArrived;

    public:



        /**
         * Default constructor
         */
        MenuCredits();



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

#endif // MENU_CREDITS_H

