
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
 * Module Score implementation file
 */

#include <fstream>
#include <sstream>
#include "Score.h"


using namespace std;



/**
 * Create a score of a player which is going to be stored
 * @param _score is the score obtained by the player during the game mode
 * @param _name is the nickname introduced by the player to save the record
 * @param _minutes is the quantity of minutes that the game has lasted
 * @param _secs is the quantity of seconds that the game has lasted
 * @param _cents_second is the quantity of hundredths of second that the game has lasted
 */
Score::Score(long long int _score, const std::string _name,
             const int _minutes, const int _secs, const int _cents_second)
{
    // Assign all the information
    score = _score;
    name = _name;
    minutes = _minutes;
    secs = _secs;
    cents_second = _cents_second;
}



/**
 * Returns all score records in order from highest to lowest score, i.e. the best record will be in
 * The size of the vector is between zero (no records) and seven (there are seven records in the
 * positions zero to six)
 * @param input is the module that has all the configuration of the game
 * @param gameMode is the game mode selected by the player
 * @return and array with the best scores registered in the game
 */
vector<Score> getGlobalScores(Input& input, const GameMode& gameMode){

    // Vector of scores
    vector<Score> globalScores;

    // Determine the path depending on the level and traffic difficulty the scores to load
    std::string path = "Resources/Score/";

    switch (gameMode)
    {
        case GameMode::ORIGINAL_MODE:
            path += "/OriginalMode/";
            break;
        case GameMode::CONTINUOUS_MODE:
            path += "/ContinuousMode/";
            break;
        case GameMode::SURVIVAL_MODE:
            path += "/SurvivalMode/";
    }

    // Check difficulty level
    switch(input.difficulty){
        case Level_Difficulty::EASY:
            path = "Resources/Score/scoresEasy";
            break;
        case Level_Difficulty::NORMAL:
            path = "Resources/Score/scoresNormal";
            break;
        case Level_Difficulty::HARD:
            path = "Resources/Score/scoresHard";
    }

    // Check the traffic level
    switch(input.traffic){
        case Level_Traffic::LOW:
            path += "Low.txt";
            break;
        case Level_Traffic::MEDIUM:
            path += "Medium.txt";
            break;
        case Level_Traffic::HIGH:
            path += "High.txt";
    }

    // Open the file with a reading flux
    ifstream fin(path);
    if (fin.is_open()) {
        string line;

        // Iterate the different lines of the file
        for (int i = 0; getline(fin, line) && i < TOTAL_SCORES; i++) {
            unsigned long score;
            string name;
            int minutes;
            int secs;
            int cents_second;
            istringstream iss(line);

            // Read all the data
            iss >> score >> name >> minutes >> secs >> cents_second;

            // Store it in the vector
            globalScores.emplace_back(score, name, minutes, secs, cents_second);
        }
        // Close the file
        fin.close();
    }
    // Return the scores
    return globalScores;
}



/**
 * Given all records in order from highest to lowest score and a score, return the
 * position of the new record (0 to 6) or -1 if not a new record
 * @param globalScores is a vector which stores all the records of the game mode
 * @param score is the score which is going to be test to check if it's a new record or not
 */
int isNewRecord(const vector<Score> &globalScores, unsigned long score) {
    // Index of the current record to be check
    int i;

    // Iterate all the records until its position in the ranking is found
    for (i = 0; i < (int)globalScores.size() && i < TOTAL_SCORES; i++) {
        // If the position of the new record has been found
        if (globalScores[i].score < score)
            // Get the position
            return i;
    }

    // If the score is located in the last position of the ranking
    if (globalScores.size() < TOTAL_SCORES)
        // Get the position
        return i;
    else
        // The new score is not between the best scores of the game mode
        return -1;
}



/**
 * Given all the records of punctuation ordered from highest to lowest punctuation
 * and a new record, add the new record of score and returns true if it has been
 * stored successfully and false if not.
 * @param input is the module that has all the configuration of the game
 * @param globalScores is a vector which stores all the records of the game mode
 * @param newRecord is the new record of the game mode which is going to be stored
 * @param gameMode is the game mode selected by the player
 */
bool saveNewRecord(Input& input, const vector<Score> &globalScores, const Score &newRecord, const GameMode& gameMode) {

    // Controls if the record has been saved correctly
    bool saved = false;

    // Determine the path depending on the level and traffic difficulty the scores to load
    std::string path = "Resources/Score/";

    switch (gameMode)
    {
        case GameMode::ORIGINAL_MODE:
            path += "/OriginalMode/";
            break;
        case GameMode::CONTINUOUS_MODE:
            path += "/ContinuousMode/";
            break;
        case GameMode::SURVIVAL_MODE:
            path += "/SurvivalMode/";
    }

    // Check difficulty level
    switch(input.difficulty){
        case Level_Difficulty::EASY:
            path += "/scoresEasy";
            break;
        case Level_Difficulty::NORMAL:
            path += "scoresNormal";
            break;
        case Level_Difficulty::HARD:
            path += "scoresHard";
    }

    // Check traffic level
    switch(input.traffic){
        case Level_Traffic::LOW:
            path += "Low.txt";
            break;
        case Level_Traffic::MEDIUM:
            path += "Medium.txt";
            break;
        case Level_Traffic::HIGH:
            path += "High.txt";
    }

    // Open the file with a writing flux and check if it has been opened
    ofstream fout(path, ofstream::trunc);

    if (fout.is_open()) {
        bool end = false;
        int i = 0;

        // Loop throughout the record
        for (int j = 0; !end && j < TOTAL_SCORES; j++) {
            // Write the new record
            if (!saved && ((i < (int)globalScores.size() && globalScores[i].score < newRecord.score) ||
                           (i >= (int)globalScores.size())))
            {
                saved = true;
                fout << newRecord.score << " " << newRecord.name << " " << newRecord.minutes << " " << newRecord.secs
                     << " " << newRecord.cents_second << endl;
            }
            else {
                // Write the rest of the records
                fout << globalScores[i].score << " " << globalScores[i].name << " " << globalScores[i].minutes << " "
                     << globalScores[i].secs << " " << globalScores[i].cents_second << endl;
                i++;
            }

            // Finish the process
            if (i >= (int)globalScores.size() && saved)
                end = true;
        }
        // Close the file
        fout.close();
    }
    // Return if the record has been saved or not
    return saved;
}
