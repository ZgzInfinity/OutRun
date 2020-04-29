/******************************************************************************
 * @file    Game.hpp
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
#include "Vehicle.hpp"


const int MAX_TIME = 75;
const unsigned long MAX_SCORE = 4294967295;

/**
 * Lógica del juego que contiene la información del jugador, la información de los mapas y el HUD.
 * La información de los mapas está compuesta por un conjunto de objetos Map y su jerarquía de conexión entre ellos.
 * El HUD está formado por un cuadro de texto donde aparece la velocidad del jugador.
 */
class Game {
    // Map info
    std::vector<std::vector<Map>> maps; // 5 blocks of 15 maps {(0), (1, 2), (3, 4, 5), (6, 7, 8, 9), (10, 11, 12, 13, 14))}
    std::pair<int, int> mapId; // (Block, num. map), ex: map0 = (0, 0); map1 = (1, 0); map2 = (1, 1); map14 = (4, 4)
    Map *currentMap;

    Vehicle player;
    std::vector<Vehicle> cars;

    // HUD
    sf::Text sText;

    // Vector of textures for represent the game panel
    vector<Texture> textures;

    // Vector of sprites for represent the game panel
    vector<Sprite> sprites;

    // Time to play
    int time;

    // Score of the player
    unsigned long score;

    // Minutes of game played
    int minutes = 0;

    // Seconds of game played
    int secs = 0;

    // decs of game played
    int decs_second = 0;

    // Available time to arrive to the next checkpoint
    Text timeToPlay;

    // Score of the player displayed in the panel
    Text textScore;

    // Time of lap consumed
    Text textLap;

    // Level indicator
    Text textLevel;

    // Game over text
    Text gameOver;

    // Clock counter of time
    Clock gameClockTime;

    // Clock counter of time
    Clock gameClockLap;

    // Actual level
    int level;

    // End of game
    bool finalGame;

    /**
     * Actualiza la lógica de los mapas y vehículos y dibuja el fragmento de mapa actual con los vehículos en la pantalla.
     * @param c
     */
    void updateAndDraw(Config &c);

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
