
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
 * Interface file of the module Map
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



/**
 * Mix all the scenarios of the game and controls
 * the status of the player car and the traffic cars
 * during the game round
 */
class Map {

    private:

        // Current scenario where the player is driving
        Biome* currentBiome;

        // Next scenario
        Biome* nextBiome;

        // Position of the player car in the current scenario
        int position;

        // Initial position of the player car in each scenario
        int iniPosition;

        // Number of scenarios visited
        int numBiomesVisited;

        // Total length of the scenario
        int trackLength;

        // Controls if the transition colors have finished (transition biomes animation)
        bool sameColors;

        // Positions of the left and right wheels in the screen (compute animation wheel effects
        float pWheelL, pWheelR;

        // Distance of the map in axis Z to be drawn
        float drawDistance;

        // Position of the first background in axis X (curve move)
        float offsetXBackground1;

        // Position of the second background in axis X (curve move)
        float offsetXBackground2;

        // Road width of the scenario
        int mapDistance;

        // Type of terrain outside the road
        int terrain;

        // Control if the transition animation between biomes is happening
        bool swapping;

        // Total swapping move of the backgrounds in axis X (transition effect)
        float backgroundSwapOffset;

        // Controls if the ending animation has to be played (goal map)
        bool ending;

        // Control when the player has selected the next scenario in the fork
        bool newBiomeChosen;

        // Main colors of the current scenario to paint the lines
        sf::Color sand, road, rumble, lane, rumbleLane;

        // Colors of the backgrounds to build the transition animation
        sf::Color bg, bg2;

        // Color of background of the current scenario
        sf::Color sky;

        // Colors of the scenario chosen in the fork
        sf::Color sand2, road2, rumble2, lane2, rumbleLane2;

        // Additional colors to compute progressively the changing of colors between scenarios
        sf::Color sandAux, sand2Aux, roadAux, road2Aux, rumbleAux, rumble2Aux, laneAux, lane2Aux, rumbleLaneAux, rumbleLane2Aux;

        /*
         * Textures of the background
         * Each scenario has two and also are store
         * the backgrounds of the scenario chosen in the fork
         */
        sf::Texture backGroundFront, backGroundBack, backGround2Front, backGround2Back;



        /**
         * Draw a relief rectangle of a concrete line of the scenario
         * @param input is the module that has all the configuration of the game
         * @param x1 is the upper left point in axis X of the rectangle
         * @param y1 is the upper left point in axis Y of the rectangle
         * @param x2 is the upper right point in axis X of the rectangle
         * @param y2 is the upper right point in axis Y of the rectangle
         * @param x3 is the lower left point in axis Y of the rectangle
         * @param y3 is the lower left point in axis Y of the rectangle
         * @param x4 is the lower right point in axis Y of the rectangle
         * @param y4 is the lower right point in axis Y of the rectangle
         * @param color is the color of the rectangle to be painted
         */
        void drawRelief(Input &input, const short x1, const short y1, const short x2, const short y2,
                        const short x3, const short y3, const short x4, const short y4, const sf::Color& color);



        /**
         * Draw the background of the current scenario
         * @param input is the module that has all the configuration of the game
         * @param x is the position of the background in the axis X
         * @param y is the position of the background in the axis Y
         * @param background contains the texture to be drawn as the background of the scenario
         * @param scale represents the scale factor to rescale the size of the image
         * @param pivot represents the coordinates of the 2d lower left point to start drawing the background
         */
        void drawBackground(Input& input, const int x, const int y, sf::RectangleShape background, const fPoint scale, const fPoint pivot);



        /**
         * Draw the sky of the current scenario
         * @param input is the module that has all the configuration of the game
         * @param x1 is the upper left corner of the sky rectangle in axis X
         * @param y1 is the upper left corner of the sky rectangle in axis Y
         * @param width is the width dimension of the sky rectangle
         * @param height is the height dimension of the sky rectangle
         * @param sky is the color of the sky in the current scenario
         */
        void drawSky(Input &input, const int x1, const int y1, const int width, const int height, const sf::Color sky);



        /**
         * Returns true if two colors are equal. Otherwise returns false
         * @param c1 is the first color to compare
         * @param c2 is the second color to compare
         */
        bool sameColor(const sf::Color& c1, const sf::Color& c2);



        /**
         * Return and interpolate progressive color between two interval colors
         * @param c1 is the initial color
         * @param c2 is the final color
         */
        sf::Color interpolateColors(const sf::Color& c1, const sf::Color& c2);



        /**
         * Make the transition between scenarios when the player is in the fork
         * @param input is the module that has all the configuration of the game
         * @param p is the player car
         */
        void interpolateBiomes(Input& input, PlayerCar& p);



        /**
         * Update the status of the wheels of the player car
         * @param p is the player car
         */
        void updateCarPlayerWheels(PlayerCar& p);



        /**
         * Update the status of all the traffic cars of the scenario
         * @param input is the module that has all the configuration of the game
         * @param cars is the vector with all the traffic cars of the scenario
         * @param p is the player car
         * @param score is the current score of the player
         * @param startCodeAi is the AI code of the traffic car to be possibly set
         */
        void updateCars(Input& input, vector<TrafficCar*> cars, const PlayerCar& p, int long long& score, int& startCodeAi);



    public:



        /**
         * Default constructor
         */
        Map();



        /**
         * Set the current biome
         * @param b is the current biome to be set and drawn
         */
        void setCurrentBiome(Biome& b);



        /**
         * Set the type of terrain outside the road
         * @param _terrain is the type of terrain (SAND, GRASS, SNOW or MUD)
         * and it is represents with integers from (1..4)
         */
        void setTerrain(const int _terrain);



        /**
         * Set the colors of the current biome
         */
        void setMapColors();



        /**
         * Set the total distance of the biome and add the fork part to the scenario
         * @param ending controls if the current scenario is the goal
         */
        void setMapDistanceAndTrackLength(const bool ending);



        /**
         * Update the map status of the game with the player and traffic cars to be after drawn
         * @param input is the module that has all the configuration of the game
         * @param cars is the vector with all the traffic cars
         * @param p is the player car
         * @param gameStatus is the status of the game
         * @param time is the real time elapsed between each iteration (allows the movement of the cars)
         * @param score is the current score of the player
         * @param checkPoint controls if the checkpoint animation has to be displayed
         * @param checkPointDisplayed controls if the checkpoint animation has been already displayed
         * @param treeMapPos is the current scenario where the player is driving in the tree indicator of the hud round (lower right corner)
         * @param level is the current level where the player is driving (1..5)
         * @param startCodeAi is the type of AI that can be assigned possibly to a traffic car
         */
        void updateMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, const float time, int long long& score,
                       bool& checkPoint, bool& checkPointDisplayed, int& treeMapPos, const int level, int& startCodeAi);



        /**
         * Draw the current scenario with all the traffic cars
         * @param input is the module that has all the configuration of the game
         * @param cars is the vector with all the traffic cars
         * @param p is the player car
         * @param gameStatus is the status of the game
         * @param pauseMode controls if the game is in pause mode or not
         */
        void renderMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, const bool pauseMode);



        /**
         * Get the current biome
         */
        Biome* getCurrentBiome() const;



        /**
         * Get the type of terrain outside the road
         */
        int getTerrain() const;



        /**
         * Get if the current scenario is the starting biome
         */
        bool getStartMap() const;



        /**
         * Get if the current scenario is the goal biome
         */
        bool getgoalMap() const;



        /**
         * Get if the ending animation has to be played
         */
        bool getEnding() const;


};

#endif // MAP_H
