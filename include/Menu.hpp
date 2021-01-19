/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
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

#ifndef OUTRUN_MENU_HPP
#define OUTRUN_MENU_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include "Button.hpp"
#include "KeywordMapper.hpp"
#include "Score.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define SCREEN_DEFAULT_X 921
#define SCREEN_DEFAULT_Y 691
#define SCREEN_HD_X 1280
#define SCREEN_HD_Y 720


const int NUM_SOUNDTRACKS = 4;



/**
 * Shows the initial animation of the game
 * @param c is the module configuration of the game
 * @return
 */
State introAnimation(Config &c);



/**
 * Displays the main menu of the game
 * @param c is the module configuration of the game
 * @return
 */
State startMenu(Config &c);



/**
 * Displays the menu of changing controllers of the player
 * @param c is the module configuration of the game
 * @return
 */
State changeCarControllers(Config &c);



/**
 * Displays the menu of volume settings
 * @param c is the module configuration of the game
 * @param inGame controls if the player was playing before entering to the sound menu
 */
State soundMenu(Config &c, const bool &inGame);



/**
 * Shows the options menu of the game
 * @param c is the module configuration of the game
 * @param inGame controls if the player was in game before enter in the options menu
 * @return
 */
State optionsMenu(Config &c, const bool &inGame);



/**
 * Shows the radio menu where the player selects the music to play
 * @param c is the module configuration of the game
 * @return
 */
State selectMusicSoundtrack(Config &c);



/**
 * Shows the ranking menu with best seven players of the Out Run mode
 * @param c is the module configuration of the game
 * @param scorePlayerGame is the score obtained by the player in the race
 * @param minutes is the number of minutes that the game has lasted
 * @param secs is the number of seconds that the game has lasted
 * @param cents_Second is the number of hundredths of second that the game has lasted
 * @return
 */
State rankingMenu(Config &c, unsigned long scorePlayerGame, int minutes, int decs, int cents_Second);



/**
 * Updates the current configuration of the game by a new one modifying
 * the configuration file
 * @param path is the path the configuration file
 * @param difficulty is the level of difficulty selected by the player
 * @param volumeSoundtracks is the volume of the soundtracks selected by the player
 * @param volumeEffects is the volume of the sound effects selected by the player
 * @param pixelArt controls if the graphics must be rendered with pixel art effect or not
 * @param fullScreen controls if the game must be load in full screen or not
 * @param axis_x is the width of the window's game
 * @param axis_y is the height of the window's game
 * @param controlLeft is the key selected by the player to turn the vehicle to the left
 * @param controlRight is the key selected by the player to turn the vehicle to the right
 * @param controlAccelerate is the key selected by the player to accelerate the vehicle
 * @param controlBrake is the key selected by the player to brake the vehicle
 * @param controlSoundtrack is the key selected by the player to change the soundtrack of the game
 * @param controlUpGear is the key selected by the player to change to a higher gear
 * @param controlLowGear is the key selected by the player to change to a lower gear
 */
void storeNewConfiguration(const string path, const Difficult difficulty, const bool enabledAi, const int volumeSoundtracks,
                           const int volumeEffects, const bool pixelArt, const bool fullScreen, const int axis_x, const int axis_y,
                           const string controlLeft, const string controlRight, const string controlAccelerate, const string controlBrake,
                           const string controlUpGear, const string controlLowGear);




State showLoadingAnimation(Config& c, const bool autoMod);



#endif //OUTRUN_MENU_HPP
