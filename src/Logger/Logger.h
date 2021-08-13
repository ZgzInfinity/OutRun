
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

#pragma once

#include <regex>
#include <fstream>
#include "../Scene/Map/Map.h"

#ifndef LOGGER_H
#define LOGGER_H

class Logger {

    private:

        const std::string LOGGER_PATH_FILE = "Resources/Logger/Logger.txt";

        const std::regex natural_number_regex{"[0-9]+"};

        const std::regex float_number_regex{"-?[0-9]+.?[0-9]+"};

        static Logger instance;

        int column;

        int row;

        bool failDetected;

        std::ifstream inputFlux;

        std::ofstream outputFlux;

    public:

        Logger();

        bool checkMapFile(const std::string& pathMapFile);

        bool checkTimeAndTerrain(Map& m);

        bool checkColors(Map& m);

        bool readColor(sf::Color& colorRead);


};

#endif // LOGGER_H

