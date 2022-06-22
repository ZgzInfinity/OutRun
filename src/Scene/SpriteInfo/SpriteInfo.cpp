
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
 * Implementation file of the module SpriteInfo
 */

#include "SpriteInfo.h"



/**
 * Default constructor
 */
SpriteInfo::SpriteInfo(){}



/**
 * Second constructor
 * @param _textureSprite is the texture of the sprite
 * @param _pivotLeft is the lower left point of the sprite to calculate its position in the screen
 * @param _pivotRight is the lower right point of the sprite to calculate its position in the screen
 * @param _scale is the scale factor used to draw the sprite
 * @param _wCol is the part of the width dimension of the sprite which can make the player car crash
 * @param _showTerrain controls if the player car has to draw terrain effects when crashes with it
 * @param _pivotColLeft is the lower left point of the sprite to calculate the possible collisions with the player car
 * @param _pivotColRight is the lower right point of the sprite to calculate the possible collisions with the player car
 * @param _collider controls if the player car can crash with the sprite
 * @param _offsetX is the position of the sprite in axis X using like reference the center of the road
 * @param _offsetY is the position of the sprite in axis Y
 * @param _side controls if the sprite must be drawn next to the left or next to the right road
 */
SpriteInfo::SpriteInfo(sf::Texture* _textureSprite, const fPoint _pivotLeft, const fPoint _pivotRight, const float _scale,
                       const int _wCol, const int _showTerrain, const fPoint _pivotColLeft, const fPoint _pivotColRight,
                       const bool _collider, const float _offsetX, const float _offsetY, const bool _side)
{
    // Assign all the information
    textureSprite = _textureSprite;
    pivotLeft = _pivotLeft;
    pivotRight = _pivotRight;
    scale = _scale;
    collider = _collider;
    widthCol = _wCol;
    showTerrain = _showTerrain;
    offsetX = _offsetX;
    offsetY = _offsetY;
    side = _side;
    pivotColLeft = _pivotColLeft;
    pivotColRight = _pivotColRight;
};



/**
 * Set the position of the sprite in axis X using like reference the center of the road
 * @param _offsetX is the position of the sprite in axis X using like reference the center of the road
 */
void SpriteInfo::setOffsetX(const float _offsetX){
    offsetX = _offsetX;
}



/**
 * Set the position of the sprite in axis Y
 * @param _offsetY is the position of the sprite in axis Y
 */
void SpriteInfo::setOffsetY(const float _offsetY){
    offsetY = _offsetY;
}



/**
 * Get the position of the sprite in axis X using like reference the center of the road
 */
float SpriteInfo::getOffsetX() const {
    return offsetX;
}



/**
 * Get the position of the sprite in axis Y
 */
float SpriteInfo::getOffsetY() const {
    return offsetY;
}


/**
 * Get the texture of the sprite
 */
sf::Texture* SpriteInfo::getTextureSprite() const {
    return textureSprite;
}


/**
 * Get the lower left point of the sprite to calculate its position in the screen
 */
fPoint SpriteInfo::getPivotLeft() const {
    return pivotLeft;
}



/**
 * the lower right point of the sprite to calculate its position in the screen
 */
fPoint SpriteInfo::getPivotRight() const {
    return pivotRight;
}



/**
 * Get the scale factor to draw the sprites in the screen
 */
float SpriteInfo::getScale() const {
    return scale;
}



/**
 * Get if the player car crash with the sprite
 */
bool SpriteInfo::getCollider() const {
    return collider;
}



/**
 * Get the side where the sprite has to be drawn (left or right)
 */
bool SpriteInfo::getSide() const {
    return side;
}



/**
 * Get the part of the width dimension of the sprite which can make the player car crash
 */
float SpriteInfo::getWidthCol() const {
    return widthCol;
}



/**
 * Get if the player car has to draw terrain effects when crashes with it
 */
int SpriteInfo::getShowTerrain() const {
    return showTerrain;
}



/**
 * Get the lower left point of the sprite to calculate the possible collisions with the player car
 */
fPoint SpriteInfo::getPivotColLeft() const {
    return pivotColLeft;
}



/**
 * Get the lower right point of the sprite to calculate the possible collisions with the player car
 */
fPoint SpriteInfo::getPivotColRight() const {
    return pivotColRight;
}


