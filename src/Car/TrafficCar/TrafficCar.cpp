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

#include "TrafficCar.h"


TrafficCar::TrafficCar() : Vehicle(){}


TrafficCar::TrafficCar(const int _posX, const int _posY, const int _posZ, const float _speed, const std::string& name,
                       const int& _id, const float& _offset, const bool& _active, const bool& _side, const bool _isTrafficCar)
                       : Vehicle( _posX, _posY, _posZ, _speed, 1.f, name, _isTrafficCar)
{
    id = _id;
    offset = _offset;
    active = _active;
    side = _side;

    readProperties(name);
}

void TrafficCar::readProperties(const std::string& name){
    ifstream fin("Resources/Vehicles/" + name + "/Car" + std::to_string(id) + ".txt");
    if (fin.is_open()) {
        while (!fin.eof()) {
            string s;
            fin >> s;
            if (s == "SCALE:" && !fin.eof())
                fin >> scale;
            else if (!s.empty())
                cerr << "WARNING: '" << s << "' at file "
                     << "Resources/Vehicles/" << name << "/Car" << std::to_string(id) << ".txt" << endl;
        }
        fin.close();
    }
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

float TrafficCar::getOffset() const {
    return offset;
}

int TrafficCar::getId() const {
    return id;
}

/**
 * Update the sprite of the enemy vehicle.
 * @param e is the current elevation of the terrain where is the camera
 * @param camX is the position of the camera in the axis x
 */
void TrafficCar::draw(const bool pauseMode) {

    if (counter_code_image >= maxCounterToChange) {
        counter_code_image = 0;

        if (speed > 0.f && !pauseMode)
            current_code_image++;

        if (textures.size() == TRAFFIC_TEXTURES) {
            if (elevation == Elevation::FLAT){
                if (playerClosed){
                    if (direction == Direction::TURNRIGHT) {
                        if (current_code_image < 1 || current_code_image > 2)
                            current_code_image = 1;
                    }
                    else if (direction == Direction::TURNLEFT) {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                }
                else {
                    if (direction == Direction::TURNRIGHT) {
                        if (current_code_image < 13 || current_code_image > 14)
                            current_code_image = 13;
                    }
                    else if (direction == Direction::TURNLEFT) {
                        if (current_code_image < 15 || current_code_image > 16)
                            current_code_image = 15;
                    }
                }
            }
            else if (elevation == Elevation::UP){
                if (direction == Direction::TURNRIGHT) {
                    if (current_code_image < 5 || current_code_image > 6)
                        current_code_image = 5;
                }
                else if (direction == Direction::TURNLEFT) {
                    if (current_code_image < 7 || current_code_image > 8)
                        current_code_image = 7;
                }
            }
            else if (elevation == Elevation::DOWN){
                if (direction == Direction::TURNRIGHT) {
                    if (current_code_image < 9 || current_code_image > 10)
                        current_code_image = 9;
                }
                else if (direction == Direction::TURNLEFT) {
                    if (current_code_image < 11 || current_code_image > 12)
                        current_code_image = 11;
                }
            }
        }
    }
    else
        counter_code_image++;
}


void TrafficCar::setPlayerClosed(const bool _playerClosed){
    playerClosed = _playerClosed;
}

bool TrafficCar::getPlayerClosed() const {
    return playerClosed;
}

float TrafficCar::getScale() const {
    return scale;
}

