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


/**
 * Initialize the player's vehicle
 * Default constructor
 */
TrafficCar::TrafficCar(const int& _id, const float& _offset, const float& _speed, const float& _zPos, const bool& _active,
                       const bool& _side, const int& _lane)
{
    textures.reserve(static_cast<unsigned long>(TRAFFIC_TEXTURES));
    const std::string path = "Resources/Vehicles/TrafficCars/Car" + std::to_string(_id);
    for (int i = 1; i <= TRAFFIC_TEXTURES; i++) {
        sf::Texture t;
        t.loadFromFile(path + "/c" + std::to_string(i) + ".png");
        t.setRepeated(false);
        textures.push_back(t);
    }

    id = _id;
    offset = _offset;
    speed = _speed;
    zPos = _zPos;
    active = _active;
    side = _side;
    lane = _lane;
    current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;
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

            if (direction == Traffic_Direction::TURNLEFT) {
                if (current_code_image < 1 || current_code_image > 2)
                    current_code_image = 1;
            }
            else {
                if (current_code_image < 3 || current_code_image > 4)
                    current_code_image = 3;
            }
        }
    }
    else {
        counter_code_image++;
    }
}

