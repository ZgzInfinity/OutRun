
/*
 * Copyright (c) 2023 Ruben Rodriguez
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
 * Interface file of the module SpriteInfo
 */


#pragma once

#ifndef SPRITE_INFO_H
#define SPRITE_INFO_H

#include "../Point/Point.h"
#include "SFML/Graphics.hpp"



/**
 * Represents the different positions of the sprite in the scenario
 */
enum class Sprite_Position : int {
    FAR_LEFT,
    NEAR_LEFT,
    FAR_RIGHT,
    NEAR_RIGHT,
    CENTER
};



/**
 * Represents the two types of animated sprites
 */
enum class Sprite_Animated : int {
    FLAGGER,
    SEMAPHORE
};




/**
 * Represents the map elements (sprites) that decorate
 * the different scenarios of the game
 */
class SpriteInfo {

    private:

        // Texture of the sprite
        sf::Texture* textureSprite;

        // Lower left point of the sprite to calculate its position in the screen
        fPoint pivotLeft;

        // Lower right point of the sprite to calculate its position in the screen
        fPoint pivotRight;

        // Scale factor to draw the sprite in the screen
        float scale;

        // Controls if the player car can crash with the sprite
        bool collider;

        // Control if the sprite must be drawn next to the left or next to the right road
        bool side;

        // Width part of the sprite with which the player car can crash
        float widthCol;

        // Position of the sprite in axis X using like reference the center of the road
        float offsetX;

        // Position of the sprite in axis Y
        float offsetY;

        // Lower left point of the sprite to calculate the possible collisions with the player car
        fPoint pivotColLeft;

        // Lower right point of the sprite to calculate the possible collisions with the player car
        fPoint pivotColRight;

        // Controls if the player car has to draw terrain effects when crashes with it
        int showTerrain;

    public:



        /**
         * Default constructor
         */
        SpriteInfo();



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
        SpriteInfo(sf::Texture* _textureSprite, const fPoint _pivotLeft, const fPoint _pivotRight, const float _scale,
                   const int _wCol, const int _showTerrain, const fPoint _pivotColLeft, const fPoint _pivotColRight,
                   const bool _collider, const float _offsetX, const float _offsetY, const bool _side);



        /**
         * Set the position of the sprite in axis X using like reference the center of the road
         * @param _offsetX is the position of the sprite in axis X using like reference the center of the road
         */
        void setOffsetX(const float _offsetX);



        /**
         * Set the position of the sprite in axis Y
         * @param _offsetY is the position of the sprite in axis Y
         */
        void setOffsetY(const float _offsetY);



        /**
         * Get the position of the sprite in axis X using like reference the center of the road
         */
        float getOffsetX() const;



        /**
         * Get the position of the sprite in axis Y
         */
        float getOffsetY() const;



        /**
         * Get the texture of the sprite
         */
        sf::Texture* getTextureSprite() const;



        /**
         * Get the lower left point of the sprite to calculate its position in the screen
         */
        fPoint getPivotLeft() const;



        /**
         * the lower right point of the sprite to calculate its position in the screen
         */
        fPoint getPivotRight() const;



        /**
         * Get the scale factor to draw the sprites in the screen
         */
        float getScale() const;



        /**
         * Get if the player car crash with the sprite
         */
        bool getCollider() const;



        /**
         * Get the side where the sprite has to be drawn (left or right)
         */
        bool getSide() const;



        /**
         * Get the part of the width dimension of the sprite which can make the player car crash
         */
        float getWidthCol() const;



        /**
         * Get if the player car has to draw terrain effects when crashes with it
         */
        int getShowTerrain() const;



        /**
         * Get the lower left point of the sprite to calculate the possible collisions with the player car
         */
        fPoint getPivotColLeft() const;



        /**
         * Get the lower right point of the sprite to calculate the possible collisions with the player car
         */
        fPoint getPivotColRight() const;
};

#endif // SPRITE_INFO_H
