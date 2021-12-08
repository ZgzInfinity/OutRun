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
 * Module Score interface file
 */

#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <vector>
#include "../Input/Input.h"

struct Score {

        long long int score;

        std::string name;

        int minutes;

        int secs;

        int cents_second;

        Score(long long int _score, const std::string _name, const int _minutes, const int _secs, const int _cents_second);

};

std::vector<Score> getGlobalScores(Input& input);

int isNewRecord(const std::vector<Score> &globalScores, unsigned long score);

bool saveNewRecord(Input& input, const std::vector<Score> &globalScores, const Score &newRecord);

#endif // SCORE_H
