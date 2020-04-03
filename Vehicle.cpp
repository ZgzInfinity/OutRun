/******************************************************************************
 * @file    Vehicle.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Vehicle.hpp"
#include <cmath>

using namespace std;
using namespace sf;

Vehicle::Vehicle(float maxSpeed, float speedMul, float accInc, float scale, int numTextures, int maxCounterToChange,
        const string &vehicle) : speedMul(speedMul), maxAcc(pow(maxSpeed / speedMul, 2.0f)), accInc(accInc),
        scale(scale), maxCounterToChange(maxCounterToChange), speed(0), acceleration(0), posX(0), posY(0), previousY(0),
        minScreenX(0), maxScreenX(0), actual_code_image(1), counter_code_image(0) {
    for (int i = 1; i <= numTextures; i++) {
        Texture t;
        t.loadFromFile("resources/" + vehicle + "/c" + to_string(i) + ".png");
        t.setSmooth(true);
        t.setRepeated(false);
        textures.push_back(t);
    }
}

void Vehicle::resetPosition(float pX, float pY) {
    posX = pX;
    posY = pY;
    speed = 0.0f;
    acceleration = 0.0f;
}

std::pair<float, float> Vehicle::getPosition() const {
    return make_pair(posX, posY);
}

float Vehicle::getRealSpeed() const {
    return speed * speedMul;
}

Vehicle::Action Vehicle::accelerationControl(Config &c) {
    Action a = NONE;

    if (Keyboard::isKeyPressed(c.brakeKey))
        a = BRAKE;

    if (a != BRAKE && Keyboard::isKeyPressed(c.accelerateKey)) {
        if (acceleration < maxAcc)
            acceleration += accInc;

        if (acceleration > maxAcc)
            acceleration = maxAcc;
    }
    else {
        float mul = 2.0f;
        if (a == BRAKE)
            mul = 4.0f;

        if (acceleration > 0.0f)
            acceleration -= accInc * mul;

        if (acceleration < 0.0f)
            acceleration = 0.0f;
    }

    if (a == NONE && acceleration > 0.0f)
        a = ACCELERATE;

    speed = sqrt(acceleration);
    if (speed > 0.0f) {
        previousY = posY;
        posY += speed;
    }

    return a;
}

Vehicle::Direction Vehicle::rotationControl(Config &c) {
    if (speed > 0.0f) {
        if (Keyboard::isKeyPressed(c.leftKey)) {
            posX -= 0.05f;
            return TURNLEFT;
        }

        if (Keyboard::isKeyPressed(c.rightKey)) {
            posX += 0.05f;
            return TURNRIGHT;
        }
    }

    return RIGHT;
}

void Vehicle::draw(Config &c, const Action &a, const Direction &d, const Map::Elevation &e) {
    if (a != NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            if (speed > 0.0f)
                actual_code_image++;

            if (a == ACCELERATE) {
                if (e == Map::UP) {
                    if (d == TURNLEFT) {
                        if (actual_code_image < 26 || actual_code_image > 27)
                            actual_code_image = 26;
                    }
                    else if (d == TURNRIGHT) {
                        if (actual_code_image < 34 || actual_code_image > 35)
                            actual_code_image = 34;
                    }
                    else {
                        if (actual_code_image < 21 || actual_code_image > 24)
                            actual_code_image = 21;
                    }
                }
                else if (e == Map::DOWN) {
                    if (d == TURNLEFT) {
                        if (actual_code_image < 46 || actual_code_image > 47)
                            actual_code_image = 46;
                    }
                    else if (d == TURNRIGHT) {
                        if (actual_code_image < 54 || actual_code_image > 55)
                            actual_code_image = 54;
                    }
                    else {
                        if (actual_code_image < 41 || actual_code_image > 44)
                            actual_code_image = 41;
                    }
                }
                else {
                    if (d == TURNLEFT) {
                        if (actual_code_image < 6 || actual_code_image > 7)
                            actual_code_image = 6;
                    }
                    else if (d == TURNRIGHT) {
                        if (actual_code_image < 14 || actual_code_image > 15)
                            actual_code_image = 14;
                    }
                    else {
                        if (actual_code_image < 1 || actual_code_image > 4)
                            actual_code_image = 1;
                    }
                }
            }
            else if (a == BRAKE) {
                if (e == Map::UP) {
                    if (d == TURNLEFT) {
                        if (actual_code_image < 86 || actual_code_image > 87)
                            actual_code_image = 86;
                    }
                    else if (d == TURNRIGHT) {
                        if (actual_code_image < 94 || actual_code_image > 95)
                            actual_code_image = 94;
                    }
                    else {
                        if (actual_code_image < 81 || actual_code_image > 84)
                            actual_code_image = 81;
                    }
                }
                else if (e == Map::DOWN) {
                    if (d == TURNLEFT) {
                        if (actual_code_image < 106 || actual_code_image > 107)
                            actual_code_image = 106;
                    }
                    else if (d == TURNRIGHT) {
                        if (actual_code_image < 114 || actual_code_image > 115)
                            actual_code_image = 114;
                    }
                    else {
                        if (actual_code_image < 101 || actual_code_image > 104)
                            actual_code_image = 101;
                    }
                }
                else {
                    if (d == TURNLEFT) {
                        if (actual_code_image < 66 || actual_code_image > 67)
                            actual_code_image = 66;
                    }
                    else if (d == TURNRIGHT) {
                        if (actual_code_image < 74 || actual_code_image > 75)
                            actual_code_image = 74;
                    }
                    else {
                        if (actual_code_image < 61 || actual_code_image > 64)
                            actual_code_image = 61;
                    }
                }
            }
        }
        else {
            counter_code_image++;
        }
    }
    else {
        actual_code_image = 1;
    }

    sprite.setTexture(textures[actual_code_image - 1]);
    sprite.setScale(scale, scale);
    minScreenX = ((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    maxScreenX = ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f;
    sprite.setPosition(minScreenX, maxScreenX);
    c.w.draw(sprite);
}

float Vehicle::getPosY() const {
    return posY;
}

float Vehicle::getPreviousY() const {
    return previousY;
}

float Vehicle::getMinScreenX() const {
    return minScreenX;
}

float Vehicle::getMaxScreenX() const {
    return maxScreenX;
}
