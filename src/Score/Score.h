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
 * Module Score interface file
 */

#pragma once

#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <vector>
#include "../Input/Input.h"
#include "../Menus/Menu/Menu.h"



/**
 * Represents a score record, with the player's name and timestamp.
 */
struct Score {

        // Score obtained by the player
        long long int score;

        // Name of the player
        std::string name;

        // Minutes that the game has lasted
        int minutes;

        // Seconds that the game has lasted
        int secs;

        // Hundredths of seconds that the game has lasted
        int cents_second;



        /**
         * Create a score of a player which is going to be stored
         * @param _score is the score obtained by the player during the game mode
         * @param _name is the nickname introduced by the player to save the record
         * @param _minutes is the quantity of minutes that the game has lasted
         * @param _secs is the quantity of seconds that the game has lasted
         * @param _cents_second is the quantity of hundredths of second that the game has lasted
         */
        Score(long long int _score, const std::string _name, const int _minutes, const int _secs, const int _cents_second);

};



/**
 * Returns all score records in order from highest to lowest score, i.e. the best record will be in
 * The size of the vector is between zero (no records) and seven (there are seven records in the
 * positions zero to six)
 * @param input is the module that has all the configuration of the game
 * @param gameMode is the game mode selected by the player
 * @return and array with the best scores registered in the game
 */
std::vector<Score> getGlobalScores(Input& input, const GameMode& gameMode);



/**
 * Given all records in order from highest to lowest score and a score, return the
 * position of the new record (0 to 6) or -1 if not a new record
 * @param globalScores is a vector which stores all the records of the game mode
 * @param score is the score which is going to be test to check if it's a new record or not
 */
int isNewRecord(const std::vector<Score> &globalScores, unsigned long score);



/**
 * Given all the records of punctuation ordered from highest to lowest punctuation
 * and a new record, add the new record of score and returns true if it has been
 * stored successfully and false if not.
 * @param input is the module that has all the configuration of the game
 * @param globalScores is a vector which stores all the records of the game mode
 * @param newRecord is the new record of the game mode which is going to be stored
 * @param gameMode is the game mode selected by the player
 */
bool saveNewRecord(Input& input, const std::vector<Score> &globalScores, const Score &newRecord, const GameMode& gameMode);

#endif // SCORE_H
