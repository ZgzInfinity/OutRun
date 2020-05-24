/******************************************************************************
 * @file    Menu.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
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
private:
    const std::vector<std::pair<int, int>> resolutions;
    int resIndex;

public:
    /**
     * Constructor por defecto.
     */
    Config();

    /**
     * Devuelve true si se ha cambiado la resolución de pantalla.
     * @return
     */
    State graphicsMenu();

    sf::RenderTexture w;
    sf::RenderWindow window;
    float screenScale;
    bool isDefaultScreen;

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
};

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
State startMenu(Config &c, bool startPressed = false);

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


#endif //OUTRUN_MENU_HPP
