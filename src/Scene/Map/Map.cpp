
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

#include "Map.h"

Map::Map()
{
    //Map parameters
	drawDistance = 200;
	segmentL = 150;
	rumbleL = 3.f;
	mapLanes = 3;
	lineW = 20;
	mapDistance = 0;
    pWheelL = -210;
	pWheelR = 210;
    offsetXBackground1 = 1000.f;

	//Initial position
	iniPosition = position = 300 * (int)SEGMENT_LENGTH;

	//Set distances for Map lanes
	dist3 = 0;
	dist4 = dist3 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist5 = dist4 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist6 = dist5 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist7 = dist6 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist8 = dist7 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	distM = dist8 + ((int)ROAD_WIDTH * 16 / 27) * 7 + ((int)ROAD_WIDTH / 18) * 7;

	startMap = goalMap = false;
}

Map::~Map()
{}

void Map::setBackground(){
    backGround.loadFromFile("Resources/Maps/MapLevels/Map1/bg.png");
    backGround.setRepeated(true);
    backgroundShape.setPosition(0, 0);
    backgroundShape.setSize(sf::Vector2f(4030, 243));
}

void Map::setColors(const std::vector<sf::Color>& colorsOfMap){
    sky = colorsOfMap[0];
    sand1 = colorsOfMap[1];
    sand2 = colorsOfMap[2];
    road1 = colorsOfMap[3];
    road2 = colorsOfMap[4];
    rumble1 = colorsOfMap[5];
    rumble2 = colorsOfMap[6];
    lane1 = colorsOfMap[7];
    lane2 = colorsOfMap[8];
}

void Map::setTime(const int _time){
    time = _time;
}

void Map::setTerrain(const int _terrain){
    terrain = _terrain;
}

int Map::getTime() const {
    return time;
}

int Map::getTerrain() const {
    return terrain;
}

bool Map::getStartMap() const {
    return startMap;
}

Line* Map::getLine(const int& index){
    return lines[index];
}


int Map::computeRoadTracks(const int numTracks){
    switch (numTracks){
        case 2:
            return distM;
        case 3:
            return dist3;
        case 4:
            return dist4;
        case 5:
            return dist5;
        case 6:
            return dist6;
        case 7:
            return dist7;
        case 8:
            return dist8;
    }
}

void Map::setMapDistanceAndTrackLength(){

    addMap(10, 400, 50, -2, 0, true, dist3);
	addMap(100, 100, 100, 0, 0, true, distM);

    mapDistance = lines[0]->distance;
	trackLength = (int)(lines.size() * segmentL);
}


void Map::drawQuad(Input &input, int x1, int y1, int width, int height, sf::Color sky, bool use_camera) {
	if (use_camera){

        sf::ConvexShape shape(4);
        shape.setPoint(0, sf::Vector2f(x1, y1 - height));
        shape.setPoint(1, sf::Vector2f(x1, y1));
        shape.setPoint(2, sf::Vector2f(x1 + width, y1));
        shape.setPoint(3, sf::Vector2f(x1 + width, y1 - height));

        shape.setFillColor(sky);
        input.gameWindow.draw(shape);
	}
}



void Map::drawPoly4(Input &input, short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, sf::Color color) {

    sf::VertexArray quad(sf::Quads, 4);

    // define it as a rectangle, located at (10, 10) and with size 100x100
    quad[0].position = sf::Vector2f(x1, (y1 + SCREEN_Y_OFFSET));
    quad[1].position = sf::Vector2f(x2, (y2 + SCREEN_Y_OFFSET));
    quad[2].position = sf::Vector2f(x3, (y3 + SCREEN_Y_OFFSET));
    quad[3].position = sf::Vector2f(x4, (y4 + SCREEN_Y_OFFSET));

	quad[0].color = color;
	quad[1].color = color;
	quad[2].color = color;
	quad[3].color = color;

    input.gameWindow.draw(quad);
}


void Map::updateCars(vector<TrafficCar*> cars, const PlayerCar& p, int long long& score){
    for (int i = 0; i < cars.size(); i++){
		TrafficCar* c = cars[i];
		c->setPosZ(c->getPosZ() + c->getSpeed());
		Line* l = lines[(int)((c->getPosZ()) / segmentL) % lines.size()];
		Line* playerLine = lines[(int)((position + p.getPosZ()) / segmentL) % lines.size()];
		switch (c->getActive()) {
            case false:
                if (l->index < playerLine->index + drawDistance && l->index > playerLine->index)
                {
                    c->setActive(true);
                    c->setSpeed(120.f);
                }
                break;
            case true:
                if (l->index > playerLine->index + drawDistance || l->index < playerLine->index)
                {
                    if (l->index < playerLine->index)
                        score += SCORE_TRAFFIC_BONUS;

                    c->setSide(rand() % 2);
                    c->setActive(false);
                    c->setSpeed(0.f);
                    float adv = (rand() % (680) + 220)*SEGMENT_LENGTH;
                    c->setPosZ(c->getPosZ() + adv);
                }
                break;
		}

		float posCar = (c->getSide() ? c->getOffset() * ROAD_WIDTH + mapDistance : c->getOffset() * ROAD_WIDTH);
		if (posCar > p.getPosX() * ROAD_WIDTH)
			c->setDirection(Direction::TURNLEFT);
		else
			c->setDirection(Direction::TURNRIGHT);
	}
}


void Map::updateCarPlayerWheels(PlayerCar& p){

	pWheelL = p.getPosX() * ROAD_WIDTH - 210;
	pWheelR = p.getPosX() * ROAD_WIDTH + 210;

	if (p.getSpeed() > 0.f) {
		if (pWheelR < -1831 || pWheelR > mapDistance + 1831){
            p.setStateWheelRight(StateWheel::SAND);
            p.setOutsideRoad(true);
		}
		else {
            p.setOutsideRoad(false);
		}

		if (pWheelL < -1831 || pWheelL > mapDistance + 1831){
			p.setStateWheelLeft(StateWheel::SAND);
            p.setOutsideRoad(true);
		}
        else {
            if (!p.getOutiseRoad())
                p.setOutsideRoad(false);
		}

		if (!p.getOutiseRoad() && mapDistance > 3662)
		{
			if (pWheelR > 1831 && pWheelR < 1831 + (mapDistance - 3662)){
                p.setStateWheelRight(StateWheel::SAND);
                p.setOutsideRoad(true);
			}
            else {
                p.setOutsideRoad(false);
            }

			if (pWheelL > 1831 && pWheelL < 1831 + (mapDistance - 3662)){
                p.setStateWheelLeft(StateWheel::SAND);
                p.setOutsideRoad(true);
			}
            else {
                if (!p.getOutiseRoad())
                    p.setOutsideRoad(false);
            }
		}
	}
}

void Map::updateMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, const float time, int long long& score){

    updateCars(cars, p, score);
	iniPosition = position;

    if (p.getSpeed() > 0.f){
        if (p.getTrafficCrash())
            position = position - (p.getSpeed() * 0.5f);
        else
            position = position + p.getSpeed();
    }

	while (position >= trackLength)
		position -= trackLength;
	while (position < 0)
		position += trackLength;

	//Calculate playerY for hills
	Line* playerLine = lines[(int)((position + p.getPosZ()) / segmentL) % lines.size()];
	p.elevationControl(playerLine->p1.yWorld, playerLine->p2.yWorld);

    if (p.getCrashing()){
        if (p.getSpeed() > 0.f){
            if (p.getSpeed() >= 20.f)
                p.setSpeed(p.getSpeed() - (p.getLowAccel() * time * 0.7f));
            else
                p.setSpeed(p.getSpeed() - (p.getLowAccel() * time * 0.9f));

            if (p.getCollisionDir() >= 0.f){
                if (p.getSpeed() <= 75.f)
                    p.setPosX(p.getPosX() - (p.getSpeed() / p.getMaxSpeed() * 2 * time));
                else
                    p.setPosX(p.getPosX() - (p.getSpeed() / p.getMaxSpeed() * 3 * time));
            }
            else {
                if (p.getSpeed() <= 75.f)
                    p.setPosX(p.getPosX() + (p.getSpeed() / p.getMaxSpeed() * 2 * time));
                else
                    p.setPosX(p.getPosX() + (p.getSpeed() / p.getMaxSpeed() * 3 * time));
            }
        }
        else {
            p.setSpeed(0.f);
            p.setLowAccel(14.2857f);
            p.setCollisionDir();
            if (p.getNumAngers() == 3){
                p.setAngryWoman();
                p.setTrafficCrash();
                p.setDrawCar(false);

                float dif = 0.f;
                if (p.getPlayerMap() == playerR::RIGHTROAD)
                    dif = ((float)mapDistance / (float)ROAD_WIDTH);
                if (p.getPosX() < -0.05f + dif)
                    p.setPosX(p.getPosX() + 0.012f);
                else if (p.getPosX() > 0.05f + dif)
                    p.setPosX(p.getPosX() - 0.012f);
                else {
                    p.setCrashing(false);
                    p.setNumAngers();
                    p.setDrawCar(true);
                }
            }
            p.setStateWheelLeft(StateWheel::NORMAL);
            p.setStateWheelRight(StateWheel::NORMAL);
        }
    }

	bool hasCrashed = false;

	if (playerLine->hasSpriteFarLeft)
        p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteFarLeft);
	if (!hasCrashed && playerLine->hasSpriteNearLeft)
        p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteNearLeft);
    if (!hasCrashed && playerLine->hasSpriteFarRight)
        p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteFarRight);
    if (!hasCrashed && playerLine->hasSpriteNearRight)
        p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteNearRight);

	if (!hasCrashed){
        for (auto& car : cars)
        {
            Line* lc = lines[(int)(car->getPosZ() / SEGMENT_LENGTH) % lines.size()];
            p.checkCollisionTrafficCar(input, playerLine, lc, car, hasCrashed);

            if (hasCrashed)
                break;
        }
	}

	float playerPerc = (float)(((position + p.getPosZ()) % (int)segmentL) / segmentL);
	p.setPosY((int)(playerLine->p1.yWorld + (playerLine->p2.yWorld - playerLine->p1.yWorld) * playerPerc));

	if (abs(playerLine->p1.xCamera) <= abs(playerLine->p11.xCamera)){
     	p.setPlayerMap(playerR::LEFTROAD);
    }
    else {
        p.setPlayerMap(playerR::RIGHTROAD);
    }

    p.controlCentrifugalForce(playerLine, time, mapDistance);
	mapDistance = (int)playerLine->distance;

	//Check road limits for player
	if (p.getPosX() < -1.5f)
		p.setPosX(-1.48f);
	else if (p.getPosX() > 1.5f + ((float)mapDistance / (float)ROAD_WIDTH))
		p.setPosX(1.48f + ((float)mapDistance / (float)ROAD_WIDTH));

	if (playerLine->mirror)
	{
		if ((float)mapDistance / (float)ROAD_WIDTH > 3.5f && p.getPosX() > 1.75f && p.getPosX() < (float)mapDistance / (float)ROAD_WIDTH - 1.75f)
		{
			if (p.getPlayerMap() == playerR::LEFTROAD)
				p.setPosX(1.73f);
			else
				p.setPosX((float)mapDistance / (float)ROAD_WIDTH - 1.73f);
		}
	}


	updateCarPlayerWheels(p);

	//Make smoke if sliding to the side when in a huge curve
	if (p.getStateWheelLeft() != StateWheel::SAND && p.getStateWheelRight() != StateWheel::SAND && p.getSpeed() > 70.f)
	{
		if (abs(p.getThresholdX()) >= (p.getSpeed() * time * 0.8f) && abs(playerLine->curve) >= 2.5f)
		{
		    p.setSkidding(true);
			p.setStateWheelLeft(StateWheel::SMOKE);
			p.setStateWheelRight(StateWheel::SMOKE);
		}
		else {
            p.setSkidding(false);
		}
	}
}


// Update: draw background
void Map::renderMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus){

	Line* playerLine = lines[(int)((position + p.getPosZ()) / segmentL) % lines.size()];
	Line* baseLine = lines[(int)(position / segmentL) % lines.size()];
	float percent = (float)((position % (int)segmentL) / segmentL);
	float difX = -(baseLine->curve * percent);
	float sumX = 0;
	Line* l;

    playerLine->projection(input, playerLine->p1, (int)((p.getPosX() * ROAD_WIDTH) - sumX),
                           (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

	float maxY = playerLine->p1.yScreen;

    int x1 = 0, y1 = input.gameWindow.getSize().y;
    int width = input.gameWindow.getSize().x;
    int height = input.gameWindow.getSize().y;
    drawQuad(input, x1, y1, width, height, sf::Color(0, 148, 255, 255));

	for (int n = 0; n < drawDistance; n++) {
		l = lines[(baseLine->index + n) % lines.size()];
		l->clip = maxY;

		l->light ? sand = sand1 : sand = sand2;
		l->light ? road = road1 : road = road2;
		l->light ? rumble = rumble1 : rumble = rumble2;
		l->light ? lane = lane1 : lane = lane2;

		l->projection(input, l->p1, (int)((p.getPosX() * ROAD_WIDTH) - sumX),
                (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

		l->projection(input, l->p2, (int)((p.getPosX() * ROAD_WIDTH) - sumX - difX),
                (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

		if (l->mirror)
		{
			l->projection(input, l->p11, (int)((p.getPosX() * ROAD_WIDTH) + sumX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

			l->projection(input, l->p21, (int)((p.getPosX() * ROAD_WIDTH) + sumX + difX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);
		}
		else
		{
			l->projection(input, l->p11, (int)((p.getPosX() * ROAD_WIDTH) - sumX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

			l->projection(input, l->p21, (int)((p.getPosX() * ROAD_WIDTH) - sumX - difX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);
		}

		sumX += difX;
		difX += l->curve;

		if ((l->p1.zCamera <= CAMERA_DISTANCE) || (l->p2.yScreen >= maxY))
			continue;

		short x1 = (short)l->p1.xScreen;
		short x2 = (short)l->p2.xScreen;
		short y1 = (short)l->p1.yScreen;
		short y2 = (short)l->p2.yScreen;
		short w1 = (short)l->p1.wScreen;
		short w2 = (short)l->p2.wScreen;

		short x11 = (short)l->p11.xScreen;
		short x21 = (short)l->p21.xScreen;
		short y11 = (short)l->p11.yScreen;
		short y21 = (short)l->p21.yScreen;
		short w11 = (short)l->p11.wScreen;
		short w21 = (short)l->p21.wScreen;

		drawPoly4(input, 0, y1, (int)input.gameWindow.getSize().x, y1, (int)input.gameWindow.getSize().x, y2, 0, y2, sand);
		drawPoly4(input, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, road);
		drawPoly4(input, x11 - w11, y11, x11 + w11, y11, x21 + w21, y21, x21 - w21, y21, road);

		drawPoly4(input, x1 - w1 - (int)(w1 / 7), y1, x1 + w1 + (int)(w1 / 7), y1,
            x2 + w2 + (int)(w2 / 7), y2, x2 - w2 - (int)(w2 / 7), y2, rumble);

		drawPoly4(input, x11 - w11 - (int)(w11 / 7), y11, x11 + w11 + (int)(w11 / 7), y11,
            x21 + w21 + (int)(w21 / 7), y21, x21 - w21 - (int)(w21 / 7), y21, rumble);

		//Draw lines in road lanes (order matters for joining them together)
		drawPoly4(input, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, lane);
		drawPoly4(input, x11 - w11, y11, x11 + w11, y11, x21 + w21, y21, x21 - w21, y21, lane);

		drawPoly4(input, x1 - w1 + (w1 / 18), y1, x1 + w1 - (w1 / 18), y1, x2 + w2 - (w2 / 18), y2,
            x2 - w2 + (w2 / 18), y2, road);

		drawPoly4(input, x11 - w11 + (w11 / 18), y11, x11 + w11 - (w11 / 18), y11,
            x21 + w21 - (w21 / 18), y21, x21 - w21 + (w21 / 18), y21, road);

		drawPoly4(input, x1 - w1 + (w1 / 18) + (w1 * 16 / 27), y1, x1 + w1 - (w1 / 18) - (w1 * 16 / 27), y1,
            x2 + w2 - (w2 / 18) - (w2 * 16 / 27), y2, x2 - w2 + (w2 / 18) + (w2 * 16 / 27), y2, lane);

		drawPoly4(input, x11 - w11 + (w11 / 18) + (w11 * 16 / 27), y11, x11 + w11 - (w11 / 18) - (w11 * 16 / 27), y11,
            x21 + w21 - (w21 / 18) - (w21 * 16 / 27), y21, x21 - w21 + (w21 / 18) + (w21 * 16 / 27), y21, lane);

		drawPoly4(input, x1 - w1 + (w1 / 18) * 2 + (w1 * 16 / 27), y1, x1 + w1 - (w1 / 18)*2 - (w1 * 16 / 27), y1,
            x2 + w2 - (w2 / 18)*2 - (w2 * 16 / 27), y2, x2 - w2 + (w2 / 18) * 2 + (w2 * 16 / 27), y2, road);

		drawPoly4(input, x11 - w11 + (w11 / 18) * 2 + (w11 * 16 / 27), y11, x11 + w11 - (w11 / 18) * 2 - (w11 * 16 / 27), y11,
            x21 + w21 - (w21 / 18) * 2 - (w21 * 16 / 27), y21, x21 - w21 + (w21 / 18) * 2 + (w21 * 16 / 27), y21, road);

		maxY = l->p2.yScreen;
	}

	sf::RectangleShape backgroundShapeSliced;
    float posBackX = backgroundShape.getPosition().x;
    float posBackY = backgroundShape.getPosition().y;
    float heightBack = backgroundShape.getSize().y;
    float widthBack = backgroundShape.getSize().x;


    if (!playerLine->mirror)
        offsetXBackground1 += playerLine->curve * (position - iniPosition) / SEGMENT_LENGTH * 2.f;

    backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                               MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

    backgroundShapeSliced.setPosition(posBackX + (int)offsetXBackground1,
                                MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));

    backgroundShapeSliced.setTexture(&backGround, true);
    backgroundShapeSliced.setTextureRect(sf::IntRect(posBackX + (int)offsetXBackground1,
                                                     MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                     input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

    drawBackground(input, 0, (int)(maxY + SCREEN_Y_OFFSET), backgroundShapeSliced, 1.f, { 1.f, 1.f }, { 0.f, 1.f });

    if (startMap && gameStatus == State::PLAY_ROUND && !Logger::getEndFlaggerAnimation())
        Logger::updateSprite(*this, Sprite_Animated::FLAGGER);

    //Draw sprites and cars
	for (int n = (int)(drawDistance - 1); n >= 0; --n)
	{
		l = lines[(baseLine->index + n) % lines.size()];

		if (l->index < playerLine->index)
			continue;

        if (startMap || goalMap){
            if (l->hasSpriteFarLeft)
                l->renderSpriteInfo(input, l->spriteFarLeft);
            if (l->hasSpriteFarRight)
                l->renderSpriteInfo(input, l->spriteFarRight);
        }


        if (l->hasSpriteNearLeft)
            l->renderSpriteInfo(input, l->spriteNearLeft);
        if (l->hasSpriteNearRight)
            l->renderSpriteInfo(input, l->spriteNearRight);


		Line* l2;
		for (unsigned int n = 0; n < cars.size(); ++n)
		{
			l2 = lines[(int)(cars[n]->getPosZ() / segmentL) % lines.size()];
			if (l2->index == l->index && l2->index >= playerLine->index && l2->index < playerLine->index + drawDistance)
			{
                l2->renderCars(input, cars[n]);
			}
		}
	}
}


void Map::addSpriteInfo(int line, SpriteInfo* p, const Sprite_Position spritePos){
	if (line < (int)lines.size()){
        switch (spritePos){
        case Sprite_Position::FAR_LEFT:
            lines[line]->spriteFarLeft = p;
            lines[line]->hasSpriteFarLeft = true;
            break;
        case Sprite_Position::NEAR_LEFT:
            lines[line]->spriteFarRight = p;
            lines[line]->hasSpriteFarRight = true;
            break;
        case Sprite_Position::FAR_RIGHT:
            lines[line]->spriteNearLeft = p;
            lines[line]->hasSpriteNearLeft = true;
            break;
        case Sprite_Position::NEAR_RIGHT:
            lines[line]->spriteNearRight = p;
            lines[line]->hasSpriteNearRight = true;
        }
	}
}

void Map::setStartSrpiteScreenY(const float _offsetY) {
    lines[310]->spriteFarLeft->setOffsetY(_offsetY);
}

void Map::addSegment(float curve, float y, bool mirror, float dist)
{
	int n = (int)lines.size();
	Line* l = new Line();

	l->index = n;
	l->p1.zWorld = (float)(n * segmentL);
	l->p11.zWorld = l->p1.zWorld;
	l->p1.yWorld = (lines.size() == 0 ? 0 : lines[lines.size() - 1]->p2.yWorld);
	l->p11.yWorld = l->p1.yWorld;
	l->p2.zWorld = (float)((n + 1) * segmentL);
	l->p21.zWorld = l->p2.zWorld;
	l->p2.yWorld = y;
	l->p21.yWorld = l->p2.yWorld;
	l->light = (int)((n / rumbleL)) % 2;
	l->curve = curve;
	l->mirror = mirror;
	l->distance = dist;
	lines.push_back(l);
}

void Map::setStartMap(const Map& m){
    sky = m.sky;
    sand1 = m.sand1;
    sand2 = m.sand2;
    road1 = m.road1;
    road2 = m.road2;
    rumble1 = m.rumble1;
    rumble2 = m.rumble2;
    lane1 = m.lane1;
    lane2 = m.lane2;
    backGround = m.backGround;
    backgroundShape = m.backgroundShape;
    time = m.getTime();

    addMap(400, 400, 400, 0, 0, false, dist8);
    mapDistance = lines[0]->distance;
	trackLength = (int)(lines.size() * segmentL);

	vector<string> objectNames;
    objectNames.reserve(41);
    for (int i = 1; i <= 41; i++){
        objectNames.push_back(std::to_string(i));
    }

    string path = "Resources/Maps/MapStart/";
    Logger::loadObjects(path, objectNames);
    Logger::loadStartMapSprites(*this);
    startMap = true;
}

void Map::addMap(int enter, int hold, int leave, float curve, float y, bool mirror, int distance)
{
	float firstY, dist, distPerc;
	float total = (float)(enter + hold + leave);
	if (mirror)
	{
		firstY = (lines.size() == 0 ? 0 : lines[lines.size() - 1]->p2.yWorld);
		dist = lines[lines.size() - 1]->distance;
		distPerc = (float)((distM - dist)) / total;
	}
	else
	{
		if (lines.size() == 0)
		{
			firstY = 0;
			dist = (float)distance;
			distPerc = 0;
		}
		else
		{
			firstY = lines[lines.size() - 1]->p2.yWorld;
			dist = lines[lines.size() - 1]->distance;
			distPerc = (float)((distance - dist)) / total;
		}
	}

	float endY = firstY + y * segmentL;
	int n;

	for (n = 0; n < enter; ++n)
	{
		dist += (int)distPerc;
		addSegment(easeIn(0, curve, (float)n / enter), easeInOut(firstY, endY, (float)n / total), mirror, dist);
	}

	for (n = 0; n < hold; ++n)
	{
		dist += (int)distPerc;
        addSegment(curve, easeInOut(firstY, endY, (float)(enter + n) / total), mirror, dist);
	}

	for (n = 0; n < leave; ++n)
	{
		dist += (int)distPerc;
		addSegment(easeInOut(curve, 0, (float)n / leave), easeInOut(firstY, endY, (float)(enter + hold + n) / total), mirror, dist);
	}

}

float Map::easeIn(float a, float b, float percent)
{
	return (a + (b - a) * pow(percent, 2));
}

float Map::easeInOut(float a, float b, float percent)
{
	return (a + (b - a) * ((-cos(percent * (float)M_PI) / 2.f) + 0.5f));
}


float Map::distance(float a, float b)
{
	return abs((b - a));
}


void Map::drawBackground(Input& input, int x, int y, sf::RectangleShape background, float speed, fPoint scale, fPoint pivot){

    int w, h;
    w = background.getSize().x;
    h = background.getSize().y;

    int xValue = (int)(w - (w * scale.x));
    int yValue = (int)(h - (h * scale.y));

    background.setPosition((int)(x - (w * pivot.x) + (int)(xValue * pivot.x)),
                           (int)(y - (h * pivot.y) + (int)(yValue * pivot.y)));

    background.setSize(sf::Vector2f((int)(w * scale.x), (int)(h * scale.y)));

    input.gameWindow.draw(background);
}

