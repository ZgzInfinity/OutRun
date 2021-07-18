
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
#include "../../Input/Input.h"

/**
 * Coefficient aliases to control the types of collision
 */
enum class HitCoeffType : int {
    HIT_CENTER,
    HIT_LEFT,
    HIT_RIGHT,
    HIT_SIDES,
    __COUNT
};

struct Prop {
	sf::Texture animLeft;
	sf::Texture animRight;
	fPoint pivotL;
	fPoint pivotR;
	float scale;
	bool collider;
	float wCol;
	fPoint pivotColL, pivotColR;
	Prop() : pivotL({ 1.f, 1.f }), pivotR({ 0.f, 1.f }),
		scale(1.f),
		collider(false),
		wCol(0.f),
		pivotColL({ 0.5f, 1.f }), pivotColR({ 0.5f, 1.f }) {}
};

struct PointLine {
	float xCamera, yCamera, zCamera;
	float xWorld, yWorld, zWorld;
	float xScreen, yScreen, wScreen;
	float scale;
};


class Line{

public:

	Line();

	~Line();

	void projection(Input& input, PointLine &p, int cameraX, int cameraY, int cameraZ, float cameraD);

	void renderProps(Input& input, int i);

	void Blit(Input& input, int x, int y, sf::Texture* t, float speed, fPoint scale, fPoint pivot);


public:

	int index;
	PointLine p1, p2;
	PointLine p11, p21;
	bool light, mirror;
	float curve, distance, clip;

    vector<Prop*> lineProps;
	vector<float> offsetsX;
	vector<float> offsetsY;
	vector<bool> sides;
};

#endif // LINE_H
