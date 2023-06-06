
/*
 * Copyright (c) 2022 Ruben Rodriguez
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
 * Implementation file of the module Game
 */


#include "Game.h"


// Semaphore mutex to control the mutual exclusion
mutex mtx;

// Atomic flag used to control the biomes loading thread
std::atomic<bool> biomesLoadDone(false);

// Atomic flag used to control the traffic cars loading thread
std::atomic<bool> trafficCarsLoadDone(false);



/**
 * Thread that loads all the biomes of the game
 * @param input is the module that has all the configuration of the game
 */
void Game::loadBiomes(Input& input){

    // Set the width dimension of the screen depending on the resolution
    Logger::setWidthScreen(input.gameWindow.getSize().x);

    // Vector of sprites of each biome
    const int nobjects[] = {26, 36, 39, 48, 53, 51, 38, 35, 33, 51, 27, 41, 40, 41, 49};
    int nm = 0;

    // Prepare the map (compsed by biomes) and clear the vector
    currentMap = new Map();
    std::string path = "";
    biomes.clear();

    // Control flag
    failBiomesLoaded = false;

    // Loop throughout the five maps
    for (int i = 0; i < 5; i++) {
        vector<Biome> bm;

        // Load the map like a tree
        for (int j = 0; j <= i; j++){

            // Get the path
            path = "Resources/Maps/MapLevels/Map" + std::to_string(nm + 1);

            // Check if the configuration file associated to the biome exists
            Logger::setFailDetected(Logger::checkMapFile(path + "/map.txt"));
            path += "/";

            // There is file associated
            if (!Logger::getFailDetected()){

                // Create the biome and sets its global configuration
                Biome* currentBiome = new Biome();
                Logger::setFailDetected(Logger::checkTimeTerrainRoad(*currentBiome));

                // Set the colors of the biome
                if (!Logger::getFailDetected())
                    Logger::setFailDetected(Logger::checkColors(*currentBiome));
                else
                    // Error
                    failBiomesLoaded = true;

                // Set the track of the biome (road)
                if (!Logger::getFailDetected())
                    Logger::setFailDetected(Logger::checkBiomeRelief(*currentBiome));
                else
                    // Error
                    failBiomesLoaded = true;

                // Set the backgrounds of the biome
                currentBiome->setBackgroundFront(path + "/front.png");
                currentBiome->setBackgroundBack(path + "/back.png");

                // Prepare the sprites of the biome
                vector<std::string> objectNames;
                if (nm == 0){
                    // Get the number of sprites of the biome
                    objectNames.reserve(45);
                    for (int i = 1; i <= 45; i++)
                        objectNames.push_back(std::to_string(i));

                    // Store the path of the sprites
                    path = "Resources/Maps/MapStartGoal/";
                    currentBiome->loadObjects(path, objectNames);
                    objectNames.clear();
                    path = "Resources/Maps/MapLevels/Map" + std::to_string(nm + 1) + "/";
                }

                /*
                 * Reserve enough memory to store the sprites
                 * and load their configuration properties
                 */

                objectNames.reserve((unsigned long) nobjects[nm]);
                for (int no = 1; no <= nobjects[nm]; no++)
                    objectNames.push_back(to_string(no));

                currentBiome->loadObjects(path, objectNames);

                // Set the starting biome (coconut beach)
                if (nm == 0)
                    currentBiome->setStartBiome();

                // Set the sprites of the biome
                if (!Logger::getFailDetected())
                    Logger::setFailDetected(Logger::checkLevelBiomeSprites(*currentBiome));
                else
                    // Error
                    failBiomesLoaded = true;

                // Store the new biome create and close the reading flux
                bm.push_back(*currentBiome);
                Logger::close();
                nm++;
            }
            else
                // Error
                failBiomesLoaded = true;

            // Check if there is any error and the thread is aborted
            if (failBiomesLoaded)
                break;
        }

        // Aborted load
        if (failBiomesLoaded){
            Logger::setFailDetected(true);
            break;
        }

        // Store the biome
        biomes.emplace_back(bm);
    }

    // There is no fail detected
    if (!Logger::getFailDetected()){

        // Set the starting biome with all its data
        currentMap->setCurrentBiome(biomes[0][0]);
        currentMap->setTerrain(currentMap->getCurrentBiome()->getTerrain());
        currentMap->setMapDistanceAndTrackLength(false, gameMode);
        currentMap->setMapColors();
        Logger::setSpriteScreenY(*currentMap->getCurrentBiome());

        // Create the goal biome
        goalBiome = new Biome();
        goalBiome->setGoalBiome();

        switch (gameMode)
        {
            case GameMode::ORIGINAL_MODE:
                // Initialize for each biome the following left and right biomes in the tree
                for (int i = 0; i <= 3; i++){
                    for (int j = 0; j <= i; j++){
                        biomes[i][j].left = &biomes[i + 1][j];
                        biomes[i][j].right = &biomes[i + 1][j + 1];
                    }
                }

                // For the last level the following biome is the goal
                for (int i = 0; i <= 4; i++){
                    biomes[4][i].left = goalBiome;
                    biomes[4][i].right = goalBiome;
                }
                break;
            case GameMode::CONTINUOUS_MODE:
                 // Initialize for each biome the following left and right biomes in the tree
                for (int i = 0; i <= 4; i++){
                    for (int j = 0; j <= i; j++){
                        if (i == j){
                            if (i != 4 && j != 4)
                                biomes[i][j].left = &biomes[i + 1][0];
                            else
                                biomes[i][j].left = goalBiome;
                        }
                        else
                            biomes[i][j].left = &biomes[i][j + 1];
                    }
                }
                break;
        }

        // Close the logger
        Logger::close();
    }
    else
        // Error
        failBiomesLoaded = true;

    // Biomes loaded correctly
    biomesLoadDone = true;
}



/**
 * Thread that loads all the traffic cars
 * @param input is the module that has all the configuration of the game
 */
void Game::loadTrafficCars(Input& input){

    // Number of cars to load
    int numTrafficCars = 0;

    // Set the number of traffic cars depending on the traffic level
    switch (input.traffic){
        case Level_Traffic::LOW:
            numTrafficCars = TRAFFIC_CARS_EASY;
            break;
        case Level_Traffic::MEDIUM:
            numTrafficCars = TRAFFIC_CARS_NORMAL;
            break;
        case Level_Traffic::HIGH:
            numTrafficCars = TRAFFIC_CARS_HARD;
    }

    // Clear the vector of the cars and set the correct space
    cars.clear();
    cars.reserve(numTrafficCars);
    float posZ = 0.f;

    // Set a random code of AI to set the cars
    startCodeAi = random_int(1, 3);

    /*
     * Create the first six cars that appear at the beginning of the round
     */

    TrafficCar* car1 = new TrafficCar(0, 0, 190.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car1", 1, 0.5f, false, true, true, startCodeAi);
    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;

    TrafficCar* car2 = new TrafficCar(0, 0, 170.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car2", 2, 0.f, false, true, true, startCodeAi);
    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;

    TrafficCar* car3 = new TrafficCar(0, 0, 165.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car3", 3, -0.5f, false, true, true, startCodeAi);
    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;

    TrafficCar* car4 = new TrafficCar(0, 0, 160.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car4", 4, -0.25f, false, true, true, startCodeAi);
    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;

    TrafficCar* car5 = new TrafficCar(0, 0, 155.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car5", 5, 0.f, false, true, true, startCodeAi);
    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;

    TrafficCar* car6 = new TrafficCar(0, 0, 150.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car6", 6, 0.25f, false, true, true, startCodeAi);
    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;

    // Push the cars
    cars.push_back(car1);
    cars.push_back(car2);
    cars.push_back(car3);
    cars.push_back(car4);
    cars.push_back(car5);
    cars.push_back(car6);

    // Create the rest of the cars
    for (int i = 7; i <= numTrafficCars; i++){

        // Set the position in axis Z (depth in the scenario)
        switch (input.traffic){
            case Level_Traffic::LOW:
                posZ = random_int(5, 7) * 100 * SEGMENT_LENGTH;
                break;
            case Level_Traffic::MEDIUM:
                posZ = random_int(5, 9) * 100 * SEGMENT_LENGTH;
                break;
            case Level_Traffic::HIGH:
                posZ = random_int(5, 11) * 100 * SEGMENT_LENGTH;
        }

        // Srt the AI and push it to the vector
        startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;
        TrafficCar* c = new TrafficCar(0, 0, posZ, random_int(10, 16) * 10.f,
                                       "TrafficCars/Car" + std::to_string(i), i, random_int(-6, 6) * 0.15f, false,
                                       random_int(0, 1), true, startCodeAi);

        cars.push_back(c);
    }

    // Traffic cars have been created correctly
    trafficCarsLoadDone = true;
}



/**
 * Thread that controls the continuous update of the timers
 * while the player is playing a game round
 */
void Game::updateTime(){

    // Define the internal clock timer and the time interval to be count
    sf::Clock gameClockLap;
    sf::Time shot_delayLap = sf::seconds(0.01);

    // Initialize the clock and take the first measure
    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    /*
     * Local variables to control the end of the thread
     * Arrival to the goal
     * Game over
     * Pause mode
     * Escape key pressed
     */
    bool arrived = false , endOfGame = false, pause = false, escaped = false;

    // Check if there is any escape condition as true
    while(!arrived && !endOfGame && !pause && !escaped){

        // Check if there is no pause
        if (!pause){
            // Measure the time elapsed and count a hundredth of second
            elapsed4 = gameClockLap.getElapsedTime().asSeconds();
            if (elapsed4 - elapsed3 >= shot_delayLap.asSeconds()){
                cents_second += elapsed4;
                if (cents_second >= 1.f){
                    // hundredth of second elapsed (update the timers)
                    cents_second -= 1.f;
                    secs++;
                    timeToPlay--;

                    // Countdown of the ten seconds
                    if (timeToPlay == 10)
                        Audio::play(Sfx::BLOND_WOMAN_TEN_SECONDS, false);
                    else if (timeToPlay < 10){
                        // Only five seconds to play
                        if (timeToPlay == 5)
                            Audio::play(Sfx::BLONDE_WOMAN_HURRY_UP, false);
                        else if (timeToPlay == 0)
                            endOfGame = true;
                        if (!Audio::isPlaying(Sfx::COUNTDOWN))
                            Audio::play(Sfx::COUNTDOWN, false);
                    }

                    // Count minutes (sixty seconds elapsed
                    if (secs == 60.f) {
                        secs = 0;
                        minutes++;
                    }
                }

                // Count all the total time elapsed since the game round started
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

            // Check the finish conditions
            mtx.lock();
            arrived = arrival;
            outOfTime = endOfGame;
            pause = pauseMode;
            escaped = escape;
            mtx.unlock();
        }
    }
}



/**
 * Default constructor
 */
Game::Game(){}



/**
 * Second constructor
 * @param input is the module that has all the configuration of the game
 */
Game::Game(Input& input){

    /*
     * Set the default status of the game when it starts
     */

    gameStatus = State::ANIMATION;
    automaticMode = false;
    firstLoad = true;
    spectatorsCongrats = false;
    showmanCongrats = false;
    countHudBonus = 0;
    levelsToComplete = 0;
    score = 0;
    time = 0;
    minutes = 0.f;
    secs = 0.f;
    cents_second = 0.f;
    minutesTrip = 0.f;
    secsTrip = 0.f;
    cents_secondTrip = 0.f;
    level = 1;
    countCheckPointDisplays = 0;
    checkPointTime = 0;
    countCheckPointDisplays = 0;
    startCodeAi = 0;
    playerCarSelected = 0;
    treeMapPos = LEVEL_FACTOR;
    currentMap = nullptr;
    player = nullptr;
    goalBiome = nullptr;
    pauseMode = false;
    escape = false;
    outOfTime = false;
    arrival = false;
    start = false;
    firstGame = false;
    endingAnimation = false;
    startingRound = true;
    carSelectionRefused = false;
    checkPoint = false;
    checkPointDisplayed = false;
    failBiomesLoaded = false;

    Audio::loadAll(input);
}



/**
 * Control the actions done by the player while the game round is in course
 * @param input is the module that has all the configuration of the game
 */
void Game::handleEvent(Input& input, const float& time){
    sf::Event event;
    // Check if the player has done any action
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            // Escape symbol (X) of the window has been pressed
            mtx.lock();
            escape = true;
            mtx.unlock();
        }
        else if (gameStatus == State::PLAY_ROUND){
            // Game is on the PLAY_ROUND status (initial animation finished)
            if (!pauseMode && !outOfTime && !arrival && input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                // Press escape key to pause the game
                mtx.lock();
                pauseMode = true;
                mtx.unlock();
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            }
        }
        else if (gameStatus == State::GAME_OVER){
            // Game in game over status (time is over)
            if (!start && outOfTime && input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                // Check if the player pressed enter key to exit and return to the initial menu
                start = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
        }
    }

    /*
     * Check if the game is showing the initial animation and it is not in
     * pause neither in game over neither the player car is crashing or is
     * been relocated in the center of the road after the crashing animation
     */

    if (gameStatus != State::PREPARE_ROUND && !escape && !pauseMode && !outOfTime && !player->getCrashing() && player->getDrawCar())
        // Control the player car
        player->accelerationControl(input, gameStatus, time);
}



/**
 * Plays the initial animation at the starting point when
 * the play round starts
 * @param input is the module that has all the configuration of the game
 */
State Game::startRound(Input& input){

    // Starts the play round (all the times)
    score = 0;
    minutes = 0;
    secs = 0;
    cents_second = 0;
    level = 1;
    int numTrafficCars = 0;
    treeMapPos = LEVEL_FACTOR;
    startingRound = true;
    checkPoint = false;
    checkPointDisplayed = false;
    blinkCheckPoint = true;
    escape = false;
    arrival = false;

    // Stop all the sounds from the possible previous game
    Audio::stopMusic();
    Audio::stopSfx();

    // Assign the time to start
    timeToPlay = currentMap->getCurrentBiome()->getTime();

    // Modify the time to play depending on the difficulty and traffic levels
    if (input.difficulty == Level_Difficulty::HARD)
        timeToPlay += 5;
    else if (input.difficulty == Level_Difficulty::EASY)
        timeToPlay -= 5;

    if (input.traffic == Level_Traffic::HIGH)
        timeToPlay += 5;
    else if (input.traffic == Level_Traffic::LOW)
        timeToPlay -= 5;

    // Create the player car
    float scale = (input.currentIndexResolution <= 1) ? 3.2f : 3.5f;
    player = new PlayerCar(0.f, 0, (int)(CAMERA_HEIGHT * CAMERA_DISTANCE) + 241, 0.f, scale,
                           "Ferraris/Ferrari" + to_string(playerCarSelected + 1), automaticMode, false,
                           currentMap->getCurrentBiome()->getRoadTerrain());

    // Load and set the round hud with all the information data
    HudRound::loadHudRound();
    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, true,
                          player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

    HudRound::setAllHudRoundIndicators(input);
    HudRound::configureHudRound(input);

    // Load and set the round hud with all the information data
    HudCheckPoint::loadHudCheckPoint();
    HudCheckPoint::setHudCheckPoint(minutes, secs, cents_second);
    HudCheckPoint::configureHudCheckPoint(input);

    // Locked until the thread that loads the traffic cars has finished
    if (!trafficCarsLoadDone)
        while (!trafficCarsLoadDone);

    int code = 121;
    float i = input.gameWindow.getSize().x / 2.f;

    sf::RectangleShape blackShape;
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

    int j = 255;

    // Play intro sound mode depending on game mode selected
    switch (gameMode)
    {
        case GameMode::ORIGINAL_MODE:
            Audio::play(Sfx::RACE_START_ORIGINAL_MODE, false);
            break;
        case GameMode::CONTINUOUS_MODE:
            Audio::play(Sfx::RACE_START_CONTINUOUS_MODE, false);
    }

    // Draw the initial start
    while(!escape && j >= 0){
        // Check the player actions
        handleEvent(input, time);
        blackShape.setFillColor(sf::Color(0, 0, 0, j));

        // Clear the screen and draw
        input.gameWindow.clear();
        currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);

        // Player car on the right side without drift
        player->drawStartStaticRound(input);
        HudRound::drawHudRound(input);
        input.gameWindow.draw(blackShape);
        input.gameWindow.display();
        j -= 5;
    }

    // Startin the drift player car animation
    while (!player->getEndAnimation() && !escape){

        // Check the actions of the player
        handleEvent(input, time);
        input.gameWindow.clear();

        // Render the scenario with the player car drifting
        currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);
        player->drawStartDriftRound(input, float(i), code);
        HudRound::drawHudRound(input);
        input.gameWindow.display();

        i -= 3;
        if (i <= input.gameWindow.getSize().x / 2.f / 1.05f)
            code = 4;
        else if (i <= input.gameWindow.getSize().x / 2.f / 1.17f)
            code = 8;
        else if (i <= input.gameWindow.getSize().x / 2.f / 1.23f)
            code = 120;
    }

    // Starts the semaphore countdown animation
    sf::Clock semaphoreClock;
    const float semaphoreLight = sf::seconds(1.0f).asSeconds();
    float timeInitial = semaphoreClock.getElapsedTime().asSeconds();
    float timeElapsed = 0.f;

    input.gameWindow.clear();
    currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);
    player->drawPlayRound(input, true, currentMap->getTerrain());
    HudRound::drawHudRound(input);
    input.gameWindow.display();

    // Play the showman starting race announcement
    Audio::play(Sfx::SHOWMAN_RACE_START, false);
    while (Audio::isPlaying(Sfx::SHOWMAN_RACE_START))
        handleEvent(input, time);

    // Countdown of the three semaphore lights
    j = 3;
    while(!escape && j >= 0){
        handleEvent(input, time);

        timeElapsed = semaphoreClock.getElapsedTime().asSeconds();

        if (timeElapsed - timeInitial >= semaphoreLight){

            // Play the semaphore light sound
            if (j < 3 && j > 0)
                Audio::play(Sfx::RACE_SEMAPHORE_PREPARE, false);
            else if (j == 0)
                Audio::play(Sfx::RACE_SEMAPHORE_START, false);

            // Update the semaphore light
            Logger::updateSprite(*currentMap->getCurrentBiome(), Sprite_Animated::SEMAPHORE);
            semaphoreClock.restart();
            j--;

            // Update the flagger position
            if (j == 1 || j < 0)
                Logger::updateSprite(*currentMap->getCurrentBiome(), Sprite_Animated::FLAGGER);
        }

        // Draw all the components
        currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);
        player->drawPlayRound(input, true, currentMap->getTerrain());
        HudRound::drawHudRound(input);
        input.gameWindow.display();
    }

    // Check if the player has exit te game
    if (escape)
        return State::EXIT;
    else {
        // Finish animation and starts the soundtrack chosen
        player->setEndAnimation(false);
        Audio::play(input.currentSoundtrack, true);

        // Play round status
        return State::PLAY_ROUND;
    }
}



/**
 * Update the game round status
 * @param input is the module that has all the configuration of the game
 */
void Game::updateRound(Input& input){

    // Check if the round has just started
    if (startingRound){
        // Start the clock
        tick_timer = clock();
        startingRound = false;
    }

    // Count the time elapsed
    time = (float)((clock() - tick_timer) / (double)CLOCKS_PER_SEC);
    tick_timer = clock();

    // Check the possible actions of the player
    handleEvent(input, time);

    // Update the hud round
    mtx.lock();
    int currentLevel = level;

    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, currentLevel, treeMapPos, checkPoint,
                            player->getGear(), player->getSpeed(), player->getHighMaxSpeed());
    mtx.unlock();

    // Prepare all the hud round indicators to draw them
    HudRound::setAllHudRoundIndicators(input);

    // Check if the game is in game over
    if (gameStatus != State::GAME_OVER)
        // Update the position of the player car in the scenario
        currentMap->updateMap(input, cars, *player, gameStatus, time, score, levelsToComplete, checkPoint, checkPointDisplayed,
                              treeMapPos, currentLevel, startCodeAi, gameMode);

    // Draw the scenario with the car
    currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);

    // Check if the round is still in course
    if (gameStatus == State::PLAY_ROUND){
        // Check if the player has reached the goal
        arrival = currentMap->getEnding();
        player->drawPlayRound(input, false, currentMap->getTerrain());
    }
    else
        // Draw the ending car animation
        player->drawEndDriftRound(input);

    // Draw the hud round in the screen
    HudRound::drawHudRound(input);

    // The player has passed the checkpoint of the scenario
    if (checkPoint && !checkPointDisplayed){

        // Set the checkpoint hud
        mtx.lock();
        HudCheckPoint::setHudCheckPoint(minutes, secs, cents_second);
        cents_second = 0;
        secs = 0;
        minutes = 0;

        // Increase the level
        level++;
        int timeToAdd = 0;

        // Increase to time to play
        if (gameMode == GameMode::ORIGINAL_MODE){
            if (level == 2)
                timeToAdd = int(currentMap->getCurrentBiome()->getTime() * MULTI_FACTOR_SECOND_LEVEL);
            else if (level == 3)
                timeToAdd = int(currentMap->getCurrentBiome()->getTime() * MULTI_FACTOR_THIRD_LEVEL);
            else if (level == 4)
                timeToAdd = int(currentMap->getCurrentBiome()->getTime() * MULTI_FACTOR_FOURTH_LEVEL);
            else if (level == 5)
                timeToAdd = int(currentMap->getCurrentBiome()->getTime() * MULTI_FACTOR_FIFTH_LEVEL);
        }
        else
            timeToAdd = int(currentMap->getCurrentBiome()->getTime() * MULTI_FACTOR_CONTINUOUS_MODE);


        timeToPlay += timeToAdd;

        // Set the new changes of the game status in the hud round
        HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                            player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

        HudRound::setAllHudRoundIndicators(input);
        mtx.unlock();
        checkPointDisplayed = true;

        if (gameMode == GameMode::ORIGINAL_MODE)
            Audio::play(Sfx::CHECKPOINT_VOICE_FIRST, false);
        else if (gameMode == GameMode::CONTINUOUS_MODE)
            Audio::play(Sfx::CHECKPOINT_VOICE_SECOND, false);
    }

    // Check if the checkpoint hus has to be displayed
    if (checkPoint) {

        if (checkPointTime < 20)
            checkPointTime++;
        else {
            checkPointTime = 0;
            countCheckPointDisplays++;
            blinkCheckPoint = !blinkCheckPoint;

            if (countCheckPointDisplays == 15){
                checkPoint = false;
                countCheckPointDisplays = 0;
                checkPointTime = 0;
                blinkCheckPoint = false;
            }
        }

        if (blinkCheckPoint){
            if (checkPointTime == 1)
                Audio::play(Sfx::CHECKPOINT_ALARM, false);

            HudCheckPoint::drawHudCheckPoint(input);
        }
    }
    else
        // Stop the checkpoint sound
        Audio::stop(Sfx::CHECKPOINT_ALARM);

    // Check if the player has arrived to the goal
    if (gameStatus == State::END_ROUND){

        // Count the time to decrease the bonus timer
        elapsedBonusTime = bonusClock.getElapsedTime().asSeconds();

        if (elapsedBonusTime - bonusTime >= bonus_delay.asSeconds()) {

            if (decsTimeBonus > 0) {
                decsTimeBonus--;
                secondsBonus = decsTimeBonus / 10;
                decs_secondBonus = decsTimeBonus % 10;
            }
        }
        bonusClock.restart();

        // Score bonus hud finishes
        if (secondsBonus == 0 && decs_secondBonus == 0 && Audio::isPlaying(Sfx::SCORE_BONUS))
            Audio::stop(Sfx::SCORE_BONUS);

        // Set the hud bonus
        HudBonus::setHudBonus(secondsBonus, decs_secondBonus);
        HudBonus::setTextHudBonusIndicator();

        if (secondsBonus == 0 && decs_secondBonus == 0){
            // Count the time to show the hud bonus
            if (countHudBonus < 100){
                HudBonus::drawHudBonus(input);
                countHudBonus++;
            }
        }
        else
            HudBonus::drawHudBonus(input);

        // Check if the player car has stopped
        if (player->getSpeed() <= 0.f){
            if (!showmanCongrats){
                // Play showman sound
                Audio::play(Sfx::SHOWMAN_CONGRATULATIONS, false);
                showmanCongrats = true;
            }
            if (!spectatorsCongrats){
                // Play spectators sound
                Audio::play(Sfx::SPECTATORS_CONGRATULATIONS, false);
                spectatorsCongrats = true;
            }
        }
    }

    // Check if the player has reached the goal and not all the bonus score has been added
    if (gameStatus == State::END_ROUND && score <= totalScore){
        // Add the score
        if (score < totalScore){
            score += int(1.5f * SCORE_BONIFICATION / 45.5f);
            if (score >= totalScore)
                score = totalScore;
        }
    }
    else {
        // Check if the player car is inside the road
        if (player->getStateWheelLeft() != StateWheel::SAND && player->getStateWheelRight() != StateWheel::SAND &&
            player->getStateWheelLeft() != StateWheel::GRASS && player->getStateWheelRight() != StateWheel::GRASS &&
            player->getStateWheelLeft() != StateWheel::SNOW && player->getStateWheelRight() != StateWheel::SNOW &&
            player->getStateWheelLeft() != StateWheel::MUD && player->getStateWheelRight() != StateWheel::MUD &&
            player->getSpeed() > 5.f)
        {
            // Add score
            score += (int)((10.f + 950.f * ((player->getSpeed() - 5.f) / (141.f))) / 3.7f) * 2.3f;
        }
    }

    // Display all in the screen
    input.gameWindow.display();
}



/**
 * Update the game during the play round status
 * @param input is the module that has all the configuration of the game
 */
State Game::playRound(Input& input){

    // Check if it is the first play round
    if (!firstGame)
        firstGame = true;

    // Check if the game was in pause mode
    if (pauseMode){
        pauseMode = false;
        outOfTime = false;
        arrival = false;
        Audio::reanudateSfxPaused();
        tick_timer = clock();
    }

    // Start the thread that counts the time
    timeCounter = std::thread(&Game::updateTime, this);

    // Updates all the internal measures with the clocks
    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    // Check the final conditions
    while (!escape && !pauseMode && !outOfTime && !arrival)
        // Update the status
        updateRound(input);

    // Wait until the thread finishes
    timeCounter.join();

    // Check if the the game as been paused
    if (pauseMode)
        // Stop all the sounds that are been played to be continued
        Audio::pauseSfxForReanudate();

    // Player has reached the goal
    if (arrival)
        return State::END_ROUND;
    // Player has closed the game
    else if (escape)
        return State::EXIT;
    // Player as paused the game
    else if (pauseMode)
        return State::PAUSE;
    // Game over
    else if (outOfTime)
        return State::GAME_OVER;
}



/**
 * Update the end round animation when the player
 * reaches the goal and completes the game round
 */
State Game::endRound(Input& input){

    // Set the control flags
    escape = false;
    bool arrivalCar = false;

    player->setEndAnimation(true);
    Logger::setLoggerStatus();

    // Load the bonus hud
    HudBonus::loadHudBonus(input);

    // Stop the soundtrack used to play
    if (Audio::isPlaying(input.currentSoundtrack))
            Audio::stop(input.currentSoundtrack);

    // Stop all the sfx of the player car
    for (int i = (int)Sfx::FERRARI_ENGINE_START; i <= (int)Sfx::FERRARI_CRASH; i++)
        Audio::stop(static_cast<Sfx>(i));

    // Play randomly one final race sound
    switch (gameMode)
    {
        case GameMode::ORIGINAL_MODE:
            if (!Audio::isPlaying(Sfx::RACE_END_FIRST_ORIGINAL_MODE) &&
                !Audio::isPlaying(Sfx::RACE_END_SECOND_ORIGINAL_MODE) &&
                !Audio::isPlaying(Sfx::RACE_END_THIRD_ORIGINAL_MODE))
            {
                int code = random_int((int)Sfx::RACE_END_FIRST_ORIGINAL_MODE, (int)Sfx::RACE_END_THIRD_ORIGINAL_MODE);
                Audio::play(static_cast<Sfx>(code), false);
            }
            break;
        case GameMode::CONTINUOUS_MODE:
            if (!Audio::isPlaying(Sfx::RACE_END_FIRST_CONTINUOUS_MODE) &&
                !Audio::isPlaying(Sfx::RACE_END_SECOND_CONTINUOUS_MODE))
            {
                int code = random_int((int)Sfx::RACE_END_FIRST_CONTINUOUS_MODE, (int)Sfx::RACE_END_SECOND_CONTINUOUS_MODE);
                Audio::play(static_cast<Sfx>(code), false);
            }
    }




    // Play the score bonus
    if (!Audio::isPlaying(Sfx::SCORE_BONUS))
        Audio::play(Sfx::SCORE_BONUS, true);

    // Compute the bonus time
    decsTimeBonus = timeToPlay * 10;
    secondsBonus = decsTimeBonus / 10;
    decs_secondBonus = decsTimeBonus % 10;

    bonusClock.restart().asSeconds();
    bonusTime = bonusClock.getElapsedTime().asSeconds();

    // Calculate the total score obtained
    totalScore = score + (timeToPlay * SCORE_BONIFICATION);

    // Check if the player closes or if the goal animation finishes
    while (!escape && (!arrivalCar || score < totalScore)){
        // Update the status
        updateRound(input);
        if (player->getSpeed() <= 0.f)
            // Player car arrives to the second goal
            arrivalCar = true;
    }

    // Player has closed the game
    if (escape)
        return State::EXIT;
    else {
        int i = 0;
        // Draw the final drifting animation of the player car
        while (!escape && i <= 100){
            handleEvent(input, time);
            input.gameWindow.clear();
            currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);
            player->drawEndDriftRound(input);
            HudRound::drawHudRound(input);
            input.gameWindow.display();
            i++;
        }

        // Player has closed the game
        if (escape)
            return State::EXIT;
        else {
            // Draw the black transition layer
            sf::RectangleShape blackShape;
            blackShape.setPosition(0, 0);
            blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

            i = 0;
            while (!escape && i <= 255){
                handleEvent(input, time);
                blackShape.setFillColor(sf::Color(0, 0, 0, i));
                input.gameWindow.draw(blackShape);
                input.gameWindow.display();
                i += 5;
            }

            // Close the game
            if (escape)
                return State::EXIT;
        }
    }

    // Goes to the ranking status to check possible record
    countHudBonus = 0;
    spectatorsCongrats = false;
    showmanCongrats = false;
    firstLoad = true;
    return State::RANKING;
}



/**
 * Update the game over status animation when the time
 * to complete the scenario is over
 * @param input is the module that has all the configuration of the game
 */
State Game::gameOverRound(Input& input){

    // Control flag
    start = false;

    // Both wheels of the player car are inside the road
    if (player->getOutsideLeftWheelRoad())
        player->setOutsideLeftWheelRoad(false);

    if (player->getOutsideRightWheelRoad())
        player->setOutsideRightWheelRoad(false);

    // Set the hud round with all the information
    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                          player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

    HudRound::setAllHudRoundIndicators(input);

    // Stop the current soundtrack and plays the game over sound
    Audio::stop(input.currentSoundtrack);
    Audio::play(Soundtrack::GAME_OVER, true);

    // Wait until player closes the game or press start
    while (!escape && !start){
        // Checking actions of the player
        handleEvent(input, time);

        // Draw the game over status
        currentMap->renderMap(input, cars, *player, gameStatus, gameMode, pauseMode);
        player->drawPlayRound(input, true, currentMap->getTerrain(), false);
        HudRound::drawHudRound(input);
        input.gameWindow.display();
    }

    // Stop game over sound
    Audio::stop(Soundtrack::GAME_OVER);

    // Player presses start
    if (start){
        sf::RectangleShape shape;
        shape.setPosition(0, 0);
        shape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
        shape.setFillColor(sf::Color(0, 0, 0, 200));

        // Draw the black transition layer
        for (int i = 0; i <= 70; i++){
            shape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(shape);
            input.gameWindow.display();
            sf::sleep(sf::milliseconds(10));
        }
        // Goes to the main menu of the game
        return State::START;
    }
    // Player closes the game
    else if (escape)
        return State::EXIT;
}



/**
 * Run the execution of all the game
 * @param input is the module that has all the configuration of the game
 */
void Game::run(Input& input){

    // window is opened and the game is not closed
    while (input.gameWindow.isOpen() && gameStatus != State::EXIT){
        switch (gameStatus) {
            case State::ANIMATION: {
                // Initial animation of the game
                MenuLogo ml = MenuLogo();
                ml.loadMenu(input);
                ml.draw(input);
                gameStatus = ml.returnMenu(input);
                break;
            }
            case State::START: {
                MenuStart mS = MenuStart();
                mS.setMenuStart(firstLoad, outOfTime);
                mS.loadMenu(input);
                mS.draw(input);
                gameStatus = mS.returnMenu(input);
                firstLoad = mS.getFirstLoad();
                outOfTime = false;
                break;
            }
            case State::GAME: {
                // Game menu
                playerCarSelected = 0;
                MenuGame mG = MenuGame();
                mG.loadMenu(input);
                mG.draw(input);
                gameStatus = mG.returnMenu(input);
                break;
            }
            case State::MODE: {
                // Default mode
                gameMode = GameMode::ORIGINAL_MODE;

                // Game menu
                MenuMode mO = MenuMode();
                mO.setGameMode(gameMode);
                mO.loadMenu(input);
                mO.draw(input);
                gameStatus = mO.returnMenu(input);
                gameMode = mO.getGameMode();

                switch (gameMode)
                {
                    case GameMode::ORIGINAL_MODE:
                        levelsToComplete = 5;
                        break;
                    case GameMode::CONTINUOUS_MODE:
                        levelsToComplete = 15;
                }
                break;
            }
            case State::CREDITS: {
                // Credits of the game
                MenuCredits mC = MenuCredits();
                mC.loadMenu(input);
                mC.draw(input);
                gameStatus = mC.returnMenu(input);
                break;
            }
            case State::OPTIONS: {
                // Options menu
                MenuOptions mO = MenuOptions();
                mO.setPauseMode(pauseMode);
                mO.loadMenu(input);
                mO.draw(input);
                gameStatus = mO.returnMenu(input);

                if (currentMap != nullptr)
                    // Set the new width of the window
                    Logger::setSpriteScreenY(*currentMap->getCurrentBiome());

                // Set the scale of the player car
                if (player != nullptr){
                    float scale = (input.currentIndexResolution <= 1) ? 3.2f : 3.5f;
                    player->setScale(scale);
                }
                break;
            }
            case State::MUSIC_CONF: {
                // Music configuration menu (volume of soundtracks and sfx)
                MenuSound mS = MenuSound();
                mS.loadMenu(input);
                mS.draw(input);
                gameStatus = mS.returnMenu(input);
                break;
            }
            case State::CONTROLS: {
                // Controllers menu
                MenuControls mC = MenuControls();
                mC.loadMenu(input);
                mC.draw(input);
                gameStatus = mC.returnMenu(input);
                break;
            }
            case State::GEARS: {
                // Gears menu

                firstLoad = true;
                if (firstLoad || outOfTime || arrival){
                    // Load the biomes
                    biomesLoader = std::thread(&Game::loadBiomes, this, ref(input));
                    biomesLoader.detach();
                }

                MenuGears mGe = MenuGears(playerCarSelected);
                mGe.setGameMode(gameMode);
                mGe.loadMenu(input);
                mGe.draw(input);
                automaticMode = mGe.getAutomaticMode();
                gameStatus = mGe.returnMenu(input);
                playerCarSelected = 0;
                firstLoad = false;
                break;
            }
            case State::VEHICLE: {

                // Load the traffic cars
                trafficCarLoader = std::thread(&Game::loadTrafficCars, this, ref(input));
                trafficCarLoader.detach();

                // Car menu selection
                MenuCarSelection mCs = MenuCarSelection();
                mCs.setGameMode(gameMode);
                mCs.loadMenu(input);
                mCs.draw(input);
                playerCarSelected = mCs.hasPlayerCarSelected();
                gameStatus = mCs.returnMenu(input);
                break;
            }
            case State::MUSIC: {
                // Music selection to play
                MenuMusicRadio mMr = MenuMusicRadio();
                mMr.loadMenu(input);
                mMr.draw(input);
                gameStatus =  mMr.returnMenu(input);
                break;
            }
            case State::LOADING: {
                // Wait until the biomes are correctly loaded
                if (!biomesLoadDone)
                    while (!biomesLoadDone);

                // Error
                if (failBiomesLoaded){
                    // Play error sound and close the game
                    Audio::play(Sfx::MENU_SELECTION_WRONG, false);
                    sf::sleep(Audio::getDurationSfx(Sfx::MENU_SELECTION_WRONG));
                    gameStatus = State::EXIT;
                }
                else {
                    // Loading animation with the controls
                    Audio::play(Sfx::BIOMES_LOADED_WELL, false);
                    MenuLoading mL = MenuLoading(automaticMode);
                    mL.loadMenu(input);
                    mL.draw(input);
                    gameStatus = mL.returnMenu(input);
                }
                break;
            }
            case State::PREPARE_ROUND: {
                // Starting animation round
                gameStatus = this->startRound(input);
                break;
            }
            case State::PLAY_ROUND: {
                // Play round
                gameStatus = this->playRound(input);
                break;
            }
            case State::END_ROUND: {
                // Goal animation
                gameStatus = this->endRound(input);
                trafficCarsLoadDone = false;
                biomesLoadDone = false;
                break;
            }
            case State::PAUSE:{
                // Pause mode

                // Set the hud round with all the information
                HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                                      player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

                HudRound::setAllHudRoundIndicators(input);

                MenuPause mP = MenuPause(*currentMap, *player, cars, currentMap->getTerrain());
                mP.setGameMode(gameMode);
                mP.loadMenu(input);
                mP.draw(input);
                gameStatus = mP.returnMenu(input);
                if (gameStatus == State::START){
                    pauseMode = false;
                    firstLoad = true;
                }
                break;
            }
            case State::GAME_OVER: {
                // Game over
                gameStatus = this->gameOverRound(input);
                trafficCarsLoadDone = false;
                biomesLoadDone = false;
                break;
            }
            case State::RANKING: {
                // Rankin menu
                MenuRanking mR = MenuRanking(score, (int)minutesTrip, (int)secsTrip, (int)cents_secondTrip);
                mR.setGameMode(gameMode);
                mR.loadMenu(input);
                mR.draw(input);
                gameStatus = mR.returnMenu(input);
                trafficCarsLoadDone = false;
                biomesLoadDone = false;
                break;
            }
        }
    }

    // Stop all the sounds
    Audio::stopMusic();
    Audio::stopSfx();

    // Liberate all the memory used
    delete currentMap;
    delete player;
    delete goalBiome;
    biomes.clear();
    int trafficCars = cars.size();
    if (trafficCars > 0){
        for (int i = 0; i < trafficCars; i++)
            delete cars[i];
    }

}
