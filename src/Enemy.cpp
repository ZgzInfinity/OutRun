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


#include <cmath>
#include "../include/Enemy.hpp"
#include "../include/Random.hpp"

using namespace std;
using namespace sf;

#define ENEMY_TEXTURES 4
#define MAX_AUTO_DIRECTION_COUNTER 1000

Enemy::Enemy(float maxSpeed, float speedMul, float scale, int maxCounterToChange, const string &vehicle, float pY, int idCar) :
        Vehicle((maxSpeed - 30) / speedMul, scale, maxCounterToChange, 0, random_float(-0.5f, 0.5f),
                pY, pY, 0, 0, vehicle, ENEMY_TEXTURES, 1, 0), oriX(this->posX),
        currentDirection(RIGHT), calculatedPath(RIGHT), current_direction_counter(0), max_direction_counter(0),
        probAI(0), typeAI(OBSTACLE), id(idCar) {}




/**
 * Updates the vehicle logic automatically for the current movement
 *
 * The AI will only be activated if the distance between the vehicle and the player is less than or equal to the distance
 * of rendering (rectangles that the player sees).
 *
 * AI aggressiveness is within the class (probAI). It is a value between 0 and 1 that indicates the probability of
 * that the AI act in this movement.
 *
 * If the AI aggressiveness is 0, the movement will be as in the original game, i.e. the car will continue
 * at a constant speed without going off the road and following a straight path or with a turn (chosen
 * randomly) and without being influenced by the player.
 *
 * If the aggressiveness of the AI is 1, the movement will be controlled by the AI and its movement will depend on the type
 * of AI:
 * OBSTACLE: Attempts to collide with the player by getting in his path and trying to catch him.
 * EVASIVE: Flees to the lane furthest from the player to try to avoid him.
 * INCONSTANT: Changes lanes too often regardless of the player's position.
 *
 * If the aggressiveness of the AI is between 0 and 1, it performs one of the two actions described (original
 * or AI) with probability p that the AI will act and p' = (1 - p) that it will be as in the original.
 *
 * @param c is the module configuration of the game
 * @param playerPosX is the position of the traffic car in the axis x
 * @param playerPosY is the position of the traffic car in the axis y
 */
void Enemy::autoControl(const Config &c, float playerPosX, float playerPosY, bool inFork, float curveCoeff, float yOffsetX, int limitMap) {
    // Check if the vehicle is in fork
    if (inFork){
        if (posX == 0.f){
            const int p = random_int(0, 1);
            if (p == 0){
                posX = yOffsetX;
            }
            else {
                posX = -yOffsetX;
            }
        }
        else if (posX > 0.f){
            // Right fork
            if (abs(posX) < 11.71f){
                posX = yOffsetX;
            }
            else if (abs(posX) >= 11.71f){
                posX = 11.71f;
            }
            directionFork = 1;
        }
        else {
            // Left Fork
            if (abs(posX) < 11.71f){
                posX = -yOffsetX;
            }
            else if (abs(posX) >= 11.71f){
                posX = -11.71f;
            }
            directionFork = -1;
        }
    }
    else if (limitMap - posY <= 420.f){
        // Check if the traffic car is in the right of the road
        if (posX > 0.f){
            posX -= 0.02f;
            if (posX < 0.0f){
                // Centered in the traffic car of the road
                posX = 0.f;
            }
        }
        // Check if the traffic car is in the left of the road
        else if (posX < 0.0f) {
            posX += 0.02f;
            if (posX > 0.0f){
                // Centered in the traffic car of the road
                posX = 0.f;
            }
        }
    }
    // Normal path
    else if (abs(playerPosY - posY) > float(c.renderLen) || random_zero_one() >= probAI) {
        // Original
        if (current_direction_counter < max_direction_counter) {
            // Increment texture
            current_direction_counter++;
        }
        else {
            // Change the texture and gets a new random direction
            max_direction_counter = random_zero_n(MAX_AUTO_DIRECTION_COUNTER);
            current_direction_counter = 0;
            calculatedPath = randomDirection();
        }

        // Compute the coordinate in axis x depending on the direction selected
        float newX = posX;
        if (calculatedPath == TURNRIGHT)
            newX += XINC * random_zero_one() * speed / maxSpeed;
        else if (calculatedPath == TURNLEFT)
            newX -= XINC * random_zero_one() * speed / maxSpeed;

        if (newX < oriX - 0.15f || newX > oriX + 0.15f)
            calculatedPath = RIGHT;
        else
            posX = newX;

        // Stores the new direction
        currentDirection = calculatedPath;
    }
    else {
        // AI
        if (typeAI == OBSTACLE) {
            const float acc = getAcceleration();
            if (abs(playerPosX - posX) > XINC) { // Rotation control
                // The vehicle is not in the player's path
                if (speed < halfMaxSpeed)
                    speed = sqrt(acc + ACC_INC_ENEMY);

                if (posX > playerPosX && posX > -0.9f) {
                    posX -= XINC * speed / maxSpeed;
                    currentDirection = TURNLEFT;
                } else if (posX < 0.9f) {
                    posX += XINC * speed / maxSpeed;
                    currentDirection = TURNRIGHT;
                }
            } else {
                // Acceleration control
                // The vehicle is in the player's path
                if (posY <= playerPosY)
                    speed = sqrt(acc + ACC_INC_ENEMY);
                else if (acc > ACC_INC_ENEMY)
                    speed = sqrt(acc - ACC_INC_ENEMY);
                else
                    speed = 0.0f;

                currentDirection = RIGHT;
            }
        } else if (typeAI == EVASIVE) {
            if (playerPosX <= -0.5) {
                posX += XINC * speed / maxSpeed;
                currentDirection = TURNRIGHT;
            } else if (playerPosX >= 0.5) {
                posX -= XINC * speed / maxSpeed;
                currentDirection = TURNLEFT;
            } else {
                if (posX > playerPosX) {
                    posX += XINC * speed / maxSpeed;
                    currentDirection = TURNRIGHT;
                } else {
                    posX -= XINC * speed / maxSpeed;
                    currentDirection = TURNLEFT;
                }
            }
        }
        else {
             // INCONSTANT
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
    }

    // Control the limits of speed of the traffic car
    if (speed > maxSpeed)
        speed = maxSpeed;
    else if (speed < 0)
        speed = 0;

    // Control that the enemy is not out of the road in the forks
    if (!inFork){
        if (posX > 0.7f) {
            posX = 0.7f;
            currentDirection = RIGHT;
        }
        else if (posX < -0.7f) {
            posX = -0.7f;
            currentDirection = RIGHT;
        }
    }

    // Update the position of the traffic car in the landscape
    previousY = posY;
    posY += speed;
}



/**
 * Initialize the vehicle status. Updates the aggressiveness of the vehicle's AI with a random value between 0
 * and maxAggressiveness.
 * @param iniPos is the initial position of the traffic car
 * @param endPos is the new position of the traffic car
 * @param maxAggressiveness is the AI aggressiveness of the traffic cars
 */
void Enemy::update(float iniPos, float endPos, float maxAggressiveness) {

    // Updating the speed of the traffic car between two thresholds
    speed = maxSpeed * random_float(0.45f, 0.65f);

    // Estimates the position of the traffic car and stores the current one
    posY = random_float(iniPos, endPos);
    previousY = posY;

    // Initialize the path counter
    current_direction_counter = 0;
    max_direction_counter = 0;
    minScreenX = 0;
    maxScreenX = 0;

    // Set the type of AI
    setAI(maxAggressiveness);
}



/**
 * Updates the aggressiveness of the vehicle AI with a random value between 0 and maxAggressiveness
 * @param maxAggressiveness is the AI aggressiveness of the traffic cars
 * @param difficulty is the difficulty level of the game selected by the player
 * @param typeOfGame is the game selected by the player
 */
void Enemy::setAI(float maxAggressiveness) {
    // Determine the aggressiveness of the AI
    if (maxAggressiveness == 0.0f)
        probAI = 0.0f;
    else
        probAI = random_float(maxAggressiveness / 2.0f, maxAggressiveness);

    // Get a random number to select the type of AI for the traffic car
    const float p = random_zero_one();

    // Check the difficulty and depending of it a kind of AI is more common than the other one
    if (p < 0.333f) {
        typeAI = OBSTACLE;
    }
    else if (p < 0.666f) {
        typeAI = EVASIVE;
    }
    else {
        typeAI = INCONSTANT;
        probAI *= 2.0f;
    }
}



/**
 * Update the sprite of the enemy vehicle.
 * @param e is the current elevation of the terrain where is the camera
 * @param camX is the position of the camera in the axis x
 */
void Enemy::draw(const Elevation &e, const float camX) {
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
                    } else {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                } else if (currentDirection == TURNLEFT) {
                    if (current_code_image < 3 || current_code_image > 4)
                        current_code_image = 3;
                } else { // Turn right
                    if (current_code_image < 1 || current_code_image > 2)
                        current_code_image = 1;
                }
            } else if (e == UP) {
                if (currentDirection == RIGHT) {
                    if (posX <= camX) {
                        if (current_code_image < 1 || current_code_image > 2)
                            current_code_image = 1;
                    } else {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                } else if (currentDirection == TURNLEFT) {
                    if (current_code_image < 3 || current_code_image > 4)
                        current_code_image = 3;
                } else { // Turn right
                    if (current_code_image < 1 || current_code_image > 2)
                        current_code_image = 1;
                }
            } else { // Down
                if (currentDirection == RIGHT) {
                    if (posX <= camX) {
                        if (current_code_image < 1 || current_code_image > 2)
                            current_code_image = 1;
                    } else {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                } else if (currentDirection == TURNLEFT) {
                    if (current_code_image < 3 || current_code_image > 4)
                        current_code_image = 3;
                } else { // Turn right
                    if (current_code_image < 1 || current_code_image > 2)
                        current_code_image = 1;
                }
            }
        }
    } else {
        counter_code_image++;
    }
}



/**
 * Sets the minimum X coordinate that the vehicle occupies.
 * @param screenX
 */
void Enemy::setMinScreenX(float screenX) {
    minScreenX = screenX;
}



/**
 * Sets the maximum X coordinate that the vehicle occupies.
 * @param screenX
 */
void Enemy::setMaxScreenX(float screenX) {
    maxScreenX = screenX;
}



/**
 * Returns the current texture of the vehicle
 * @return
 */
const Texture *Enemy::getCurrentTexture() const {
    return &textures[current_code_image - 1];
}



/**
 * Returns the current scale of the vehicle.
 * @return
 */
float Enemy::getScale() const {
    return scale;
}



/**
 * Returns true if there has been a collision between the traffic vehicle and the player's vehicle.
 * If true, it also returns the Y position where they have collided
 * @param currentY is the current position of the player's vehicle in the axis y
 * @param prevY is the last position of the player's vehicle in the axis y
 * @param minX is the minimum position in axis x occupied by the vehicle
 * @param maxX is the maximum position in axis y occupied by the vehicle
 * @param crashPos is the position in axis y where the crash has happened
 * @return
 */
bool Enemy::hasCrashed(float prevY, float currentY, float minX, float maxX, float &crashPos) const {
    // Check if the path of the traffic car is approximately the same path of the player's vehicle
    if (minScreenX != maxScreenX && ((prevY <= posY + 5.f && currentY >= posY - 5.f) ||
                                     (currentY <= posY + 5.f && prevY >= posY - 5.f)) && // y matches
        ((minX >= minScreenX && minX <= maxScreenX) ||
         (maxX >= minScreenX && maxX <= maxScreenX) ||
         (minScreenX >= minX && minScreenX <= maxX) ||
         (maxScreenX >= minX && maxScreenX <= maxX))) { // x matches
        // There is a crash between both cars
        crashPos = posY;
        return true;
    }
    // There is no crash
    return false;
}


/**
 * Returns true if the car is displayed on screen and the distance to the player, otherwise returns false.
 * @param c is the module configuration of the game
 * @param minY is the position of the camera in the axis y
 * @param playerX is the player position in the axis x
 * @param playerY is the player position in the axis y
 * @param distanceX is the distance between the traffic car and the vehicle of the player in the axis x
 * @param distanceY is the distance between the traffic car and the vehicle of the player in the axis y
 * @return
 */
bool Enemy::isVisible(const Config &c, float minY, float playerX, float playerY, float &distanceX, float &distanceY) const {
    // Check if the traffic car is visible from the position of the player's vehicle
    if (posY < minY || posY > minY + float(c.renderLen) || minScreenX < 0 || maxScreenX > c.w.getSize().y) {
        // The traffic car is not visible
        return false;
    }
    else {
        // The traffic car is visible and calculate the distance in both axis with it
        distanceX = abs(playerX - posX);
        distanceY = abs(playerY - posY);
        return true;
    }
}



/**
 * Returns the numeric identifier of the car
 */
int Enemy::getId() const {
    return id;
}
