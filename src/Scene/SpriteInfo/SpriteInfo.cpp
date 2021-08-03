
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

#include "SpriteInfo.h"

SpriteInfo::SpriteInfo(){}

SpriteInfo::SpriteInfo(sf::Texture* _textureSprite, const fPoint _pivotL, const fPoint _pivotR, const float _scale,
               const int _wCol, const fPoint _pivotColL, const fPoint _pivotColR, const float _offsetX,
               const float _offsetY, const bool _side)
{
    textureSprite = _textureSprite;
    pivotLeft = _pivotL;
    pivotRight = _pivotR;
    scale = _scale;
    collider = true;
    wCol = _wCol;
    offsetX = _offsetX;
    offsetY = _offsetY;
    side = _side;
    pivotColLeft = _pivotColL;
    pivotColRight = _pivotColR;
};


