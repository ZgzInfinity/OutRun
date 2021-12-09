
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
	mapLanes = 3;
	lineW = 20;
	mapDistance = 0;
    pWheelL = -210;
	pWheelR = 210;
    offsetXBackground1 = 1000.f;

    swapping = false;
	backgroundSwapOffset = 0.f;
	ending = false;
	sameColors = 0;
	notDrawn = false;

	//Initial position
	iniPosition = position = 300 * (int)SEGMENT_LENGTH;

    startBiome = nullptr;
    currentBiome = nullptr;
    goalBiome = nullptr;
}

Map::~Map()
{}

bool Map::getStartMap() const {
    return currentBiome->startBiome;
}

bool Map::getgoalMap() const {
    return currentBiome->goalBiome;
}

void Map::setMapDistanceAndTrackLength(){

    currentBiome->addBiome(10, 400, 50, -2, 0, true, currentBiome->dist3);
	currentBiome->addBiome(100, 100, 100, 0, 0, true, currentBiome->distM);
	currentBiome->addBiome(10, 10, 10, 0, 0, false, currentBiome->dist3);

    mapDistance = (int)currentBiome->lines[0]->distance;
	trackLength = (int)(currentBiome->lines.size() * SEGMENTL);
	currentBiome->lastLine = currentBiome->lines.size() - drawDistance;
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
    Line* l;
    for (int i = 0; i < cars.size(); i++){
		TrafficCar* c = cars[i];
		c->setPosZ(c->getPosZ() + c->getSpeed());
		l = currentBiome->lines[(int)((c->getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
		Line* playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
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

void Map::updateMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, const float time, int long long& score){

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

	Line* playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
	p.elevationControl(playerLine->p1.yWorld, playerLine->p2.yWorld);
    float playerPerc = (float)(((position + p.getPosZ()) % (int)SEGMENTL) / SEGMENTL);
	p.setPosY((int)(playerLine->p1.yWorld + (playerLine->p2.yWorld - playerLine->p1.yWorld) * playerPerc));

	// FALTA EL ONROAD
    if (!currentBiome->end && playerLine->index > currentBiome->lastLine){
        if (p.getPlayerMap() == playerR::LEFTROAD)
            currentBiome = currentBiome->getLeft();
        else
            currentBiome = currentBiome->getRight();

        if (currentBiome->end)
            ending = true;

        for (unsigned int i = 0; i < cars.size(); ++i)
		{
		    int posZ = cars[i]->getPosZ();
		    posZ -= position;
			cars[i]->setPosZ(posZ);
			//Cars on the other road will be moved away
			if (cars[i]->getActive() && ((p.getPlayerMap() == playerR::LEFTROAD && cars[i]->getSide()) || (p.getPlayerMap() == playerR::RIGHTROAD && !cars[i]->getSide())))
			{
                int posZ = cars[i]->getPosZ();
                posZ += (rand() % (680) + 220) * SEGMENT_LENGTH;
				cars[i]->setPosZ(posZ);
				cars[i]->setActive(false);
				cars[i]->setSide(rand() % 2);
				cars[i]->setSpeed(0.f);
			}
		}

        position = iniPosition = 0;
		trackLength = (int)(currentBiome->lines.size() * SEGMENTL);
		p.setPosY(0);

		notDrawn = true;
    }

	//Activate end sequence
	if (currentBiome->getGoalBiome() && p.getEndAnimation() && playerLine->index >= 500)
        p.setEndAnimation(false);

    //Check for biome colors change
	if (!currentBiome->biomeSwap && !currentBiome->end && !swapping && playerLine->index > currentBiome->swapLine)
	{
		swapping = true;
		Biome* biome = currentBiome->left;
		if (p.getPlayerMap() == playerR::RIGHTROAD)
			biome = currentBiome->right;

		backGround2 = biome->backGround;
		bg2 = biome->skyBiome;
		sandAux = biome->sandBiome1; sand2Aux = biome->sandBiome2; roadAux = biome->roadBiome1; road2Aux = biome->roadBiome2;
		rumbleAux = biome->rumbleBiome1; rumble2Aux = biome->rumbleBiome2; laneAux = biome->laneBiome1; lane2Aux = biome->laneBiome2;
	}

	if (gameStatus == State::PLAY_ROUND){
        if (p.getCrashing()){
            if (p.getSpeed() > 0.f){
                if (p.getSpeed() >= 20.f)
                    p.setSpeed(p.getSpeed() - (p.getLowAccel() * time * 0.7f));
                else
                    p.setSpeed(p.getSpeed() - (p.getLowAccel() * time * 0.9f));

                if (p.getCollisionDir() >= 0.f){
                    if (p.getSpeed() <= 75.f)
                        p.setPosX(p.getPosX() - (p.getSpeed() / p.getHighMaxSpeed() * 2 * time));
                    else
                        p.setPosX(p.getPosX() - (p.getSpeed() / p.getHighMaxSpeed() * 3 * time));
                }
                else {
                    if (p.getSpeed() <= 75.f)
                        p.setPosX(p.getPosX() + (p.getSpeed() / p.getHighMaxSpeed() * 2 * time));
                    else
                        p.setPosX(p.getPosX() + (p.getSpeed() / p.getHighMaxSpeed() * 3 * time));
                }
            }
            else {
                p.setSpeed(0.f);
                p.setLowAccel(p.getLowMaxSpeed() / 6.5f);
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
                Line* lc = currentBiome->lines[(int)(car->getPosZ() / SEGMENT_LENGTH) % currentBiome->lines.size()];
                p.checkCollisionTrafficCar(input, playerLine, lc, car, hasCrashed);

                if (hasCrashed)
                    break;
            }
        }
	}

    playerPerc = (float)(((position + p.getPosZ()) % (int)SEGMENTL) / SEGMENTL);
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

        input.gameWindow.clear();

        if (swapping)
            interpolateBiomes(input);

        Line* playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
        Line* baseLine = currentBiome->lines[(int)(position / SEGMENTL) % currentBiome->lines.size()];
        float percent = (float)((position % (int)SEGMENTL) / SEGMENTL);
        float difX = -(baseLine->curve * percent);
        float sumX = 0;
        Line* l;

        playerLine->projection(input, playerLine->p1, (int)((p.getPosX() * ROAD_WIDTH) - sumX),
                               (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

        float maxY = playerLine->p1.yScreen;

        int x1 = 0, y1 = input.gameWindow.getSize().y;
        int width = input.gameWindow.getSize().x;
        int height = input.gameWindow.getSize().y;
        drawQuad(input, x1, y1, width, height, bg);

        sf::Color sandColor, roadColor, rumbleColor, laneColor;

        for (int n = 0; n < drawDistance; n++) {
            l = currentBiome->lines[(baseLine->index + n) % currentBiome->lines.size()];
            l->clip = maxY;

            l->light ? sandColor = sand : sandColor = sand2;
            l->light ? roadColor = road : roadColor = road2;
            l->light ? rumbleColor = rumble : rumbleColor = rumble2;
            l->light ? laneColor = lane : laneColor = lane2;

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

            drawPoly4(input, 0, y1, (int)input.gameWindow.getSize().x, y1, (int)input.gameWindow.getSize().x, y2, 0, y2, sandColor);
            drawPoly4(input, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, roadColor);
            drawPoly4(input, x11 - w11, y11, x11 + w11, y11, x21 + w21, y21, x21 - w21, y21, roadColor);

            drawPoly4(input, x1 - w1 - (int)(w1 / 7), y1, x1 + w1 + (int)(w1 / 7), y1,
                x2 + w2 + (int)(w2 / 7), y2, x2 - w2 - (int)(w2 / 7), y2, rumbleColor);

            drawPoly4(input, x11 - w11 - (int)(w11 / 7), y11, x11 + w11 + (int)(w11 / 7), y11,
                x21 + w21 + (int)(w21 / 7), y21, x21 - w21 - (int)(w21 / 7), y21, rumbleColor);

            //Draw lines in road lanes (order matters for joining them together)
            drawPoly4(input, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, laneColor);
            drawPoly4(input, x11 - w11, y11, x11 + w11, y11, x21 + w21, y21, x21 - w21, y21, laneColor);

            drawPoly4(input, x1 - w1 + (w1 / 18), y1, x1 + w1 - (w1 / 18), y1, x2 + w2 - (w2 / 18), y2,
                x2 - w2 + (w2 / 18), y2, roadColor);

            drawPoly4(input, x11 - w11 + (w11 / 18), y11, x11 + w11 - (w11 / 18), y11,
                x21 + w21 - (w21 / 18), y21, x21 - w21 + (w21 / 18), y21, roadColor);

            drawPoly4(input, x1 - w1 + (w1 / 18) + (w1 * 16 / 27), y1, x1 + w1 - (w1 / 18) - (w1 * 16 / 27), y1,
                x2 + w2 - (w2 / 18) - (w2 * 16 / 27), y2, x2 - w2 + (w2 / 18) + (w2 * 16 / 27), y2, laneColor);

            drawPoly4(input, x11 - w11 + (w11 / 18) + (w11 * 16 / 27), y11, x11 + w11 - (w11 / 18) - (w11 * 16 / 27), y11,
                x21 + w21 - (w21 / 18) - (w21 * 16 / 27), y21, x21 - w21 + (w21 / 18) + (w21 * 16 / 27), y21, laneColor);

            drawPoly4(input, x1 - w1 + (w1 / 18) * 2 + (w1 * 16 / 27), y1, x1 + w1 - (w1 / 18)*2 - (w1 * 16 / 27), y1,
                x2 + w2 - (w2 / 18)*2 - (w2 * 16 / 27), y2, x2 - w2 + (w2 / 18) * 2 + (w2 * 16 / 27), y2, roadColor);

            drawPoly4(input, x11 - w11 + (w11 / 18) * 2 + (w11 * 16 / 27), y11, x11 + w11 - (w11 / 18) * 2 - (w11 * 16 / 27), y11,
                x21 + w21 - (w21 / 18) * 2 - (w21 * 16 / 27), y21, x21 - w21 + (w21 / 18) * 2 + (w21 * 16 / 27), y21, roadColor);

            maxY = l->p2.yScreen;
        }

        sf::RectangleShape backgroundShapeSliced, backgroundShapeNewBiome;
        float posBackX = currentBiome->backgroundShape.getPosition().x;
        float posBackY = currentBiome->backgroundShape.getPosition().y;
        float heightBack = currentBiome->backgroundShape.getSize().y;
        float widthBack = currentBiome->backgroundShape.getSize().x;

        if (!swapping){

            if (!playerLine->mirror)
                offsetXBackground1 += playerLine->curve * (position - iniPosition) / SEGMENT_LENGTH * 1.5f;

            backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                               MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

            backgroundShapeSliced.setPosition(posBackX + (int)offsetXBackground1,
                                        MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));


            backgroundShapeSliced.setTexture(&backGround1, true);
            backgroundShapeSliced.setTextureRect(sf::IntRect(posBackX + (int)offsetXBackground1,
                                                             MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                             input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

            drawBackground(input, 0, (int)(maxY + SCREEN_Y_OFFSET), backgroundShapeSliced, 1.f, { 1.f, 1.f }, { 0.f, 1.f });
        }
        else {

            // First background of the map
            backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                               MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

            backgroundShapeSliced.setPosition(posBackX + (int)offsetXBackground1,
                                        MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));

            backgroundShapeSliced.setTexture(&backGround1, true);
            backgroundShapeSliced.setTextureRect(sf::IntRect(posBackX + (int)offsetXBackground1,
                                                             MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                             input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

            drawBackground(input, 0 - (int)backgroundSwapOffset, (int)(maxY + SCREEN_Y_OFFSET), backgroundShapeSliced, 1.f, { 1.f, 1.f }, { 0.f, 1.f });

            // Second background of the map
            backgroundShapeNewBiome.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                               MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

            backgroundShapeNewBiome.setPosition(posBackX + (int)offsetXBackground1,
                                        MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));

            backgroundShapeNewBiome.setTexture(&backGround2, true);

            backgroundShapeNewBiome.setTextureRect(sf::IntRect(posBackX + BACKGROUND_MOVING_OFFSET,
                                                             MAX(posBackY, posBackY + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                             input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

            drawBackground(input, input.gameWindow.getSize().x - (int)backgroundSwapOffset, (int)(maxY + SCREEN_Y_OFFSET), backgroundShapeNewBiome, 1.f, { 1.f, 1.f }, { 0.f, 1.f });
        }


        if ((currentBiome->getStartBiome() || currentBiome->getGoalBiome()) && (gameStatus == State::PLAY_ROUND || gameStatus == State::END_ROUND) && !Logger::getEndFlaggerAnimation())
            Logger::updateSprite(*currentBiome, Sprite_Animated::FLAGGER);

        //Draw sprites and cars
        for (int n = (int)(drawDistance - 1); n >= 0; --n)
        {
            l = currentBiome->lines[(baseLine->index + n) % currentBiome->lines.size()];

            if (l->index < playerLine->index)
                continue;

            if (currentBiome->getStartBiome() || currentBiome->getGoalBiome()){
                if (l->hasSpriteFarLeft)
                    l->renderSpriteInfo(input, l->spriteFarLeft);
                if (l->hasSpriteFarRight)
                    l->renderSpriteInfo(input, l->spriteFarRight);
            }


            if (l->hasSpriteNearLeft){
                l->renderSpriteInfo(input, l->spriteNearLeft);
            }

            if (l->hasSpriteNearRight)
                l->renderSpriteInfo(input, l->spriteNearRight);


            Line* l2;
            for (unsigned int n = 0; n < cars.size(); ++n)
            {
                l2 = currentBiome->lines[(int)(cars[n]->getPosZ() / SEGMENTL) % currentBiome->lines.size()];
                if (l2->index == l->index && l2->index >= playerLine->index && l2->index < playerLine->index + drawDistance)
                {
                    l2->renderCars(input, cars[n]);
                }
            }
        }
}

void Map::setCurrentBiome(Biome& b){
    currentBiome = &b;
    backGround1 = currentBiome->backGround;
}

void Map::setGoalBiome(Biome& b){
    goalBiome = &b;
}


Biome* Map::getCurrentBiome() const {
    return currentBiome;
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

void Map::interpolateBiomes(Input& input)
{
	if (backgroundSwapOffset < input.gameWindow.getSize().x)
	{
		backgroundSwapOffset += 5.f;
		if (backgroundSwapOffset > input.gameWindow.getSize().x)
			backgroundSwapOffset = input.gameWindow.getSize().x;
	}

	bg = interpolateColors(bg, bg2);
	sand = interpolateColors(sand, sandAux);
	sand2 = interpolateColors(sand2, sand2Aux);
	road = interpolateColors(road, roadAux);
	road2 = interpolateColors(road2, road2Aux);
	rumble = interpolateColors(rumble, rumbleAux);
	rumble2 = interpolateColors(rumble2, rumble2Aux);
	lane = interpolateColors(lane, laneAux);
	lane2 = interpolateColors(lane2, lane2Aux);

	if (backgroundSwapOffset >= input.gameWindow.getSize().x && sameColor(bg, bg2) && sameColor(sand, sandAux) && sameColor(sand2, sand2Aux) && sameColor(road, roadAux) && sameColor(road2, road2Aux) &&
		sameColor(rumble, rumbleAux) && sameColor(rumble2, rumble2Aux) && sameColor(lane, laneAux) && sameColor(lane2, lane2Aux))
	{
		backGround1 = backGround2;
		backgroundSwapOffset = 0.f;
		swapping = false;
		currentBiome->biomeSwap = true;
		offsetXBackground1 = BACKGROUND_MOVING_OFFSET;
	}
}

sf::Color Map::interpolateColors(const sf::Color& c1, const sf::Color& c2){
	sf::Color c = c1;
	if (c1.r > c2.r) --c.r;
	else if (c1.r < c2.r) ++c.r;
	if (c1.g > c2.g) --c.g;
	else if (c1.g < c2.g) ++c.g;
	if (c1.b > c2.b) --c.b;
	else if (c1.b < c2.b) ++c.b;

	return c;
}

bool Map::sameColor(const sf::Color& c1, const sf::Color& c2){
	return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}

void Map::setMapColors(){
    bg = currentBiome->skyBiome;
    sand = currentBiome->sandBiome1;
    sand2 = currentBiome->sandBiome2;
    road = currentBiome->roadBiome1;
    road2 = currentBiome->roadBiome2;
    rumble = currentBiome->rumbleBiome1;
    rumble2 = currentBiome->rumbleBiome2;
    lane = currentBiome->laneBiome1;
    lane2 = currentBiome->laneBiome2;
}

bool Map::getEnding() const {
    return ending;
}

void Map::setNotDrawn(const bool _notDrawn){
    notDrawn = _notDrawn;
}

bool Map::getNotDrawn() const {
    return notDrawn;
}

