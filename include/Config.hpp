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



/*
 * Module Config interface file
 */


#ifndef OUTRUN_CONFIG_HPP
#define OUTRUN_CONFIG_HPP


#include <vector>
#include <iostream>
#include <fstream>
#include "Button.hpp"
#include "KeywordMapper.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define SCREEN_DEFAULT_X 921
#define SCREEN_DEFAULT_Y 691
#define SCREEN_HD_X 1280
#define SCREEN_HD_Y 720

#define FPS 60
// Screen
#define SCREEN_DEFAULT make_pair(SCREEN_DEFAULT_X, SCREEN_DEFAULT_Y)
// HD
#define SCREEN_1 make_pair(SCREEN_HD_X, SCREEN_HD_Y)
// HD+
#define SCREEN_2 make_pair(1366, 768)
// FULL HD
#define SCREEN_3 make_pair(1920, 1080)
// QHD
#define SCREEN_4 make_pair(2560, 1440)
// UHD
#define SCREEN_5 make_pair(3840, 2160)


using namespace sf;



// Possible states of the game
enum State {
    ANIMATION,
    START,
    OPTIONS,
    GEARS,
    MUSIC,
    LOAD_MAPS,
    LOADING,
    GAME,
    RANKING,
    EXIT
};



// Levels of difficulty of the game
enum Difficult {
    PEACEFUL,
    EASY,
    NORMAL,
    HARD
};



/*
 * Represents the configuration module of the game
 */
struct Config {

    // Vector with all the available resolutions
    const std::vector<std::pair<int, int>> resolutions;

    // Index of resolution
    int resIndex;

    // Window of the game
    sf::RenderTexture w;
    sf::RenderWindow window;

    float screenScale;

    // Control if the screen is the default resolution
    bool isDefaultScreen;

    // Control if the game is in full screen mode or not
    bool fullScreen;

    // Controllers of the game
    sf::Keyboard::Key menuKey;
    sf::Keyboard::Key menuUpKey;
    sf::Keyboard::Key menuDownKey;
    sf::Keyboard::Key menuEnterKey;
    sf::Keyboard::Key accelerateKey;
    sf::Keyboard::Key brakeKey;
    sf::Keyboard::Key upGearKey;
    sf::Keyboard::Key lowGearKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key rightKey;

    // Fonts of the game
    Font timeToPlay;
    Font speedVehicle;
    Font options;

    // Vector with all the soundtracks to reproduce
    std::vector<std::unique_ptr<sf::Music>> themes;

    // Vector with all the sound effects to reproduce
    std::vector<std::unique_ptr<sf::Music>> effects;

    const float camD; // Camera depth
    const int renderLen; // Length rendered

    // Control the volume of the effects and the music
    int volumeEffects, volumeMusic;

    // Difficult level
    Difficult level;

    // Control if the configuration has been changed correctly
    bool modifiedConfig;

    // Identifier of soundtrack to reproduce
    int currentSoundtrack;

    // max AI aggressiveness level: max probability that the AI will be activated
    float maxAggressiveness;

    // Control if the AI is active or not
    bool enableAI;

    // Control if the graphics have to drawn with pixel art or not
    bool enablePixelArt;


    /**
     * Assigns to the game the configuration read from the xml configuration file
     * @param difficulty is the difficulty level of the game
     * @param pixelArt controls if the graphics of the game have to be drawn with pixel art effect
     * @param fullScreen controls if the game is in full screen
     * @param axis_x is the width of the screen
     * @param axis_y is the height of the screen
     * @param controlLeft is the code of the key to move the vehicle to the left
     * @param controlLeft is the code of the key to move the vehicle to the right
     * @param controlAccelerate is the code of the key to accelerate the vehicle
     * @param controlSoundtrack is the code of the key to change the soundtrack of the game
     * @param controlUpGear is the code of the key to change to a higher gear
     * @param controlLowGear is the code of the key to change to a lower gear
     */
    Config(const Difficult difficulty, const bool pixelArt, const bool enabledAi, const bool fullScreen, const int axis_x,
           const int axis_y, const string controlLeft,const string controlRight, const string controlAccelerate,
           const string controlBrake, const string controlUpGear, const string controlLowGear, const int volEffects, const int volSoundtracks);



    /**
     * Represents on the screen the graphics menu and returns to options menu
     * @return
     */
    State graphicsMenu();


};



/**
 * Returns the font used to write the time in the elapsed time panel
 * @return
 */
Font initializeFontTimePlay();



/**
 * Returns the font used to represent the HUD during the game
 * @return
 */
Font initializeFontSpeed();




/**
 * Returns the font used to represent all the text indicators in
 * the animations of the game
 * @return
 */
Font initializeFontMenus();

#endif // OUTRUN_CONFIG_HPP
