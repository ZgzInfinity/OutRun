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

#define PLAYER_TEXTURES 132
#define ENEMY_TEXTURES 16

Vehicle::Vehicle(float maxSpeed, float speedMul, float accInc, float scale, bool isPlayer, int maxCounterToChange,
        const string &vehicle, float pX, float pY) : speedMul(speedMul), maxSpeed(maxSpeed / speedMul),
                                                     halfMaxSpeed(this->maxSpeed / 2.0f), maxAcc(pow(maxSpeed / speedMul, 2.0f)), accInc(accInc), scale(scale),
                                                     maxCounterToChange(maxCounterToChange), speed(0), acceleration(0), posX(pX), posY(pY), previousY(pY),
                                                     minScreenX(0), maxScreenX(0), current_code_image(1), counter_code_image(0), crashing(false) {
    int numTextures = PLAYER_TEXTURES;
    if (!isPlayer)
        numTextures = ENEMY_TEXTURES;
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
    previousY = pY;
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

void Vehicle::autoControl(float playerX, float playerY, Vehicle::Action &a, Vehicle::Direction &d) {
    posX = playerX;
    posY = playerY + 150.0f;
}

void Vehicle::drawPlayer(Config &c, const Action &a, const Direction &d, const Elevation &e) {
    if (a != NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            if (speed > 0.0f)
                current_code_image++;

            if (textures.size() == PLAYER_TEXTURES) {
                if (a == ACCELERATE) {
                    if (e == FLAT) {
                        if (d == RIGHT) {
                            if (current_code_image < 1 || current_code_image > 4)
                                current_code_image = 1;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 5 || current_code_image > 8)
                                    current_code_image = 5;
                            }
                            else {
                                if (current_code_image < 9 || current_code_image > 12)
                                    current_code_image = 9;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 13 || current_code_image > 16)
                                    current_code_image = 13;
                            }
                            else {
                                if (current_code_image < 17 || current_code_image > 20)
                                    current_code_image = 17;
                            }
                        }
                    }
                    else if (e == UP) {
                        if (d == RIGHT) {
                            if (current_code_image < 21 || current_code_image > 24)
                                current_code_image = 21;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 25 || current_code_image > 28)
                                    current_code_image = 25;
                            }
                            else {
                                if (current_code_image < 29 || current_code_image > 32)
                                    current_code_image = 29;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 33 || current_code_image > 36)
                                    current_code_image = 33;
                            }
                            else {
                                if (current_code_image < 37 || current_code_image > 40)
                                    current_code_image = 37;
                            }
                        }
                    }
                    else { // Down
                        if (d == RIGHT) {
                            if (current_code_image < 41 || current_code_image > 44)
                                current_code_image = 41;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 45 || current_code_image > 48)
                                    current_code_image = 45;
                            }
                            else {
                                if (current_code_image < 49 || current_code_image > 52)
                                    current_code_image = 49;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 53 || current_code_image > 56)
                                    current_code_image = 53;
                            }
                            else {
                                if (current_code_image < 57 || current_code_image > 60)
                                    current_code_image = 57;
                            }
                        }
                    }
                }
                else if (a == BRAKE) {
                    if (e == FLAT) {
                        if (d == RIGHT) {
                            if (current_code_image < 61 || current_code_image > 64)
                                current_code_image = 61;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 65 || current_code_image > 68)
                                    current_code_image = 65;
                            }
                            else {
                                if (current_code_image < 69 || current_code_image > 72)
                                    current_code_image = 69;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 73 || current_code_image > 76)
                                    current_code_image = 73;
                            }
                            else {
                                if (current_code_image < 77 || current_code_image > 80)
                                    current_code_image = 77;
                            }
                        }
                    }
                    else if (e == UP) {
                        if (d == RIGHT) {
                            if (current_code_image < 81 || current_code_image > 84)
                                current_code_image = 81;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 85 || current_code_image > 88)
                                    current_code_image = 85;
                            }
                            else {
                                if (current_code_image < 89 || current_code_image > 92)
                                    current_code_image = 89;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 93 || current_code_image > 96)
                                    current_code_image = 93;
                            }
                            else {
                                if (current_code_image < 97 || current_code_image > 100)
                                    current_code_image = 97;
                            }
                        }
                    }
                    else { // Down
                        if (d == RIGHT) {
                            if (current_code_image < 101 || current_code_image > 104)
                                current_code_image = 101;
                        }
                        else if (d == TURNLEFT) {
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 105 || current_code_image > 108)
                                    current_code_image = 105;
                            }
                            else {
                                if (current_code_image < 109 || current_code_image > 112)
                                    current_code_image = 109;
                            }
                        }
                        else { // Turn right
                            if (speed < halfMaxSpeed) {
                                if (current_code_image < 113 || current_code_image > 116)
                                    current_code_image = 113;
                            }
                            else {
                                if (current_code_image < 117 || current_code_image > 120)
                                    current_code_image = 117;
                            }
                        }
                    }
                }
                else {
                    // Crash
                    if (posX > 0.0f) {
                        if (posX > XINC) {
                            if (current_code_image < 121 || current_code_image > 124)
                                current_code_image = 121;
                        }
                        else {
                            if (current_code_image < 129 || current_code_image > 130)
                                current_code_image = 129;
                        }
                    }
                    else {
                        if (posX < -XINC) {
                            if (current_code_image < 125 || current_code_image > 128)
                                current_code_image = 125;
                        }
                        else {
                            if (current_code_image < 131 || current_code_image > 132)
                                current_code_image = 131;
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
        current_code_image = 1;
    }

    sprite.setTexture(textures[current_code_image - 1], true);
    sprite.setScale(scale, scale);
    minScreenX = ((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    maxScreenX = minScreenX + sprite.getGlobalBounds().width;
    sprite.setPosition(minScreenX, ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f);
    c.w.draw(sprite);
}

void Vehicle::drawEnemy(Config &c, const Action &a, const Direction &d, const Elevation &e, const float camX) {
    if (a != NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            if (speed > 0.0f)
                current_code_image++;

            if (textures.size() == ENEMY_TEXTURES) {
                if (e == FLAT) {
                    if (d == RIGHT) {
                        if (posX <= camX) {
                            if (current_code_image < 1 || current_code_image > 2)
                                current_code_image = 1;
                        }
                        else {
                            if (current_code_image < 3 || current_code_image > 4)
                                current_code_image = 3;
                        }
                    }
                    else if (d == TURNLEFT) {
                        if (current_code_image < 7 || current_code_image > 8)
                            current_code_image = 7;
                    }
                    else { // Turn right
                        if (current_code_image < 5 || current_code_image > 6)
                            current_code_image = 5;
                    }
                }
                else if (e == UP) {
                    if (d == RIGHT) {
                        if (posX <= camX) {
                            if (current_code_image < 13 || current_code_image > 14)
                                current_code_image = 13;
                        }
                        else {
                            if (current_code_image < 15 || current_code_image > 16)
                                current_code_image = 15;
                        }
                    }
                    else if (d == TURNLEFT) {
                        if (current_code_image < 15 || current_code_image > 16)
                            current_code_image = 15;
                    }
                    else { // Turn right
                        if (current_code_image < 13 || current_code_image > 14)
                            current_code_image = 13;
                    }
                }
                else { // Down
                    if (d == RIGHT) {
                        if (posX <= camX) {
                            if (current_code_image < 9 || current_code_image > 10)
                                current_code_image = 9;
                        }
                        else {
                            if (current_code_image < 11 || current_code_image > 12)
                                current_code_image = 11;
                        }
                    }
                    else if (d == TURNLEFT) {
                        if (current_code_image < 11 || current_code_image > 12)
                            current_code_image = 11;
                    }
                    else { // Turn right
                        if (current_code_image < 9 || current_code_image > 10)
                            current_code_image = 9;
                    }
                }
            }
        }
        else {
            counter_code_image++;
        }
    }
    else {
        if (posX <= camX)
            current_code_image = 1;
        else
            current_code_image = 3;
    }
}

void Vehicle::setMinScreenX(float screenX) {
    minScreenX = screenX;
}

void Vehicle::setMaxScreenX(float screenX) {
    maxScreenX = screenX;
}

float Vehicle::getMinScreenX() const {
    return minScreenX;
}

float Vehicle::getMaxScreenX() const {
    return maxScreenX;
}

const Texture *Vehicle::getCurrentTexture() const {
    return &textures[current_code_image - 1];
}

float Vehicle::getScale() const {
    return scale;
}
