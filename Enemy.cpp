/******************************************************************************
 * @file    Enemy.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Enemy.hpp"
#include "Random.hpp"
#include <cmath>

using namespace std;
using namespace sf;

#define ENEMY_TEXTURES 16
#define MAX_AUTO_DIRECTION_COUNTER 1000

Enemy::Enemy(float maxSpeed, float speedMul, float scale, int maxCounterToChange, const string &vehicle, float pY) :
        Vehicle(maxSpeed / speedMul, scale, maxCounterToChange, 0, random_float(-0.5f, 0.5f),
                pY, pY, 0, 0, vehicle, ENEMY_TEXTURES, 1, 0), oriX(this->posX),
                currentDirection(RIGHT), calculatedPath(RIGHT), current_direction_counter(0), max_direction_counter(0) {}

void Enemy::autoControl(Config &c, float playerPosX, float playerPosY) {
    const float probAI = float(c.aggressiveness) / 100.0f;

    if (random_zero_one() >= probAI) {
        // Original
        if (current_direction_counter < max_direction_counter) {
            current_direction_counter++;
        }
        else {
            max_direction_counter = random_zero_n(MAX_AUTO_DIRECTION_COUNTER);
            current_direction_counter = 0;
            calculatedPath = randomDirection();
        }

        float newX = posX;
        if (calculatedPath == TURNRIGHT)
            newX += XINC * random_zero_one() * speed / maxSpeed;
        else if (calculatedPath == TURNLEFT)
            newX -= XINC * random_zero_one() * speed / maxSpeed;

        if (newX < oriX - 0.15f || newX > oriX + 0.15f)
            calculatedPath = RIGHT;
        else
            posX = newX;

        currentDirection = calculatedPath;
    }
    else {
        // AI
        const float acc = getAcceleration();
        if (abs(playerPosX - posX) > XINC) { // Rotation control
            // The vehicle is not in the player's path
            if (speed < halfMaxSpeed)
                speed = sqrt(acc + ACC_INC);

            if (posX > playerPosX && posX > -0.9f) {
                posX -= XINC * speed / maxSpeed;
                currentDirection = TURNLEFT;
            }
            else if (posX < 0.9f) {
                posX += XINC * speed / maxSpeed;
                currentDirection = TURNRIGHT;
            }
        }
        else { // Acceleration control
            // The vehicle is in the player's path
            if (posY <= playerPosY)
                speed = sqrt(acc + ACC_INC);
            else if (acc > ACC_INC)
                speed = sqrt(acc - ACC_INC);
            else
                speed = 0.0f;

            currentDirection = RIGHT;
        }
    }

    if (speed > maxSpeed)
        speed = maxSpeed;
    else if (speed < 0)
        speed = 0;

    previousY = posY;
    posY += speed;
}

void Enemy::update(float iniPos, float endPos) {
    speed = maxSpeed * random_float(0.25f, 1.0f);

    posY = random_float(iniPos, endPos);
    previousY = posY;

    current_direction_counter = 0;
    max_direction_counter = 0;

    minScreenX = 0;
    maxScreenX = 0;
}

void Enemy::draw(Config &c, const Elevation &e, const float camX) {
    if (counter_code_image >= maxCounterToChange) {
        counter_code_image = 0;

        if (speed > 0.0f)
            current_code_image++;

        if (textures.size() == ENEMY_TEXTURES) {
            if (e == FLAT) {
                if (currentDirection == RIGHT) {
                    if (posX <= camX) {
                        if (current_code_image < 1 || current_code_image > 2)
                            current_code_image = 1;
                    }
                    else {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                }
                else if (currentDirection == TURNLEFT) {
                    if (current_code_image < 7 || current_code_image > 8)
                        current_code_image = 7;
                }
                else { // Turn right
                    if (current_code_image < 5 || current_code_image > 6)
                        current_code_image = 5;
                }
            }
            else if (e == UP) {
                if (currentDirection == RIGHT) {
                    if (posX <= camX) {
                        if (current_code_image < 13 || current_code_image > 14)
                            current_code_image = 13;
                    }
                    else {
                        if (current_code_image < 15 || current_code_image > 16)
                            current_code_image = 15;
                    }
                }
                else if (currentDirection == TURNLEFT) {
                    if (current_code_image < 15 || current_code_image > 16)
                        current_code_image = 15;
                }
                else { // Turn right
                    if (current_code_image < 13 || current_code_image > 14)
                        current_code_image = 13;
                }
            }
            else { // Down
                if (currentDirection == RIGHT) {
                    if (posX <= camX) {
                        if (current_code_image < 9 || current_code_image > 10)
                            current_code_image = 9;
                    }
                    else {
                        if (current_code_image < 11 || current_code_image > 12)
                            current_code_image = 11;
                    }
                }
                else if (currentDirection == TURNLEFT) {
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

void Enemy::setMinScreenX(float screenX) {
    minScreenX = screenX;
}

void Enemy::setMaxScreenX(float screenX) {
    maxScreenX = screenX;
}

const Texture *Enemy::getCurrentTexture() const {
    return &textures[current_code_image - 1];
}

float Enemy::getScale() const {
    return scale;
}

bool Enemy::hasCrashed(const Config &c, float prevY, float currentY, float minX, float maxX, float &crashPos) const {
    if (minScreenX != maxScreenX && ((prevY <= posY + 2.5f && currentY >= posY - 2.5f) ||
        (currentY <= posY + 2.5f && prevY >= posY - 2.5f)) && // y matches
        ((minX >= minScreenX && minX <= maxScreenX) ||
         (maxX >= minScreenX && maxX <= maxScreenX) ||
         (minScreenX >= minX && minScreenX <= maxX) ||
         (maxScreenX >= minX && maxScreenX <= maxX))) { // x matches
        crashPos = posY;
        return true;
    }
    return false;
}

bool Enemy::isVisible(const Config &c, float minY, float playerX, float playerY, float &distanceX, float &distanceY) const {
    if (posY < minY || posY > minY + float(c.renderLen) || minScreenX < 0 || maxScreenX > c.w.getSize().y) {
        return false;
    }
    else {
        distanceX = abs(playerX - posX);
        distanceY = abs(playerY - posY);
        return true;
    }
}
