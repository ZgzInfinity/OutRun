
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
 * Module Player interface file
 */

#ifndef PLAYER_CAR_H
#define PLAYER_CAR_H


#include "../../Globals.h"
#include "../../Input/Input.h"
#include "../../Scene/Line/Line.h"
#include "../Vehicle/Vehicle.h"

enum class StateWheel : int {
    NORMAL,
    SMOKE,
    SAND,
    __COUNT
};


enum class playerR : int {
	LEFTROAD,
	RIGHTROAD,
	__COUNT
};


/**
 * Represents a player as an available vehicle to be
 * chosen by the player in the vehicle selection menu
 */
class PlayerCar : public Vehicle {

    private:

        enum class Collision : int {
            TYPE_A,
            TYPE_B
        };

        float maxSpeed, lowAccel, brakeAccel, highAccel;
        float thresholdX, varThresholdX, forceX;
        float collisionDir, speedGear;

        Direction direction;
        Action action;
        Elevation elevation;

        bool firstTurnLeft, firstTurnRight, motorEngineSound, skidding, trafficCrash, drawCar,
             crashing, outsideRoad, angryWoman, increaseGear, decreaseGear, automaticMode, endAnimation;

        Collision modeCollision;

        int numAngers, out, counterOut;

        int playerW, gear;

        StateWheel wheelL, wheelR;

        playerR playerMap;

        sf::Sprite sprite;

public:

    PlayerCar();

    PlayerCar(const int _posX, const int _posY, const int _posZ, const float _speed, const int numTextures,
              const std::string& name, const bool _automaticMode);

    void setNumAngers();

    int getNumAngers() const;

    int getGear() const;

    float getMaxSpeed() const;

    float getThresholdX() const;

    void setPlayerMap(const playerR& playerRoad);

    void setDrawCar(const bool _drawCar);

    playerR getPlayerMap() const;

    void setLowAccel(const float& _lowAccel);

    float getLowAccel() const;

    bool getEndAnimation() const;

    void setCollisionDir();

    float getCollisionDir() const;

    void setCrashing(const bool& _crashing);

    bool getTrafficCrash() const;

    void setTrafficCrash();

    void setAngryWoman();

    void setOutsideRoad(const bool& _outsideRoad);

    bool getOutiseRoad() const;

    bool getCrashing() const;

    void setSkidding(const bool& skid);

    void setStateWheelLeft(const StateWheel& state);

    void setStateWheelRight(const StateWheel& state);

    StateWheel getStateWheelLeft() const;

    StateWheel getStateWheelRight() const;

    void accelerationControlAutomaic(Input& input, const float time);

    void elevationControl(const int& yWorld1, const int& yWorld2);

    void controlCentrifugalForce(const Line* playerLine, const float& time, const int& mapDistance);

    void checkCollisionSpriteInfo(Input& input, const Line* playerLine, bool& crashed, const SpriteInfo* sprite);

    void checkCollisionTrafficCar(Input& input, const Line* playerLine, const Line* trafficCarLine,
                                  const TrafficCar* c, bool& crashed);

    bool hasCrashed(float x1, int w1, float x2, float w2, float scale);


    void drawStartStaticRound(Input& input);


    void drawStartDriftRound(Input &input, float x, int& code);


    void drawPlayRound(Input& input, const bool& pauseMode, const bool& motorEngineSound = true);


};


#endif // PLAYER_CAR_H


