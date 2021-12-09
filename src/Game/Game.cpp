
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

Game::Game(Input& input){
    gameStatus = State::ANIMATION;
    automaticMode = false;
    firstLoad = true;
    spectatorsCongrats = false;
    showmanCongrats = false;
    countHudBonus = 0;
    time = 0;
    minutes = 0.f;
    secs = 0.f;
    cents_second = 0.f;
    minutesTrip = 0.f;
    secsTrip = 0.f;
    cents_secondTrip = 0.f;
    mapId = make_pair(0, 0);
    Audio::loadAll(input);
    level = 0;
    currentMap = nullptr;
    player = nullptr;
    pauseMode = false;
    escape = false;
    outOfTime = false;
    arrival = false;
    start = false;
    firstGame = false;
    endingAnimation = false;
    startingRound = true;
}

void Game::handleEvent(Input& input, const float& time){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escape = true;
        }
        else if (gameStatus == State::PLAY_ROUND){
            if (!outOfTime && !arrival && input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                pauseMode = true;
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            }
            else if (outOfTime && input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
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

    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, player->getGear(), player->getSpeed(), player->getHighMaxSpeed());
    HudRound::setAllHudRoundIndicators(input);

    if (gameStatus != State::GAME_OVER)
        currentMap->updateMap(input, cars, *player, gameStatus, time, score);

    currentMap->renderMap(input, cars, *player, gameStatus);

    if (gameStatus == State::PLAY_ROUND)
        arrival = currentMap->getEnding();

    if (gameStatus == State::PLAY_ROUND)
        player->drawPlayRound(input, false);
    else
        player->drawEndDriftRound(input);

    HudRound::drawHudRound(input);

    if (gameStatus == State::PLAY_ROUND){
        elapsed4 = gameClockLap.getElapsedTime().asSeconds();

        if (elapsed4 - elapsed3 >= shot_delayLap.asSeconds()) {
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
        }

        elapsed2 = gameClockTime.getElapsedTime().asSeconds();

        if (elapsed2 - elapsed1 >= shot_delayTime.asSeconds()) {
            timeToPlay--;
            if (timeToPlay == 10)
                Audio::play(Sfx::BLOND_WOMAN_TEN_SECONDS, false);
            else if (timeToPlay < 10) {
                if (timeToPlay == 5)
                    Audio::play(Sfx::BLONDE_WOMAN_HURRY_UP, false);
                else if (timeToPlay == 0)
                    outOfTime = true;
                Audio::play(Sfx::COUNTDOWN, false);
            }

            gameClockTime.restart();
        }
    }
    else if (gameStatus == State::END_ROUND){
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
        if (player->getStateWheelLeft() != StateWheel::SAND && player->getStateWheelRight() != StateWheel::SAND && player->getSpeed() > 5.f)
            score += (int)((10.f + 950.f * ((player->getSpeed() - 5.f) / (141.f))) / 3.7f) * 2.3f;
    }

    if (!currentMap->getNotDrawn())
        input.gameWindow.display();
    else
        currentMap->setNotDrawn(false);
}

State Game::startRound(Input& input){

    score = 0;
    startingRound = true;
    timeToPlay = currentMap->getCurrentBiome()->getTime();
    float scale = (input.currentIndexResolution <= 1) ? 3.2f : 3.5f;
    player = new PlayerCar(0.f, 0, (int)(CAMERA_HEIGHT * CAMERA_DISTANCE) + 241, 0.f, scale,
                           "Ferraris/Ferrari1", automaticMode, false);

    HudRound::loadHudRound();
    HudRound::setHudRound(timeToPlay, score, minutes, secs, cents_second, level, player->getGear(), player->getSpeed(), player->getHighMaxSpeed());
    HudRound::configureHudRound(input);

    TrafficCar* car1 = new TrafficCar(0, 0, 190.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car1", 1, 0.5f, false, true, 1, true);
    TrafficCar* car2 = new TrafficCar(0, 0, 170.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car2", 2, 0.f, false, true, 1, true);
    TrafficCar* car3 = new TrafficCar(0, 0, 165.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car3", 3, 0.5f, false, true, 1, true);
    TrafficCar* car4 = new TrafficCar(0, 0, 160.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car4", 4, -0.5f, false, true, 1, true);
    TrafficCar* car5 = new TrafficCar(0, 0, 155.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car5", 5, 0.5f, false, true, 1, true);
    TrafficCar* car6 = new TrafficCar(0, 0, 150.f * SEGMENT_LENGTH, 120.f, "TrafficCars/Car6", 6, 0.f, false, true, 1, true);

    cars.push_back(car1);
    cars.push_back(car2);
    cars.push_back(car3);
    cars.push_back(car4);
    cars.push_back(car5);
    cars.push_back(car6);

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
        currentMap->renderMap(input, cars, *player, gameStatus);
        player->drawStartStaticRound(input);
        HudRound::drawHudRound(input);
        input.gameWindow.draw(blackShape);
        input.gameWindow.display();
        j -= 5;
    }

    while (!player->getEndAnimation() && !escape){
        handleEvent(input, time);

        input.gameWindow.clear();
        currentMap->renderMap(input, cars, *player, gameStatus);
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
    currentMap->renderMap(input, cars, *player, gameStatus);
    player->drawPlayRound(input, true);
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

        currentMap->renderMap(input, cars, *player, gameStatus);
        player->drawPlayRound(input, true);
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
        tick_timer = clock();
    }

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    while (!escape && !pauseMode && !outOfTime && !arrival)
        updateRound(input);

    if (arrival)
        return State::END_ROUND;
    else if (escape)
        return State::EXIT;
    else if (pauseMode)
        return State::PAUSE;
    else if (outOfTime)
        return State::GAME_OVER;
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
            currentMap->renderMap(input, cars, *player, gameStatus);
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

    while (!escape && !start){
        handleEvent(input, time);
        currentMap->renderMap(input, cars, *player, gameStatus);
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


State Game::loadBiomes(Input& input){

    Logger::setWidthScreen(input.gameWindow.getSize().x);
    Logger::setFailDetected(Logger::checkMapFile("Resources/Maps/MapLevels/Map1/map.txt"));

    if (!Logger::getFailDetected()){

        currentMap = new Map();
        Biome* currentBiome = new Biome();

        Logger::setFailDetected(Logger::checkTimeAndTerrain(*currentBiome));

        if (!Logger::getFailDetected())
            Logger::setFailDetected(Logger::checkColors(*currentBiome));
        else
            return State::EXIT;

        if (!Logger::getFailDetected())
            Logger::setFailDetected(Logger::checkBiomeRelief(*currentBiome));
        else
            return State::EXIT;

        vector<string> objectNames;
        objectNames.reserve(45);
        for (int i = 1; i <= 45; i++){
            objectNames.push_back(std::to_string(i));
        }

        string path = "Resources/Maps/MapStartGoal/";
        currentBiome->loadObjects(path, objectNames);

        path = "Resources/Maps/MapLevels/Map1/";
        objectNames.clear();
        objectNames.reserve(26);
        for (int i = 1; i <= 26; i++){
            objectNames.push_back(std::to_string(i));
        }

        currentBiome->loadObjects(path, objectNames);

        currentBiome->setBackground("Resources/Maps/MapLevels/Map1/bg.png");
        currentBiome->setStartBiome();

        if (!Logger::getFailDetected())
            Logger::setFailDetected(Logger::checkLevelBiomeSprites(*currentBiome));
        else
            return State::EXIT;

        currentMap->setCurrentBiome(*currentBiome);
        currentMap->setMapDistanceAndTrackLength();
        currentMap->setMapColors();

        Biome* goalBiome = new Biome();
        goalBiome->setGoalBiome();
        currentMap->getCurrentBiome()->left = goalBiome;
        currentMap->getCurrentBiome()->right = goalBiome;

        Logger::close();
        return State::LOADING;
    }
    else
        return State::EXIT;
}

void Game::run(Input& input){
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
                MenuStart mS = MenuStart();
                mS.setMenuStart(firstLoad, pauseMode, outOfTime);
                mS.loadMenu(input);
                mS.draw(input);
                gameStatus = mS.returnMenu(input);
                firstLoad = mS.getFirstLoad();
                break;
            }
            case State::GAME: {
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

                if (currentMap != nullptr){
                    bool startMap = currentMap->getStartMap();

                    if (firstGame && (startMap || currentMap->getgoalMap()))
                        Logger::setSpriteScreenY(*currentMap->getCurrentBiome(), startMap);
                }

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
                MenuGears mGe = MenuGears();
                mGe.loadMenu(input);
                mGe.draw(input);
                automaticMode = mGe.getAutomaticMode();
                gameStatus = mGe.returnMenu(input);
                break;
            }
            case State::MUSIC: {
                MenuMusicRadio mMr = MenuMusicRadio();
                mMr.loadMenu(input);
                mMr.draw(input);
                gameStatus =  mMr.returnMenu(input);
                break;
            }
            case State::LOAD_MAPS: {
                gameStatus = this->loadBiomes(input);
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
            case State::PAUSE: {
                MenuPause mP = MenuPause(*currentMap, *player, cars);
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
                score = 1000;
                minutesTrip = 2;
                secsTrip = 35;
                cents_secondTrip = 44;
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
