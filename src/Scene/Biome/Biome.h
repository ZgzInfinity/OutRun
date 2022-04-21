
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

#ifndef BIOME_H
#define BIOME_H

#include <vector>
#include <iostream>
#include "../Line/Line.h"
#include "../../Logger/Logger.h"

struct Biome{

	std::vector<Line*> lines;

	Biome *left;

	Biome *right;

	bool end;

    int dist3, dist4, dist5, dist6, dist7, dist8, distM;

	int swapLine, lastLine, lineCheckPoint;

    int time, terrain, roadTerrain;

    sf::Texture backGroundFront, backGroundBack;
    sf::RectangleShape backgroundShapeFront, backgroundShapeBack;

	bool biomeSwap;

    bool startBiome, goalBiome;

	sf::Color skyBiome, sandBiome1, sandBiome2, roadBiome1, roadBiome2,
                rumbleBiome1, rumbleBiome2, laneBiome1, laneBiome2, rumbleLane1, rumbleLane2;

    vector<sf::Texture> objects;

    vector<bool> collisions;

    vector<float> scaleCoeffs;

    vector<int> widthCollisionCoeffs;

    vector<int> showTerrainCoeffs;

    vector<fPoint> pivotLeftPoints;

    vector<fPoint> pivotRightPoints;

    vector<fPoint> pivotLeftColPoints;

    vector<fPoint> pivotRightColPoints;

    vector<int> floatingSprites;

	Biome();

    void setTime(const int _time);

    void setTerrain(const int _terrain);

    void setRoadTerrain(const int _roadTerrain);

    int getTime() const;

    int getTerrain() const;

    bool getStartBiome() const;

    bool getGoalBiome() const;

    int getRoadTerrain() const;

    void setColors(const std::vector<sf::Color>& colorsOBiome);

    int computeRoadTracks(const int numTracks);

    float easeIn(float a, float b, float percent);

    float easeInOut(float a, float b, float percent);

    void addSegment(float curve, float y, bool mirror, float dist, int& linesOfBiome);

    void addBiome(int enter, int hold, int leave, float curve, float y, bool mirror, int distance, int& linesOfBiome);

    void addSpriteInfo(int line, SpriteInfo* p, const Sprite_Position spritePos, const bool isCheckPoint);

    void setBackgroundFront(const string path);

    void setBackgroundBack(const string path);

    void setStartBiome();

    void setGoalBiome();

    void setSpriteScreenY(const int index, const float _offsetY);

    void loadObjects(const string &path, const vector<string> &objectNames);

    Biome* getLeft() const;

    Biome* getRight() const;

    Line* getLine(const int n, const Line* baseLine);


};

#endif // HUD_BONUS_H
