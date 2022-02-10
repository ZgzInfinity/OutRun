
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

#include "Biome.h"

Biome::Biome(){
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
	dist3 = 0;
	dist4 = dist3 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist5 = dist4 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist6 = dist5 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist7 = dist6 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	dist8 = dist7 + ((int)ROAD_WIDTH * 16 / 27) + ((int)ROAD_WIDTH / 18);
	distM = dist8 + ((int)ROAD_WIDTH * 16 / 27) * 20 + ((int)ROAD_WIDTH / 18) * 7;

}

void Biome::setTime(const int _time){
    time = _time;
}

void Biome::setTerrain(const int _terrain){
    terrain = _terrain;
}

void Biome::setRoadTerrain(const int _roadTerrain){
    roadTerrain = _roadTerrain;
}

int Biome::getTime() const {
    return time;
}

int Biome::getTerrain() const {
    return terrain;
}


bool Biome::getStartBiome() const {
    return startBiome;
};

bool Biome::getGoalBiome() const {
    return goalBiome;
};

int Biome::getRoadTerrain() const {
    return roadTerrain;
}


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

int Biome::computeRoadTracks(const int numTracks){
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

void Biome::addBiome(int enter, int hold, int leave, float curve, float y, bool mirror, int distance, int& linesOfBiome){

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

	float endY = firstY + y * SEGMENTL;
	int n;

	for (n = 0; n < enter; ++n)
	{
		dist += (int)distPerc;
		addSegment(easeIn(0, curve, (float)n / enter), easeInOut(firstY, endY, (float)n / total), mirror, dist, linesOfBiome);
	}

	for (n = 0; n < hold; ++n)
	{
		dist += (int)distPerc;
        addSegment(curve, easeInOut(firstY, endY, (float)(enter + n) / total), mirror, dist, linesOfBiome);

        if (n == (int)(hold / 2))
			if (mirror && swapLine == 0)
				swapLine = (int)lines.size();
	}

	for (n = 0; n < leave; ++n)
	{
		dist += (int)distPerc;
		addSegment(easeInOut(curve, 0, (float)n / leave), easeInOut(firstY, endY, (float)(enter + hold + n) / total), mirror, dist, linesOfBiome);
	}

	if (mirror && lastLine == 0)
		lastLine = (int)lines.size();
}


float Biome::easeIn(float a, float b, float percent){
	return (a + (b - a) * pow(percent, 2));
}


float Biome::easeInOut(float a, float b, float percent){
	return (a + (b - a) * ((-cos(percent * (float)M_PI) / 2.f) + 0.5f));
}


void Biome::addSegment(float curve, float y, bool mirror, float dist, int& linesOfBiome){
	int n = (int)lines.size();
	Line* l = new Line();

	l->index = n;
	l->p1.zWorld = (float)(n * SEGMENTL);
	l->p11.zWorld = l->p1.zWorld;
	l->p1.yWorld = (lines.size() == 0 ? 0 : lines[lines.size() - 1]->p2.yWorld);
	l->p11.yWorld = l->p1.yWorld;
	l->p2.zWorld = (float)((n + 1) * SEGMENTL);
	l->p21.zWorld = l->p2.zWorld;
	l->p2.yWorld = y;
	l->p21.yWorld = l->p2.yWorld;
	l->light = (int)((n / RUMBLE_LENGTH)) % 2;
	l->curve = curve;
	l->mirror = mirror;
	l->distance = dist;
	lines.push_back(l);

	if (linesOfBiome != NOT_COUNT_LINES)
        linesOfBiome++;
}

void Biome::addSpriteInfo(int line, SpriteInfo* p, const Sprite_Position spritePos, const bool isCheckPoint){
	if (line < (int)lines.size()){
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

void Biome::setBackgroundFront(const string path){
    backGroundFront.loadFromFile(path);
    backGroundFront.setRepeated(true);
    backgroundShapeFront.setPosition(0, 0);
    backgroundShapeFront.setSize(sf::Vector2f(4030, 253));
}

void Biome::setBackgroundBack(const string path){
    backGroundBack.loadFromFile(path);
    backGroundBack.setRepeated(true);
    backgroundShapeBack.setPosition(0, 0);
    backgroundShapeBack.setSize(sf::Vector2f(4030, 253));
}

void Biome::setStartBiome(){
    Logger::loadStartBiomeSprites(*this);
    startBiome = true;
}


void Biome::setGoalBiome(){
    skyBiome = sf::Color(0, 207, 205);
    sandBiome1 = sf::Color(255, 235, 221);
    sandBiome2 = sf::Color(251, 229, 215);
    roadBiome1 = sf::Color(156, 156, 156);
    roadBiome2 = sf::Color(148, 148, 148);
    rumbleBiome1 = sf::Color(247, 247, 247);
    rumbleBiome2 = sf::Color(148, 148, 148);
    laneBiome1 = sf::Color(156, 156, 156);
    laneBiome2 = sf::Color(247, 247, 247);
    rumbleLane1 = sf::Color(156, 156, 156);
    rumbleLane2 = sf::Color(247, 247, 247);

    time = 80;
    int not_count_lines = NOT_COUNT_LINES;
    addBiome(250, 250, 250, 0, 0, false, dist3, not_count_lines);

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

    setBackgroundFront("Resources/Maps/MapStartGoal/front.png");
    setBackgroundBack("Resources/Maps/MapStartGoal/back.png");
}

void Biome::setSpriteScreenY(const int index, const float _offsetY) {
    if (startBiome || goalBiome)
        lines[index]->spriteFarLeft->setOffsetY(_offsetY);
    else {
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

void Biome::loadObjects(const string &path, const vector<string> &objectNames){

    for (const string &objName : objectNames) {

        // Load textures
        sf::Texture t;
        t.loadFromFile(path + objName + ".png");
        objects.push_back(t);

        ifstream fin(path + objName + ".txt");

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

        if (fin.is_open()) {
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

Biome *Biome::getLeft() const {
    return left;
}

Biome *Biome::getRight() const {
    return right;
}

Line* Biome::getLine(const int n, const Line* baseLine) {
    if ((baseLine->index + n) < (int)lines.size() || left == nullptr){
        return lines[(baseLine->index + n) % lines.size()];
    }
    else {
        return left->lines[(n - lines.size()) % left->lines.size()];
    }
}
