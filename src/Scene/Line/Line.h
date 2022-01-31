
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

#ifndef LINE_H
#define LINE_H

#include "../Point/Point.h"
#include "../SpriteInfo/SpriteInfo.h"
#include "../../Input/Input.h"
#include "../../Car/TrafficCar/TrafficCar.h"


struct PointLine {
	float xCamera, yCamera, zCamera;
	float xWorld, yWorld, zWorld;
	float xScreen, yScreen, wScreen;
	float scale;
};


struct Line {

    int index;
	PointLine p1, p2;
	PointLine p11, p21;
	bool light, mirror, hasSpriteNearLeft, hasSpriteNearRight, hasSpriteFarLeft, hasSpriteFarRight, hasSpriteCenter;
	float curve, distance, clip;

	SpriteInfo* spriteFarLeft;
	SpriteInfo* spriteNearLeft;
	SpriteInfo* spriteFarRight;
    SpriteInfo* spriteNearRight;
    SpriteInfo* spriteCenter;

	Line();

	~Line();

	void projection(Input& input, PointLine &p, int cameraX, int cameraY, int cameraZ, float cameraD);

    void renderSpriteInfo(Input& input, SpriteInfo* sprite);

	void renderCars(Input& input, TrafficCar* car, const bool pauseMode);

	void drawObject(Input& input, int x, int y, sf::Texture* t, float speed, fPoint scale, fPoint pivot);

};

#endif // LINE_H
