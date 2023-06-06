
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
 * Main program file of Out Run
 * Author: ZgzInfinity
 * Start date: 20-20-2020
 * End date: 06-06-2023
 */

#include <iostream>

using namespace std;

#include "Input/Input.h"
#include "Game/Game.h"

int main(){

    // Set the input module of the game to let the communication with the player
    Input input;

    // Load the configuration and start the game
    Game g = Game(input);

    // Execute and ending the game
    g.run(input);

    return 0;
}
