
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
 * Interface file of the module MenuControl
 */

#pragma once

#ifndef MENU_CONTROLS_H
#define MENU_CONTROLS_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"



/**
 * Represents the menu that lets the player
 * to personalize the controllers of the game
 */

class MenuControls : public Menu {

    private:

        // Background texture
        sf::Texture backgroundTexture;

        // Texture of the main panel of the menu
        sf::Texture texturePanel;

        // Rectangle that contains the background of the menu
        sf::IntRect backgroundMenu;

        // Sprite that stores the background texture
        sf::Sprite background;

        // Sprite of the main panel of the menu
        sf::RectangleShape mainPanel;

        // Vector of buttons of the menu
        vector<Button> menuButtons;

        // Font of the text indicators
        sf::Font fontMenu;

        // Title of the menu
        sf::Text titleText;

        // Information text of the menu
        sf::Text informationText;

        // Control if a controller has been selected by the player
        bool controlSelected;

    public:



        /**
         * Default constructor
         */
        MenuControls();



        /**
         * Change the option menu selected
         * @param menuUpPressed controls if the menu_up key has been or not pressed
         */
        void changeButtonSelected(const bool menuUpPressed);



        /**
         * Update a game controller key of the game
         * @param input is the module that has all the configuration of the game
         * @param key is the new keyword entered by the player to be assigned to a game controller
         * @param action is the action whose controller has been changed by the player
         */
        void updateGameControl(Input& input, const sf::Keyboard::Key& key, const Key& action);



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

 #endif // MENU_CONTROLS_H

