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
 * Module Player implementation file
 */

#include "TrafficCar.h"



/**
 * Default constructor
 */
TrafficCar::TrafficCar() : Vehicle(){}



/**
 * Read the properties associated to the traffic car and assign them to it
 * @param name is the path of the file that contains the properties of the car
 */
void TrafficCar::readProperties(const std::string& name){
    // Open the properties file
    ifstream fin("Resources/Vehicles/" + name + "/Car" + std::to_string(id) + ".txt");
    // If it is open read the content
    if (fin.is_open()){
        // Read the contain line per line
        while (!fin.eof()) {
            string s;
            fin >> s;
            // Read the scale of the car to the draw the sprites of the car
            if (s == "SCALE:" && !fin.eof())
                fin >> scale;
            else if (!s.empty())
                cerr << "WARNING: '" << s << "' at file "
                     << "Resources/Vehicles/" << name << "/Car" << std::to_string(id) << ".txt" << endl;
        }
        // Close the file
        fin.close();
    }
}



/**
 * Second constructor
 * @param _posX is the position of the car in axis X
 * @param _posY is the position of the car in axis Y
 * @param _posZ is the position of the car in axis Z
 * @param _speed is the initial speed of the car
 * @param _name is the path where the images of the car are stored
 * @param _id is the identifier of the traffic car
 * @param _offset is the distance between the position of the car in axis X and the center of the road
 * @param _active controls if the traffic car must be drawn or not
 * @param _side controls if the car is in the left or right road
 * @param _isTrafficCar controls if the car is a traffic car or not (player car).
 * @param _startCodeAi is the index of the AI to be assigned to the traffic car
 */
TrafficCar::TrafficCar(const int _posX, const int _posY, const int _posZ, const float _speed, const std::string _name,
                       const int _id, const float _offset, const bool _active, const bool _side, const bool _isTrafficCar,
                       const int startCodeAi) : Vehicle( _posX, _posY, _posZ, _speed, 1.f, _name, _isTrafficCar)
{

    // Set all the main information
    id = _id;
    offset = _offset;
    active = _active;
    side = _side;
    pathSelected = -1;
    offsetDest = -1000.f;
    timeToReturn = 0;

    // Set the AI type of the of the traffic car
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

    // Configuration of the properties of the car (scale)
    readProperties(_name);
}



/**
 * Set the car active (true) or inactive (false) to be drawn or not
 * @param _active means if the car has to be drawn or not (true or false)
 */
void TrafficCar::setActive(const bool _active) {
    active = _active;
}



/**
 * Set the car in the left (false) or right (true) road
 * @param _side represents the road where the car must be played (left or right)
 */
void TrafficCar::setSide(const bool _side){
    side = _side;
}



/**
 * Set the distance between the center of the road and the position of the car in axis X
 * @param _offset is distance between the center of the road and the position of the car in axis X
 */
void TrafficCar::setOffset(const float _offset){
    offset = _offset;
}



/**
 * Set if the player car is near or not to the traffic car
 * @param _playerClosed shows if the player car is near or not to the traffic car
 */
void TrafficCar::setPlayerClosed(const bool _playerClosed){
    playerClosed = _playerClosed;
}



/**
 * Set the type of AI of the traffic car to compute its movements
 * @param startCodeAi is the index of the AI to be assigned to the traffic car
 */
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


/**
 * Get if the car is active or not
 */
bool TrafficCar::getActive() const {
    return active;
}



/**
 * Get if the car is on the left road (false) or right road (true)
 */
bool TrafficCar::getSide() const {
    return side;
}



/**
 * Get the distance between the center of the road and the position of the car in axis X
 */
float TrafficCar::getOffset() const {
    return offset;
}



/**
 * Get if the player car is near or not to the traffic car
 */
bool TrafficCar::getPlayerClosed() const {
    return playerClosed;
}



/**
 * Get the identifier of the car
 */
int TrafficCar::getId() const {
    return id;
}



/**
 * Get the scale factor to draw the sprites of the car
 */
float TrafficCar::getScale() const {
    return scale;
}



/**
 * Control the movement that the traffic car has to do depedin on the player car positions and also on its type of AI
 * @param p is the player car
 * @param playerLine is the line in the biome where the player car is located
 * @param trafficCarLine is the line in the biome where the traffic car is located
 */
void TrafficCar::controlAiTrack(const PlayerCar& p, const Line* playerLine, const Line* trafficCarLine){

    // Check if the traffic car has to move away from the player car when it is near (type of AI EASIVE)
    if (typeAi == Traffic_Ai::EVASIVE && abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y){

        // Check if there if path (left or right to move)
        if (pathSelected != -1){
            // Path left
            if (pathSelected == 0){
                // Check if the destiny point has been specified
                if (offsetDest == -1000.f)
                    // Establish it inside the road
                    offsetDest = offset - 0.5f;
                    if (offsetDest < -0.9f)
                        offsetDest = -0.9f;
                else {
                    // Destiny point determined
                    if (offset > offsetDest){
                        // Move and check if it has been reached
                        offset -= 0.005f;
                        if (offset <= offsetDest){
                            // Reached and start counting time to make another movement
                            offset = offsetDest;
                            offsetDest = -1000.f;
                            if (timeToReturn < 200)
                                timeToReturn++;
                            else {
                                // Let another movement
                                timeToReturn = 0;
                                pathSelected = -1;
                            }
                        }
                    }
                }
            }
            // Path right
            else if (pathSelected == 1){
                // Check if the destiny point has been specified
                if (offsetDest == -1000.f)
                    // Establish it inside the road
                    offsetDest = offset + 0.5f;
                    if (offsetDest > 0.9f)
                        offsetDest = 0.9f;
                else {
                    // Destiny point determined
                    if (offset < offsetDest){
                        // Move and check if it has been reached
                        offset += 0.005f;
                        if (offset >= offsetDest){
                            // Reached and start counting time to make another movement
                            offset = offsetDest;
                            offsetDest = -1000.f;
                            if (timeToReturn < 200)
                                timeToReturn++;
                            else {
                                // Let another movement
                                timeToReturn = 0;
                                pathSelected = -1;
                            }
                        }
                    }
                }
            }
        }
        else {
            // Path not determined

            // Compute where extreme of the road is farthest (left or right)
            float distXLeft = offset + 0.9f;
            float distXRight = 0.9f - offset;

            // Select the farthest extreme
            if (distXLeft > distXRight)
                pathSelected = 0;
            else if (distXLeft < distXRight)
                pathSelected = 1;
            else
                // If both are at the same distance select one randomly
                pathSelected = random_int(0 , 1);
        }

    }
    else {
        // AI type is not EVASIVE
        if (side){
            // Check if both (player car and traffic car are in the right road)
            if (p.getPlayerRoad() == Player_Road::RIGHTROAD){
                // Check if the traffic car is on the right side of the player car
                if (posX > p.getPosX() * ROAD_WIDTH &&
                    abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    // If the type is OBSTACLE move in front of the player car (left move)
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
                // Check if the traffic car is on the left side of the player car
                else if (posX < p.getPosX() * ROAD_WIDTH &&
                         abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    // If the type is OBSTACLE move in front of the player car (right move)
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
            // Check if both (player car and traffic car are in the right road)
            if (p.getPlayerRoad() == Player_Road::LEFTROAD){
                // Check if the traffic car is on the right side of the player car
                if (posX > p.getPosX() * ROAD_WIDTH &&
                    abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    // If the type is OBSTACLE move in front of the player car (right move)
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
                // Check if the traffic car is on the left side of the player car
                else if (posX < p.getPosX() * ROAD_WIDTH &&
                         abs(trafficCarLine->index - playerLine->index) <= MINIMUM_DISTANCE_Y)
                {
                    // If the type is OBSTACLE move in front of the player car (right move)
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



/**
 * Set the index of the sprite that has to be drawn
 * @param pauseMode controls if the game is in pause or not
 * @param gameStatus is the status of the game
 */
void TrafficCar::draw(const bool pauseMode, const State gameStatus) {

    // Check if the sprite has to be updated and restarts the counter
    if (counter_code_image >= maxCounterToChange) {
        counter_code_image = 0;

        /*
         * Check if the vehicle is moving and is not in pause and also not in game over
         * In these cases the vehicle have to be unmodified
         */

        if (speed > 0.f && !pauseMode && gameStatus != State::GAME_OVER)
            // Update the index of the texture to be drawn
            current_code_image++;

        /*
         * Depending of the distance to the car the sprites change to
         * show graphically that proximity between them (traffic and player cars)
         */

        // Check if all the textures have been loaded correctly
        if (textures.size() == TRAFFIC_TEXTURES) {
            // Flat terrain
            if (elevation == Elevation::FLAT){
                // Player car closed
                if (playerClosed){
                    // Right direction
                    if (direction == Direction::TURNRIGHT) {
                        if (current_code_image < 1 || current_code_image > 2)
                            current_code_image = 1;
                    }
                    // Left direction
                    else if (direction == Direction::TURNLEFT) {
                        if (current_code_image < 3 || current_code_image > 4)
                            current_code_image = 3;
                    }
                }
                // Player car far
                else {
                    // Right direction
                    if (direction == Direction::TURNRIGHT) {
                        if (current_code_image < 13 || current_code_image > 14)
                            current_code_image = 13;
                    }
                    // Left direction
                    else if (direction == Direction::TURNLEFT) {
                        if (current_code_image < 15 || current_code_image > 16)
                            current_code_image = 15;
                    }
                }
            }
            // Up elevation
            else if (elevation == Elevation::UP){
                // Right direction
                if (direction == Direction::TURNRIGHT) {
                    if (current_code_image < 5 || current_code_image > 6)
                        current_code_image = 5;
                }
                // Left direction
                else if (direction == Direction::TURNLEFT) {
                    if (current_code_image < 7 || current_code_image > 8)
                        current_code_image = 7;
                }
            }
            // Down elevation
            else if (elevation == Elevation::DOWN){
                // Right direction
                if (direction == Direction::TURNRIGHT) {
                    if (current_code_image < 9 || current_code_image > 10)
                        current_code_image = 9;
                }
                // Left direction
                else if (direction == Direction::TURNLEFT) {
                    if (current_code_image < 11 || current_code_image > 12)
                        current_code_image = 11;
                }
            }
        }
    }
    else
        // Increment the counter to update the sprite of the car
        counter_code_image++;
}








