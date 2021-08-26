
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


#include "Logger.h"


Logger Logger::instance;

Logger::Logger(){
    instance.failDetected = false;
    instance.outputFlux.open(instance.LOGGER_PATH_FILE);
    if (!instance.outputFlux.is_open())
        instance.failDetected = true;
}

void Logger::setFailDetected(const bool _failDetected){
    instance.failDetected = _failDetected;
}

bool Logger::getFailDetected(){
    return instance.failDetected;
}

bool Logger::checkMapFile(const std::string& pathMapFile){
    instance.inputFlux.open(pathMapFile);
    if (instance.inputFlux.is_open()){
        instance.outputFlux << "FILE MAP: " << pathMapFile << " FOUND." << std::endl;
        instance.column = 1;
        instance.row = 1;
    }
    else {
        instance.outputFlux << "FILE MAP: " << pathMapFile << " DOES NOT EXIST." << std::endl;
        instance.failDetected = true;
    }
    return instance.failDetected;
}


bool Logger::checkTimeAndTerrain(Map& m){
    std::string informationRead;
    instance.inputFlux >> informationRead;

    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN GLOBAL_CONF NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        if (informationRead != "GLOBAL_CONF"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN GLOBAL_CONF BUT FOUND " << informationRead << "." << std::endl;

            return !instance.failDetected;
        }
        else {

            instance.row++;
            instance.column = 1;

            instance.inputFlux >> informationRead;

            if (instance.inputFlux.eof()){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". IDENTIFIER TOKEN TIME: NOT FOUND" << std::endl;

                return !instance.failDetected;
            }
            else {
                if (informationRead != "TIME:"){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". EXPECTED IDENTIFIER TOKEN TIME: BUT FOUND " << informationRead << "." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    instance.column += 2;
                    instance.inputFlux >> informationRead;
                    if (instance.inputFlux.eof()){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". TIME VALUE NOT FOUND." << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        if (std::regex_match(informationRead, instance.natural_number_regex)){
                            m.setTime(std::stoi(informationRead));
                        }
                        else {
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". TIME VALUE " << informationRead << " IS NOT A POSITIVE INTEGER." << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }

                instance.column = 1;
                instance.row++;

                instance.inputFlux >> informationRead;
                if (instance.inputFlux.eof()){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". IDENTIFIER TOKEN TERRAIN: NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    if (informationRead != "TERRAIN:"){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". EXPECTED IDENTIFIER TOKEN TERRAIN: BUT FOUND " << informationRead << "." << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        instance.column += 2;
                        instance.inputFlux >> informationRead;
                        if (instance.inputFlux.eof()){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". TERRAIN VALUE NOT FOUND." << std::endl;

                            return !instance.failDetected;
                        }
                        else {
                            if (std::regex_match(informationRead, instance.natural_number_regex)){
                                int terrain = std::stoi(informationRead);
                                if (terrain < 0 || terrain > 2){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". TERRAIN VALUE " << informationRead << " IS OUT OF RANGE. MUST BE "
                                                        << "0 (SAND), 1 (GRASS) OR 2 (SNOW)." << std::endl;
                                }
                                else {
                                    m.setTerrain(terrain);
                                }
                            }
                            else {
                                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                    instance.column << ". TERRAIN VALUE " << informationRead << " IS NOT A POSITIVE INTEGER." << std::endl;

                                return !instance.failDetected;
                            }
                        }
                    }
                }
            }
        }
    }
    instance.outputFlux << "GLOBAL_CONF PART CORRECTLY SPECIFIED." << std::endl;
    return instance.failDetected;
}


bool Logger::checkColors(Map& m){
    std::string informationRead, colorToBeRead;
    std::vector<sf::Color> colorsOfMap;
    sf::Color colorRead;

    instance.row++;
    instance.column = 1;

    instance.inputFlux >> informationRead;

    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN COLORS NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        if (informationRead != "COLORS"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN COLORS BUT FOUND " << informationRead << "." << std::endl;

            return !instance.failDetected;
        }
        else {

            instance.row++;
            instance.column = 1;

            for (int i = 1; i <= 9; i++){
                if (i == 1)
                    colorToBeRead = "COLOR_BACKGROUND:";
                else if (i == 2)
                    colorToBeRead = "COLOR_SAND_1:";
                else if (i == 3)
                    colorToBeRead = "COLOR_SAND_2:";
                else if (i == 4)
                    colorToBeRead = "COLOR_ROAD_1:";
                else if (i == 5)
                    colorToBeRead = "COLOR_ROAD_2:";
                else if (i == 6)
                    colorToBeRead = "COLOR_RUMBLE_1:";
                else if (i == 7)
                    colorToBeRead = "COLOR_RUMBLE_2:";
                else if (i == 8)
                    colorToBeRead = "COLOR_LANE_1:";
                else
                    colorToBeRead = "COLOR_LANE_2:";

                instance.inputFlux >> informationRead;
                if (instance.inputFlux.eof()){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". IDENTIFIER TOKEN " << colorToBeRead << " NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    if (informationRead != colorToBeRead){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". EXPECTED IDENTIFIER TOKEN " << colorToBeRead << " BUT FOUND " << informationRead << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        instance.column += 2;
                        instance.failDetected = readColor(colorRead);
                        if (!instance.failDetected){
                            colorsOfMap.push_back(colorRead);
                            if (i != 9){
                                instance.row++;
                                instance.column = 1;
                            }
                        }
                        else
                            return !instance.failDetected;
                    }
                }
            }
            m.setColors(colorsOfMap);
            instance.outputFlux << "COLORS PART CORRECTLY SPECIFIED." << std::endl;
            return instance.failDetected;
        }
    }
}


bool Logger::readColor(sf::Color& colorRead){
    int red = 0, green = 0, blue = 0, alpha = 0, colorChannelsRead = 0;
    string informationRead, channelFailed;

    while (!instance.failDetected && colorChannelsRead < 4){
        instance.inputFlux >> informationRead;
        if (instance.inputFlux.eof()){
            switch (colorChannelsRead){
                case 0:
                    channelFailed = "RED CHANNEL";
                    break;
                case 1:
                    channelFailed = "GREEN CHANNEL";
                    break;
                case 2:
                    channelFailed = "BLUE CHANNEL";
                    break;
                case 3:
                    channelFailed = "ALPHA CHANNEL";
            }
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". " << channelFailed << " NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            if (std::regex_match(informationRead, instance.integer_number_regex)){
                int chanel = std::stoi(informationRead);
                if (chanel >= 0 && chanel <= 255){
                    switch (colorChannelsRead){
                        case 0:
                            red = chanel;
                            break;
                        case 1:
                            green = chanel;
                            break;
                        case 2:
                            blue = chanel;
                            break;
                        case 3:
                            alpha = chanel;
                    }
                    instance.column += 2;
                    colorChannelsRead++;
                    if (colorChannelsRead == 4)
                        colorRead = sf::Color(red, green, blue, alpha);
                }
                else {
                    switch (colorChannelsRead){
                        case 0:
                            channelFailed = "RED CHANNEL VALUE";
                            break;
                        case 1:
                            channelFailed = "GREEN CHANNEL VALUE";
                            break;
                        case 2:
                            channelFailed = "BLUE CHANNEL VALUE";
                            break;
                        case 3:
                            channelFailed = "ALPHA CHANNEL VALUE";
                    }
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". " << channelFailed << " " << chanel
                                        << " IS OUT OF RANGE. MUST BE BETWEEN 0 AND 255." << std::endl;

                    return !instance.failDetected;
                }
            }
            else {
                switch (colorChannelsRead){
                    case 0:
                        channelFailed = "RED CHANNEL VALUE " + informationRead;
                        break;
                    case 1:
                        channelFailed = "GREEN CHANNEL VALUE " + informationRead;
                        break;
                    case 2:
                        channelFailed = "BLUE CHANNEL VALUE " + informationRead;
                        break;
                    case 3:
                        channelFailed = "ALPHA CHANNEL VALUE " + informationRead;
                }
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". " << channelFailed << " IS NOT A POSITIVE INTEGER." << std::endl;

                return !instance.failDetected;
            }
        }
    }
    return instance.failDetected;
}



bool Logger::checkReliefStraight(Map& m){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 5;

    bool mirror;
    int enter, hold, leave, numTracks;

    while (!instance.failDetected && parametersRead < totalParametersToRead){
        instance.inputFlux >> informationRead;
        parametersRead++;

        if (instance.inputFlux.eof()){

            std::string parameterFailed;

            if (parametersRead == 1)
                parameterFailed = "ENTER";
            else if (parametersRead == 2)
                parameterFailed = "HOLD";
            else if (parametersRead == 3)
                parameterFailed = "LEAVE";
            else if (parametersRead == 4)
                parameterFailed = "MIRROR";
            else
                parameterFailed = "NUM TRACKS";

            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". " << parameterFailed << " PARAMETER NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            if (parametersRead == 4){
               if (informationRead != "0" && informationRead != "1"){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". CANNOT CONVERT MIRROR VALUE " << informationRead <<
                        " TO BOOLEAN. THE VALUE MUST BE 0 OR 1." << std::endl;

                    return !instance.failDetected;
                }
                else if (informationRead == "1")
                    mirror = true;
                else
                    mirror = false;
            }
            else if (std::regex_match(informationRead, instance.natural_number_regex)){
                if (parametersRead == 1)
                    enter = std::stoi(informationRead);
                else if (parametersRead == 2)
                    hold = std::stoi(informationRead);
                else if (parametersRead == 3)
                    leave = std::stoi(informationRead);
                else {
                    numTracks = std::stoi(informationRead);
                    if (numTracks < 2 || numTracks > 8){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". NUM TRACKS VALUE " << numTracks <<
                            " IS OUT F RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                        return !instance.failDetected;
                    }
                }
            }
            else {
                if (parametersRead == 1){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". ENTER VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                    return !instance.failDetected;
                }
                else if (parametersRead == 2){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". HOLD VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                    return !instance.failDetected;
                }
                else if (parametersRead == 3){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". LEAVE VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". NUM TRACKS VALUE " << informationRead <<
                        " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                    return !instance.failDetected;
                }
            }
        }
    }
    m.addMap(enter, hold, leave, 0, 0, mirror, numTracks);
    return instance.failDetected;
}


bool Logger::checkReliefCurve(Map& m){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 7;

    bool mirror;
    int enter, hold, leave, numTracks, factor_length;
    float direction;

    while (!instance.failDetected && parametersRead < totalParametersToRead){
        instance.inputFlux >> informationRead;
        parametersRead++;

        if (instance.inputFlux.eof()){

            std::string parameterFailed;

            if (parametersRead == 1)
                parameterFailed = "ENTER";
            else if (parametersRead == 2)
                parameterFailed = "HOLD";
            else if (parametersRead == 3)
                parameterFailed = "LEAVE";
            else if (parametersRead == 4)
                parameterFailed = "DIRECTION CURVE";
            else if (parametersRead == 5)
                parameterFailed = "MIRROR";
            else if (parametersRead == 6)
                parameterFailed = "NUM TRACKS";
            else
                parameterFailed = "FACTOR LENGTH";

            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". " << parameterFailed << " PARAMETER NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            if (parametersRead == 4){
                if (std::regex_match(informationRead, instance.float_number_regex))
                    direction = std::stof(informationRead);
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". DIRECTION CURVE PARAMETER " << informationRead << " MUST BE FLOAT." << std::endl;

                    return !instance.failDetected;
                }
            }
            else if (parametersRead == 5){
                if (informationRead != "0" && informationRead != "1"){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". CANNOT CONVERT MIRROR VALUE " << informationRead <<
                        " TO BOOLEAN. THE VALUE MUST BE 0 OR 1." << std::endl;

                    return !instance.failDetected;
                }
                else if (informationRead == "1")
                    mirror = true;
                else
                    mirror = false;
            }
            else {
                if (std::regex_match(informationRead, instance.natural_number_regex)){
                    if (parametersRead == 1)
                        enter = std::stoi(informationRead);
                    else if (parametersRead == 2)
                        hold = std::stoi(informationRead);
                    else if (parametersRead == 3)
                        leave = std::stoi(informationRead);
                    else if (parametersRead == 6){
                        numTracks = std::stoi(informationRead);
                        if (numTracks < 2 || numTracks > 8){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". NUM TRACKS VALUE " << numTracks <<
                                " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                            return !instance.failDetected;
                        }
                    }
                    else if (parametersRead == 7){
                        factor_length = std::stoi(informationRead);
                        if (factor_length <= 0){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". FACTOR LENGTH CANNOT BE NEGATIVE OR ZERO. " << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }
                else {
                    if (parametersRead == 1){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". ENTER VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 2){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". HOLD VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 3){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". LEAVE VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 6){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". NUM TRACKS VALUE " << informationRead <<
                            " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 7){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". FACTOR LENGTH VALUE " << informationRead <<
                            " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                }
            }
        }
    }
    m.addMap(enter, hold * factor_length, leave, direction, 0, mirror, numTracks);
    return instance.failDetected;
}

bool Logger::checkReliefHillStraight(Map& m){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 6;
    int enter, hold, leave, slope, numTracks, factor_length;

    while (!instance.failDetected && parametersRead < totalParametersToRead){
        instance.inputFlux >> informationRead;
        parametersRead++;

        if (instance.inputFlux.eof()){

            std::string parameterFailed;

            if (parametersRead == 1)
                parameterFailed = "ENTER";
            else if (parametersRead == 2)
                parameterFailed = "HOLD";
            else if (parametersRead == 3)
                parameterFailed = "LEAVE";
            else if (parametersRead == 4)
                parameterFailed = "SLOPE";
            else if (parametersRead == 5)
                parameterFailed = "NUM TRACKS";
            else
                parameterFailed = "FACTOR LENGTH";

            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". " << parameterFailed << " PARAMETER NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            if (parametersRead == 4){
                if (std::regex_match(informationRead, instance.integer_number_regex))
                    slope = std::stoi(informationRead);
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". SLOPE PARAMETER " << informationRead << " MUST BE INTEGER." << std::endl;

                    return !instance.failDetected;
                }
            }
            else {
                if (std::regex_match(informationRead, instance.natural_number_regex)){
                    if (parametersRead == 1)
                        enter = std::stoi(informationRead);
                    else if (parametersRead == 2)
                        hold = std::stoi(informationRead);
                    else if (parametersRead == 3)
                        leave = std::stoi(informationRead);
                    else if (parametersRead == 5){
                        numTracks = std::stoi(informationRead);
                        if (numTracks < 2 || numTracks > 8){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". NUM TRACKS VALUE " << numTracks <<
                                " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                            return !instance.failDetected;
                        }
                    }
                    else if (parametersRead == 6){
                        factor_length = std::stoi(informationRead);
                        if (factor_length <= 0){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". FACTOR LENGTH CANNOT BE NEGATIVE OR ZERO. " << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }
                else {
                    if (parametersRead == 1){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". ENTER VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 2){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". HOLD VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 3){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". LEAVE VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 5){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". NUM TRACKS VALUE " << informationRead <<
                            " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 6){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". FACTOR LENGTH VALUE " << informationRead <<
                            " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                }
            }
        }
    }
    m.addMap(enter, hold * factor_length, leave, 0, slope, false, numTracks);
    return instance.failDetected;
}

bool Logger::checkReliefHillCurve(Map& m){

    int parametersRead = 0, totalParametersToRead = 7;

    std::string informationRead;
    bool mirror;
    int enter, hold, leave, slope, numTracks;
    float direction;

    while (!instance.failDetected && parametersRead < totalParametersToRead){
        instance.inputFlux >> informationRead;
        parametersRead++;

        if (instance.inputFlux.eof()){

            std::string parameterFailed;

            if (parametersRead == 1)
                parameterFailed = "ENTER";
            else if (parametersRead == 2)
                parameterFailed = "HOLD";
            else if (parametersRead == 3)
                parameterFailed = "LEAVE";
            else if (parametersRead == 4)
                parameterFailed = "SLOPE";
            else if (parametersRead == 5)
                parameterFailed = "DIRECTION CURVE";
             else if (parametersRead == 6)
                parameterFailed = "MIRROR";
            else
                parameterFailed = "NUM TRACKS";

            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". " << parameterFailed << " PARAMETER NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            if (parametersRead == 4){
                if (std::regex_match(informationRead, instance.integer_number_regex))
                    slope = std::stoi(informationRead);
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". SLOPE PARAMETER " << informationRead << " MUST BE INTEGER." << std::endl;

                    return !instance.failDetected;
                }
            }
            else if (parametersRead == 5){
                if (std::regex_match(informationRead, instance.float_number_regex))
                    direction = std::stof(informationRead);
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". DIRECTION CURVE PARAMETER " << informationRead << " MUST BE FLOAT." << std::endl;

                    return !instance.failDetected;
                }
            }
            else if (parametersRead == 6){
                if (informationRead != "0" && informationRead != "1"){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". CANNOT CONVERT MIRROR VALUE " << informationRead <<
                        " TO BOOLEAN. THE VALUE MUST BE 0 OR 1." << std::endl;

                    return !instance.failDetected;
                }
                else if (informationRead == "1")
                    mirror = true;
                else
                    mirror = false;
            }
            else {
                if (std::regex_match(informationRead, instance.natural_number_regex)){
                    if (parametersRead == 1)
                        enter = std::stoi(informationRead);
                    else if (parametersRead == 2)
                        hold = std::stoi(informationRead);
                    else if (parametersRead == 3)
                        leave = std::stoi(informationRead);
                    else if (parametersRead == 7){
                        numTracks = std::stoi(informationRead);
                        if (numTracks < 2 || numTracks > 8){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". NUM TRACKS VALUE " << numTracks <<
                                " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }
                else {
                    if (parametersRead == 1){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". ENTER VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 2){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". HOLD VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 3){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". LEAVE VALUE " << informationRead << " MUST BE AN INTEGER POSITIVE." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (parametersRead == 7){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". NUM TRACKS VALUE " << informationRead <<
                            " IS OUT OF RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                        return !instance.failDetected;
                    }
                }
            }
        }
    }
    m.addMap(enter, hold, enter, direction, slope, mirror, numTracks);
    return instance.failDetected;
}

bool Logger::checkMapRelief(Map& m){

    std::string informationRead;
    instance.inputFlux >> informationRead;

    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN START_RELIEF NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        if (informationRead != "START_RELIEF"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN START_RELIEF BUT FOUND " << informationRead << "." << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.inputFlux >> informationRead;

            while (!instance.failDetected && informationRead != "SPRITES"){

                instance.row++;
                instance.column = 1;

                if (instance.inputFlux.eof()){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". CANDIDATE IDENTIFIER TOKEN FOR RELIEF NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    if (informationRead != "STRAIGHT:" && informationRead != "CURVE_LEFT:" && informationRead != "CURVE_RIGHT:" &&
                        informationRead != "HILL_STRAIGHT:" && informationRead != "HILL_LEFT:" && informationRead != "HILL_RIGHT:")
                    {
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". EXPECTED ONE OF THIS RELIEF IDENTIFIER TOKENS (STRAIGHT:, CURVE_LEFT:, CURVE_RIGHT:"
                                        << " HILL_STRAIGHT:, HILL_LEFT:, HILL_RIGHT:. BUT FOUND " << informationRead << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        if (informationRead == "STRAIGHT:"){
                            checkReliefStraight(m);
                        }
                        else if (informationRead == "CURVE_LEFT:" || informationRead == "CURVE_RIGHT:"){
                            checkReliefCurve(m);
                        }
                        else if (informationRead == "HILL_STRAIGHT:"){
                            checkReliefHillStraight(m);
                        }
                        else {
                            checkReliefHillCurve(m);
                        }
                    }
                }
                instance.inputFlux >> informationRead;
            }
            instance.outputFlux << "RELIEF PART CORRECTLY SPECIFIED." << std::endl;
            return instance.failDetected;
        }
    }
}


