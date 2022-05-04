
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
 * Interface file of the module Line
 */

#pragma once

#ifndef LINE_H
#define LINE_H

#include "../Point/Point.h"
#include "../SpriteInfo/SpriteInfo.h"
#include "../../Input/Input.h"
#include "../../Car/TrafficCar/TrafficCar.h"
#include "../../Menus/Menu/Menu.h"

class TrafficCar;



/**
 * Represents the structure of dots forming a map rectangle (3d)
 * and its representation in the screen (2d)
 */
struct VertexPoint {

    /*
     * Position of the camera in the screen
     */

    // Position of the camera in axis X
	float xPosCamera;

    // Position of the camera in axis Y
	float yPosCamera;

    // Position of the camera in axis Z
	float zPosCamera;

	/*
	 * Representation of the elements of the scenarios in 3d
	 */

    // Position of the elements in axis X using 3d world representation
	float xPosWorld;

    // Position of the elements in axis X using 3d world representation
	float yPosWorld;

    // Position of the elements in axis X using 3d world representation
	float zPosWorld;

	/*
	 * Representation of the elements of the scenarios in 2d
	 */

    // Position of the elements in axis X using 2d world representation
	float xPosScreen;

    // Position of the elements in axis Y using 2d world representation
	float yPosScreen;

    // Factor to modify the elements of the screen to create the depth effect (2.5d)
	float wPosScreen;

	// Scale factor used to draw the elements of the scenario
	float scale;
};



/**
 * Represents the standard unit that compose a scenario
 */
struct Line {

    // Identifier of a line in the scenario
    int index;

    // Lower left point of a rectangle that can be drawn in the line
	VertexPoint lowerLeftPoint;

    // Lower right point of a rectangle that can be drawn in the line
    VertexPoint lowerRightPoint;

    // Upper left point of a rectangle that can be drawn in the line
	VertexPoint upperLeftPoint;

    // Upper right point of a rectangle that can be drawn in the line
	VertexPoint upperRightPoint;

	// Control which colors have to be used to used to paint the rectangles of the line (odd or even)
	bool light;

	// Control if the line has to be drawn to times in mirror effect (fork simulation)
	bool mirror;

    // Control if a line has a sprite in the near left position to be drawn
	bool hasSpriteNearLeft;

    // Control if a line has a sprite in the near right position to be drawn
    bool hasSpriteNearRight;

    // Control if a line has a sprite in the far left position to be drawn
    bool hasSpriteFarLeft;

    // Control if a line has a sprite in the far right position to be drawn
    bool hasSpriteFarRight;

    // Control if a line has a sprite in the center position to be drawn
    bool hasSpriteCenter;

	// Direction coefficient of a curve (positive => right and negative => left)
	float curve;

	// Width of the road
	float distance;

	// Elevation slope factor to create the elevation effect
	float clip;

	// Far left sprite
	SpriteInfo* spriteFarLeft;

	// Near right sprite
	SpriteInfo* spriteNearLeft;

	// Far right sprite
	SpriteInfo* spriteFarRight;

	// Near right sprite
    SpriteInfo* spriteNearRight;

    // Center sprite
    SpriteInfo* spriteCenter;


    /**
     * Default constructor
     */
	Line();



	/**
	 * Destructor
	 */
	~Line();



	/**
	 * Convert and project the position of an element in 3d world coordinates in a position in 2d (screen system)
     * @param input is the module that has all the configuration of the game
     * @param p is the structure of 4 points in 3d to be represented in 2d
     * @param cameraX is the position of the camera in axis X
     * @param cameraY is the position of the camera in axis Y
     * @param cameraZ is the position of the camera in axis Z
     * @param camD is the rendering depth
     */
	void projection(Input& input, VertexPoint &p, int cameraX, int cameraY, int cameraZ, float cameraD);



	/**
	 * Render a sprite of the scenario
     * @param input is the module that has all the configuration of the game
     * @param sprite is the element of the scenario to be drawn
     */
    void renderSpriteInfo(Input& input, SpriteInfo* sprite);



    /**
	 * Render a traffic car in the scenario
     * @param input is the module that has all the configuration of the game
     * @param car is the traffic car to be drawn
     * @param pauseMode controls if the game is in pause mode
     * @param gameStatus is the status of the game
     */
	void renderCars(Input& input, TrafficCar* car, const bool pauseMode, const State gameStatus);



    /**
	 * Draw a sprite of the scenario in the screen
     * @param input is the module that has all the configuration of the game
     * @param x is the position of the element in axis X in the screen
     * @param y is the position of the element in axis Y in the screen
     * @param t is the texture be drawn in the screen
     * @param scale is the 2d scale point factor to rescale the size of the sprite
     * @param pivot is the 2d point from where the sprite has to be drawn
     */
	void drawObject(Input& input, int x, int y, sf::Texture* t, fPoint scale, fPoint pivot);

};

#endif // LINE_H
