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
                     const int numTextures, const std::string& name)
                     : Vehicle(_posX, _posY, _posZ, _speed, numTextures, name)
{
	playerW = 77;
	highAccel = 10.f;
	thresholdX = 1.f;
	varThresholdX = 0.06f;
	maxSpeed = 300.f;
	lowAccel = maxSpeed / 6.5f;
	forceX = 0.3f;
	direction = Direction::FRONT;

	offsetCrash1 = 0.f;
	collisionDir = 0.f;
	minScreenX = 0;
	maxScreenX = 0;
	current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;
	scale = 3.2f;
	scaleY = 3.2f;
	numAngers = 0;
	thresholdX = 0.f;
    playerMap = playerR::LEFTROAD;

	wheelL = StateWheel::NORMAL;
	wheelR = StateWheel::NORMAL;

    firstCrash = true;
    firstTurnLeft = true;
    firstTurnRight = true;
    motorEngineSound = false;
    skidding = false;
}

float PlayerCar::getThresholdX() const{
    return thresholdX;
}

void PlayerCar::setPlayerMap(const playerR& playerRoad){
    playerMap = playerRoad;
}

playerR PlayerCar::getPlayerMap() const {
    return playerMap;
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

    if ((input.pressed(Key::ACCELERATE, event) || input.held(Key::ACCELERATE)) && speed < maxSpeed){
        speed += lowAccel * time;
		if (speed > maxSpeed){
			speed = maxSpeed;
		}
		action = (speed > 20.f) ? Action::ACCELERATE : Action::BOOT;
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
    }
    else if ((input.pressed(Key::BRAKE, event) || input.held(Key::BRAKE)) && speed > 0.f){
        speed -= lowAccel * time;
		if (speed < 0.f) {
			speed = 0.f;
			motorEngineSound = false;
			wheelL = StateWheel::NORMAL;
			wheelR = StateWheel::NORMAL;
		}
		action = (speed == 0.f) ? Action::NONE : Action::BRAKE;
    }
    else {
        speed -= lowAccel * time;
		if (speed < 0.f) {
			speed = 0.f;
			action = Action::NONE;
			motorEngineSound = false;
            wheelL = StateWheel::NORMAL;
			wheelR = StateWheel::NORMAL;
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


void PlayerCar::checkCollisionProps(Input& input, const Line* playerLine, bool& crashed){
    for (unsigned int n = 0; !crashed && n < playerLine->lineProps.size(); ++n) {
		Prop* p = playerLine->lineProps[n];

		if (p->collider)
		{
			PointLine point = playerLine->p1;
			if (playerLine->sides[n])
				point = playerLine->p11;

			float x2 = point.xScreen + (playerLine->offsetsX[n] * point.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
			float scale = 1.6f * (0.3f * (1.f / 170.f)) * point.scale * input.gameWindow.getSize().x * ROAD_WIDTH * p->scale;

			if (playerLine->offsetsX[n] >= 0)
				x2 = x2 + p->animRight.getSize().x * scale * p->pivotColR.x;

			else
				x2 = x2 - p->animRight.getSize().x * scale * (1 - p->pivotColL.x);

			if (hasCrashed((int)(input.gameWindow.getSize().x / 2) + 5, playerW, x2, p->wCol, scale))
			{
				collisionDir = posX;
				if (action != Action::CRASH){
                    action = Action::CRASH;
                    crashed = true;
				}
			}
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
            crashed = true;
        }
    }
}


bool PlayerCar::hasCrashed(float x1, int w1, float x2, float w2, float scale){
	//Player offsetX at 0.5
	float min1 = x1 - (((float)w1*3.2f) / 2.f);
	float max1 = x1 + (((float)w1*3.2f) / 2.f);

	float min2 = x2 - ((w2*scale) / 2.f);
	float max2 = x2 + ((w2*scale) / 2.f);

	// App->renderer->DrawQuad({ (int)min1, 60, (int)(max1 - min1), 90 }, 255, 0, 0, 255, false);
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
            else {
                if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN)) {
                    Audio::play(Sfx::FERRARI_ENGINE_RUN, false);
                }
            }
            if (skidding && !Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING)) {
                Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, false);
            }
        }
        else {
            Audio::stop(Sfx::FERRARI_ENGINE_START);
            Audio::stop(Sfx::FERRARI_ENGINE_RUN);
            Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
        }
    }
    else {
        Audio::stop(Sfx::FERRARI_ENGINE_START);
        Audio::stop(Sfx::FERRARI_ENGINE_RUN);
        Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
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
                                current_code_image = 104;
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

    sprite.setTexture(textures[current_code_image - 1], true);
    sprite.setScale(scale * input.screenScaleX, scaleY * input.screenScaleY);
    minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    maxScreenX = minScreenX + sprite.getGlobalBounds().width;
    sprite.setPosition(minScreenX, ((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f);
    input.gameWindow.draw(sprite);

    if (wheelL == StateWheel::SMOKE){
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;
        sprite.setTexture(textures[146 + current_code_image % 4], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                           j - sprite.getGlobalBounds().height);
        input.gameWindow.draw(sprite);
    }
    if (wheelR == StateWheel::SMOKE){
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;
        sprite.setTexture(textures[146 + current_code_image % 4], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height);
        input.gameWindow.draw(sprite);
    }
}
