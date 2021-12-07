
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


#include "Input.h"

Input::Input(){

    // Resolutions of the game
    currentSoundtrack = 1;

    resolutions[(int)Resolution::SCREEN_0] = SCREEN_0;
    resolutions[(int)Resolution::SCREEN_1] = SCREEN_1;
    resolutions[(int)Resolution::SCREEN_2] = SCREEN_2;
    resolutions[(int)Resolution::SCREEN_3] = SCREEN_3;

    // Check if the Input file exists
    exitsSettings = existSettingsFile("Resources/Settings/Settings.info");

    // Check if exits a default Input for the game
    if (!exitsSettings){

        // Write a default cofiguration file
        writeDefaultInput();

        // Selected default resolution
        currentIndexResolution = 0;

        // Driving actions
        set(Key::ACCELERATE, sf::Keyboard::LControl);
        set(Key::BRAKE, sf::Keyboard::RAlt);
        set(Key::TURN_LEFT, sf::Keyboard::Left);
        set(Key::TURN_RIGHT, sf::Keyboard::Right);
        set(Key::UP_GEAR, sf::Keyboard::Z);
        set(Key::DOWN_GEAR, sf::Keyboard::RControl);

        // Store the difficult of the game
        difficulty = Level_Difficulty::NORMAL;
        traffic = Level_Traffic::MEDIUM;

        // By default any aspect of the Input has been modified
        modifiedinputig = false;

        // Default level of AI aggressiveness
        maxAggressiveness = 0.0f;
    }
    else {

        loadGameInput();
    }

    set(Key::MENU_UP, sf::Keyboard::Up);
    set(Key::MENU_DOWN, sf::Keyboard::Down);
    set(Key::MENU_LEFT, sf::Keyboard::Left);
    set(Key::MENU_RIGHT, sf::Keyboard::Right);
    set(Key::MENU_ACCEPT, sf::Keyboard::Enter);
    set(Key::MENU_CANCEL, sf::Keyboard::Escape);
    set(Key::MENU_PAUSE, sf::Keyboard::Escape);
    set(Key::MENU_CREDITS, sf::Keyboard::Space);

    if (currentIndexResolution <= 1)
        camD = 0.84;
    else
        camD = 0.83;

    // input.screenScale = float(w.getSize().x) / float(SCREEN_0.first);
    setGameWindow(static_cast<unsigned int>(resolutions[currentIndexResolution].first),
                  static_cast<unsigned int>(resolutions[currentIndexResolution].second));
}



/**
 * Returns true if a the Input file of the game exits or not. Otherwise returns false
 * @param path is the path of the Input file of the game
 */
bool Input::existSettingsFile(const char path[]){
    // Check if the Input file exits
    int fd = open(path, O_RDONLY);
    if (fd > 0){
        // The file exits and the flux is closed
        close(fd);
        return true;
    }
    else {
        // Creates the file with writing and reading permissions
        creat(path, 0777);
        return false;
    }
}




inline void Input::loadGameInput(){

     // Load the Input of the game
    ifstream fin("Resources/Settings/Settings.info");


    string s;
    if (fin.is_open()) {

        while (!fin.eof()){
            fin >> s;
            if (s == "DIFFICULTY:" && !fin.eof()) {
                fin >> s;
                if (s == "EASY"){
                    difficulty = Level_Difficulty::EASY;
                }
                else if (s == "NORMAL"){
                    difficulty = Level_Difficulty::NORMAL;
                }
                else if (s == "HARD"){
                    difficulty = Level_Difficulty::HARD;
                }
            }
            if (s == "TRAFFIC:" && !fin.eof()) {
                fin >> s;
                if (s == "NONE"){
                    traffic = Level_Traffic::NONE;
                }
                else if (s == "LOW"){
                    traffic = Level_Traffic::LOW;
                }
                else if (s == "MEDIUM"){
                    traffic = Level_Traffic::MEDIUM;
                }
                else if (s == "HIGH"){
                    traffic = Level_Traffic::HIGH;
                }
            }
            else if (s == "VOLUME_SOUNDTRACKS:" && !fin.eof()) {
                fin >> s;
                volumeMusic = stoi(s);
            }
            else if (s == "VOLUME_EFFECTS:" && !fin.eof()) {
                fin >> s;
                volumeEffects = stoi(s);
            }
            else if (s == "RESOLUTION:" && !fin.eof()) {
                fin >> s;
                currentIndexResolution = stoi(s);
            }
            else if (s == "CONTROL_LEFT:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                set(Key::TURN_LEFT, getKeyCode(s));
            }
            else if (s == "CONTROL_RIGHT:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                Input::set(Key::TURN_RIGHT, Input::getKeyCode(s));
            }
            // Keyword to accelerate the player's vehicle
            else if (s == "CONTROL_ACCELERATE:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                Input::set(Key::ACCELERATE, Input::getKeyCode(s));
            }
            // Keyword to brake the player's vehicle
            else if (s == "CONTROL_BRAKE:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                Input::set(Key::BRAKE, Input::getKeyCode(s));
            }
            // Keyword to change to a higher gear
            else if (s == "CONTROL_UP_GEAR:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                Input::set(Key::UP_GEAR, Input::getKeyCode(s));
            }
            // Keyword to change to a lower gear
            else if (s == "CONTROL_DOWN_GEAR:" && !fin.eof()) {
                fin.ignore(1);
                getline(fin, s);
                Input::set(Key::DOWN_GEAR, Input::getKeyCode(s));
            }
        }
        fin.close();
    }
}



/**
 * Write a default Input for the game
 * @param path is the path with the default Input of the game
 */
void Input::writeDefaultInput(){

    //  New file which stores the new Input of the game
    ofstream f("Resources/Settings/Settings.info");

    // Check if the file has been opened correctly
    if (f.is_open()){
        // Write all the Input of the game
        f << "DIFFICULTY: NORMAL" << endl;
        f << "TRAFFIC: MEDIUM" << endl;
        f << "VOLUME_SOUNDTRACKS: 100" << endl;
        f << "VOLUME_EFFECTS: 100" << endl;
        f << "FULL_SCREEN: DISABLED" << endl;
        f << "RESOLUTION: 0" << endl;
        f << "CONTROL LEFT: LEFT CURSOR" << endl;
        f << "CONTROL_RIGHT: RIGHT CURSOR" << endl;
        f << "CONTROL_ACCELERATE: LEFT CONTROL" << endl;
        f << "CONTROL_BRAKE: LEFT ALT" << endl;
        f << "CONTROL_UP_GEAR: Z" << endl;
        f << "CONTROL_LOW_GEAR: RIGHT CTRL" << endl;
    }
    // Close the flux of the Input file
    f.close();
}



std::string Input::getActionName(const Key action) {

    std::string ret;

    switch (action) {
        // Driving actions
        case Key::ACCELERATE:
            ret = "ACCELERATE";
            break;
        case Key::BRAKE:
            ret = "BRAKE";
            break;
        case Key::TURN_LEFT:
            ret = "TURN LEFT";
            break;
        case Key::TURN_RIGHT:
            ret = "TURN RIGHT";
            break;
        case Key::UP_GEAR:
            ret = "UP GEAR";
            break;
        case Key::DOWN_GEAR:
            ret = "DOWN GEAR";
            break;
        default:
            ret = "?";
            break;
    }
    return ret;
}



std::string Input::getKeyCodeName(const sf::Keyboard::Key keycode) {
    std::string ret;
    switch (keycode) {
        case sf::Keyboard::A:
            ret = "A";
            break;
        case sf::Keyboard::B:
            ret = "B";
            break;
        case sf::Keyboard::C:
            ret = "C";
            break;
        case sf::Keyboard::D:
            ret = "D";
            break;
        case sf::Keyboard::E:
            ret = "E";
            break;
        case sf::Keyboard::F:
            ret = "F";
            break;
        case sf::Keyboard::G:
            ret = "G";
            break;
        case sf::Keyboard::H:
            ret = "H";
            break;
        case sf::Keyboard::I:
            ret = "I";
            break;
        case sf::Keyboard::J:
            ret = "J";
            break;
        case sf::Keyboard::K:
            ret = "K";
            break;
        case sf::Keyboard::L:
            ret = "L";
            break;
        case sf::Keyboard::M:
            ret = "M";
            break;
        case sf::Keyboard::N:
            ret = "N";
            break;
        case sf::Keyboard::O:
            ret = "O";
            break;
        case sf::Keyboard::P:
            ret = "P";
            break;
        case sf::Keyboard::Q:
            ret = "Q";
            break;
        case sf::Keyboard::R:
            ret = "R";
            break;
        case sf::Keyboard::S:
            ret = "S";
            break;
        case sf::Keyboard::T:
            ret = "T";
            break;
        case sf::Keyboard::U:
            ret = "U";
            break;
        case sf::Keyboard::V:
            ret = "V";
            break;
        case sf::Keyboard::W:
            ret = "W";
            break;
        case sf::Keyboard::X:
            ret = "X";
            break;
        case sf::Keyboard::Y:
            ret = "Y";
            break;
        case sf::Keyboard::Z:
            ret = "Z";
            break;
        case sf::Keyboard::Num0:
            ret = "0";
            break;
        case sf::Keyboard::Num1:
            ret = "1";
            break;
        case sf::Keyboard::Num2:
            ret = "2";
            break;
        case sf::Keyboard::Num3:
            ret = "3";
            break;
        case sf::Keyboard::Num4:
            ret = "4";
            break;
        case sf::Keyboard::Num5:
            ret = "5";
            break;
        case sf::Keyboard::Num6:
            ret = "6";
            break;
        case sf::Keyboard::Num7:
            ret = "7";
            break;
        case sf::Keyboard::Num8:
            ret = "8";
            break;
        case sf::Keyboard::Num9:
            ret = "9";
            break;
        case sf::Keyboard::Escape:
            ret = "ESCAPE";
            break;
        case sf::Keyboard::LControl:
            ret = "LEFT CONTROL";
            break;
        case sf::Keyboard::LShift:
            ret = "LEFT SHIFT";
            break;
        case sf::Keyboard::LAlt:
            ret = "LEFT ALT";
            break;
        case sf::Keyboard::LSystem:
            ret = "LEFT SYSTEM";
            break;
        case sf::Keyboard::RControl:
            ret = "RIGHT CONTROL";
            break;
        case sf::Keyboard::RShift:
            ret = "RIGHT SHIFT";
            break;
        case sf::Keyboard::RAlt:
            ret = "RIGHT ALT";
            break;
        case sf::Keyboard::RSystem:
            ret = "RIGHT SYSTEM";
            break;
        case sf::Keyboard::Dash:
            ret = "DASH";
            break;
        case sf::Keyboard::Space:
            ret = "SPACE";
            break;
        case sf::Keyboard::Enter:
            ret = "ENTER";
            break;
        case sf::Keyboard::BackSpace:
            ret = "DELETE";
            break;
        case sf::Keyboard::Tab:
            ret = "TAB";
            break;
        case sf::Keyboard::PageUp:
            ret = "PG UP";
            break;
        case sf::Keyboard::PageDown:
            ret = "PG DOWN";
            break;
        case sf::Keyboard::End:
            ret = "END";
            break;
        case sf::Keyboard::Home:
            ret = "HOME";
            break;
        case sf::Keyboard::Insert:
            ret = "INSERT";
            break;
        case sf::Keyboard::Delete:
            ret = "DELETE";
            break;
        case sf::Keyboard::Left:
            ret = "LEFT CURSOR";
            break;
        case sf::Keyboard::Right:
            ret = "RIGHT CURSOR";
            break;
        case sf::Keyboard::Up:
            ret = "UP CURSOR";
            break;
        case sf::Keyboard::Down:
            ret = "DOWN CURSOR";
            break;
        case sf::Keyboard::Numpad0:
            ret = "NUMPAD 0";
            break;
        case sf::Keyboard::Numpad1:
            ret = "NUMPAD 1";
            break;
        case sf::Keyboard::Numpad2:
            ret = "NUMPAD 2";
            break;
        case sf::Keyboard::Numpad3:
            ret = "NUMPAD 3";
            break;
        case sf::Keyboard::Numpad4:
            ret = "NUMPAD 4";
            break;
        case sf::Keyboard::Numpad5:
            ret = "NUMPAD 5";
            break;
        case sf::Keyboard::Numpad6:
            ret = "NUMPAD 6";
            break;
        case sf::Keyboard::Numpad7:
            ret = "NUMPAD 7";
            break;
        case sf::Keyboard::Numpad8:
            ret = "NUMPAD 8";
            break;
        case sf::Keyboard::Numpad9:
            ret = "NUMPAD 9";
            break;
        case sf::Keyboard::F1:
            ret = "F1";
            break;
        case sf::Keyboard::F2:
            ret = "F2";
            break;
        case sf::Keyboard::F3:
            ret = "F3";
            break;
        case sf::Keyboard::F4:
            ret = "F4";
            break;
        case sf::Keyboard::F5:
            ret = "F5";
            break;
        case sf::Keyboard::F6:
            ret = "F6";
            break;
        case sf::Keyboard::F7:
            ret = "F7";
            break;
        case sf::Keyboard::F8:
            ret = "F8";
            break;
        case sf::Keyboard::F9:
            ret = "F9";
            break;
        case sf::Keyboard::F10:
            ret = "F10";
            break;
        case sf::Keyboard::F11:
            ret = "F11";
            break;
        case sf::Keyboard::F12:
            ret = "F12";
            break;
        default:
            ret = "?";
            break;
    }
    return ret;
}



// code based on:
// https://en.sfml-dev.org/forums/index.php?topic=15226.0
sf::Keyboard::Key Input::getKeyCode(const std::string code) {

    if (code.compare("A") == 0){
        return sf::Keyboard::A;
    }
    else if (code.compare("B") == 0){
        return sf::Keyboard::B;
    }
    else if (code.compare("C") == 0){
        return sf::Keyboard::C;
    }
    else if (code.compare("D") == 0){
        return sf::Keyboard::D;
    }
    else if (code.compare("E") == 0){
        return sf::Keyboard::E;
    }
    else if (code.compare("F") == 0){
        return sf::Keyboard::F;
    }
    else if (code.compare("G") == 0){
        return sf::Keyboard::G;
    }
    else if (code.compare("H") == 0){
        return sf::Keyboard::B;
    }
    else if (code.compare("I") == 0){
        return sf::Keyboard::B;
    }
    else if (code.compare("J") == 0){
        return sf::Keyboard::J;
    }
    else if (code.compare("K") == 0){
        return sf::Keyboard::K;
    }
    else if (code.compare("L") == 0){
        return sf::Keyboard::L;
    }
    else if (code.compare("M") == 0){
        return sf::Keyboard::M;
    }
    else if (code.compare("N") == 0){
        return sf::Keyboard::N;
    }
    else if (code.compare("0") == 0){
        return sf::Keyboard::O;
    }
    else if (code.compare("P") == 0){
        return sf::Keyboard::P;
    }
    else if (code.compare("Q") == 0){
        return sf::Keyboard::Q;
    }
    else if (code.compare("R") == 0){
        return sf::Keyboard::R;
    }
    else if (code.compare("S") == 0){
        return sf::Keyboard::S;
    }
    else if (code.compare("T") == 0){
        return sf::Keyboard::T;
    }
    else if (code.compare("U") == 0){
        return sf::Keyboard::U;
    }
    else if (code.compare("V") == 0){
        return sf::Keyboard::V;
    }
    else if (code.compare("W") == 0){
        return sf::Keyboard::W;
    }
    else if (code.compare("X") == 0){
        return sf::Keyboard::X;
    }
    else if (code.compare("Y") == 0){
        return sf::Keyboard::Y;
    }
    else if (code.compare("Z") == 0){
        return sf::Keyboard::Z;
    }
    else if (code.compare("0") == 0){
        return sf::Keyboard::Num0;
    }
    else if (code.compare("1") == 0){
        return sf::Keyboard::Num1;
    }
    else if (code.compare("2") == 0){
        return sf::Keyboard::Num2;
    }
    else if (code.compare("3") == 0){
        return sf::Keyboard::Num3;
    }
    else if (code.compare("4") == 0){
        return sf::Keyboard::Num4;
    }
    else if (code.compare("5") == 0){
        return sf::Keyboard::Num5;
    }
    else if (code.compare("6") == 0){
        return sf::Keyboard::Num6;
    }
    else if (code.compare("7") == 0){
        return sf::Keyboard::Num7;
    }
    else if (code.compare("8") == 0){
        return sf::Keyboard::Num8;
    }
    else if (code.compare("9") == 0){
        return sf::Keyboard::Num9;
    }
    else if (code.compare("ESCAPE") == 0){
        return sf::Keyboard::Escape;
    }
    else if (code.compare("LEFT CONTROL") == 0){
        return sf::Keyboard::LControl;
    }
    else if (code.compare("LEFT SHIFT") == 0){
        return sf::Keyboard::LShift;
    }
    else if (code.compare("LEFT ALT") == 0){
        return sf::Keyboard::LAlt;
    }
    else if (code.compare("LEFT SYSTEM") == 0){
        return sf::Keyboard::RSystem;
    }
    else if (code.compare("RIGHT CONTROL") == 0){
        return sf::Keyboard::RControl;
    }
    else if (code.compare("RIGHT SHIFT") == 0){
        return sf::Keyboard::RShift;
    }
    else if (code.compare("RIGHT ALT") == 0){
        return sf::Keyboard::RAlt;
    }
    else if (code.compare("RIGHT SYSTEM") == 0){
        return sf::Keyboard::RSystem;
    }
    else if (code.compare("DASH") == 0){
        return sf::Keyboard::Dash;
    }
    else if (code.compare("SPACE") == 0){
        return sf::Keyboard::Space;
    }
    else if (code.compare("ENTER") == 0){
        return sf::Keyboard::Enter;
    }
    else if (code.compare("BACKSPACE") == 0){
        return sf::Keyboard::BackSpace;
    }
    else if (code.compare("TAB") == 0){
        return sf::Keyboard::Tab;
    }
    else if (code.compare("PAGE UP") == 0){
        return sf::Keyboard::PageUp;
    }
    else if (code.compare("PAGE DOWN") == 0){
        return sf::Keyboard::PageDown;
    }
    else if (code.compare("END") == 0){
        return sf::Keyboard::End;
    }
    else if (code.compare("HOME") == 0){
        return sf::Keyboard::Home;
    }
    else if (code.compare("INSERT") == 0){
        return sf::Keyboard::Insert;
    }
    else if (code.compare("DELETE") == 0){
        return sf::Keyboard::Delete;
    }
    else if (code.compare("LEFT CURSOR") == 0){
        return sf::Keyboard::Left;
    }
    else if (code.compare("RIGHT CURSOR") == 0){
        return sf::Keyboard::Right;
    }
    else if (code.compare("UP CURSOR") == 0){
        return sf::Keyboard::Up;
    }
    else if (code.compare("DOWN CURSOR") == 0){
        return sf::Keyboard::Down;
    }
    else if (code.compare("NUMPAD 0") == 0){
        return sf::Keyboard::Numpad0;
    }
    else if (code.compare("NUMPAD 1") == 0){
        return sf::Keyboard::Numpad1;
    }
    else if (code.compare("NUMPAD 2") == 0){
        return sf::Keyboard::Numpad2;
    }
    else if (code.compare("NUMPAD 3") == 0){
        return sf::Keyboard::Numpad3;
    }
    else if (code.compare("NUMPAD 4") == 0){
        return sf::Keyboard::Numpad4;
    }
    else if (code.compare("NUMPAD 5") == 0){
        return sf::Keyboard::Numpad5;
    }
    else if (code.compare("NUMPAD 6") == 0){
        return sf::Keyboard::Numpad6;
    }
    else if (code.compare("NUMPAD 7") == 0){
        return sf::Keyboard::Numpad7;
    }
    else if (code.compare("NUMPAD 8") == 0){
        return sf::Keyboard::Numpad8;
    }
    else if (code.compare("NUMPAD 9") == 0){
        return sf::Keyboard::Numpad9;
    }
    else if (code.compare("F1") == 0){
        return sf::Keyboard::F1;
    }
    else if (code.compare("F2") == 0){
        return sf::Keyboard::F2;
    }
    else if (code.compare("F3") == 0){
        return sf::Keyboard::F3;
    }
    else if (code.compare("F4") == 0){
        return sf::Keyboard::F4;
    }
    else if (code.compare("F5") == 0){
        return sf::Keyboard::F5;
    }
    else if (code.compare("F6") == 0){
        return sf::Keyboard::F6;
    }
    else if (code.compare("F7") == 0){
        return sf::Keyboard::F7;
    }
    else if (code.compare("F8") == 0){
        return sf::Keyboard::F8;
    }
    else if (code.compare("F9") == 0){
        return sf::Keyboard::F9;
    }
    else if (code.compare("F10") == 0){
        return sf::Keyboard::F10;
    }
    else if (code.compare("F11") == 0){
        return sf::Keyboard::F11;
    }
    else if (code.compare("F12") == 0){
        return sf::Keyboard::F12;
    }
    else {
        return sf::Keyboard::Unknown;
    }
}



void Input::writeNewInput (){

    //  New file which stores the new Input of the game
    ofstream f("Resources/Settings/Settings.info");

    // Check if the file has been opened correctly
    if (f.is_open()){
        f << "DIFFICULTY: ";
        switch (difficulty){
            case Level_Difficulty::EASY:
                f << "EASY" << endl;
                break;
            case Level_Difficulty::NORMAL:
                f << "NORMAL" << endl;
                break;
            case Level_Difficulty::HARD:
                f << "HARD" << endl;
        }

        f << "TRAFFIC: ";
        switch (traffic){
            case Level_Traffic::NONE:
                f << "NONE" << endl;
                break;
            case Level_Traffic::LOW:
                f << "LOW" << endl;
                break;
            case Level_Traffic::MEDIUM:
                f << "MEDIUM" << endl;
                break;
            case Level_Traffic::HIGH:
                f << "HIGH" << endl;
        }

        int musicVol = (Audio::getMusicVolume() + 0.005f) * 100;
        int sfxVol = (Audio::getSfxVolume() + 0.005f) * 100;

        f << "VOLUME_SOUNDTRACKS: " << musicVol << endl;
        f << "VOLUME_EFFECTS: " << sfxVol << endl;
        f << "RESOLUTION: " << currentIndexResolution << endl;
        f << "CONTROL_LEFT: " << getKeyCodeName(get(Key::TURN_LEFT)) << endl;
        f << "CONTROL_RIGHT: " << getKeyCodeName(get(Key::TURN_RIGHT)) << endl;
        f << "CONTROL_ACCELERATE: " << getKeyCodeName(get(Key::ACCELERATE)) << endl;
        f << "CONTROL_BRAKE: " << getKeyCodeName(get(Key::BRAKE)) << endl;
        f << "CONTROL_UP_GEAR: " << getKeyCodeName(get(Key::UP_GEAR)) << endl;
        f << "CONTROL_DOWN_GEAR: " << getKeyCodeName(get(Key::DOWN_GEAR)) << endl;
    }
    f.close();
}
