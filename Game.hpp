/******************************************************************************
 * @file    Engine.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_GAME_HPP
#define OUTRUN_GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Menu.hpp"
#include "Map.hpp"

class Game {
    float speed;
    float acceleration;
    int posX;

    std::vector<std::vector<Map>> maps; // 5 blocks of 15 maps {(0), (1, 2), (3, 4, 5), (6, 7, 8, 9), (10, 11, 12, 13, 14))}
    std::pair<int, int> mapId; // (Block, num. map), ex: map0 = (0, 0); map1 = (1, 0); map2 = (1, 1); map14 = (4, 4)

    std::vector<sf::Texture> playerTextures;
    sf::Sprite sPlayer;
    int actual_code_image;
    int counter_code_image; // Counter to change actual_code_image
    enum Action {
        NONE,
        BRAKE,
        ACCELERATE
    };
    enum Direction {
        RIGHT,
        TURNLEFT,
        TURNRIGHT
    };

    sf::Text sText;

    void mapControl(Config &c);
    Action accelerationControl(Config &c);
    Direction rotationControl(Config &c);
    void drawPlayer(Config &c, Action a, Direction d);

public:
    explicit Game(Config &c);

    State play(Config &c);
};


#endif //OUTRUN_GAME_HPP
