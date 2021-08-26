
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

#include <iostream>

using namespace std;

#include "Input/Input.h"
#include "Game/Game.h"
#include "Logger/Logger.h"

int main(){

    Logger::checkMapFile("Resources/Maps/Map1/map.txt");

    Map m;

    Logger::setFailDetected(Logger::checkTimeAndTerrain(m));

    if (!Logger::getFailDetected())
        Logger::setFailDetected(Logger::checkColors(m));

    if (!Logger::getFailDetected())
        Logger::setFailDetected(Logger::checkMapRelief(m));

    Input input;

    Game g = Game(input);

    g.run(input);

    return 0;
}
