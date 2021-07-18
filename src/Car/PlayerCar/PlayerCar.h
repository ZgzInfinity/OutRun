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

#ifndef OUTRUN_PLAYER_HPP
#define OUTRUN_PLAYER_HPP


#include "../../Globals.h"
#include "../../Input/Input.h"
#include "../../Scene/Line/Line.h"


enum class Direction : int {
    TURNLEFT,
    FRONT,
    TURNRIGHT,
    __COUNT
};

// Actions that can be made with the vehicle
enum class Action : int {
    NONE,
    BRAKE,
    ACCELERATE,
    CRASH,
    BOOT,
    __COUNT
};

enum class Elevation : int {
    UP,
    FLAT,
    DOWN,
    __COUNT
};

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
class PlayerCar {

    private:

        float playerX;
        int playerY, playerZ;
        float speed, maxSpeed, lowAccel, highAccel;
        float thresholdX, varThresholdX, forceX;
        float collisionDir;

        Direction direction;
        Action action;
        Elevation elevation;

        // Vector with the textures of the vehicle
        vector<sf::Texture> textures;

        // Numeric code of the sprite to be drawn in the screen
        int current_code_image;


        int numTextures;

        // Counter to change actual_code_image
        int counter_code_image;

        // Minimum coefficient to draw the vehicle in the screen
        float minScreenX;

        // Maximum coefficient to draw the vehicle in the screen
        float maxScreenX;

        // Scale coefficient to be drawn on the screen depending of the resolutions
        float scale;

        // Counter to change the direction path of the vehicle
        int maxCounterToChange;

        // Scaling factor of the player's vehicle in axis y
        float scaleY;

        // Control if the player's vehicle has crashed more than one time
        bool firstCrash;


        bool firstTurnLeft, firstTurnRight, motorEngineSound, skidding;

        // Mode type of collision
        int mode;

        // Number of angers of the blonde woman
        int numAngers;


        float offsetCrash1;


        int playerW;


        StateWheel wheelL, wheelR;

        playerR playerMap;

        // Sprite to draw the texture of the player's vehicle in the screen
        sf::Sprite sprite;


public:



    /**
     * Initialize the player's vehicle
     * Default constructor
     */
    PlayerCar();

    float getSpeed() const;

    float getMaxSpeed() const;

    void setPlayerX(const float& pX);

    void setPlayerY(const int& pY);

    void setPlayerZ(const int& pZ);

    float getThresholdX() const;

    float getPlayerX() const;

    int getPlayerY() const;

    int getPlayerZ() const;

    void setPlayerMap(const playerR& playerRoad);

    playerR getPlayerMap() const;

    void setSkidding(const bool& skid);

    void setStateWheelLeft(const StateWheel& state);

    void setStateWheelRight(const StateWheel& state);

    StateWheel getStateWheelLeft() const;

    StateWheel getStateWheelRight() const;

    void accelerationControlAutomaic(Input& input, const float time);

    void elevationControl(const int& yWorld1, const int& yWorld2);

    void controlCentrifugalForce(const Line* playerLine, const float& time, const int& mapDistance);

    void checkCollision(Input& input, const Line* playerLine);

    bool hasCrashed(float x1, int w1, float x2, float w2, float scale);


    /**
     * Updates the player's vehicle sprite and draws it in the screen
     * @param c is the module configuration of the game
     * @param r is the sound player module of the game
     * @param a is the action to be done by the player's vehicle
     * @param d is the direction to be followed by the player's vehicle
     * @param e is the current elevation of the player's vehicle in the landscape
     * @param enableSound indicates if the motor of the player's vehicle has to make noise
     */
    void draw(Input& input, const bool& pauseMode, const bool& motorEngineSound = true);


};


#endif // OUTRUN_PLAYER_HPP
