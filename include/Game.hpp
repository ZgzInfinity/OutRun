/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */



/*
 * Module Game interface file
 */

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
#include "Globals.hpp"

#define SCORE_BONIFICATION 1000000.0f



/**
 * Game logic containing player information, map information and HUD.
 * The information of the maps is composed by a set of objects Map and its hierarchy of connection between them.
 * The HUD is formed by a text box where the speed of the player appears.
 */
class Game {

    // Map info
    // 5 blocks of 15 maps {(0), (1, 2), (3, 4, 5), (6, 7, 8, 9), (10, 11, 12, 13, 14))}
    std::vector<std::vector<Map>> maps;

    // (Block, num. map), ex: map0 = (0, 0); map1 = (1, 0); map2 = (1, 1); map14 = (4, 4)
    std::pair<int, int> mapId;

    // Current map where the player is driving
    Map *currentMap;

    // Final map where the goal is drawn
    Map goalMap;

    // Position of the goal and the goal flagger in the goal map
    int goalFlagger, goalEnd;

    // Vehicle player
    Player player;

    // Vector with the cars
    std::vector<Enemy> cars;

    // Last position in axis y of the player
    float lastY;

    // Control if the player has crashed against
    bool vehicleCrash;

    // Time to play
    int time;

    // Factor to multiply the time
    float timeMul;

    // Adding time when the player plays with AI active
    float timeAI;

     // Semaphores to control mutual exclusion
    mutex mtx;

    // Internal threads of the game
    thread timer0, timer1, timer2;

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

    // Time to be displayed the checkpoint
    int timeCheck = 0;

    // Control the blink of the text
    bool blink;

    // Control if the player has arrived to the goal
    bool arrival;

    // Counter time to update the HUD indicators
    float elapsed1, elapsed2, elapsed3, elapsed4,
            elapsed5, elapsed6, elapsed7, elapsed8,
            elapsed9, elapsed10, elapsed11, elapsed12;

    // Vector of textures for represent the game panel
    std::vector<sf::Texture> textures;

    // Tree map with levels
    sf::Texture treeMap[5][5];

    // Stores the time inverted in the total race and in the checkpoint animation
    std::string lap, lapCheckPoint;



    /**
     * Updates the time elapsed in the game
     */
    void updateTimeElapsed();



    /**
     * Updates the time of the game to complete the landscape
     */
    void updateTimeLandScape();



    /**
     * Updates the score indicator in the Out Run game mode
     */
    void updateScore();



    /**
     * Shows the HUD of the game
     * @param c is the configuration module of the game
     */
    void drawHUD(Config &c);



    /**
     * Shows the checkpoint animation of the game
     * @param c is the configuration module of the game
     * @param visible controls if the animation has to be drawn or not
     */
    void drawCheckpoint(Config &c, bool visible);



    /**
     * Shows the game over animation at the end of the race
     * @param c is the configuration module of the game
     */
    static void drawGameOver(Config &c);



    /**
     * Shows the bonus animation when the player ends the race
     * @param c is the configuration module of the game
     * @param seconds is the number of seconds inverted in the race
     * @param cents_second is the number of hundreds of second inverted in the race
     */
    void drawBonus(Config &c, int seconds, int cents_second);



    /**
     * Shows the initial animation when the race starts
     * @param c is the configuration module of the game
     */
    State initialAnimation(Config &c);



    /**
     * Shows the goal animation at the end of the race
     * @param c is the configuration module of the game
     */
    State goalAnimation(Config &c);



    /**
     * Updates the logic of the map and vehicles and draws the current map fragment
     * with the vehicles on the screen
     * @param c is the configuration of the game
     * @param action is the action of the player that is going to be processed
     * @param direction is the direction of the player that is going to be processed
     * @param terrain is the kind of terrain of the landscape
     */
    void updateAndDraw(Config &c, Vehicle::Action &action, Vehicle::Direction &direction, const int terrain);



public:



    /**
     * Initialize the game logic and load the vehicles and maps
     * @param c is the module configuration of the game
     */
    explicit Game(Config &c);



    /**
     * Check the difficulty and adjust the corresponding parameters.
     * @param c is the module configuration of the game
     */
    void checkDifficulty(Config &c);



    /**
     * Returns true if you are in the middle of a game
     * @return
     */
    bool isInGame() const;



    /**
     * Returns the total score
     * @return
     */
    unsigned long getScore() const;



    /**
     * Returns the total minutes of the race
     * @return
     */
    float getMinutesTrip() const;



    /**
     * Returns the total seconds of the race
     * @return
     */
    float getSecsTrip() const;



    /**
     * Returns the hundreds of seconds of the race
     * @return
     */
    float getCents_SecondTrip() const;



    /**
     * Updates the logic of the game and refreshes the screen until you leave the game
     * @param c is the configuration module of the game
     * @return
     */
    State play(Config &c);



    /**
     * Stops the game and shows the pause menu
     * @param c is the configuration of the game
     * @param a is the last action done by the player
     * @param d is the last direction done by the player
     * @return
     */
    State pause(Config &c, const Vehicle::Action &a, const Vehicle::Direction &d);

};


#endif // OUTRUN_GAME_HPP
