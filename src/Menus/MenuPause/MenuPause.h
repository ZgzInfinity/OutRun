
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
 * Interface file of the module MenuPause
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



/**
 * Represents the pause menu of the game
 */
class MenuPause : public Menu {

    private:

        // Black screen to focus the attention in the menu
        sf::RectangleShape blackScreen;

        // Panel of the menu that contains all the buttons
        sf::RectangleShape pausePanel;

        // Vector with all the menu buttons
        vector<Button> menuButtons;

        // Vector with all the traffic cars to be drawn
        vector<TrafficCar*> cars;

        // Font of the text indicators of the menu
        sf::Font fontMenu;

        // Title text of the menu
        sf::Text titleTextMenu;

        // Scenario to be drawn
        Map m;

        // Car of the player
        PlayerCar player;

        // Status of the game
        State gameStatus;

        // Terrain of the scenario to be drawn
        int terrain;

    public:



        /**
         * Default constructor
         * @param _m is the scenario to be drawn in the screen
         * @param _p is the player car
         * @param _trafficCars is a vector that stores all the traffic cars of the game round
         * @param _terrain is the type of terrain outside the road in
         * the scenario (SAND, GRASS, SNOW or MUD) presented with values (1..4)
         */
        MenuPause(const Map& _m, const PlayerCar& _p, const vector<TrafficCar*> _trafficCars, const int _terrain);



        /**
         * Set the game status of the game
         * @param _gameStatus is the current status of the game
         */
        void setGameStatus(const State _gameStatus);



        /**
         * Change the option menu selected
         * @param menuUpPressed controls if the menu_up key has been or not pressed
         */
        void changeButtonSelected(const bool menuUpPressed);



        /**
         * Load the menu with all its configuration
         * @param input is the module that has all the configuration of the game
         */
        void loadMenu(Input& input) override;



        /**
         * Detect an action of the player and executes it
         * @param input is the module that has all the configuration of the game
         */
        void handleEvent(Input& input) override;



        /**
         * Draw the menu in the screen
         * @param input is the module that has all the configuration of the game
         */
        void draw(Input& input) override;



        /**
         * Return the next status of the game after and option of the menu
         * has been selected by the player
         * @param input is the module that has all the configuration of the game
         */
        State returnMenu(Input& input) override;

};

#endif // MENU_OPTIONS_H

