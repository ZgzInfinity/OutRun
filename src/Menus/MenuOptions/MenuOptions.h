
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
 * Interface file of the module MenuOptions
 */

#pragma once

#ifndef MENU_OPTIONS_H
#define MENU_OPTIONS_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"
#include "../../Gui/Huds/HudRound/HudRound.h"
#include "../../Gui/Huds/HudCheckPoint/HudCheckPoint.h"
#include "../../Logger/Logger.h"



/**
 * Represents the options menu of the game
 */
class MenuOptions : public Menu {

    private:

        // Background texture of the game
        sf::Texture backgroundTexture;

        // Main panel texture
        sf::Texture panelTexture;

        // Rectangle to locate the background texture
        sf::IntRect backgroundRect;

        // Sprite that stores the background texture
        sf::Sprite background;

        // Main panel to locate the panel texture
        sf::RectangleShape panel;

        // Vector of buttons of the menu
        vector<Button> menuButtons;

        // Font of the text indicators
        sf::Font fontMenu;

        // Title of the menu
        sf::Text titleText;

        // Description indicator of the menu
        sf::Text informationText;

        // Difficulty level of the game
        std::string difficulty;

        // Traffic level of the game
        std::string traffic;

        // Shows the player that in that option there is a sub-menu
        std::string subMenu;

        // Shows that the sub-menu can be entered by the player
        std::string access;

        // Shows the player that the sub-menu configuration has been stored
        std::string saved;

        // Control if the screen has been resized or not
        bool resized;

        // Control if a option configuration has been selected
        bool controlSelected;

        // Control
        bool pauseMode;

    public:



        /**
         * Default constructor
         */
        MenuOptions();



        /**
         * Set the game in pause mode
         * @param _pauseMode controls if the game is in pause mode or not
         */
        void setPauseMode(const bool _pauseMode);



        /**
         * Change the option menu selected
         * @param menuUpPressed controls if the menu_up key has been or not pressed
         */
        void changeButtonSelected(const bool menuUpPressed);



        /**
         * Change the difficulty level of the game
         * @param input is the module that has all the configuration of the game
         * @param event is the current event of the player that has been detected
         */
        void changeDifficulty(Input& input, const sf::Event& event);



        /**
         * Change the traffic level of the game
         * @param input is the module that has all the configuration of the game
         * @param event is the current event of the player that has been detected
         */
        void changeTrafficLevel(Input& input, const sf::Event& event);



        /**
         * Change the resolution of the game
         * @param input is the module that has all the configuration of the game
         * @param event is the current event of the player that has been detected
         */
        void changeResolution(Input& input, const sf::Event& event);



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

#endif // MENU_OPTIONS_H
