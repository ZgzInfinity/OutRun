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
 * Module Game implementation file
 */

#include "../include/Game.hpp"
#include <fstream>

using namespace sf;
using namespace std;

#define MAX_SPEED 300.f
#define MAX_SPEED_TRAFFIC_CAR 300.f
#define SPEED_MUL 70.0f
#define MAX_COUNTER 2
#define VEHICLE_DENSITY 3.f // Greater than 0
#define VEHICLE_MIN_DISTANCE 5.0f // Minimum number of rectangles between enemies
#define DEL_VEHICLE 50.0f // Minimum number of rectangles behind the camera to delete the enemy



/**
 * Updates the time elapsed in the game
 */
void Game::updateTimeElapsed(){

    // Clock counter of time
    Clock gameClockLap;

    // Time to update the clock counter lap
    Time shot_delayLap = seconds(0.01);

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    // Local variables
    bool arrived = false , endOfGame = false, pause = false;

    // While the game is in curse
    while(!arrived && !endOfGame && !pause){

        // Check if it is on pause
        mtx.lock();
        pause = onPause;
        mtx.unlock();

        // Not in pause
        if (!pause){

            // Sleep the thread
            this_thread::sleep_for(chrono::milliseconds(50));

            // Get the actual time
            elapsed4 = gameClockLap.getElapsedTime().asSeconds();

            // Check if a tenth of second has passed between both timestamps
            if (elapsed4 - elapsed3 >= shot_delayLap.asSeconds()) {
                mtx.lock();
                cents_second += elapsed4;
                if (cents_second >= 1.f) {
                    cents_second -= 1.f;
                    secs++;
                    if (secs == 60.f) {
                        secs = 0;
                        minutes++;
                    }
                }
                cents_secondTrip += elapsed4;
                if (cents_secondTrip >= 1.f) {
                    cents_secondTrip -= 1.f;
                    secsTrip++;
                    if (secsTrip == 60.f) {
                        secsTrip = 0;
                        minutesTrip++;
                    }
                }
                gameClockLap.restart();
                mtx.unlock();
            }
            // Update the context of the game
            mtx.lock();
            arrived = arrival;
            endOfGame = finalGame;
            pause = onPause;
            mtx.unlock();
        }
        else {
            // Restart the clock to count a new interval
            gameClockLap.restart();
        }
    }
}



/**
 * Updates the time of the game to complete the landscape
 */
void Game::updateTimeLandScape(){

    // Initialize the clock counters of time
    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    // Time to update the clock counter
    Time shot_delayTime = seconds(1.0);

    // Local variables
    bool arrived = false , endOfGame = false, pause = false;

    // While the game is in curse
    while(!arrived && !endOfGame && !onPause){

        // Check if the game is on pause
        mtx.lock();
        pause = onPause;
        mtx.unlock();

        // If not in pause
        if (!pause){

             // Sleep the thread
            this_thread::sleep_for(chrono::milliseconds(50));

            // Get the actual time
            elapsed2 = gameClockTime.getElapsedTime().asSeconds();

            // Check if a second has passed between both timestamps
            if (elapsed2 - elapsed1 >= shot_delayTime.asSeconds()) {
                // Draw time
                mtx.lock();
                if (!finalGame && !currentMap->isGoalMap()){
                    time--;
                }
                mtx.unlock();
                gameClockTime.restart();
            }

            // Update the context of the game
            mtx.lock();
            arrived = arrival;
            endOfGame = finalGame;
            pause = onPause;
            mtx.unlock();
        }
        else {
            // Restart the clock to measure a new interval
            gameClockTime.restart();
        }
    }
}



/**
 * Updates the score indicator in the Out Run game mode
 */
void Game::updateScore(){

    // Control the end of the process
    bool arrived = false , endOfGame = false, pause = false;
    float speed;

    // While the race is still in process
    while(!arrived && !endOfGame && !pause){

        mtx.lock();
        // Check pause
        pause = onPause;
        mtx.unlock();

        if (!pause){

            // Sleep the thread
            this_thread::sleep_for(chrono::milliseconds(50));

            mainMutex.lock();
            speed = player.getRealSpeed();
            mainMutex.unlock();


            // Update the score of the player if the player is not stopped
            if (speed > 0.f && speed <= 150.f) {
                // Add score
                mainMutex.lock();
                score += int(player.getRealSpeed() * scoreMul);
                mainMutex.unlock();
            }
            else if (speed > 150.f && speed <= 250.f) {
                // Add score
                mainMutex.lock();
                score += int(player.getRealSpeed() * (1.5f *scoreMul));
                mainMutex.unlock();
            }
            else if (speed > 250.f && speed <= 300.f) {
                // Add score
                mainMutex.lock();
                score += int(player.getRealSpeed() * (2.0f *scoreMul));
                mainMutex.unlock();
            }
        }
        // Update the context of the game
        mainMutex.lock();
        arrived = arrival;
        endOfGame = finalGame;
        pause = onPause;
        mainMutex.unlock();

    }
}



void Game::loadLandScape(Config& c, const int nm, int timeMap, mutex landScapes[], vector<Map>& vm, vector<string> objectNames){

    // Load the landscape using the correspond path
    Map m(c, "Resources/Maps/Map" + to_string(nm + 1) + "/", "bg/bg1.png", objectNames, false, timeMap);

    // Check if the threads has the permission store the landscapes
    landScapes[nm].lock();

    // Store the landscape in the vector
    vm.push_back(m);

    // Wake up to the next thread
    if (nm != 14){
        landScapes[nm + 1].unlock();
    }
}



/**
 * Initialize the game logic and load the vehicles and maps
 * @param c is the module configuration of the game
 */
Game::Game(Config &c) : player(MAX_SPEED, SPEED_MUL, ACC_INC, 3.2f, 3.2f, MAX_COUNTER,
                               "Vehicles/Ferrari", 0.0f, RECTANGLE), lastY(0), vehicleCrash(false), timeMul(1.0f),
                        scoreMul(1.0f), timeAI(0.0f),
                        goalMap(goalFlagger, goalEnd)
{
     // Index of the map to be processed
    int nm = 0;

    // Vector with the times of the maps
    const int times[] = {110, 62, 62, 59, 58, 54, 53, 50, 50, 47, 47, 48, 47, 50, 45};

    // Vector with the different numbers of elements of each map
    const int nobjects[] = {22, 32, 39, 39, 29, 33, 30, 32, 38, 34, 32, 32, 36, 38, 35};
    for (int i = 0; i < 5; i++) {
        vector<Map> vm;
        // Store the maps
        for (int j = 0; j <= i; j++) {
            // Keep the aliases of the map elements of each map
            vector<string> objectNames;
            objectNames.reserve((unsigned long) nobjects[nm]);
            for (int no = 1; no <= nobjects[nm]; no++)
                objectNames.push_back(to_string(no) + ".png");

            // Create the map and store it
            Map m(c, "Resources/Maps/Map" + to_string(nm + 1) + "/", "bg/bg1.png", objectNames, false, times[nm]);
            vm.push_back(m);

            nm++;
        }
        maps.emplace_back(vm);
    }

    mapId = make_pair(0, 0);
    currentMap = &maps[mapId.first][mapId.second];
    if (mapId.first < 4)
        currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
    else {
        goalMap.setColors(*currentMap);
        currentMap->addNextMap(&goalMap);
    }


    Texture t;
    // Load the textures of the panel and assign them to their sprites
    for (int i = 1; i <= 7; i++) {
        // Load the texture from the file
        switch (i) {
            case 1:
                t1.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
                break;
            case 2:
                t2.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
                break;
            case 3:
                t3.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
                break;
            case 4:
                t4.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
                break;
            case 5:
                t5.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
                break;
            case 6:
                t6.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
                break;
            case 7:
                t7.loadFromFile("Resources/Hud/" + to_string(i) + ".png");
        }
    }

    // Code of first Map
    int idFirstMap = 9;

    // Fill the matrix with the tree maps
    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= i; j++) {
            t.loadFromFile("Resources/Hud/" + to_string(idFirstMap) + ".png");
            treeMap[i][j] = t;
            idFirstMap++;
        }
    }

    time = int(float(currentMap->getTime()) * timeMul);
    score = 0;
    level = mapId.first + 1;

    // Control if the player is still playing
    finalGame = false;
    inGame = false;
    onPause = false;
    comeFromOptions = false;
    blink = false;
    arrival = false;
    autoMode = false;
    woman_delay = seconds(5.0f);
    traffic_delay = seconds(2.f);
    blink_delay = seconds(0.5f);
    bonus_delay = seconds(0.01f);

    checkDifficulty(c); // Loads enemies and time
}



/**
 * Shows the HUD of the game
 * @param c is the configuration module of the game
 */
void Game::drawHUD(Config &c) {
    Sprite s;

    // Assign positions in the game console for the game panel indicators
    const float up = float(c.w.getSize().y) / 10.0f;

    // UP
    s.setTexture(t1, true);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    const float separation = s.getGlobalBounds().width / 3.0f;
    s.setPosition(separation, up - 1.1f * s.getGlobalBounds().height);
    float initial = separation + s.getGlobalBounds().width + separation;
    c.w.draw(s);

    // Initialize the HUD indicator of time
    // Available time to arrive to the next checkpoint
    Text timeToPlay;
    timeToPlay.setString("000");
    timeToPlay.setFont(c.timeToPlay);
    timeToPlay.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    timeToPlay.setFillColor(Color::Yellow);
    timeToPlay.setOutlineColor(Color::Black);
    timeToPlay.setOutlineThickness(3.0f * c.screenScale);
    timeToPlay.setPosition(initial, up - 1.1f * float(timeToPlay.getCharacterSize()));
    timeToPlay.setString(to_string(time));
    c.w.draw(timeToPlay);
    initial += timeToPlay.getGlobalBounds().width + separation;

    s.setTexture(t2, true);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    s.setPosition(initial, up - 1.1f * s.getGlobalBounds().height);
    c.w.draw(s);
    initial += s.getGlobalBounds().width + separation;

    // Initialize the HUD indicator of score
    // Score of the player displayed in the panel
    Text textScore;
    textScore.setString(to_string(int(SCORE_BONIFICATION) * 100));
    textScore.setFont(c.timeToPlay);
    textScore.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textScore.setFillColor(Color(183, 164, 190));
    textScore.setOutlineColor(Color::Black);
    textScore.setOutlineThickness(3.0f * c.screenScale);
    textScore.setPosition(initial, up - 1.15f * float(textScore.getCharacterSize()));
    textScore.setString(to_string(score));
    c.w.draw(textScore);

    // Initialize the HUD indicator of lap time
    // Time of lap consumed
    Text textLap;
    textLap.setFont(c.timeToPlay);
    textLap.setString("00' 00'' 00");
    textLap.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textLap.setFillColor(Color(146, 194, 186));
    textLap.setOutlineColor(Color::Black);
    textLap.setOutlineThickness(3.0f * c.screenScale);
    initial = float(c.w.getSize().x) - separation - textLap.getGlobalBounds().width;
    textLap.setPosition(initial, up - 1.15f * float(textLap.getCharacterSize()));

    s.setTexture(t3, true);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    initial -= separation + s.getGlobalBounds().width;
    s.setPosition(initial, up - 1.1f * s.getGlobalBounds().height);

    textLap.setString(lap);
    c.w.draw(textLap);
    c.w.draw(s);

    // DOWN
    Texture t8;
    t8.loadFromFile("Resources/Hud/8.png");
    s.setTexture(t8, true);
    s.setScale(2.f * c.screenScale, 1.5f * c.screenScale);
    const float down = float(c.w.getSize().y) - s.getGlobalBounds().height * 1.5f;
    s.setPosition(separation, float(c.w.getSize().y) - s.getGlobalBounds().height * 1.25f);
    initial = separation + s.getGlobalBounds().width / 4.0f;
    t8.loadFromFile("Resources/Hud/8.png",
                             IntRect(0, 0, static_cast<int>(player.getRealSpeed() * 117.0f / MAX_SPEED * c.screenScale),
                                     static_cast<int>(20.0f * c.screenScale)));
    s.setTexture(t8, true);
    if (player.getRealSpeed() > 0.0f)
        c.w.draw(s);

    // Text
    // HUD
    Text sText;
    sText.setFont(c.speedVehicle);
    sText.setString("0001");
    sText.setCharacterSize(static_cast<unsigned int>(int(70.0f * c.screenScale)));
    sText.setFillColor(Color(206, 73, 73));
    sText.setOutlineColor(Color::Black);
    sText.setOutlineThickness(3.0f * c.screenScale);
    sText.setPosition(initial - (sText.getLocalBounds().width * 0.26f), down - float(sText.getCharacterSize()));
    initial += sText.getGlobalBounds().width;
    string strSpeed = to_string(player.getRealSpeed());
    sText.setString(strSpeed.substr(0, strSpeed.find('.')));
    c.w.draw(sText);

    s.setTexture(t4, true);
    s.setScale(2.f * c.screenScale, 2.f * c.screenScale);
    s.setPosition(initial / 1.2f, down - s.getGlobalBounds().height);
    c.w.draw(s);

    s.setTexture(treeMap[mapId.first][mapId.second], true);
    s.setScale(4.f * c.screenScale, 4.f * c.screenScale);
    initial = float(c.w.getSize().x) - separation - s.getGlobalBounds().width;
    s.setPosition(initial, down - s.getGlobalBounds().height - 1.f);
    c.w.draw(s);

    // Initialize the HUD stage indicator
    // Level indicator
    Text textLevel;
    textLevel.setFont(c.timeToPlay);
    textLevel.setString("0");
    textLevel.setCharacterSize(static_cast<unsigned int>(int(40.0f * c.screenScale)));
    textLevel.setFillColor(Color(146, 194, 186));
    textLevel.setOutlineColor(Color::Black);
    textLevel.setOutlineThickness(3.0f * c.screenScale);
    initial -= separation + textLevel.getGlobalBounds().width;
    textLevel.setPosition(initial, down - 1.05f * float(textLevel.getCharacterSize()));
    textLevel.setString(to_string(level));
    c.w.draw(textLevel);

    textLevel.setOutlineThickness(3.0f * c.screenScale);
    textLevel.setPosition(initial / 1.04f, down - 2.15f * float(textLevel.getCharacterSize()));
    textLevel.setString(to_string(player.getGearPlayer()));
    c.w.draw(textLevel);

    s.setTexture(t5, true);
    s.setScale(2.7f * c.screenScale, 2.7f * c.screenScale);
    initial -= separation + s.getGlobalBounds().width;
    s.setPosition(initial, down - s.getGlobalBounds().height);
    c.w.draw(s);

    s.setTexture(t6, true);
    s.setScale(2.7f * c.screenScale, 2.7f * c.screenScale);
    s.setPosition(initial, down - s.getGlobalBounds().height * 2.8f);
    c.w.draw(s);
}



/**
 * Shows the checkpoint animation of the game
 * @param c is the configuration module of the game
 * @param visible controls if the animation has to be drawn or not
 */
void Game::drawCheckpoint(Config &c, bool visible) {
    Sprite s;

    // CheckPoint title
    Text checkPointTitle;
    checkPointTitle.setFont(c.timeToPlay);
    checkPointTitle.setString("CHECKPOINT!");
    checkPointTitle.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));

    checkPointTitle.setOutlineThickness(3.0f * c.screenScale);
    float initial = c.w.getSize().y / 3.0f + 0.25f * float(checkPointTitle.getCharacterSize());
    checkPointTitle.setPosition((float(c.w.getSize().x) - checkPointTitle.getGlobalBounds().width) / 2.0f,
                                c.w.getSize().y / 3.0f - float(checkPointTitle.getCharacterSize()));

    // Time inverted by the player for complete the game
    s.setTexture(t7, true);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    s.setPosition((float(c.w.getSize().x) - s.getGlobalBounds().width) / 2.0f, initial);
    initial += s.getGlobalBounds().height * 1.25f;

    // Check if the animation has to be drawn or not
    if (visible) {
        checkPointTitle.setFillColor(Color::Yellow);
        checkPointTitle.setOutlineColor(Color(14, 29, 184));
        c.w.draw(s);
        c.w.draw(checkPointTitle);

    } else {
        checkPointTitle.setFillColor(Color::Transparent);
        checkPointTitle.setOutlineColor(Color::Transparent);
    }

    // Text with the time inverted
    Text textForLap;
    textForLap.setFont(c.timeToPlay);
    textForLap.setString("00' 00'' 00");
    textForLap.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textForLap.setOutlineThickness(3.0f * c.screenScale);

    s.setTexture(t3, true);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    textForLap.setPosition(
            (float(c.w.getSize().x) - textForLap.getGlobalBounds().width - s.getGlobalBounds().width) / 1.8f,
            initial + 1.1f * s.getGlobalBounds().height);
    s.setPosition((float(c.w.getSize().x) + textForLap.getGlobalBounds().width / 2.0f) / 2.4f, initial);

    // Check if the animation has to be drawn or not
    if (visible) {
        textForLap.setFillColor(Color(146, 194, 186));
        textForLap.setOutlineColor(Color::Black);
        c.w.draw(s);
    } else {
        textForLap.setFillColor(Color::Transparent);
        textForLap.setOutlineColor(Color::Transparent);
    }

    textForLap.setString(lapCheckPoint);
    c.w.draw(textForLap);
}



/**
 * Shows the game over animation at the end of the race
 * @param c is the configuration module of the game
 */
void Game::drawGameOver(Config &c) {
    // Game over text
    Text gameOver;
    gameOver.setFont(c.timeToPlay);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(static_cast<unsigned int>(int(60.0f * c.screenScale)));
    gameOver.setFillColor(Color::Yellow);
    gameOver.setOutlineColor(Color(14, 29, 184));
    gameOver.setOutlineThickness(3.0f * c.screenScale);
    gameOver.setPosition((float(c.w.getSize().x) - gameOver.getGlobalBounds().width) / 2.0f,
                         (float(c.w.getSize().y) - float(gameOver.getCharacterSize())) / 2.0f);
    c.w.draw(gameOver);
}



/**
 * Shows the bonus animation when the player ends the race
 * @param c is the configuration module of the game
 * @param seconds is the number of seconds inverted in the race
 * @param cents_second is the number of hundreds of second inverted in the race
 */
void Game::drawBonus(Config &c, int seconds, int cents_second) {
    // Only for separation
    Sprite s;
    s.setTexture(t1, true);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    s.setScale(2.5f * c.screenScale, 2.5f * c.screenScale);
    const float separation = s.getGlobalBounds().width / 3.0f;

    // Final score after completing all the levels
    Text bonification;
    bonification.setFont(c.timeToPlay);
    bonification.setString("BONUS POINTS!");
    bonification.setCharacterSize(static_cast<unsigned int>(int(65.0f * c.screenScale)));
    bonification.setFillColor(Color::Yellow);
    bonification.setOutlineColor(Color(12, 12, 12));
    bonification.setOutlineThickness(3.0f * c.screenScale);
    float initialY = float(c.w.getSize().y) / 3.0f + float(bonification.getCharacterSize());
    float initial = (float(c.w.getSize().x) - bonification.getGlobalBounds().width) / 2.0f;
    bonification.setPosition(initial, float(c.w.getSize().y) / 3.0f - float(bonification.getCharacterSize()));
    c.w.draw(bonification);

    // Time bonus to the player
    Text timeBonus;
    timeBonus.setFont(c.timeToPlay);
    timeBonus.setString("000.0");
    timeBonus.setCharacterSize(static_cast<unsigned int>(int(55.0f * c.screenScale)));
    timeBonus.setFillColor(Color::Yellow);
    timeBonus.setOutlineColor(Color(12, 12, 12));
    timeBonus.setOutlineThickness(3.0f * c.screenScale);
    initial -= timeBonus.getLocalBounds().width;
    timeBonus.setPosition(initial, initialY);
    initialY += float(timeBonus.getCharacterSize());
    initial += 1.25f * timeBonus.getLocalBounds().width;
    timeBonus.setString(to_string(seconds) + "." + to_string(cents_second));
    c.w.draw(timeBonus);

    // Seconds arrival indicator
    Text secondsIndicator;
    secondsIndicator.setFont(c.timeToPlay);
    secondsIndicator.setString("SEC");
    secondsIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    secondsIndicator.setFillColor(Color(183, 164, 190));
    secondsIndicator.setOutlineColor(Color::Black);
    secondsIndicator.setOutlineThickness(3.0f * c.screenScale);
    secondsIndicator.setPosition(initial, initialY - float(secondsIndicator.getCharacterSize()));
    initial += separation + secondsIndicator.getLocalBounds().width;
    c.w.draw(secondsIndicator);

    // Seconds arrival indicator
    Text crossSign;
    crossSign.setFont(c.timeToPlay);
    crossSign.setString("x");
    crossSign.setCharacterSize(static_cast<unsigned int>(int(40.0f * c.screenScale)));
    crossSign.setFillColor(Color(232, 191, 157));
    crossSign.setOutlineColor(Color::Black);
    crossSign.setOutlineThickness(3.0f * c.screenScale);
    crossSign.setPosition(initial, initialY - float(crossSign.getCharacterSize()));
    initial += separation + crossSign.getLocalBounds().width;
    c.w.draw(crossSign);

    // Score factor to multiply
    Text scoreMultiply;
    scoreMultiply.setFont(c.timeToPlay);
    scoreMultiply.setString(to_string((long long) SCORE_BONIFICATION));
    scoreMultiply.setCharacterSize(static_cast<unsigned int>(int(55.0f * c.screenScale)));
    scoreMultiply.setFillColor(Color::Yellow);
    scoreMultiply.setOutlineColor(Color(12, 12, 12));
    scoreMultiply.setOutlineThickness(3.0f * c.screenScale);
    scoreMultiply.setPosition(initial, initialY - float(scoreMultiply.getCharacterSize()));
    initial += separation + scoreMultiply.getLocalBounds().width;
    c.w.draw(scoreMultiply);

    // Seconds arrival indicator
    Text pointsIndicator;
    pointsIndicator.setFont(c.timeToPlay);
    pointsIndicator.setString("PTS");
    pointsIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    pointsIndicator.setFillColor(Color(183, 164, 190));
    pointsIndicator.setOutlineColor(Color::Black);
    pointsIndicator.setOutlineThickness(3.0f * c.screenScale);
    pointsIndicator.setPosition(initial, initialY - float(pointsIndicator.getCharacterSize()));
    c.w.draw(pointsIndicator);
}



/**
 * Check the difficulty and adjust the corresponding parameters.
 * @param c is the module configuration of the game
 */
void Game::checkDifficulty(Config &c) {
    int numCars = static_cast<int>(cars.size()); // Number of vehicles simultaneously
    time = int(float(time - int(timeAI)) / timeMul); // Restore original time

    float prevScoreMul = scoreMul;
    // Adapt the parameters depending on the difficulty to play
    switch (c.level) {
        case PEACEFUL:
            numCars = 0;
            timeMul = 1.1f;
            scoreMul = 0.25f;
            if (c.enableAI)
                c.maxAggressiveness = 0.0f;
            break;
        case EASY:
            numCars = 15;
            timeMul = 1.1f;
            scoreMul = 0.5f;
            if (c.enableAI)
                c.maxAggressiveness = 0.25f;
            break;
        case NORMAL:
            numCars = 18;
            timeMul = 1.0f;
            scoreMul = 1.0f;
            if (c.enableAI)
                c.maxAggressiveness = 0.5f;
            break;
        case HARD:
            numCars = 22;
            timeMul = 0.9f;
            scoreMul = 1.5f;
            if (c.enableAI)
                c.maxAggressiveness = 0.75f;
            break;
        default:
            break;
    }
    if (prevScoreMul < scoreMul) // Keep the least multiplier
        scoreMul = prevScoreMul;

    if (!c.enableAI)
        c.maxAggressiveness = 0.0f;

    // Vehicles
    cars.reserve(static_cast<unsigned long>(numCars));
    if (cars.size() > numCars) {
        while (cars.size() > numCars)
            cars.pop_back();
    }
    else if (cars.size() < numCars) {
        const int maxSprites = 12;
        const float vehicleScales[maxSprites] = {1.3f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f};
        for (int i = static_cast<int>(cars.size()); i < numCars; i++) {
            Enemy v(MAX_SPEED_TRAFFIC_CAR, SPEED_MUL, vehicleScales[i % maxSprites], MAX_COUNTER,
                    "Vehicles/TrafficCars/Car" + to_string(1 + i % maxSprites), -RECTANGLE * DEL_VEHICLE * 3.0f, 1 + i % maxSprites);
            cars.push_back(v);
        }
    }

    // Assign the type of AI for each car
    for (Enemy &v : cars)
        v.setAI(c.maxAggressiveness);

    // Add the additional time for playing with the AI active
    time = int(float(time) * timeMul);
    timeAI = !cars.empty() ? float(time) * c.maxAggressiveness / 3.0f : 0.0f;
    time += int(timeAI);

    if (time < 0)
        time = 0;

}



/**
 * Returns true if you are in the middle of a game
 * @return
 */
bool Game::isInGame() const {
    return inGame;
}



/**
 * Returns the total score
 * @return
 */
unsigned long Game::getScore() const {
    return static_cast<unsigned long>(score);
}



/**
 * Returns the total minutes of the race
 * @return
 */
float Game::getMinutesTrip() const {
    return minutesTrip;
}



/**
 * Returns the total seconds of the race
 * @return
 */
float Game::getSecsTrip() const {
    return secsTrip;
}



/**
 * Returns the hundreds of seconds of the race
 * @return
 */
float Game::getCents_SecondTrip() const {
    return cents_secondTrip;
}



/**
 * Updates the logic of the game and refreshes the screen until you leave the game
 * @param c is the configuration module of the game
 * @return
 */
State Game::play(Config &c) {

    // Check if the game has to be continued
    if (!inGame) {
        // New game and with initial animation
        inGame = true;
        indexMap = 0;
        checkPointDisplayed = false;
        State status = initialAnimation(c);
        if (status == EXIT){
            return status;
        }
    }

    // Enabled keywords repetition
    c.window.setKeyRepeatEnabled(false);

     // Get the kind of terrain of the landscape
    int terrain = currentMap->getTerrain();

    // Time to update the clock counter
    Time shot_delayTime = seconds(1.0);

    // Time to update the clock counter lap
    Time shot_delayLap = seconds(0.01);

    // Initialize all the clocks

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    womanShot.restart();
    elapsed5 = womanShot.getElapsedTime().asSeconds();

    trafficCarSound.restart();
    elapsed7 = trafficCarSound.getElapsedTime().asSeconds();

    blinkTime.restart();
    elapsed9 = blinkTime.getElapsedTime().asSeconds();

    Vehicle::Action action;
    Vehicle::Direction direction;

    State status;

    int localTime;

    // The options menu was not in the screen
    if (!comeFromOptions){
        // Thread to control the elapsed time
        timer0 = thread(updateTimeElapsed, this);

        // Thread to control the time of the landscape
        timer1 = thread(updateTimeLandScape, this);

        // Thread to counter the score of the game
        timer2 = thread(updateScore, this);
    }

    // While the game is in curse
    while (!finalGame && !arrival && c.window.isOpen()) {

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)){
            // Close the screen
            if (e.type == Event::Closed){
                mtx.lock();
                finalGame = true;
                mtx.unlock();
                timer0.join();
                timer1.join();
                timer2.join();
                return EXIT;
            }
        }

        // Update the game context
        updateAndDraw(c, action, direction, terrain);

        // Check if the game has finished badly
        if (!finalGame) {
            // Check if it is on pause
            if (Keyboard::isKeyPressed(c.menuKey) || onPause) {
                // Pause the game
                c.effects[1]->stop();
                c.effects[1]->play();
                status = pause(c, action, direction);

                // Options
                if (status == OPTIONS) {
                    comeFromOptions = true;
                    mtx.lock();
                    finalGame = true;
                    mtx.unlock();
                    break;
                }
                // Initial menu
                else if (status == START) {
                    mtx.lock();
                    finalGame = true;
                    mtx.unlock();
                    inGame = false;
                    return START;
                }
                // Resume
                else if (status == GAME) {
                    timer0 = thread(updateTimeElapsed, this);
                    timer1 = thread(updateTimeLandScape, this);
                    timer2 = thread(updateScore, this);
                    c.themes[c.currentSoundtrack]->play();
                }
                // Close the game
                else if (status == EXIT) {
                    mtx.lock();
                    finalGame = true;
                    onPause = false;
                    mtx.unlock();
                    sleep(c.effects[2]->getDuration());
                    return EXIT;
                }
                gameClockLap.restart();
            }

            // Update the indicators
            if (!finalGame){
                lap = (minutes < 10) ? "0" + to_string(int(minutes)) + " '" : to_string(int(minutes)) + " ''";
                lap += (secs < 10) ? "0" + to_string(int(secs)) + " ''" : to_string(int(secs)) + " ''";
                lap += to_string(int(cents_second * 100.f));
            }

            // Display the HUD of the game
            drawHUD(c);

            // Display the context in the screen
            Sprite bufferSprite(c.w.getTexture());
            c.w.display();
            c.window.draw(bufferSprite);
            c.window.display();

            // Store the time to play
            mtx.lock();
            localTime = time;
            mtx.unlock();

            if (localTime == 10) {
                c.effects[25]->stop();
                c.effects[25]->play();
            }

            // Check if the player has time to continue
            if (localTime == 0) {
                // Final game
                mtx.lock();
                finalGame = true;
                mtx.unlock();
            }
        }
    }

    // Check the ending of the game
    if (!comeFromOptions){
        timer0.join();
        timer1.join();
        timer2.join();
    }

    mtx.lock();
    if (finalGame){
        finalGame = false;
        mtx.unlock();
    }

    // The game has finished because the game has arrived to the goal
    if (arrival) {
        arrival = false;
        mtx.unlock();
        return RANKING;
    }
    mtx.unlock();

    // The game has finished due to the time has expired
    if (status != OPTIONS && status != START) {
        // Draw the game over text in the console window
        drawGameOver(c);
        c.themes[c.currentSoundtrack]->stop();
        c.effects[24]->stop();
        c.effects[24]->play();
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(c.effects[24]->getDuration());

        bool startPressed = false;
        c.themes[5]->play();
        while (!startPressed) {

            // Detect the possible events
            Event e{};
            while (c.window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    return EXIT;
                }
            }

            startPressed = Keyboard::isKeyPressed(c.menuEnterKey);
        }
        c.themes[5]->stop();
        c.effects[2]->stop();
        c.effects[2]->play();
        return START;
    }
    return status;
}



/**
 * Shows the initial animation when the race starts
 * @param c is the configuration module of the game
 */
State Game::initialAnimation(Config &c) {
    c.themes[c.currentSoundtrack]->stop();
    int flagger, semaphore;
    Map *initMap = new Map(*currentMap, flagger, semaphore);
    initMap->addNextMap(currentMap);
    initMap->setColors(*currentMap);
    currentMap = initMap;

    c.w.clear(Color(0, 0, 0));
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    // Creation of the panel rectangle of the menu
    RectangleShape blackShape;
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(c.w.getSize().x, c.w.getSize().y));

    // Prepare car
    bool end = false;

    // slideCar
    c.effects[38]->stop();
    c.effects[38]->play();

    // Draw the landscape animation
    for (int j = 255; j >= 0; j -= 5){

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Draw map
        currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                         &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

        player.drawStaticAnimation(c);

        blackShape.setFillColor(Color(0, 0, 0, j));
        c.w.draw(blackShape);

        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(milliseconds(30));
    }

    // slideCar
    c.effects[8]->stop();
    c.effects[8]->play();

    int code = 121;

    for (int i = (int) c.w.getSize().x / 2; !end; i -= 3) {

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                         &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

        player.drawInitialAnimation(c, float(i), end, code);
        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        if (i <= (int)(c.w.getSize().x / 2.f) / 1.1){
            code = 11;
        }
        else if (i <= (int)(c.w.getSize().x / 2.f) / 1.23){
            code = 120;
        }
    }

    // Detect the possible events
    Event e{};
    while (c.window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            return EXIT;
        }
    }

    sleep(milliseconds(30));

    // Semaphore and flagger
    currentMap->incrementSpriteIndex(flagger, false, -1);
    int ms = 1000;

    currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                     &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

    player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()), currentMap->getTerrain());
    bufferSprite.setTexture(c.w.getTexture(), true);
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    // Detect the possible events
    while (c.window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            return EXIT;
        }
    }

    // comentaristIntro
    c.effects[11]->stop();
    c.effects[11]->play();
    while (c.effects[11]->getStatus() != SoundSource::Stopped){
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }
    }

    for (int i = 0; i < 3; i++) {

        // Detect the possible events
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                         &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

        player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()), currentMap->getTerrain());
        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        // Flagger
        if (i == 2) {
            for (; ms > 0; ms -= 200) {
                sleep(milliseconds(200));

                currentMap->incrementSpriteIndex(flagger, false);

                // Draw map
                c.w.clear();
                currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                                 &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

                player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT,
                            currentMap->getElevation(player.getPosY()), currentMap->getTerrain());
                bufferSprite.setTexture(c.w.getTexture(), true);
                c.w.display();
                c.window.draw(bufferSprite);
                c.window.display();
            }
        }

        // Change semaphore state
        if (ms > 0) {
            sleep(milliseconds(ms));
            c.effects[4]->stop();
            c.effects[4]->play();
        }
        currentMap->incrementSpriteIndex(semaphore, false);
        c.effects[5]->stop();
        c.effects[5]->play();
    }
    currentMap->incrementSpriteIndex(flagger, false, -1);
    c.themes[c.currentSoundtrack]->play();

    return GAME;
}



/**
 * Shows the goal animation at the end of the race
 * @param c is the configuration module of the game
 */
State Game::goalAnimation(Config &c) {
    // Stop music level
    c.themes[c.currentSoundtrack]->stop();

    // GoalSound
    c.effects[27]->stop();
    c.effects[27]->play();

    // Hide enemies
    for (Enemy &v : cars)
        v.setPosition(v.getPosX(), -RECTANGLE);

    player.setSmoking(false);
    int increment = 0;
    float currentTime = gameClockTime.getElapsedTime().asMilliseconds();

    // Bonus seconds
    int decsTime = time * 10;
    int seconds = decsTime / 10;
    int decs_second = decsTime % 10;

    bonus.restart();
    elapsed11 = bonus.getElapsedTime().asSeconds(); // TODO: Esta marca solo se actualiza aquí ?????

    while (int(player.getPosY()) < goalEnd) {

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                mtx.lock();
                finalGame = true;
                mtx.unlock();
                timer0.join();
                timer1.join();
                timer2.join();
                return EXIT;
            }
        }

        // Update camera
        currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                         &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

        player.setPosition(player.getPosX(), player.getPosY() + 1);
        player.draw(c, Vehicle::Action::ACCELERATE, Vehicle::Direction::RIGHT,
                    currentMap->getElevation(player.getPosY()), currentMap->getTerrain(), false);

        // Flager animation
        if (gameClockTime.getElapsedTime().asMilliseconds() - currentTime >= 200.0f) {
            if (increment >= 5) {
                increment = 0;
                currentMap->incrementSpriteIndex(goalFlagger, false, -5);
            } else {
                currentMap->incrementSpriteIndex(goalFlagger, false);
                increment++;
            }
            currentTime = gameClockTime.getElapsedTime().asMilliseconds();
        }

        elapsed12 = bonus.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed12 - elapsed11 >= bonus_delay.asSeconds()) {
            // Decrement one Tenth of a second

            if (decsTime > 0) {
                decsTime--;

                seconds = decsTime / 10;
                // Decs per second
                decs_second = decsTime % 10;
            }

            score += int(scoreMul * SCORE_BONIFICATION / 10.0f); // Bonif. per dec.

            bonus.restart();
        }

        // Draw the bonus indicators
        drawBonus(c, seconds, decs_second);
        drawHUD(c);

        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
    }

    // Car animation
    int step = 0, lastStep;
    bool firstEnd = false, end = false;

    // slideCar
    c.effects[8]->stop();
    c.effects[8]->play();

    // claps
    c.effects[28]->stop();
    c.effects[28]->play();

    bonus.restart();
    sleep(milliseconds(20));

    while (!end) {

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                mtx.lock();
                finalGame = true;
                mtx.unlock();
                timer0.join();
                timer1.join();
                timer2.join();
                return EXIT;
            }
        }

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                         &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

        if (!firstEnd) {
            player.drawGoalAnimation(c, step, end);
        } else {
            player.drawGoalAnimation(c, lastStep, end, false);
        }

        if (end && !firstEnd) {
            firstEnd = true;
        }
        if (firstEnd) {
            lastStep = step - 1;
        }

        if (decsTime > 0) {
            elapsed12 = bonus.getElapsedTime().asSeconds();

            // Check if a second has passed between both timestamps
            if (elapsed12 - elapsed11 >= bonus_delay.asSeconds()) {
                // Decrement one Tenth of a second
                decsTime--;

                seconds = decsTime / 10;
                // Decs per second
                decs_second = decsTime % 10;

                score += int(scoreMul * SCORE_BONIFICATION / 10.0f); // Bonif. per dec.

                bonus.restart();
            }

            // Draw the bonus indicators
            drawBonus(c, seconds, decs_second);

            // Draw speed
            drawHUD(c);

            end = false;
        }

        // Draw the bonus indicators
        drawBonus(c, seconds, decs_second);

        // Draw speed
        drawHUD(c);

        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
    }
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    while (c.effects[27]->getStatus() != SoundSource::Stopped){
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                mtx.lock();
                finalGame = true;
                mtx.unlock();
                timer0.join();
                timer1.join();
                timer2.join();
                return EXIT;
            }
        }
    }
    return RANKING;
}



/**
 * Updates the logic of the map and vehicles and draws the current map fragment
 * with the vehicles on the screen
 * @param c is the configuration of the game
 * @param action is the action of the player that is going to be processed
 * @param direction is the direction of the player that is going to be processed
 * @param terrain is the kind of terrain of the landscape
 */
void Game::updateAndDraw(Config &c, Vehicle::Action &action, Vehicle::Direction &direction, const int terrain) {
    // Update camera
    currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

    // If the player has left  the current map
    if (currentMap->isOver()) {

        // Update the position o the traffic cars
        for (Enemy &v : cars){
            // Only are relocated the traffic cars with the same direction of the player
            if (v.getDirectionFork() == player.getDirectionFork()){
                v.setPositionX(0.f);
            }
            else {
                // Update the speed and position for the car
                v.update(lastY, lastY + float(c.renderLen) / VEHICLE_DENSITY, c.maxAggressiveness);
                lastY = v.getPosY() + VEHICLE_MIN_DISTANCE * RECTANGLE;
            }
        }

        player.setDirectionFork();

        // New map reached
        checkPointDisplayed = false;
        indexMap++;

        if (currentMap->getNext() != nullptr) {

            // Update player and vehicle positions
            player.setPosition(player.getPosX() + currentMap->getOffsetX(), player.getPosY() - currentMap->getMaxY());


            for (Enemy &v : cars)
                v.setPosition(v.getPosX(), v.getPosY() - currentMap->getMaxY());

            const bool isInitMap = currentMap->isInitMap();
            currentMap = currentMap->getNext();

            if (!isInitMap && !currentMap->isGoalMap()) {
                level++;
                // Update fork maps
                if (currentMap == &maps[mapId.first + 1][mapId.second + 1])
                    mapId.second++;
                mapId.first++;
                if (mapId.first < 4)
                    currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
                else {
                    goalMap.setColors(*currentMap);
                    currentMap->addNextMap(&goalMap);
                }
            }

            currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

            lastY = currentMap->getCamY();
        }
        else {
            finalGame = true;
        }
    }
    else {
        // Get the position of the player
        float posY = player.getPosY();

        // Control the checkpoint animation
        if (!currentMap->isInitMap() && !currentMap->isGoalMap() && indexMap > 1){
            if (posY >= currentMap->getCheckPointLine() && !checkPointDisplayed){

                 // BeepSound
                c.effects[23]->stop();
                c.effects[23]->play();

                // Checkpoint reached
                checkPoint = true;

                // Update time when map changes
                time += int(float(currentMap->getTime()) * timeMul);

                timeCheck = time;
            }
            // Control the lap time in the checkpoint
            if (checkPoint && !checkPointDisplayed){

                lapCheckPoint = (minutes < 10) ? "0" + to_string(int(minutes)) + " '" : to_string(int(minutes)) + " ''";
                lapCheckPoint += (secs < 10) ? "0" + to_string(int(secs)) + " ''" : to_string(int(secs)) + " ''";
                lapCheckPoint += int(cents_second * 100.f) < 10 ? "0" + to_string(int(cents_second * 100.f)) :  to_string(int(cents_second * 100.f));

                // Initialize to zero the time
                cents_second = 0;
                secs = 0;
                minutes = 0;

                checkPointDisplayed = true;
            }
        }
    }

    // Check if the current map is the goal
    if (currentMap->isGoalMap()) {
        // Display the goal animation
        State status = goalAnimation(c);
        if (status == EXIT) {
            exit(1);
        }
        mtx.lock();
        arrival = true;
        mtx.unlock();
    }
    // Check if the game has finished badly
    if (!finalGame) {

        // Update and prepare cars to draw
        if (lastY <= currentMap->getCamY() + float(c.renderLen))
            lastY = currentMap->getCamY() + float(c.renderLen);

        // Traffic cars
        for (Enemy &v : cars) {
            if (v.getPosY() + DEL_VEHICLE < currentMap->getCamY()) {
                // Update the speed and position for the car
                v.update(lastY, lastY + float(c.renderLen) / VEHICLE_DENSITY, c.maxAggressiveness);
                lastY = v.getPosY() + VEHICLE_MIN_DISTANCE * RECTANGLE;
            }

            // Draw the car
            float posY = v.getPosY();
            v.draw(currentMap->getElevation(posY), currentMap->getCamX());
        }

        // Draw map with cars
        c.w.clear();
        currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                         &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

        // Player update and draw
        action = Vehicle::CRASH;
        direction = Vehicle::RIGHT;

        // Check if the player's vehicle is not crashing
        if (!player.isCrashing()) {
            // Registers the new actions of the user
            action = player.accelerationControl(c, currentMap->hasGotOut(player.getPosX(), player.getPosY()));
            direction = player.rotationControl(c, currentMap->getCurveCoefficient(player.getPosY()),
                                               currentMap->inFork(player.getPosY()));
        }
        else {
            // Check the crashing animation point
            player.hitControl(vehicleCrash);
        }

        // Draw the player's vehicle in the screen
        player.draw(c, action, direction, currentMap->getElevation(player.getPosY()), currentMap->getTerrain());

        // Check if the player's car is not crashing
        if (!player.isCrashing()) {
            vehicleCrash = false;
            float crashPos;

            // Check if the player's car has crashed with any map elements
            bool crash = currentMap->hasCrashed(c, player.getPreviousY(), player.getPosY(), player.getPosX(),
                                                player.getMinScreenX(), player.getMaxScreenX(), crashPos);

            // Any crash detected
            if (!crash)
                // Detect if the player's car has crashed with any traffic car
                for (int i = 0; !vehicleCrash && i < cars.size(); i++)
                    vehicleCrash = cars[i].hasCrashed(player.getPreviousY(), player.getPosY(),
                                                      player.getMinScreenX(), player.getMaxScreenX(),
                                                      crashPos);


            // Draw the traffic car collision
            if (crash || vehicleCrash) {
                player.setModeCollision();
                player.setPosition(player.getPosX(), crashPos);
                player.hitControl(vehicleCrash);
                action = Vehicle::CRASH;
                direction = Vehicle::RIGHT;

                player.draw(c, action, direction, currentMap->getElevation(player.getPosY()), currentMap->getTerrain());
            }
        }

        // Compute the route and actions of each traffic car with its AI
        for (Enemy &v : cars){

            if (currentMap->inFork(v.getPosY())){
                v.autoControl(c, player.getPosX(), player.getPosY(), true,
                              currentMap->getCurveCoefficient(v.getPosY()), currentMap->getLine(v.getPosY())->yOffsetX, currentMap->getLinesWithoutFork());
            }
            else {
                v.autoControl(c, player.getPosX(), player.getPosY(), false,
                              currentMap->getCurveCoefficient(v.getPosY()), currentMap->getLine(v.getPosY())->yOffsetX, currentMap->getLinesWithoutFork());
            }
        }

        // Check if enemies are displayed on the screen
        for (Enemy &v : cars) {
            float distX, distY;
            bool visible = v.isVisible(c, currentMap->getCamY(), player.getPosX(), player.getPosY(), distX, distY);
            if (visible) {
                if (distY <= 20.f && distX <= 0.3f) {
                    // Thread with sound of the woman
                    elapsed6 = womanShot.getElapsedTime().asSeconds();
                    if (elapsed6 - elapsed5 >= woman_delay.asSeconds()) {
                        // WomanSound
                        c.effects[13]->stop();
                        c.effects[14]->stop();
                        c.effects[15]->stop();
                        c.effects[random_int(13, 15)]->play();
                        womanShot.restart();
                    }
                }
                if (distY <= 30.f && distX <= 1.2f) {
                    // Thread with sound of the woman
                    elapsed8 = trafficCarSound.getElapsedTime().asSeconds();
                    if (elapsed8 - elapsed7 >= traffic_delay.asSeconds()) {
                        // Check the type of traffic car
                        if (v.getId() <= 4){
                            c.effects[33]->stop();
                            c.effects[34]->stop();
                            c.effects[random_int(33, 34)]->play();

                            c.effects[32]->stop();
                            c.effects[32]->play();
                        }
                        else if (v.getId() >= 5 && v.getId() <= 7) {
                            c.effects[35]->stop();
                            c.effects[36]->stop();
                            c.effects[random_int(35, 36)]->play();
                        }
                        else {
                            c.effects[33]->stop();
                            c.effects[34]->stop();
                            c.effects[random_int(33, 34)]->play();

                            c.effects[20]->stop();
                            c.effects[21]->stop();
                            c.effects[random_int(20, 21)]->play();
                        }
                        trafficCarSound.restart();
                    }
                }
            }
        }
        // Check if the checkpoint animation has to be drawn or not
        if (checkPoint) {
            elapsed10 = blinkTime.getElapsedTime().asSeconds();
            if (elapsed10 - elapsed9 >= blink_delay.asSeconds()) {
                blink = !blink;
                blinkTime.restart();
            }
            if (blink) {
                drawCheckpoint(c, true);
                // BeepSound
                c.effects[22]->stop();
                c.effects[22]->play();
            }
            else {
                drawCheckpoint(c, false);
            }

            if (timeCheck - time > 5) {
                checkPoint = false;
            }
        }
    }
}



/**
 * Stops the game and shows the pause menu
 * @param c is the configuration of the game
 * @param a is the last action done by the player
 * @param d is the last direction done by the player
 * @return
 */
State Game::pause(Config &c, const Vehicle::Action &a, const Vehicle::Direction &d) {
    c.w.clear();

    // Draw the map
    currentMap->draw(c, cars, player.getPosX(), player.getPosY(), currentMap->inFork(player.getPosY()),
                     &maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

    // Draw the vehicle of the player
    player.draw(c, a, d, currentMap->getElevation(player.getPosY()), currentMap->getTerrain(), false);

    drawHUD(c);

    c.w.display();
    const Texture bgTexture(c.w.getTexture());
    Sprite bgSprite(bgTexture);
    bgSprite.setScale(float(c.window.getSize().x) / float(c.w.getSize().x),
                      float(c.window.getSize().y) / float(c.w.getSize().y));

    c.w.clear();
    c.window.setView(View(Vector2f(c.window.getSize().x / 2.0f, c.window.getSize().y / 2.0f),
                          Vector2f(c.window.getSize().x, c.window.getSize().y)));
    c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
               static_cast<unsigned int>(c.window.getView().getSize().y));
    c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);

    // Start the pause menu of the game
    vector<Button> menuButtons;


    mtx.lock();
    onPause = true;
    mtx.unlock();

    // Wait until all threads are death
    if (!comeFromOptions){
        timer0.join();
        timer1.join();
        timer2.join();
    }

    // Stop the music of the level
    c.themes[c.currentSoundtrack]->pause();

    RectangleShape shape;
    shape.setPosition(0, 0);
    shape.setSize(Vector2f(c.w.getSize().x, c.w.getSize().y));
    shape.setFillColor(Color(0, 0, 0, 200));

    RectangleShape pauseShape;
    pauseShape.setPosition(c.w.getSize().x / 2.f - 120.0f * c.screenScale,
                           c.w.getSize().y / 2.f - 180.0f * c.screenScale);
    pauseShape.setSize(sf::Vector2f(250.0f * c.screenScale, 400.0f * c.screenScale));
    pauseShape.setFillColor(Color(0, 0, 0));
    pauseShape.setOutlineColor(Color::Green);
    pauseShape.setOutlineThickness(5.0f * c.screenScale);

    Text textMenu;
    textMenu.setPosition(c.w.getSize().x / 2.f - 75.0f * c.screenScale, c.w.getSize().y / 2.f - 150.0f * c.screenScale);
    textMenu.setFont(c.options);
    textMenu.setFillColor(Color(214, 234, 12));
    textMenu.setOutlineColor(Color(12, 72, 234));
    textMenu.setOutlineThickness(2.0f * c.screenScale);
    textMenu.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textMenu.setString("PAUSE");

    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Buttons of the menu
    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Resume", Color(0, 255, 0), Color(255, 255, 0), 1, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale, c.w.getSize().y / 2.f,
                             200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                             "Options", Color(0, 255, 0), Color(255, 255, 0), 0, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Home", Color(0, 255, 0), Color(255, 255, 0), 0, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 140.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Exit", Color(0, 255, 0), Color(255, 255, 0), 0, c.screenScale);

    while (!startPressed) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Check if the up or down cursor keys have been pressed or not
        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuDownKey)) {
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1)) {
                // Change the color appearance of both buttons
                c.effects[0]->stop();
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
            }
        } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuUpKey)) {
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0) {
                c.effects[0]->stop();
                c.effects[0]->play();
                optionSelected--;
                // Change the color appearance of both buttons
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
            }
        } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
            if (comeFromOptions) {
                comeFromOptions = false;
            } else {
                startPressed = true;
            }
        }

        c.w.draw(bgSprite);

        c.w.draw(shape);

        c.w.draw(pauseShape);
        c.w.draw(textMenu);

        for (Button b : menuButtons) {
            b.render(&c.w);
        }

        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(milliseconds(100));
    }

    c.effects[2]->stop();
    c.effects[2]->play();

    if (c.enablePixelArt) {
        if (c.isDefaultScreen)
            c.window.setView(View(Vector2f(SCREEN_DEFAULT_X / 4.0f, SCREEN_DEFAULT_Y / 4.0f),
                                  Vector2f(SCREEN_DEFAULT_X / 2.0f, SCREEN_DEFAULT_Y / 2.0f)));
        else
            c.window.setView(View(Vector2f(SCREEN_HD_X / 4.0f, SCREEN_HD_Y / 4.0f),
                                  Vector2f(SCREEN_HD_X / 2.0f, SCREEN_HD_Y / 2.0f)));
        c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
                   static_cast<unsigned int>(c.window.getView().getSize().y));
        c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);
    }

    // Check the option selected by the user
    switch (optionSelected) {
        case 0:
            // Resume button selected and continue the music
            mtx.lock();
            onPause = false;
            mtx.unlock();
            return GAME;
        case 1:
            // Options button selected
            return OPTIONS;
        case 2:
            // Quit button selected
            mtx.lock();
            onPause = false;
            finalGame = true;
            arrival = false;
            mtx.unlock();
            comeFromOptions = false;
            return START;
        default:
            return EXIT;
    }
}



/**
 * Sets the game is played in manual or automatic mode
 * @param autoMod controls if the game changes the gears manually or automatically
 * @return
 */
 void Game::setAutoMode(const bool autoMod){
    autoMode = autoMode;
 }



/**
 * Returns if the game is played in manual or automatic mode
 * @return
 */
 bool Game::getAutoMode() const {
    return autoMode;
 }
