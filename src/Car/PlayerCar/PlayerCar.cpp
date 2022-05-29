/*
 * Copyright (c) 2022 Ruben Rodriguez
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
 * Implementation file of the module PlayerCar
 */


#pragma once

#include <cmath>
#include "PlayerCar.h"
#include "../../Random/Random.h"

using namespace std;
using namespace sf;



/**
 * Default constructor
 */
PlayerCar::PlayerCar() : Vehicle(){}



/**
 * Second constructor
 * @param _posX is the position of the car in axis X
 * @param _posY is the position of the car in axis Y
 * @param _posZ is the position of the car in axis Z
 * @param _speed is the initial speed of the car
 * @param _scale is the scale factor to draw the sprites
 * @param _name is the path where the images of the car are stored
 * @param _automaticMode controls if the car operates with automatic or manual gears
 * @param _isTrafficCar controls if the car is a traffic car or not (player car).
 * @param _roadTerrain is type of terrain that has to be drawn by the wheels when the road is not asphalted
 */
PlayerCar::PlayerCar(const int _posX, const int _posY, const int _posZ, const float _speed, const float _scale,
                     const std::string _name, const bool _automaticMode, const bool _isTrafficCar, const int _roadTerrain)
                     : Vehicle(_posX, _posY, _posZ, _speed, _scale, _name, _isTrafficCar)
{
    // Assign the information to the car
	playerW = 77;
	varThresholdX = 0.06f;
	maxLowSpeed = maxHighSpeed = 100.f;
	lowAccel = maxHighSpeed / 6.5f;
	brakeAccel = maxHighSpeed / 2.0f;
	maxHighSpeed = 75.f;
	direction = Direction::FRONT;
	collisionDir = 0.f;
	collisionCurve = 0.f;
	out = 0;
    skidIndex = -1;
	gear = 0;
	showTerrain = 0;
	terrain = 0;
	current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;
	numAngers = 0;
	counterOut = 0;
	soundTrafficCounter = 0;
	current_terrain_image = 0;
	current_smoke_image = 0;
	numberCollisionLaps = 0;
	thresholdX = 0.f;
	speedGear = 0.f;
    playerRoad = Player_Road::LEFTROAD;

	wheelL = StateWheel::SMOKE;
	wheelR = StateWheel::SMOKE;
	elevation = Elevation::FLAT;

	roadTerrain = _roadTerrain;
    automaticMode = _automaticMode;
    crashing = false;
    firstTurnLeft = true;
    firstTurnRight = true;
    motorEngineSound = false;
    skidding = false;
    angryWoman = false;
    increaseGear = false;
    decreaseGear = false;
    trafficCrash = false;
    drawCar = true;
    endAnimation = false;
    outsideLeftWheelRoad = false;
    outsideRightWheelRoad = false;
}



/**
 * Set the acceleration of the car
 * @param _lowAccel is the new acceleration of the car
 */
void PlayerCar::setLowAccel(const float _lowAccel){
    lowAccel = _lowAccel;
}



/**
 * Set the gear of the car to the down position
 */
void PlayerCar::setGear(){
    gear = 0;
}



/**
 * Set the type of road (left or right where the
 * player car is driving
 * @param _playerRoad is the road where the car is driving
 */
void PlayerCar::setPlayerRoad(const Player_Road _playerRoad){
    playerRoad = _playerRoad;
}



/**
 * Set if the player car is currently crashing or it is playing the
 * collision animation
 * @param _crashing controls if the player has crashed or it is playing
 * the collision animation due to it crashed before
 */
void PlayerCar::setCrashing(const bool _crashing){
    crashing = _crashing;
}



/**
 * Set that there is not crash with any traffic car
 */
void PlayerCar::setTrafficCrash(){
    trafficCrash = false;
}



/**
 * Set the direction where the player has to crash before
 * in order to let more collisions
 */
void PlayerCar::setCollisionDir(){
    collisionDir = 0.f;
}



/**
 * Reset the curve coefficient where the player has crashed before
 * in order to let more collisions
 */
void PlayerCar::setCollisionCurve(){
    collisionCurve = 0.f;
}



/**
 * Set if the player car has to be drawn or not in the screen
 * Deactivated during the relocation of the car in the center of the nearest road
 * after finishing the collision animation
 * @param _drawCar controls if the car has to be drawn or not
 */
void PlayerCar::setDrawCar(const bool _drawCar){
    drawCar = _drawCar;
}



/**
 * Set that the woman has not to play the collision shout at the end
 */
void PlayerCar::setAngryWoman(){
    angryWoman = false;
}



/**
 * Set the total number of angers done by the blond woman
 * when the player car crashes against something (0..3)
 */
void PlayerCar::setNumAngers(){
    numAngers = 0;
}



/**
 * Set the type of terrain outside the road to display the correct effect by the wheels
 * @param _terrain is the type of terrain outside the road
 */
void PlayerCar::setTerrain(const int _terrain){
    terrain = _terrain;
}




/**
 * Set the type of terrain of the road to display the correct effect by the wheels
 * @param _roadTerrain is the type of terrain of the road
 */
void PlayerCar::setRoadTerrain(const int _roadTerrain){
    roadTerrain = _roadTerrain;
}



/**
 * Set the showTerrain effect to deactivated
 * Any kind of wheel effect is drawn when the car is in the road
 * unless there is terrain inside it
 */
void PlayerCar::setShowTerrain(){
    showTerrain = 0;
}



/**
 * Set the status of the left wheel of the car (outside or inside the road)
 * @param _outsideLeftWheelRoad is the status of the left wheel
 */
void PlayerCar::setOutsideLeftWheelRoad(const bool _outsideLeftWheelRoad){
    outsideLeftWheelRoad = _outsideLeftWheelRoad;
}



/**
 * Set the status of the right wheel of the car (outside or inside the road)
 * @param _outsideLeftWheelRoad is the status of the right wheel
 */
void PlayerCar::setOutsideRightWheelRoad(const bool _outsideRightWheelRoad){
    outsideRightWheelRoad = _outsideRightWheelRoad;
}



/**
 * Set the status of the left wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
 * @param _state is the status of the left wheel
 */
void PlayerCar::setStateWheelLeft(const StateWheel state){
    wheelL = state;
}



/**
 * Set the status of the right wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
 * @param _state is the status of the right wheel
 */
void PlayerCar::setStateWheelRight(const StateWheel state){
    wheelR = state;
}



/**
 * Set if the car is skidding or not
 * @param skid controls if the car is skidding or not
 */
void PlayerCar::setSkidding(const bool skid){
    skidding = skid;
}



/**
 * Set if the car has terminated the animation of drift when starts
 * and also when the game round finishes
 */
void PlayerCar::setEndAnimation(const bool _endAnimation){
    endAnimation = _endAnimation;
}



/**
 * Get the acceleration of the car
 */
float PlayerCar::getLowAccel() const {
    return lowAccel;
}



/**
 * Get the gear of the car
 */
int PlayerCar::getGear() const {
    return gear;
}



/**
 * Get the road where the player car is driving
 */
Player_Road PlayerCar::getPlayerRoad() const {
    return playerRoad;
}



/**
 * Get if the car is crashing or playing the crash animation
 */
bool PlayerCar::getCrashing() const {
    return crashing;
}



/**
 * Get if the car has crashed or not with a traffic car
 */
bool PlayerCar::getTrafficCrash() const {
    return trafficCrash;
}



/**
 * Get the collision direction of the car
 */
float PlayerCar::getCollisionDir() const {
    return collisionDir;
}



/**
 * Get the coefficient of the curve where the car has crashed
 */
float PlayerCar::getCollisionCurve() const {
    return collisionCurve;
}



/**
 * Get the number of angers done by the blond woman
 */
int PlayerCar::getNumAngers() const {
    return numAngers;
}



/**
 * Get the status of the left wheel of the car (outside or inside the road)
 */
bool PlayerCar::getOutsideLeftWheelRoad() const {
    return outsideLeftWheelRoad;
}



/**
 * Get the status of the right wheel of the car (outside or inside the road)
 */
bool PlayerCar::getOutsideRightWheelRoad() const {
    return outsideRightWheelRoad;
}



/**
 * Get the status of the left wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
 */
StateWheel PlayerCar::getStateWheelLeft() const {
    return wheelL;
}



/**
 * Get the status of the right wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
 */
StateWheel PlayerCar::getStateWheelRight() const {
    return wheelR;
}



/**
 * Get if the car has finished the starting or ending drift animations
 */
bool PlayerCar::getEndAnimation() const {
    return endAnimation;
}



/**
 * Get the maximum speed of the player car (depending on the gear used)
 */
float PlayerCar::getHighMaxSpeed() const {
    return maxHighSpeed;
}



/**
 * Get the maximum speed reached by the car when it is using the low gear
 */
float PlayerCar::getLowMaxSpeed() const {
    return maxLowSpeed;
}



/**
 * Get the total capability of the car to turn
 */
float PlayerCar::getThresholdX() const {
    return thresholdX;
}



/**
 * Compute the actions introduced by the player to move, accelerate or brake the car
 * @param input is the module that has all the configuration of the game
 * @param gameStatus is the status of the game
 * @param time is the time elapsed between the previous move and the current one
 */
void PlayerCar::accelerationControl(Input& input, const State gameStatus, const float time){

    // Default actions
    sf::Event event;
    action = Action::ACCELERATE;
    direction = Direction::FRONT;

    // Check the game status
    switch(gameStatus){
        // Play round (player controls the car)
        case State::PLAY_ROUND:
            if ((wheelL == StateWheel::SAND || wheelR == StateWheel::SAND ||
                 wheelL == StateWheel::GRASS || wheelR == StateWheel::GRASS ||
                 wheelL == StateWheel::SNOW || wheelR == StateWheel::SNOW ||
                 wheelL == StateWheel::MUD || wheelR == StateWheel::MUD) &&
                (outsideLeftWheelRoad || outsideRightWheelRoad))
            {
                // The car is outside the road (one or both wheels) reduce the speed
                maxHighSpeed = 75.f;
            }
            else
            {
                // Inside the road

                // Depending on the gear (down or up) has a maximum speed to reach
                if (gear == 1){
                    maxHighSpeed = 200.f;
                }
                else {
                    maxHighSpeed = 100.f;
                }
            }

            // Check if the car has speed and it is braking
            if ((input.pressed(Key::BRAKE, event) || input.held(Key::BRAKE)) && speed > 0.f){

                // Reduce speed until definitely stops
                speed -= brakeAccel * time;
                if (speed <= 0.f) {
                    speed = 0.f;
                    motorEngineSound = false;
                    wheelL = wheelR = StateWheel::NORMAL;
                }

                // Decrease gear if it goes with the up gear
                if (gear == 1 && speed < 100.f){
                    gear = 0;
                    decreaseGear = true;
                }
                else if (decreaseGear)
                    if (speed < 90.f)
                        decreaseGear = false;

                // Activation of the smoke effect
                wheelL = wheelR = StateWheel::SMOKE;
                action = (speed == 0.f) ? Action::NONE : Action::BRAKE;
            }
            else if ((input.pressed(Key::ACCELERATE, event) || input.held(Key::ACCELERATE)) && speed <= maxHighSpeed &&
                     !outsideLeftWheelRoad && !outsideRightWheelRoad)
            {
                // Acceleration until reaches the maximum speed
                if (speed <= maxHighSpeed){
                    speed += lowAccel * time;
                    if (speed > maxHighSpeed)
                        speed = maxHighSpeed;
                }
                else
                    speed = maxHighSpeed;

                // Activate sounds
                action = (speed > 15.f) ? Action::ACCELERATE : Action::BOOT;
                motorEngineSound = true;

                // Activate smoke effects when the car is in the road but is booting
                if (speed < 50.f && wheelL != StateWheel::SAND && wheelR != StateWheel::SAND &&
                    wheelL != StateWheel::GRASS && wheelR != StateWheel::GRASS &&
                    wheelL != StateWheel::SNOW && wheelR != StateWheel::SNOW &&
                    wheelL != StateWheel::MUD && wheelR != StateWheel::MUD)
                {
                    wheelR = StateWheel::SMOKE;
                }
                else {
                    wheelR = StateWheel::NORMAL;
                }
                if (speed < 50.f && wheelR != StateWheel::SAND && wheelL != StateWheel::SAND &&
                    wheelR != StateWheel::GRASS && wheelL != StateWheel::GRASS &&
                    wheelR != StateWheel::SNOW && wheelL != StateWheel::SNOW &&
                    wheelR != StateWheel::MUD && wheelL != StateWheel::MUD)
                {
                    wheelL = StateWheel::SMOKE;
                }
                else {
                    wheelL = StateWheel::NORMAL;
                }

                // Automatic mode (gears change automatically when maximum speed is reached)
                if (automaticMode){
                    if (speed == maxHighSpeed && gear == 0){
                        gear = 1;
                        increaseGear = true;
                    }
                    if (increaseGear && speed > 110.f)
                        increaseGear = false;
                }
                else {
                    // Manual gears
                    if ((input.pressed(Key::UP_GEAR, event) || input.held(Key::UP_GEAR) && gear != 1)){
                        // Up gear
                        gear = 1;
                        increaseGear = true;
                        speedGear = speed;
                    }
                    else if ((input.pressed(Key::DOWN_GEAR, event) || input.held(Key::DOWN_GEAR)) && gear != 0){
                        // Down gear
                        gear = 0;
                        decreaseGear = true;
                        speedGear = speed;

                        if (speed > 100.f)
                            speed -= lowAccel * time;
                    }
                    if (increaseGear && (speed > speedGear + 10.f))
                        increaseGear = false;
                    else if (decreaseGear && speed + 10.f < speedGear)
                        decreaseGear = false;
                }
            }
            else {
                // No controls is being pressed (automatically lose speed
                if (speed <= 0.f) {
                    speed = 0.f;
                    action = Action::NONE;
                    motorEngineSound = false;
                }
                else
                    // Lose speed but less than when it brakes
                    speed -= (lowAccel * time * 0.75f);

                wheelL = wheelR = StateWheel::NORMAL;
                increaseGear = false;

                if (gear == 1 && speed < 100.f){
                    gear = 0;
                    decreaseGear = true;
                }
                else if (decreaseGear){
                    if (speed < 90.f)
                        decreaseGear = false;

                    action = Action::BRAKE;
                    wheelL = wheelR = StateWheel::SMOKE;
                }
            }

            // Compute the angle to turn the car
            varThresholdX = speed * time;

            // Control if turning keys are played
            if (input.pressed(Key::TURN_LEFT, event) || input.held(Key::TURN_LEFT))
            {
                // Turning left
                direction = Direction::TURNLEFT;
                if (thresholdX > -varThresholdX)
                    thresholdX -= 0.3f;
                else if (thresholdX < -varThresholdX)
                    thresholdX += 0.3f;
            }
            else if (input.pressed(Key::TURN_RIGHT, event) || input.held(Key::TURN_RIGHT))
            {
                // Turnin right
                direction = Direction::TURNRIGHT;
                if (thresholdX < varThresholdX)
                    thresholdX += 0.3f;
                else if (thresholdX > varThresholdX)
                    thresholdX -= 0.3f;
            }
            else {
                // Go straight
                direction = Direction::FRONT;
                if (thresholdX != 0.f)
                {
                    thresholdX -= thresholdX * 0.1f;
                    if (thresholdX < 0.1f && thresholdX > -0.1f)
                        thresholdX = 0.f;
                }
            }
            posX += (thresholdX * time);

            if (increaseGear)
                wheelL = wheelR = StateWheel::SMOKE;

            break;
        case State::END_ROUND:
            // Behavior of the car at the ending animation
            if (endAnimation){
                // The car increase speed until maximum
                if (gear == 0)
                    gear = 1;

                maxHighSpeed = (int)SEGMENT_LENGTH;
                speed += lowAccel * time;
                if (speed > maxHighSpeed)
                    speed = maxHighSpeed;

                wheelL = wheelR = StateWheel::NORMAL;

                // The car is centered in the road
                if (posX > 0.1f){
                    posX -= 0.5f * time;
                    direction = Direction::TURNLEFT;
                }
                if (posX < -0.1f){
                    posX += 0.5f * time;
                    direction = Direction::TURNRIGHT;
                }
                if (posX != 0.f && posX < 0.1f && posX > -0.1f){
                    posX = 0.f;
                    direction = Direction::FRONT;
                }
            }
            else {
                // There car starts to brake and after to drift
                action = Action::BRAKE;

                // Decrease speed until it stops and activate smoke effect
                if (speed > 0.f){
                    speed -= lowAccel * time * 3.f;
                    if (speed < 0.f)
                        speed = 0.f;

                    wheelL = wheelR = StateWheel::SMOKE;
                }
                else {
                    action = Action::NONE;
                    wheelL = wheelR = StateWheel::NORMAL;
                }
            }
        }
}



/**
 * Makes the effect of centrifugal force when the player car is in a curve and goes so fast
 * @param playerLine is the position of the player in the scenario
 * @param time is the time elapsed between the previous move and the current one
 * @param roadDistance is the width of the road
 */
void PlayerCar::controlCentrifugalForce(const Line* playerLine, const float& time, const int roadDistance){

    // Start a centrifugal effect force depending of the speed
	float centrifugal = (speed > 26) ? 0.5f : 0.f;
	if (speed >= 100.f)
		centrifugal = (speed - 50.f) / 90.f;

    // Modify the position in axis X depending on speed, position and road
	switch (playerRoad){
        case Player_Road::RIGHTROAD:
            if (roadDistance != playerLine->distance)
                posX += (playerLine->distance - roadDistance) / (float)ROAD_WIDTH;
            if (playerLine->mirror)
                posX += (playerLine->curve * MIN((speed / maxHighSpeed), 1.f) * centrifugal * time);
            else
                posX -= (playerLine->curve * MIN((speed / maxHighSpeed), 1.f) * centrifugal * time);
            break;
        case Player_Road::LEFTROAD:
            posX -= (playerLine->curve * MIN((speed / maxHighSpeed), 1.f) * centrifugal * time);
	}
}



/**
 * Check if the car has crashed with any element (car or element of the scene)
 * @param x1 is the bottom left corner of the car image
 * @param w1 is the width of the car texture
 * @param x2 is the bottom left corner of the map element or traffic car sprite
 * @param w2 is the width of the map element or traffic car texture
 * @param scale is th scale factor used to display the sprites in the screen
 */
bool PlayerCar::hasCrashed(float x1, int w1, float x2, float w2, float scale){

    // Get the limit points of collision and compares them
	float min1 = x1 - (((float)w1 * 3.2f) / 2.f);
	float max1 = x1 + (((float)w1 * 3.2f) / 2.f);

	float min2 = x2 - ((w2 * scale) / 2.f);
	float max2 = x2 + ((w2 * scale) / 2.f);

	return (max1 >= min2 && max2 >= min1);
}



/**
 * Check if there is collision between the player car and a map element of the scenario
 * @param input is the module that has all the configuration of the game
 * @param playerLine is the position of the player in the scenario
 * @param crashed controls if the player is crashing now or not
 * @param sprite is the map element with which the collision is going to be checked
 */
void PlayerCar::checkCollisionSpriteInfo(Input& input, const Line* playerLine, bool& crashed, const SpriteInfo* sprite){

    // Check if the sprite can be collide
    if (sprite->getCollider())
    {

        // Get the the reference point of the sprite
        VertexPoint point = playerLine->lowerLeftPoint;
        if (sprite->getSide())
            point = playerLine->upperLeftPoint;

        // Compute the coordinates and dimensions to control the collide
        float x2 = point.xPosScreen + (sprite->getOffsetX() * point.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
        float scale = 1.6f * (0.3f * (1.f / 170.f)) * point.scale * input.gameWindow.getSize().x * ROAD_WIDTH * sprite->getScale();
        int width = sprite->getTextureSprite()->getSize().x;

        if (sprite->getOffsetX() >= 0)
            x2 = x2 + width * scale * sprite->getPivotColRight().x;

        else
            x2 = x2 - width * scale * (1 - sprite->getPivotColLeft().x);

        // Check if there is collision
        if (hasCrashed((((int) input.gameWindow.getSize().x) / 2.0f) + 5,
                       playerW, x2, sprite->getWidthCol(), scale))
        {
            // Get the collision cure coefficient
            collisionCurve = (abs(playerLine->curve) >= 2.f) ? playerLine->curve * 0.3f : playerLine->curve;
            collisionDir = posX;
            crashing = true;
            crashed = true;

            // No collision with traffic car and decide type of animation
            trafficCrash = false;
            modeCollision = static_cast<Collision>(random_int((int)Collision::LEFT_TO_RIGHT,
                                                              (int)Collision::RIGHT_TO_LEFT));

            // Set the number of laps of the collision
            if (speed <= 20.f)
                numberCollisionLaps = 1;
            else if (speed >= 20.f && speed <= 60.f)
                numberCollisionLaps = 2;
            else if (speed > 60.f && speed <= 120.f)
                numberCollisionLaps = 3;
            else
                numberCollisionLaps = 4;

            // Calculate how much acceleration has to lose and draw smoke in the wheels
            lowAccel = speed / (numberCollisionLaps * SPRITES_PER_COLLISION);
            wheelL = StateWheel::SMOKE;
            wheelR = StateWheel::SMOKE;
            Audio::play(Sfx::FERRARI_CRASH, false);

            // Play a random spectators sound
            Audio::play(static_cast<Sfx>(random_int((int)Sfx::SPECTATORS_FIRST_SHOUT,
                                                    (int)Sfx::SPECTATORS_EIGHTH_SHOUT)), false);

        }
    }
    // Detect "collision" between the car and terrain in the road
    else if (!outsideLeftWheelRoad && !outsideRightWheelRoad && showTerrain == 0 && sprite->getShowTerrain() == 1)
        showTerrain = 1;
}



/**
 * Check if there is collision between the player car and a traffic car of the scenario
 * @param input is the module that has all the configuration of the game
 * @param playerLine is the position of the player in the scenario
 * @param trafficCarLine is the position of the traffic car in the scenario
 * @param trafficCar is the traffic car with which the collision is going to be checked
 * @param crashed controls if the player is crashing now or not
 */
void PlayerCar::checkCollisionTrafficCar(Input& input, const Line* playerLine, const Line* trafficCarLine,
                                         TrafficCar* trafficCar, bool& crashed)
{

    // Check if the player and the traffic car are in the same position
    if (trafficCarLine->index == playerLine->index)
    {
        // The reference points of the player car
        VertexPoint point = playerLine->lowerLeftPoint;
        VertexPoint point2 = playerLine->lowerRightPoint;
        if (trafficCar->getSide())
        {
            // Get the reference points of the player car but in the other side (drives in the right road)
            point = playerLine->upperLeftPoint;
            point2 = playerLine->upperRightPoint;
        }

        // Compute the scale and the coordinate points to determine the collision
        float perc = (float)(((int)(trafficCar->getPosZ()) % (int)SEGMENT_LENGTH) / (float)SEGMENT_LENGTH);
        float scaleOffset = point.scale + (point2.scale - point.scale) * perc;
        float xOffset = point.xPosScreen + (point2.xPosScreen - point.xPosScreen) * perc;
        float yOffset = point.yPosScreen + (point2.yPosScreen - point.yPosScreen) * perc;

        float x2 = xOffset + (trafficCar->getOffset() * scaleOffset * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
        float scale = 1.6f * (0.3f * (1.f / 170.f)) * scaleOffset * input.gameWindow.getSize().x * ROAD_WIDTH * 1.2f;

        // Check if there is collision
        if (hasCrashed((float)(input.gameWindow.getSize().x / 2) + 5.f, playerW, x2,
                        (float)trafficCar->getTexture().getSize().x, scale))
        {
            // There is collision and establish the starting point
            collisionDir = posX;
            crashing = true;
            crashed = true;

            // Collision with traffic car activated and set the type of animation
            trafficCrash = true;
            modeCollision = static_cast<Collision>(random_int((int)Collision::LEFT_TO_RIGHT,
                                                              (int)Collision::RIGHT_TO_LEFT));


            // Calculation of the number of laps depending on the speed
            if (speed <= 20.f)
                numberCollisionLaps = 1;
            else if (speed >= 20.f && speed <= 60.f)
                numberCollisionLaps = 2;
            else if (speed > 60.f && speed <= 120.f)
                numberCollisionLaps = 3;
            else
                numberCollisionLaps = 4;

            // Compute how much acceleration has to lose in each lap and put smoke in the wheels
            lowAccel = speed / (numberCollisionLaps * SPRITES_PER_COLLISION);
            wheelL = StateWheel::SMOKE;
            wheelR = StateWheel::SMOKE;
            Audio::play(Sfx::FERRARI_CRASH, false);
            Audio::play(static_cast<Sfx>(random_int((int)Sfx::SPECTATORS_FIRST_SHOUT,
                                                    (int)Sfx::SPECTATORS_EIGHTH_SHOUT)), false);
        }
    }
    else if (abs((posX * ROAD_WIDTH) - trafficCar->getPosX()) <= MINIMUM_DISTANCE_X &&
             abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
    {
        // There is no collision between them but they are so near
        if (soundTrafficCounter == 0){

            // Determine which car is and play traffic sound depending on it
            if (trafficCar->getId() % 6 == 0 || trafficCar->getId() == 32)
                Audio::play(static_cast<Sfx>(random_int((int)Sfx::TRAFFIC_THIRD_CLAXON,
                                                        (int)Sfx::TRAFFIC_FOURTH_CLAXON)), false);
            else
                Audio::play(static_cast<Sfx>(random_int((int)Sfx::TRAFFIC_FIRST_CLAXON,
                                                        (int)Sfx::TRAFFIC_SECOND_CLAXON)), false);

            // Same with the kind of motor sound
            if (trafficCar->getId() == 1 || trafficCar->getId() == 7 || trafficCar->getId() == 13 ||
                trafficCar->getId() == 19 || trafficCar->getId() == 25 ||
                (trafficCar->getId() < 30 && trafficCar->getId() % 3 == 0 && trafficCar->getId() % 6 != 0))
            {
                Audio::play(Sfx::TRAFFIC_THIRD_ENGINE, false);
            }
            else if (trafficCar->getId() % 6 != 0 && trafficCar->getId() != 32)
                Audio::play(static_cast<Sfx>(random_int((int)Sfx::TRAFFIC_FIRST_ENGINE,
                                                        (int)Sfx::TRAFFIC_SECOND_ENGINE)), false);

            // Count time to play another sound (not play multiple sounds at the same time)
            soundTrafficCounter++;

            // Play woman sounds
            if (abs((posX * ROAD_WIDTH) - trafficCar->getPosX()) <= (MINIMUM_DISTANCE_X - 1000) &&
                abs(trafficCarLine->index - playerLine->index) <= (MINIMUM_DISTANCE_Y - 10))
            {
                Audio::play(static_cast<Sfx>(random_int((int)Sfx::BLOND_WOMAN_FIRST_SHOUT,
                                                        (int)Sfx::BLOND_WOMAN_THIRD_SHOUT)), false);
            }
        }

        // Traffic car is near
        if (!trafficCar->getPlayerClosed())
            trafficCar->setPlayerClosed(true);
    }
    else
        // There is not traffic car close
        trafficCar->setPlayerClosed(false);

    // Count time between sounds
    if (soundTrafficCounter > 0 && soundTrafficCounter < MAX_TRAFFIC_COUNTER_WAIT)
        soundTrafficCounter++;
    else
        soundTrafficCounter = 0;

}



/**
 * Draw the car on the screen before starts to drift to the center
 * @param input is the module that has all the configuration of the game
 */
void PlayerCar::drawStartStaticRound(Input& input) {
    // Check if all the textures are loaded correctly (avoiding errors)
    if (textures.size() == PLAYER_TOTAL_TEXTURES) {
        // Draw the car on the right position on the screen before starts to drift to the center
        float x = input.gameWindow.getSize().x / 2;
        sprite.setTexture(textures[121], true);
        sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
        sprite.setPosition(x, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f));
        input.gameWindow.draw(sprite);
    }
}



/**
 * Draw the starting animation drift of the player car
 * @param input is the module that has all the configuration of the game
 * @param x is the coordinate of the car in axis X
 * @param code is the identifier of the car sprite to be drawn
 */
void PlayerCar::drawStartDriftRound(Input &input, float x, int& code){

    // Play the skidding sound
    if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
        Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, false);

    // Check if all the textures are loaded correctly (avoiding errors)
    if (textures.size() == PLAYER_TOTAL_TEXTURES){

        // Get the index of the smoke to be displayed
        if (counter_code_image >= maxCounterToChange){
            current_code_image++;
            counter_code_image = 0;
        }
        if (current_code_image < 148 || current_code_image > 151)
            current_code_image = 148;

        // Control when the player arrives to the destiny position in axis X to finish the drift
        float width = input.gameWindow.getSize().x;
        endAnimation = x < width * 0.4f || x >= width;
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

        // Draw the car
        sprite.setTexture(textures[code], true);
        sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
        sprite.setPosition(x, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f));
        input.gameWindow.draw(sprite);

        // Draw the smoke effects (left and right)
        sprite.setTexture(textures[current_code_image], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(x, j - sprite.getGlobalBounds().height);
        input.gameWindow.draw(sprite);

        float offsetSkid = (code > 120) ? 1.6f : 1.5f;
        sprite.setPosition(x * offsetSkid, j - sprite.getGlobalBounds().height);
        input.gameWindow.draw(sprite);

        // Check if the animation has finished to deactivate wheel effects and establish the car straight
        if (endAnimation) {
            current_code_image = 1;
            counter_code_image = 0;
            wheelL == StateWheel::NORMAL;
            wheelR == StateWheel::NORMAL;
        }
        else
            // Continue the animation
            counter_code_image++;
    }
}



/**
 * Draw the player car during the game round
 * @param input is the module that has all the configuration of the game
 * @param pauseMode controls if the game is or not paused
 * @param terrain stores the type of terrain outside the road
 * @param motorEngineSound controls if the sounds of the car must be played
 */
void PlayerCar::drawPlayRound(Input& input, const bool& pauseMode, const int terrain, const bool& motorEngineSound) {

    //Check if the car has to be drawn in the screen
    if (drawCar){
        // Check if the sounds have to played and if it moving
        if (motorEngineSound) {
            if (speed > 0.0f) {

                // Booting from stopped
                if (action == Action::BOOT){

                    // Booting sound
                    if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_START))
                        Audio::play(Sfx::FERRARI_ENGINE_START, false);
                }
                else if (action == Action::ACCELERATE){

                    // Stop braking and play acceleration sound in loop
                    Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

                    if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN))
                        Audio::play(Sfx::FERRARI_ENGINE_RUN, true);
                }
                else if (action == Action::BRAKE){
                    // Braking

                    // Stop acceleration sound
                    if (!decreaseGear)
                        Audio::stop(Sfx::FERRARI_ENGINE_RUN);

                    // Play braking sound
                    if (!decreaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                        Audio::play(Sfx::FERRARI_ENGINE_BRAKE, true);
                }

                // Play skidding sounds
                if (!crashing && skidding && !Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
                    Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, true);
                else if (crashing || (!skidding && Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING)))
                    Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);

                // Play the car outside sounds when both wheels or one of them is out of the road
                if (!crashing && (outsideLeftWheelRoad || outsideRightWheelRoad) && !Audio::isPlaying(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE))
                    Audio::play(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE, true);
                else if (crashing || (!outsideLeftWheelRoad && !outsideRightWheelRoad && Audio::isPlaying(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE)))
                    Audio::stop(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE);
            }
            else
                // The car is stopped
                shutDownSounds();

            // Play car drift sound when is  crashing otherwise stop (spinning effect
            if (crashing && speed > 0.f) {
                if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_DRIFT))
                    Audio::play(Sfx::FERRARI_ENGINE_DRIFT, true);
            }
            else
                Audio::stop(Sfx::FERRARI_ENGINE_DRIFT);

            // Car is crashing
            if (crashing){

                // Set the action and establish both wheels with smoke (spin)
                action = Action::CRASH;
                outsideLeftWheelRoad = false;
                outsideRightWheelRoad = false;

                if (speed > 0.f){
                    wheelL = StateWheel::SMOKE;
                    wheelR = StateWheel::SMOKE;
                }

                // Stop acceleration and braking sounds
                if (Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN))
                    Audio::stop(Sfx::FERRARI_ENGINE_RUN);

                if (Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                    Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

                // Play the woman shout when the car has crashed
                if (angryWoman && numAngers == 1 && !Audio::isPlaying(Sfx::BLOND_WOMAN_DIE))
                    Audio::play(Sfx::BLOND_WOMAN_DIE, false);
            }
            else {
                if (speed <= 0.f) {
                    // Crash animation has finished (stop drawing smoke)
                    wheelL = StateWheel::NORMAL;
                    wheelR = StateWheel::NORMAL;
                }
            }

            // Play the sound of increasing or decreasing the gears
            if (increaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_UP_GEAR))
                Audio::play(Sfx::FERRARI_ENGINE_UP_GEAR, false);
            else if (!increaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_UP_GEAR))
                Audio::stop(Sfx::FERRARI_ENGINE_UP_GEAR);

            if (decreaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_DOWN_GEAR)){
                Audio::play(Sfx::FERRARI_ENGINE_DOWN_GEAR, false);
                decreaseGear = false;
            }
        }
        else
            // Stop all the sounds
            shutDownSounds();

        // Controls if the car has started turning left or right
        if (direction != Direction::TURNLEFT)
            firstTurnLeft = true;
        if (direction != Direction::TURNRIGHT)
            firstTurnRight = true;

        // Check the current action of the car to be drawn in the screen
        if (action != Action::NONE) {
            if (counter_code_image >= maxCounterToChange) {
                counter_code_image = 0;

                // Check if it is not paused to change the sprite to drawn
                if (!pauseMode){

                    // Index of the wheel effects when the car is outside the road
                    if (outsideLeftWheelRoad || outsideRightWheelRoad || roadTerrain != 0 || showTerrain == 1)
                        current_terrain_image = (current_terrain_image < 5) ? current_terrain_image + 1 : 0;

                    if (wheelL == StateWheel::SMOKE || wheelR == StateWheel::SMOKE)
                        current_smoke_image = (current_smoke_image < 3) ? current_smoke_image + 1 : 0;
                }

                // Increment the texture counter only if it moves
                if (!pauseMode && (speed > 0.f || (speed <= 0.f && numAngers < 3)))
                    current_code_image++;

                // Get the new code of the sprite to drawn depending on its action, direction and elevation
                if (textures.size() == PLAYER_TOTAL_TEXTURES){
                    if (action == Action::ACCELERATE || action == Action::BOOT){
                        if (elevation == Elevation::FLAT) {
                            if (direction == Direction::FRONT) {
                                if (current_code_image < 1 || current_code_image > 4)
                                    current_code_image = 1;
                            }
                            else if (direction == Direction::TURNLEFT) {
                                if (firstTurnLeft) {
                                    if (current_code_image < 5 || current_code_image > 12)
                                        current_code_image = 5;
                                    if (current_code_image == 9)
                                        firstTurnLeft = false;
                                }
                                else {
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
                                }
                                else {
                                    if (current_code_image < 17 || current_code_image > 20)
                                        current_code_image = 17;
                                }
                            }
                        }
                        else if (elevation == Elevation::UP){
                            if (direction == Direction::FRONT) {
                                if (current_code_image < 21 || current_code_image > 24)
                                    current_code_image = 21;
                            }
                            else if (direction == Direction::TURNLEFT) {
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
                        else {
                            if (direction == Direction::FRONT) {
                                if (current_code_image < 41 || current_code_image > 44)
                                    current_code_image = 41;
                            }
                            else if (direction == Direction::TURNLEFT) {
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
                    }
                    else if (action == Action::BRAKE){
                        if (elevation == Elevation::FLAT){
                            if (direction == Direction::FRONT) {
                                if (current_code_image < 61 || current_code_image > 64)
                                    current_code_image = 61;
                            }
                            else if (direction == Direction::TURNLEFT) {
                                if (firstTurnLeft) {
                                    if (current_code_image < 65 || current_code_image > 72)
                                        current_code_image = 65;
                                    if (current_code_image == 69)
                                        firstTurnLeft = false;
                                }
                                else {
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
                                }
                                else {
                                    if (current_code_image < 77 || current_code_image > 80)
                                        current_code_image = 77;
                                }
                            }
                        }
                        else if (elevation == Elevation::UP) {
                            if (direction == Direction::FRONT) {
                                if (current_code_image < 81 || current_code_image > 84)
                                    current_code_image = 81;
                            }
                            else if (direction == Direction::TURNLEFT) {
                                if (firstTurnLeft) {
                                    if (current_code_image < 85 || current_code_image > 92)
                                        current_code_image = 85;
                                    if (current_code_image == 89)
                                        firstTurnLeft = false;
                                }
                                else {
                                    if (current_code_image < 89 || current_code_image > 92)
                                        current_code_image = 89;
                                }
                            }
                            else { // Turn right
                                if (firstTurnRight) {
                                    if (current_code_image < 93 || current_code_image > 100)
                                        current_code_image = 93;
                                    if (current_code_image == 97)
                                        firstTurnRight = false;
                                }
                                else {
                                    if (current_code_image < 97 || current_code_image > 100)
                                        current_code_image = 97;
                                }
                            }
                        }
                        else { // Down
                            if (direction == Direction::FRONT){
                                if (current_code_image < 101 || current_code_image > 104)
                                    current_code_image = 101;
                            }
                            else if (direction == Direction::TURNLEFT){
                                if (firstTurnLeft) {
                                    if (current_code_image < 105 || current_code_image > 112)
                                        current_code_image = 105;
                                    if (current_code_image == 109)
                                        firstTurnLeft = false;
                                }
                                else {
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
                                }
                                else {
                                    if (current_code_image < 117 || current_code_image > 120)
                                        current_code_image = 117;
                                }
                            }
                        }
                    }
                    else if (action == Action::CRASH){
                        if (modeCollision == Collision::LEFT_TO_RIGHT) {
                            // First type of animation
                            if (speed <= 0.f){

                                if (numAngers < 3){
                                    if (current_code_image < 129 || current_code_image > 133)
                                        current_code_image = 129;
                                    else {
                                        if (current_code_image == 133){
                                            numAngers++;

                                            if (numAngers == 1)
                                                angryWoman = true;
                                        }
                                     }
                                }
                                else
                                    current_code_image = 0;
                            }
                            else {
                                if (current_code_image < 121 || current_code_image > 128)
                                    current_code_image = 121;
                            }
                        }
                        else {
                            // Second type of animation
                            if (speed <= 0.f){

                                if (numAngers < 3){
                                     if (current_code_image < 142 || current_code_image > 146)
                                        current_code_image = 142;
                                     else {
                                        if (current_code_image == 146){
                                            numAngers++;

                                            if (numAngers == 1)
                                                angryWoman = true;
                                        }
                                     }
                                }
                                else
                                    current_code_image = 0;
                            }
                            else {
                                if (current_code_image < 134 || current_code_image > 141)
                                    current_code_image = 134;
                            }
                        }
                    }
                }
            }
            else
                counter_code_image++;
        }
        else {
            if (elevation == Elevation::FLAT)
                current_code_image = 1;
            else if (elevation == Elevation::UP)
                current_code_image = 21;
            else
                current_code_image = 41;
        }

        // Check if the game is paused (not buzzing effect)
        if (!pauseMode){
            // Te car is outside the road and the buzz effect is done
            if ((outsideLeftWheelRoad || outsideRightWheelRoad) && speed > 0.f){
                if (counterOut == 5){
                    out = (out == -1) ? 0 : -1;
                    counterOut = 0;
                }
                else
                    counterOut++;
            }
            else {
                out = 0;
                counterOut = 0;
            }
        }
        else
            out = 0;

        // Draw the car
        sprite.setTexture(textures[current_code_image - 1], true);
        sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
        float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
        sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f) + out);
        input.gameWindow.draw(sprite);

        // Check if it crashing to decrease the speed of the spinners
        if (crashing) {
            if (speed <= 0.f)
                maxCounterToChange = 6;
            else if (speed > 0.f && speed < 20.f)
                maxCounterToChange = 5;
            else if (speed >= 20.f && speed < 60.f)
                maxCounterToChange = 4;
            else if (speed >= 60.f && speed < 100.f)
                maxCounterToChange = 3;
        }
        else
             maxCounterToChange = 2;

        // Draw the wheels effects (left, right or both wheels)
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

        // Display the sand effect
        if ((wheelL == StateWheel::SAND || (roadTerrain == 1 && !outsideLeftWheelRoad) || (showTerrain == 1 && terrain == 1))  && !crashing && speed > 0.f){
            sprite.setTexture(textures[152 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width),
                                j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        if ((wheelR == StateWheel::SAND || (roadTerrain == 1 && !outsideRightWheelRoad) || (showTerrain == 1 && terrain == 1))  && !crashing && speed > 0.f){
            sprite.setTexture(textures[152 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }

        // Display the grass effect
        if ((wheelL == StateWheel::GRASS || (roadTerrain == 2 && !outsideLeftWheelRoad) || (showTerrain == 1 && terrain == 2)) && !crashing && speed > 0.f){
            sprite.setTexture(textures[158 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width),
                                j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        if ((wheelR == StateWheel::GRASS || (roadTerrain == 2 && !outsideRightWheelRoad) || (showTerrain == 1 && terrain == 2)) && !crashing && speed > 0.f){
            sprite.setTexture(textures[158 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }

        // Display the snow effect
        if ((wheelL == StateWheel::SNOW || (roadTerrain == 3 && !outsideLeftWheelRoad) || (showTerrain == 1 && terrain == 3))  && !crashing && speed > 0.f){
            sprite.setTexture(textures[164 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width),
                                j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        if ((wheelR == StateWheel::SNOW || (roadTerrain == 3 && !outsideRightWheelRoad) || (showTerrain == 1 && terrain == 3))  && !crashing && speed > 0.f){
            sprite.setTexture(textures[164 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }

        // Display the mud effect
        if ((wheelL == StateWheel::MUD || (roadTerrain == 4 && !outsideLeftWheelRoad) || (showTerrain == 1 && terrain == 4))  && !crashing && speed > 0.f){
            sprite.setTexture(textures[170 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width),
                                j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        if ((wheelR == StateWheel::MUD || (roadTerrain == 4 && !outsideRightWheelRoad) || (showTerrain == 1 && terrain == 4))  && !crashing && speed > 0.f){
            sprite.setTexture(textures[170 + current_terrain_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }

        // Display the smoke effect (last one because has more priority)
        if (wheelL == StateWheel::SMOKE){
            sprite.setTexture(textures[148 + current_smoke_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                               j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        if (wheelR == StateWheel::SMOKE){
            sprite.setTexture(textures[148 + current_smoke_image], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
    }
    else {
        // Default sprites of the car to drawn depending on the elevation
        if (elevation == Elevation::FLAT)
            current_code_image = 1;
        else if (elevation == Elevation::UP)
            current_code_image = 21;
        else
            current_code_image = 41;

        // There is not wheel effects and sounds
        wheelL = wheelR = StateWheel::NORMAL;
        shutDownSounds();
    }
}



/**
 * Draw the ending animation of the car when the game round has finished
 * @param input is the module that has all the configuration of the game
 */
void PlayerCar::drawEndDriftRound(Input &input){

    // Check if all the textures of the car are correctly loaded
    if (textures.size() == PLAYER_TOTAL_TEXTURES){

        // Check the speed and the animation in order to start the drift effect
        if (speed < 60.f && !endAnimation){

            // Stop brake sound
            if (Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

            // Play drift sound
            if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
                Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, true);

            // Draw the car doing the drift effect
            int code = (speed >= 40.f) ? 146 : (speed <= 0.f) ? 121 : 147;
            sprite.setTexture(textures[code], true);
            sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
            float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
            sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f) + out);
            input.gameWindow.draw(sprite);

            // Get the code of the sprite to display the drift effect
            if (speed > 0.f){
                if (current_code_image < 148 || current_code_image > 150)
                    current_code_image = 148;
                else {
                    if (counter_code_image >= maxCounterToChange){
                        current_code_image++;
                        counter_code_image = 0;
                    }
                    else
                        counter_code_image++;
                }

                // Draw the smoke in the wheels to get a better quality of the effect

                // Left wheel
                const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;
                sprite.setTexture(textures[current_code_image], true);
                sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
                sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                               j - sprite.getGlobalBounds().height);
                input.gameWindow.draw(sprite);

                // Right wheel
                sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height);
                input.gameWindow.draw(sprite);
            }
            else
                // Car is still moving
                Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
        }
        else {
            // The car is moving straight until reach maximum speed (endAnimation = true)
            if (endAnimation){

                if (current_code_image < 0 || current_code_image > 2)
                    current_code_image = 0;
                else {
                    if (counter_code_image >= maxCounterToChange){
                        current_code_image++;
                        counter_code_image = 0;
                    }
                    else
                        counter_code_image++;
                }
            }
            else {

                // Reached maximum speed (endAnimation = false)

                // Starts to brake
                if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                    Audio::play(Sfx::FERRARI_ENGINE_BRAKE, true);

                // Display the effect of brake while is going straight
                if (speed >= 100.f){
                    if (current_code_image < 60 || current_code_image > 62)
                        current_code_image = 60;
                    else
                        if (counter_code_image >= maxCounterToChange){
                            current_code_image++;
                            counter_code_image = 0;
                            skidIndex = 148 + current_code_image % 4;
                        }
                        else
                            counter_code_image++;
                }
                // Brake and starts going to left to prepare the drift
                else if (speed >= 60.f && speed < 100.f){
                    if (current_code_image < 64 || current_code_image > 71)
                        current_code_image = 64;
                    else
                        if (counter_code_image >= maxCounterToChange){
                            if (current_code_image <= 70){
                                current_code_image++;
                                skidIndex = 148 + current_code_image % 4;
                            }
                            else {
                                if (skidIndex > 150)
                                    skidIndex = 148;
                                else
                                    skidIndex++;
                            }
                            counter_code_image = 0;
                        }
                        else
                            counter_code_image++;
                }
            }

            // Draw the car
            const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

            sprite.setTexture(textures[current_code_image], true);
            sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
            float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
            sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f));
            input.gameWindow.draw(sprite);

            // Draw the smoke in both wheels (left and right) because the car is braking
            if (!endAnimation){
                int code = (skidIndex == -1) ? 148 + current_code_image % 4 : skidIndex;
                sprite.setTexture(textures[code], true);
                sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
                sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                                   j - sprite.getGlobalBounds().height);
                input.gameWindow.draw(sprite);

                sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height);
                input.gameWindow.draw(sprite);
            }
        }
    }
}




/**
 * Stop all the player car sounds
 */
void PlayerCar::shutDownSounds(){
    Audio::stop(Sfx::FERRARI_ENGINE_START);
    Audio::stop(Sfx::FERRARI_ENGINE_RUN);
    Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
    Audio::stop(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE);
    Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);
    Audio::stop(Sfx::FERRARI_ENGINE_DRIFT);
    Audio::stop(Sfx::FERRARI_ENGINE_UP_GEAR);
    Audio::stop(Sfx::FERRARI_ENGINE_DOWN_GEAR);
}
