
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

#pragma once

#ifndef GAME_H
#define GAME_H

#include "../Input/Input.h"
#include "../Audio/Audio.h"
#include "../Menus/Menu/Menu.h"
#include "../Scene/Map/Map.h"
#include "../Menus/MenuLogo/MenuLogo.h"
#include "../Menus/MenuStart/MenuStart.h"
#include "../Menus/MenuGame/MenuGame.h"
#include "../Menus/MenuGears/MenuGears.h"
#include "../Menus/MenuMusicRadio/MenuMusicRadio.h"
#include "../Menus/MenuCredits/MenuCredits.h"
#include "../Menus/MenuOptions/MenuOptions.h"
#include "../Menus/MenuSound/MenuSound.h"
#include "../Menus/MenuLoading/MenuLoading.h"
#include "../Menus/MenuControls/MenuControls.h"
#include "../Menus/MenuPause/MenuPause.h"
#include "../Car/TrafficCar/TrafficCar.h"
#include "../Gui/Huds/HudRound/HudRound.h"
#include "../Gui/Huds/HudBonus/HudBonus.h"
#include "../Logger/Logger.h"

 class Game {

    private:

        const sf::Time shot_delayTime = sf::seconds(1.0);

        const sf::Time shot_delayLap = sf::seconds(0.01);

        const sf::Time bonus_delay = sf::seconds(0.01);

        sf::Clock gameClockTime;

        sf::Clock gameClockLap;

        sf::Clock bonusClock;

        State gameStatus;

        bool automaticMode;

        bool firstLoad;

        bool spectatorsCongrats, showmanCongrats;

        float time;

        clock_t tick_timer;

        int timeToPlay, decsTimeBonus, secondsBonus, decs_secondBonus, countHudBonus;

        long long int score, totalScore;

        float minutes;

        float secs;

        float cents_second;

        float minutesTrip;

        float secsTrip;

        float cents_secondTrip;

        int level;

        Map* currentMap;

        vector<TrafficCar*> cars;

        PlayerCar* player;

        bool pauseMode;

        bool escape;

        bool outOfTime;

        bool arrival;

        bool firstGame;

        bool start;

        bool endingAnimation;

        bool startingRound;

        float elapsed1, elapsed2, elapsed3, elapsed4, bonusTime, elapsedBonusTime;

        std::pair<int, int> mapId;

    public:

        Game(Input& input);

        void handleEvent(Input& input, const float& time);

        void updateRound(Input& input);

        State startRound(Input& input);

        State playRound(Input& input);

        State endRound(Input& input);

        State gameOverRound(Input& input);

        State loadBiomes(Input& input);

        void run(Input& input);
 };

 #endif // GAME_H
