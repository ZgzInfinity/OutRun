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
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

sf::Font arial();


using namespace std;
using namespace sf;

const int NUM_SEGA_ICONS = 39;
const int NUM_SOUNDTRACKS = 3;

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

    // Buffer to reproduce the soundtracks
    vector<sf::SoundBuffer> reproductor;

    // Vector with the soundtracks to reproduce
    sf::Sound soundtrackList;

    float camD; // Camera depth
    int renderLen; // Length rendered

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

State selectMusicSoundtrack(Config& c);

State startMenu(Config &c);

State pauseMenu(Config &c);

State endMenu(Config &c);


#endif //OUTRUN_MENU_HPP
