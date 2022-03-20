
/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
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

#include "Game.h"

mutex mtx;
std::atomic<bool> biomesLoadDone(false);

void Game::updateTime(){

    sf::Clock gameClockLap;

    sf::Time shot_delayLap = sf::seconds(0.01);

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    bool arrived = false , endOfGame = false, pause = false, escaped = false;

    while(!arrived && !endOfGame && !pause && !escaped){

        if (!pause){

            elapsed4 = gameClockLap.getElapsedTime().asSeconds();

            if (elapsed4 - elapsed3 >= shot_delayLap.asSeconds()){
                cents_second += elapsed4;
                if (cents_second >= 1.f) {
                    cents_second -= 1.f;
                    secs++;
                    timeToPlay--;
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

            mtx.lock();
            arrived = arrival;
            endOfGame = outOfTime;
            pause = pauseMode;
            escaped = escape;
            mtx.unlock();
        }
    }
}

Game::Game(Input& input){
    gameStatus = State::ANIMATION;
    automaticMode = false;
    firstLoad = true;
    spectatorsCongrats = false;
    showmanCongrats = false;
    countHudBonus = 0;
    score = 0;
    time = 0;
    minutes = 0.f;
    secs = 0.f;
    cents_second = 0.f;
    minutesTrip = 0.f;
    secsTrip = 0.f;
    cents_secondTrip = 0.f;
    level = 1;
    timeCheck = 0;
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

void Game::handleEvent(Input& input, const float& time){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            mtx.lock();
            escape = true;
            mtx.unlock();
        }
        else if (gameStatus == State::PLAY_ROUND){
            if (!pauseMode && !outOfTime && !arrival && input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                mtx.lock();
                pauseMode = true;
                mtx.unlock();
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            }
            else if (!start && outOfTime && input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                start = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
        }
    }
    if (gameStatus != State::PREPARE_ROUND && !escape && !pauseMode && !outOfTime && !player->getCrashing())
        player->accelerationControl(input, gameStatus, time);
}



void Game::updateRound(Input& input){

    if (startingRound){
        tick_timer = clock();
        startingRound = false;
    }

    time = (float)((clock() - tick_timer) / (double)CLOCKS_PER_SEC);
    tick_timer = clock();
    handleEvent(input, time);

    mtx.lock();
    int currentLevel = level;

    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, currentLevel, treeMapPos, checkPoint,
                            player->getGear(), player->getSpeed(), player->getHighMaxSpeed());
    mtx.unlock();
    HudRound::setAllHudRoundIndicators(input);

    if (gameStatus != State::GAME_OVER)
        currentMap->updateMap(input, cars, *player, gameStatus, time, score, checkPoint, checkPointDisplayed, treeMapPos, currentLevel);

    currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);

    if (gameStatus == State::PLAY_ROUND)
        arrival = currentMap->getEnding();

    if (gameStatus == State::PLAY_ROUND)
        player->drawPlayRound(input, false, currentMap->getTerrain());
    else
        player->drawEndDriftRound(input);

    HudRound::drawHudRound(input);

    if (checkPoint && !checkPointDisplayed){
        mtx.lock();
        HudCheckPoint::setHudCheckPoint(minutes, secs, cents_second);
        cents_second = 0;
        secs = 0;
        minutes = 0;

        timeToPlay += int(currentMap->getCurrentBiome()->getTime());
        timeCheck = timeToPlay;
        level++;

        HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                            player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

        HudRound::setAllHudRoundIndicators(input);
        mtx.unlock();
        checkPointDisplayed = true;
        Audio::play(Sfx::CHECKPOINT_VOICE, false);
    }

    if (checkPoint) {
        checkPointElapsed = blinkTime.getElapsedTime().asSeconds();
        if (checkPointElapsed - checkPointInitial >= blink_delay.asSeconds()) {
            blinkCheckPoint = !blinkCheckPoint;
            blinkTime.restart();
        }
        if (blinkCheckPoint){
            Audio::play(Sfx::CHECKPOINT_ALARM, false);
            HudCheckPoint::drawHudCheckPoint(input);
        }

        mtx.lock();
        int timeHideCheck = timeToPlay;
        mtx.unlock();

        if (timeCheck - timeHideCheck > 5)
            checkPoint = false;
    }
    else
         Audio::stop(Sfx::CHECKPOINT_ALARM);

    if (gameStatus == State::END_ROUND){
        elapsedBonusTime = bonusClock.getElapsedTime().asSeconds();

        if (elapsedBonusTime - bonusTime >= bonus_delay.asSeconds()) {

            if (decsTimeBonus > 0) {
                decsTimeBonus--;
                secondsBonus = decsTimeBonus / 10;
                decs_secondBonus = decsTimeBonus % 10;
            }
        }
        bonusClock.restart();

        if (secondsBonus == 0 && decs_secondBonus == 0 && Audio::isPlaying(Sfx::SCORE_BONUS))
            Audio::stop(Sfx::SCORE_BONUS);

        HudBonus::setHudBonus(secondsBonus, decs_secondBonus);
        HudBonus::setTextHudBonusIndicator();

        if (secondsBonus == 0 && decs_secondBonus == 0){
            if (countHudBonus < 100){
                HudBonus::drawHudBonus(input);
                countHudBonus++;
            }
        }
        else
            HudBonus::drawHudBonus(input);

        if (player->getSpeed() <= 0.f){
            if (!showmanCongrats){
                Audio::play(Sfx::SHOWMAN_CONGRATULATIONS, false);
                showmanCongrats = true;
            }
            if (!spectatorsCongrats){
                Audio::play(Sfx::SPECTATORS_CONGRATULATIONS, false);
                spectatorsCongrats = true;
            }
        }
    }

    if (gameStatus == State::END_ROUND && score <= totalScore){
        if (score < totalScore){
            score += int(1.5f * SCORE_BONIFICATION / 45.5f);
            if (score >= totalScore)
                score = totalScore;
        }
    }
    else {
        if (player->getStateWheelLeft() != StateWheel::SAND && player->getStateWheelRight() != StateWheel::SAND &&
            player->getStateWheelLeft() != StateWheel::GRASS && player->getStateWheelRight() != StateWheel::GRASS &&
            player->getStateWheelLeft() != StateWheel::SNOW && player->getStateWheelRight() != StateWheel::SNOW &&
            player->getStateWheelLeft() != StateWheel::MUD && player->getStateWheelRight() != StateWheel::MUD &&
            player->getSpeed() > 5.f)
        {
            score += (int)((10.f + 950.f * ((player->getSpeed() - 5.f) / (141.f))) / 3.7f) * 2.3f;
        }
    }

    if (!currentMap->getNotDrawn())
        input.gameWindow.display();
    else
        currentMap->setNotDrawn(false);
}

State Game::startRound(Input& input){

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
    blinkCheckPoint = false;

    cars.clear();

    timeToPlay = currentMap->getCurrentBiome()->getTime();
    float scale = (input.currentIndexResolution <= 1) ? 3.2f : 3.5f;
    player = new PlayerCar(0.f, 0, (int)(CAMERA_HEIGHT * CAMERA_DISTANCE) + 241, 0.f, scale,
                           "Ferraris/Ferrari" + to_string(playerCarSelected + 1), automaticMode, false,
                           currentMap->getCurrentBiome()->getRoadTerrain());

    HudRound::loadHudRound();
    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                          player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

    HudRound::setAllHudRoundIndicators(input);
    HudRound::configureHudRound(input);

    HudCheckPoint::loadHudCheckPoint();
    HudCheckPoint::setHudCheckPoint(minutes, secs, cents_second);
    HudCheckPoint::configureHudCheckPoint(input);

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

    cars.reserve(numTrafficCars);

    TrafficCar* car1 = new TrafficCar(0, 0, 190.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car1", 1, 0.5f, false, true, true);
    TrafficCar* car2 = new TrafficCar(0, 0, 170.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car2", 2, 0.f, false, true, true);
    TrafficCar* car3 = new TrafficCar(0, 0, 165.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car3", 3, 0.5f, false, true, true);
    TrafficCar* car4 = new TrafficCar(0, 0, 160.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car4", 4, -0.5f, false, true, true);
    TrafficCar* car5 = new TrafficCar(0, 0, 155.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car5", 5, 0.5f, false, true, true);
    TrafficCar* car6 = new TrafficCar(0, 0, 150.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car6", 6, 0.f, false, true, true);

    cars.push_back(car1);
    cars.push_back(car2);
    cars.push_back(car3);
    cars.push_back(car4);
    cars.push_back(car5);
    cars.push_back(car6);

    for (int i = 7; i <= numTrafficCars; i++){
        TrafficCar* c = new TrafficCar(0, 0, random_int(5, 7) * 100 * SEGMENT_LENGTH, random_int(10, 16) * 10.f,
                                       "TrafficCars/Car" + std::to_string(i), i, random_int(-6, 6) * 0.15f, false,
                                       random_int(0, 1), true);

        cars.push_back(c);
    }

    int counterAnimation = 0;
    int code = 121;
    float i = input.gameWindow.getSize().x / 2.f;

    sf::RectangleShape blackShape;
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

    int j = 255;
    Audio::play(Sfx::RACE_START, false);

    while(!escape && j >= 0){
        handleEvent(input, time);
        blackShape.setFillColor(sf::Color(0, 0, 0, j));

        input.gameWindow.clear();
        currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);
        player->drawStartStaticRound(input);
        HudRound::drawHudRound(input);
        input.gameWindow.draw(blackShape);
        input.gameWindow.display();
        j -= 5;
    }

    while (!player->getEndAnimation() && !escape){
        handleEvent(input, time);

        input.gameWindow.clear();
        currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);
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

    sf::Clock semaphoreClock;

    const float semaphoreLight = sf::seconds(1.0f).asSeconds();
    float timeInitial = semaphoreClock.getElapsedTime().asSeconds();
    float timeElapsed = 0.f;

    input.gameWindow.clear();
    currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);
    player->drawPlayRound(input, true, currentMap->getTerrain());
    HudRound::drawHudRound(input);
    input.gameWindow.display();

    Audio::play(Sfx::SHOWMAN_RACE_START, false);
    while (Audio::isPlaying(Sfx::SHOWMAN_RACE_START))
        handleEvent(input, time);

    j = 3;
    while(!escape && j >= 0){
        handleEvent(input, time);

        timeElapsed = semaphoreClock.getElapsedTime().asSeconds();

        if (timeElapsed - timeInitial >= semaphoreLight){

            if (j < 3 && j > 0)
                Audio::play(Sfx::RACE_SEMAPHORE_PREPARE, false);
            else if (j == 0)
                Audio::play(Sfx::RACE_SEMAPHORE_START, false);

            Logger::updateSprite(*currentMap->getCurrentBiome(), Sprite_Animated::SEMAPHORE);
            semaphoreClock.restart();
            j--;

            if (j == 1 || j < 0)
                Logger::updateSprite(*currentMap->getCurrentBiome(), Sprite_Animated::FLAGGER);
        }

        currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);
        player->drawPlayRound(input, true, currentMap->getTerrain());
        HudRound::drawHudRound(input);
        input.gameWindow.display();
    }

    if (escape)
        return State::EXIT;
    else {
        player->setEndAnimation(false);
        Audio::play(input.currentSoundtrack, true);

        return State::PLAY_ROUND;
    }
}


State Game::playRound(Input& input){

    if (!firstGame)
        firstGame = true;

    if (pauseMode){
        pauseMode = false;
        outOfTime = false;
        arrival = false;
        Audio::reanudateSfxPaused();
        tick_timer = clock();
    }

    timeCounter = std::thread(updateTime, this);

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    blinkTime.restart();
    checkPointInitial = blinkTime.getElapsedTime().asSeconds();

    while (!escape && !pauseMode && !outOfTime && !arrival)
        updateRound(input);

    timeCounter.join();

    if (pauseMode)
        Audio::pauseSfxForReanudate();

    if (arrival)
        return State::END_ROUND;
    else if (escape)
        return State::EXIT;
    else if (pauseMode)
        return State::PAUSE;
    else if (outOfTime){
        return State::GAME_OVER;
    }
}

State Game::endRound(Input& input){

    escape = false;
    bool arrivalCar = false;

    player->setEndAnimation(true);
    Logger::setLoggerStatus();

    HudBonus::loadHudBonus(input);

    if (Audio::isPlaying(input.currentSoundtrack))
            Audio::stop(input.currentSoundtrack);

    for (int i = (int)Sfx::FERRARI_ENGINE_START; i <= (int)Sfx::FERRARI_CRASH; i++)
        Audio::stop(static_cast<Sfx>(i));

    if (!Audio::isPlaying(Sfx::RACE_END_FIRST) &&
        !Audio::isPlaying(Sfx::RACE_END_SECOND) &&
        !Audio::isPlaying(Sfx::RACE_END_THIRD))
    {
        int code = random_int((int)Sfx::RACE_END_FIRST, (int)Sfx::RACE_END_THIRD);
        Audio::play(static_cast<Sfx>(code), false);
    }

    if (!Audio::isPlaying(Sfx::SCORE_BONUS))
        Audio::play(Sfx::SCORE_BONUS, true);

    decsTimeBonus = timeToPlay * 10;
    secondsBonus = decsTimeBonus / 10;
    decs_secondBonus = decsTimeBonus % 10;

    bonusClock.restart().asSeconds();
    bonusTime = bonusClock.getElapsedTime().asSeconds();
    totalScore = score + (timeToPlay * SCORE_BONIFICATION);

    while (!escape && (!arrivalCar || score < totalScore)){
        updateRound(input);
        if (player->getSpeed() <= 0.f)
            arrivalCar = true;
    }

    if (escape)
        return State::EXIT;
    else {
        int i = 0;
        while (!escape && i <= 100){
            handleEvent(input, time);
            input.gameWindow.clear();
            currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);
            player->drawEndDriftRound(input);
            HudRound::drawHudRound(input);
            input.gameWindow.display();
            i++;
        }

        if (escape)
            return State::EXIT;
        else {
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

            if (escape)
                return State::EXIT;
        }
    }
    countHudBonus = 0;
    spectatorsCongrats = false;
    showmanCongrats = false;
    firstLoad = true;
    return State::RANKING;
}


State Game::gameOverRound(Input& input){

    if (player->getOutiseRoad())
        player->setOutsideRoad(false);

    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                          player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

    HudRound::setAllHudRoundIndicators(input);

    Audio::stop(input.currentSoundtrack);
    Audio::play(Soundtrack::GAME_OVER, true);

    while (!escape && !start){
        handleEvent(input, time);
        currentMap->renderMap(input, cars, *player, gameStatus, pauseMode);
        player->drawPlayRound(input, true, false);
        HudRound::drawHudRound(input);
        input.gameWindow.display();
    }

    Audio::stop(Soundtrack::GAME_OVER);

    if (start){
        sf::RectangleShape shape;
        shape.setPosition(0, 0);
        shape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
        shape.setFillColor(sf::Color(0, 0, 0, 200));

        for (int i = 0; i <= 70; i++){
            shape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(shape);
            input.gameWindow.display();
            sf::sleep(sf::milliseconds(10));
        }
        return State::START;
    }
    else if (escape)
        return State::EXIT;
}


void Game::loadBiomes(Input& input){

    Logger::setWidthScreen(input.gameWindow.getSize().x);

    const int nobjects[] = {26, 36, 39, 48, 53, 51, 38, 35, 33, 51, 27, 41, 40, 41, 49};
    int nm = 0;
    currentMap = new Map();
    std::string path = "";
    biomes.clear();

    failBiomesLoaded = false;

    for (int i = 0; i < 5; i++) {
        vector<Biome> bm;

        for (int j = 0; j <= i; j++){

            path = "Resources/Maps/MapLevels/Map" + std::to_string(nm + 1);

            Logger::setFailDetected(Logger::checkMapFile(path + "/map.txt"));
            path += "/";

            if (!Logger::getFailDetected()){

                Biome* currentBiome = new Biome();
                Logger::setFailDetected(Logger::checkTimeTerrainRoad(*currentBiome));

                if (!Logger::getFailDetected())
                    Logger::setFailDetected(Logger::checkColors(*currentBiome));
                else
                    failBiomesLoaded = true;

                if (!Logger::getFailDetected())
                    Logger::setFailDetected(Logger::checkBiomeRelief(*currentBiome));
                else
                    failBiomesLoaded = true;

                currentBiome->setBackgroundFront(path + "/front.png");
                currentBiome->setBackgroundBack(path + "/back.png");

                vector<std::string> objectNames;
                if (nm == 0){
                    objectNames.reserve(45);
                    for (int i = 1; i <= 45; i++)
                        objectNames.push_back(std::to_string(i));

                    path = "Resources/Maps/MapStartGoal/";
                    currentBiome->loadObjects(path, objectNames);
                    objectNames.clear();
                    path = "Resources/Maps/MapLevels/Map" + std::to_string(nm + 1) + "/";
                }

                objectNames.reserve((unsigned long) nobjects[nm]);
                for (int no = 1; no <= nobjects[nm]; no++)
                    objectNames.push_back(to_string(no));

                currentBiome->loadObjects(path, objectNames);

                if (nm == 0)
                    currentBiome->setStartBiome();

                if (!Logger::getFailDetected())
                    Logger::setFailDetected(Logger::checkLevelBiomeSprites(*currentBiome));
                else
                    failBiomesLoaded = true;

                bm.push_back(*currentBiome);
                Logger::close();
                nm++;
            }
            else
                failBiomesLoaded = true;

            if (failBiomesLoaded)
                break;
        }

        if (failBiomesLoaded){
            Logger::setFailDetected(true);
            break;
        }

        biomes.emplace_back(bm);
    }

    if (!Logger::getFailDetected()){
        currentMap->setCurrentBiome(biomes[0][0]);
        currentMap->setTerrain(currentMap->getCurrentBiome()->getTerrain());
        currentMap->setMapDistanceAndTrackLength();
        currentMap->setMapColors();
        Logger::setSpriteScreenY(*currentMap->getCurrentBiome());

        goalBiome = new Biome();
        goalBiome->setGoalBiome();

        for (int i = 0; i <= 3; i++){
            for (int j = 0; j <= i; j++){
                biomes[i][j].left = &biomes[i + 1][j];
                biomes[i][j].right = &biomes[i + 1][j + 1];
            }
        }

        for (int i = 0; i <= 4; i++){
            biomes[4][i].left = goalBiome;
            biomes[4][i].right = goalBiome;
        }

        Logger::close();
    }
    else
        failBiomesLoaded = true;

    biomesLoadDone = true;
}

void Game::run(Input& input){

    biomesLoader = std::thread(loadBiomes, this, ref(input));
    biomesLoader.detach();

    while (input.gameWindow.isOpen() && gameStatus != State::EXIT){
        switch (gameStatus) {
            case State::ANIMATION: {
                MenuLogo ml = MenuLogo();
                ml.loadMenu(input);
                ml.draw(input);
                gameStatus = ml.returnMenu(input);
                break;
            }
            case State::START: {
                if (!firstLoad){
                    biomesLoader = std::thread(loadBiomes, this, ref(input));
                    biomesLoader.detach();
                }
                MenuStart mS = MenuStart();
                mS.setMenuStart(firstLoad, pauseMode, outOfTime);
                mS.loadMenu(input);
                mS.draw(input);
                gameStatus = mS.returnMenu(input);
                firstLoad = mS.getFirstLoad();
                break;
            }
            case State::GAME: {
                playerCarSelected = 0;
                MenuGame mG = MenuGame();
                mG.loadMenu(input);
                mG.draw(input);
                gameStatus = mG.returnMenu(input);
                break;
            }
            case State::CREDITS: {
                MenuCredits mC = MenuCredits();
                mC.loadMenu(input);
                mC.draw(input);
                gameStatus = mC.returnMenu(input);
                break;
            }
            case State::OPTIONS: {
                MenuOptions mO = MenuOptions();
                mO.setPauseMode(pauseMode);
                mO.loadMenu(input);
                mO.draw(input);
                gameStatus = mO.returnMenu(input);

                if (currentMap != nullptr)
                    Logger::setSpriteScreenY(*currentMap->getCurrentBiome());

                if (player != nullptr){
                    float scale = (input.currentIndexResolution <= 1) ? 3.2f : 3.5f;
                    player->setScale(scale);
                }
                break;
            }
            case State::MUSIC_CONF: {
                MenuSound mS = MenuSound();
                mS.loadMenu(input);
                mS.draw(input);
                gameStatus = mS.returnMenu(input);
                break;
            }
            case State::CONTROLS: {
                MenuControls mC = MenuControls();
                mC.loadMenu(input);
                mC.draw(input);
                gameStatus = mC.returnMenu(input);
                break;
            }
            case State::GEARS: {
                MenuGears mGe = MenuGears(playerCarSelected);
                mGe.loadMenu(input);
                mGe.draw(input);
                automaticMode = mGe.getAutomaticMode();
                gameStatus = mGe.returnMenu(input);
                playerCarSelected = -1;
                break;
            }
            case State::VEHICLE: {
                MenuCarSelection mCs = MenuCarSelection();
                mCs.loadMenu(input);
                mCs.draw(input);
                playerCarSelected = mCs.hasPlayerCarSelected();
                gameStatus = mCs.returnMenu(input);
                break;
            }
            case State::MUSIC: {
                MenuMusicRadio mMr = MenuMusicRadio();
                mMr.loadMenu(input);
                mMr.draw(input);
                gameStatus =  mMr.returnMenu(input);

                if (!biomesLoadDone)
                    while (!biomesLoadDone);

                if (failBiomesLoaded)
                    gameStatus = State::EXIT;

                break;
            }
            case State::LOADING: {
                MenuLoading mL = MenuLoading(automaticMode);
                mL.loadMenu(input);
                mL.draw(input);
                gameStatus = mL.returnMenu(input);
                break;
            }
            case State::PREPARE_ROUND: {
                gameStatus = this->startRound(input);
                break;
            }
            case State::PLAY_ROUND: {
                gameStatus = this->playRound(input);
                break;
            }
            case State::END_ROUND: {
                gameStatus = this->endRound(input);
                break;
            }
            case State::PAUSE:{
                HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, treeMapPos, checkPoint,
                                      player->getGear(), player->getSpeed(), player->getHighMaxSpeed());

                HudRound::setAllHudRoundIndicators(input);

                MenuPause mP = MenuPause(*currentMap, *player, cars, currentMap->getTerrain());
                mP.loadMenu(input);
                mP.draw(input);
                gameStatus = mP.returnMenu(input);
                break;
            }
            case State::GAME_OVER: {
                gameStatus = this->gameOverRound(input);
                break;
            }
            case State::RANKING: {
                MenuRanking mR = MenuRanking(score, (int)minutesTrip, (int)secsTrip, (int)cents_secondTrip);
                mR.loadMenu(input);
                mR.draw(input);
                gameStatus = mR.returnMenu(input);
                break;
            }
        }
    }
    Audio::stopMusic();
    Audio::stopSfx();
    delete currentMap;
    delete player;
}
