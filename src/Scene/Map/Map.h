
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
#include "../../Globals.h"
#include "../../Car/PlayerCar/PlayerCar.h"
#include "../../Car/TrafficCar/TrafficCar.h"

class Map {

    private:

        vector<Line*> lines;
        int position, iniPosition;

        sf::Color sky, sand1, sand2, road1,
                    road2, rumble1, rumble2, lane1, lane2,
                    sand, road, rumble, lane;

        int trackLength;
        float pWheelL, pWheelR;

        float drawDistance, offsetXBackground1;
        float segmentL, rumbleL;
        int mapLanes, mapDistance;
        int time, terrain;
        short lineW;

        int dist3, dist4, dist5, dist6, dist7, dist8, distM;

        sf::Texture backGround;
        sf::RectangleShape backgroundShape;

        sf::Texture backGround2;

        vector<sf::Texture> objects;

        vector<float> scaleCoeffs;

        vector<int> widthCollisionCoeffs;

        vector<fPoint> pivotLeftPoints;

        vector<fPoint> pivotRightPoints;

        vector<fPoint> pivotLeftColPoints;

        vector<fPoint> pivotRightColPoints;

        void addSegment(float curve, float y, bool mirror, float dist);

        void addMap(int enter, int hold, int leave, float curve, float y, bool mirror, int distance);

        float easeIn(float a, float b, float percent);

        float easeInOut(float a, float b, float percent);

        float distance(float a, float b);

        void drawBackground(Input& input, int x, int y, sf::RectangleShape background, float speed, fPoint scale, fPoint pivot);

    public:

        Map();

        Map(Input& input);

        ~Map();

        void setColors(const std::vector<sf::Color>& colorsOfMap);

        void setTime(const int _time);

        void setTerrain(const int _terrain);

        int getTime() const;

        int getTerrain() const;

        Line* getLine(const int& index);

        int computeRoadTracks(const int numTracks);

        void loadObjects(const string &path, const vector<string> &objectNames);

        void initMap();

        void drawQuad(Input &input, int x1, int y1, int width, int height, sf::Color sky, bool use_camera = true);

        void drawPoly4(Input &input, short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, sf::Color color);

        void updateCars(vector<TrafficCar*> cars, const PlayerCar& p, int long long& score);

        void updateCarPlayerWheels(PlayerCar& p);

        void updateMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, const float time, int long long& score);

        void renderMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p);

        void addSpriteInfo(int line, SpriteInfo* p, bool left);

};

#endif // MAP_H
