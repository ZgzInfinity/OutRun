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
#include <random>
#include "Random.hpp"
#include "Menu.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Interface.hpp"

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
    Map goalMap;
    int goalFlagger, goalEnd;

    Player player;
    std::vector<Enemy> cars;
    float lastY;
    bool vehicleCrash;

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

    // Clock counter of time
    Clock gameClockTime;

    // Clock counter of time
    Clock gameClockLap;

    // Clock to control the woman shout
    Clock womanShot;

    // Time to update the clock counter lap
    Time woman_delay;

    // Actual level
    int level;

    // End of game
    bool finalGame;

    // Control the game
    bool inGame;

    // Pause of the game
    bool onPause;

    // Counter time to update the HUD indicators
    float elapsed1, elapsed2, elapsed3, elapsed4, elapsed5, elapsed6;

    /**
     * Muestra la animación inicial del comienzo de la partida.
     * @param c
     */
    void initialAnimation(Config &c);

    /**
     * Muestra la animación final de la partida.
     * @param c
     */
    void goalAnimation(Config &c);

    /**
     * Actualiza la lógica de los mapas y vehículos y dibuja el fragmento de mapa actual con los vehículos en la pantalla.
     * @param c
     */
    void updateAndDraw(Config &c, Vehicle::Action& action, Vehicle::Direction &direction);

public:
    /**
     * Inicializa la lógica del juego y carga los vehículos y los mapas.
     * @param c
     */
    explicit Game(Config &c, Interface& iface);


    bool isInGame();

    /**
     * Actualiza la lógica del juego y lo actualiza la pantalla hasta que se sale del juego.
     * @param c
     * @return
     */
    State play(Config &c, Interface& i);

    State pause(Config &c, Interface& i, const Vehicle::Action& a, const Vehicle::Direction &d);

};


#endif //OUTRUN_GAME_HPP
