
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
 * Interface file of the module MenuSound
 */

#pragma once

#ifndef MENU_SOUND_H
#define MENU_SOUND_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"



/**
 * Represents the configuration menu sound of the game
 */
class MenuSound : public Menu {

    private:

        // Background texture of the menu
        sf::Texture background;

        // Texture of the main panel of the menu
        sf::Texture panel;

        // Rectangle that contains the background menu
        sf::IntRect backgroundMenu;

        // Store the background texture
        sf::Sprite backGroundSprite;

        // Main panel with all the buttons
        sf::RectangleShape mainPanel;

        // Vector of buttons of the menu
        vector<Button> menuButtons;

        // Font of the text indicators
        sf::Font fontMenu;

        // Title text of the menu
        sf::Text titleMenuText;

        // Information text of the menu
        sf::Text infoMenuText;

        // Detect if any option (volume of soundtracks or sfx) has been selected
        bool controlSelected;

    public:



        /**
         * Default constructor
         */
        MenuSound();



        /**
         * Change the option menu selected
         * @param menuUpPressed controls if the menu_up key has been or not pressed
         */
        void changeButtonSelected(const bool menuUpPressed);



        /**
         * Change the level volume of the option selected
         * @param menuLeftPressed controls if the menu_left key has been or not pressed
         */
        void changeVolume(Input& input, const bool menuLeftPressed);



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

#endif // MENU_SOUND_H

