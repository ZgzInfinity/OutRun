/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
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
 * Module Player implementation file
 */

#include "TrafficCar.h"


TrafficCar::TrafficCar() : Vehicle(){}


TrafficCar::TrafficCar(const int _posX, const int _posY, const int _posZ, const float _speed, const std::string& name,
                       const int& _id, const float& _offset, const bool& _active, const bool& _side, const bool _isTrafficCar,
                       const int startCodeAi) : Vehicle( _posX, _posY, _posZ, _speed, 1.f, name, _isTrafficCar)
{
    id = _id;
    offset = _offset;
    active = _active;
    side = _side;
    pathSelected = -1;
    offsetDest = -1000.f;
    timeToReturn = 0;

    switch (startCodeAi){
        case 1:
            typeAi = Traffic_Ai::PACIFIC;
            break;
        case 2:
            typeAi = Traffic_Ai::EVASIVE;
            break;
        case 3:
            typeAi = Traffic_Ai::OBSTACLE;
    }

    readProperties(name);
}

void TrafficCar::readProperties(const std::string& name){
    ifstream fin("Resources/Vehicles/" + name + "/Car" + std::to_string(id) + ".txt");
    if (fin.is_open()) {
        while (!fin.eof()) {
            string s;
            fin >> s;
            if (s == "SCALE:" && !fin.eof())
                fin >> scale;
            else if (!s.empty())
                cerr << "WARNING: '" << s << "' at file "
                     << "Resources/Vehicles/" << name << "/Car" << std::to_string(id) << ".txt" << endl;
        }
        fin.close();
    }
}

void TrafficCar::setActive(const bool& _active) {
    active = _active;
}

bool TrafficCar::getActive() const {
    return active;
}

void TrafficCar::setSide(const bool& _side){
    side = _side;
}

bool TrafficCar::getSide() const {
    return side;
}

void TrafficCar::setOffset(const float& _offset){
    offset = _offset;
}

float TrafficCar::getOffset() const {
    return offset;
}

int TrafficCar::getId() const {
    return id;
}

/**
 * Update the sprite of the enemy vehicle.
 * @param e is the current elevation of the terrain where is the camera
 * @param camX is the position of the camera in the axis x
 */
void TrafficCar::draw(const bool pauseMode, const State gameStatus) {

    if (counter_code_image >= maxCounterToChange) {
        counter_code_image = 0;

        if (speed > 0.f && !pauseMode && gameStatus != State::GAME_OVER)
            current_code_image++;

        if (textures.size() == TRAFFIC_TEXTURES) {
            if (elevation == Elevation::FLAT){
                if (playerClosed){
                    if (direction == Direction::TURNRIGHT) {
                        if (current_code_image < 1 || current_code_image > 2)
                            current_code_image = 1;
                    }
                    else if (direction == Direction::TURNLEFT) {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                }
                else {
                    if (direction == Direction::TURNRIGHT) {
                        if (current_code_image < 13 || current_code_image > 14)
                            current_code_image = 13;
                    }
                    else if (direction == Direction::TURNLEFT) {
                        if (current_code_image < 15 || current_code_image > 16)
                            current_code_image = 15;
                    }
                }
            }
            else if (elevation == Elevation::UP){
                if (direction == Direction::TURNRIGHT) {
                    if (current_code_image < 5 || current_code_image > 6)
                        current_code_image = 5;
                }
                else if (direction == Direction::TURNLEFT) {
                    if (current_code_image < 7 || current_code_image > 8)
                        current_code_image = 7;
                }
            }
            else if (elevation == Elevation::DOWN){
                if (direction == Direction::TURNRIGHT) {
                    if (current_code_image < 9 || current_code_image > 10)
                        current_code_image = 9;
                }
                else if (direction == Direction::TURNLEFT) {
                    if (current_code_image < 11 || current_code_image > 12)
                        current_code_image = 11;
                }
            }
        }
    }
    else
        counter_code_image++;
}


void TrafficCar::setPlayerClosed(const bool _playerClosed){
    playerClosed = _playerClosed;
}

bool TrafficCar::getPlayerClosed() const {
    return playerClosed;
}

float TrafficCar::getScale() const {
    return scale;
}

void TrafficCar::controlAiTrack(const PlayerCar& p, const Line* playerLine, const Line* trafficCarLine){
    if (typeAi == Traffic_Ai::EVASIVE && abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y){
        if (pathSelected != -1){
            if (pathSelected == 0){
                if (offsetDest == -1000.f)
                    offsetDest = offset - 0.5f;
                    if (offsetDest < -0.9f)
                        offsetDest = -0.9f;
                else {
                    if (offset > offsetDest){
                        offset -= 0.005f;
                        if (offset <= offsetDest){
                            offset = offsetDest;
                            offsetDest = -1000.f;
                            if (timeToReturn < 200)
                                timeToReturn++;
                            else {
                                timeToReturn = 0;
                                pathSelected = -1;
                            }
                        }
                    }
                }
            }
            else if (pathSelected == 1){
                if (offsetDest == -1000.f)
                    offsetDest = offset + 0.5f;
                    if (offsetDest > 0.9f)
                        offsetDest = 0.9f;
                else {
                    if (offset < offsetDest){
                        offset += 0.005f;
                        if (offset >= offsetDest){
                            offset = offsetDest;
                            offsetDest = -1000.f;
                            if (timeToReturn < 200)
                                timeToReturn++;
                            else {
                                timeToReturn = 0;
                                pathSelected = -1;
                            }
                        }
                    }
                }
            }
        }
        else {
            float distXLeft = offset + 0.9f;
            float distXRight = 0.9f - offset;

            if (distXLeft > distXRight)
                pathSelected = 0;
            else if (distXLeft < distXRight)
                pathSelected = 1;
            else
                pathSelected = random_int(0 , 1);
        }

    }
    else {
        if (side){
            if (p.getPlayerMap() == playerR::RIGHTROAD){
                if (posX > p.getPosX() * ROAD_WIDTH &&
                    abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    switch (typeAi){
                        case Traffic_Ai::OBSTACLE:
                            if (offset > -0.9f){
                                offset -= 0.005f;
                                if (offset < -0.9f)
                                    offset = -0.9f;

                            }
                            break;
                    }
                }
                else if (posX < p.getPosX() * ROAD_WIDTH &&
                         abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    switch (typeAi){
                        case Traffic_Ai::OBSTACLE:
                            if (offset < 0.9f){
                                offset += 0.005f;
                                if (offset > 0.9f)
                                    offset = 0.9f;
                            }
                            break;
                    }
                }
            }
        }
        else {
            if (p.getPlayerMap() == playerR::LEFTROAD){
                if (posX > p.getPosX() * ROAD_WIDTH &&
                    abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    switch (typeAi){
                        case Traffic_Ai::OBSTACLE:
                            if (offset > -0.9f){
                                offset -= 0.005f;
                                if (offset < -0.9f)
                                    offset = -0.9f;

                            }
                            break;
                    }
                }
                else if (posX < p.getPosX() * ROAD_WIDTH &&
                         abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    switch (typeAi){
                        case Traffic_Ai::OBSTACLE:
                            if (offset < 0.9f){
                                offset += 0.005f;
                                if (offset > 0.9f)
                                    offset = 0.9f;
                            }
                            break;
                    }
                }
            }
        }
    }
}


void TrafficCar::setAi(const int startCodeAi){
     switch (startCodeAi){
        case 1:
            typeAi = Traffic_Ai::PACIFIC;
            break;
        case 2:
            typeAi = Traffic_Ai::EVASIVE;
            break;
        case 3:
            typeAi = Traffic_Ai::OBSTACLE;
    }
}

