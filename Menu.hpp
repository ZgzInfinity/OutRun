/******************************************************************************
 * @file    Menu.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_MENU_HPP
#define OUTRUN_MENU_HPP

#include <SFML/Graphics.hpp>

sf::Font arial();

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

    float camD; // Camera depth
};

enum State {
    START,
    GAME,
    PAUSE,
    END,
    EXIT
};

State startMenu(Config &c);

State pauseMenu(Config &c);

State endMenu(Config &c);


#endif //OUTRUN_MENU_HPP
