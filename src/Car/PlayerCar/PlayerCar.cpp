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
#include "PlayerCar.h"
#include "../../Random/Random.h"

using namespace std;
using namespace sf;

#define PLAYER_TEXTURES 170
#define MAX_INERTIA 10


PlayerCar::PlayerCar() : Vehicle(){}


PlayerCar::PlayerCar(const int _posX, const int _posY, const int _posZ, const float _speed,
                     const int numTextures, const std::string& name, const bool _automaticMode)
                     : Vehicle(_posX, _posY, _posZ, _speed, numTextures, name)
{
	playerW = 77;
	highAccel = 10.f;
	thresholdX = 1.f;
	varThresholdX = 0.06f;
	maxSpeed = 87.f;
	lowAccel = 15.f;
	brakeAccel = maxSpeed / 3.5f;
	direction = Direction::FRONT;
	collisionDir = 0.f;
	out = 0;

	gear = 0;
	current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;
	numAngers = 0;
	counterOut = 0;
	thresholdX = 0.f;
	speedGear = 0.f;
    playerMap = playerR::LEFTROAD;

	wheelL = StateWheel::NORMAL;
	wheelR = StateWheel::NORMAL;

    automaticMode = _automaticMode;
    crashing = false;
    firstTurnLeft = true;
    firstTurnRight = true;
    motorEngineSound = false;
    skidding = false;
    outsideRoad = false;
    angryWoman = false;
    increaseGear = false;
    decreaseGear = false;
}

void PlayerCar::setNumAngers(){
    numAngers = 0;
}

int PlayerCar::getNumAngers() const {
    return numAngers;
}

int PlayerCar::getGear() const {
    return gear;
}

float PlayerCar::getMaxSpeed() const {
    return maxSpeed;
}

float PlayerCar::getThresholdX() const {
    return thresholdX;
}

void PlayerCar::setPlayerMap(const playerR& playerRoad){
    playerMap = playerRoad;
}

playerR PlayerCar::getPlayerMap() const {
    return playerMap;
}

void PlayerCar::setLowAccel(const float& _lowAccel){
    lowAccel = _lowAccel;
}

float PlayerCar::getLowAccel() const {
    return lowAccel;
}

void PlayerCar::setCollisionDir(){
    collisionDir = 0.f;
}

float PlayerCar::getCollisionDir() const {
    return collisionDir;
}

void PlayerCar::setCrashing(const bool& _crashing){
    crashing = _crashing;
}

void PlayerCar::setAngryWoman(){
    angryWoman = false;
}

void PlayerCar::setOutsideRoad(const bool& _outsideRoad){
    outsideRoad = _outsideRoad;
}

bool PlayerCar::getOutiseRoad() const {
    return outsideRoad;
}

bool PlayerCar::getCrashing() const {
    return crashing;
}

void PlayerCar::setSkidding(const bool& skid){
    skidding = skid;
}

void PlayerCar::setStateWheelLeft(const StateWheel& state){
    wheelL = state;
}

void PlayerCar::setStateWheelRight(const StateWheel& state){
    wheelR = state;
}

StateWheel PlayerCar::getStateWheelLeft() const {
    return wheelL;
}

StateWheel PlayerCar::getStateWheelRight() const {
    return wheelR;
}


// Control the actions to accelerate or brake the vehicle of the player
void PlayerCar::accelerationControlAutomaic(Input& input, const float time){

    sf::Event event;
    action = Action::ACCELERATE;
    direction = Direction::FRONT;

    if (wheelL == StateWheel::SAND && wheelR == StateWheel::SAND)
		maxSpeed = 70.f;
	else
	{
		if (gear == 1)
			maxSpeed = (int)SEGMENT_LENGTH;
		else
			maxSpeed = 87.f;
	}

    if ((input.pressed(Key::BRAKE, event) || input.held(Key::BRAKE)) && speed > 0.f){

        speed -= brakeAccel * time;
		if (speed <= 0.f) {
			speed = 0.f;
			motorEngineSound = false;
			wheelL = StateWheel::NORMAL;
			wheelR = StateWheel::NORMAL;
		}

		if (gear == 1 && speed < 87.f){
            gear = 0;
            decreaseGear = true;
		}
		else if (decreaseGear)
            if (speed < 65.f)
                decreaseGear = false;

        wheelL = StateWheel::SMOKE;
        wheelR = StateWheel::SMOKE;
		action = (speed == 0.f) ? Action::NONE : Action::BRAKE;
    }
    else if ((input.pressed(Key::ACCELERATE, event) || input.held(Key::ACCELERATE)) && speed <= maxSpeed){
        if (speed <= maxSpeed){
            speed += lowAccel * time;
            if (speed > maxSpeed){
                speed = maxSpeed;
            }
        }
        else
            speed = maxSpeed;

		action = (speed > 15.f) ? Action::ACCELERATE : Action::BOOT;
		motorEngineSound = true;

		if (speed < 50.f && wheelL != StateWheel::SAND && wheelR != StateWheel::SAND){
			wheelR = StateWheel::SMOKE;
		}
		else {
            wheelR = StateWheel::NORMAL;
		}
        if (speed < 50.f && wheelR != StateWheel::SAND && wheelL != StateWheel::SAND){
            wheelL = StateWheel::SMOKE;
        }
        else {
            wheelL = StateWheel::NORMAL;
        }

        if (automaticMode){
            if (speed == maxSpeed && gear == 0){
                gear = 1;
                increaseGear = true;
            }
            if (increaseGear && speed > 97.f)
                increaseGear = false;
        }
        else {
            if ((input.pressed(Key::UP_GEAR, event) || input.held(Key::UP_GEAR) && gear != 1)){
                gear = 1;
                increaseGear = true;
                speedGear = speed;
            }
            else if ((input.pressed(Key::DOWN_GEAR, event) || input.held(Key::DOWN_GEAR)) && gear != 0){
                gear = 0;
                decreaseGear = true;

                if (speed > 87.f)
                    speed -= lowAccel * time;
            }
            if (increaseGear && (speed > speedGear + 10.f))
                increaseGear = false;
            else if (decreaseGear && speed < 77.f)
                decreaseGear = false;
        }
    }
    else {
		if (speed <= 0.f) {
			speed = 0.f;
			action = Action::NONE;
			motorEngineSound = false;
		}
		else
            speed -= (lowAccel * time * 0.75f);

		wheelL = StateWheel::NORMAL;
        wheelR = StateWheel::NORMAL;

        increaseGear = false;

        if (gear == 1 && speed < 87.f){
            gear = 0;
            decreaseGear = true;
		}
		else if (decreaseGear){
            if (speed < 77.f)
                decreaseGear = false;

            action = Action::BRAKE;
            wheelL = StateWheel::SMOKE;
            wheelR = StateWheel::SMOKE;
		}
    }

    varThresholdX = speed * time;

    //Check input for side
	if (input.pressed(Key::TURN_LEFT, event) || input.held(Key::TURN_LEFT))
	{
        direction = Direction::TURNLEFT;
	    if (thresholdX > -varThresholdX)
            thresholdX -= 0.15f;
        else if (thresholdX < -varThresholdX)
            thresholdX += 0.15f;
	}
	else if (input.pressed(Key::TURN_RIGHT, event) || input.held(Key::TURN_RIGHT))
	{
        direction = Direction::TURNRIGHT;
        if (thresholdX < varThresholdX)
            thresholdX += 0.15f;
        else if (thresholdX > varThresholdX)
            thresholdX -= 0.15f;
	}
	else {
        direction = Direction::FRONT;
        if (thresholdX != 0.f)
        {
            thresholdX -= thresholdX * 0.1f;
            if (thresholdX < 0.1f && thresholdX > -0.1f)
                thresholdX = 0.f;
        }
	}
    posX += (thresholdX * time);

    if (increaseGear){
        wheelL = StateWheel::SMOKE;
        wheelR = StateWheel::SMOKE;
    }
}


void PlayerCar::elevationControl(const int& yWorld1, const int& yWorld2){
    if (yWorld1 == yWorld2){
        elevation = Elevation::FLAT;
    }
    else if (yWorld1 < yWorld2){
        elevation = Elevation::UP;
    }
    else {
        elevation = Elevation::DOWN;
    }
}


void PlayerCar::controlCentrifugalForce(const Line* playerLine, const float& time, const int& mapDistance){
    //Apply centrifugal to curves
	float centrifugal = (speed > 26) ? 0.5f : 0.f;
	if (speed >= 70)
		centrifugal = (speed - 50.f) / 70.f;
	switch (playerMap)
	{
	case playerR::RIGHTROAD:
		//Displacement when player is in right road, and roads are changing distance between them
		if (mapDistance != playerLine->distance)
			posX += (playerLine->distance - mapDistance) / (float)ROAD_WIDTH;
		if (playerLine->mirror)
			posX += (playerLine->curve * MIN((speed / maxSpeed), 1.f) * centrifugal * time);
		else
			posX -= (playerLine->curve * MIN((speed / maxSpeed),1.f) * centrifugal * time);
		break;
	case playerR::LEFTROAD:
        posX -= (playerLine->curve * MIN((speed /maxSpeed),1.f) * centrifugal * time);
		break;
	}
}


void PlayerCar::checkCollisionSpriteInfo(Input& input, const Line* playerLine, bool& crashed, const bool& left){

    SpriteInfo* p;
    if (left)
        p = playerLine->spriteLeft;
    else
        p = playerLine->spriteRight;

    if (p->getCollider())
    {
        PointLine point = playerLine->p1;
        if (p->getSide())
            point = playerLine->p11;

        float x2 = point.xScreen + (p->getOffsetX() * point.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
        float scale = 1.6f * (0.3f * (1.f / 170.f)) * point.scale * input.gameWindow.getSize().x * ROAD_WIDTH * p->getScale();
        int width = p->getTextureSprite()->getSize().x;

        if (p->getOffsetX() >= 0)
            x2 += x2 + width * scale * p->getPivotColRight().x;

        else
            x2 = x2 - width * scale * (1 - p->getPivotColLeft().x);

        if (hasCrashed((int)(input.gameWindow.getSize().x / 2) + 5, playerW, x2, p->getWidthCol(), scale))
        {
            collisionDir = posX;
            crashing = true;
            crashed = true;
            modeCollision = static_cast<Collision>(random_int((int)Collision::TYPE_A,
                                                              (int)Collision::TYPE_B));
            if (speed <= 80 ) {
                lowAccel = speed / 2.0f;
            }
            else if (speed >= 80 && speed <= 150){
                lowAccel = speed / 0.8f;
            }
            else if (speed > 150 && speed <= 190){
                lowAccel = speed / 1.6f;
            }
            wheelL = StateWheel::SMOKE;
            wheelR = StateWheel::SMOKE;
            Audio::play(Sfx::FERRARI_CRASH, false);
            Audio::play(static_cast<Sfx>(random_int((int)Sfx::SPECTATORS_FIRST_SHOUT,
                                                    (int)Sfx::SPECTATORS_EIGHTH_SHOUT)), false);
        }
    }
}



void PlayerCar::checkCollisionTrafficCar(Input& input, const Line* playerLine, const Line* trafficCarLine,
                                          const TrafficCar* c, bool& crashed)
{
    if (trafficCarLine->index == playerLine->index)
    {
        PointLine point = playerLine->p1;
        PointLine point2 = playerLine->p2;
        if (c->getSide())
        {
            point = playerLine->p11;
            point2 = playerLine->p21;
        }

        float perc = (float)(((int)(c->getPosZ()) % (int)SEGMENT_LENGTH) / (float)SEGMENT_LENGTH);
        float scaleOffset = point.scale + (point2.scale - point.scale)*perc;
        float xOffset = point.xScreen + (point2.xScreen - point.xScreen)*perc;
        float yOffset = point.yScreen + (point2.yScreen - point.yScreen)*perc;

        float x2 = xOffset + (c->getOffset() * scaleOffset * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
        float scale = 1.6f * (0.3f * (1.f / 170.f)) * scaleOffset * input.gameWindow.getSize().x * ROAD_WIDTH * 1.2f;

        if (hasCrashed((float)(input.gameWindow.getSize().x / 2) + 5.f, playerW, x2,
                        (float)c->getTexture().getSize().x, scale))
        {
            collisionDir = posX;
            crashing = true;
            crashed = true;
            modeCollision = static_cast<Collision>(random_int((int)Collision::TYPE_A, (int)Collision::TYPE_B));
            if (speed <= 80 ) {
                lowAccel = speed / 2.0f;
            }
            else if (speed >= 80 && speed <= 150){
                lowAccel = speed / 0.8f;
            }
            else if (speed > 150 && speed <= 190){
                lowAccel = speed / 1.6f;
            }
            wheelL = StateWheel::SMOKE;
            wheelR = StateWheel::SMOKE;
            Audio::play(Sfx::FERRARI_CRASH, false);
        }
    }
}


bool PlayerCar::hasCrashed(float x1, int w1, float x2, float w2, float scale){

	float min1 = x1 - (((float)w1*3.2f) / 2.f);
	float max1 = x1 + (((float)w1*3.2f) / 2.f);

	float min2 = x2 - ((w2*scale) / 2.f);
	float max2 = x2 + ((w2*scale) / 2.f);

	return (max1 >= min2 && max2 >= min1);
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
void PlayerCar::draw(Input& input, const bool& pauseMode, const bool& motorEngineSound) {

    // Control the sounds of the devastator
    if (motorEngineSound) {
        if (speed > 0.0f) {
            if (action == Action::BOOT) {
                // Acceleration sound
                if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_START)) {
                    Audio::play(Sfx::FERRARI_ENGINE_START, false);
                }
            }
            else if (action == Action::ACCELERATE){

                Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

                if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN))
                    Audio::play(Sfx::FERRARI_ENGINE_RUN, true);
            }
            else if (action == Action::BRAKE){
                if (!decreaseGear)
                    Audio::stop(Sfx::FERRARI_ENGINE_RUN);

                if (!decreaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                    Audio::play(Sfx::FERRARI_ENGINE_BRAKE, true);
            }


            if (skidding && !Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING)) {
                Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, true);
            }
            else if (!skidding && Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING)) {
                Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
            }

            if (outsideRoad && !Audio::isPlaying(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE)) {
                Audio::play(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE, true);
            }
            else if (!outsideRoad && Audio::isPlaying(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE)){
                Audio::stop(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE);
            }
        }
        else {
            Audio::stop(Sfx::FERRARI_ENGINE_START);
            Audio::stop(Sfx::FERRARI_ENGINE_RUN);
            Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
            Audio::stop(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE);
            Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);
            Audio::stop(Sfx::FERRARI_ENGINE_DRIFT);
            Audio::stop(Sfx::FERRARI_ENGINE_UP_GEAR);
            Audio::stop(Sfx::FERRARI_ENGINE_DOWN_GEAR);
        }

        if (crashing && speed > 0.f) {
            if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_DRIFT))
                Audio::play(Sfx::FERRARI_ENGINE_DRIFT, true);
        }
        else {
            Audio::stop(Sfx::FERRARI_ENGINE_DRIFT);
        }

        if (crashing){
            action = Action::CRASH;

            if (Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN))
                Audio::stop(Sfx::FERRARI_ENGINE_RUN);

            if (Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

            if (angryWoman && numAngers == 1)
                Audio::play(Sfx::BLOND_WOMAN_DIE, false);
        }

        if (increaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_UP_GEAR))
            Audio::play(Sfx::FERRARI_ENGINE_UP_GEAR, false);
        else if (decreaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_DOWN_GEAR))
            Audio::play(Sfx::FERRARI_ENGINE_DOWN_GEAR, false);

    }
    else {
        Audio::stop(Sfx::FERRARI_ENGINE_START);
        Audio::stop(Sfx::FERRARI_ENGINE_RUN);
        Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
        Audio::stop(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE);
        Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);
        Audio::stop(Sfx::FERRARI_ENGINE_DRIFT);
        Audio::stop(Sfx::FERRARI_ENGINE_UP_GEAR);
        Audio::stop(Sfx::FERRARI_ENGINE_DOWN_GEAR);
    }

    if (direction != Direction::TURNLEFT){
        firstTurnLeft = true;
    }
    if (direction != Direction::TURNRIGHT){
        firstTurnRight = true;
    }

    // Check the current action of the devastator to be drawn in the screen
    if (action != Action::NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            // Increment the texture counter only if it moves
            if ((!pauseMode) && (speed > 0.0f || speed <= 0.0f && numAngers < 3))
                current_code_image++;

            if (textures.size() == PLAYER_TEXTURES){
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
                    // The devastator is crashing
                    if (modeCollision == Collision::TYPE_A) {
                        // First type of animation
                        if (speed <= 0.f){

                            if (numAngers < 3){
                                if (current_code_image < 129 || current_code_image > 133){
                                    current_code_image = 129;
                                }
                                else {
                                    if (current_code_image == 133){
                                        numAngers++;

                                        if (numAngers == 1)
                                            angryWoman = true;
                                    }
                                 }
                            }

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
                                 if (current_code_image < 142 || current_code_image > 146){
                                    current_code_image = 142;
                                 }
                                 else {
                                    if (current_code_image == 146){
                                        numAngers++;

                                        if (numAngers == 1)
                                            angryWoman = true;
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
            counter_code_image++;
        }
    }
    else {
        if (elevation == Elevation::FLAT) {
            current_code_image = 1;
        }
        else if (elevation == Elevation::UP){
            current_code_image = 21;
        }
        else {
            current_code_image = 41;
        }
    }

    if (outsideRoad && speed > 0.f){
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

    sprite.setTexture(textures[current_code_image - 1], true);
    sprite.setScale(3.2f * input.screenScaleX, 3.2f * input.screenScaleY);
    float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f) + out);
    input.gameWindow.draw(sprite);

    if (crashing) {
        if (speed <= 0.f){
            maxCounterToChange = 6;
        }
        else if (speed > 0.f && speed < 20.f){
            maxCounterToChange = 5;
        }
        else if (speed >= 20.f && speed < 60.f){
            maxCounterToChange = 4;
        }
        else if (speed >= 60.f && speed < 100.f){
            maxCounterToChange = 3;
        }
    }
    else {
         maxCounterToChange = 2;
    }

    const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

    if (wheelL == StateWheel::SMOKE){
        sprite.setTexture(textures[146 + current_code_image % 4], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                           j - sprite.getGlobalBounds().height + out);
        input.gameWindow.draw(sprite);
    }
    else if (wheelL == StateWheel::SAND){
        sprite.setTexture(textures[150 + current_code_image % 4], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width),
                            j - sprite.getGlobalBounds().height + out);
        input.gameWindow.draw(sprite);
    }

    if (wheelR == StateWheel::SMOKE){
        sprite.setTexture(textures[146 + current_code_image % 4], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height + out);
        input.gameWindow.draw(sprite);
    }
    else if (wheelR == StateWheel::SAND){
        sprite.setTexture(textures[150 + current_code_image % 4], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height + out);
        input.gameWindow.draw(sprite);
    }

}
