
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

#include <cmath>
#include "../Line/Line.h"
#include "../../Globals.h"

Line::Line()
{
    p1.xWorld = p1.yWorld = p1.zWorld = p11.xWorld = p11.yWorld = p11.zWorld = 0;
	p2.xWorld = p2.yWorld = p2.zWorld = p21.xWorld = p21.yWorld = p21.zWorld = 0;
	curve = distance = 0, clip = 0;
	light = mirror = false;

	lineProps.reserve(2);
	offsetsX.reserve(2);
	offsetsY.reserve(2);
	sides.reserve(2);
}

Line::~Line()
{}

void Line::projection(Input& input, PointLine &p, int cameraX, int cameraY, int cameraZ, float cameraD)
{
	// scale = (zWorld - cameraZ != 0 ? cameraD / (zWorld - cameraZ) : cameraD);
    p.xCamera = p.xWorld - cameraX;
	p.yCamera = p.yWorld - cameraY;
	p.zCamera = p.zWorld - cameraZ;
	p.scale = cameraD / p.zCamera;
	p.xScreen = ((int)input.gameWindow.getSize().x / 2) + ((int)input.gameWindow.getSize().x / 2) * p.xCamera * p.scale;
	p.yScreen = ((int)input.gameWindow.getSize().y / 2) - ((int)input.gameWindow.getSize().y / 2) * p.yCamera * p.scale;
	p.wScreen = p.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2;
}



void Line::renderProps(Input& input, int i)
{

    PointLine p = p1;
	if (sides[i])
		p = p11;


	float spriteX = p.xScreen + (offsetsX[i] * p.scale * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
	float spriteY = p.yScreen + (offsetsY[i] * p.scale * 1000.f * input.gameWindow.getSize().y / 2);

	sf::Texture rectDest;
	fPoint pivot;

	if (offsetsX[i] >= 0){
		rectDest = lineProps[i]->animRight;
		pivot = lineProps[i]->pivotR;
	}
	else {
		rectDest = lineProps[i]->animLeft;
		pivot = lineProps[i]->pivotL;
	}

    float width = rectDest.getSize().x;
    float height = rectDest.getSize().y;

	float destW = (width * p.scale * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);
	float destH = (height * p.scale * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);

	//If has to be clipped
	if (clip < spriteY)
	{
		float clipH = MAX(0, clip - (spriteY - (destH*3.43f * lineProps[i]->scale)));
		float clipHPerc = (clipH / (destH * 3.43f * lineProps[i]->scale));
		height = (int)(height * clipHPerc);
		spriteY = clip;
		destH *= clipHPerc;
	}

	if (height > 0){
		Blit(input, (int)spriteX, (int)(spriteY + SCREEN_Y_OFFSET), &(rectDest), 1.f, { (destW / width) * 3.2f * lineProps[i]->scale, (destH / height) * 3.43f * lineProps[i]->scale}, pivot);
	}
}


void Line::renderCars(Input& input, TrafficCar* car) {

	PointLine p = p1;
	PointLine px = p2;
	if (car->side)
	{
		p = p11;
		px = p21;
	}

	float perc = (float)(((int)(car->zPos) % (int)SEGMENT_LENGTH) / (float)SEGMENT_LENGTH);
	float scaleOffset = p.scale + (px.scale - p.scale)* perc;
	float xOffset = p.xScreen + (px.xScreen - p.xScreen)* perc;
	float yOffset = p.yScreen + (px.yScreen - p.yScreen)* perc;

	float spriteX = xOffset + (car->offset * scaleOffset * ROAD_WIDTH * input.gameWindow.getSize().x / 2);
	float spriteY = yOffset;
	fPoint pivot = { 0.5f, 1.f };
	car->draw();
	sf::Texture rectDest = car->textures[car->current_code_image - 1];

    float width = rectDest.getSize().x;
    float height = rectDest.getSize().y;

	float destW = (width * scaleOffset * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);
	float destH = (height * scaleOffset * input.gameWindow.getSize().x / 2) * ((0.3f * (1.f / 170.f)) * ROAD_WIDTH);

	//If has to be clipped
	if (clip < spriteY)
	{
		float clipH = MAX(0, clip - (spriteY - (destH*3.43f*1.2f)));
		float clipHPerc = (clipH / (destH*3.43f*1.2f));
		height = (int)(height * clipHPerc);
		spriteY = clip;
		destH *= clipHPerc;
	}

	if (height > 0)
		Blit(input, (int)spriteX, (int)(spriteY + SCREEN_Y_OFFSET), &(rectDest), 1.f, { (destW / width) * 3.2f *1.2f, (destH / height) * 3.43f * 1.2f }, pivot);
}


void Line::Blit(Input& input, int x, int y, sf::Texture* t, float speed, fPoint scale, fPoint pivot){

    if (t != nullptr){
        sf::Sprite sprite;
        int w, h;
        w = t->getSize().x;
        h =  t->getSize().y;

        int xValue = (int)(w - (w * scale.x));
        int yValue = (int)(h - (h * scale.y));

        sprite.setTexture(*t);
        sprite.setScale((w * scale.x) / sprite.getLocalBounds().width,
                        (h * scale.y) / sprite.getLocalBounds().height);

        sprite.setPosition((int)(x - (w * pivot.x) + (int)(xValue * pivot.x)),
                           (int)(y - (h * pivot.y) + (int)(yValue * pivot.y)));

        input.gameWindow.draw(sprite);
    }
}
