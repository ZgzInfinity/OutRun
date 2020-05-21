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
#include "KeywordMapper.hpp"
#include "Score.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define SCREEN_DEFAULT_X 921
#define SCREEN_DEFAULT_Y 691

sf::Font initializeFontTimePlay();

sf::Font initializeFontSpeed();

sf::Font initializeFontOptions();

const int NUM_SEGA_ICONS = 39;
const int NUM_SOUNDTRACKS = 4;

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
    State graphicsMenu();

    sf::RenderTexture w;
    sf::RenderWindow window;
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

    // max AI aggressiveness level: max probability that the ai will be activated
    float maxAggressiveness;
    bool enableAI;

    bool enablePixelArt;
};

State introAnimation(Config& c);

State optionsMenu(Config& c, const bool& inGame);

State changeCarControllers(Config& c);

State selectMusicSoundtrack(Config& c);

State startMenu(Config &c, bool startPressed = false);

State endMenu(Config &c);

State rankingMenu(Config& c, unsigned long scorePlayerGame, const int minutes, const int decs, const int cents_Second);


#endif //OUTRUN_MENU_HPP
