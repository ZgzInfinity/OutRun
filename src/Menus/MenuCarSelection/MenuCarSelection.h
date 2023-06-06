
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
 * Interface file of the module MenuCarSelection
 */

#pragma once

#ifndef MENU_CAR_SELECTION_H
#define MENU_CAR_SELECTION_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"

class MenuCarSelection : public Menu {

    private:

        // Font of the text indicators
        sf::Font fontMenu;

        // Background image texture
        sf::Texture backgroundMenu;

        // Garage image texture
        sf::Texture garageTexture;

        // Sprite of the background
        sf::Sprite mainMenu;

        // Sprite to store the car texture
        sf::Sprite vehicleCar;

        // Sprite to store the shadow texture
        sf::Sprite shadow;

        // Main panel rectangle
        sf::RectangleShape mainPanel;

        // Vehicle sub-panel
        sf::RectangleShape vehiclePanel;

        // Title of the menu
        sf::Text titleMenuText;

        // Description of the menu navigation to change the color car
        sf::Text carDescriptionText;

        // Triangle left sprite
        sf::CircleShape leftTriangle;

        // Triangle right sprite
        sf::CircleShape rightTriangle;

        // Texture of the shadow that makes the cars
        sf::Texture shadowTexture;

        // Matrix with the textures of the car in the different available colors
        sf::Texture vehiclePlayerTextures[TOTAL_PLAYER_CARS][TOTAL_COLOR_TEXTURES];

        // Index of the color car selected
        int colorCarSprite;

        // Flag to control if the key to move left has been pressed
        bool cursorLeftPressed;

        // Flag to control if the key to move right has been pressed
        bool cursorRightPressed;

        // Factor of resolution to control the size of the car
        float factorResolution;

        // Game mode selected by the player
        GameMode gameMode;

    public:



        /**
         * Default constructor
         */
        MenuCarSelection();



        /**
         * Returns if the player has selected a color car or not
         */
        int hasPlayerCarSelected();


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

#endif // MENU_CAR_SELECTION_H
