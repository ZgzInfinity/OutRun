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
 * Module Player implementation file
 */

#include <cmath>
#include "TrafficCar.h"
#include "../../Random/Random.h"


TrafficCar::TrafficCar() : Vehicle(){}


TrafficCar::TrafficCar(const int _posX, const int _posY, const int _posZ, const float _speed, const int numTextures, const std::string& name,
                       const int& _id, const float& _offset, const bool& _active, const bool& _side, const int& _lane)
                       : Vehicle( _posX, _posY, _posZ, _speed, numTextures, name)
{
    id = _id;
    offset = _offset;
    active = _active;
    side = _side;
    lane = _lane;
}


void TrafficCar::setActive(const bool& _active) {
    active = _active;
}

bool TrafficCar::getActive() const {
    return active;
}

void TrafficCar::setSide(const bool& _side){
    side = _side;
}

bool TrafficCar::getSide() const {
    return side;
}

void TrafficCar::setOffset(const float& _offset){
    offset = _offset;
}

bool TrafficCar::getOffset() const {
    return offset;
}



/**
 * Update the sprite of the enemy vehicle.
 * @param e is the current elevation of the terrain where is the camera
 * @param camX is the position of the camera in the axis x
 */
void TrafficCar::draw() {

    if (counter_code_image >= maxCounterToChange) {
        counter_code_image = 0;

        if (speed > 0.f)
            current_code_image++;

        if (textures.size() == TRAFFIC_TEXTURES) {
            if (direction == Direction::TURNLEFT) {
                if (current_code_image < 1 || current_code_image > 2)
                    current_code_image = 1;
            }
            else if (direction == Direction::TURNRIGHT) {
                if (current_code_image < 3 || current_code_image > 4)
                    current_code_image = 3;
            }
        }
    }
    else {
        counter_code_image++;
    }
}

