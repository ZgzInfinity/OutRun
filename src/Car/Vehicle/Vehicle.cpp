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


#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle(){}

Vehicle::Vehicle(const int _posX, const int _posY, const int _posZ, const float _speed,
                 const float _scale, const std::string& name, const bool _isTrafficCar)
{
    posX = _posX;
	posY = _posY;
	posZ = _posZ;
	speed = _speed;
	scale = _scale;
	isTrafficCar = _isTrafficCar;
    current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;

	if (!isTrafficCar){
        for (int i = 1; i <= PLAYER_CAR_TEXTURES; i++) {
            sf::Texture t;
            t.loadFromFile("Resources/Vehicles/PlayerCar/" + name + "/c" + std::to_string(i) + ".png");
            t.setRepeated(false);
            textures.push_back(t);
        }

        for (int i = 1; i <= PLAYER_WHEELS_TEXTURES; i++) {
            sf::Texture t;
            t.loadFromFile("Resources/Vehicles/PlayerCar/Wheels/c" + std::to_string(i) + ".png");
            t.setRepeated(false);
            textures.push_back(t);
        }
	}
	else {
        for (int i = 1; i <= TRAFFIC_TEXTURES; i++) {
            sf::Texture t;
            t.loadFromFile("Resources/Vehicles/" + name + "/c" + std::to_string(i) + ".png");
            t.setRepeated(false);
            textures.push_back(t);
        }
	}
}

void Vehicle::setSpeed(const float& sp){
    speed = sp;
}

void Vehicle::setDirection(const Direction& _direction){
    direction = _direction;
}


float Vehicle::getSpeed() const {
    return speed;
}

void Vehicle::setScale(const float _scale){
    scale = _scale;
}

void Vehicle::setPosX(const float& pX){
    posX = pX;
}

void Vehicle::setPosY(const int& pY){
    posY = pY;
}

void Vehicle::setPosZ(const int& pZ){
    posZ = pZ;
}

float Vehicle::getPosX() const {
    return posX;
}

int Vehicle::getPosY() const {
    return posY;
}

int Vehicle::getPosZ() const {
    return posZ;
}

sf::Texture Vehicle::getTexture() const {
    return textures[current_code_image - 1];
}
