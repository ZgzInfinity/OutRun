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

/**
 * Lógica del juego que contiene la información del jugador, la información de los mapas y el HUD.
 * La información del jugador está compuesta por su velocidad, aceleración, posición x, conjunto de texturas del
 * vehículo, sprite actual y accíon actual y dirección actual.
 * La información de los mapas está compuesta por un conjunto de objetos Map y su jerarquía de conexión entre ellos.
 * El HUD está formado por un cuadro de texto donde aparece la velocidad del jugador.
 */
class Game {
    float speed;
    float acceleration;
    float posX;

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

    /**
     * Actualiza la lógica de los mapas y dibuja el fragmento de mapa actual en la pantalla.
     * @param c
     */
    void mapControl(Config &c);

    /**
     * Actualiza la lógica de la aceleración y frenado del jugador.
     * @param c
     * @return
     */
    Action accelerationControl(Config &c);

    /**
     * Actualiza la lógica de giro del jugador.
     * @param c
     * @return
     */
    Direction rotationControl(Config &c);

    /**
     * Actualiza el sprite del jugador y lo dibuja en la pantalla.
     * @param c
     * @param a
     * @param d
     */
    void drawPlayer(Config &c, Action a, Direction d);

public:
    /**
     * Inicializa la lógica del juego y carga los vehículos y los mapas.
     * @param c
     */
    explicit Game(Config &c);

    /**
     * Actualiza la lógica del juego y lo actualiza la pantalla hasta que se sale del juego.
     * @param c
     * @return
     */
    State play(Config &c);
};


#endif //OUTRUN_GAME_HPP
