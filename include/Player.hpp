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

#include "Vehicle.hpp"



/**
 * Represents a player as an available vehicle to be
 * chosen by the player in the vehicle selection menu
 */
class Player : public Vehicle {

    // Gear with the player is currently running
    int gearPlayer;

    // Factor to multiply the speed of the player's vehicle
    float speedMul;

    // Maximum acceleration reached by the player's vehicle
    float maxAcc;

    // Acceleration increment of the player's vehicle
    float accInc;

    // Scaling factor of the player's vehicle in axis y
    float scaleY;

    // Current acceleration of the player's vehicle
    float acceleration;

    // Minimum acceleration reached when the player's vehicle crashes
    float minCrashAcc;

    // Coordinate in axis x that the player's vehicle must reach when it crashes
    float xDest;

    // Magnitude of the inertia force that the player's vehicle experiments in the curves
    int inertia;

    // Sprite to draw the texture of the player's vehicle in the screen
    sf::Sprite sprite;

    // True if crashing state is on
    bool crashing;

    // True if player generates smoke
    bool smoking;

    // True if player is skidding
    bool skidding;

    // Control if the player's vehicle has crashed more than one time
    bool firstCrash;

    // Control if the last direction of the player's vehicle was turning left
    bool firstTurnLeft;

    // Control if the last direction of the player's vehicle was turning right
    bool firstTurnRight;

    // Mode type of collision
    int mode;

    // Speed of the player's vehicle at the moment of a collision
    float speedCollision;

    // Control if the player's vehicle is inside the road or not
    bool outSideRoad;

    // Number of angers of the blonde woman
    int numAngers;

    //


public:



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
    Player(float maxSpeed, float speedMul, float accInc, float scaleX, float scaleY, int maxCounterToChange,
           const std::string &vehicle, float pX, float pY);



    /**
     * Set the direction of the player after the fork
     * @return
     */
    void setDirectionFork();



    /**
     * Returns the last position of the player's vehicle in axis y
     * @return
     */
    float getPreviousY() const;



    /**
     * Updates the crash logic of the player's vehicle and restores speed and acceleration
     * @param vehicleCrash true if it is a crash between vehicles
     */
    void hitControl(bool vehicleCrash);



    /**
     * Returns true if the player's vehicle is crashing. Otherwise returns false
     * @return
     */
    bool isCrashing() const;



    /**
     * Returns the real speed of the player's vehicle
     * @return
     */
    float getRealSpeed() const;



    /**
     * Updates the logic of the player's vehicle acceleration and braking
     * @param c is the module configuration of the game
     * @param hasGotOut indicates if it's gone off track
     * @return
     */
    Action accelerationControl(Config &c, bool hasGotOut);



    /**
     * Updates the logic direction turn of the player's vehicle
     * @param c is the module configuration of the game
     * @param curveCoefficient is the coefficient curve
     * @return
     */
    Direction rotationControl(Config &c, float curveCoefficient, bool inFork);



    /**
     * Updates the player's vehicle sprite and draws it in the screen
     * @param c is the module configuration of the game
     * @param r is the sound player module of the game
     * @param a is the action to be done by the player's vehicle
     * @param d is the direction to be followed by the player's vehicle
     * @param e is the current elevation of the player's vehicle in the landscape
     * @param enableSound indicates if the motor of the player's vehicle has to make noise
     */
    void draw(Config &c, const Action &a, const Direction &d, const Elevation &e, int terrain, bool enableSound = true);



    /**
     * Draw the animation of the player's vehicle at the departure
     * @param c is the module configuration of the game
     */
    void drawStaticAnimation(Config &c);



    /**
     * Draw the starting animation in the screen at beginning of the race and returns if it has finished
     * @param c is the module configuration of the game
     * @param x is the position of the car in the axis x
     * @param end controls if the animation has finished
     */
    void drawInitialAnimation(Config &c, float x, bool &end, int& code);



    /**
     * Draw the final animation in the screen at the end of the race and returns if it has finished
     * @param c is the module configuration of the game
     * @param step is the position of the car in the axis x
     * @param end controls if the animation has finished
     * @param smoke controls that the smoke has to be drawn
     */
    void drawGoalAnimation(Config &c, int &step, bool &end, bool smoke = true);



    /**
     * It forces the player's vehicle to be smoking or not
     * @param smoke indicates if the player's vehicle has to make smoke or not
     */
    void setSmoking(bool smoke);



    /**
     * Sets the type mode of collision
     * @return
     */
    void setModeCollision();



    /**
     * Return the gear of the player's vehicle
     */
    int getGearPlayer();

};


#endif // OUTRUN_PLAYER_HPP
