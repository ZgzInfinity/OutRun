
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
}

void Game::handleEvent(Input& input, const float& time){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escape = true;
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            pauseMode = true;
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
        }
    }
    if (!escape && !pauseMode)
        player->accelerationControlAutomaic(input, time);
}


void Game::playRound(Input& input){

    if (!pauseMode){
        m = new Map(input);
        m->initMap();
        player = new PlayerCar();
        tick_timer = clock();
        Hud::loadHud();
        Hud::configureHud(input);

        TrafficCar* car1 = new TrafficCar(1, 0.5f, 120.f, 190.f * SEGMENT_LENGTH, false, true, 1);
        TrafficCar* car2 = new TrafficCar(2, 0.f, 120.f, 170.f * SEGMENT_LENGTH, false, true, 1);
        TrafficCar* car3 = new TrafficCar(3, 0.5f, 120.f, 165.f * SEGMENT_LENGTH, false, true, 1);
        TrafficCar* car4 = new TrafficCar(4, -0.5f, 120.f, 160.f * SEGMENT_LENGTH, false, true, 1);
        TrafficCar* car5 = new TrafficCar(5, 0.5f, 120.f, 155.f * SEGMENT_LENGTH, false, true, 1);
        TrafficCar* car6 = new TrafficCar(6, 0.f, 120.f, 150.f * SEGMENT_LENGTH, false, true, 1);

        cars.push_back(car1);
        cars.push_back(car2);
        cars.push_back(car3);
        cars.push_back(car4);
        cars.push_back(car5);
        cars.push_back(car6);

    }
    else {
        pauseMode = false;
        tick_timer = clock();
    }

    while (!pauseMode && !escape){
        time = (float)((clock() - tick_timer) / (double)CLOCKS_PER_SEC);
        tick_timer = clock();
        handleEvent(input, time);
        m->updateMap(input, cars, *player, time);
        m->renderMap(input, cars, *player);
        player->draw(input, false);
        Hud::drawHud(input);
        input.gameWindow.display();
    }
}


State Game::returnRound(){
    if (escape){
        return State::EXIT;
    }
    else if (pauseMode){
        return State::PAUSE;
    }
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
                mS.setMenuStart(firstLoad, pauseMode);
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


