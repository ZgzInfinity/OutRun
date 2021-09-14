
/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
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

#pragma once

#ifndef SPRITE_INFO_H
#define SPRITE_INFO_H

#include "../Point/Point.h"
#include "SFML/Graphics.hpp"


enum class Sprite_Position : int {
    FAR_LEFT,
    NEAR_LEFT,
    FAR_RIGHT,
    NEAR_RIGHT
};


class SpriteInfo {

    private:

        sf::Texture* textureSprite;
        fPoint pivotLeft, pivotRight;
        float scale;
        bool collider, side;
        float widthCol, offsetX, offsetY;
        fPoint pivotColLeft, pivotColRight;

    public:

        SpriteInfo();

        SpriteInfo(sf::Texture* _textureSprite, const fPoint _pivotL, const fPoint _pivotR, const float _scale,
                   const int _wCol, const fPoint _pivotColL, const fPoint _pivotColR, const float _offsetX,
                   const float _offsetY, const bool _side);

        sf::Texture* getTextureSprite() const;

        fPoint getPivotLeft() const;

        fPoint getPivotRight() const;

        float getScale() const;

        bool getCollider() const;

        bool getSide() const;

        float getWidthCol() const;

        float getOffsetX() const;

        void setOffsetY(const float _offsetY);

        float getOffsetY() const;

        fPoint getPivotColLeft() const;

        fPoint getPivotColRight() const;
};

#endif // SPRITE_INFO_H
