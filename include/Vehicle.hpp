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
 * Module Vehicle interface file
 */

#ifndef OUTRUN_VEHICLE_HPP
#define OUTRUN_VEHICLE_HPP

#include <vector>
#include "Menu.hpp"
#include "Random.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>

#define XINC 0.033f // x increment
#define ACC_INC 0.01f
#define ACC_INC_ENEMY 0.005f
#define COUNTER 2



/**
 * The vehicle information is composed of its speed, acceleration, x-position, set of textures of the
 * vehicle, current sprite and current action and direction.
 */
class Vehicle {

    public:

        // Types of elevation of the terrain
        enum Elevation {
            UP,
            FLAT,
            DOWN
        };

        // Actions that can be made with the vehicle
        enum Action {
            NONE,
            BRAKE,
            ACCELERATE,
            CRASH,
            BOOT
        };

        // Possible directions of the vehicle
        enum Direction {
            RIGHT,
            TURNLEFT,
            TURNRIGHT
        };

    protected:

        // Maximum speed reached by the vehicle in KM/H
        float maxSpeed;

        // Half speed reached by the vehicle in KM/H
        float halfMaxSpeed;

        // Scale coefficient to be drawn on the screen depending of the resolutions
        float scale;

        // Counter to change the direction path of the vehicle
        int maxCounterToChange;

        // Current speed of the vehicle in KM/H
        float speed;

        // Current position in axis x
        float posX;

        // Current position in axis y
        float posY;

        // Last position in axis y
        float previousY;

        // Last position in axis x
        float previousX;

        // Minimum coefficient to draw the vehicle in the screen
        float minScreenX;

        // Maximum coefficient to draw the vehicle in the screen
        float maxScreenX;

        // Vector with the textures of the vehicle
        vector<sf::Texture> textures;

        // Numeric code of the sprite to be drawn in the screen
        int current_code_image;

        // Counter to change actual_code_image
        int counter_code_image;

        // Type of direction  selected in the fork
        int directionFork;


public:



    /**
     * Initialize a vehicle with all its properties
     * @param maxSpeed is the maximum speed reached by the vehicle in KM/H
     * @param scale is the scaling factor to draw the vehicle in the screen with different resolutions
     * @param maxCounterToChange is limit to change the direction of the vehicle
     * @param speed is the speed of the vehicle in KM/H
     * @param posX is the position of the vehicle in the axis x
     * @param posY is the position of the vehicle in the axis y
     * @param previousY is the last position of the vehicle in the axis y
     * @param minScreenX is the minimum coefficient to draw the vehicle in the screen
     * @param maxScreenX is the maximum coefficient to draw the vehicle in the screen
     * @param vehicle is the type of vehicle selected by the player in the selection vehicle menu
     * @param numTextures is the number of textures of the vehicle selected
     * @param currentCodeImage is the code of the sprite to be drawn in the screen
     * @param counterCodeImage is a counter that change the sprite to be drawn in the screen
     */
    Vehicle(float maxSpeed, float scale, int maxCounterToChange, float speed, float posX, float posY, float previousY,
            float minScreenX, float maxScreenX, const std::string &vehicle, int numTextures, int currentCodeImage,
            int counterCodeImage);



    /**
     * Establish the vehicle in a concrete position in the landscape
     * @param pX is the position in the axis x when the vehicle must be located
     * @param pY is the position in the axis y when the vehicle must be located
     */
    void setPosition(float pX, float pY);



    /**
     * Establish the vehicle in a concrete position in the landscape
     * @param pX is the position in the axis x when the vehicle must be located
     */
    void setPositionX(float pX);



    /**
     * Returns the current position of the vehicle in the axis x
     * @return
     */
    float getPosX() const;



    /**
     * Returns the current position of the vehicle in the axis y
     * @return
     */
    float getPosY() const;



    /**
     * Returns the previous position of the vehicle in the axis x
     * @return
     */
    float getPreviousX() const;



    /**
     * Returns the minimum coordinate of the screen in axis x used by the vehicle
     * @return
     */
    float getMinScreenX() const;



    /**
     * Returns the maximum coordinate of the screen in axis x used by the vehicle
     * @return
     */
    float getMaxScreenX() const;



    /**
     * Returns the acceleration of the vehicle
     * @return
     */
    float getAcceleration() const;



    /**
     * Get the direction followed by the vehicle in the fork
     * @return
     */
    int getDirectionFork() const;


};



/**
 * Randomly choose the direction the vehicle must go
 */
Vehicle::Direction randomDirection();


#endif // OUTRUN_VEHICLE_HPP
