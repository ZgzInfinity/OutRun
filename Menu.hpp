/******************************************************************************
 * @file    Menu.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_MENU_HPP
#define OUTRUN_MENU_HPP

#include <vector>
#include <iostream>
#include "KeywordMapper.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

sf::Font arial();

sf::Font initializeFontTimePlay();

sf::Font initializeFontSpeed();

using namespace std;
using namespace sf;

const int NUM_SEGA_ICONS = 39;
const int NUM_SOUNDTRACKS = 3;
const int NUM_OPTIONS = 5;
const int NUM_CONTROLLERS = 4;


enum Difficult {
    EASY = 0,
    MEDIUM,
    HARD
};


struct Config {
    Config();

    sf::RenderWindow w;

    sf::Keyboard::Key menuKey;
    sf::Keyboard::Key menuUpKey;
    sf::Keyboard::Key menuDownKey;
    sf::Keyboard::Key menuEnterKey;
    sf::Keyboard::Key accelerateKey;
    sf::Keyboard::Key brakeKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key rightKey;

    sf::Font font;

    sf::Font timeToPlay;

    sf:: Font speedVehicle;

    // Vector with all the soundtracks to reproduce
    vector<unique_ptr<Music>> themes;

    float camD; // Camera depth
    int renderLen; // Length rendered

    // Control the volume of the effects and the music
    int volumeEffects = 100, volumeMusic = 100;

    // Difficult level
    Difficult level = EASY;

    // Control of the traffic
    bool trafficControl = true;

    // Control if the configuration has been changed correctly
    bool modifiedConfig = false;

    // Identifier of soundtrack to reproduce
    int currentSoundtrack = 0;

};

enum State {
    ANIMATION,
    START,
    OPTIONS,
    MUSIC,
    GAME,
    PAUSE,
    END,
    EXIT
};


State introAnimation(Config& c);

State optionsMenu(Config& c);

void changeCarControllers(Config& c);

State selectMusicSoundtrack(Config& c);

State startMenu(Config &c);

State pauseMenu(Config &c);

State endMenu(Config &c);


#endif //OUTRUN_MENU_HPP
