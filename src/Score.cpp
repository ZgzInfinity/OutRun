/******************************************************************************
 * @file    Score.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include <fstream>
#include <sstream>
#include "../include/Score.hpp"


using namespace std;

Score::Score(unsigned long score, const string &name, int minutes, int secs, int centsSecond) : score(score),
                                                                                                name(name),
                                                                                                minutes(minutes),
                                                                                                secs(secs),
                                                                                                cents_second(
                                                                                                        centsSecond) {}

vector<Score> getGlobalScores() {
    vector<Score> globalScores;

    ifstream fin("Resources/Score/scores.info");
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
    for (; i < globalScores.size() && i < 7; i++) {
        if (globalScores[i].score < score)
            return i;
    }

    if (globalScores.size() < 7)
        return i;

    return -1;
}

bool saveNewRecord(const vector<Score> &globalScores, const Score &newRecord) {
    bool saved = false;

    ofstream fout("Resources/Score/scores.info", ofstream::trunc);
    if (fout.is_open()) {
        bool end = false;
        int i = 0;
        for (int j = 0; !end && j < 7; j++) {
            if (!saved && ((i < globalScores.size() && globalScores[i].score < newRecord.score) ||
                           (i >= globalScores.size()))) {
                saved = true;
                fout << newRecord.score << " " << newRecord.name << " " << newRecord.minutes << " " << newRecord.secs
                     << " " << newRecord.cents_second << endl;
            } else {
                fout << globalScores[i].score << " " << globalScores[i].name << " " << globalScores[i].minutes << " "
                     << globalScores[i].secs << " " << globalScores[i].cents_second << endl;
                i++;
            }

            if (i >= globalScores.size() && saved)
                end = true;
        }
        fout.close();
    }

    return saved;
}
