
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

 /*
 * Module Globals interface file
 */

#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <mutex>
#include <thread>
#include "SFML/Graphics.hpp"

using namespace std;

const int FPS = 60;
const int TIME_CREDIT_SCREEN = 300;

const float M_PI = 3.14159265358979323846264338327950288;
const float SEGMENTL = 150.f;
const float RUMBLE_LENGTH = 3.f;


const pair<const int ,const int> SCREEN_0 = make_pair(921, 691);
const pair<const int ,const int> SCREEN_1 = make_pair(1024, 768);
const pair<const int ,const int> SCREEN_2 = make_pair(1280, 720);
const pair<const int ,const int> SCREEN_3 = make_pair(1366, 768);

const sf::Color BUTTON_IDLE_COLOR = sf::Color(93, 244, 75);
const sf::Color BUTTON_HOVER_COLOR = sf::Color(233, 240, 100);
const sf::Color BUTTON_SELECTED_COLOR = sf::Color(255, 255, 0);

// Configuration -----------
const int SCORE_TRAFFIC_BONUS = 20000;
const int ROAD_WIDTH = 1600;
const int SEGMENT_LENGTH = 150;
const int SCREEN_Y_OFFSET = 130;
const int PLAYER_TEXTURES = 172;
const int TRAFFIC_TEXTURES = 4;
const int SCORE_BONIFICATION = 1000000;
const int BACKGROUND_MOVING_OFFSET = 91;

const float RUMBLE_WIDTH = 1.08f;
const float XINC = 0.06f;
const float FOV = 120.f;
const float CAMERA_DISTANCE = 1 / tan((float)((FOV / 2.f) * M_PI / 180.0f));
const float CAMERA_HEIGHT = 800.f;

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

// Global variables to control the mutual exclusion
extern mutex mainMutex;

#endif // GLOBALS_H
