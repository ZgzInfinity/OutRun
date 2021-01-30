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

#include <windows.h>
#include <SFML/Graphics.hpp>
#include "../include/Menu.hpp"
#include "../include/Game.hpp"

using namespace sf;
using namespace std;


mutex mainMutex;



/**
 * Load the current configuration of the game
 * @param path is the xml file with the configuration
 * @param difficulty is the difficulty of the game
 * @param volumeSoundtracks is the volume of the music
 * @param volumeEffects is the volume of the SFX
 * @param pixelArt controls if the graphics have to be drawn with pixel art
 * @param fullScreen controls if the screen has to be drawn in completed screen
 * @param axis_x is the width of the screen
 * @param axis_y is the height of the screen
 * @param controlLeft controls the key to move the vehicle of the player to the left
 * @param controlRight controls the key to move the vehicle of the player to the right
 * @param controlAccelerate controls the key to accelerate the vehicle of the player
 * @param controlBrake controls the key to brake the vehicle of the player
 * @param controlUpGear controls the key to change to a higher gear
 * @param controlLowGear controls the key to change to a lower gear
 */
void loadGameConfiguration (const string path, Difficult& difficulty, bool& enabledAi,
                            int& volumeSoundtracks, int& volumeEffects, bool& pixelArt,
                            bool& fullScreen, int& axis_x,int& axis_y, string& controlLeft,
                            string& controlRight, string& controlAccelerate, string& controlBrake,
                            string& controlUpGear, string& controlLowGear)
{
     // Load the configuration of the game
    ifstream fin(path );

    string s;
    if (fin.is_open()) {

        while (!fin.eof()){
            fin >> s;
            if (s == "DIFFICULTY:" && !fin.eof()) {
                fin >> s;
                // Get the difficulty value
                if (s == "Easy"){
                    // Level easy
                    difficulty = EASY;
                }
                else if (s == "Normal"){
                    // Level normal
                    difficulty = NORMAL;
                }
                else if (s == "Hard"){
                    // Level hard
                    difficulty = HARD;
                }
                else if (s == "Peaceful"){
                    // Level hard
                    difficulty = PEACEFUL;
                }
            }
            // Read the AI controller
            else if (s == "AI:" && !fin.eof()) {
                fin >> s;
                if (s == "Enabled"){
                    enabledAi = true;
                }
                else if (s == "Disabled"){
                    enabledAi = false;
                }
            }
            // Volume of the soundtracks
            else if (s == "VOLUME_SOUNDTRACKS:" && !fin.eof()) {
                fin >> s;
                volumeSoundtracks = stoi(s);
            }
            // Volume of the sound effects
            else if (s == "VOLUME_EFFECTS:" && !fin.eof()) {
                fin >> s;
                volumeEffects = stoi(s);
            }
            // control if the graphics have to been drawn with pixel art
            else if (s == "PIXEL_ART:" && !fin.eof()) {
                fin >> s;
                if (s == "Enabled"){
                    pixelArt = true;
                }
                else if (s == "Disabled"){
                    pixelArt = false;
                }
            }
            // Full screen
            else if (s == "FULL_SCREEN:" && !fin.eof()) {
                fin >> s;
                if (s == "Enabled"){
                    fullScreen = true;
                }
                else if (s == "Disabled"){
                    fullScreen = false;
                }
            }
            // Width of the screen
            else if (s == "RESOLUTION_X:" && !fin.eof()) {
                fin >> s;
                axis_x = stoi(s);
            }
            // height of the screen
            else if (s == "RESOLUTION_Y:" && !fin.eof()) {
                fin >> s;
                axis_y = stoi(s);
            }
            // Keyword to turn left the player's vehicle
            else if (s == "CONTROLLER_LEFT:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                controlLeft = s;
            }
            // Keyword to turn right the player's vehicle
            else if (s == "CONTROLLER_RIGHT:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                controlRight = s;
            }
            // Keyword to accelerate the player's vehicle
            else if (s == "CONTROLLER_ACCELERATE:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                controlAccelerate = s;
            }
            // Keyword to brake the player's vehicle
            else if (s == "CONTROLLER_BRAKE:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                controlBrake = s;
            }
            // Keyword to change to a higher gear
            else if (s == "CONTROLLER_UP_GEAR:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                controlUpGear = s;
            }
            // Keyword to change to a lower gear
            else if (s == "CONTROLLER_LOW_GEAR:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                controlLowGear = s;
            }
        }
        fin.close();
    }
}


/**
 * Main program that controls Out Run
 */
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show){

    // Throw the application with hight priority
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

    // Variables to control the configuration of the game
    Difficult difficulty;
    bool pixelArt, enabledAi, fullScreen;
    int volumeSoundtracks, volumeEffects, axis_x, axis_y;
    string controlLeft, controlRight, controlAccelerate, controlBrake, controlUpGear, controlLowGear;

    // Load the properties of the game
    string path = "Resources/Settings/Settings.info";

    loadGameConfiguration(path, difficulty, enabledAi, volumeSoundtracks, volumeEffects, pixelArt, fullScreen, axis_x, axis_y,
                          controlLeft, controlRight, controlAccelerate, controlBrake, controlUpGear, controlLowGear);

    // Creation of the configuration module of the game
    Config c(difficulty, pixelArt, enabledAi, fullScreen, axis_x, axis_y, controlLeft, controlRight, controlAccelerate,
             controlBrake, controlUpGear, controlLowGear, volumeEffects, volumeSoundtracks);

    State state = ANIMATION;

    // Main loop of the game AFD
    while (c.window.isOpen() && state != EXIT) {

        Game engine(c);

        if (state == START)
            state = startMenu(c);

        while (c.window.isOpen() && state != START) {
            switch (state) {
                case ANIMATION: {
                    state = introAnimation(c);
                    break;
                }
                case GEARS: {
                    bool autoGear = true;
                    state = gearsMenu(c, autoGear);
                    engine.setAutoMode(autoGear);
                    break;
                }
                case MUSIC: {
                    state = selectMusicSoundtrack(c);
                    break;
                }
                case LOADING: {
                    bool autoMod = engine.getAutoMode();
                    state = showLoadingAnimation(c, autoMod);
                    break;
                }
                case OPTIONS: {
                    bool inGame = engine.isInGame();
                    state = optionsMenu(c, inGame);
                    engine.checkDifficulty(c);
                    break;
                }
                case GAME: {
                    state = engine.play(c);
                    break;
                }
                case RANKING: {
                    unsigned long scorePlayer = engine.getScore();
                    int minutes = int(engine.getMinutesTrip());
                    int secs = int(engine.getSecsTrip());
                    int cents_Second = int(engine.getCents_SecondTrip());
                    state = rankingMenu(c, scorePlayer, minutes, secs, cents_Second);
                    break;
                }
                default: {
                    c.window.close();
                    break;
                }
            }
        }
    }
    return 0;
}
