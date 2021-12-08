
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

/*
 * Module inputig interface file
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


enum class Resolution : int {
    SCREEN_0,
    SCREEN_1,
    SCREEN_2,
    SCREEN_3,
    __COUNT
};



// Levels of difficulty of the game
enum class Level_Difficulty : int{
    EASY,
    NORMAL,
    HARD,
    __COUNT
};


enum class Level_Traffic : int {
    NONE,
    LOW,
    MEDIUM,
    HIGH,
    __COUNT
};


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

    sf::Keyboard::Key map[(int)Key::__COUNT];

    sf::RenderWindow gameWindow;

    // Level of difficulty
    Level_Difficulty difficulty;
    Level_Traffic traffic;

    // Vector with all the available resolutions
    std::array<std::pair<int, int>, (int)Resolution::__COUNT> resolutions;

    // Index of resolution
    int currentIndexResolution;


    float screenScaleX, screenScaleY;

    // Camera depth
    float camD;

    // Control the volume of the effects and the music
    int volumeEffects, volumeMusic;

    // Identifier of soundtrack to reproduce
    int currentSoundtrack;

    // max AI aggressiveness level: max probability that the AI will be activated
    float maxAggressiveness;

    // Control if there is any modification in the game Input
    bool modifiedinputig;

    bool exitsSettings;

    bool existSettingsFile(const char path[]);

    void loadGameInput();

    void writeDefaultInput();

    void writeNewInput();



    /**
     * Default constructor
     */
    Input();


    // set window for focus checks
    inline void setGameWindow(const int width, const int heigth){

        if (currentIndexResolution == (int)Resolution::__COUNT){
            gameWindow.create(sf::VideoMode(SCREEN_1.first, SCREEN_1.second), "Out Run", sf::Style::Fullscreen);
        }
        else {
            // Create the screen with not full screen resolution
            gameWindow.create(sf::VideoMode(width, heigth), "Out Run", sf::Style::Titlebar | sf::Style::Close);
        }

        gameWindow.setFramerateLimit(FPS);
        gameWindow.setKeyRepeatEnabled(false);
        gameWindow.setVerticalSyncEnabled(true);

        gameWindow.setView(sf::View(sf::Vector2f(gameWindow.getSize().x / 2.0f, gameWindow.getSize().y / 2.0f),
        sf::Vector2f(gameWindow.getSize().x, gameWindow.getSize().y)));

        // Calculation of the screen factor between the current resolution and the default resolution
        screenScaleX = float(gameWindow.getSize().x) / float(SCREEN_0.first);
        screenScaleY = float(gameWindow.getSize().y) / float(SCREEN_0.second);

        sf::Image i;
        i.loadFromFile("Resources/Icon/OutRun.png");
        gameWindow.setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());
    }



    // Read/write the key map
    inline void set(const Key action, const sf::Keyboard::Key code) {
        map[(int)action] = code;
    }

    inline const sf::Keyboard::Key &get(Key action) {
        return map[(int)action];
    }

    inline bool equal(const Key action, const sf::Keyboard::Key code) {
        return get(action) == code;
    }

    // Check for key press/release/hold events
    inline bool pressed(const Key action, const sf::Event &event) {
        return isKeypressedEvent(event) && event.key.code == get(action);
    }

    inline bool isKeypressedEvent(const sf::Event &event) {
        return event.type == sf::Event::KeyPressed;
    }

    inline bool pressedKeyIsValidLetter(const sf::Event &event) {
        return event.key.code >= 0 && event.key.code <= 25;
    }

    inline bool released(const Key action, const sf::Event &event) {
        return event.type == sf::Event::KeyReleased &&
               event.key.code == get(action);
    }

    inline bool held(const Key action) {
        return sf::Keyboard::isKeyPressed(get(action)) &&
                gameWindow.hasFocus();
    }

    inline bool closed(const sf::Event &event) {
        return event.type == sf::Event::Closed;
    }

    // returns true if key is accepted in game
    std::string getActionName(const Key action);

    // code based on:
    // https://en.sfml-dev.org/forums/index.php?topic=15226.0
    // returns true if key is accepted in game
    std::string getKeyCodeName(const sf::Keyboard::Key code);

    // code based on:
    // https://en.sfml-dev.org/forums/index.php?topic=15226.0
    sf::Keyboard::Key getKeyCode(const std::string code);

};

#endif // INPUT_H

