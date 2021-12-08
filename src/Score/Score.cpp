
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
 * Module Score implementation file
 */

#include <fstream>
#include <sstream>
#include "Score.h"


using namespace std;

Score::Score(long long int _score, const std::string _name,
             const int _minutes, const int _secs, const int _cents_second)
{
    score = _score;
    name = _name;
    minutes = _minutes;
    secs = _secs;
    cents_second = _cents_second;
}

vector<Score> getGlobalScores(Input& input){

    vector<Score> globalScores;

    string path;

    switch(input.difficulty){
        case Level_Difficulty::EASY:
            path = SCORE_FILE_EASY;
            break;
        case Level_Difficulty::NORMAL:
            path = SCORE_FILE_NORMAL;
            break;
        case Level_Difficulty::HARD:
            path = SCORE_FILE_HARD;
    }

    ifstream fin(path);
    if (fin.is_open()) {
        string line;

        for (int i = 0; getline(fin, line) && i < 7; i++) {
            unsigned long score;
            string name;
            int minutes;
            int secs;
            int cents_second;
            istringstream iss(line);
            iss >> score >> name >> minutes >> secs >> cents_second;

            globalScores.emplace_back(score, name, minutes, secs, cents_second);
        }
        fin.close();
    }
    return globalScores;
}

int isNewRecord(const vector<Score> &globalScores, unsigned long score) {
    int i = 0;
    for (; i < (int)globalScores.size() && i < 7; i++) {
        if (globalScores[i].score < score)
            return i;
    }
    if (globalScores.size() < 7)
        return i;
    else
        return -1;
}

bool saveNewRecord(Input& input, const vector<Score> &globalScores, const Score &newRecord) {

    bool saved = false;

    string path;

    switch(input.difficulty){
        case Level_Difficulty::EASY:
            path = SCORE_FILE_EASY;
            break;
        case Level_Difficulty::NORMAL:
            path = SCORE_FILE_NORMAL;
            break;
        case Level_Difficulty::HARD:
            path = SCORE_FILE_HARD;
    }

    ofstream fout(path, ofstream::trunc);

    if (fout.is_open()) {
        bool end = false;
        int i = 0;

        for (int j = 0; !end && j < 7; j++) {
            if (!saved && ((i < (int)globalScores.size() && globalScores[i].score < newRecord.score) ||
                           (i >= (int)globalScores.size())))
            {
                saved = true;
                fout << newRecord.score << " " << newRecord.name << " " << newRecord.minutes << " " << newRecord.secs
                     << " " << newRecord.cents_second << endl;
            }
            else {
                fout << globalScores[i].score << " " << globalScores[i].name << " " << globalScores[i].minutes << " "
                     << globalScores[i].secs << " " << globalScores[i].cents_second << endl;
                i++;
            }

            if (i >= (int)globalScores.size() && saved)
                end = true;
        }
        fout.close();
    }
    return saved;
}
