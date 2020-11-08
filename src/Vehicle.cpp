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

#include "../include/Vehicle.hpp"

using namespace std;
using namespace sf;

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
        t.setSmooth(true);
        t.setRepeated(false);
        textures.push_back(t);
    }
}

void Vehicle::setPosition(float pX, float pY) {
    posX = pX;
    posY = pY;
    previousY = pY;
}

float Vehicle::getPosX() const {
    return posX;
}

float Vehicle::getPosY() const {
    return posY;
}

float Vehicle::getMinScreenX() const {
    return minScreenX;
}

float Vehicle::getMaxScreenX() const {
    return maxScreenX;
}

float Vehicle::getAcceleration() const {
    return speed * speed;
}
