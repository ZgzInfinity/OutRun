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
 * Interface file of the module Vehicle
 */

#pragma once

#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "../../Globals.h"



/**
 * Represents the different kinds of direction of a vehicle
 */
enum class Direction : int {
    TURNLEFT,
    FRONT,
    TURNRIGHT
};



/**
 * Represents the different kinds of actions (statuses) of a vehicle
 */
enum class Action : int {
    NONE,
    BRAKE,
    ACCELERATE,
    CRASH,
    BOOT
};



/**
 * Represents the different kinds of elevations of a
 * vehicle depending the gradient of the terrain
 */
enum class Elevation : int {
    UP,
    FLAT,
    DOWN
};



/**
 * Represents a generic class that contains all the main information
 * in order to store the status of the player and traffic cars
 */
class Vehicle {

    protected:

        // Position of the car in axis X
        float posX;

        // Scale factor to draw the sprites
        float scale;

        // Position of the car in axises Y and Z
        int posY, posZ;

        // Direction path of the car (pay attention to the enum class)
        Direction direction;

        // Elevation status of the car (pay attention to the enum class)
        Elevation elevation;

        // Index of the sprite texture of the car to be drawn
        int current_code_image;

        // Time control counter that warns when the sprite of the car must be changed (maxCounterToChange is reached)
        int counter_code_image;

        // Time that has to be reached by counter_code_image to change the sprite of the car
        int maxCounterToChange;

        // Speed of the car
        float speed;

        // Control if the car is a traffic car or not (player car is false)
        bool isTrafficCar;

        // Vector with all the textures
        std::vector<sf::Texture> textures;

    public:



        /**
         * Default constructor
         */
        Vehicle();



        /**
         * Second constructor
         * @param _posX is the position of the car in axis X
         * @param _posY is the position of the car in axis Y
         * @param _posZ is the position of the car in axis Z
         * @param _speed is the initial speed of the car
         * @param _scale is the scale factor to draw the sprites of the car
         * @param _name is the path where the images of the car are stored
         * @param _isTrafficCar controls if the car is a traffic car or not (player car).
         */
        Vehicle(const int _posX, const int _posY, const int _posZ, const float _speed,
                const float _scale, const std::string& _name, const bool _isTrafficCar);



        /**
         * Set the position of the car in axis X
         * @param _pX is the position in axis X
         */
        void setPosX(const float _pX);



        /**
         * Set the position of the car in axis Y
         * @param _pY is the position in axis Y
         */
        void setPosY(const int _pY);



        /**
         * Set the position of the car in axis Z
         * @param _pZ is the position in axis Z
         */
        void setPosZ(const int _pZ);



        /**
         * Set the speed of the car
         * @param _speed is the speed of the car
         */
        void setSpeed(const float _speed);



        /**
         * Set the scale factor to draw the sprites of the car
         * @param _scale is the scale factor (minimum value 0)
         */
        void setScale(const float _scale);



        /**
         * Set the direction status of the car
         * @param _direction is the status direction of the car
         */
        void setDirection(const Direction _direction);



        /**
         * Get the speed of the car
         */
        float getSpeed() const;



        /**
         * Get the position of the car in axis X
         */
        float getPosX() const;



        /**
         * Get the position of the car in axis Y
         */
        int getPosY() const;



        /**
         * Get the position of the car in axis Z
         */
        int getPosZ() const;



        /**
         * Get the index of the current car sprite i to be drawn
         */
        int getCounter_code_image() const;



        /**
         * Get the time maximum to change the sprite of the car to be drawn
         */
        int getMaxCounterToChange() const;



        /**
         * Get the texture image of the current car sprite to be drawn
         */
        sf::Texture getTexture() const;



        /**
         * Compute the new elevation status of the car using the slope of the terrain
         * @param ySlopeFirst is the slope of the previous point position of the car in axis Y
         * @param ySlopeSecond is the slope of the current point position of the car in axis Y
         */
        void elevationControl(const int ySlopeFirst, const int ySlopeSecond);

};


#endif // VEHICLE_H
