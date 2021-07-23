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
#include "../Vehicle/Vehicle.h"

class TrafficCar : public Vehicle {

    private:

        int id;
        float offset, percent;
        bool active, side;
        int lane;

    public:

        TrafficCar();

        TrafficCar(const int _posX, const int _posY, const int _posZ, const float _speed, const int numTextures, const std::string& name,
                   const int& _id, const float& _offset, const bool& _active, const bool& _side, const int& _lane);

        void setActive(const bool& _active);

        bool getActive() const;

        void setSide(const bool& _side);

        bool getSide() const;

        void setOffset(const float& _offset);

        bool getOffset() const;

        void draw();
};


#endif // TRAFFIC_CAR_H