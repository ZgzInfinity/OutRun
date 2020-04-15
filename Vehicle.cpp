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
        const string &vehicle, float pX, float pY) : speedMul(speedMul), maxSpeed(maxSpeed / speedMul),
        halfMaxSpeed(this->maxSpeed / 2.0f), maxAcc(pow(maxSpeed / speedMul, 2.0f)), accInc(accInc), scale(scale),
        maxCounterToChange(maxCounterToChange), speed(0), acceleration(0), posX(pX), posY(pY), previousY(pY),
        minScreenX(0), maxScreenX(0), actual_code_image(1), counter_code_image(0), crashing(false) {
    textures.reserve(numTextures);
    for (int i = 1; i <= numTextures; i++) {
        Texture t;
        t.loadFromFile("resources/" + vehicle + "/c" + to_string(i) + ".png");
        t.setSmooth(true);
        t.setRepeated(false);
        textures.push_back(t);
    }
}

void Vehicle::setPosition(float pX, float pY) {
    posX = pX;
    posY = pY;
}

float Vehicle::getPosX() const {
    return posX;
}

float Vehicle::getPosY() const {
    return posY;
}

float Vehicle::getPreviousY() const {
    return previousY;
}

void Vehicle::hitControl() {
    crashing = true;

    if (posX > XINC)
        posX -= XINC;
    else if (posX < -XINC)
        posX += XINC;

    acceleration -= accInc * 2.5f;
    if (speed > 1.333f * halfMaxSpeed) // Reduces hit time
        acceleration -= accInc * 5.0f;
    else if (speed > halfMaxSpeed)
        acceleration -= accInc * 2.5f;

    if (acceleration < 0.0f)
        acceleration = 0.0f;

    speed = sqrt(acceleration);
    if (speed <= 0.0f) {
        speed = 0.0f;
        acceleration = 0.0f;
        crashing = false;

        posX = 0.0f;
        previousY = posY;
    }
}

bool Vehicle::isCrashing() const {
    return crashing;
}

float Vehicle::getRealSpeed() const {
    return speed * speedMul;
}

Vehicle::Action Vehicle::accelerationControl(Config &c, bool hasGotOut) {
    Action a = NONE;

    if (Keyboard::isKeyPressed(c.brakeKey))
        a = BRAKE;

    if (a != BRAKE && Keyboard::isKeyPressed(c.accelerateKey)) {
        if (hasGotOut) {
            if (acceleration < maxAcc / 2.0f)
                acceleration += accInc / 2.0f;
            else
                acceleration -= accInc;
        }
        else {
            if (acceleration < maxAcc)
                acceleration += accInc;
        }

        if (acceleration > maxAcc)
            acceleration = maxAcc;
    }
    else {
        float mul = 2.0f;
        if (a == BRAKE)
            mul *= 2.0f;
        if (hasGotOut)
            mul *= 1.5f;

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

Vehicle::Direction Vehicle::rotationControl(Config &c, float curveCoefficient) {
    if (speed > 0.0f) {
        if (curveCoefficient > 0.0f)
            posX -= XINC * speed / (maxSpeed * 2.0f);
        else if (curveCoefficient < 0.0f)
            posX += XINC * speed / (maxSpeed * 2.0f);

        if (Keyboard::isKeyPressed(c.leftKey)) {
            posX -= XINC * speed / maxSpeed;
            return TURNLEFT;
        }
        if (Keyboard::isKeyPressed(c.rightKey)) {
            posX += XINC * speed / maxSpeed;
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

            if (textures.size() == 132) {
                if (a == ACCELERATE) {
                    if (e == Map::FLAT) {
                        if (d == RIGHT) {
                            if (actual_code_image < 1 || actual_code_image > 4)
                                actual_code_image = 1;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 5 || actual_code_image > 8)
                                    actual_code_image = 5;
                            }
                            else {
                                if (actual_code_image < 9 || actual_code_image > 12)
                                    actual_code_image = 9;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 13 || actual_code_image > 16)
                                    actual_code_image = 13;
                            }
                            else {
                                if (actual_code_image < 17 || actual_code_image > 20)
                                    actual_code_image = 17;
                            }
                        }
                    }
                    else if (e == Map::UP) {
                        if (d == RIGHT) {
                            if (actual_code_image < 21 || actual_code_image > 24)
                                actual_code_image = 21;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 25 || actual_code_image > 28)
                                    actual_code_image = 25;
                            }
                            else {
                                if (actual_code_image < 29 || actual_code_image > 32)
                                    actual_code_image = 29;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 33 || actual_code_image > 36)
                                    actual_code_image = 33;
                            }
                            else {
                                if (actual_code_image < 37 || actual_code_image > 40)
                                    actual_code_image = 37;
                            }
                        }
                    }
                    else { // Down
                        if (d == RIGHT) {
                            if (actual_code_image < 41 || actual_code_image > 44)
                                actual_code_image = 41;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 45 || actual_code_image > 48)
                                    actual_code_image = 45;
                            }
                            else {
                                if (actual_code_image < 49 || actual_code_image > 52)
                                    actual_code_image = 49;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 53 || actual_code_image > 56)
                                    actual_code_image = 53;
                            }
                            else {
                                if (actual_code_image < 57 || actual_code_image > 60)
                                    actual_code_image = 57;
                            }
                        }
                    }
                }
                else if (a == BRAKE) {
                    if (e == Map::FLAT) {
                        if (d == RIGHT) {
                            if (actual_code_image < 61 || actual_code_image > 64)
                                actual_code_image = 61;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 65 || actual_code_image > 68)
                                    actual_code_image = 65;
                            }
                            else {
                                if (actual_code_image < 69 || actual_code_image > 72)
                                    actual_code_image = 69;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 73 || actual_code_image > 76)
                                    actual_code_image = 73;
                            }
                            else {
                                if (actual_code_image < 77 || actual_code_image > 80)
                                    actual_code_image = 77;
                            }
                        }
                    }
                    else if (e == Map::UP) {
                        if (d == RIGHT) {
                            if (actual_code_image < 81 || actual_code_image > 84)
                                actual_code_image = 81;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 85 || actual_code_image > 88)
                                    actual_code_image = 85;
                            }
                            else {
                                if (actual_code_image < 89 || actual_code_image > 92)
                                    actual_code_image = 89;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 93 || actual_code_image > 96)
                                    actual_code_image = 93;
                            }
                            else {
                                if (actual_code_image < 97 || actual_code_image > 100)
                                    actual_code_image = 97;
                            }
                        }
                    }
                    else { // Down
                        if (d == RIGHT) {
                            if (actual_code_image < 101 || actual_code_image > 104)
                                actual_code_image = 101;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 105 || actual_code_image > 108)
                                    actual_code_image = 105;
                            }
                            else {
                                if (actual_code_image < 109 || actual_code_image > 112)
                                    actual_code_image = 109;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (actual_code_image < 113 || actual_code_image > 116)
                                    actual_code_image = 113;
                            }
                            else {
                                if (actual_code_image < 117 || actual_code_image > 120)
                                    actual_code_image = 117;
                            }
                        }
                    }
                }
                else {
                    // Crash
                    if (posX > 0.0f) {
                        if (posX > XINC) {
                            if (actual_code_image < 121 || actual_code_image > 124)
                                actual_code_image = 121;
                        }
                        else {
                            if (actual_code_image < 129 || actual_code_image > 130)
                                actual_code_image = 129;
                        }
                    }
                    else {
                        if (posX < -XINC) {
                            if (actual_code_image < 125 || actual_code_image > 128)
                                actual_code_image = 125;
                        }
                        else {
                            if (actual_code_image < 131 || actual_code_image > 132)
                                actual_code_image = 131;
                        }
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

    sprite.setTexture(textures[actual_code_image - 1], true);
    sprite.setScale(scale, scale);
    minScreenX = ((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    maxScreenX = minScreenX + sprite.getGlobalBounds().width;
    sprite.setPosition(minScreenX, ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f);
    c.w.draw(sprite);
}

float Vehicle::getMinScreenX() const {
    return minScreenX;
}

float Vehicle::getMaxScreenX() const {
    return maxScreenX;
}
