
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
 * Implementation file of the module Map
 */

#include "Map.h"



/**
 * Draw a relief rectangle of a concrete line of the scenario
 * @param input is the module that has all the configuration of the game
 * @param x1 is the upper left point in axis X of the rectangle
 * @param y1 is the upper left point in axis Y of the rectangle
 * @param x2 is the upper right point in axis X of the rectangle
 * @param y2 is the upper right point in axis Y of the rectangle
 * @param x3 is the lower left point in axis Y of the rectangle
 * @param y3 is the lower left point in axis Y of the rectangle
 * @param x4 is the lower right point in axis Y of the rectangle
 * @param y4 is the lower right point in axis Y of the rectangle
 * @param color is the color of the rectangle to be painted
 */
void Map::drawRelief(Input &input, const short x1, const short y1, const short x2, const short y2,
                     const short x3, const short y3, const short x4, const short y4, const sf::Color& color) {

    sf::VertexArray quad(sf::Quads, 4);

    // Define it as a rectangle with all the vertex points
    quad[0].position = sf::Vector2f(x1, (y1 + SCREEN_Y_OFFSET));
    quad[1].position = sf::Vector2f(x2, (y2 + SCREEN_Y_OFFSET));
    quad[2].position = sf::Vector2f(x3, (y3 + SCREEN_Y_OFFSET));
    quad[3].position = sf::Vector2f(x4, (y4 + SCREEN_Y_OFFSET));

    // Set the color of its vertex and draw it
	quad[0].color = color;
	quad[1].color = color;
	quad[2].color = color;
	quad[3].color = color;

    input.gameWindow.draw(quad);
}


/**
 * Draw the background texture of the current scenario
 * @param input is the module that has all the configuration of the game
 * @param x is the position of the background in the axis X
 * @param y is the position of the background in the axis Y
 * @param background contains the texture to be drawn as the background of the scenario
 * @param scale represents the scale factor to rescale the size of the image
 * @param pivot represents the coordinates of the 2d lower left point to start drawing the background
 */
void Map::drawBackground(Input& input, const int x, const int y, sf::RectangleShape background, const fPoint scale, const fPoint pivot){

    // Get the width and height of the texture
    int w, h;
    w = background.getSize().x;
    h = background.getSize().y;

    // Obtain the new rescaled point positions of the texture
    int xValue = (int)(w - (w * scale.x));
    int yValue = (int)(h - (h * scale.y));

    // Set the position
    background.setPosition((int)(x - (w * pivot.x) + (int)(xValue * pivot.x)),
                           (int)(y - (h * pivot.y) + (int)(yValue * pivot.y)));

    // Rescale the texture and draw it
    background.setSize(sf::Vector2f((int)(w * scale.x), (int)(h * scale.y)));
    input.gameWindow.draw(background);
}



/**
 * Draw the sky of the current scenario
 * @param input is the module that has all the configuration of the game
 * @param x1 is the upper left corner of the sky rectangle in axis X
 * @param y1 is the upper left corner of the sky rectangle in axis Y
 * @param width is the width dimension of the sky rectangle
 * @param height is the height dimension of the sky rectangle
 * @param sky is the color of the sky in the current scenario
 */
void Map::drawSky(Input &input, const int x1, const int y1, const int width, const int height, const sf::Color sky){

    // Create the rectangle using the points
    sf::ConvexShape shape(4);
    shape.setPoint(0, sf::Vector2f(x1, y1 - height));
    shape.setPoint(1, sf::Vector2f(x1, y1));
    shape.setPoint(2, sf::Vector2f(x1 + width, y1));
    shape.setPoint(3, sf::Vector2f(x1 + width, y1 - height));

    // Fill the color of the rectangle and draw it
    shape.setFillColor(sky);
    input.gameWindow.draw(shape);
}



/**
 * Returns true if two colors are equal. Otherwise returns false
 * @param c1 is the first color to compare
 * @param c2 is the second color to compare
 */
bool Map::sameColor(const sf::Color& c1, const sf::Color& c2){
	return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}



/**
 * Return and interpolate progressive color between two interval colors
 * @param c1 is the initial color
 * @param c2 is the final color
 */
sf::Color Map::interpolateColors(const sf::Color& c1, const sf::Color& c2){

    /*
     * Control if the color channels RGB are equal
     * If they are different they are modified to create a transition
     */

	sf::Color c = c1;
	if (c1.r > c2.r) --c.r;
	else if (c1.r < c2.r) ++c.r;
	if (c1.g > c2.g) --c.g;
	else if (c1.g < c2.g) ++c.g;
	if (c1.b > c2.b) --c.b;
	else if (c1.b < c2.b) ++c.b;

	// Return the new color
	return c;
}



/**
 * Make the transition between scenarios when the player is in the fork
 * @param input is the module that has all the configuration of the game
 * @param p is the player car
 * @param gameMode is the game mode selected by the player
 */
void Map::interpolateBiomes(Input& input, PlayerCar& p, const GameMode& gameMode){

    // Check if the moving transition of the backgrounds in axis X as finished
    if (backgroundSwapOffset < input.gameWindow.getSize().x){
        // Continues
        backgroundSwapOffset += 5.f;
        // Check again the limit
        if (backgroundSwapOffset > input.gameWindow.getSize().x)
            backgroundSwapOffset = input.gameWindow.getSize().x;
    }

    // Interpolate the colors of the scenarios to make the transition
    bg = interpolateColors(bg, bg2);
    sand = interpolateColors(sand, sandAux);
    sand2 = interpolateColors(sand2, sand2Aux);
    road = interpolateColors(road, roadAux);
    road2 = interpolateColors(road2, road2Aux);
    rumble = interpolateColors(rumble, rumbleAux);
    rumble2 = interpolateColors(rumble2, rumble2Aux);
    lane = interpolateColors(lane, laneAux);
    lane2 = interpolateColors(lane2, lane2Aux);
    rumbleLane = interpolateColors(rumbleLane, rumbleLaneAux);
    rumbleLane2 = interpolateColors(rumbleLane2, rumbleLane2Aux);

    // Check if the color transitions and the background moves have finished
    if (backgroundSwapOffset >= input.gameWindow.getSize().x && sameColor(bg, bg2) && sameColor(sand, sandAux) &&
        sameColor(sand2, sand2Aux) && sameColor(road, roadAux) && sameColor(road2, road2Aux) && sameColor(rumble, rumbleAux) &&
        sameColor(rumble2, rumble2Aux) && sameColor(lane, laneAux) && sameColor(lane2, lane2Aux) && sameColor(rumbleLane2, rumbleLane2Aux))
    {
        // Transition has finished

        // Set the backgrounds and offset positions
        backGroundFront = backGround2Front;
        backGroundBack = backGround2Back;
        backgroundSwapOffset = 0.f;

        // set the swapping animation as finished
        swapping = false;
        currentBiome->biomeSwap = true;
        offsetXBackground1 = BACKGROUND_MOVING_OFFSET;
        offsetXBackground2 = BACKGROUND_MOVING_OFFSET;

        // Check the player scenario chosen and get its data
        if (p.getPlayerRoad() == Player_Road::LEFTROAD){
            // Left scenario
            setTerrain(currentBiome->getLeft()->getTerrain());
            p.setRoadTerrain(currentBiome->getLeft()->getRoadTerrain());
            p.setTerrain(currentBiome->getLeft()->getTerrain());
        }
        else {
            if (gameMode == GameMode::ORIGINAL_MODE){
                // Right scenario
                setTerrain(currentBiome->getRight()->getTerrain());
                p.setRoadTerrain(currentBiome->getRight()->getRoadTerrain());
                p.setTerrain(currentBiome->getRight()->getTerrain());
            }
            else if (gameMode == GameMode::CONTINUOUS_MODE){
                // Left scenario
                setTerrain(currentBiome->getLeft()->getTerrain());
                p.setRoadTerrain(currentBiome->getLeft()->getRoadTerrain());
                p.setTerrain(currentBiome->getLeft()->getTerrain());
            }
        }
    }
}


/**
 * Update the status of the wheels of the player car
 * @param p is the player car
 */
void Map::updateCarPlayerWheels(PlayerCar& p){

    // Get the new position of the wheels in screen coordinates
	pWheelL = p.getPosX() * ROAD_WIDTH - 210;
	pWheelR = p.getPosX() * ROAD_WIDTH + 210;

	// set both wheels inside the road
	p.setOutsideRightWheelRoad(false);
	p.setOutsideLeftWheelRoad(false);

	// Check if the player is moving
	if (p.getSpeed() > 0.f) {

        // Check right wheels outside the road and draw the wheel effect depending on the terrain of the scenario
		if (pWheelR < -1831 || pWheelR > mapDistance + 1831){
            switch (terrain){
                case 1:
                    p.setStateWheelRight(StateWheel::SAND);
                    break;
                case 2:
                    p.setStateWheelRight(StateWheel::GRASS);
                    break;
                case 3:
                    p.setStateWheelRight(StateWheel::SNOW);
                    break;
                case 4:
                    p.setStateWheelRight(StateWheel::MUD);
            }
            p.setOutsideRightWheelRoad(true);
		}

        // Check left wheels outside the road and draw the wheel effect depending on the terrain of the scenario
		if (pWheelL < -1831 || pWheelL > mapDistance + 1831){
			switch (terrain){
                case 1:
                    p.setStateWheelLeft(StateWheel::SAND);
                    break;
                case 2:
                    p.setStateWheelLeft(StateWheel::GRASS);
                    break;
                case 3:
                    p.setStateWheelLeft(StateWheel::SNOW);
                    break;
                case 4:
                    p.setStateWheelLeft(StateWheel::MUD);
            }
            p.setOutsideLeftWheelRoad(true);
		}

		// Two roads
		if (mapDistance > 3662){

            // Check right wheels outside the road and draw the wheel effect depending on the terrain of the scenario
			if (pWheelR > 1831 && pWheelR < 1831 + (mapDistance - 3662)){
                switch (terrain){
                    case 1:
                        p.setStateWheelRight(StateWheel::SAND);
                        break;
                    case 2:
                        p.setStateWheelRight(StateWheel::GRASS);
                        break;
                    case 3:
                        p.setStateWheelRight(StateWheel::SNOW);
                        break;
                    case 4:
                        p.setStateWheelRight(StateWheel::MUD);
                }
                p.setOutsideRightWheelRoad(true);
			}

            // Check left wheels outside the road and draw the wheel effect depending on the terrain of the scenario
			if (pWheelL > 1831 && pWheelL < 1831 + (mapDistance - 3662)){
                switch (terrain){
                    case 1:
                        p.setStateWheelLeft(StateWheel::SAND);
                        break;
                    case 2:
                        p.setStateWheelLeft(StateWheel::GRASS);
                        break;
                    case 3:
                        p.setStateWheelLeft(StateWheel::SNOW);
                        break;
                    case 4:
                        p.setStateWheelLeft(StateWheel::MUD);
                }
                p.setOutsideLeftWheelRoad(true);
			}
		}
	}
}



/**
 * Update the status of all the traffic cars of the scenario
 * @param input is the module that has all the configuration of the game
 * @param cars is the vector with all the traffic cars of the scenario
 * @param p is the player car
 * @param score is the current score of the player
 * @param startCodeAi is the AI code of the traffic car to be possibly set
 */
void Map::updateCars(Input& input, vector<TrafficCar*> cars, const PlayerCar& p, int long long& score, int& startCodeAi){

    // Get the line of the scenario where currently is the player
    Line* l;
    Line* trafficOldLine;
    float posX;
    Line* playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];

    // Loop all the traffic cars
    for (int i = 0; i < cars.size(); i++){

        // Get where traffic car is located in the scenario
		TrafficCar* c = cars[i];
		trafficOldLine = currentBiome->lines[(int)((c->getPosZ()) / SEGMENTL) % currentBiome->lines.size()];

		// Check if the traffic car is nearer than a threshold limit distance to the player car to move it
		if (abs(trafficOldLine->index - playerLine->index) <= (drawDistance * 8))
            c->setPosZ(c->getPosZ() + c->getSpeed());

        // Get the new position of the traffic car after being moved and determine its elevation
		l = currentBiome->lines[(int)((c->getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
        c->elevationControl(l->lowerLeftPoint.yPosWorld, l->lowerRightPoint.yPosWorld);

        // Check if the car is active (visible by the player)
		switch (c->getActive()) {
            case false:
                // Check if the traffic car is in front of the player or inside the limit drawing distance
                if (l->index < playerLine->index + drawDistance && l->index > playerLine->index){

                    // Set as active and initialize its new speed and type of AI
                    c->setActive(true);
                    c->setSpeed(random_int(10, 16) * 10.f);

                    startCodeAi = (startCodeAi <= 3) ? startCodeAi + 1 : 1;
                    c->setAi(startCodeAi);
                }
                break;
            case true:
                // Check if the traffic car is in behind of the player or outside the limit drawing distance
                if (l->index > playerLine->index + drawDistance || l->index < playerLine->index){

                    // Traffic car is behind and score is increased
                    if (l->index < playerLine->index)
                        score += SCORE_TRAFFIC_BONUS;

                    // Set as inactive with no speed and new position in the scenario (in front of the player but far)
                    c->setSide(random_int(0, 1));
                    c->setActive(false);
                    c->setSpeed(0.f);
                    c->setOffset(random_int(-6, 6) * 0.15f);


                    switch (input.traffic){
                        case Level_Traffic::LOW:
                            c->setPosZ(c->getPosZ() + (random_int(5, 7) * 100 * SEGMENT_LENGTH));
                            break;
                        case Level_Traffic::MEDIUM:
                            c->setPosZ(c->getPosZ() + (random_int(5, 9) * 100 * SEGMENT_LENGTH));
                            break;
                        case Level_Traffic::HIGH:
                            c->setPosZ(c->getPosZ() + (random_int(5, 11) * 100 * SEGMENT_LENGTH));
                    }
                }
                break;
		}

		// Determine the position of the traffic car in axis X to set its direction (left or right)
        posX = (c->getSide() ? c->getOffset() * ROAD_WIDTH + mapDistance : c->getOffset() * ROAD_WIDTH);
		c->setPosX(posX);

		// Set the direction perspective using the player car position
		if (c->getPosX() > p.getPosX() * ROAD_WIDTH)
			c->setDirection(Direction::TURNLEFT);
		else
			c->setDirection(Direction::TURNRIGHT);

        // Move the traffic car depending on its AI code
        c->controlAiTrack(p, playerLine, l, mapDistance);
	}
}



/**
 * Default constructor
 */
Map::Map(){

    //Map default configuration parameters
	drawDistance = 200;
	mapDistance = 0;
    pWheelL = -210;
	pWheelR = 210;
    offsetXBackground1 = 1000.f;
    offsetXBackground2 = 1000.f;

    swapping = false;
	backgroundSwapOffset = 0.f;
	ending = false;
	sameColors = 0;
	numBiomesVisited = 0;
	newBiomeChosen = false;

	//Initial position
	iniPosition = position = 300 * (int)SEGMENT_LENGTH;
    currentBiome = nullptr;
    nextBiome = nullptr;
}



/**
 * Set the current biome
 * @param b is the current biome to be set and drawn
 */
void Map::setCurrentBiome(Biome& b){
    // Set the biome and the backgrounds
    currentBiome = &b;
    backGroundFront = currentBiome->backGroundFront;
    backGroundBack = currentBiome->backGroundBack;
}



/**
 * Set the type of terrain outside the road
 * @param _terrain is the type of terrain (SAND, GRASS, SNOW or MUD)
 * and it is represents with integers from (1..4)
 */
void Map::setTerrain(const int _terrain){
    terrain = _terrain;
}



/**
 * Set the colors of the current biome
 */
void Map::setMapColors(){
    // Set all the colors of the scenario
    bg = currentBiome->skyBiome;
    sand = currentBiome->sandBiome1;
    sand2 = currentBiome->sandBiome2;
    road = currentBiome->roadBiome1;
    road2 = currentBiome->roadBiome2;
    rumble = currentBiome->rumbleBiome1;
    rumble2 = currentBiome->rumbleBiome2;
    lane = currentBiome->laneBiome1;
    lane2 = currentBiome->laneBiome2;
    rumbleLane = currentBiome->rumbleLane1;
    rumbleLane2 = currentBiome->rumbleLane2;
}



/**
 * Set the total distance of the biome and add the fork part to the scenario
 * @param ending controls if the current scenario is the goal
 * @param gameMode is the game mode selected by the player
 */
void Map::setMapDistanceAndTrackLength(const bool ending, const GameMode& gameMode){
    int not_count_lines = NOT_COUNT_LINES;
    int gradient;

    switch (gameMode){
        case GameMode::ORIGINAL_MODE:
            // Check if it is the goal scenario and select the slope of the fork
            if (ending)
                gradient = 1;
            else
                gradient = random_int(-1, 1);

            // Add the fork to the current scenario
            currentBiome->addBiome(10, 300, 50, -2.0, gradient * GRADIENT_FACTOR, true, false, currentBiome->threeTracksDistance, not_count_lines);
            currentBiome->addBiome(100, 100, 100, 0, 0, true, false, currentBiome->twoTracksDistance, not_count_lines);
            currentBiome->addBiome(100, 100, 100, 0, 0, false, false, currentBiome->fiveTracksDistance, not_count_lines);
            currentBiome->addBiome(100, 150, 150, 0, 0, false, false, currentBiome->threeTracksDistance, not_count_lines);
            currentBiome->addBiome(100, 100, 100, 0, 0, false, false, currentBiome->threeTracksDistance, not_count_lines);
            currentBiome->addBiome(100, 100, 100, 0, 0, false, false, currentBiome->threeTracksDistance, not_count_lines);
            break;
        case GameMode::CONTINUOUS_MODE:
            currentBiome->addBiome(200, 200, 200, 0, 0, false, true, currentBiome->threeTracksDistance, not_count_lines);
            currentBiome->addBiome(200, 200, 200, 0, 0, false, false, currentBiome->threeTracksDistance, not_count_lines);
    }

    // Set the default width road of the scenario with the total length and its last line added
    mapDistance = (int)currentBiome->lines[0]->distance;
	trackLength = (int)(currentBiome->lines.size() * SEGMENTL);
	currentBiome->lastLine = currentBiome->lines.size() - drawDistance;
}



/**
 * Update the map status of the game with the player and traffic cars to be after drawn
 * @param input is the module that has all the configuration of the game
 * @param cars is the vector with all the traffic cars
 * @param p is the player car
 * @param gameStatus is the status of the game
 * @param time is the real time elapsed between each iteration (allows the movement of the cars)
 * @param score is the current score of the player
 * @param levelsToComplete is the total of levels to complete in the game mode
 * @param checkPoint controls if the checkpoint animation has to be displayed
 * @param checkPointDisplayed controls if the checkpoint animation has been already displayed
 * @param treeMapPos is the current scenario where the player is driving in the tree indicator of the hud round (lower right corner)
 * @param level is the current level where the player is driving (1..5)
 * @param startCodeAi is the type of AI that can be assigned possibly to a traffic car
 * @param gameMode is the game mode selected by the player
 */
void Map::updateMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, const float time, int long long& score,
                    const int levelsToComplete, bool& checkPoint, bool& checkPointDisplayed, int& treeMapPos, const int level,
                    int& startCodeAi, const GameMode& gameMode){

    // Update all the traffic cars
    updateCars(input, cars, p, score, startCodeAi);

    // Set the initial position
	iniPosition = position;

	// Check if the player is not stopped
    if (p.getSpeed() > 0.f){
        // Check if the player has crashed against a traffic car
        if (p.getTrafficCrash()){
            // Moves in
            position = position - (p.getSpeed() * 0.5f);
            // Check the initial limit (avoid overflow)
            if (position <= 0)
                position = 0;
        }
        else
            // Advance in the scenario
            position = position + p.getSpeed();
    }

    // Regulate the position between both thresholds (begin and end)
	while (position >= trackLength)
		position -= trackLength;
	while (position < 0)
		position += trackLength;

    // Get the new position of the player in the scenario and compute the new elevation with its position in axis Y
	Line* playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
	p.elevationControl(playerLine->lowerLeftPoint.yPosWorld, playerLine->lowerRightPoint.yPosWorld);
    float playerPerc = (float)(((position + p.getPosZ()) % (int)SEGMENTL) / SEGMENTL);
	p.setPosY((int)(playerLine->lowerLeftPoint.yPosWorld + (playerLine->lowerRightPoint.yPosWorld - playerLine->lowerLeftPoint.yPosWorld) * playerPerc));

	// Check if the player has arrived to the swapping line and not has chosen direction (only one time)
	if (playerLine->index > currentBiome->swapLine && !newBiomeChosen){

        switch (gameMode){
            case GameMode::ORIGINAL_MODE:
                    // Choose the left road
                    if (p.getPlayerRoad() == Player_Road::LEFTROAD){
                        currentBiome->left->end = true;
                        nextBiome = currentBiome->getLeft();
                        treeMapPos += level;
                    }
                    // Choose the right road
                    else if (p.getPlayerRoad() == Player_Road::RIGHTROAD){
                        currentBiome->right->end = true;
                        nextBiome = currentBiome->getRight();
                        treeMapPos += (level + 1);
                    }
                    break;
            case GameMode::CONTINUOUS_MODE:
                nextBiome = currentBiome->getLeft();
                treeMapPos++;
        }


        // LOGICA CONTINUOUS_MODE
        // Selection done
        newBiomeChosen = true;
	}

	// Check if it the final round and the player has finished the last playable scenario
    if (!currentBiome->end && playerLine->index > currentBiome->lastLine){
        // Not game round finished

        // Get the new scenario chosen in the fork period
        currentBiome = nextBiome;

        // Set the floating sprites
        if (!currentBiome->goalBiome)
            Logger::setSpriteScreenY(*currentBiome);

        // Increment the number of scenarios visited
        numBiomesVisited++;

        // If there are more stages to visit deactivate the ending flag
        if (numBiomesVisited != levelsToComplete)
            currentBiome->end = false;

        // Prepare the fork for the next scenario and set its colors
        setMapDistanceAndTrackLength(numBiomesVisited == (levelsToComplete - 1), gameMode);
        setMapColors();

        // Let the swapping animation when the next biome is finished and checkpoint status is deactivated (must be passed)
        swapping = false;
        checkPointDisplayed = false;
        checkPoint = false;
        newBiomeChosen = false;

        if (currentBiome->goalBiome)
            ending = true;

        // Regulate the position of the traffic cars for the new scenario
        for (unsigned int i = 0; i < cars.size(); ++i){
		    int posZ = cars[i]->getPosZ();
		    posZ -= position;
			cars[i]->setPosZ(posZ);
		}

		// Set the player car status for the new scenario
        position = iniPosition = 0;
		trackLength = (int)(currentBiome->lines.size() * SEGMENTL);
		p.setPosY(0);

		// Get the line of the player car in the scenario
		playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
    }

    // Control the reproduction of the animations (starting and goal biomes)
	if (currentBiome->getGoalBiome() && p.getEndAnimation() && playerLine->index >= 500)
        p.setEndAnimation(false);

    // Check if the swapping animation is happening and update it
	if (!currentBiome->biomeSwap && !currentBiome->end && !swapping && playerLine->index > currentBiome->swapLine){

        // Select the next scenario to load
        swapping = true;
        Biome* biome = currentBiome->left;

        if (gameMode == GameMode::ORIGINAL_MODE && p.getPlayerRoad() == Player_Road::RIGHTROAD)
            biome = currentBiome->right;

        // Prepare the backgrounds and colors of the chosen scenario
        backGround2Front = biome->backGroundFront;
        backGround2Back = biome->backGroundBack;
        bg2 = biome->skyBiome;
        sandAux = biome->sandBiome1; sand2Aux = biome->sandBiome2; roadAux = biome->roadBiome1; road2Aux = biome->roadBiome2;
        rumbleAux = biome->rumbleBiome1; rumble2Aux = biome->rumbleBiome2; laneAux = biome->laneBiome1; lane2Aux = biome->laneBiome2;
        rumbleLaneAux = biome->rumbleLane1; rumbleLane2Aux = biome->rumbleLane2;
	}

	// Check if the game is in play round to detect collisions
	if (gameStatus == State::PLAY_ROUND){
        // Check if the player is crashing
        if (p.getCrashing()){
            // Still moving
            if (p.getSpeed() > 0.f){
                // Make spinning effect in the road (depending on the curve direction (changes the direction)
                if (p.getCounter_code_image() >= p.getMaxCounterToChange()){
                    p.setSpeed(p.getSpeed() - p.getLowAccel());
                    if (p.getSpeed() <= 0.f)
                        p.setSpeed(0.f);
                }

                // Calculate the position in axis X (crashing spinning animation)
                float offsetX = (p.getCollisionCurve() == 0.f) ? 1.67f : p.getCollisionCurve();
                if (p.getCollisionDir() >= 0.f){
                    if (offsetX < 0.f)
                        offsetX = -offsetX;

                    p.setPosX(p.getPosX() - MIN(p.getSpeed() / p.getHighMaxSpeed(), 1.f) * 0.6f * offsetX * time);
                }
                else {
                    if (offsetX < 0.f)
                        offsetX = -offsetX;

                    p.setPosX(p.getPosX() + MIN(p.getSpeed() / p.getHighMaxSpeed(), 1.f) * 0.6f * offsetX * time);
                }
            }
            else {
                // Car is stopped (set all the status to let the player boot)
                p.setSpeed(0.f);
                p.setGear();
                p.setLowAccel(p.getLowMaxSpeed() / 6.5f);
                p.setCollisionDir();
                p.setCollisionCurve();
                if (p.getNumAngers() == 3){
                    p.setAngryWoman();
                    p.setTrafficCrash();
                    p.setDrawCar(false);

                    // Relocate the player car in the center of the nearest road
                    float dif = 0.f;
                    if (p.getPlayerRoad() == Player_Road::RIGHTROAD)
                        dif = ((float)mapDistance / (float)ROAD_WIDTH);
                    if (p.getPosX() < -0.05f + dif)
                        p.setPosX(p.getPosX() + 0.012f);
                    else if (p.getPosX() > 0.05f + dif)
                        p.setPosX(p.getPosX() - 0.012f);
                    else {
                        p.setCrashing(false);
                        p.setNumAngers();
                        p.setDrawCar(true);
                        p.setSkidding(false);
                    }
                }
                p.setStateWheelLeft(StateWheel::NORMAL);
                p.setStateWheelRight(StateWheel::NORMAL);
            }
        }
        else if (p.getSpeed() <= 0.f && (p.getOutsideLeftWheelRoad() || p.getOutsideRightWheelRoad())){

            /*
             * Relocate the player car in the center of the nearest road when it stops outside the road
             * (same animation when there is a collision)
             */

            p.setDrawCar(false);
            float dif = 0.f;
            if (p.getPlayerRoad() == Player_Road::RIGHTROAD)
                dif = ((float)mapDistance / (float)ROAD_WIDTH);
            if (p.getPosX() < -0.05f + dif)
                p.setPosX(p.getPosX() + 0.012f);
            else if (p.getPosX() > 0.05f + dif)
                p.setPosX(p.getPosX() - 0.012f);
            else {
                p.setCrashing(false);
                p.setNumAngers();
                p.setDrawCar(true);
                p.setOutsideLeftWheelRoad(false);
                p.setOutsideRightWheelRoad(false);

                if (!Audio::isPlaying(Sfx::BLONDE_WOMAN_HURRY_UP))
                    Audio::play(Sfx::BLONDE_WOMAN_HURRY_UP, false);
            }
        }

        bool hasCrashed = false;

        // Deactivate the flag to show wheels effect (terrain on the road)
        p.setShowTerrain();

        // Check collisions with map elements and traffic cars when there is no one and not is the final scenario
        if (!currentBiome->end && !p.getCrashing()){

            // Check the map elements and detects collision with them until one is true (the rest ignored)
            if (playerLine->hasSpriteFarLeft)
                p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteFarLeft);
            if (!hasCrashed && playerLine->hasSpriteNearLeft)
                p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteNearLeft);
            if (!hasCrashed && playerLine->hasSpriteCenter)
                p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteCenter);
            if (!hasCrashed && playerLine->hasSpriteFarRight)
                p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteFarRight);
            if (!hasCrashed && playerLine->hasSpriteNearRight)
                p.checkCollisionSpriteInfo(input, playerLine, hasCrashed, playerLine->spriteNearRight);

            // If the player has not crashed with the map elements checks collisions with the traffic cars
            if (!hasCrashed){
                for (auto& car : cars){
                    Line* lc = currentBiome->lines[(int)(car->getPosZ() / SEGMENT_LENGTH) % currentBiome->lines.size()];
                    p.checkCollisionTrafficCar(input, playerLine, lc, car, hasCrashed);

                    if (hasCrashed)
                        break;
                }
            }
        }
	}

	// Set the position of the player in axis Y
    playerPerc = (float)(((position + p.getPosZ()) % (int)SEGMENTL) / SEGMENTL);
	p.setPosY((int)(playerLine->lowerLeftPoint.yPosWorld + (playerLine->lowerRightPoint.yPosWorld - playerLine->lowerLeftPoint.yPosWorld) * playerPerc));

	// Check in which road the player is driving (left or right)
	if (abs(playerLine->lowerLeftPoint.xPosCamera) <= abs(playerLine->upperLeftPoint.xPosCamera))
     	p.setPlayerRoad(Player_Road::LEFTROAD);
    else
        p.setPlayerRoad(Player_Road::RIGHTROAD);

    // Check the speed of the car to make centrifugal force event
    if (p.getSpeed() >= 30.5f)
        p.controlCentrifugalForce(playerLine, time, mapDistance);

    // Set the width of the road
	mapDistance = (int)playerLine->distance;

    //Check road limits for player (left road)
    if (p.getPosX() < -1.5f)
        p.setPosX(-1.48f);
    else if (p.getPosX() > 1.5f + ((float)mapDistance / (float)ROAD_WIDTH))
        p.setPosX(1.48f + ((float)mapDistance / (float)ROAD_WIDTH));

        //Check road limits for player (left and right road)
    if (playerLine->mirror){
        if ((float)mapDistance / (float)ROAD_WIDTH > 3.5f && p.getPosX() > 1.75f && p.getPosX() < (float)mapDistance / (float)ROAD_WIDTH - 1.75f){
            if (p.getPlayerRoad() == Player_Road::LEFTROAD)
                // Left road
                p.setPosX(1.73f);
            else
                // Right road
                p.setPosX((float)mapDistance / (float)ROAD_WIDTH - 1.73f);
        }
    }

    // Update the wheel statuses
    if (p.getDrawCar())
        updateCarPlayerWheels(p);

	//Make smoke if sliding to the side when in a huge curve
	if (p.getStateWheelLeft() != StateWheel::SAND && p.getStateWheelRight() != StateWheel::SAND &&
        p.getStateWheelLeft() != StateWheel::GRASS && p.getStateWheelRight() != StateWheel::GRASS &&
        p.getStateWheelLeft() != StateWheel::SNOW && p.getStateWheelRight() != StateWheel::SNOW &&
        p.getStateWheelLeft() != StateWheel::MUD && p.getStateWheelRight() != StateWheel::MUD && p.getSpeed() > 70.f)
	{
	    // Depending on the centrifugal force draws the skidding effect in the player car (smoke in the wheels)
		if (abs(p.getThresholdX()) >= (p.getSpeed() * time * 0.8f) && abs(playerLine->curve) >= 2.75f){
		    p.setSkidding(true);
			p.setStateWheelLeft(StateWheel::SMOKE);
			p.setStateWheelRight(StateWheel::SMOKE);
		}
		else
            p.setSkidding(false);
    }

    // Detect the checkpoint when the scenario is not the starting and goal (there are not checkpoints in that scenarios
	if (!currentBiome->getStartBiome() && !currentBiome->getGoalBiome() &&
        !checkPointDisplayed && playerLine->index > currentBiome->lineCheckPoint)
        // Checkpoint passed
        checkPoint = true;
}



/**
 * Draw the current scenario with all the traffic cars
 * @param input is the module that has all the configuration of the game
 * @param cars is the vector with all the traffic cars
 * @param p is the player car
 * @param gameStatus is the status of the game
 * @param gameMode is the game mode selected by the player
 * @param pauseMode controls if the game is in pause mode or not
 */
void Map::renderMap(Input &input, vector<TrafficCar*> cars, PlayerCar& p, State& gameStatus, GameMode& gameMode, const bool pauseMode){

    // Clear the window
    input.gameWindow.clear();

    // Check if the game is not pause and it's not in game over
    if (gameStatus != State::GAME_OVER && !pauseMode && swapping)
        // If the scenarios are swapping interpolate the colors between them
        interpolateBiomes(input, p, gameMode);

    // Get the position line of the player car in the scenario
    Line* playerLine = currentBiome->lines[(int)((position + p.getPosZ()) / SEGMENTL) % currentBiome->lines.size()];
    Line* baseLine = currentBiome->lines[(int)(position / SEGMENTL) % currentBiome->lines.size()];
    float percent = (float)((position % (int)SEGMENTL) / SEGMENTL);
    float difX = -(baseLine->curve * percent);
    float sumX = 0;
    Line* l;

    // Project the car player from 3d world  into 2d world (screen)
    playerLine->projection(input, playerLine->lowerLeftPoint, (int)((p.getPosX() * ROAD_WIDTH) - sumX),
                           (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

    // Maximum position in axis Y (horizon line)
    float maxY = playerLine->lowerLeftPoint.yPosScreen;

    // Draw the sky of the current scenario
    int x1 = 0, y1 = input.gameWindow.getSize().y;
    int width = input.gameWindow.getSize().x;
    int height = input.gameWindow.getSize().y;
    drawSky(input, x1, y1, width, height, bg);

    // Colors to paint the scenario
    sf::Color sandColor, roadColor, rumbleColor, laneColor, rumbleLaneColor;

    // Loop throughout all the visible lines
    for (int n = 0; n < drawDistance; n++){

        // Get the line
        l = currentBiome->lines[(baseLine->index + n) % currentBiome->lines.size()];
        l->clip = maxY;

        // Depending on it is odd or even select a color or another one
        l->light ? sandColor = sand : sandColor = sand2;
        l->light ? roadColor = road : roadColor = road2;
        l->light ? rumbleColor = rumble : rumbleColor = rumble2;
        l->light ? laneColor = lane : laneColor = lane2;
        l->light ? rumbleLaneColor = rumbleLane : rumbleLaneColor = rumbleLane2;

        // Project the line from 3d world to 2d world (terrain of the scenario)
        l->projection(input, l->lowerLeftPoint, (int)((p.getPosX() * ROAD_WIDTH) - sumX),
                (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

        l->projection(input, l->lowerRightPoint, (int)((p.getPosX() * ROAD_WIDTH) - sumX - difX),
                (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

        // Mirror effect
        if (l->mirror){
            // Project the line from 3d world to 2d world (two road of the scenario)
            l->projection(input, l->upperLeftPoint, (int)((p.getPosX() * ROAD_WIDTH) + sumX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

            l->projection(input, l->upperRightPoint, (int)((p.getPosX() * ROAD_WIDTH) + sumX + difX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);
        }
        else {
            // Project the line from 3d world to 2d world (one road of the scenario)
            l->projection(input, l->upperLeftPoint, (int)((p.getPosX() * ROAD_WIDTH) - sumX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);

            l->projection(input, l->upperRightPoint, (int)((p.getPosX() * ROAD_WIDTH) - sumX - difX - mapDistance),
                 (int)((float)CAMERA_HEIGHT + p.getPosY()), position, CAMERA_DISTANCE);
        }

        // Compute the curve radius direction to generate the curves
        sumX += difX;
        difX += l->curve;

        // Line not visible
        if ((l->lowerLeftPoint.zPosCamera <= CAMERA_DISTANCE) || (l->lowerRightPoint.yPosScreen >= maxY))
            continue;


        // Compute all the vertex points
        short x1 = (short)l->lowerLeftPoint.xPosScreen;
        short x2 = (short)l->lowerRightPoint.xPosScreen;
        short y1 = (short)l->lowerLeftPoint.yPosScreen;
        short y2 = (short)l->lowerRightPoint.yPosScreen;
        short w1 = (short)l->lowerLeftPoint.wPosScreen;
        short w2 = (short)l->lowerRightPoint.wPosScreen;

        short x11 = (short)l->upperLeftPoint.xPosScreen;
        short x21 = (short)l->upperRightPoint.xPosScreen;
        short y11 = (short)l->upperLeftPoint.yPosScreen;
        short y21 = (short)l->upperRightPoint.yPosScreen;
        short w11 = (short)l->upperLeftPoint.wPosScreen;
        short w21 = (short)l->upperRightPoint.wPosScreen;

        /*
         * Draw the line rectangles (grass, road and rumbles)
         */

        drawRelief(input, 0, y1, (int)input.gameWindow.getSize().x, y1, (int)input.gameWindow.getSize().x, y2, 0, y2, sandColor);

        drawRelief(input, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, roadColor);
        drawRelief(input, x11 - w11, y11, x11 + w11, y11, x21 + w21, y21, x21 - w21, y21, roadColor);

        drawRelief(input, x1 - w1 - (int)(w1 / 7), y1, x1 + w1 + (int)(w1 / 7), y1,
            x2 + w2 + (int)(w2 / 7), y2, x2 - w2 - (int)(w2 / 7), y2, rumbleColor);

        drawRelief(input, x11 - w11 - (int)(w11 / 7), y11, x11 + w11 + (int)(w11 / 7), y11,
            x21 + w21 + (int)(w21 / 7), y21, x21 - w21 - (int)(w21 / 7), y21, rumbleColor);

        // Draw lines in road lanes (order matters for joining them together)
        drawRelief(input, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, rumbleLaneColor);
        drawRelief(input, x11 - w11, y11, x11 + w11, y11, x21 + w21, y21, x21 - w21, y21, rumbleLaneColor);

        drawRelief(input, x1 - w1 + (w1 / 18), y1, x1 + w1 - (w1 / 18), y1, x2 + w2 - (w2 / 18), y2,
            x2 - w2 + (w2 / 18), y2, roadColor);

        drawRelief(input, x11 - w11 + (w11 / 18), y11, x11 + w11 - (w11 / 18), y11,
            x21 + w21 - (w21 / 18), y21, x21 - w21 + (w21 / 18), y21, roadColor);

        drawRelief(input, x1 - w1 + (w1 / 18) + (w1 * 16 / 27), y1, x1 + w1 - (w1 / 18) - (w1 * 16 / 27), y1,
            x2 + w2 - (w2 / 18) - (w2 * 16 / 27), y2, x2 - w2 + (w2 / 18) + (w2 * 16 / 27), y2, laneColor);

        drawRelief(input, x11 - w11 + (w11 / 18) + (w11 * 16 / 27), y11, x11 + w11 - (w11 / 18) - (w11 * 16 / 27), y11,
            x21 + w21 - (w21 / 18) - (w21 * 16 / 27), y21, x21 - w21 + (w21 / 18) + (w21 * 16 / 27), y21, laneColor);

        drawRelief(input, x1 - w1 + (w1 / 18) * 2 + (w1 * 16 / 27), y1, x1 + w1 - (w1 / 18)*2 - (w1 * 16 / 27), y1,
            x2 + w2 - (w2 / 18)*2 - (w2 * 16 / 27), y2, x2 - w2 + (w2 / 18) * 2 + (w2 * 16 / 27), y2, roadColor);

        drawRelief(input, x11 - w11 + (w11 / 18) * 2 + (w11 * 16 / 27), y11, x11 + w11 - (w11 / 18) * 2 - (w11 * 16 / 27), y11,
            x21 + w21 - (w21 / 18) * 2 - (w21 * 16 / 27), y21, x21 - w21 + (w21 / 18) * 2 + (w21 * 16 / 27), y21, roadColor);

        maxY = l->lowerRightPoint.yPosScreen;
    }

    /*
     * Position coordinates of both backgrounds
     */

    sf::RectangleShape backgroundShapeSliced, backgroundShapeNewBiome;

    float posBackXFront = currentBiome->backgroundShapeFront.getPosition().x;
    float posBackYFront = currentBiome->backgroundShapeFront.getPosition().y;
    float heightFront = currentBiome->backgroundShapeFront.getSize().y;
    float widthFront = currentBiome->backgroundShapeFront.getSize().x;

    float posBackXBack = currentBiome->backgroundShapeBack.getPosition().x;
    float posBackYBack = currentBiome->backgroundShapeBack.getPosition().y;
    float heightBack = currentBiome->backgroundShapeBack.getSize().y;
    float widthBack = currentBiome->backgroundShapeBack.getSize().x;

    // Not swapping
    if (!swapping){

        // Move the backgrounds when the player is in curve
        if (!pauseMode && !playerLine->mirror && gameStatus != State::GAME_OVER){
            offsetXBackground1 += playerLine->curve * (position - iniPosition) / SEGMENT_LENGTH * 2.0f;
            offsetXBackground2 += playerLine->curve * (position - iniPosition) / SEGMENT_LENGTH * 2.5f;
        }

        /*
         * Calculate the new position, scale and draw it
         * First draw the back background and then the front background
         */

        backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                           MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeSliced.setPosition(posBackXBack + (int)offsetXBackground1,
                                    MAX(posBackYBack, posBackYBack + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));


        backgroundShapeSliced.setTexture(&backGroundBack, true);
        backgroundShapeSliced.setTextureRect(sf::IntRect(posBackXBack + (int)offsetXBackground1,
                                                         MAX(posBackYBack, posBackYBack + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                         input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

        drawBackground(input, 0, (int)(maxY + SCREEN_Y_OFFSET), backgroundShapeSliced, { 1.f, 1.f }, { 0.f, 1.f });



        backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                           MIN(heightFront, (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeSliced.setPosition(posBackXFront + (int)offsetXBackground2,
                                    MAX(posBackYFront, posBackYFront + (heightFront - (int)(maxY + SCREEN_Y_OFFSET))));


        backgroundShapeSliced.setTexture(&backGroundFront, true);
        backgroundShapeSliced.setTextureRect(sf::IntRect(posBackXBack + (int)offsetXBackground2,
                                                         MAX(posBackYFront, posBackYFront + (heightFront - (int)(maxY + SCREEN_Y_OFFSET))),
                                                         input.gameWindow.getSize().x, MIN(heightFront, (int)(maxY + SCREEN_Y_OFFSET))));

        drawBackground(input, 0, (int)(maxY + SCREEN_Y_OFFSET), backgroundShapeSliced, { 1.f, 1.f }, { 0.f, 1.f });

    }
    else {

        /*
         * Swapping effect (draw the four backgrounds)
         * First draw the back background and then the front background of the current scenario
         * Second draw the back background and then the front background of the new scenario selected
         */

        // First background of the map
        backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                           MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeSliced.setPosition(posBackXBack + (int)offsetXBackground1,
                                    MAX(posBackYBack, posBackYBack + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeSliced.setTexture(&backGroundBack, true);
        backgroundShapeSliced.setTextureRect(sf::IntRect(posBackXBack + (int)offsetXBackground1,
                                                         MAX(posBackYBack, posBackYBack + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                         input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

        drawBackground(input, 0 - (int)backgroundSwapOffset, (int)(maxY + SCREEN_Y_OFFSET),
                       backgroundShapeSliced, { 1.f, 1.f }, { 0.f, 1.f });


        // First background of the map
        backgroundShapeSliced.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                           MIN(heightFront, (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeSliced.setPosition(posBackXFront + (int)offsetXBackground2,
                                    MAX(posBackYFront, posBackYFront + (heightFront - (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeSliced.setTexture(&backGroundFront, true);
        backgroundShapeSliced.setTextureRect(sf::IntRect(posBackXFront + (int)offsetXBackground2,
                                                         MAX(posBackYFront, posBackYFront + (heightFront - (int)(maxY + SCREEN_Y_OFFSET))),
                                                         input.gameWindow.getSize().x, MIN(heightFront, (int)(maxY + SCREEN_Y_OFFSET))));

        drawBackground(input, 0 - (int)backgroundSwapOffset, (int)(maxY + SCREEN_Y_OFFSET),
                       backgroundShapeSliced, { 1.f, 1.f }, { 0.f, 1.f });


        // Second background of the map
        backgroundShapeNewBiome.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                           MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeNewBiome.setPosition(posBackXBack + (int)offsetXBackground1,
                                    MAX(posBackYBack, posBackYBack + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeNewBiome.setTexture(&backGround2Back, true);

        backgroundShapeNewBiome.setTextureRect(sf::IntRect(posBackXBack + BACKGROUND_MOVING_OFFSET,
                                                         MAX(posBackYBack, posBackYBack + (heightBack - (int)(maxY + SCREEN_Y_OFFSET))),
                                                         input.gameWindow.getSize().x, MIN(heightBack, (int)(maxY + SCREEN_Y_OFFSET))));

        drawBackground(input, input.gameWindow.getSize().x - (int)backgroundSwapOffset, (int)(maxY + SCREEN_Y_OFFSET),
                       backgroundShapeNewBiome, { 1.f, 1.f }, { 0.f, 1.f });


        // Second background of the map
        backgroundShapeNewBiome.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                           MIN(heightFront, (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeNewBiome.setPosition(posBackXFront + (int)offsetXBackground2,
                                    MAX(posBackYFront, posBackYFront + (heightFront - (int)(maxY + SCREEN_Y_OFFSET))));

        backgroundShapeNewBiome.setTexture(&backGround2Front, true);

        backgroundShapeNewBiome.setTextureRect(sf::IntRect(posBackXFront + BACKGROUND_MOVING_OFFSET,
                                                         MAX(posBackYFront, posBackYFront + (heightFront - (int)(maxY + SCREEN_Y_OFFSET))),
                                                         input.gameWindow.getSize().x, MIN(heightFront, (int)(maxY + SCREEN_Y_OFFSET))));

        drawBackground(input, input.gameWindow.getSize().x - (int)backgroundSwapOffset, (int)(maxY + SCREEN_Y_OFFSET),
                       backgroundShapeNewBiome, { 1.f, 1.f }, { 0.f, 1.f });

    }

    // Check if the game is in play round or end round and the scenario is the starting or goal biome
    if ((currentBiome->getStartBiome() || currentBiome->getGoalBiome()) && (gameStatus == State::PLAY_ROUND || gameStatus == State::END_ROUND) && !Logger::getEndFlaggerAnimation())
        // Update the sprite for play the animation if it has not finished yet
        Logger::updateSprite(*currentBiome, Sprite_Animated::FLAGGER);

    // Draw sprites and cars
    for (int n = (int)(drawDistance - 1); n >= 0; --n){

        // Get the line
        l = currentBiome->lines[(baseLine->index + n) % currentBiome->lines.size()];

        // Line behind the player car (not visible
        if (l->index < playerLine->index)
            continue;

        /*
         * Check what sprites the line has and draw all of them
         */

        if (l->hasSpriteFarLeft)
            l->renderSpriteInfo(input, l->spriteFarLeft);
        if (l->hasSpriteNearLeft)
            l->renderSpriteInfo(input, l->spriteNearLeft);
        if (l->hasSpriteCenter)
            l->renderSpriteInfo(input, l->spriteCenter);
        if (l->hasSpriteFarRight)
            l->renderSpriteInfo(input, l->spriteFarRight);
        if (l->hasSpriteNearRight)
            l->renderSpriteInfo(input, l->spriteNearRight);

        // Check if the scenario is the goal
        if (!currentBiome->goalBiome){
            Line* l2;

            // If it the goal scenario the traffic cars are not drawn (easier ending animation)
            for (unsigned int n = 0; n < cars.size(); ++n){
                // Get the line of the scenario were is the car and if it visible it is drawn in the screen
                l2 = currentBiome->lines[(int)(cars[n]->getPosZ() / SEGMENTL) % currentBiome->lines.size()];
                if (l2->index == l->index && l2->index >= playerLine->index && l2->index < playerLine->index + drawDistance)
                    l2->renderCars(input, cars[n], pauseMode, gameStatus);
            }
        }
    }
}




/**
 * Get the current biome
 */
Biome* Map::getCurrentBiome() const {
    return currentBiome;
}



/**
 * Get the type of terrain outside the road
 */
int Map::getTerrain() const {
    return terrain;
}



/**
 * Get if the current scenario is the starting biome
 */
bool Map::getStartMap() const {
    return currentBiome->startBiome;
}



/**
 * Get if the current scenario is the goal biome
 */
bool Map::getgoalMap() const {
    return currentBiome->goalBiome;
}



/**
 * Get if the ending animation has to be played
 */
bool Map::getEnding() const {
    return ending;
}

