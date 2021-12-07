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

PlayerCar::PlayerCar() : Vehicle(){}


PlayerCar::PlayerCar(const int _posX, const int _posY, const int _posZ, const float _speed, const float _scale,
                     const int numTextures, const std::string& name, const bool _automaticMode)
                     : Vehicle(_posX, _posY, _posZ, _speed, _scale, numTextures, name)
{
	playerW = 77;
	highAccel = 10.f;
	varThresholdX = 0.03f;
	maxLowSpeed = maxHighSpeed = 100.f;
	lowAccel = maxHighSpeed / 6.5f;
	brakeAccel = maxHighSpeed / 3.0f;
	maxHighSpeed = 75.f;
	direction = Direction::FRONT;
	collisionDir = 0.f;
	out = 0;
    skidIndex = -1;
	gear = 0;
	current_code_image = 1;
	maxCounterToChange = 2;
	counter_code_image = 0;
	numAngers = 0;
	counterOut = 0;
	thresholdX = 0.f;
	speedGear = 0.f;
    playerMap = playerR::LEFTROAD;

	wheelL = StateWheel::SMOKE;
	wheelR = StateWheel::SMOKE;
	elevation = Elevation::FLAT;

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
    trafficCrash = false;
    drawCar = true;
    endAnimation = false;
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

float PlayerCar::getHighMaxSpeed() const {
    return maxHighSpeed;
}

float PlayerCar::getLowMaxSpeed() const {
    return maxLowSpeed;
}

float PlayerCar::getThresholdX() const {
    return thresholdX;
}

void PlayerCar::setPlayerMap(const playerR& playerRoad){
    playerMap = playerRoad;
}

void PlayerCar::setDrawCar(const bool _drawCar){
    drawCar = _drawCar;
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

void PlayerCar::setEndAnimation(const bool _endAnimation){
    endAnimation = _endAnimation;
}

bool PlayerCar::getEndAnimation() const {
    return endAnimation;
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

bool PlayerCar::getTrafficCrash() const {
    return trafficCrash;
}

void PlayerCar::setTrafficCrash(){
    trafficCrash = false;
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
void PlayerCar::accelerationControl(Input& input, const State gameStatus, const float time){

    sf::Event event;
    action = Action::ACCELERATE;
    direction = Direction::FRONT;

    switch(gameStatus){
        case State::PLAY_ROUND:
            if (wheelL == StateWheel::SAND && wheelR == StateWheel::SAND)
                maxHighSpeed = 75.f;
            else
            {
                if (gear == 1)
                    maxHighSpeed = (int)SEGMENT_LENGTH;
                else
                    maxHighSpeed = 100.f;
            }

            if ((input.pressed(Key::BRAKE, event) || input.held(Key::BRAKE)) && speed > 0.f){

                speed -= brakeAccel * time;
                if (speed <= 0.f) {
                    speed = 0.f;
                    motorEngineSound = false;
                    wheelL = wheelR = StateWheel::NORMAL;
                }

                if (gear == 1 && speed < 100.f){
                    gear = 0;
                    decreaseGear = true;
                }
                else if (decreaseGear)
                    if (speed < 90.f)
                        decreaseGear = false;

                wheelL = wheelR = StateWheel::SMOKE;
                action = (speed == 0.f) ? Action::NONE : Action::BRAKE;
            }
            else if ((input.pressed(Key::ACCELERATE, event) || input.held(Key::ACCELERATE)) && speed <= maxHighSpeed){
                if (speed <= maxHighSpeed){
                    speed += lowAccel * time;
                    if (speed > maxHighSpeed){
                        speed = maxHighSpeed;
                    }
                }
                else
                    speed = maxHighSpeed;

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
                    if (speed == maxHighSpeed && gear == 0){
                        gear = 1;
                        increaseGear = true;
                    }
                    if (increaseGear && speed > 110.f)
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
                if (speed <= 0.f) {
                    speed = 0.f;
                    action = Action::NONE;
                    motorEngineSound = false;
                }
                else
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

            if (increaseGear)
                wheelL = wheelR = StateWheel::SMOKE;

            break;
        case State::END_ROUND:
            if (endAnimation){
                if (!gear)
                    gear = true;

                maxHighSpeed = (int)SEGMENT_LENGTH;
                speed += lowAccel * time;
                if (speed > maxHighSpeed)
                    speed = maxHighSpeed;

                wheelL = wheelR = StateWheel::NORMAL;

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
                action = Action::BRAKE;

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
	float centrifugal = (speed > 26) ? 0.5f : 0.f;
	if (speed >= 100.f)
		centrifugal = (speed - 50.f) / 90.f;
	switch (playerMap){
        case playerR::RIGHTROAD:
            if (mapDistance != playerLine->distance)
                posX += (playerLine->distance - mapDistance) / (float)ROAD_WIDTH;
            if (playerLine->mirror)
                posX += (playerLine->curve * MIN((speed / maxHighSpeed), 1.f) * centrifugal * time);
            else
                posX -= (playerLine->curve * MIN((speed / maxHighSpeed), 1.f) * centrifugal * time);
            break;
        case playerR::LEFTROAD:
            posX -= (playerLine->curve * MIN((speed / maxHighSpeed), 1.f) * centrifugal * time);
	}
}


void PlayerCar::checkCollisionSpriteInfo(Input& input, const Line* playerLine, bool& crashed, const SpriteInfo* sprite){

    if (sprite->getCollider())
    {
        PointLine point = playerLine->p1;
        if (sprite->getSide())
            point = playerLine->p11;

        float x2 = point.xScreen + (sprite->getOffsetX() * point.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
        float scale = 1.6f * (0.3f * (1.f / 170.f)) * point.scale * input.gameWindow.getSize().x * ROAD_WIDTH * sprite->getScale();
        int width = sprite->getTextureSprite()->getSize().x;

        if (sprite->getOffsetX() >= 0)
            x2 = x2 + width * scale * sprite->getPivotColRight().x;

        else
            x2 = x2 - width * scale * (1 - sprite->getPivotColLeft().x);

        if (hasCrashed(((float) input.gameWindow.getSize().x) / 2.0f + 5,
                       playerW, x2, sprite->getWidthCol(), scale))
        {
            collisionDir = posX;
            crashing = true;
            crashed = true;
            trafficCrash = false;
            modeCollision = static_cast<Collision>(random_int((int)Collision::TYPE_A,
                                                              (int)Collision::TYPE_B));
            if (speed <= 80 )
                lowAccel = speed / 2.0f;
            else if (speed >= 80 && speed <= 150)
                lowAccel = speed / 0.8f;
            else if (speed > 150 && speed <= 190)
                lowAccel = speed / 1.6f;

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
            trafficCrash = true;
            modeCollision = static_cast<Collision>(random_int((int)Collision::TYPE_A, (int)Collision::TYPE_B));

            if (speed <= 80 )
                lowAccel = speed / 2.0f;
            else if (speed >= 80 && speed <= 150)
                lowAccel = speed / 0.8f;
            else if (speed > 150 && speed <= 190)
                lowAccel = speed / 1.6f;

            wheelL = StateWheel::SMOKE;
            wheelR = StateWheel::SMOKE;
            Audio::play(Sfx::FERRARI_CRASH, false);
            Audio::play(static_cast<Sfx>(random_int((int)Sfx::SPECTATORS_FIRST_SHOUT,
                                                    (int)Sfx::SPECTATORS_EIGHTH_SHOUT)), false);
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


void PlayerCar::drawStartStaticRound(Input& input) {
    if (textures.size() == PLAYER_TEXTURES) {
        float x = input.gameWindow.getSize().x / 2;
        sprite.setTexture(textures[121], true);
        sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
        sprite.setPosition(x, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f));
        input.gameWindow.draw(sprite);
    }
}

void PlayerCar::drawStartDriftRound(Input &input, float x, int& code){

    if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
        Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, false);

    if (textures.size() == PLAYER_TEXTURES){
        if (counter_code_image >= maxCounterToChange){
            current_code_image++;
            counter_code_image = 0;
        }
        if (current_code_image < 148 || current_code_image > 151)
            current_code_image = 148;

        float width = input.gameWindow.getSize().x;
        endAnimation = x < width * 0.4f || x >= width;
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

        sprite.setTexture(textures[code], true);
        sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
        sprite.setPosition(x, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f));
        input.gameWindow.draw(sprite);

        sprite.setTexture(textures[current_code_image], true);
        sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
        sprite.setPosition(x, j - sprite.getGlobalBounds().height);
        input.gameWindow.draw(sprite);

        float offsetSkid = (code > 120) ? 1.6f : 1.5f;
        sprite.setPosition(x * offsetSkid, j - sprite.getGlobalBounds().height);
        input.gameWindow.draw(sprite);

        if (endAnimation) {
            current_code_image = 1;
            counter_code_image = 0;
            wheelL == StateWheel::NORMAL;
            wheelR == StateWheel::NORMAL;
        }
        else
            counter_code_image++;
    }
}

void PlayerCar::drawPlayRound(Input& input, const bool& pauseMode, const bool& motorEngineSound) {

    if (drawCar){
        if (motorEngineSound) {
            if (speed > 0.0f) {
                if (action == Action::BOOT) {
                    // Acceleration sound
                    if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_START))
                        Audio::play(Sfx::FERRARI_ENGINE_START, false);
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


                if (skidding && !Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
                    Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, true);
                else if (!skidding && Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
                    Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);

                if (outsideRoad && !Audio::isPlaying(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE))
                    Audio::play(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE, true);
                else if (!outsideRoad && Audio::isPlaying(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE))
                    Audio::stop(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE);
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
            else
                Audio::stop(Sfx::FERRARI_ENGINE_DRIFT);

            if (crashing){
                action = Action::CRASH;
                outsideRoad = false;

                if (speed > 0.f){
                    wheelL = StateWheel::SMOKE;
                    wheelR = StateWheel::SMOKE;
                }

                if (Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN))
                    Audio::stop(Sfx::FERRARI_ENGINE_RUN);

                if (Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                    Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

                if (angryWoman && numAngers == 1)
                    Audio::play(Sfx::BLOND_WOMAN_DIE, false);
            }
            else {
                if (speed <= 0.f) {
                    wheelL = StateWheel::NORMAL;
                    wheelR = StateWheel::NORMAL;
                }
            }

            if (increaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_UP_GEAR))
                Audio::play(Sfx::FERRARI_ENGINE_UP_GEAR, false);
            else if (!increaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_UP_GEAR))
                Audio::stop(Sfx::FERRARI_ENGINE_UP_GEAR);

            if (decreaseGear && !Audio::isPlaying(Sfx::FERRARI_ENGINE_DOWN_GEAR))
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

        if (direction != Direction::TURNLEFT)
            firstTurnLeft = true;
        if (direction != Direction::TURNRIGHT)
            firstTurnRight = true;

        // Check the current action of the devastator to be drawn in the screen
        if (action != Action::NONE) {
            if (counter_code_image >= maxCounterToChange) {
                counter_code_image = 0;

                // Increment the texture counter only if it moves
                if ((!pauseMode) && (speed > 0.f || speed <= 0.f && numAngers < 3)){
                    current_code_image++;
                }

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
        sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
        float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
        sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f) + out);
        input.gameWindow.draw(sprite);

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

        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

        if (wheelL == StateWheel::SMOKE){
            sprite.setTexture(textures[148 + current_code_image % 4], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                               j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        else if (wheelL == StateWheel::SAND){
            sprite.setTexture(textures[152 + current_code_image % 4], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width),
                                j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }

        if (wheelR == StateWheel::SMOKE){
            sprite.setTexture(textures[148 + current_code_image % 4], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
        else if (wheelR == StateWheel::SAND){
            sprite.setTexture(textures[152 + current_code_image % 4], true);
            sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
            sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f, j - sprite.getGlobalBounds().height + out);
            input.gameWindow.draw(sprite);
        }
    }
    else {
        if (elevation == Elevation::FLAT)
            current_code_image = 1;
        else if (elevation == Elevation::UP)
            current_code_image = 21;
        else
            current_code_image = 41;

        wheelL = wheelR = StateWheel::NORMAL;
    }
}


void PlayerCar::drawEndDriftRound(Input &input){

    if (Audio::isPlaying(Sfx::FERRARI_ENGINE_RUN))
        Audio::stop(Sfx::FERRARI_ENGINE_RUN);

    if (textures.size() == PLAYER_TEXTURES){
        if (speed < 60.f){

            if (Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                Audio::stop(Sfx::FERRARI_ENGINE_BRAKE);

            if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_SKIDDING))
                Audio::play(Sfx::FERRARI_ENGINE_SKIDDING, true);

            int code = (speed >= 40.f) ? 146 : (speed <= 0.f) ? 121 : 147;
            sprite.setTexture(textures[code], true);
            sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
            float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
            sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f) + out);
            input.gameWindow.draw(sprite);

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
                const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;
                sprite.setTexture(textures[current_code_image], true);
                sprite.setScale(2.5f * input.screenScaleX, 2.5f * input.screenScaleX);
                sprite.setPosition(((float) input.gameWindow.getSize().x) / 2.0f - (sprite.getGlobalBounds().width * 1.2f),
                               j - sprite.getGlobalBounds().height);
                input.gameWindow.draw(sprite);

                sprite.setPosition(((float) input.gameWindow.getSize().x) / 1.9f, j - sprite.getGlobalBounds().height);
                input.gameWindow.draw(sprite);
            }
            else
                Audio::stop(Sfx::FERRARI_ENGINE_SKIDDING);
        }
        else {
            if (!Audio::isPlaying(Sfx::FERRARI_ENGINE_BRAKE))
                Audio::play(Sfx::FERRARI_ENGINE_BRAKE, true);

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

            const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;

            sprite.setTexture(textures[current_code_image], true);
            sprite.setScale(scale * input.screenScaleX, scale * input.screenScaleY);
            float minScreenX = ((float) input.gameWindow.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
            sprite.setPosition(minScreenX, (((float) input.gameWindow.getSize().y) * input.camD - sprite.getGlobalBounds().height / 4.f));
            input.gameWindow.draw(sprite);

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

