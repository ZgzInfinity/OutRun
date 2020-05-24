/******************************************************************************
 * @file    Game.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
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

#define SCORE_BONIFICATION 1000000.0f

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
    float timeMul;
    float timeAI;

    // Score of the player
    long long int score;
    float scoreMul;

    // Minutes of game played
    float minutes = 0;

    // Seconds of game played
    float secs = 0;

    // decs of game played
    float cents_second = 0;

    // Minutes of game played
    float minutesTrip = 0;

    // Seconds of game played
    float secsTrip = 0;

    // decs of game played
    float cents_secondTrip = 0;

    // Clock counter of time
    sf::Clock gameClockTime;

    // Clock counter of time
    sf::Clock gameClockLap;

    // Clock to control the woman shout
    sf::Clock womanShot;

    // Control the sound of the traffic
    sf::Clock trafficCarSound;

    // Control the
    sf::Clock blinkTime;

    // Discount time bonus
    sf::Clock bonus;

    // Time to update the clock counter lap
    sf::Time woman_delay;

    // Time to reproduce a new traffic car sound
    sf::Time traffic_delay;

    // Time to reproduce a new traffic car sound
    sf::Time blink_delay;

    // Time to reproduce a discount the time bonus
    sf::Time bonus_delay;

    // Actual level
    int level;

    // End of game
    bool finalGame;

    // Control the game
    bool inGame;

    // Pause of the game
    bool onPause;

    // Control the options menu
    bool comeFromOptions;

    // Control the checkpoint
    bool checkPoint = false;
    int timeCheck = 0;

    bool blink;

    bool arrival;

    // Counter time to update the HUD indicators
    float elapsed1, elapsed2, elapsed3, elapsed4,
            elapsed5, elapsed6, elapsed7, elapsed8,
            elapsed9, elapsed10, elapsed11, elapsed12;

    // Vector of textures for represent the game panel
    std::vector<sf::Texture> textures;

    // Tree map with levels
    sf::Texture treeMap[5][5];

    std::string lap, lapCheckPoint;

    /**
     * Dibuja el HUD en pantalla.
     * @param c
     */
    void drawHUD(Config &c);

    /**
     * Muestra el mensaje de checkpoint en pantalla.
     * @param c
     * @param visible
     */
    void drawCheckpoint(Config &c, bool visible);

    /**
     * Muestra el mensaje de GAME OVER en pantalla.
     * @param c
     */
    static void drawGameOver(Config &c);

    /**
     * Muestra el mensaje de bonus en pantalla.
     * @param c
     * @param seconds
     * @param decs_second
     */
    void drawBonus(Config &c, int seconds, int decs_second);

    /**
     * Muestra la animación inicial del comienzo de la partida.
     * @param c
     */
    State initialAnimation(Config &c);

    /**
     * Muestra la animación final de la partida.
     * @param c
     */
    State goalAnimation(Config &c);

    /**
     * Actualiza la lógica de los mapas y vehículos y dibuja el fragmento de mapa actual con los vehículos en la pantalla.
     * @param c
     */
    void updateAndDraw(Config &c, Vehicle::Action &action, Vehicle::Direction &direction);

public:
    /**
     * Inicializa la lógica del juego y carga los vehículos y los mapas.
     * @param c
     */
    explicit Game(Config &c);

    /**
     * Comprueba la dificultad y ajusta los parámetros correspondientes.
     * @param c
     */
    void checkDifficulty(Config &c);

    /**
     * Devuelve true si se está en medio de una partida.
     * @return
     */
    bool isInGame() const;

    /**
     * Devuelve la puntuación total.
     * @return
     */
    unsigned long getScore() const;

    /**
     * Devuelve los minutos totales.
     * @return
     */
    float getMinutesTrip() const;

    /**
     * Devuelve los segundos totales.
     * @return
     */
    float getSecsTrip() const;

    /**
     * Devuelve las centésimas de segundo totales.
     * @return
     */
    float getCents_SecondTrip() const;

    /**
     * Actualiza la lógica del juego y lo actualiza la pantalla hasta que se sale del juego.
     * @param c
     * @return
     */
    State play(Config &c);

    /**
     * Menú de pausa.
     * @param c
     * @param a
     * @param d
     * @return
     */
    State pause(Config &c, const Vehicle::Action &a, const Vehicle::Direction &d);
};


#endif //OUTRUN_GAME_HPP
