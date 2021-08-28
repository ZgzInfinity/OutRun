
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
            instance.row++;
            instance.column = 1;
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
    numTracks = m.computeRoadTracks(numTracks);
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
    numTracks = m.computeRoadTracks(numTracks);
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
    numTracks = m.computeRoadTracks(numTracks);
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
    numTracks = m.computeRoadTracks(numTracks);
    m.addMap(enter, hold, enter, direction, slope, mirror, numTracks);
    return instance.failDetected;
}

bool Logger::checkSprite(Map& m, const int startPos, const int endPos, const int incrementor, const int frequency,
                         bool& indexSpecified, bool& spritesProcessed, const bool left){

    std::string informationRead;
    int idProp;
    float offsetX, offsetY;
    bool side;
    instance.column += 2;

    if (indexSpecified){
        int parametersToRead = 4;
        for (int i = 0; i < parametersToRead; i++){
            instance.inputFlux >> informationRead;
            if (instance.inputFlux.eof()){
                std::string parameterFailed;
                if (i == 0){
                    parameterFailed = "IDENTIFIER OF SPRITE";
                }
                else if (i == 1){
                    parameterFailed = "POSITION OF SPRITE IN X";
                }
                else if (i == 2){
                    parameterFailed = "POSITION OF SPRITE IN Y";
                }
                else {
                    parameterFailed = "SIDE OF SPRITE";
                }

                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". " << parameterFailed << " NOT FOUND." << std::endl;

                return !instance.failDetected;
            }
            else {
                if (i == 0){
                    if (std::regex_match(informationRead, instance.natural_number_regex) && informationRead != "0"){
                        idProp = std::stoi(informationRead);
                    }
                    else {
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". IDENTIFIER OF SPRITE " << informationRead << " MUST BE A POSITIVE INTEGER." << std::endl;

                        return !instance.failDetected;
                    }
                }
                else if (i == 3){
                    if (informationRead != "0" && informationRead != "1"){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". CANNOT CONVERT SIDE OF SPRITE VALUE " << informationRead <<
                            " TO BOOLEAN. THE VALUE MUST BE 0 OR 1." << std::endl;

                        return !instance.failDetected;
                    }
                    else if (informationRead == "1")
                        side = true;
                    else
                        side = false;
                }
                else {
                    if (std::regex_match(informationRead, instance.float_number_regex)){
                        if (i == 1)
                            offsetX = std::stof(informationRead);
                        else
                            offsetY = std::stof(informationRead);
                    }
                    else {
                        std::string parameterFailed;
                        if (i == 1)
                            parameterFailed = "POSITION OF SPRITE IN X";
                        else
                            parameterFailed = "POSITION OF SPRITE IN Y";

                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". " << parameterFailed << " VALUE MUST BE FLOAT." << std::endl;

                        return !instance.failDetected;
                    }
                }
            }
        }
    }
    else {
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". SPRITES PART IS TOTALLY EMPTY." << std::endl;

        return !instance.failDetected;
    }

    SpriteInfo* newSprite = new SpriteInfo(&instance.objects[idProp - 1], instance.pivotLeftPoints[idProp - 1],
                                           instance.pivotRightPoints[idProp - 1], instance.scaleCoeffs[idProp - 1],
                                           instance.widthCollisionCoeffs[idProp - 1], instance.pivotLeftColPoints[idProp - 1],
                                           instance.pivotRightColPoints[idProp - 1], offsetX, offsetY, side);

    if (endPos == -1)
        m.addSpriteInfo(startPos, newSprite, left);
    else {
        for (int i = startPos; i < endPos; i += incrementor){
            if (i % frequency == 0)
                m.addSpriteInfo(i, newSprite, left);
        }
    }
    instance.row++;
    instance.column = 1;

    if (!spritesProcessed)
        spritesProcessed = true;

    return instance.failDetected;
}

bool Logger::checkMapSprites(Map& m){

    std::string informationRead;
    bool spritesProcessed = false;
    bool indexSpecified = false, spriteLeft = false, spriteRight = false, newInterval = false;
    int startPos = 0, endPos = -1, incrementor = 0, frequency = 0;

    instance.inputFlux >> informationRead;


    while (!instance.inputFlux.eof()){
        if (informationRead == "GROUP_LINES:"){
            if (indexSpecified){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". INTERVAL OF LINES PROCESSED, SRITE_LEFT: OR SPRITE_RIGHT: TOKEN WAS EXPECTED." << std::endl;

                return !instance.failDetected;
            }
            else {
                spriteLeft = false;
                spriteRight = false;
                newInterval = false;
                instance.column += 2;
                int parametersToRead = 4;
                for (int i = 0; i < parametersToRead; i++){
                    instance.inputFlux >> informationRead;
                    if (instance.inputFlux.eof()){
                        std::string parameterFailed;
                        if (i == 0){
                            parameterFailed = "START POSITION OF INTERVAL";
                        }
                        else if (i == 1){
                            parameterFailed = "END POSITION OF INTERVAL";
                        }
                        else if (i == 2){
                            parameterFailed = "INCREMENTOR PARAMETER";
                        }
                        else {
                            parameterFailed = "FREQUENCY PARAMETER";
                        }

                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". " << parameterFailed << " NOT FOUND." << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        if (std::regex_match(informationRead, instance.natural_number_regex)){
                            if (i == 0){
                                startPos = std::stoi(informationRead);
                            }
                            else if (i == 1){
                                endPos = std::stoi(informationRead);

                                if (endPos <= startPos){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". END INTERVAL VALUE " << endPos << " MUST BE GREATER THAN "
                                                        << "START INTERVAL VALUE " << startPos << std::endl;

                                    return !instance.failDetected;
                                }
                            }
                            else if (i == 2){
                                incrementor = std::stoi(informationRead);
                                if (incrementor == 0){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". INCREMENTOR VALUE " << incrementor << " MUST BE POSITIVE." << std::endl;

                                    return !instance.failDetected;
                                }
                            }
                            else {
                                frequency = std::stoi(informationRead);

                                if (frequency == 0){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". FREQUENCY VALUE " << frequency << " MUST BE POSITIVE." << std::endl;

                                    return !instance.failDetected;
                                }
                            }
                            instance.column += 2;
                        }
                        else {
                            std::string parameterFailed;
                            if (i == 0){
                                parameterFailed = "START POSITION OF INTERVAL";
                            }
                            else if (i == 1){
                                parameterFailed = "END POSITION OF INTERVAL";
                            }
                            else if (i == 2){
                                parameterFailed = "INCREMENTOR PARAMETER";
                            }
                            else {
                                parameterFailed = "FREQUENCY PARAMETER";
                            }

                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". " << parameterFailed << " VALUE " << informationRead
                                                << " MUST BE A AN INTEGER POSITIVE AND NOT ZERO. " << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }
                instance.row++;
                instance.column = 1;
                indexSpecified = true;
            }
        }
        else if (informationRead == "LINE:"){
            if (indexSpecified){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". INTERVAL OF LINES PROCESSED, SRITE_LEFT OR SPRITE_RIGHT TOKEN WAS EXPECTED." << std::endl;

                return !instance.failDetected;
            }
            else {
                spriteLeft = false;
                spriteRight = false;
                newInterval = false;
                instance.column += 2;
                instance.inputFlux >> informationRead;
                if (instance.inputFlux.eof()){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". INDEX CODE OF LINE NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    if (std::regex_match(informationRead, instance.natural_number_regex)){
                        startPos = std::stoi(informationRead);
                        instance.row++;
                        instance.column = 1;
                    }
                    else {
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". INDEX CODE OF LINE " << informationRead
                                            << " MUST BE AN INTEGER." << std::endl;

                        return !instance.failDetected;
                    }
                }
                indexSpecified = true;
            }
        }
        else if (informationRead == "SPRITE_LEFT:"){
            if (spriteLeft){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_LEFT ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_LEFT MUST BE DEFINED BEFORE SPRITE_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkSprite(m, startPos, endPos, incrementor, frequency,
                                                            indexSpecified, spritesProcessed, true);

                if (!instance.failDetected)
                    spriteLeft = true;

                instance.inputFlux >> informationRead;

                if (informationRead == "GROUP_LINES:" || informationRead == "LINE:"){
                    indexSpecified = false;
                }
                newInterval = true;
            }
        }
        else if (informationRead == "SPRITE_RIGHT:"){
            if (spriteRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_RIGHTA ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkSprite(m, startPos, endPos, incrementor, frequency,
                                                            indexSpecified, spritesProcessed, false);

                if (!instance.failDetected)
                    spriteRight = true;

                indexSpecified = false;
                newInterval = false;
            }
        }
        else {
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN LINE:, GROUP_LINES:, SPRITE_LEFT: OR SPRITE_RIGHT: BUT FOUND "
                                << informationRead << std::endl;

            return !instance.failDetected;
        }
        if (!newInterval){
            instance.inputFlux >> informationRead;
        }
    }
    if (spritesProcessed){
        instance.outputFlux << "SPRITES PART CORRECTLY SPECIFIED." << std::endl;
        return instance.failDetected;
    }
    else {
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". SPRITE PART IS TOTALLY EMPTY." << std::endl;

        return !instance.failDetected;
    }
}

bool Logger::checkMapRelief(Map& m){

    std::string informationRead;
    bool reliefProccessed = false;

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
                            instance.failDetected = checkReliefStraight(m);
                        }
                        else if (informationRead == "CURVE_LEFT:" || informationRead == "CURVE_RIGHT:"){
                            instance.failDetected = checkReliefCurve(m);
                        }
                        else if (informationRead == "HILL_STRAIGHT:"){
                            instance.failDetected = checkReliefHillStraight(m);
                        }
                        else {
                            instance.failDetected = checkReliefHillCurve(m);
                        }
                        if (!reliefProccessed)
                            reliefProccessed = true;
                    }
                }
                instance.inputFlux >> informationRead;
            }
            instance.row++;
            instance.column = 1;

            if (!reliefProccessed){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITES IDENTIFIER TOKEN FOUND BUT RELIEF PART IS TOTALLY EMPTY." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.outputFlux << "RELIEF PART CORRECTLY SPECIFIED." << std::endl;
                instance.row++;
                instance.column = 1;
                return instance.failDetected;
            }
        }
    }
}


void Logger::loadObjects(const string &path, const vector<string> &objectNames){

    instance.objects.reserve(objectNames.size());
    instance.widthCollisionCoeffs.reserve(objectNames.size());
    instance.pivotLeftPoints.reserve(objectNames.size());
    instance.pivotRightPoints.reserve(objectNames.size());
    instance.pivotLeftColPoints.reserve(objectNames.size());
    instance.pivotRightColPoints.reserve(objectNames.size());

    for (const string &objName : objectNames) {

        // Load textures
        sf::Texture t;
        t.loadFromFile(path + objName + ".png");
        instance.objects.push_back(t);

        ifstream fin(path + objName + ".txt");

        float scale = 1.f;
        float widthCollision = t.getSize().x;
        fPoint pivotLeft = {1.f, 1.f};
        fPoint pivotRight = {0.f, 1.f};
        fPoint pivotColLeft = {0.5f, 1.f};
        fPoint pivotColRight = {0.5f, 1.f};

        float pointX, pointY;

        if (fin.is_open()) {
            while (!fin.eof()) {
                string s;
                fin >> s;
                if (s == "SCALE:" && !fin.eof()){
                    fin >> scale;
                }
                else if (s == "WIDTH_COLLISION:" && !fin.eof()){
                    fin >> widthCollision;
                }
                else if (s == "PIVOT_LEFT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotLeft = {pointX, pointY};
                }
                else if (s == "PIVOT_RIGHT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotRight = {pointX, pointY};
                }
                else if (s == "PIVOT_COL_LEFT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotColLeft = {pointX, pointY};
                }
                else if (s == "PIVOT_COL_RIGHT:" && !fin.eof()){
                    fin >> pointX >> pointY;
                    pivotColRight = {pointX, pointY};
                }
                else if (!s.empty()) {
                    cerr << "WARNING: '" << s << "' at file " << path + objName + ".info" << endl;
                }
            }
            fin.close();
        }
        instance.scaleCoeffs.push_back(scale);
        instance.widthCollisionCoeffs.push_back(widthCollision);
        instance.pivotLeftPoints.push_back(pivotLeft);
        instance.pivotRightPoints.push_back(pivotRight);
        instance.pivotLeftColPoints.push_back(pivotColLeft);
        instance.pivotRightColPoints.push_back(pivotColRight);
    }
}


