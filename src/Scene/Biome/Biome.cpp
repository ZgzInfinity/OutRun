
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
 * Implementation file of the module Biome
 */

#include "Biome.h"



/**
 * Default constructor
 */
Biome::Biome(){

    /*
     * Assign all the default configuration of a biome
     */

    left = nullptr;
    right = nullptr;
	end = false;
	biomeSwap = false;
	startBiome = false;
	goalBiome = false;
	swapLine = 0;
	lastLine = 0;
	lineCheckPoint = 0;

	// Set distances for Map lanes
	threeTracksDistance = 0;
	fourTracksDistance = threeTracksDistance + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	fiveTracksDistance = fourTracksDistance + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	sixTracksDistance = fiveTracksDistance + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	sevenTracksDistance = sixTracksDistance + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	eightTracksDistance = sevenTracksDistance + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	twoTracksDistance = eightTracksDistance + ((int)ROAD_WIDTH * 16 / 27) * 20 + ((int)ROAD_WIDTH / 18) * 7;

}



/**
 * Set the time of the scenario to be completed
 * @param _time is the time of the scenario to be completed by the player
 */
void Biome::setTime(const int _time){
    time = _time;
}



/**
 * Set the type of terrain outside the road
 * @param _terrain is the type of terrain (SAND, GRASS, SNOW or MUD)
 * and it is represents with integers from (1..4)
 */
void Biome::setTerrain(const int _terrain){
    terrain = _terrain;
}



/**
 * Set the type of terrain outside the road
 * @param _terrain is the type of terrain (SAND, GRASS, SNOW or MUD)
 * and it is represents with integers from (1..4). The value 0 means
 * that the road is asphalted
 */
void Biome::setRoadTerrain(const int _roadTerrain){
    roadTerrain = _roadTerrain;
}



/**
 * Set the second background of the scenario (moves quicker)
 * @param path is the file where the image is located
 */
void Biome::setBackgroundFront(const string path){
    backGroundFront.loadFromFile(path);
    backGroundFront.setRepeated(true);
    backgroundShapeFront.setPosition(0, 0);
    backgroundShapeFront.setSize(sf::Vector2f(4030, 253));
}



/**
 * Set the second background of the scenario (moves slower)
 * @param path is the file where the image is located
 */
void Biome::setBackgroundBack(const string path){
    backGroundBack.loadFromFile(path);
    backGroundBack.setRepeated(true);
    backgroundShapeBack.setPosition(0, 0);
    backgroundShapeBack.setSize(sf::Vector2f(4030, 253));
}



/**
 * Load all the sprites of the scenario
 * @param path stores the path of the scenario
 * @param objectNames stores the paths from the sprites in that scenario
 */
void Biome::loadObjects(const string &path, const vector<string> &objectNames){

    // Loop of all the sprite paths
    for (const string &objName : objectNames) {

        // Load textures
        sf::Texture t;
        t.loadFromFile(path + objName + ".png");
        objects.push_back(t);

        // Check if the texture has properties file
        ifstream fin(path + objName + ".txt");

        // Default properties configuration
        string detectCollision;
        bool collision = true;
        float scale = 1.f;
        float widthCollision = t.getSize().x;
        int showTerrain = 0;
        fPoint pivotLeft = {1.f, 1.f};
        fPoint pivotRight = {0.f, 1.f};
        fPoint pivotColLeft = {0.5f, 1.f};
        fPoint pivotColRight = {0.5f, 1.f};

        float pointX, pointY;

        // Check if the properties file has been opened
        if (fin.is_open()) {
            // Read all the properties specified and modifies the default parameters
            while (!fin.eof()) {
                string s;
                fin >> s;
                if (s == "COLLISION:" && !fin.eof()){
                    fin >> detectCollision;
                    collision = (detectCollision == "1") ? true : false;
                }
                else if (s == "SCALE:" && !fin.eof()){
                    fin >> scale;
                }
                else if (s == "WIDTH_COLLISION:" && !fin.eof()){
                    fin >> widthCollision;
                }
                else if (s == "PIVOT_LEFT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotLeft = {pointX, pointY};
                }
                else if (s == "PIVOT_RIGHT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotRight = {pointX, pointY};
                }
                else if (s == "PIVOT_COL_LEFT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotColLeft = {pointX, pointY};
                }
                else if (s == "PIVOT_COL_RIGHT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotColRight = {pointX, pointY};
                }
                else if (s == "SHOW_TERRAIN:" && !fin.eof()){
                    fin >>  showTerrain;
                }
                else if (!s.empty()) {
                    cerr << "WARNING: '" << s << "' at file " << path + objName + ".txt" << endl;
                }
            }
            fin.close();
        }

        // Store them in the vectors (the position in them is the identifier of the sprite)
        collisions.push_back(collision);
        scaleCoeffs.push_back(scale);
        widthCollisionCoeffs.push_back(widthCollision);
        showTerrainCoeffs.push_back(showTerrain);
        pivotLeftPoints.push_back(pivotLeft);
        pivotRightPoints.push_back(pivotRight);
        pivotLeftColPoints.push_back(pivotColLeft);
        pivotRightColPoints.push_back(pivotColRight);
    }
}



/**
 * Set the biome like the starting biome
 */
void Biome::setStartBiome(){
    // Load the sprites of the starting animation and activates the flag
    Logger::loadStartBiomeSprites(*this);
    startBiome = true;
}



/**
 * Set the biome like the goal biome
 */
void Biome::setGoalBiome(){

    /*
     * Set the colors of the biome
     */

    skyBiome = sf::Color(0, 207, 205);
    sandBiome1 = sf::Color(255, 235, 221);
    sandBiome2 = sf::Color(251, 229, 215);
    roadBiome1 = sf::Color(156, 156, 156);
    roadBiome2 = sf::Color(148, 148, 148);
    rumbleBiome1 = sf::Color(156, 156, 156);
    rumbleBiome2 = sf::Color(247, 247, 247);
    laneBiome1 = sf::Color(156, 156, 156);
    laneBiome2 = sf::Color(247, 247, 247);
    rumbleLane1 = sf::Color(247, 247, 247);
    rumbleLane2 = sf::Color(148, 148, 148);

    // Set a default time and create a straight road
    time = 80;
    int not_count_lines = NOT_COUNT_LINES;
    addBiome(250, 250, 250, 0, 0, false, threeTracksDistance, not_count_lines);

    // Fill the scenario with the sprites
	vector<string> objectNames;
    objectNames.reserve(45);
    for (int i = 1; i <= 45; i++){
        objectNames.push_back(std::to_string(i));
    }

    string path = "Resources/Maps/MapStartGoal/";

    loadObjects(path, objectNames);
    Logger::loadGoalBiomeSprites(*this);

    goalBiome = true;
    end = true;

    // Set the background of the scenario
    setBackgroundFront("Resources/Maps/MapStartGoal/front.png");
    setBackgroundBack("Resources/Maps/MapStartGoal/back.png");
}



/**
 * Set the colors of the scenario to paint the lines of it
 * @param colorsOfMap is the vector with all the colors of the scenario
 */
void Biome::setColors(const std::vector<sf::Color>& colorsOfMap){
    skyBiome = colorsOfMap[0];
    sandBiome1 = colorsOfMap[1];
    sandBiome2 = colorsOfMap[2];
    roadBiome1 = colorsOfMap[3];
    roadBiome2 = colorsOfMap[4];
    rumbleBiome1 = colorsOfMap[5];
    rumbleBiome2 = colorsOfMap[6];
    laneBiome1 = colorsOfMap[7];
    laneBiome2 = colorsOfMap[8];
    rumbleLane1 = colorsOfMap[9];
    rumbleLane2 = colorsOfMap[10];
}



/**
 * Set the position in axis Y from a floating sprite of the scenario
 * @param index is the identifier of the line whose sprites in axis Y has to be modified
 * @param _offsetY is the new position of the sprite in axis Y
 */
void Biome::setSpriteScreenY(const int index, const float _offsetY){
    // Check if the biome is the starting or goal biome
    if (startBiome || goalBiome)
        // Start and goal flags
        lines[index]->spriteFarLeft->setOffsetY(_offsetY);
    else {
        // Check what sprites has the line and also check if they are floating to update them
        if (lines[index]->hasSpriteFarLeft && lines[index]->spriteFarLeft->getOffsetY() != 0.f)
            lines[index]->spriteFarLeft->setOffsetY(_offsetY);
        if (lines[index]->hasSpriteNearLeft && lines[index]->spriteNearLeft->getOffsetY() != 0.f)
            lines[index]->spriteNearLeft->setOffsetY(_offsetY);
        if (lines[index]->hasSpriteFarRight && lines[index]->spriteFarRight->getOffsetY() != 0.f)
            lines[index]->spriteFarRight->setOffsetY(_offsetY);
        if (lines[index]->hasSpriteNearRight && lines[index]->spriteNearRight->getOffsetY() != 0.f)
            lines[index]->spriteNearRight->setOffsetY(_offsetY);
        if (lines[index]->hasSpriteCenter && lines[index]->spriteCenter->getOffsetY() != 0.f)
            lines[index]->spriteCenter->setOffsetY(_offsetY);
    }
}



/**
 * Determine the distance between tracks that the road is going to have
 * @param numTracks is the number of tracks of that road portion
 */
int Biome::computeRoadTracks(const int numTracks){
    // Return the distance between according to the number of them
    switch (numTracks){
        case 2:
            return twoTracksDistance;
        case 3:
            return threeTracksDistance;
        case 4:
            return fourTracksDistance;
        case 5:
            return fiveTracksDistance;
        case 6:
            return sixTracksDistance;
        case 7:
            return sevenTracksDistance;
        case 8:
            return eightTracksDistance;
    }
}



/**
 * Create a gradual upping slope between to points according to percent slope
 * @param a is the starting reference point of the upping slope
 * @param b is the ending reference point of the upping slope
 * @param percent is the factor to build the upping slope
 */
float Biome::easeIn(float a, float b, float percent){
	return (a + (b - a) * pow(percent, 2));
}




/**
 * Create a gradual descending slope between to points according to percent slope
 * @param a is the starting reference point of the descending slope
 * @param b is the ending reference point of the descending slope
 * @param percent is the factor to build the descending slope
 */
float Biome::easeInOut(float a, float b, float percent){
	return (a + (b - a) * ((-cos(percent * (float)PI) / 2.f) + 0.5f));
}



/**
 * Add a segment part relief to the biome and increments the number of total lines
 * @param curve is the curve coefficient of the new portion
 * @param y is the final position of the relief in axis Y
 * @param mirror controls if the relief has to be duplicated (fork effect)
 * @param dist is the width distance of the road in that part of the relief
 * @param linesOfBiome is the number of lines in the scenario
 */
void Biome::addSegment(float curve, float y, bool mirror, float dist, int& linesOfBiome){

    // Get the last line and insert another one behind
	int n = (int)lines.size();
	Line* l = new Line();

	// Set the index of the line
	l->index = n;

	// Determine the positions of the new line in 3d world
	l->lowerLeftPoint.zPosWorld = (float)(n * SEGMENTL);
	l->upperLeftPoint.zPosWorld = l->lowerLeftPoint.zPosWorld;
	l->lowerLeftPoint.yPosWorld = (lines.size() == 0 ? 0 : lines[lines.size() - 1]->lowerRightPoint.yPosWorld);
	l->upperLeftPoint.yPosWorld = l->lowerLeftPoint.yPosWorld;
	l->lowerRightPoint.zPosWorld = (float)((n + 1) * SEGMENTL);
	l->upperRightPoint.zPosWorld = l->lowerRightPoint.zPosWorld;
	l->lowerRightPoint.yPosWorld = y;
	l->upperRightPoint.yPosWorld = l->lowerRightPoint.yPosWorld;

	// Determine which colors are going to be painted
	l->light = (int)((n / RUMBLE_LENGTH)) % 2;

	// Set the curve radius, the mirror effect and the width road
	l->curve = curve;
	l->mirror = mirror;
	l->distance = dist;

	// Add the new line
	lines.push_back(l);

	// Count the line
	if (linesOfBiome != NOT_COUNT_LINES)
        linesOfBiome++;
}



/**
 * Add a new sub-biome part to the current biome and increments the number of total lines
 * @param enter is the index where the new biome starts
 * @param hold is the length of the new sub-biome part
 * @param leave is the index where the new biome ends
 * @param curve is the curve coefficient of the new portion
 * @param y is the final position of the relief in axis Y
 * @param mirror controls if the relief has to be duplicated (fork effect)
 * @param distance is the width distance of the road in that part of the relief
 * @param linesOfBiome is the number of lines in the scenario
 */
void Biome::addBiome(int enter, int hold, int leave, float curve, float y, bool mirror, int distance, int& linesOfBiome){

    // Determine the length of the new sub-biome
	float firstY, dist, distPerc;
	float total = (float)(enter + hold + leave);

	// Check mirror effect
	if (mirror){
	    // Duplicate the sub-biome
		firstY = (lines.size() == 0 ? 0 : lines[lines.size() - 1]->lowerRightPoint.yPosWorld);
		dist = lines[lines.size() - 1]->distance;
		distPerc = (float)((twoTracksDistance - dist)) / total;
	}
	else{
        // Starting the biome (first subb-biome)
		if (lines.size() == 0){
			firstY = 0;
			dist = (float)distance;
			distPerc = 0;
		}
		else{
		    // Continues from the last part inserted
			firstY = lines[lines.size() - 1]->lowerRightPoint.yPosWorld;
			dist = lines[lines.size() - 1]->distance;
			distPerc = (float)((distance - dist)) / total;
		}
	}

	// Compute the new final
	float endY = firstY + y * SEGMENTL;
	int n;

	/*
	 * Add the new sub-biome to the current biome
	 */

	for (n = 0; n < enter; ++n){
		dist += (int)distPerc;
		addSegment(easeIn(0, curve, (float)n / enter), easeInOut(firstY, endY, (float)n / total), mirror, dist, linesOfBiome);
	}

	for (n = 0; n < hold; ++n){
		dist += (int)distPerc;
        addSegment(curve, easeInOut(firstY, endY, (float)(enter + n) / total), mirror, dist, linesOfBiome);

        if (n == (int)(hold / 2))
			if (mirror && swapLine == 0)
				swapLine = (int)lines.size();
	}

	for (n = 0; n < leave; ++n){
		dist += (int)distPerc;
		addSegment(easeInOut(curve, 0, (float)n / leave), easeInOut(firstY, endY, (float)(enter + hold + n) / total), mirror, dist, linesOfBiome);
	}

	// Determine the last line of the sub-bbiome
	if (mirror && lastLine == 0)
		lastLine = (int)lines.size();
}






/**
 * Add a sprite to the biome
 * @param line is the index of the line were the sprite has to be inserted
 * @param spritePos is the position of the sprite in the road using like reference the road
 * @param isCheckPoint controls if the sprite to add is the checkpoint
 */
void Biome::addSpriteInfo(int line, SpriteInfo* p, const Sprite_Position spritePos, const bool isCheckPoint){
    // Check first if the line in inside the scenario
	if (line < (int)lines.size()){
        // Check the position of the sprite to it and it is the checkpoint actives the flag
        switch (spritePos){
            case Sprite_Position::FAR_LEFT:
                lines[line]->spriteFarLeft = p;
                lines[line]->hasSpriteFarLeft = true;

                if (isCheckPoint)
                    lineCheckPoint = line;

                break;
            case Sprite_Position::FAR_RIGHT:
                lines[line]->spriteFarRight = p;
                lines[line]->hasSpriteFarRight = true;

                if (isCheckPoint)
                    lineCheckPoint = line;

                break;
            case Sprite_Position::NEAR_LEFT:
                lines[line]->spriteNearLeft = p;
                lines[line]->hasSpriteNearLeft = true;

                if (isCheckPoint)
                    lineCheckPoint = line;

                break;
            case Sprite_Position::NEAR_RIGHT:
                lines[line]->spriteNearRight = p;
                lines[line]->hasSpriteNearRight = true;

                if (isCheckPoint)
                    lineCheckPoint = line;

                break;
            case Sprite_Position::CENTER:
                lines[line]->spriteCenter = p;
                lines[line]->hasSpriteCenter = true;

                if (isCheckPoint)
                    lineCheckPoint = line;
        }
	}
}



/**
 * Get the time of the scenario
 */
int Biome::getTime() const {
    return time;
}



/**
 * Get the type of terrain of the scenario outside the road
 */
int Biome::getTerrain() const {
    return terrain;
}



/**
 * Get the type of terrain of the road
 */
int Biome::getRoadTerrain() const {
    return roadTerrain;
}



/**
 * Get if the biome is the starting scenario
 */
bool Biome::getStartBiome() const {
    return startBiome;
};



/**
 * Get if the biome is the goal scenario
 */
bool Biome::getGoalBiome() const {
    return goalBiome;
};



/**
 * Get the left scenario in the fork
 */
Biome *Biome::getLeft() const {
    return left;
}



/**
 * Get the right scenario in the fork
 */
Biome *Biome::getRight() const {
    return right;
}



/**
 * Return a concrete line of the scenario
 * @param n is the index of the line to get
 * @param baseline is the reference line of the scenario to look for the desire line
 */
Line* Biome::getLine(const int n, const Line* baseLine) {
    if ((baseLine->index + n) < (int)lines.size() || left == nullptr)
        return lines[(baseLine->index + n) % lines.size()];
    else
        return left->lines[(n - lines.size()) % left->lines.size()];
}
