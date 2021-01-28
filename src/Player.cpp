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

#include <cmath>
#include "../include/Player.hpp"
#include "../include/Random.hpp"

using namespace std;
using namespace sf;

#define PLAYER_TEXTURES 170
#define MAX_INERTIA 10



 /**
 * Initialize the player's vehicle
 * @param maxSpeed is the maximum speed that the player's vehicle can reach
 * @param speedMul is factor number that when it is multiplied by speed obtains the real speed
 * @param accInc is the acceleration increment
 * @param scaleX is the scaling factor in the axis x
 * @param scaleY is the scaling factor in the axis y
 * @param maxCounterToChange lets to update the sprite of the player's vehicle that is drawn in the screen
 * @param vehicle is the type of vehicle selected by the player
 * @param pX is the position of the player in the axis x
 * @param pY is the position of the player in the axis y
 */
Player::Player(float maxSpeed, float speedMul, float accInc, float scaleX, float scaleY, int maxCounterToChange,
               const string &vehicle, float pX, float pY) : Vehicle(maxSpeed / speedMul, scaleX, maxCounterToChange,
                                                                    0.0f, pX, pY, pY, 0, 0,
                                                                    vehicle, PLAYER_TEXTURES, 1, 0), speedMul(speedMul),
                                                            maxAcc(pow(maxSpeed / speedMul, 2.0f)), accInc(accInc),
                                                            scaleY(scaleY), acceleration(0),
                                                            minCrashAcc(0), xDest(1000.f), inertia(0), crashing(false),
                                                            smoking(false), skidding(false),
                                                            firstCrash(true), firstTurnLeft(true),
                                                            firstTurnRight(true)
{
    mode = -1;
    speedCollision = 0.f;
    outSideRoad = false;
    numAngers = 0;
    gearPlayer = 0;
    increaseGear = false;
    decreaseGear = false;
}



/**
 * Set the direction of the player after the fork
 * @return
 */
void Player::setDirectionFork() {
    directionFork = 0;
}



/**
 * Returns the last position of the player's vehicle in axis y
 * @return
 */
float Player::getPreviousY() const {
    return previousY;
}



/**
 * Updates the crash logic of the player's vehicle and restores speed and acceleration
 * @param vehicleCrash true if it is a crash between vehicles
 */
void Player::hitControl(const bool vehicleCrash) {
    // By default the devastator is crashing
    crashing = true;

    // By default the devastator is not smoking
    smoking = false;

    // By default the devastator is not skidding
    skidding = false;

    // Check the speed when starts the crash if has not been stored
    if (speedCollision == 0.f){
        // Store the speed
        speedCollision = getRealSpeed();
    }

    // Calculation of the position in axis x where finish the crash
    if (xDest == 1000.f){
        // Check the side of the track where the crash happens
        if (posX > 0.f){
            // The devastator goes to the left
            xDest = -(acceleration * 1.7f / maxAcc);

        }
        else {
            // The devastator goes to the right
            xDest = acceleration * 1.7f / maxAcc;
        }
    }

    // Check the destination coordinate in axis x of the crash
    if (xDest > 0.f){
        // Increment to the right depending if the speed of the devastator
        if (speedCollision <= 40.f){
            posX = posX + (acceleration * XINC / maxAcc) * 40.f;
        }
        else if (speedCollision > 40.f && speedCollision < 85.f){
            posX = posX + (acceleration * XINC / maxAcc) * 20.f;
        }
        else if (speedCollision >= 85.f && speedCollision < 100.f){
            posX = posX + (acceleration * XINC / maxAcc) * 10.f;
        }
        else if (speedCollision >= 100.f && speedCollision < 120.f){
            posX = posX + (acceleration * XINC / maxAcc) * 5.f;
        }
        else {
            posX = posX + (acceleration * XINC / maxAcc) * 1.5f;
        }
        // Avoid to be trapped inside the obstacles of the left
        if (posX > 1.2f){
            posX = 1.2f;
        }
    }
    else {
        // Increment to the left depending if the speed of the devastator
        if (speedCollision <= 40.f){
            posX = posX - (acceleration * XINC / maxAcc) * 40.f;
        }
        else if (speedCollision > 40.f && speedCollision <= 85.f){
            posX = posX - (acceleration * XINC / maxAcc) * 20.f;
        }
        else if (speedCollision > 85.f && speedCollision < 100.f){
            posX = posX - (acceleration * XINC / maxAcc) * 10.f;
        }
        else if (speedCollision >= 100.f && speedCollision < 120.f){
            posX = posX - (acceleration * XINC / maxAcc) * 5.f;
        }
        else {
            posX = posX - (acceleration * XINC / maxAcc) * 1.5f;
        }
        // Avoid to be trapped inside the obstacles of the right
        if (posX < -1.2f){
            posX = -1.2f;
        }
    }

    // Check if the crash is against another car
    if (vehicleCrash){
        if (minCrashAcc <= 0.f) {
            // Only first time reduces the speed and the acceleration
            minCrashAcc = (speed * 0.1555f) * (speed * 0.1555f);
            acceleration = (speed * 0.75f) * (speed * 0.75f);
        }
    }

    // Decrement the acceleration
    acceleration -= accInc * 2.5f;

    // Reduce the acceleration in minor quantities each time
    if (speed > 1.333f * halfMaxSpeed){
        acceleration -= accInc * 7.5f;
    }
    else if (speed > halfMaxSpeed){
        acceleration -= accInc * 5.0f;
    }
    else if (speed > 0.5f * halfMaxSpeed){
        acceleration -= accInc * 2.5f;
    }

    // Avoid negative acceleration
    if (acceleration < 0.0f){
        acceleration = 0.0f;
    }

    // Calculate the speed of the devastator using its acceleration
    mainMutex.lock();
    speed = sqrt(acceleration);
    mainMutex.unlock();

    // Update the new gear of the car
    if (this->getRealSpeed() <= 0.f){
        if (gearPlayer != 0){
            gearPlayer = 0;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }
    else if (this->getRealSpeed() >= 220.f){
        if (gearPlayer != 6){
            gearPlayer = 6;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }
    else if (this->getRealSpeed() >= 170.f){
        if (gearPlayer != 5){
            gearPlayer = 5;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }
    else if (this->getRealSpeed() >= 130.f){
        if (gearPlayer != 4){
            gearPlayer = 4;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }
    else if (this->getRealSpeed() >= 80.f){
        if (gearPlayer != 3){
            gearPlayer = 3;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }
    else if (this->getRealSpeed() >= 40.f){
        if (gearPlayer != 2){
            gearPlayer = 2;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }
    else if (this->getRealSpeed() >= 20.f){
        if (gearPlayer != 1){
            gearPlayer = 1;
            decreaseGear = true;
        }
        else {
            decreaseGear = false;
        }
    }

    // Control the movement in the axis y
    if (xDest > 0.f && posX < xDest){
        // Check the crash has been with an element of the map
        // or with another car
        if (!vehicleCrash){
            // Crash with element of map and devastator moves forward
            posY = posY + acceleration * 7.f / maxAcc;
        }
        else {
            // Crash with element of map and devastator moves backwards
            posY = posY - acceleration * 7.f / maxAcc;
        }
    }
    else if (xDest < 0.f && posX > xDest) {
        // Check the crash has been with an element of the map
        // or with another car
        if (!vehicleCrash){
            // Crash with element of map and devastator moves forward
            posY = posY + acceleration * 7.f / maxAcc;
        }
        else {
            // Crash with element of map and devastator moves backwards
            posY = posY - acceleration * 7.f / maxAcc;
        }
    }

    // Control if the crash animation has been finished
    if (acceleration <= minCrashAcc && numAngers == 3){
        speed = sqrt(acceleration);
        crashing = false;
        minCrashAcc = 0.0f;
        acceleration = minCrashAcc;
        xDest = 1000;
        inertia = 0;
        previousY = posY;
        mode = -1;
        speedCollision = 0.0f;
        outSideRoad = false;
        numAngers = 0;
    }
}



/**
 * Returns true if the player's vehicle is crashing. Otherwise returns false
 * @return
 */
bool Player::isCrashing() const {
    return crashing;
}



/**
 * Returns the real speed of the player's vehicle
 * @return
 */
float Player::getRealSpeed() const {
    return speed * speedMul;
}



/**
 * Updates the logic of the player's vehicle acceleration and braking
 * @param c is the module configuration of the game
 * @param hasGotOut indicates if it's gone off track
 * @return
 */
Vehicle::Action Player::accelerationControl(Config &c, bool hasGotOut) {

    // Default action
    Action a = NONE;
    smoking = false;

    // Store the current acceleration
    float previousAcc = acceleration;

    // Check if the braking control key has been pressed
    if (c.window.hasFocus() && Keyboard::isKeyPressed(c.brakeKey)){
        // Player's car brakes
        a = BRAKE;
        increaseGear = false;


        // Update the new gear of the car
        if (this->getRealSpeed() <= 0.f){
            if (gearPlayer != 0){
                gearPlayer = 0;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 20.f){
            if (gearPlayer != 1){
                gearPlayer = 1;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 90.f){
            if (gearPlayer != 2){
                gearPlayer = 2;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 130.f){
            if (gearPlayer != 3){
                gearPlayer = 3;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 180.f){
            if (gearPlayer != 4){
                gearPlayer = 4;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 220.f){
            if (gearPlayer != 5){
                gearPlayer = 5;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
    }

    // Check if the accelerating key has been pressed
    if (a != BRAKE && c.window.hasFocus() && Keyboard::isKeyPressed(c.accelerateKey)) {

        decreaseGear = false;

        // Check if the player's car is outside the road
        if (hasGotOut) {
            outSideRoad = true;
            // The acceleration increases slower
            if (acceleration < maxAcc / 4.5f)
                acceleration += accInc / 3.0f;
            else
                acceleration -= accInc * 1.5f;
        }
        else {
            // The acceleration increases quicker
            outSideRoad = false;
            if (acceleration < maxAcc)
                acceleration += accInc;
        }

        // Control the limit of the acceleration
        if (acceleration > maxAcc)
            acceleration = maxAcc;

        // Check if the player's vehicle must start to smoke
        smoking = acceleration < maxAcc * 0.05f;


        if (this->getRealSpeed() >= 220.f){
            if (gearPlayer != 6){
                gearPlayer = 6;
                increaseGear = true;
            }
            else if (this->getRealSpeed() >= 230.f) {
                increaseGear = false;
            }
        }
        else if (this->getRealSpeed() >= 180.f){
            if (gearPlayer != 5){
                gearPlayer = 5;
                increaseGear = true;
            }
            else if (this->getRealSpeed() >= 190.f) {
                increaseGear = false;
            }
        }
        else if (this->getRealSpeed() >= 130.f){
            if (gearPlayer != 4){
                gearPlayer = 4;
                increaseGear = true;
            }
            else if (this->getRealSpeed() >= 145.f) {
                increaseGear = false;
            }
        }
        else if (this->getRealSpeed() >= 90.f){
            if (gearPlayer != 3){
                gearPlayer = 3;
                increaseGear = true;
            }
            else if (this->getRealSpeed() >= 110.f) {
                increaseGear = false;
            }
        }
        else if (this->getRealSpeed() >= 20.f){
            if (gearPlayer != 2){
                gearPlayer = 2;
                increaseGear = true;
            }
            else if (this->getRealSpeed() >= 50.f) {
                increaseGear = false;
            }
        }
        else if (this->getRealSpeed() >= 0.f){
            if (gearPlayer != 1){
                gearPlayer = 1;
            }
        }
    }
    else {
        // The player's car is braking
        float mul = 2.0f;
        if (a == BRAKE)
            mul *= 2.0f;
        if (hasGotOut)
            mul *= 1.5f;

        // Reduces acceleration
        if (acceleration > 0.0f)
            acceleration -= accInc * mul;

        if (acceleration < 0.0f)
            acceleration = 0.0f;

        increaseGear = false;
        decreaseGear = false;

        // Update the new gear of the car
        if (this->getRealSpeed() <= 0.f){
            if (gearPlayer != 0){
                gearPlayer = 0;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 20.f){
            if (gearPlayer != 1){
                gearPlayer = 1;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 90.f){
            if (gearPlayer != 2){
                gearPlayer = 2;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 130.f){
            if (gearPlayer != 3){
                gearPlayer = 3;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 180.f){
            if (gearPlayer != 4){
                gearPlayer = 4;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
        else if (this->getRealSpeed() <= 220.f){
            if (gearPlayer != 5){
                gearPlayer = 5;
                decreaseGear = true;
            }
            else {
                decreaseGear = false;
            }
        }
    }

    // Control if the player's car is going to boot the motor
    if (previousAcc == 0.0f && acceleration > 0.0f)
        a = BOOT;
    else if (a == NONE && acceleration > 0.0f)
        // The player's car accelerates because the rest of actions has not happened
        a = ACCELERATE;

    // Calculate the new speed of the player's car
    mainMutex.lock();
    speed = sqrt(acceleration);
    mainMutex.unlock();

    // Control the advance of the player's vehicle in the landscape
    if (speed > 0.0f) {
        // Store the last position in axis y
        previousY = posY;
        // Store the new position using the current speed
        posY += speed;
    }

    return a;
}



/**
 * Updates the logic direction turn of the player's vehicle
 * @param c is the module configuration of the game
 * @param curveCoefficient is the coefficient curve
 * @return
 */
Vehicle::Direction Player::rotationControl(Config &c, float curveCoefficient, bool inFork) {

    // The player's car is not skidding by default
    skidding = false;

    if (speed > 0.0f) {
        // Decrement the position in axis x using the maximum speed and the acceleration
        if (speed < 0.66f * maxSpeed)
            posX -= XINC * curveCoefficient * sqrt(speed / 2.0f) * speed / maxSpeed;
        else
            posX -= XINC * curveCoefficient * sqrt(speed) * speed / maxSpeed;

        // Check if the player's car has to start to skid
        if (abs(curveCoefficient) >= 0.33f && speed >= 0.66f * maxSpeed)
            skidding = true;

        // Control if the turning left control key has been pressed
        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.leftKey)) {

            // Measure the effect of the inertia force
            if (inertia > -MAX_INERTIA)
                inertia--;

            if (inertia < 0) {
                if (curveCoefficient > 0.0f)
                    skidding = false;

                // Control the power of turning in the fork
                if (!inFork){
                    if (speed < halfMaxSpeed){
                        posX -= 1.5f * XINC * speed / maxSpeed;
                    }
                    else if (curveCoefficient == 0.0f){
                        posX -= 1.25f * XINC * speed / maxSpeed;
                    }
                    else {
                        posX -= XINC * speed / maxSpeed;
                    }
                }
                // Control the position in axis x
                else {
                    if (speed < halfMaxSpeed){
                        posX -= 2.f * (XINC * 1.65f) * speed / maxSpeed;
                    }
                    else if (curveCoefficient == 0.0f){
                        posX -= 1.5f * (XINC * 1.65f) * speed / maxSpeed;
                    }
                    else {
                        posX -= 1.25f * (XINC * 1.65f) * speed / maxSpeed;
                    }
                    directionFork = -1;
                }
            }
            return TURNLEFT;
        }
        // Control if the turning right control key has been pressed
        else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.rightKey)) {

            // Measure the effect of the inertia force
            if (inertia < MAX_INERTIA)
                inertia++;

            // The inertia force makes the player's car skid
            if (inertia > 0) {
                if (curveCoefficient < 0.0f)
                    skidding = false;

                if (!inFork){
                    // Control the position in axis x
                    if (speed < halfMaxSpeed){
                        posX += 1.5f * XINC * speed / maxSpeed;
                    }
                    else if (curveCoefficient == 0.0f){
                        posX += 1.25f * XINC * speed / maxSpeed;
                    }
                    else {
                        posX += XINC * speed / maxSpeed;
                    }
                }
                else {
                    if (speed < halfMaxSpeed){
                        posX += 2.f * (XINC * 1.65f) * speed / maxSpeed;
                    }
                    else if (curveCoefficient == 0.0f){
                        posX += 1.5f * (XINC * 1.65f) * speed / maxSpeed;
                    }
                    else {
                        posX += 1.25f * (XINC * 1.65f) * speed / maxSpeed;
                    }
                    directionFork = 1;
                }
                return TURNRIGHT;
            }
        }
        else if (inertia > 0) {
            inertia--;
        }
        else if (inertia < 0) {
            inertia++;
        }
        skidding = false;
    }
    // Player's vehicle goes right
    return RIGHT;
}



/**
 * Updates the player's vehicle sprite and draws it in the screen
 * @param c is the module configuration of the game
 * @param r is the sound player module of the game
 * @param a is the action to be done by the player's vehicle
 * @param d is the direction to be followed by the player's vehicle
 * @param e is the current elevation of the player's vehicle in the landscape
 * @param enableSound indicates if the motor of the player's vehicle has to make noise
 */
void Player::draw(Config &c, const Action &a, const Direction &d, const Elevation &e, int terrain, bool enableSound) {
    // Sound effects
    if (a != CRASH)
        firstCrash = true;

    // Control the sounds of the devastator
    if (enableSound) {
        if (speed > 0.0f) {
            if (a == BOOT) {
                // Acceleration sound
                c.effects[12]->stop();
                c.effects[12]->play();
            }
            if (c.effects[6]->getStatus() != SoundSource::Playing) {
                // Engine sound
                c.effects[6]->stop();
                c.effects[6]->play();
            }
            c.effects[6]->setVolume((33.0f + 67.0f * speed / maxSpeed) * float(c.volumeEffects) / 100.f);
            if (skidding && c.effects[8]->getStatus() != SoundSource::Playing) {
                // Skidding sound
                c.effects[8]->stop();
                c.effects[8]->play();
            }
            if (outSideRoad && c.effects[30]->getStatus() != SoundSource::Playing) {
                // Outside sound
                c.effects[30]->stop();
                c.effects[30]->play();
            }
            else if (!outSideRoad && c.effects[30]->getStatus() == SoundSource::Playing){
                // Outside sound
                c.effects[30]->stop();
            }
        }
        else {
            c.effects[12]->stop();
            c.effects[6]->stop();
            c.effects[8]->stop();
            c.effects[30]->stop();
            c.effects[43]->stop();
            c.effects[44]->stop();
        }

        if (a == CRASH && firstCrash) {
            firstCrash = false;
            c.effects[17]->stop();
            c.effects[18]->stop();
            c.effects[19]->stop();
            c.effects[17]->play();
            c.effects[random_int(18, 19)]->play();
            if (c.effects[31]->getStatus() != SoundSource::Playing) {
                // Engine sound
                c.effects[31]->stop();
                c.effects[31]->play();
            }
        }

        // Increasing gear sound
        if (increaseGear && c.effects[43]->getStatus() != SoundSource::Playing){
            c.effects[43]->stop();
            c.effects[43]->play();
        }
        // Decreasing gear sound
        else if (decreaseGear && c.effects[44]->getStatus() != SoundSource::Playing){
            c.effects[44]->stop();
            c.effects[44]->play();
        }

    }
    else {
        c.effects[12]->stop();
        c.effects[6]->stop();
        c.effects[8]->stop();
        c.effects[30]->stop();
        c.effects[17]->stop();
        c.effects[18]->stop();
        c.effects[19]->stop();
    }

    // Draw

    if (d != TURNLEFT){
        firstTurnLeft = true;
    }
    if (d != TURNRIGHT){
        firstTurnRight = true;
    }

    // Check the current action of the devastator to be drawn in the screen
    if (a != NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            // Increment the texture counter only if it moves
            if (speed > 0.0f || (speed <= 0.0f && numAngers < 3))
                current_code_image++;

            if (textures.size() == PLAYER_TEXTURES) {
                if (a == ACCELERATE || a == BOOT) {
                    if (e == FLAT) {
                        if (d == RIGHT) {
                            if (current_code_image < 1 || current_code_image > 4)
                                current_code_image = 1;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 5 || current_code_image > 12)
                                    current_code_image = 5;
                                if (current_code_image == 9)
                                    firstTurnLeft = false;
                            } else {
                                if (current_code_image < 9 || current_code_image > 12)
                                    current_code_image = 9;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 13 || current_code_image > 20)
                                    current_code_image = 13;
                                if (current_code_image == 17)
                                    firstTurnRight = false;
                            } else {
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
                            if (firstTurnLeft) {
                                if (current_code_image < 25 || current_code_image > 32)
                                    current_code_image = 25;
                                if (current_code_image == 29)
                                    firstTurnLeft = false;
                            } else {
                                if (current_code_image < 29 || current_code_image > 32)
                                    current_code_image = 29;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 33 || current_code_image > 40)
                                    current_code_image = 33;
                                if (current_code_image == 37)
                                    firstTurnRight = false;
                            } else {
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
                            if (firstTurnLeft) {
                                if (current_code_image < 45 || current_code_image > 52)
                                    current_code_image = 45;
                                if (current_code_image == 49)
                                    firstTurnLeft = false;
                            } else {
                                if (current_code_image < 49 || current_code_image > 52)
                                    current_code_image = 49;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 53 || current_code_image > 60)
                                    current_code_image = 53;
                                if (current_code_image == 57)
                                    firstTurnRight = false;
                            } else {
                                if (current_code_image < 57 || current_code_image > 60)
                                    current_code_image = 57;
                            }
                        }
                    }
                } else if (a == BRAKE) {
                    if (e == FLAT) {
                        if (d == RIGHT) {
                            if (current_code_image < 61 || current_code_image > 64)
                                current_code_image = 61;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 65 || current_code_image > 72)
                                    current_code_image = 65;
                                if (current_code_image == 69)
                                    firstTurnLeft = false;
                            } else {
                                if (current_code_image < 69 || current_code_image > 72)
                                    current_code_image = 69;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 73 || current_code_image > 80)
                                    current_code_image = 73;
                                if (current_code_image == 77)
                                    firstTurnRight = false;
                            } else {
                                if (current_code_image < 77 || current_code_image > 80)
                                    current_code_image = 77;
                            }
                        }
                    } else if (e == UP) {
                        if (d == RIGHT) {
                            if (current_code_image < 81 || current_code_image > 84)
                                current_code_image = 81;
                        } else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 85 || current_code_image > 92)
                                    current_code_image = 85;
                                if (current_code_image == 89)
                                    firstTurnLeft = false;
                            } else {
                                if (current_code_image < 89 || current_code_image > 92)
                                    current_code_image = 89;
                            }
                        } else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 93 || current_code_image > 100)
                                    current_code_image = 93;
                                if (current_code_image == 97)
                                    firstTurnRight = false;
                            } else {
                                if (current_code_image < 97 || current_code_image > 100)
                                    current_code_image = 97;
                            }
                        }
                    } else { // Down
                        if (d == RIGHT) {
                            if (current_code_image < 101 || current_code_image > 104)
                                current_code_image = 104;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 105 || current_code_image > 112)
                                    current_code_image = 105;
                                if (current_code_image == 109)
                                    firstTurnLeft = false;
                            } else {
                                if (current_code_image < 109 || current_code_image > 112)
                                    current_code_image = 109;
                            }
                        }
                        else {
                            if (firstTurnRight) {
                                if (current_code_image < 113 || current_code_image > 120)
                                    current_code_image = 113;
                                if (current_code_image == 117)
                                    firstTurnRight = false;
                            } else {
                                if (current_code_image < 117 || current_code_image > 120)
                                    current_code_image = 117;
                            }
                        }
                    }
                }
                else {
                    // The devastator is crashing
                    if (mode == 0) {
                        // First type of animation
                        if (speed <= 0.f){
                            if (numAngers < 3){
                                if (current_code_image < 129 || current_code_image > 133){
                                    current_code_image = 129;
                                }
                                else {
                                    if (current_code_image == 133){
                                        numAngers++;
                                    }
                                 }
                            }
                        }
                        else {
                            if (current_code_image < 121 || current_code_image > 128)
                                current_code_image = 121;
                        }
                    }
                    else if (mode == 1) {
                        // Second type of animation
                        if (speed <= 0.f){
                            if (numAngers < 3){
                                 if (current_code_image < 142 || current_code_image > 146){
                                    current_code_image = 142;
                                 }
                                 else {
                                    if (current_code_image == 146){
                                        numAngers++;
                                    }
                                 }
                            }
                        }
                        else {
                            if (current_code_image < 134 || current_code_image > 141)
                                current_code_image = 134;
                        }
                    }
                }
            }
        }
        else {
            // Increment the code of the ferrari texture to be drawn
            counter_code_image++;
        }
    }
    else {
        // Default code when the ferrari does not move
        current_code_image = 1;
    }

    // Draw the ferrari in the screen adapted to the current screen resolution and pixel art effect
    sprite.setTexture(textures[current_code_image - 1], true);
    sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
    minScreenX = ((float) c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    maxScreenX = minScreenX + sprite.getGlobalBounds().width;
    if (c.isDefaultScreen){
        if (c.enablePixelArt){
            sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 10.f);
        }
        else {
            sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 20.f);
        }
    }
    else {
        if (c.enablePixelArt){
            sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 5.f);
        }
        else {
            sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 10.f);
        }
    }
    c.w.draw(sprite);

    // Check if the ferrari is crashing or skidding or smoking
    if (smoking || skidding || crashing || increaseGear) {
        if (!crashing){
            // Accelerate the counter of changing the sprite
            maxCounterToChange = COUNTER + 1;
        }
        else {
            // Give it a value depending of the speed of the ferrari
            if (getRealSpeed() < 20.f){
                maxCounterToChange = COUNTER + 4;
            }
            else if (getRealSpeed() >= 20.f && getRealSpeed() < 60.f){
                maxCounterToChange = COUNTER + 3;
            }
            else if (getRealSpeed() >= 60.f && getRealSpeed() < 100.f){
                maxCounterToChange = COUNTER + 2;
            }
            else if (getRealSpeed() >= 100.f && getRealSpeed() < 120.f){
                maxCounterToChange = COUNTER + 1;
            }
            else if (getRealSpeed() >= 120.f){
                maxCounterToChange = COUNTER;
            }
        }
        // Draw the smoke effect of the ferrari
        if (speed > 0.f){
            const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;
            sprite.setTexture(textures[146 + current_code_image % 4], true);
            sprite.setScale(3.f * c.screenScale, 3.5f * c.screenScale);
            sprite.setPosition(((float) c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width,
                               j - sprite.getGlobalBounds().height);
            c.w.draw(sprite);
            sprite.setPosition(((float) c.w.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height);
            c.w.draw(sprite);
        }
    }
    // Check if the ferrari is outside the road and it is moving
    else if (outSideRoad && speed != 0.f){
        // Must raise land
        maxCounterToChange = COUNTER;
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

        // Check the kind of terrain of the landscape and draw it
        switch(terrain){
        case 0:
            // Grass
            sprite.setTexture(textures[156 + current_code_image % 8], true);
            break;
        case 1:
            // Land
            sprite.setTexture(textures[150 + current_code_image % 6], true);
            break;
        case 2:
            // Snow
            sprite.setTexture(textures[164 + current_code_image % 6], true);
        }
        sprite.setScale(3.f * c.screenScale, 3.5f * c.screenScale);
        sprite.setPosition(((float) c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width,
                           j - sprite.getGlobalBounds().height);
        c.w.draw(sprite);
        sprite.setPosition(((float) c.w.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height);
        c.w.draw(sprite);
    }
    else if (!crashing) {
        // Make the devastator change the sprite quickly
        maxCounterToChange = COUNTER;
    }
}



/**
 * Draw the animation of the player's vehicle at the departure
 * @param c is the module configuration of the game
 */
void Player::drawStaticAnimation(Config &c) {
    if (textures.size() == PLAYER_TEXTURES) {
        float x = (float) c.w.getSize().x / 2;

        // Vehicle
        sprite.setTexture(textures[121], true);
        sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
        if (c.isDefaultScreen){
            if (c.enablePixelArt){
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 10.f);
            }
            else {
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 20.f);
            }
        }
        else {
            if (c.enablePixelArt){
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 5.f);
            }
            else {
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 10.f);
            }
        }
        c.w.draw(sprite);
    }
}



/**
 * Draw the starting animation in the screen at beginning of the race and returns if it has finished
 * @param c is the module configuration of the game
 * @param x is the position of the car in the axis x
 * @param end controls if the animation has finished
 */
void Player::drawInitialAnimation(Config &c, float x, bool &end, int& code) {
    if (textures.size() == PLAYER_TEXTURES) {
        if (counter_code_image >= maxCounterToChange) {
            current_code_image++;
            counter_code_image = 0;
        }
        if (current_code_image < 146 || current_code_image > 149){
            current_code_image = 146;
        }

        int index = 140;
        end = x < ((float) c.w.getSize().x) * 0.4f || x >= c.w.getSize().x;

        // Vehicle
        sprite.setTexture(textures[code], true);
        sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
        if (c.isDefaultScreen){
            if (c.enablePixelArt){
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 10.f);
            }
            else {
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 20.f);
            }
        }
        else {
            if (c.enablePixelArt){
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 5.f);
            }
            else {
                sprite.setPosition(x, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 10.f);
            }
        }
        c.w.draw(sprite);

        // Smoke
        float i = x - sprite.getGlobalBounds().width / 3, j = sprite.getPosition().y + sprite.getGlobalBounds().height;
        while (i < (float) c.w.getSize().x) {
            index = current_code_image;
            sprite.setTexture(textures[index], true);
            sprite.setScale(4.0f * c.screenScale, 4.0f * c.screenScale);
            sprite.setPosition(i, j - sprite.getGlobalBounds().height);
            c.w.draw(sprite);
            i += sprite.getGlobalBounds().width;
        }

        if (end) {
            current_code_image = 0;
            counter_code_image = 0;
        } else {
            counter_code_image++;
        }
    }
}



/**
 * Draw the final animation in the screen at the end of the race and returns if it has finished
 * @param c is the module configuration of the game
 * @param step is the position of the car in the axis x
 * @param end controls if the animation has finished
 * @param smoke controls that the smoke has to be drawn
 */
void Player::drawGoalAnimation(Config &c, int &step, bool &end, bool smoke) {
    if (textures.size() == PLAYER_TEXTURES) {
        if (counter_code_image >= maxCounterToChange) {
            current_code_image++;
            counter_code_image = 0;
            step++;
        }
        if (current_code_image < 146 || current_code_image > 149)
            current_code_image = 146;

        int index = 122;
        if (step < 2)
            index = 124;
        else if (step < 4)
            index = 121;

        // Vehicle
        sprite.setTexture(textures[index], true);
        sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
        minScreenX = ((float) c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
        maxScreenX = minScreenX + sprite.getGlobalBounds().width;
        if (c.isDefaultScreen){
            if (c.enablePixelArt){
                sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 10.f);
            }
            else {
                sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f + 20.f);
            }
        }
        else {
            if (c.enablePixelArt){
                sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 5.f);
            }
            else {
                sprite.setPosition(minScreenX, ((float) c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f - 10.f);
            }
        }
        c.w.draw(sprite);

        // Smoke
        if (smoke) {
            float i = minScreenX - sprite.getGlobalBounds().width / 3, j =
                    sprite.getPosition().y + sprite.getGlobalBounds().height;
            while (i < (float) c.w.getSize().x) {
                index = current_code_image;
                sprite.setTexture(textures[index], true);
                sprite.setScale(4.0f * c.screenScale, 4.0f * c.screenScale);
                sprite.setPosition(i, j - sprite.getGlobalBounds().height);
                c.w.draw(sprite);

                i += sprite.getGlobalBounds().width;
            }
        }

        if (step >= 6)
            end = true;

        if (end) {
            current_code_image = 0;
            counter_code_image = 0;
        } else {
            counter_code_image++;
        }
    }
}



/**
 * It forces the player's vehicle to be smoking or not
 * @param smoke indicates if the player's vehicle has to make smoke or not
 */
void Player::setSmoking(bool smoke) {
    smoking = smoke;
}



/**
 * Sets the type mode of collision
 */
void Player::setModeCollision(){
    // Check if the type of crash animation has been chosen
    if (mode == -1){
        // Get randomly the type of crash animation
        mode = random_int(0, 1);
    }
}




/**
 * Return the gear of the player's vehicle
 */
int Player::getGearPlayer(){
    return gearPlayer;
}
