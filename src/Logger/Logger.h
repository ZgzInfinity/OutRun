
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
#include "../Scene/Biome/Biome.h"
#include "../Random/Random.h"

#ifndef LOGGER_H
#define LOGGER_H

class Biome;

class Logger {

    private:

        enum class Flagger_Status : int {
            UPPING_FLAG,
            MOVING_FLAG,
            MOVING_HAND,
            MOVING_BACK
        };

        static constexpr const int FLAGGER_FLAG = 2;

        static constexpr const int FLAGGER_HAND = 9;

        static constexpr const int FLAGGER_BACK = 11;

        static constexpr const int FLAGGER_END_ANIMATION = 13;

        static constexpr const int FLAGGER_CHANGE_STATUS = 10;

        static constexpr const int FLAGGER_CHANGE_ANIM = 5;

        static constexpr const int MAX_SEMAPHORE_CODE_IMAGE = 41;

        const std::string LOGGER_PATH_FILE = "Resources/Logger/Logger.txt";

        const std::regex natural_number_regex{"[0-9]+"};

        const std::regex float_number_regex{"-?[0-9]+.?[0-9]*"};

        const std::regex integer_number_regex{"-?[0-9]+"};

        static Logger instance;

        int column;

        int row;

        int semaphore_code_image, flagger_code_image;

        int attemps, waitingFlagger, numIterations, totalTimes, linesOfBiome;

        float widthScreen;

        bool failDetected;

        bool endFlaggerAnimation;

        Flagger_Status previousStatus, status;

        std::ifstream inputFlux;

        std::ofstream outputFlux;

        std::vector<int> floatingSprites;

    public:

        Logger();

        void static setLoggerStatus();

        void static setWidthScreen(const int _widthScreen);

        void static setSpriteScreenY(Biome& m);

        void static setFailDetected(const bool _failDetected);

        bool static getFailDetected();

        bool static checkMapFile(const std::string& pathMapFile);

        bool static checkTimeTerrainRoad(Biome& m);

        bool static checkColors(Biome& m);

        bool static readColor(sf::Color& colorRead);

        bool static checkReliefStraight(Biome& m);

        bool static checkReliefCurve(Biome& m, const bool leftDirection);

        bool static checkReliefHillStraight(Biome& m);

        bool static checkReliefHillCurve(Biome& m, const bool leftDirection);

        void static loadStartBiomeSprites(Biome& m);

        void static loadGoalBiomeSprites(Biome& m);

        bool static checkLevelBiomeSprite(Biome& m, const int startPos, const int endPos, const int incrementor, const int frequency,
                                        bool& indexSpecified, bool& spritesProcessed, const int pos);

        bool static checkLevelBiomeSprites(Biome& m);

        bool static checkBiomeRelief(Biome& m);

        void static updateSprite(Biome& m, const Sprite_Animated spriteAnimated);

        bool static getEndFlaggerAnimation();

        void static close();
};

#endif // LOGGER_H

