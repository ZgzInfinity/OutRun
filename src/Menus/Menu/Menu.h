
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
    MUSIC,
    LOAD_MAPS,
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


class Menu {

    protected:

        bool startPressed;

        bool escapePressed;

        bool backPressed;

        int optionSelected;

    public:

        Menu(){
            startPressed = false;
            escapePressed = false;
            backPressed = false;
            optionSelected = -1;
        };

        virtual void loadMenu(Input& input) = 0;

        virtual void handleEvent(Input& input) = 0;

        virtual void draw(Input& input) = 0;

        virtual State returnMenu(Input& input) = 0;

};

#endif // MENU_H

