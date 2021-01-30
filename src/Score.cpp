/******************************************************************************
 * @file    Score.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/


 /*
 * Module Score implementation file
 */

#include <fstream>
#include <sstream>
#include "../include/Score.hpp"


using namespace std;



/**
 * Create a record of a player which is going to be stored
 * @param score is the score obtained by the player during the game mode
 * @param name is the nickname introduced by the player to save the record
 * @param minutes is the quantity of minutes that the game has lasted
 * @param secs is the quantity of seconds that the game has lasted
 * @param centsSecond is the quantity of hundredths of second that the game has lasted
 */
Score::Score(unsigned long score, const string &name, int minutes, int secs, int centsSecond) : score(score),
                                                                                                name(name),
                                                                                                minutes(minutes),
                                                                                                secs(secs),
                                                                                                cents_second(
                                                                                                centsSecond) {}


/**
 * Returns all score records in order from highest to lowest score, i.e. the best record will be in
 * The size of the vector is between zero (no records) and seven (there are seven records in the
 * positions zero to six)
 * @param c is the module configuration of the game
 * @param typeOfGame is the game mode selected by the player
 * @return
 */
vector<Score> getGlobalScores(Config& c) {

    // Vector to store the records
    vector<Score> globalScores;

    string path;

    // Check the path the records depending on the difficulty
    switch(c.level){
        case PEACEFUL:
            path = "Resources/Score/scoresPeaceful.info";
            break;
        case EASY:
            path = "Resources/Score/scoresEasy.info";
            break;
        case NORMAL:
            path = "Resources/Score/scoresNormal.info";
            break;
        case HARD:
            path = "Resources/Score/scoresHard.info";
    }

    // Open the file to read the records
    ifstream fin(path);
    if (fin.is_open()) {
        string line;
        // Read the file line by line
        for (int i = 0; getline(fin, line) && i < 7; i++) {
            unsigned long score;
            string name;
            int minutes;
            int secs;
            int cents_second;
            istringstream iss(line);
            iss >> score >> name >> minutes >> secs >> cents_second;
            // Store a new record with all its information
            globalScores.emplace_back(score, name, minutes, secs, cents_second);
        }
        fin.close();
    }
    // Return the records
    return globalScores;
}



/**
 * Given all records in order from highest to lowest score and a score, return the
 * position of the new record (0 to 6) or -1 if not a new record
 * @param globalScores is a vector which stores all the records of the game mode
 * @param score is the score which is going to be test to check if it's a new record or not
 * @return
 */
int isNewRecord(const vector<Score> &globalScores, unsigned long score) {
    // Index of the current record to be check
    int i = 0;
    // Iterate all the records until its position in the ranking is found
    for (; i < (int)globalScores.size() && i < 7; i++) {
        // If the position of the new record has been found
        if (globalScores[i].score < score)
            // Get the position
            return i;
    }
    // If the score is located in the last position of the ranking
    if (globalScores.size() < 7)
        // Get the position
        return i;

    // The new score is not between the best scores of the game mode
    return -1;
}



/**
 * Given all the records of punctuation ordered from highest to lowest punctuation and a new record, add the new record
 * of score and returns true if it has been stored successfully and false if not.
 * @param c is the module configuration of the game
 * @param globalScores is a vector which stores all the records of the game mode
 * @param newRecord is the new record of the game mode which is going to be stored
 * @return
 */
bool saveNewRecord(Config& c, const vector<Score> &globalScores, const Score &newRecord) {

    // Control if the records has been saved correctly
    bool saved = false;

    string path;

    // Determine the path of the file to store the record depending on the difficulty
    switch(c.level){
        case PEACEFUL:
            path = "Resources/Score/scoresPeaceful.info";
            break;
        case EASY:
            path = "Resources/Score/scoresEasy.info";
            break;
        case NORMAL:
            path = "Resources/Score/scoresNormal.info";
            break;
        case HARD:
            path = "Resources/Score/scoresHard.info";
    }

    // Open the file to write the records
    ofstream fout(path, ofstream::trunc);

    if (fout.is_open()) {
        bool end = false;
        int i = 0;
        // Iterate the vector writing the records until the record to store
        for (int j = 0; !end && j < 7; j++) {
            if (!saved && ((i < (int)globalScores.size() && globalScores[i].score < newRecord.score) ||
                           (i >= (int)globalScores.size()))) {
                // Replace the new record is the right position in the ranking
                saved = true;
                fout << newRecord.score << " " << newRecord.name << " " << newRecord.minutes << " " << newRecord.secs
                     << " " << newRecord.cents_second << endl;
            } else {
                // Write a record of the vector
                fout << globalScores[i].score << " " << globalScores[i].name << " " << globalScores[i].minutes << " "
                     << globalScores[i].secs << " " << globalScores[i].cents_second << endl;
                i++;
            }

            // Stores correctly
            if (i >= (int)globalScores.size() && saved)
                end = true;
        }
        fout.close();
    }
    return saved;
}
