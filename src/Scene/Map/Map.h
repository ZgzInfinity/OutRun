
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

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "../Line/Line.h"
#include "../Biome/Biome.h"
#include "../../Globals.h"
#include "../../Car/PlayerCar/PlayerCar.h"
#include "../../Car/TrafficCar/TrafficCar.h"
#include "../../Logger/Logger.h"
#include "../../Menus/Menu/Menu.h"

class Map {

    private:

        Biome* currentBiome;
        Biome* nextBiome;

        int position, iniPosition, numBiomesVisited;

        int trackLength, sameColors;
        float pWheelL, pWheelR;

        float drawDistance, offsetXBackground1, offsetXBackground2;
        int mapLanes, mapDistance, terrain;
        short lineW;
        bool swapping;
        float backgroundSwapOffset;
        bool ending;
        bool notDrawn;
        bool newBiomeChosen;

        sf::Color sand, road, rumble, lane, rumbleLane;
        sf::Color bg, bg2;
        sf::Color sky, sand2, road2, rumble2, lane2, rumbleLane2;
        sf::Color sandAux, sand2Aux, roadAux, road2Aux, rumbleAux, rumble2Aux, laneAux, lane2Aux, rumbleLaneAux, rumbleLane2Aux;

        sf::Texture backGroundFront, backGroundBack, backGround2Front, backGround2Back;

        void drawBackground(Input& input, int x, int y, sf::RectangleShape background, float speed, fPoint scale, fPoint pivot);

    public:

        Map();

        ~Map();

        void setCurrentBiome(Biome& b);

        Biome* getCurrentBiome() const;

        float distance(float a, float b);

        bool getStartMap() const;

        bool getgoalMap() const;

        void setMapDistanceAndTrackLength();

        void drawQuad(Input &input, int x1, int y1, int width, int height, sf::Color sky, bool use_camera = true);

        void drawPoly4(Input &input, short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, sf::Color color);

        void updateCars(Input& input, vector<TrafficCar*> cars, const PlayerCar& p, int long long& score);

        void updateCarPlayerWheels(PlayerCar& p);

        void updateMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, const float time, int long long& score,
                       bool& checkPoint, bool& checkPointDisplayed, int& treeMapPos, const int level);

        void renderMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, const bool pauseMode);

        void interpolateBiomes(Input& input, PlayerCar& p);

        sf::Color interpolateColors(const sf::Color& c1, const sf::Color& c2);

        bool sameColor(const sf::Color& c1, const sf::Color& c2);

        void setMapColors();

        bool getEnding() const;

        void setNotDrawn(const bool _notDrawn);

        bool getNotDrawn() const;

        void setTerrain(const int _terrain);

        int getTerrain() const;


};

#endif // MAP_H
