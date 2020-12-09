
#ifndef CONFIG_HPP
#define CONFIG_HPP

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


enum State {
    ANIMATION,
    START,
    OPTIONS,
    MUSIC,
    GAME,
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

    const std::vector<std::pair<int, int>> resolutions;
    int resIndex;

    sf::RenderTexture w;
    sf::RenderWindow window;
    float screenScale;
    bool isDefaultScreen;
    bool fullScreen;

    sf::Keyboard::Key menuKey;
    sf::Keyboard::Key menuUpKey;
    sf::Keyboard::Key menuDownKey;
    sf::Keyboard::Key menuEnterKey;
    sf::Keyboard::Key accelerateKey;
    sf::Keyboard::Key brakeKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key rightKey;

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

    // max AI aggressiveness level: max probability that the ai will be activated
    float maxAggressiveness;
    bool enableAI;

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
     */
    Config(const Difficult difficulty, const bool pixelArt, const bool enabledAi, const bool fullScreen, const int axis_x,
           const int axis_y, const string controlLeft,const string controlRight, const string controlAccelerate,
           const string controlBrake, const int volEffects, const int volSoundtracks);



    /**
     * Devuelve true si se ha cambiado la resolución de pantalla.
     * @return
     */
    State graphicsMenu();


};



Font initializeFontTimePlay();



Font initializeFontSpeed();



Font initializeFontOptions();


#endif
