
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
    time = 0;
    score = 0;
    minutes = 0.f;
    secs = 0.f;
    cents_second = 0.f;
    minutesTrip = 0.f;
    secsTrip = 0.f;
    cents_secondTrip = 0.f;
    mapId = make_pair(0, 0);
    Audio::loadAll(input);
    level = 0;
    m = nullptr;
    player = nullptr;
    pauseMode = false;
    escape = false;
    outOfTime = false;
    arrival = false;
    start = false;
}

void Game::handleEvent(Input& input, const float& time){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escape = true;
        }
        else if (!outOfTime && !arrival && input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            pauseMode = true;
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
        }
        else if (outOfTime && input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            start = true;
            Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
        }
    }
    if (!escape && !pauseMode && !outOfTime && !arrival && !player->getCrashing())
        player->accelerationControlAutomaic(input, time);
}



void Game::updateRound(Input& input){

    time = (float)((clock() - tick_timer) / (double)CLOCKS_PER_SEC);
    tick_timer = clock();
    handleEvent(input, time);

    Hud::setHud(timeToPlay, score, minutes, secs, cents_second, level, player->getGear(), player->getSpeed(), player->getMaxSpeed());
    Hud::setAllHudIndicators(input);

    m->updateMap(input, cars, *player, time, score);
    m->renderMap(input, cars, *player);
    player->draw(input, false);
    Hud::drawHud(input);
    input.gameWindow.display();

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

    //Player only scores if in road
	if (player->getStateWheelLeft() != StateWheel::SAND && player->getStateWheelRight() != StateWheel::SAND && player->getSpeed() > 5.f)
		score += (int)((10.f + 950.f * ((player->getSpeed() - 5.f) / (145.f))) / 10.f) * 10;
}



void Game::playRound(Input& input){

    if (!pauseMode){
        m = new Map(input);
        m->initMap();
        timeToPlay = m->getTime();
        player = new PlayerCar(0.f, 0, (int)(CAMERA_HEIGHT * CAMERA_DISTANCE) + 241, 0.f, PLAYER_TEXTURES,
                               "Ferrari", automaticMode);

        tick_timer = clock();
        Hud::loadHud();
        Hud::setHud(timeToPlay, score, minutes, secs, cents_second, level, player->getGear(), player->getSpeed(), player->getMaxSpeed());
        Hud::configureHud(input);

        TrafficCar* car1 = new TrafficCar(0, 0, 190.f * SEGMENT_LENGTH, 120.f, TRAFFIC_TEXTURES, "TrafficCars/Car1", 1, 0.5f, false, true, 1);
        TrafficCar* car2 = new TrafficCar(0, 0, 170.f * SEGMENT_LENGTH, 120.f, TRAFFIC_TEXTURES, "TrafficCars/Car2", 2, 0.f, false, true, 1);
        TrafficCar* car3 = new TrafficCar(0, 0, 165.f * SEGMENT_LENGTH, 120.f, TRAFFIC_TEXTURES, "TrafficCars/Car3", 3, 0.5f, false, true, 1);
        TrafficCar* car4 = new TrafficCar(0, 0, 160.f * SEGMENT_LENGTH, 120.f, TRAFFIC_TEXTURES, "TrafficCars/Car4", 4, -0.5f, false, true, 1);
        TrafficCar* car5 = new TrafficCar(0, 0, 155.f * SEGMENT_LENGTH, 120.f, TRAFFIC_TEXTURES, "TrafficCars/Car5", 5, 0.5f, false, true, 1);
        TrafficCar* car6 = new TrafficCar(0, 0, 150.f * SEGMENT_LENGTH, 120.f, TRAFFIC_TEXTURES, "TrafficCars/Car6", 6, 0.f, false, true, 1);

        cars.push_back(car1);
        cars.push_back(car2);
        cars.push_back(car3);
        cars.push_back(car4);
        cars.push_back(car5);
        cars.push_back(car6);

    }
    else {
        pauseMode = false;
        outOfTime = false;
        arrival = false;
        tick_timer = clock();
    }

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    while (!escape && !pauseMode && !outOfTime && !arrival){
        updateRound(input);
    }
}


State Game::returnRound(){
    if (escape){
        return State::EXIT;
    }
    else if (pauseMode){
        return State::PAUSE;
    }
    else if (outOfTime){
        return State::GAME_OVER;
    }
}


State Game::gameOverRound(Input& input){

    Hud::setHud(timeToPlay, score, minutes, secs, cents_second, level, player->getGear(), player->getSpeed(), player->getMaxSpeed());
    Hud::setAllHudIndicators(input);
    Audio::play(Soundtrack::GAME_OVER, true);

    while (!escape && !start){
        handleEvent(input, time);
        m->renderMap(input, cars, *player);
        player->draw(input, true, false);
        Hud::drawHud(input);
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
            case State::LOADING: {
                MenuLoading mL = MenuLoading(automaticMode);
                mL.loadMenu(input);
                mL.draw(input);
                gameStatus = mL.returnMenu(input);
                break;
            }
            case State::PLAY: {
                this->playRound(input);
                gameStatus = this->returnRound();
                break;
            }
            case State::PAUSE: {
                MenuPause mP = MenuPause(*m, *player, cars);
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
                break;
            }
        }
    }
    Audio::stopMusic();
    Audio::stopSfx();
    delete m;
    delete player;
}


