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
#include "Button.hpp"
#include "Interface.hpp"
#include "KeywordMapper.hpp"
#include "Score.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

sf::Font initializeFontTimePlay();

sf::Font initializeFontSpeed();

sf::Font initializeFontOptions();

const int NUM_SEGA_ICONS = 39;
const int NUM_SOUNDTRACKS = 4;

enum Difficult {
    PEACEFUL, // Without enemies
    EASY,
    NORMAL,
    HARD
};


struct Config {
private:
    const std::vector<std::pair<int, int>> resolutions;
    int resIndex;

public:
    Config();

    /**
     * Devuelve true si se ha cambiado la resolución de pantalla.
     * @return
     */
    bool graphicsMenu();

    sf::RenderWindow w;
    float screenScale;

    sf::Keyboard::Key menuKey;
    sf::Keyboard::Key menuUpKey;
    sf::Keyboard::Key menuDownKey;
    sf::Keyboard::Key menuEnterKey;
    sf::Keyboard::Key accelerateKey;
    sf::Keyboard::Key brakeKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key rightKey;

    sf::Font timeToPlay;

    sf::Font speedVehicle;

    sf::Font options;

    // Vector with all the soundtracks to reproduce
    vector<unique_ptr<Music>> themes;

     // Vector with all the sound effects to reproduce
    vector<unique_ptr<Music>> effects;

    float camD; // Camera depth
    int renderLen; // Length rendered

    // Control the volume of the effects and the music
    int volumeEffects, volumeMusic;

    // Difficult level
    Difficult level;

    // Control if the configuration has been changed correctly
    bool modifiedConfig;

    // Identifier of soundtrack to reproduce
    int currentSoundtrack;

    // AI aggressiveness level: probability that the ai will be activated (0 is original game; 100 is only AI)
    int aggressiveness;
};

enum State {
    ANIMATION,
    START,
    OPTIONS,
    MUSIC,
    GAME,
    END,
    RANKING,
    EXIT
};

State introAnimation(Config& c);

State optionsMenu(Config& c, const bool& inGame);

void changeCarControllers(Config& c);

State selectMusicSoundtrack(Config& c);

State startMenu(Config &c, bool startPressed = false);

State endMenu(Config &c);

State rankingMenu(Config& c, unsigned long scorePlayerGame, const int minutes, const int decs, const int cents_Second);


#endif //OUTRUN_MENU_HPP
