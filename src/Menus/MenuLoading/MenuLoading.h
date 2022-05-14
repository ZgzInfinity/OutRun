
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
 * Interface file of the module MenuLoading
 */

#pragma once

#ifndef MENU_LOADING_H
#define MENU_LOADING_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"



/**
 * Represents the menu loading of the game to make the animation menu
 */
class MenuLoading : public Menu {

    private:

        // Rectangle to locate the background of the menu
        sf::IntRect background;

        // Background of the menu
        sf::Texture iconBackground;

        // Main panel of the menu
        sf::Texture panel;

        // Sprite that stores the background texture
        sf::Sprite sprite;

        // Sprite that stores the main panel texture
        sf::RectangleShape mainPanel;

        // Vector of buttons of the menu
        vector<Button> menuButtons;

        // Vector of point to make the loading animation
        vector<string> points;

        // Font for the text indicators
        sf::Font fontMenu;

        // Text indicator to show the available controllers
        sf::Text controlsText;

        // Loading text indicator
        sf::Text loadingText;

        // Text indicator to draw the points
        sf::Text pointText;

        // Total of points to be drawn
        int totalPoints;

        // Distance between the load points and the loading text indicator
        int offsetText;

        // Control if the player has selected automatic transmission in order to show the controllers
        bool automaticMode;

    public:



        /**
         * Default constructor
         * @param _automaticMode controls if the player
         * has selected automatic transmission in order to show the controllers
         */
        MenuLoading(const bool& _automaticMode);



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

#endif // MENU_LOADING_H
