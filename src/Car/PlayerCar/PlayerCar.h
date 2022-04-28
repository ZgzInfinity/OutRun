
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
 * Interface file of the module PlayerCar
 */

#pragma once

#ifndef PLAYER_CAR_H
#define PLAYER_CAR_H

#include "../TrafficCar/TrafficCar.h"
#include "../Vehicle/Vehicle.h"
#include "../../Globals.h"
#include "../../Input/Input.h"
#include "../../Scene/Line/Line.h"
#include "../../Menus/Menu/Menu.h"

class TrafficCar;
class Line;



/**
 * Represents the different types of terrain
 * that can be drawn by the wheels of the car
 */
enum class StateWheel : int {
    NORMAL,
    SMOKE,
    SAND,
    GRASS,
    SNOW,
    MUD,
    __COUNT
};



/**
 * Represents rhe two types of road
 * by which the player can drive
 */
enum class Player_Road : int {
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

        /*
         * Types of collision
         * Type_A rotates from left to right
         * Type_B rotates from right to left
         */
        enum class Collision : int {
            LEFT_TO_RIGHT,
            RIGHT_TO_LEFT
        };

        // Maximum speed that the car can reach (depends of the gear used)
        float maxHighSpeed;

        // Maximum speed that the car can reach with the down gear
        float maxLowSpeed;

        // Acceleration of the car
        float lowAccel;

        // Deceleration of the car
        float brakeAccel;

        // total capability of turn for the car depending on the angle of the wheels
        float thresholdX;

        // Angle of turn for the wheels of the car
        float varThresholdX;

        // Destination point in axis X where the car has to finish the collision animation
        float collisionDir;

        // Reference speed to play sound of up or down gear the car changes it
        float speedGear;

        // Curvature of the road when the car crashes
        float collisionCurve;

        // Direction of the car
        Direction direction;

        // Status of the car
        Action action;

        // Control if the car has started to turn left
        bool firstTurnLeft;

        // Control if the car has started to turn right
        bool firstTurnRight;

        // Control if the sounds of the car have to be played
        bool motorEngineSound;

        // Checks if the player car is skidding or not
        bool skidding;

        // Controls if the car has crashed against a traffic car
        bool trafficCrash;

        // Check if the car must be drawn in the screen
        bool drawCar;

        // Control if the car has crashed recently or is drawing the collision animation
        bool crashing;

        // Check the status of the left wheel (inside or outside the road)
        bool outsideLeftWheelRoad;

        // Check the status of the right wheel (inside or outside the road)
        bool outsideRightWheelRoad;

        // Control if the blond woman has to shout to the driver
        bool angryWoman;

        // Detects when the car uses the up gear
        bool increaseGear;

        // Detects when the car uses the down gear
        bool decreaseGear;

        // Control if the car drives with automatic or manual gear changing
        bool automaticMode;

        // Control if the car is drawing the starting of ending animations
        bool endAnimation;

        // Type of collision that has to be reproduced
        Collision modeCollision;

        // Number of angers of the blond woman to be down when the player crashes
        int numAngers;

        // Control if the car is or out the road in order to be drawn with a bit of buzz
        int out;

        // Time to maintain the buzz when the player is out the road
        int counterOut;

        // Type of terrain of the road
        int roadTerrain;

        // Type of terrain outside the road
        int terrain;

        // Time between the reproduction of the traffic car sound when the player is near the traffic cars
        int soundTrafficCounter;

        // Number of laps to be done during the collision animation
        int numberCollisionLaps;

        // Width of the player car
        int playerW;

        // Gear of the car
        int gear;

        // Index of the skid car textures to be drawn during the end drift animation at the end of the game
        int skidIndex;

        // Control if there is terrain in the road
        int showTerrain;

        // Index of the terrain sprite to be drawn with the effect wheels
        int current_terrain_image;

        // Index of the smoke sprite to be drawn with the effect wheels
        int current_smoke_image;

        // Status of the left wheel
        StateWheel wheelL;

        // Status of the right wheel
        StateWheel wheelR;

        // Road where the player car is driving (left or right)
        Player_Road playerRoad;

        // Sprite to drawn the textures of the car
        sf::Sprite sprite;

public:



    /**
     * Default constructor
     */
    PlayerCar();



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
    PlayerCar(const int _posX, const int _posY, const int _posZ, const float _speed, const float _scale,
              const std::string _name, const bool _automaticMode, const bool _isTrafficCar, const int _roadTerrain);


    /**
     * Set the acceleration of the car
     * @param _lowAccel is the new acceleration of the car
     */
    void setLowAccel(const float _lowAccel);



    /**
     * Set the gear of the car to the down position
     */
    void setGear();



    /**
     * Set the type of road (left or right where the
     * player car is driving
     * @param playerRoad is the road where the car is driving
     */
    void setPlayerRoad(const Player_Road _playerRoad);



    /**
     * Set if the player car is currently crashing or it is playing the
     * collision animation
     * @param _crashing controls if the player has crashed or it is playing
     * the collision animation due to it crashed before
     */
    void setCrashing(const bool _crashing);



    /**
     * Set that there is not crash with any traffic car
     */
    void setTrafficCrash();



    /**
     * Set the direction where the player has to crash before
     * in order to let more collisions
     */
    void setCollisionDir();



    /**
     * Reset the curve coefficient where the player has crashed before
     * in order to let more collisions
     */
    void setCollisionCurve();



    /**
     * Set if the player car has to be drawn or not in the screen
     * Deactivated during the relocation of the car in the center of the nearest road
     * after finishing the collision animation
     * @param _drawCar controls if the car has to be drawn or not
     */
    void setDrawCar(const bool _drawCar);



    /**
     * Set that the woman has not to play the collision shout at the end
     */
    void setAngryWoman();



    /**
     * Set the total number of angers done by the blond woman
     * when the player car crashes against something (0..3)
     */
    void setNumAngers();



    /**
     * Set the type of terrain outside the road to display the correct effect by the wheels
     * @param _terrain is the type of terrain outside the road
     */
    void setTerrain(const int _terrain);



    /**
     * Set the type of terrain of the road to display the correct effect by the wheels
     * @param _roadTerrain is the type of terrain of the road
     */
    void setRoadTerrain(const int _roadTerrain);



    /**
     * Set the showTerrain effect to deactivated
     * Any kind of wheel effect is drawn when the car is in the road
     * unless there is terrain inside it
     */
    void setShowTerrain();



    /**
     * Set the status of the left wheel of the car (outside or inside the road)
     * @param _outsideLeftWheelRoad is the status of the left wheel
     */
    void setOutsideLeftWheelRoad(const bool _outsideLeftWheelRoad);



    /**
     * Set the status of the right wheel of the car (outside or inside the road)
     * @param _outsideLeftWheelRoad is the status of the right wheel
     */
    void setOutsideRightWheelRoad(const bool _outsideRightWheelRoad);



    /**
     * Set the status of the left wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
     * @param _state is the status of the left wheel
     */
    void setStateWheelLeft(const StateWheel _state);



    /**
     * Set the status of the right wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
     * @param _state is the status of the right wheel
     */
    void setStateWheelRight(const StateWheel _state);



    /**
     * Set if the car is skidding or not
     * @param skid controls if the car is skidding or not
     */
    void setSkidding(const bool skid);



    /**
     * Set if the car has terminated the animation of drift when starts
     * and also when the game round finishes
     */
    void setEndAnimation(const bool _endAnimation);



    /**
     * Get the acceleration of the car
     */
    float getLowAccel() const;



    /**
     * Get the gear of the car
     */
    int getGear() const;



    /**
     * Get the road where the player car is driving
     */
    Player_Road getPlayerRoad() const;



    /**
     * Get if the car is crashing or playing the crash animation
     */
    bool getCrashing() const;



    /**
     * Get if the car has crashed or not with a traffic car
     */
    bool getTrafficCrash() const;



    /**
     * Get the collision direction of the car
     */
    float getCollisionDir() const;



    /**
     * Get the coefficient of the curve where the car has crashed
     */
    float getCollisionCurve() const;



    /**
     * Get the number of angers done by the blond woman
     */
    int getNumAngers() const;



    /**
     * Get the status of the left wheel of the car (outside or inside the road)
     */
    bool getOutsideLeftWheelRoad() const;



    /**
     * Get the status of the right wheel of the car (outside or inside the road)
     */
    bool getOutsideRightWheelRoad() const;



    /**
     * Get the status of the left wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
     */
    StateWheel getStateWheelLeft() const;



    /**
     * Get the status of the right wheel of the car (SMOKE, SAND, SNOW, GRASS or MUD)
     */
    StateWheel getStateWheelRight() const;



    /**
     * Get if the car has finished the starting or ending drift animations
     */
    bool getEndAnimation() const;



    /**
     * Get the maximum speed of the player car (depending on the gear used)
     */
    float getHighMaxSpeed() const;



    /**
     * Get the maximum speed reached by the car when it is using the low gear
     */
    float getLowMaxSpeed() const;



    /**
     * Get the total capability of the car to turn
     */
    float getThresholdX() const;



    /**
     * Compute the actions introduced by the player to move, accelerate or brake the car
     * @param input is the module that has all the configuration of the game
     * @param gameStatus is the status of the game
     * @param time is the time elapsed between the previous move and the current one
     */
    void accelerationControl(Input& input, const State gameStatus, const float time);



    /**
     * Makes the effect of centrifugal force when the player car is in a curve and goes so fast
     * @param playerLine is the position of the player in the scenario
     * @param time is the time elapsed between the previous move and the current one
     * @param mapDistance is the width of the road
     */
    void controlCentrifugalForce(const Line* playerLine, const float& time, const int roadDistance);



    /**
     * Check if the car has crashed with any element (car or element of the scene)
     * @param x1 is the bottom left corner of the car image
     * @param w1 is the width of the car texture
     * @param x2 is the bottom left corner of the map element or traffic car sprite
     * @param w2 is the width of the map element or traffic car texture
     * @param scale is th scale factor used to display the sprites in the screen
     */
    bool hasCrashed(float x1, int w1, float x2, float w2, float scale);



    /**
     * Check if there is collision between the player car and a map element of the scenario
     * @param input is the module that has all the configuration of the game
     * @param playerLine is the position of the player in the scenario
     * @param crashed controls if the player is crashing now or not
     * @param sprite is the map element with which the collision is going to be checked
     */
    void checkCollisionSpriteInfo(Input& input, const Line* playerLine, bool& crashed, const SpriteInfo* sprite);



    /**
     * Check if there is collision between the player car and a traffic car of the scenario
     * @param input is the module that has all the configuration of the game
     * @param playerLine is the position of the player in the scenario
     * @param trafficCarLine is the position of the traffic car in the scenario
     * @param trafficCar is the traffic car with which the collision is going to be checked
     * @param crashed controls if the player is crashing now or not
     */
    void checkCollisionTrafficCar(Input& input, const Line* playerLine, const Line* trafficCarLine,
                                  TrafficCar* car, bool& crashed);



    /**
     * Draw the car on the screen before starts to drift to the center
     * @param input is the module that has all the configuration of the game
     */
    void drawStartStaticRound(Input& input);



    /**
     * Draw the starting animation drift of the player car
     * @param input is the module that has all the configuration of the game
     * @param x is the coordinate of the car in axis X
     * @param code is the identifier of the car sprite to be drawn
     */
    void drawStartDriftRound(Input &input, float x, int& code);



    /**
     * Draw the player car during the game round
     * @param input is the module that has all the configuration of the game
     * @param pauseMode controls if the game is or not paused
     * @param terrain stores the type of terrain outside the road
     * @param motorEngineSound controls if the sounds of the car must be played
     */
    void drawPlayRound(Input& input, const bool& pauseMode, const int terrain, const bool& motorEngineSound = true);



    /**
     * Draw the ending animation of the car when the game round has finished
     * @param input is the module that has all the configuration of the game
     */
    void drawEndDriftRound(Input &input);



    /**
     * Stop all the player car sounds
     */
    void shutDownSounds();

};


#endif // PLAYER_CAR_H


