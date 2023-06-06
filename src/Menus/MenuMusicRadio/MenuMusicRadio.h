
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
 * Interface file of the module MenuMusicRadio
 */

#pragma once

#ifndef MENU_MUSIC_RADIO_H
#define MENU_MUSIC_RADIO_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"



/**
 * Represents the radio music menu where the player
 * can select the soundtrack to be played during the game round
 */
class MenuMusicRadio : public Menu {

    private:

        // Texture that stores the background
        sf::Texture backgroundMusic;

        // Vector with the textures of the music titles
        vector<sf::Texture> musicTitleTextures;

        // Vector with the textures of the radio channels
        vector<sf::Texture> radioTextures;

        // Vector with the textures of the hand positions
        vector<sf::Texture> handTextures;

        // Sprite of the background texture
        sf::Sprite radioMenu;

        // Sprite of the music title
        sf::Sprite music;

        // Sprite of the radio channel
        sf::Sprite radio;

        // Sprite of the hand
        sf::Sprite hand;

    public:



        /**
         * Default constructor
         */
        MenuMusicRadio();



        /**
         * Changes the soundtrack to be listened in the menu
         * @param menuLeftPressed controls if the move left key has been pressed or not
         */
        void changeGameMusic(Input& input, const bool& menuLeft);



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
         */
        State returnMenu(Input& input) override;

};

#endif // MENU_MUSIC_RADIO_H


