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
 * Interface file of the module TrafficCar
 */

#pragma once

#ifndef TRAFFIC_CAR_H
#define TRAFFIC_CAR_H


#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Vehicle/Vehicle.h"
#include "../PlayerCar/PlayerCar.h"
#include "../../Globals.h"
#include "../../Input/Input.h"
#include "../../Random/Random.h"
#include "../../Scene/Line/Line.h"
#include "../../Menus/Menu/Menu.h"

class PlayerCar;



/**
 * Represents the class that contains all the main information
 * in order to store the status of the traffic cars
 */
class TrafficCar : public Vehicle {

    private:

        // Identifier of the car
        int id;

        // Determines the movement of the car (left 0 and right 1) -1 unknown
        int pathSelected;

        // Time that has to be spent in order to do another movement
        int timeToReturn;

        // Distance between the center of the road and the position of the car in axis x
        float offset;

        // Destiny point in axis x
        float offsetDest;

        // Control if the car has to be drawn or not
        bool active;

        // Control if the car is in left (false) o right (true) road
        bool side;

        // Control if the player car is near of the traffic car
        bool playerClosed;

        // Types of AI
        enum class Traffic_Ai {
            PACIFIC,
            EVASIVE,
            OBSTACLE
        };

        // AI of the traffic car
        Traffic_Ai typeAi;

    public:



        /**
         * Default constructor
         */
        TrafficCar();



        /**
         * Read the properties associated to the traffic car and assign them to it
         * @param name is the path of the file that contains the properties of the car
         */
        void readProperties(const std::string& name);



        /**
         * Second constructor
         * @param _posX is the position of the car in axis X
         * @param _posY is the position of the car in axis Y
         * @param _posZ is the position of the car in axis Z
         * @param _speed is the initial speed of the car
         * @param _name is the path where the images of the car are stored
         * @param _id is the identifier of the traffic car
         * @param _offset is the distance between the position of the car in axis X and the center of the road
         * @param _active controls if the traffic car must be drawn or not
         * @param _side controls if the car is in the left or right road
         * @param _isTrafficCar controls if the car is a traffic car or not (player car).
         * @param _startCodeAi is the index of the AI to be assigned to the traffic car
         */
        TrafficCar(const int _posX, const int _posY, const int _posZ, const float _speed, const std::string _name,
                   const int _id, const float _offset, const bool _active, const bool _side, const bool _isTrafficCar,
                   const int _startCodeAi);



        /**
         * Set the car active (true) or inactive (false) to be drawn or not
         * @param _active means if the car has to be drawn or not (true or false)
         */
        void setActive(const bool _active);



        /**
         * Set the car in the left (false) or right (true) road
         * @param _side represents the road where the car must be played (left or right)
         */
        void setSide(const bool _side);


        /**
         * Set the distance between the center of the road and the position of the car in axis X
         * @param _offset is distance between the center of the road and the position of the car in axis X
         */
        void setOffset(const float _offset);



        /**
         * Set if the player car is near or not to the traffic car
         * @param _playerClosed shows if the player car is near or not to the traffic car
         */
        void setPlayerClosed(const bool _playerClosed);



        /**
         * Set the type of AI of the traffic car to compute its movements
         * @param startCodeAi is the index of the AI to be assigned to the traffic car
         */
        void setAi(const int startCodeAi);



        /**
         * Get if the car is active or not
         */
        bool getActive() const;



        /**
         * Get if the car is on the left road (false) or right road (true)
         */
        bool getSide() const;



        /**
         * Get the distance between the center of the road and the position of the car in axis X
         */
        float getOffset() const;



        /**
         * Get if the player car is near or not to the traffic car
         */
        bool getPlayerClosed() const;



        /**
         * Get the identifier of the car
         */
        int getId() const;



        /**
         * Get the scale factor to draw the sprites of the car
         */
        float getScale() const;



        /**
         * Control the movement that the traffic car has to do depedin on the player car positions and also on its type of AI
         * @param p is the player car
         * @param playerLine is the line in the biome where the player car is located
         * @param trafficCarLine is the line in the biome where the traffic car is located
         */
        void controlAiTrack(const PlayerCar& p, const Line* playerLine, const Line* trafficCarLine);



        /**
         * Set the index of the sprite that has to be drawn
         * @param pauseMode controls if the game is in pause or not
         * @param gameStatus is the status of the game
         */
        void draw(const bool pauseMode, const State gameStatus);








};


#endif // TRAFFIC_CAR_H
