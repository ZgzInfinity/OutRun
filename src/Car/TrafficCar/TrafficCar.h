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
 * Module Player interface file
 */

#pragma once

#ifndef TRAFFIC_CAR_H
#define TRAFFIC_CAR_H


#include <vector>
#include <SFML/Graphics.hpp>
#include "../../Globals.h"
#include "../../Input/Input.h"


enum class Traffic_Direction : int {
    TURNLEFT,
    TURNRIGHT,
    __COUNT
};


class TrafficCar {

    public:

        Traffic_Direction direction;
        int current_code_image;
        int counter_code_image;
        int maxCounterToChange;
        int id;
        float offset, speed, zPos, percent;
        bool active, side;
        int lane;

        vector<sf::Texture> textures;

        TrafficCar(const int& _id, const float& _offset, const float& _speed,
                   const float& _zPos, const bool& _active, const bool& _side, const int& _lane);

        void draw();
};


#endif // TRAFFIC_CAR_H
