/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
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
 * Module Vehicle interface file
 */

#pragma once

#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "../../Globals.h"


enum class Direction : int {
    TURNLEFT,
    FRONT,
    TURNRIGHT
};

// Actions that can be made with the vehicle
enum class Action : int {
    NONE,
    BRAKE,
    ACCELERATE,
    CRASH,
    BOOT
};

enum class Elevation : int {
    UP,
    FLAT,
    DOWN
};

/**
 * The vehicle information is composed of its speed, acceleration, x-position, set of textures of the
 * vehicle, current sprite and current action and direction.
 */
class Vehicle {

    protected:

        float posX, scale;
        int posY, posZ;
        Direction direction;
        Elevation elevation;
        int current_code_image;
        int counter_code_image;
        int maxCounterToChange;
        float speed;
        bool isTrafficCar;

        std::vector<sf::Texture> textures;

    public:

        Vehicle();

        Vehicle(const int _posX, const int _posY, const int _posZ, const float _speed,
                const float _scale, const std::string& name, const bool _isTrafficCar);

        void setSpeed(const float& sp);

        void setDirection(const Direction& _direction);

        float getSpeed() const;

        void setScale(const float _scale);

        void setPosX(const float& pX);

        void setPosY(const int& pY);

        void setPosZ(const int& pZ);

        float getPosX() const;

        int getPosY() const;

        int getPosZ() const;

        int getMaxCounterToChange() const;

        int getCounter_code_image() const;

        sf::Texture getTexture() const;

        void elevationControl(const int& yWorld1, const int& yWorld2);

};


#endif // VEHICLE_H
