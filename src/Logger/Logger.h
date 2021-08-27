
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

        const std::regex float_number_regex{"-?[0-9]+.?[0-9]*"};

        const std::regex integer_number_regex{"-?[0-9]+"};

        static Logger instance;

        int column;

        int row;

        bool failDetected;

        std::ifstream inputFlux;

        std::ofstream outputFlux;

        vector<sf::Texture> objects;

        vector<float> scaleCoeffs;

        vector<int> widthCollisionCoeffs;

        vector<fPoint> pivotLeftPoints;

        vector<fPoint> pivotRightPoints;

        vector<fPoint> pivotLeftColPoints;

        vector<fPoint> pivotRightColPoints;

    public:

        Logger();

        void static loadObjects(const string &path, const vector<string> &objectNames);

        void static setFailDetected(const bool _failDetected);

        bool static getFailDetected();

        bool static checkMapFile(const std::string& pathMapFile);

        bool static checkTimeAndTerrain(Map& m);

        bool static checkColors(Map& m);

        bool static readColor(sf::Color& colorRead);

        bool static checkReliefStraight(Map& m);

        bool static checkReliefCurve(Map& m);

        bool static checkReliefHillStraight(Map& m);

        bool static checkReliefHillCurve(Map& m);

        bool static checkMapSprites(Map& m);

        bool static checkMapRelief(Map& m);
};

#endif // LOGGER_H

