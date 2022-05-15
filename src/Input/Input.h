
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
 * Interface file of the module Audio
 */

#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <utility>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Globals.h"
#include "../Audio/Audio.h"

using namespace std;



/**
 * Represents the different types of resolution available
 */
enum class Resolution : int {
    SCREEN_0,
    SCREEN_1,
    SCREEN_2,
    SCREEN_3,
    __COUNT
};



/**
 * Represents the different types of difficulty levels available
 */
enum class Level_Difficulty : int{
    EASY,
    NORMAL,
    HARD,
    __COUNT
};



/**
 * Represents the different types of traffic levels available
 */
enum class Level_Traffic : int {
    LOW,
    MEDIUM,
    HIGH,
    __COUNT
};



/**
 * Represents the keywords to play and manage the game
 */
enum class Key : int {

    // Driving actions
    ACCELERATE,
    BRAKE,
    TURN_LEFT,
    TURN_RIGHT,
    UP_GEAR,
    DOWN_GEAR,

    // Menu actions
    MENU_UP,
    MENU_DOWN,
    MENU_LEFT,
    MENU_RIGHT,
    MENU_ACCEPT,
    MENU_CANCEL,
    MENU_PAUSE,
    MENU_CREDITS,
    __COUNT
};



/*
 * Represents the Input module of the game
 */
struct Input {

    // Structure of store all the keywords
    sf::Keyboard::Key map[(int)Key::__COUNT];

    // Window of the game
    sf::RenderWindow gameWindow;

    // Level of difficulty (EASY, NORMAL or HARD)
    Level_Difficulty difficulty;

    // Traffic level (LOW, MEDIUM or HIGH
    Level_Traffic traffic;

    // Vector with all the available resolutions
    std::array<std::pair<int, int>, (int)Resolution::__COUNT> resolutions;

    // Index of resolution
    int currentIndexResolution;

    // Scale factors to draw the elements in the screen in both axis X and Y
    float screenScaleX, screenScaleY;

    // Camera depth
    float camD;

    // Control the volume of the effects and the music
    int volumeEffects, volumeMusic;

    // Identifier of soundtrack to play
    int currentSoundtrack;

    // Control if there is any modification in the game Input
    bool modifiedConfig;

    // Checks if there is a settings file with a configuration to load
    bool exitsSettings;



    /**
     * Set the render window properties
     * @param width is the horizontal dimension of the game screen
     * @param height is the vertical dimension of the game screen
     */
    inline void setGameWindow(const int width, const int heigth){

        // Check if the resolution of the game and create the window with that resolution
        if (currentIndexResolution == (int)Resolution::__COUNT)
            gameWindow.create(sf::VideoMode(SCREEN_1.first, SCREEN_1.second), "Out Run", sf::Style::Fullscreen);
        else
            gameWindow.create(sf::VideoMode(width, heigth), "Out Run", sf::Style::Titlebar | sf::Style::Close);

        // Establish the frames per sec and more properties of the window
        gameWindow.setFramerateLimit(FPS);
        gameWindow.setKeyRepeatEnabled(false);
        gameWindow.setVerticalSyncEnabled(true);

        // Establish the view to watch the game with good quality
        gameWindow.setView(sf::View(sf::Vector2f(gameWindow.getSize().x / 2.0f, gameWindow.getSize().y / 2.0f),
                                    sf::Vector2f(gameWindow.getSize().x, gameWindow.getSize().y)));

        // Calculation of the screen factor between the current resolution and the default resolution
        screenScaleX = float(gameWindow.getSize().x) / float(SCREEN_0.first);
        screenScaleY = float(gameWindow.getSize().y) / float(SCREEN_0.second);

        // Assign an icon to the window of the game
        sf::Image i;
        i.loadFromFile("Resources/Icon/Outrun.png");
        gameWindow.setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());
        gameWindow.setMouseCursorVisible(false);
    }



    /**
     * Match and action of the game (driving or navigation) with a concrete key
     * @param action is the action to be matched
     * @param code is the numeric code that identifies the key in the keyboard to be used
     */
    inline void set(const Key action, const sf::Keyboard::Key code) {
        map[(int)action] = code;
    }



    /**
     * Returns the key associated to a concrete action of the game
     * @param action is the action whose associated key is desired to be obtained
     */
    inline const sf::Keyboard::Key &get(Key action) {
        return map[(int)action];
    }



    /**
     * Controls if the code of the keyword pressed matches with the action done
     * @param action is the action done by the player
     * @param code is the identifier of the keyword used to do it
     */
    inline bool equal(const Key action, const sf::Keyboard::Key code) {
        return get(action) == code;
    }



    /**
     * Returns if the player has pressed the key matched with its action
     * @param action is the action done by the player
     * @param event is the interruption event captured
     */
    inline bool pressed(const Key action, const sf::Event &event) {
        return isKeypressedEvent(event) && event.key.code == get(action);
    }



    /**
     * Returns if the type of interruption event is key pressed
     * @param event is the interruption event captured
     */
    inline bool isKeypressedEvent(const sf::Event &event) {
        return event.type == sf::Event::KeyPressed;
    }



    /**
     * Returns if the keyword pressed captured in the interruption
     * event has a available identifier
     */
    inline bool pressedKeyIsValidLetter(const sf::Event &event) {
        return event.key.code >= 0 && event.key.code <= 25;
    }



    /**
     * Returns if the player has released the key matched with its action
     * @param action is the action done by the player
     * @param event is the interruption event captured
     */
    inline bool released(const Key action, const sf::Event &event) {
        return event.type == sf::Event::KeyReleased &&
               event.key.code == get(action);
    }



    /**
     * Returns if the player has holding (pressing) the key matched with its action
     * @param action is the action done by the player
     */
    inline bool held(const Key action) {
        return sf::Keyboard::isKeyPressed(get(action)) &&
                gameWindow.hasFocus();
    }



    /**
     * Returns of the type of interruption event is close the window
     * @param event is the interruption event captured
     */
    inline bool closed(const sf::Event &event) {
        return event.type == sf::Event::Closed;
    }



    /**
     * Default constructor
     */
    Input();



    /**
     * Returns if there is a file with a default configuration to load the game
     * @param path is the path of the file to look for
     */
    bool existSettingsFile(const char path[]);



    /**
     * Load the configuration stored in the settings file of the game
     */
    void loadGameInput();



    /**
     * Writes a file with a default configuration only when this file does not exist
     */
    void writeDefaultInput();



    /**
     * Writes a file with the new configuration specified by the player
     */
    void writeNewInput();



    /**
     * Returns the action name associated to the action done by the player
     * @param action is the action done by the player
     */
    std::string getActionName(const Key action);



    /**
     * Returns the name of the keyword pressed by the player
     * using its numeric identifier
     * @param code is the numeric identifier of the code entered by the player
     */
    std::string getKeyCodeName(const sf::Keyboard::Key code);



    /**
     * Returns the keyword associated to a concrete name of keyword
     * @param code is the name of the keyword introduce
     */
    sf::Keyboard::Key getKeyCode(const std::string code);

};

#endif // INPUT_H

