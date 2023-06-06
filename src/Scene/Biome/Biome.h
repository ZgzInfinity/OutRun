
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
 * Interface file of the module Biome
 */

#pragma once

#ifndef BIOME_H
#define BIOME_H

#include <vector>
#include <iostream>
#include "../Line/Line.h"
#include "../../Logger/Logger.h"


/**
 * Structure that represents the different scenarios of the game
 */
struct Biome{

    // Vector of lines that compose all the scenario
	std::vector<Line*> lines;

	// Left scenario in the fork
	Biome *left;

	// Right scenario in the fork
	Biome *right;

    // Next scenario
	Biome *next;

	// Control if the next scenario is the goal
	bool end;

	// Distance to create the different tracks of the road
    int threeTracksDistance, fourTracksDistance, fiveTracksDistance, sixTracksDistance, sevenTracksDistance, eightTracksDistance, twoTracksDistance;

    // Line that controls when the swapping animation between scenarios starts
	int swapLine;

	// Last line of the scenario
	int lastLine;

	// Line when the checkpoint in located
	int lineCheckPoint;

	// Time of the scenario to be completed
    int time;

    // Type of terrain outside the road
    int terrain;

    // Type of terrain of the road
    int roadTerrain;

    // Textures and sprites of the two backgrounds the scenario
    sf::Texture backGroundFront, backGroundBack;
    sf::RectangleShape backgroundShapeFront, backgroundShapeBack;

    // Control when the swapping animation between scenarios has to be played
	bool biomeSwap;

	// Control if the scenario is the start
    bool startBiome;

    // Control if the scenario is the goal
    bool goalBiome;

    // Colors of the scenario that represents the odd and even lines
	sf::Color skyBiome, sandBiome1, sandBiome2, roadBiome1, roadBiome2,
              rumbleBiome1, rumbleBiome2, laneBiome1, laneBiome2, rumbleLane1, rumbleLane2;

    // Vector with all the sprites of the biome
    vector<sf::Texture> objects;

    // Vector with all the colliders of the sprites
    vector<bool> collisions;

    // Vector with all the scale coefficients of the sprites
    vector<float> scaleCoeffs;

    // Vector with all the width crash coefficients of the sprites
    vector<int> widthCollisionCoeffs;

    /*
     * Vector with all the show terrain coefficients of the sprites
     * They are used to display the wheel effects without crashing
     */
    vector<int> showTerrainCoeffs;

    // Vector with all the left lower points to draw the sprites
    vector<fPoint> pivotLeftPoints;

    // Vector with all the right lower points to draw the sprites
    vector<fPoint> pivotRightPoints;

    // Vector with all the left lower points to control the crash collisions
    vector<fPoint> pivotLeftColPoints;

    // Vector with all the right lower points to control the crash collisions
    vector<fPoint> pivotRightColPoints;

    // Vector with all the floating sprites of the scenario
    vector<int> floatingSprites;



    /**
     * Default constructor
     */
	Biome();



	/**
	 * Set the time of the scenario to be completed
	 * @param _time is the time of the scenario to be completed by the player
	 */
    void setTime(const int _time);



    /**
     * Set the type of terrain outside the road
     * @param _terrain is the type of terrain (SAND, GRASS, SNOW or MUD)
     * and it is represents with integers from (1..4)
     */
    void setTerrain(const int _terrain);



    /**
     * Set the type of terrain outside the road
     * @param _terrain is the type of terrain (SAND, GRASS, SNOW or MUD)
     * and it is represents with integers from (1..4). The value 0 means
     * that the road is asphalted
     */
    void setRoadTerrain(const int _roadTerrain);



    /**
     * Set the second background of the scenario (moves quicker)
     * @param path is the file where the image is located
     */
    void setBackgroundFront(const string path);



    /**
     * Set the second background of the scenario (moves slower)
     * @param path is the file where the image is located
     */
    void setBackgroundBack(const string path);



    /**
     * Load all the sprites of the scenario
     * @param path stores the path of the scenario
     * @param objectNames stores the paths from the sprites in that scenario
     */
    void loadObjects(const string &path, const vector<string> &objectNames);



    /**
     * Set the biome like the starting biome
     */
    void setStartBiome();



    /**
     * Set the biome like the goal biome
     */
    void setGoalBiome();



    /**
     * Set the colors of the scenario to paint the lines of it
     * @param colorsOfMap is the vector with all the colors of the scenario
     */
    void setColors(const std::vector<sf::Color>& colorsOBiome);



    /**
     * Set the position in axis Y from a floating sprite of the scenario
     * @param index is the identifier whose position in axis Y has to be modified
     * @param _offsetY is the new position of the sprite in axis Y
     */
    void setSpriteScreenY(const int index, const float _offsetY);



    /**
     * Determine the distance between tracks that the road is going to have
     * @param numTracks is the number of tracks of that road portion
     */
    int computeRoadTracks(const int numTracks);



    /**
     * Create a gradual upping slope between to points according to percent slope
     * @param a is the starting reference point of the upping slope
     * @param b is the ending reference point of the upping slope
     * @param percent is the factor to build the upping slope
     */
    float easeIn(float a, float b, float percent);



    /**
     * Create a gradual descending slope between to points according to percent slope
     * @param a is the starting reference point of the descending slope
     * @param b is the ending reference point of the descending slope
     * @param percent is the factor to build the descending slope
     */
    float easeInOut(float a, float b, float percent);



    /**
     * Add a segment part relief to the biome and increments the number of total lines
     * @param curve is the curve coefficient of the new portion
     * @param y is the final position of the relief in axis Y
     * @param mirror controls if the relief has to be duplicated (fork effect)
     * @param dist is the width distance of the road in that part of the relief
     * @param linesOfBiome is the number of lines in the scenario
     */
    void addSegment(float curve, float y, bool mirror, float dist, int& linesOfBiome);



    /**
     * Add a new sub-biome part to the current biome and increments the number of total lines
     * @param enter is the index where the new biome starts
     * @param hold is the length of the new sub-biome part
     * @param leave is the index where the new biome ends
     * @param curve is the curve coefficient of the new portion
     * @param y is the final position of the relief in axis Y
     * @param mirror controls if the relief has to be duplicated (fork effect)
     * @param forceSwap controls when the transition between must be forced
     * @param distance is the width distance of the road in that part of the relief
     * @param linesOfBiome is the number of lines in the scenario
     */
    void addBiome(int enter, int hold, int leave, float curve, float y, bool mirror, bool forceSwap, int distance, int& linesOfBiome);



    /**
     * Add a sprite to the biome
     * @param line is the index of the line were the sprite has to be inserted
     * @param spritePos is the position of the sprite in the road using like reference the road
     * @param isCheckPoint controls if the sprite to add is the checkpoint
     */
    void addSpriteInfo(int line, SpriteInfo* p, const Sprite_Position spritePos, const bool isCheckPoint);



    /**
     * Get the time of the scenario
     */
    int getTime() const;



    /**
     * Get the type of terrain of the scenario outside the road
     */
    int getTerrain() const;



    /**
     * Get the type of terrain of the road
     */
    int getRoadTerrain() const;



    /**
     * Get if the biome is the starting scenario
     */
    bool getStartBiome() const;



    /**
     * Get if the biome is the goal scenario
     */
    bool getGoalBiome() const;



    /**
     * Get the left scenario in the fork
     */
    Biome* getLeft() const;



    /**
     * Get the right scenario in the fork
     */
    Biome* getRight() const;



    /**
     * Get the next biome
     */
    Biome* getNext() const;



    /**
     * Return a concrete line of the scenario
     * @param n is the index of the line to get
     * @param baseline is the reference line of the scenario to look for the desire line
     */
    Line* getLine(const int n, const Line* baseLine);


};

#endif // BIOME_H
