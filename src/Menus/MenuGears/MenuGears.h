
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
 * Interface file of the module MenuGears
 */

#pragma once

#ifndef MENU_GEARS_H
#define MENU_GEARS_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"



/**
 * Represents the menu where the player select the gears transmission of the car
 */
class MenuGears : public Menu {

    private:

        // Font menu for the text indicators of the menu
        sf::Font fontMenu;

        // Background texture of the menu
        sf::Texture backgroundMenu;

        // Texture of the game Icon tag
        sf::Texture gameIcon;

        // Sprite to store the background
        sf::Sprite background;

        // Sprite to store the name of the game
        sf::Sprite nameGame;

        // Main gear panel
        sf::RectangleShape mainPanel;

        // Black shadow to make the darkness transition
        sf::RectangleShape blackShape;

        // Title of the menu
        sf::Text titleMenuText;

        // Description of the menu
        sf::Text descriptionMenuText;

        // Vector with all the game icon textures
        vector<sf::Texture> gameIcons;

        // Vector with all the sprites of the game icons
        vector<sf::Sprite> nameGames;

        // Vector with the gear textures
        vector<sf::Texture> gearIcons;

        // Vector where the textures of the gear are going to be stored
        vector<sf::Sprite> gears;

        // Offset of the main panel animation move in axis X
        float offsetY;

        // Offset of the main title text indicator move in axis X
        float offsetTitleMenuText;

        // Offset of the description text indicator move in axis X
        float offsetDescriptionMenuText;

        /*
         * Flags that control the gear intro panel animation
         */

        // Flag of the main title text animation
        bool mainTextArrived;

        // Flag of the description text animation
        bool descriptionTextArrived;

        // Control if the player has selected automatic or manual mode (true auto and false manual)
        bool automaticMode;

        // Control if the player has pressed a key
        bool pressedKey;

        // Determine if the player has selected a color for the car
        int playerCarSelected;

        // Game mode selected by the player
        GameMode gameMode;

        // Control if it the first time that the menu is loaded
        bool firstLoad;

        // Control it is a new game round
        bool newGame;

    public:



        /**
         * Default constructor
         * @param _playerCarSelected controls if the player has selected a color for the car
         */
        MenuGears(const int _playerCarSelected);



        /**
         * Get if the player has selected automatic mode or not
         */
        bool getAutomaticMode() const;



        /**
         * Set the start menu configuration flags
         * @param _firstLoad controls if it the first load of the menu
         * @param _newGame controls if it is a new game round
         */
        void setMenuGears(const bool _firstLoad, const bool _newGame);



        /**
         * Get if it is the first time that the menu has been loaded
         */
        bool getFirstLoad() const;



        /**
         * Set the game mode to be played
         * @param _gameMode is the default game mode to be played
         */
        void setGameMode(const GameMode _gameMode);



        /**
         * Get the game mode selected by the player
         */
        GameMode getGameMode() const;



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

#endif // MENU_GEARS_H

