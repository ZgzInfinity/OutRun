
/*
 * Copyright (c) 2023 Ruben Rodriguez
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
 * Implementation file of the module Vehicle
 */

#include "Vehicle.h"
#include <iostream>



/**
 * Default constructor
 */
Vehicle::Vehicle(){}



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
Vehicle::Vehicle(const int _posX, const int _posY, const int _posZ, const float _speed,
                 const float _scale, const std::string& _name, const bool _isTrafficCar)
{

    // Set all the main information of the car
    posX = _posX;
	posY = _posY;
	posZ = _posZ;
	speed = _speed;
	scale = _scale;
	isTrafficCar = _isTrafficCar;
    current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;

	// Check if it is a traffic car
	if (!isTrafficCar){
        // Load the textures of the player car
        for (int i = 1; i <= PLAYER_CAR_TEXTURES; i++) {
            sf::Texture t;
            t.loadFromFile("Resources/Vehicles/PlayerCar/" + _name + "/c" + std::to_string(i) + ".png");
            t.setRepeated(false);
            textures.push_back(t);
        }

        // Load the textures to draw the wheel effects (smoke, grass, snow, mud or sand)
        for (int i = 1; i <= PLAYER_WHEELS_TEXTURES; i++) {
            sf::Texture t;
            t.loadFromFile("Resources/Vehicles/PlayerCar/Wheels/c" + std::to_string(i) + ".png");
            t.setRepeated(false);
            textures.push_back(t);
        }
	}
	else {
        // Load the sprites of the traffic car
        for (int i = 1; i <= TRAFFIC_TEXTURES; i++) {
            sf::Texture t;
            t.loadFromFile("Resources/Vehicles/" + _name + "/c" + std::to_string(i) + ".png");
            t.setRepeated(false);
            textures.push_back(t);
        }
	}
}



/**
 * Set the position of the car in axis X
 * @param _pX is the position in axis X
 */
void Vehicle::setPosX(const float _pX){
    posX = _pX;
}



/**
 * Set the position of the car in axis Y
 * @param _pY is the position in axis Y
 */
void Vehicle::setPosY(const int _pY){
    posY = _pY;
}



/**
 * Set the position of the car in axis Z
 * @param _pZ is the position in axis Z
 */
void Vehicle::setPosZ(const int _pZ){
    posZ = _pZ;
}



/**
 * Set the speed of the car
 * @param _speed is the speed of the car
 */
void Vehicle::setSpeed(const float _speed){
    speed = _speed;
}



/**
 * Set the scale factor to draw the sprites of the car
 * @param _scale is the scale factor (minimum value 0)
 */
void Vehicle::setScale(const float _scale){
    scale = _scale;
}



/**
 * Set the direction status of the car
 * @param _direction is the status direction of the car
 */
void Vehicle::setDirection(const Direction _direction){
    direction = _direction;
}



/**
 * Get the speed of the car
 */
float Vehicle::getSpeed() const {
    return speed;
}



/**
 * Get the position of the car in axis X
 */
float Vehicle::getPosX() const {
    return posX;
}



/**
 * Get the position of the car in axis Y
 */
int Vehicle::getPosY() const {
    return posY;
}



/**
 * Get the position of the car in axis Z
 */
int Vehicle::getPosZ() const {
    return posZ;
}



/**
 * Get the index of the current car sprite i to be drawn
 */
int Vehicle::getCounter_code_image() const {
    return counter_code_image;
}



/**
 * Get the time maximum to change the sprite of the car to be drawn
 */
int Vehicle::getMaxCounterToChange() const {
    return maxCounterToChange;
}



/**
 * Get the texture image of the current car sprite to be drawn
 */
sf::Texture Vehicle::getTexture() const {
    return textures[current_code_image - 1];
}



/**
 * Compute the new elevation status of the car using the slope of the terrain
 * @param ySlopeFirst is the slope of the previous point position of the car in axis Y
 * @param ySlopeSecond is the slope of the current point position of the car in axis Y
 */
void Vehicle::elevationControl(const int ySlopeFirst, const int ySlopeSecond){
    if (ySlopeFirst == ySlopeSecond)
        // Same slopes
        elevation = Elevation::FLAT;
    else if (ySlopeFirst < ySlopeSecond)
        // Higher the second (up)
        elevation = Elevation::UP;
    else
        // Lower the second (down)
        elevation = Elevation::DOWN;
}
