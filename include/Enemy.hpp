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
 * Module Enemy interface file
 */


#ifndef OUTRUN_ENEMY_HPP
#define OUTRUN_ENEMY_HPP

#include "Vehicle.hpp"


/**
 * Represents an enemy that is going to compete in some
 * game modes of the game
 */
class Enemy : public Vehicle {

    // Coordinate in the axis x
    const float oriX;

    // Direction that must be followed by the traffic car
    Direction currentDirection;

    // Direction that is going to be computed
    Direction calculatedPath;

    // Counter that stores the sprite of the traffic car to draw in the screen
    int current_direction_counter;

    // Counter that express how much the traffic car must maintain the current direction
    int max_direction_counter;

    // Probabilistic value of the AI
    float probAI;

    // Control if the traffic car is a truck or not
    bool isTruck;

    // Type of enemy car
    int id;



    /**
     * Types of AI:
     * OBSTACLE: Attempts to collide with the player by getting in his path and trying to catch him.
     * EVASIVE: Flees to the lane furthest from the player to try to avoid him.
     * INCONSTANT: Changes lanes too often regardless of the player's position.
     */
    enum TypeAI {
        OBSTACLE,
        EVASIVE,
        INCONSTANT
    };


    // AI mode of the traffic car
    TypeAI typeAI;

public:



    /**
     * Initialize the rival vehicle.
     * @param maxSpeed is the maximum speed reached by the traffic car
     * @param speedMultiplying the speed that multiplied by speed gets the real speed
     * @param scale vehicle sprite scaling
     * @param maxCounterToChange when counter_code_image arrives at maxCounterToChange the sprite is updated
     * @param vehicle name
     * @param pY is the position of the traffic car in the axis y
     * @param idCar is the numeric identifier of the vehicle
     */
    Enemy(float maxSpeed, float speedMul, float scale, int maxCounterToChange, const std::string &vehicle, float pY, int idCar);



    /**
     * Updates the vehicle logic automatically for the current movement
     *
     * The AI will only be activated if the distance between the vehicle and the player is less than or equal to the distance
     * of rendering (rectangles that the player sees).
     *
     * AI aggressiveness is within the class (probAI). It is a value between 0 and 1 that indicates the probability of
     * that the AI act in this movement.
     *
     * If the AI aggressiveness is 0, the movement will be as in the original game, i.e. the car will continue
     * at a constant speed without going off the road and following a straight path or with a turn (chosen
     * randomly) and without being influenced by the player.
     *
     * If the aggressiveness of the AI is 1, the movement will be controlled by the AI and its movement will depend on the type
     * of AI:
     * OBSTACLE: Attempts to collide with the player by getting in his path and trying to catch him.
     * EVASIVE: Flees to the lane furthest from the player to try to avoid him.
     * INCONSTANT: Changes lanes too often regardless of the player's position.
     *
     * If the aggressiveness of the AI is between 0 and 1, it performs one of the two actions described (original
     * or AI) with probability p that the AI will act and p' = (1 - p) that it will be as in the original.
     *
     * @param c is the module configuration of the game
     * @param playerPosX is the position of the traffic car in the axis x
     * @param playerPosY is the position of the traffic car in the axis y
     */
    void autoControl(const Config &c, float playerPosX, float playerPosY, bool inFork, float curveCoeff, float yOffset, int limitMap);



    /**
     * Initialize the vehicle status. Updates the aggressiveness of the vehicle's AI with a random value between 0
     * and maxAggressiveness.
     * @param iniPos is the initial position of the traffic car
     * @param endPos is the new position of the traffic car
     * @param maxAggressiveness is the AI aggressiveness of the traffic cars
     */
    void update(float iniPos, float endPos, float maxAggressiveness);



    /**
     * Updates the aggressiveness of the vehicle AI with a random value between 0 and maxAggressiveness
     * @param maxAggressiveness is the AI aggressiveness of the traffic cars
     */
    void setAI(float maxAggressiveness);



    /**
     * Update the sprite of the enemy vehicle.
     * @param e is the current elevation of the terrain where is the camera
     * @param camX is the position of the camera in the axis x
     */
    void draw(const Elevation &e, float camX);



    /**
     * Sets the minimum X coordinate that the vehicle occupies.
     * @param screenX
     */
    void setMinScreenX(float screenX);



    /**
     * Sets the maximum X coordinate that the vehicle occupies.
     * @param screenX
     */
    void setMaxScreenX(float screenX);



    /**
     * Returns the current texture of the vehicle
     * @return
     */
    const sf::Texture *getCurrentTexture() const;



    /**
     * Returns the current scale of the vehicle.
     * @return
     */
    float getScale() const;



    /**
     * Returns true if there has been a collision between the traffic vehicle and the player's vehicle.
     * If true, it also returns the Y position where they have collided
     * @param currentY is the current position of the player's vehicle in the axis y
     * @param prevY is the last position of the player's vehicle in the axis y
     * @param minX is the minimum position in axis x occupied by the vehicle
     * @param maxX is the maximum position in axis y occupied by the vehicle
     * @param crashPos is the position in axis y where the crash has happened
     * @return
     */
    bool hasCrashed(float prevY, float currentY, float minX, float maxX, float &crashPos) const;



    /**
     * Returns true if the car is displayed on screen and the distance to the player, otherwise returns false.
     * @param c is the module configuration of the game
     * @param minY is the position of the camera in the axis y
     * @param playerX is the player position in the axis x
     * @param playerY is the player position in the axis y
     * @param distanceX is the distance between the traffic car and the vehicle of the player in the axis x
     * @param distanceY is the distance between the traffic car and the vehicle of the player in the axis y
     * @return
     */
    bool isVisible(const Config& c, float minY, float playerX, float playerY, float &distanceX, float &distanceY) const;



    /**
     * Returns the numeric identifier of the car
     */
    int getId() const;

};


#endif //OUTRUN_ENEMY_HPP
