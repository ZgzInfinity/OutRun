
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
 * Implementation file of the module Line
 */

#include <cmath>
#include "../Line/Line.h"
#include "../../Globals.h"



/**
 * Default constructor
 */
Line::Line()
{
    /*
     * Assign a default data
     */

    lowerLeftPoint.xPosWorld = 0;
    lowerLeftPoint.yPosWorld = 0;
    lowerLeftPoint.zPosWorld = 0;
    upperLeftPoint.xPosWorld = 0;
    upperLeftPoint.yPosWorld = 0;
    upperLeftPoint.zPosWorld = 0;
	lowerRightPoint.xPosWorld = 0;
	lowerRightPoint.yPosWorld = 0;
	lowerRightPoint.zPosWorld = 0;
	upperRightPoint.xPosWorld = 0;
	upperRightPoint.yPosWorld = 0;
	upperRightPoint.zPosWorld = 0;

	curve = distance = clip = 0.f;

	light = false;
	mirror = false;
	hasSpriteNearLeft = false;
	hasSpriteNearRight = false;
	hasSpriteFarLeft = false;
	hasSpriteFarRight = false;
	hasSpriteCenter = false;

	spriteFarLeft = nullptr;
	spriteNearLeft = nullptr;
	spriteFarRight = nullptr;
	spriteNearRight = nullptr;
	spriteCenter = nullptr;
}



/**
* Convert and project the position of an element in 3d world coordinates in a position in 2d (screen system)
* @param input is the module that has all the configuration of the game
* @param p is the structure of 4 points in 3d to be represented in 2d
* @param cameraX is the position of the camera in axis X
* @param cameraY is the position of the camera in axis Y
* @param cameraZ is the position of the camera in axis Z
* @param camD is the rendering depth
*/
void Line::projection(Input& input, VertexPoint &p, const int cameraX, const int cameraY, const int cameraZ, const float cameraD){

    // Calculation of the camera position
    p.xPosCamera = p.xPosWorld - cameraX;
	p.yPosCamera = p.yPosWorld - cameraY;
	p.zPosCamera = p.zPosWorld - cameraZ;

	// Compute the scale factor
	p.scale = cameraD / p.zPosCamera;

	// Calculation of the screen coordinates
	p.xPosScreen = ((int)input.gameWindow.getSize().x / 2) + ((int)input.gameWindow.getSize().x / 2) * p.xPosCamera * p.scale;
	p.yPosScreen = ((int)input.gameWindow.getSize().y / 2) - ((int)input.gameWindow.getSize().y / 2) * p.yPosCamera * p.scale;
	p.wPosScreen = p.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2;
}



/**
 * Render a sprite of the scenario
 * @param input is the module that has all the configuration of the game
 * @param sprite is the element of the scenario to be drawn
 */
void Line::renderSpriteInfo(Input& input, SpriteInfo* sprite)
{
    // Determine the reference to draw the sprite depending on the nearest road
    VertexPoint p = lowerLeftPoint;
	if (sprite->getSide())
		p = upperLeftPoint;

    // Calculate the coordinates in axis X and Y using the scale
	float spriteX = p.xPosScreen + (sprite->getOffsetX() * p.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
	float spriteY = p.yPosScreen + (sprite->getOffsetY() * p.scale * 1000.f * input.gameWindow.getSize().y / 2);

    fPoint pivot;

    // Determine the point from where the sprite has to be drawn depending on the offset in axis X
	if (sprite->getOffsetX() >= 0){
		pivot = sprite->getPivotRight();
	}
	else {
		pivot = sprite->getPivotLeft();
	}

	// Compute the width and height of the sprite
    float width = sprite->getTextureSprite()->getSize().x;
    float height = sprite->getTextureSprite()->getSize().y;

    // Calculate the coordinates in the screen
	float destW = (width * p.scale * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);
	float destH = (height * p.scale * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);

	// If has to be clipped (elevation effect
	if (clip < spriteY)
	{
		float clipH = MAX(5, clip - (spriteY - (destH * 3.43f * sprite->getScale())));
		float clipHPerc = (clipH / (destH * 3.43f * sprite->getScale()));
		height = (int)(height * clipHPerc);
		spriteY = clip;
		destH *= clipHPerc;
	}

	// Check if the sprite is on the floor or higher
	if (height > 0){
	    // Draw the sprite
		drawObject(input, (int)spriteX, (int)(spriteY + SCREEN_Y_OFFSET),
             sprite->getTextureSprite(), { (destW / width) * 3.2f * sprite->getScale(), (destH / height) * 3.43f * sprite->getScale()}, pivot);
	}
}



/**
 * Render a traffic car in the scenario
 * @param input is the module that has all the configuration of the game
 * @param car is the traffic car to be drawn
 * @param pauseMode controls if the game is in pause mode
 * @param gameStatus is the status of the game
 */
void Line::renderCars(Input& input, TrafficCar* car, const bool pauseMode, const State gameStatus) {

    // Determine the reference to draw the traffic car sprite depending on the nearest road
	VertexPoint p = lowerLeftPoint;
	VertexPoint px = lowerRightPoint;
	if (car->getSide()){
		p = upperLeftPoint;
		px = upperRightPoint;
	}

    // Calculate the coordinates in axis X and Y using the scale
	float perc = (float)(((int)(car->getPosZ()) % (int)SEGMENT_LENGTH) / (float)SEGMENT_LENGTH);
	float scaleOffset = p.scale + (px.scale - p.scale)* perc;
	float xOffset = p.xPosScreen + (px.xPosScreen - p.xPosScreen)* perc;
	float yOffset = p.yPosScreen + (px.yPosScreen - p.yPosScreen)* perc;
	float spriteX = xOffset + (car->getOffset() * scaleOffset * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
	float spriteY = yOffset;

	fPoint pivot = { 0.5f, 1.f };

	// Determine the texture to be drawn and get it
	car->draw(pauseMode, gameStatus);
	sf::Texture rectDest = car->getTexture();

    // Compute the width and height of the traffic car sprite
    float width = rectDest.getSize().x;
    float height = rectDest.getSize().y;

    // Calculate the coordinates in the screen
	float destW = (width * scaleOffset * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);
	float destH = (height * scaleOffset * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);

	// If has to be clipped
	if (clip < spriteY)
	{
		float clipH = MAX(0, clip - (spriteY - (destH *3.43f * car->getScale())));
		float clipHPerc = (clipH / (destH * 3.43f * car->getScale()));
		height = (int)(height * clipHPerc);
		spriteY = clip;
		destH *= clipHPerc;
	}

    // Check if the traffic car sprite is on the floor or higher
	if (height > 0)
        // Draw the sprite
		drawObject(input, (int)spriteX, (int)(spriteY + SCREEN_Y_OFFSET),
             &(rectDest), { (destW / width) * 3.2f * car->getScale(), (destH / height) * 3.43f * car->getScale() }, pivot);
}



/**
 * Draw a sprite of the scenario in the screen
 * @param input is the module that has all the configuration of the game
 * @param x is the position of the element in axis X in the screen
 * @param y is the position of the element in axis Y in the screen
 * @param t is the texture be drawn in the screen
 * @param scale is the 2d scale point factor to rescale the size of the sprite
 * @param pivot is the 2d point from where the sprite has to be drawn
 */
void Line::drawObject(Input& input, const int x, const int y, const sf::Texture* t, const fPoint scale, const fPoint pivot){

    // Check if there is texture to draw
    if (t != nullptr){

        // Get the sprite size
        sf::Sprite sprite;
        int w, h;
        w = t->getSize().x;
        h =  t->getSize().y;

        // Compute the scale
        int xValue = (int)(w - (w * scale.x));
        int yValue = (int)(h - (h * scale.y));

        // Assign the texture and set scale of the sprite
        sprite.setTexture(*t);
        sprite.setScale((w * scale.x) / sprite.getLocalBounds().width,
                        (h * scale.y) / sprite.getLocalBounds().height);

        // Set the position of the sprite in the screen
        sprite.setPosition((int)(x - (w * pivot.x) + (int)(xValue * pivot.x)),
                           (int)(y - (h * pivot.y) + (int)(yValue * pivot.y)));

        // Draw the sprite
        input.gameWindow.draw(sprite);
    }
}



