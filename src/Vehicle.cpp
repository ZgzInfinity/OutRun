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
 * Module Vehicle implementation file
 */

#include "../include/Vehicle.hpp"

using namespace std;
using namespace sf;



/**
 * Initialize a vehicle with all its properties
 * @param maxSpeed is the maximum speed reached by the vehicle in KM/H
 * @param scale is the scaling factor to draw the vehicle in the screen with different resolutions
 * @param maxCounterToChange is limit to change the direction of the vehicle
 * @param speed is the speed of the vehicle in KM/H
 * @param posX is the position of the vehicle in the axis x
 * @param posY is the position of the vehicle in the axis y
 * @param previousY is the last position of the vehicle in the axis y
 * @param minScreenX is the minimum coefficient to draw the vehicle in the screen
 * @param maxScreenX is the maximum coefficient to draw the vehicle in the screen
 * @param vehicle is the type of vehicle selected by the player in the selection vehicle menu
 * @param numTextures is the number of textures of the vehicle selected
 * @param currentCodeImage is the code of the sprite to be drawn in the screen
 * @param counterCodeImage is a counter that change the sprite to be drawn in the screen
 */
Vehicle::Vehicle(const float maxSpeed, const float scale, const int maxCounterToChange, float speed, float posX,
                 float posY, float previousY, float minScreenX, float maxScreenX, const string &vehicle,
                 int numTextures,
                 int currentCodeImage, int counterCodeImage) : maxSpeed(maxSpeed), scale(scale),
                                                               maxCounterToChange(maxCounterToChange), speed(speed),
                                                               halfMaxSpeed(maxSpeed / 2.0f), posX(posX), posY(posY),
                                                               previousY(previousY), minScreenX(minScreenX),
                                                               maxScreenX(maxScreenX),
                                                               current_code_image(currentCodeImage),
                                                               counter_code_image(counterCodeImage) {
    textures.reserve(static_cast<unsigned long>(numTextures));
    for (int i = 1; i <= numTextures; i++) {
        Texture t;
        t.loadFromFile("Resources/" + vehicle + "/c" + to_string(i) + ".png");
        t.setRepeated(false);
        textures.push_back(t);
    }
}



/**
 * Establish the vehicle in a concrete position in the landscape
 * @param pX is the position in the axis x when the vehicle must be located
 * @param pY is the position in the axis y when the vehicle must be located
 */
void Vehicle::setPosition(float pX, float pY) {
    posX = pX;
    posY = pY;
    previousY = pY;
}



/**
 * Returns the current position of the vehicle in the axis x
 * @return
 */
float Vehicle::getPosX() const {
    return posX;
}



/**
 * Returns the current position of the vehicle in the axis y
 * @return
 */
float Vehicle::getPosY() const {
    return posY;
}



/**
 * Returns the minimum coordinate of the screen in axis x used by the vehicle
 * @return
 */
float Vehicle::getMinScreenX() const {
    return minScreenX;
}



/**
 * Returns the maximum coordinate of the screen in axis x used by the vehicle
 * @return
 */
float Vehicle::getMaxScreenX() const {
    return maxScreenX;
}



/**
 * Returns the acceleration of the vehicle
 * @return
 */
float Vehicle::getAcceleration() const {
    return speed * speed;
}



/**
 * Randomly choose the direction the vehicle must go
 */
Vehicle::Direction randomDirection() {
    // Get a probabilistic number between zero and one
    const float p = random_zero_one();
    // Check the value of the number obtained
    if (p < 0.6f){
        // Straight direction
        return Vehicle::Direction::RIGHT;
    }
    else if (p < 0.8f){
        // Turn right
        return Vehicle::Direction::TURNRIGHT;
    }
    else {
        // Turn left
        return Vehicle::Direction::TURNLEFT;
    }
}
