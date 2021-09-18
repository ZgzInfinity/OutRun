
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

#ifndef MENU_PAUSE_H
#define MENU_PAUSE_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"
#include "../../Scene/Map/Map.h"
#include "../../Car/PlayerCar/PlayerCar.h"
#include "../../Car/TrafficCar/TrafficCar.h"
#include "../../Gui/Huds/HudRound/HudRound.h"

class MenuPause : public Menu {

    private:

        sf::RectangleShape shape, pauseShape;

        vector<Button> menuButtons;

        vector<TrafficCar*> cars;

        sf::Font fontMenu;
        sf::Text textMenu;

        Map map;
        PlayerCar player;

        State gameStatus;

    public:

        MenuPause(const Map& m, const PlayerCar& p, const vector<TrafficCar*> trafficCars);

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

        void changeButtonSelected(const bool& menuUpPressed);

        void setGameStatus(State& _gameStatus);

};

#endif // MENU_OPTIONS_H

