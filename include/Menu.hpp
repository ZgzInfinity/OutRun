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

/**
 * Devuelve la fuente para el tiempo.
 * @return
 */
sf::Font initializeFontTimePlay();

/**
 * Devuelve la fuente para la velocidad.
 * @return
 */
sf::Font initializeFontSpeed();

/**
 * Devuelve la fuente para las opciones.
 * @return
 */
sf::Font initializeFontOptions();

const int NUM_SEGA_ICONS = 39;
const int NUM_SOUNDTRACKS = 4;



/**
 * Animación inicial.
 * @param c
 * @return
 */
State introAnimation(Config &c);



/**
 * Menú de opciones.
 * @param c
 * @param inGame
 * @return
 */
State optionsMenu(Config &c, const bool &inGame);

/**
 * Menú para cambiar los controles.
 * @param c
 * @return
 */
State changeCarControllers(Config &c);

/**
 * Animación para saleccionar la música.
 * @param c
 * @return
 */
State selectMusicSoundtrack(Config &c);

/**
 * Menú inicial.
 * @param c
 * @param startPressed
 * @return
 */
State startMenu(Config &c);

/**
 * Pantalla del ranking.
 * @param c
 * @param scorePlayerGame
 * @param minutes
 * @param decs
 * @param cents_Second
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
 */
void storeNewConfiguration(const string path, const Difficult difficulty, const bool enabledAi, const int volumeSoundtracks,
                           const int volumeEffects, const bool pixelArt, const bool fullScreen, const int axis_x, const int axis_y,
                           const string controlLeft, const string controlRight, const string controlAccelerate, const string controlBrake);



#endif //OUTRUN_MENU_HPP
