
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
 * Interface file of the module Menu
 */

#pragma once

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "../../Input/Input.h"


// Possible states of the game
enum class State : int {
    ANIMATION,
    START,
    GAME,
    OPTIONS,
    MUSIC_CONF,
    CONTROLS,
    GEARS,
    VEHICLE,
    MUSIC,
    LOADING,
    PREPARE_ROUND,
    PLAY_ROUND,
    END_ROUND,
    PAUSE,
    GAME_OVER,
    RANKING,
    CREDITS,
    EXIT,
    __COUNT
};



/**
 * Represents the global interface to build the menus
 */
class Menu {

    protected:

        // Check when the start key is pressed
        bool startPressed;

        // Check when the escape key (close or pause) is pressed
        bool escapePressed;

        // Check when the back key is pressed (return in the menus)
        bool backPressed;

        // Check what configuration option has been selected in the menu
        int optionSelected;

    public:



        /**
         * Default constructor
         */
        Menu(){
            startPressed = false;
            escapePressed = false;
            backPressed = false;
            optionSelected = -1;
        };




        /**
         * Load the menu with all its configuration
         * @param input is the module that has all the configuration of the game
         */
        virtual void loadMenu(Input& input) = 0;



        /**
         * Detect an action of the player and executes it
         * @param input is the module that has all the configuration of the game
         */
        virtual void handleEvent(Input& input) = 0;



        /**
         * Draw the menu in the screen
         * @param input is the module that has all the configuration of the game
         */
        virtual void draw(Input& input) = 0;



        /**
         * Return the next status of the game after and option of the menu
         * has been selected by the player
         * @param input is the module that has all the configuration of the game
         */
        virtual State returnMenu(Input& input) = 0;

};

#endif // MENU_H

