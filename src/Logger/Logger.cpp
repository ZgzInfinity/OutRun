
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
    instance.firstTimeInvoked = true;
    setLoggerStatus();
}

void Logger::setLoggerStatus(){
    instance.failDetected = false;
    instance.endFlaggerAnimation = false;
    instance.flagger_code_image = 0;
    instance.semaphore_code_image = 38;
    instance.attemps = 0;
    instance.numIterations = 0;
    instance.totalTimes = 0;
    instance.status = Flagger_Status::UPPING_FLAG;
    instance.linesOfBiome = 0;
}

void Logger::setFailDetected(const bool _failDetected){
    instance.failDetected = _failDetected;
}

void Logger::setWidthScreen(const int _widthScreen){
    instance.widthScreen = _widthScreen;
}

void Logger::setSpriteScreenY(Biome& m){
    if (m.startBiome)
        m.setSpriteScreenY(310, float(instance.widthScreen * -1.6f / SCREEN_0.first));
    else if (m.goalBiome){
        m.setSpriteScreenY(706, float(instance.widthScreen * -1.6f / SCREEN_0.first));
        m.setSpriteScreenY(806, float(instance.widthScreen * -1.6f / SCREEN_0.first));
    }
    else {
        for (int i = 0; i < m.floatingSprites.size(); i++){
            m.setSpriteScreenY(m.floatingSprites.at(i), float(instance.widthScreen * -1.45f / SCREEN_0.first));
        }
    }
}

bool Logger::getFailDetected(){
    return instance.failDetected;
}

bool Logger::checkMapFile(const std::string& pathMapFile){

    if (instance.firstTimeInvoked){
        instance.firstTimeInvoked = false;
        instance.outputFlux.open(instance.LOGGER_PATH_FILE, std::fstream::trunc);
    }
    else
        instance.outputFlux.open(instance.LOGGER_PATH_FILE, std::ios_base::app);

    if (!instance.outputFlux.is_open())
        instance.failDetected = true;

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


bool Logger::checkTimeTerrainRoad(Biome& m){
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
                                if (terrain < 1 || terrain > 4){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". TERRAIN VALUE " << informationRead << " IS OUT OF RANGE. MUST BE "
                                                        << "1 (SAND), 2 (GRASS), 3 (SNOW) OR 4 (MUD)." << std::endl;
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

                        instance.row++;
                        instance.column = 1;

                        instance.inputFlux >> informationRead;

                        if (instance.inputFlux.eof()){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". IDENTIFIER ROAD_TERRAIN: NOT FOUND." << std::endl;

                            return !instance.failDetected;
                        }
                        else {
                            if (informationRead != "ROAD_TERRAIN:"){
                                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                    instance.column << ". EXPECTED IDENTIFIER TOKEN ROAD_TERRAIN: BUT FOUND " << informationRead << "." << std::endl;

                                return !instance.failDetected;
                            }
                            else {
                                instance.column += 2;
                                instance.inputFlux >> informationRead;
                                if (instance.inputFlux.eof()){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". ROAD_TERRAIN VALUE NOT FOUND." << std::endl;

                                    return !instance.failDetected;
                                }
                                else {
                                    if (std::regex_match(informationRead, instance.natural_number_regex)){
                                        int roadTerrain = stoi(informationRead);
                                        if (roadTerrain < 0 || roadTerrain > 4){
                                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                                instance.column << ". ROAD VALUE " << roadTerrain << " MUST BETWEEN 0 AND 4" << std::endl;
                                        }
                                        else
                                            m.setRoadTerrain(roadTerrain);
                                    }
                                }
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


bool Logger::checkColors(Biome& m){
    std::string informationRead, colorToBeRead;
    std::vector<sf::Color> colorsOfBiome;
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

            for (int i = 1; i <= 11; i++){
                if (i == 1)
                    colorToBeRead = "COLOR_BACKGROUND:";
                else if (i == 2)
                    colorToBeRead = "COLOR_OUTSIDE_ROAD_1:";
                else if (i == 3)
                    colorToBeRead = "COLOR_OUTSIDE_ROAD_2:";
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
                else if (i == 9)
                    colorToBeRead = "COLOR_LANE_2:";
                 else if (i == 10)
                    colorToBeRead = "COLOR_RUMBLE_LANE_1:";
                else
                    colorToBeRead = "COLOR_RUMBLE_LANE_2:";

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
                            colorsOfBiome.push_back(colorRead);
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
            m.setColors(colorsOfBiome);
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



bool Logger::checkReliefStraight(Biome& m){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 5;

    bool mirror;
    int enter, hold, leave, numTracks, distM;

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
    m.addBiome(enter, hold, leave, 0, 0, mirror, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}


bool Logger::checkReliefCurve(Biome& m, const bool leftDirection){

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
                if (std::regex_match(informationRead, instance.float_number_regex)){
                    direction = std::stof(informationRead);

                    if (leftDirection && direction >= 0.0f){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". LEFT CURVE DIRECTION " << direction << " MUST BE LOWER THAN ZERO. " << std::endl;

                        return !instance.failDetected;
                    }
                    else if (!leftDirection && direction <= 0.0f){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". RIGHT CURVE DIRECTION " << direction << " MUST BE HIGHER THAN ZERO. " << std::endl;

                        return !instance.failDetected;
                    }
                }
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
    m.addBiome(enter, hold * factor_length, leave, direction, 0, mirror, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}

bool Logger::checkReliefHillStraight(Biome& m){

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
    m.addBiome(enter, hold * factor_length, leave, 0, slope, false, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}

bool Logger::checkReliefHillCurve(Biome& m, const bool leftDirection){

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
                if (std::regex_match(informationRead, instance.float_number_regex)){
                    direction = std::stof(informationRead);

                    if (leftDirection && direction >= 0.0f){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". LEFT HILL DIRECTION " << direction << " MUST BE LOWER THAN ZERO. " << std::endl;

                        return !instance.failDetected;
                    }
                    else if (!leftDirection && direction <= 0.0f){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". RIGHT HILL DIRECTION " << direction << " MUST BE HIGHER THAN ZERO. " << std::endl;

                        return !instance.failDetected;
                    }
                }
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
    m.addBiome(enter, hold, enter, direction, slope, mirror, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}

bool Logger::checkLevelBiomeSprite(Biome& m, const int startPos, const int endPos, const int incrementor, const int frequency,
                         bool& indexSpecified, bool& spritesProcessed, const int pos, const bool isCheckPoint){

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

    SpriteInfo* newSprite = new SpriteInfo(&m.objects[idProp - 1], m.pivotLeftPoints[idProp - 1],
                                           m.pivotRightPoints[idProp - 1], m.scaleCoeffs[idProp - 1],
                                           m.widthCollisionCoeffs[idProp - 1], m.showTerrainCoeffs[idProp - 1],
                                           m.pivotLeftColPoints[idProp - 1], m.pivotRightColPoints[idProp - 1],
                                           m.collisions[idProp - 1], offsetX, offsetY, side);

    Sprite_Position spritePos = (pos == 0) ? Sprite_Position::FAR_LEFT : (pos == 1) ? Sprite_Position::NEAR_LEFT :
                                (pos == 2) ? Sprite_Position::CENTER : (pos == 3) ? Sprite_Position::FAR_RIGHT : Sprite_Position::NEAR_RIGHT;

    if (endPos == -1){
        m.addSpriteInfo(startPos, newSprite, spritePos, isCheckPoint);
        if (offsetY != 0.f)
            m.floatingSprites.push_back(startPos);
    }
    else {
        for (int i = startPos; i < endPos; i += incrementor){
            if (i % frequency == 0){
                m.addSpriteInfo(i, newSprite, spritePos, false);

                if (offsetY != 0.f)
                    m.floatingSprites.push_back(i);
            }
        }
    }
    instance.row++;
    instance.column = 1;

    if (!spritesProcessed)
        spritesProcessed = true;

    return instance.failDetected;
}

bool Logger::checkLevelBiomeSprites(Biome& m){

    std::string informationRead;

    bool isCheckPoint = false;
    bool spritesProcessed = false;
    bool indexSpecified = false, spriteFarLeft = false, spriteFarRight = false,
         spriteNearLeft = false, spriteNearRight = false, newInterval = false,
         spriteCenter = false;

    int startPos = 0, endPos = -1, incrementor = 0, frequency = 0;

    instance.inputFlux >> informationRead;

    while (!instance.failDetected && informationRead != "END_FILE"){

        if (informationRead == "GROUP_LINES:"){
            if (indexSpecified){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". INTERVAL OF LINES PROCESSED, SRITE_NEAR_LEFT: OR SPRITE_NEAR_RIGHT: TOKEN WAS EXPECTED." << std::endl;

                return !instance.failDetected;
            }
            else {
                spriteFarLeft = false;
                spriteFarRight = false;
                spriteNearLeft = false;
                spriteNearRight = false;
                spriteCenter = false;
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

                                if (startPos >= instance.linesOfBiome){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". START INTERVAL VALUE " << startPos << " IS HIGHER OR EQUAL THAN "
                                                        << "MAP SIZE " << instance.linesOfBiome << std::endl;

                                    return !instance.failDetected;
                                }
                            }
                            else if (i == 1){
                                endPos = std::stoi(informationRead);

                                if (endPos >= instance.linesOfBiome){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". END INTERVAL VALUE " << endPos << " IS HIGHER THAN "
                                                        << "MAP SIZE " << instance.linesOfBiome << std::endl;

                                    return !instance.failDetected;
                                }

                                if (endPos <= startPos){
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". END INTERVAL VALUE " << endPos << " MUST BE GREATER OR EQUAL THAN "
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
        else if (informationRead == "LINE:" || informationRead == "LINE_CHECKPOINT:"){
            if (indexSpecified){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". INTERVAL OF LINES PROCESSED, SRITE_NEAR_LEFT OR SPRITE_NEAR_RIGHT TOKEN WAS EXPECTED." << std::endl;

                return !instance.failDetected;
            }
            else {
                if (informationRead == "LINE_CHECKPOINT:")
                    isCheckPoint = true;

                endPos = -1;
                spriteFarLeft = false;
                spriteFarRight = false;
                spriteNearLeft = false;
                spriteNearRight = false;
                spriteCenter = false;
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

                        if (startPos >= instance.linesOfBiome){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". START INTERVAL VALUE " << startPos << " IS HIGHER OR EQUAL THAN "
                                                << "MAP SIZE " << instance.linesOfBiome << std::endl;

                            return !instance.failDetected;
                        }

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
        else if (informationRead == "SPRITE_FAR_LEFT:"){
            if (spriteFarLeft){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_LEFT ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteNearLeft){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_LEFT MUST BE DEFINED BEFORE SPRITE_NEAR_LEFT." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteCenter){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_LEFT MUST BE DEFINED BEFORE SPRITE_CENTER." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteFarRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_LEFT MUST BE DEFINED BEFORE SPRITE_FAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteNearRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_LEFT MUST BE DEFINED BEFORE SPRITE_NEAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkLevelBiomeSprite(m, startPos, endPos, incrementor, frequency,
                                                                    indexSpecified, spritesProcessed, 0, isCheckPoint);

                if (!instance.failDetected)
                    spriteFarLeft = true;

                instance.inputFlux >> informationRead;

                if (informationRead == "GROUP_LINES:" || informationRead == "LINE:" || informationRead == "LINE_CHECKPOINT:"){
                    indexSpecified = false;
                    isCheckPoint = false;
                }
                newInterval = true;
            }
        }
        else if (informationRead == "SPRITE_NEAR_LEFT:"){
            if (spriteNearLeft){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_NEAR_LEFT ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteCenter){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_NEAR_LEFT MUST BE DEFINED BEFORE SPRITE_CENTER." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteFarRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_NEAR_LEFT MUST BE DEFINED BEFORE SPRITE_FAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteNearRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_NEAR_LEFT MUST BE DEFINED BEFORE SPRITE_NEAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkLevelBiomeSprite(m, startPos, endPos, incrementor, frequency,
                                                                    indexSpecified, spritesProcessed, 1, isCheckPoint);

                if (!instance.failDetected)
                    spriteNearLeft = true;

                instance.inputFlux >> informationRead;

                if (informationRead == "GROUP_LINES:" || informationRead == "LINE:" || informationRead == "LINE_CHECKPOINT:"){
                    indexSpecified = false;
                    isCheckPoint = false;
                }
                newInterval = true;
            }
        }
        else if (informationRead == "SPRITE_CENTER:"){
            if (spriteCenter){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". SPRITE_CENTER ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteFarRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_CENTER MUST BE DEFINED BEFORE SPRITE_FAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteNearRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_CENTER MUST BE DEFINED BEFORE SPRITE_NEAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkLevelBiomeSprite(m, startPos, endPos, incrementor, frequency,
                                                                    indexSpecified, spritesProcessed, 2, isCheckPoint);

                if (!instance.failDetected)
                    spriteCenter = true;

                instance.inputFlux >> informationRead;

                if (informationRead == "GROUP_LINES:" || informationRead == "LINE:" || informationRead == "LINE_CHECKPOINT:"){
                    indexSpecified = false;
                    isCheckPoint = false;
                }
                newInterval = true;
            }
        }
        else if (informationRead == "SPRITE_FAR_RIGHT:"){
            if (spriteFarRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_RIGHT ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else if (spriteNearRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_FAR_RIGHT MUST BE DEFINED BEFORE SPRITE_NEAR_RIGHT." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkLevelBiomeSprite(m, startPos, endPos, incrementor, frequency,
                                                                    indexSpecified, spritesProcessed, 3, isCheckPoint);

                if (!instance.failDetected)
                    spriteFarRight = true;

                instance.inputFlux >> informationRead;

                if (informationRead == "GROUP_LINES:" || informationRead == "LINE:" || informationRead == "LINE_CHECKPOINT:"){
                    indexSpecified = false;
                    isCheckPoint = false;
                }
                newInterval = true;
            }
        }
        else if (informationRead == "SPRITE_NEAR_RIGHT:"){
            if (spriteNearRight){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITE_NEAR_RIGHT ALREADY SPECIFIED." << std::endl;

                return !instance.failDetected;
            }
            else {
                instance.failDetected = Logger::checkLevelBiomeSprite(m, startPos, endPos, incrementor, frequency,
                                                                    indexSpecified, spritesProcessed, 4, isCheckPoint);

                if (!instance.failDetected)
                    spriteNearRight = true;

                indexSpecified = false;
                newInterval = false;
                isCheckPoint = false;
            }
        }
        else {
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN LINE:, LINE_CHECKPOINT:, GROUP_LINES:, SPRITE_NEAR_LEFT:"
                                << " OR SPRITE_NEAR_RIGHT: BUT FOUND "
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

bool Logger::checkBiomeRelief(Biome& m){

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
                            bool leftDirection = (informationRead == "CURVE_LEFT:") ? true : false;
                            instance.failDetected = checkReliefCurve(m, leftDirection);
                        }
                        else if (informationRead == "HILL_STRAIGHT:")
                            instance.failDetected = checkReliefHillStraight(m);
                        else {
                            bool leftDirection = (informationRead == "HILL_LEFT:") ? true : false;
                            instance.failDetected = checkReliefHillCurve(m, leftDirection);
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

void Logger::loadStartBiomeSprites(Biome& m){

    // First row
    SpriteInfo* cameraman = new SpriteInfo(&m.objects[24], m.pivotLeftPoints[24], m.pivotRightPoints[24],
                                           m.scaleCoeffs[24], m.widthCollisionCoeffs[24], m.showTerrainCoeffs[24],
                                           m.pivotLeftColPoints[24], m.pivotRightColPoints[24], m.collisions[24],
                                           -0.6f, 0.f, false);

    SpriteInfo* musicman = new SpriteInfo(&m.objects[32], m.pivotLeftPoints[32], m.pivotRightPoints[32],
                                           m.scaleCoeffs[32], m.widthCollisionCoeffs[32], m.showTerrainCoeffs[32],
                                           m.pivotLeftColPoints[32],m.pivotRightColPoints[32],  m.collisions[32],
                                           -0.32f, 0.f, false);

    SpriteInfo* man1 = new SpriteInfo(&m.objects[15], m.pivotLeftPoints[15], m.pivotRightPoints[15],
                                      m.scaleCoeffs[15], m.widthCollisionCoeffs[15], m.showTerrainCoeffs[15],
                                      m.pivotLeftColPoints[15], m.pivotRightColPoints[15], m.collisions[15],
                                      0.65f, 0.f, false);

    SpriteInfo* woman1 = new SpriteInfo(&m.objects[27], m.pivotLeftPoints[27], m.pivotRightPoints[27],
                                        m.scaleCoeffs[27], m.widthCollisionCoeffs[27], m.showTerrainCoeffs[27],
                                        m.pivotLeftColPoints[27], m.pivotRightColPoints[27], m.collisions[27],
                                        0.32f, 0.f, false);

    // Second row
    SpriteInfo* woman2 = new SpriteInfo(&m.objects[30], m.pivotLeftPoints[30], m.pivotRightPoints[30],
                                        m.scaleCoeffs[30], m.widthCollisionCoeffs[30], m.showTerrainCoeffs[30],
                                        m.pivotLeftColPoints[30], m.pivotRightColPoints[30], m.collisions[30],
                                        -0.58f, 0.f, false);

    SpriteInfo* flagger = new SpriteInfo(&m.objects[0], m.pivotLeftPoints[0], m.pivotRightPoints[0], m.scaleCoeffs[0],
                                         m.widthCollisionCoeffs[0], m.showTerrainCoeffs[0], m.pivotLeftColPoints[0],
                                         m.pivotRightColPoints[0], m.collisions[0], -0.32f, 0.f, false);

    SpriteInfo* man2 = new SpriteInfo(&m.objects[17], m.pivotLeftPoints[17], m.pivotRightPoints[17],
                                      m.scaleCoeffs[17], m.widthCollisionCoeffs[17], m.showTerrainCoeffs[17],
                                      m.pivotLeftColPoints[17], m.pivotRightColPoints[17], m.collisions[17],
                                      0.28f, 0.f, false);

    SpriteInfo* man3 = new SpriteInfo(&m.objects[21], m.pivotLeftPoints[21], m.pivotRightPoints[21],
                                      m.scaleCoeffs[21], m.widthCollisionCoeffs[21], m.showTerrainCoeffs[21],
                                      m.pivotLeftColPoints[21], m.pivotRightColPoints[21], m.collisions[21],
                                      0.68f, 0.f, false);

    // Third row
    SpriteInfo* man4 = new SpriteInfo(&m.objects[19], m.pivotLeftPoints[19], m.pivotRightPoints[19],
                                      m.scaleCoeffs[19], m.widthCollisionCoeffs[19], m.showTerrainCoeffs[19],
                                      m.pivotLeftColPoints[19], m.pivotRightColPoints[19], m.collisions[19],
                                      -0.75f, 0.f, false);


    SpriteInfo* man5 = new SpriteInfo(&m.objects[23], m.pivotLeftPoints[23], m.pivotRightPoints[23], m.scaleCoeffs[23],
                                      m.widthCollisionCoeffs[23], m.showTerrainCoeffs[23], m.pivotLeftColPoints[23],
                                      m.pivotRightColPoints[23], m.collisions[23], -0.58f, 0.f, false);

    SpriteInfo* woman3 = new SpriteInfo(&m.objects[29], m.pivotLeftPoints[29], m.pivotRightPoints[29], m.scaleCoeffs[29],
                                        m.widthCollisionCoeffs[29], m.showTerrainCoeffs[29], m.pivotLeftColPoints[29],
                                        m.pivotRightColPoints[29], m.collisions[29], 0.74f, 0.f, false);

    SpriteInfo* woman4 = new SpriteInfo(&m.objects[26], m.pivotLeftPoints[26], m.pivotRightPoints[26], m.scaleCoeffs[26],
                                        m.widthCollisionCoeffs[26], m.showTerrainCoeffs[26],m.pivotLeftColPoints[26],
                                        m.pivotRightColPoints[26], m.collisions[26], 0.60f, 0.f, false);

    SpriteInfo* cameramanRight = new SpriteInfo(&m.objects[24], m.pivotLeftPoints[24], m.pivotRightPoints[24],
                                                m.scaleCoeffs[24], m.widthCollisionCoeffs[24], m.showTerrainCoeffs[24],
                                                m.pivotLeftColPoints[24], m.pivotRightColPoints[24], m.collisions[24],
                                                0.28f, 0.f, false);

    SpriteInfo* man6 = new SpriteInfo(&m.objects[20], m.pivotLeftPoints[20], m.pivotRightPoints[20], m.scaleCoeffs[20],
                                      m.widthCollisionCoeffs[20], m.showTerrainCoeffs[20], m.pivotLeftColPoints[20],
                                      m.pivotRightColPoints[20], m.collisions[20], -0.31f, 0.f, false);

    SpriteInfo* palm1 = new SpriteInfo(&m.objects[35], m.pivotLeftPoints[35], m.pivotRightPoints[35], m.scaleCoeffs[35],
                                       m.widthCollisionCoeffs[35], m.showTerrainCoeffs[35], m.pivotLeftColPoints[35],
                                       m.pivotRightColPoints[35], m.collisions[35], -0.8f, 0.f, false);

    SpriteInfo* palm2 = new SpriteInfo(&m.objects[36], m.pivotLeftPoints[36], m.pivotRightPoints[36], m.scaleCoeffs[36],
                                       m.widthCollisionCoeffs[36], m.showTerrainCoeffs[36], m.pivotLeftColPoints[36],
                                       m.pivotRightColPoints[36], m.collisions[36], 0.8f, 0.f, false);

    // Fifth row
    SpriteInfo* startFlag = new SpriteInfo(&m.objects[37], m.pivotLeftPoints[37], m.pivotRightPoints[37], m.scaleCoeffs[37],
                                           m.widthCollisionCoeffs[37], m.showTerrainCoeffs[37], m.pivotLeftColPoints[37],
                                           m.pivotRightColPoints[37], m.collisions[37], -1.33f,
                                           float(instance.widthScreen * -1.6f / SCREEN_0.first), false);

    SpriteInfo* trafficLightLeftPanel = new SpriteInfo(&m.objects[38], m.pivotLeftPoints[38], m.pivotRightPoints[38],
                                                       m.scaleCoeffs[38], m.widthCollisionCoeffs[38], m.showTerrainCoeffs[38],
                                                       m.pivotLeftColPoints[38], m.pivotRightColPoints[38], m.collisions[38],
                                                       -0.8f, 0.f, false);

    SpriteInfo* trafficLightRightPanel = new SpriteInfo(&m.objects[42], m.pivotLeftPoints[42], m.pivotRightPoints[42],
                                           m.scaleCoeffs[42], m.widthCollisionCoeffs[42], m.showTerrainCoeffs[42],
                                           m.pivotLeftColPoints[42], m.pivotRightColPoints[42], m.collisions[42],
                                           0.8f, 0.f, false);

    SpriteInfo* palm1Start = new SpriteInfo(&m.objects[35], m.pivotLeftPoints[35], m.pivotRightPoints[35],
                                           m.scaleCoeffs[35], m.widthCollisionCoeffs[35], m.showTerrainCoeffs[35],
                                           m.pivotLeftColPoints[35], m.pivotRightColPoints[35], m.collisions[35],
                                           -0.6f, 0.f, false);

    SpriteInfo* palm2Start = new SpriteInfo(&m.objects[36], m.pivotLeftPoints[36], m.pivotRightPoints[36],
                                           m.scaleCoeffs[36], m.widthCollisionCoeffs[36], m.showTerrainCoeffs[36],
                                           m.pivotLeftColPoints[36], m.pivotRightColPoints[36], m.collisions[36],
                                           0.6f, 0.f, false);

    m.addSpriteInfo(305, cameraman, Sprite_Position::FAR_LEFT, false);
    m.addSpriteInfo(305, musicman, Sprite_Position::NEAR_LEFT, false);

    m.addSpriteInfo(306, man1, Sprite_Position::FAR_RIGHT, false);
    m.addSpriteInfo(306, woman1, Sprite_Position::NEAR_RIGHT, false);
    m.addSpriteInfo(306, woman2, Sprite_Position::FAR_LEFT, false);
    m.addSpriteInfo(306, flagger, Sprite_Position::NEAR_LEFT, false);

    m.addSpriteInfo(307, man2, Sprite_Position::FAR_RIGHT, false);
    m.addSpriteInfo(307, man3, Sprite_Position::NEAR_RIGHT, false);
    m.addSpriteInfo(307, man4, Sprite_Position::FAR_LEFT, false);
    m.addSpriteInfo(307, man5, Sprite_Position::NEAR_LEFT, false);

    m.addSpriteInfo(308, woman3, Sprite_Position::FAR_RIGHT, false);
    m.addSpriteInfo(308, woman4, Sprite_Position::NEAR_RIGHT, false);

	m.addSpriteInfo(309, palm1Start, Sprite_Position::FAR_LEFT, false);
    m.addSpriteInfo(309, palm2Start, Sprite_Position::FAR_RIGHT, false);
    m.addSpriteInfo(308, man6, Sprite_Position::NEAR_LEFT, false);
    m.addSpriteInfo(308, cameramanRight, Sprite_Position::NEAR_RIGHT, false);

	m.addSpriteInfo(310, startFlag, Sprite_Position::FAR_LEFT, false);
    m.addSpriteInfo(311, trafficLightLeftPanel, Sprite_Position::NEAR_LEFT, false);
    m.addSpriteInfo(311, trafficLightRightPanel, Sprite_Position::FAR_RIGHT, false);
}


void Logger::loadGoalBiomeSprites(Biome& m){

    float offsetPublic = 1.31f;
    int j = 250;

    for (int i = 250; i <= 590; i++){
        if (i >= 270){
            if (i % 52 == 0){
                SpriteInfo* cameramanLeft = new SpriteInfo(&m.objects[24], m.pivotLeftPoints[24], m.pivotRightPoints[24],
                                           m.scaleCoeffs[24], m.widthCollisionCoeffs[24], m.showTerrainCoeffs[24],
                                           m.pivotLeftColPoints[24], m.pivotRightColPoints[24], m.collisions[24],
                                           -0.5f, 0.f, false);

                SpriteInfo* cameramanRight = new SpriteInfo(&m.objects[25], m.pivotLeftPoints[25], m.pivotRightPoints[25],
                                                   m.scaleCoeffs[25], m.widthCollisionCoeffs[25], m.showTerrainCoeffs[25],
                                                   m.pivotLeftColPoints[25], m.pivotRightColPoints[25], m.collisions[25],
                                                   0.5f, 0.f, false);

                m.addSpriteInfo(i, cameramanLeft, Sprite_Position::NEAR_LEFT, false);
                m.addSpriteInfo(i, cameramanRight, Sprite_Position::NEAR_RIGHT, false);
            }
            else if (i % 60 == 0){
                SpriteInfo* man = new SpriteInfo(&m.objects[14], m.pivotLeftPoints[14], m.pivotRightPoints[14],
                                                   m.scaleCoeffs[14], m.widthCollisionCoeffs[14], m.showTerrainCoeffs[14],
                                                   m.pivotLeftColPoints[14], m.pivotRightColPoints[14], m.collisions[14],
                                                   random_float(-0.61f, -0.77f), 0.f, false);

                m.addSpriteInfo(i, man, Sprite_Position::NEAR_LEFT, false);
            }
            else if (i % 55 == 0){
                SpriteInfo* woman = new SpriteInfo(&m.objects[30], m.pivotLeftPoints[30], m.pivotRightPoints[30],
                                                   m.scaleCoeffs[30], m.widthCollisionCoeffs[30], m.showTerrainCoeffs[30],
                                                   m.pivotLeftColPoints[30], m.pivotRightColPoints[30], m.collisions[30],
                                                   random_float(-0.35f, -0.84f), 0.f, false);

                SpriteInfo* man = new SpriteInfo(&m.objects[17], m.pivotLeftPoints[17], m.pivotRightPoints[17],
                                                   m.scaleCoeffs[17], m.widthCollisionCoeffs[17], m.showTerrainCoeffs[17],
                                                   m.pivotLeftColPoints[17], m.pivotRightColPoints[17], m.collisions[17],
                                                   random_float(0.35f, 0.84f), 0.f, false);

                m.addSpriteInfo(i, woman, Sprite_Position::NEAR_LEFT, false);
                m.addSpriteInfo(i, man, Sprite_Position::NEAR_RIGHT, false);
            }
            else if (i % 63 == 0){
                SpriteInfo* woman = new SpriteInfo(&m.objects[29], m.pivotLeftPoints[29], m.pivotRightPoints[29],
                                                   m.scaleCoeffs[29], m.widthCollisionCoeffs[29], m.showTerrainCoeffs[29],
                                                   m.pivotLeftColPoints[29], m.pivotRightColPoints[29], m.collisions[29],
                                                   random_float(0.4f, 0.8f), 0.f, false);

                m.addSpriteInfo(i, woman, Sprite_Position::NEAR_RIGHT, false);
            }
            else if (i % 65 == 0){
                SpriteInfo* woman = new SpriteInfo(&m.objects[26], m.pivotLeftPoints[26], m.pivotRightPoints[26],
                                                   m.scaleCoeffs[26], m.widthCollisionCoeffs[26], m.showTerrainCoeffs[26],
                                                   m.pivotLeftColPoints[26], m.pivotRightColPoints[26], m.collisions[26],
                                                   random_float(-0.4f, -0.7f), 0.f, false);

                m.addSpriteInfo(i, woman, Sprite_Position::NEAR_LEFT, false);
            }
            else if (i % 74 == 0){
                SpriteInfo* woman = new SpriteInfo(&m.objects[27], m.pivotLeftPoints[27], m.pivotRightPoints[27],
                                                   m.scaleCoeffs[27], m.widthCollisionCoeffs[27], m.showTerrainCoeffs[27],
                                                   m.pivotLeftColPoints[27], m.pivotRightColPoints[27], m.collisions[27],
                                                   random_float(0.41f, 0.77f), 0.f, false);

                m.addSpriteInfo(i, woman, Sprite_Position::NEAR_RIGHT, false);
            }
            else if (i % 95 == 0){
                SpriteInfo* man = new SpriteInfo(&m.objects[18], m.pivotLeftPoints[18], m.pivotRightPoints[18],
                                                   m.scaleCoeffs[18], m.widthCollisionCoeffs[18], m.showTerrainCoeffs[18],
                                                   m.pivotLeftColPoints[18], m.pivotRightColPoints[18], m.collisions[18],
                                                   random_float(0.39f, 0.85f), 0.f, false);

                m.addSpriteInfo(i, man, Sprite_Position::NEAR_LEFT, false);
            }
            else if (i % 70 == 0){
                SpriteInfo* man = new SpriteInfo(&m.objects[23], m.pivotLeftPoints[23], m.pivotRightPoints[23],
                                                   m.scaleCoeffs[23], m.widthCollisionCoeffs[23], m.showTerrainCoeffs[23],
                                                   m.pivotLeftColPoints[23], m.pivotRightColPoints[23], m.collisions[23],
                                                   random_float(-0.61f, -0.77f), 0.f, false);

                m.addSpriteInfo(i, man, Sprite_Position::NEAR_LEFT, false);
            }
            else if (i % 80 == 0){
                SpriteInfo* man = new SpriteInfo(&m.objects[19], m.pivotLeftPoints[19], m.pivotRightPoints[19],
                                                   m.scaleCoeffs[19], m.widthCollisionCoeffs[19], m.showTerrainCoeffs[19],
                                                   m.pivotLeftColPoints[19], m.pivotRightColPoints[19], m.collisions[19],
                                                   -0.66, 0.f, false);

                m.addSpriteInfo(i, man, Sprite_Position::NEAR_LEFT, false);
            }
            else if (i % 90 == 0){
                SpriteInfo* man1 = new SpriteInfo(&m.objects[20], m.pivotLeftPoints[20], m.pivotRightPoints[20],
                                                   m.scaleCoeffs[20], m.widthCollisionCoeffs[20], m.showTerrainCoeffs[20],
                                                   m.pivotLeftColPoints[20], m.pivotRightColPoints[20], m.collisions[20],
                                                   random_float(-0.57f, -0.65f), 0.f, false);

                SpriteInfo* man2 = new SpriteInfo(&m.objects[21], m.pivotLeftPoints[21], m.pivotRightPoints[21],
                                                   m.scaleCoeffs[21], m.widthCollisionCoeffs[21],m.showTerrainCoeffs[21],
                                                   m.pivotLeftColPoints[21], m.pivotRightColPoints[21], m.collisions[21],
                                                   random_float(0.63f, 0.78f), 0.f, false);

                m.addSpriteInfo(i, man1, Sprite_Position::NEAR_LEFT, false);
                m.addSpriteInfo(i, man2, Sprite_Position::NEAR_RIGHT, false);
            }
        }

        SpriteInfo* publicLeft = new SpriteInfo(&m.objects[33], m.pivotLeftPoints[33], m.pivotRightPoints[33],
                                           m.scaleCoeffs[33], m.widthCollisionCoeffs[33], m.showTerrainCoeffs[33],
                                           m.pivotLeftColPoints[33], m.pivotRightColPoints[33], m.collisions[33],
                                           -offsetPublic, 0.f, false);

        SpriteInfo* publicRight = new SpriteInfo(&m.objects[34], m.pivotLeftPoints[34], m.pivotRightPoints[34],
                                           m.scaleCoeffs[34], m.widthCollisionCoeffs[34], m.showTerrainCoeffs[34],
                                           m.pivotLeftColPoints[34], m.pivotRightColPoints[34], m.collisions[34],
                                           offsetPublic, 0.f, false);

        m.addSpriteInfo(j, publicLeft, Sprite_Position::FAR_LEFT, false);
        m.addSpriteInfo(j, publicRight, Sprite_Position::FAR_RIGHT, false);

        if (j < 590)
            j += 3;
    }

    for (int i = 594; i <= 607; i++){
        SpriteInfo* publicLeft = new SpriteInfo(&m.objects[33], m.pivotLeftPoints[33], m.pivotRightPoints[33],
                                           m.scaleCoeffs[33], m.widthCollisionCoeffs[33], m.showTerrainCoeffs[33],
                                           m.pivotLeftColPoints[33], m.pivotRightColPoints[33], m.collisions[33],
                                           -offsetPublic, 0.f, false);

        SpriteInfo* publicRight = new SpriteInfo(&m.objects[34], m.pivotLeftPoints[34], m.pivotRightPoints[34],
                                           m.scaleCoeffs[34], m.widthCollisionCoeffs[34], m.showTerrainCoeffs[34],
                                           m.pivotLeftColPoints[34], m.pivotRightColPoints[34], m.collisions[34],
                                           offsetPublic, 0.f, false);

        m.addSpriteInfo(i, publicLeft, Sprite_Position::FAR_LEFT, false);
        m.addSpriteInfo(i, publicRight, Sprite_Position::FAR_RIGHT, false);
        offsetPublic -= 0.1f;
    }

    setLoggerStatus();

    SpriteInfo* flagger = new SpriteInfo(&m.objects[0], m.pivotLeftPoints[0], m.pivotRightPoints[0], m.scaleCoeffs[0],
                                         m.widthCollisionCoeffs[0], m.showTerrainCoeffs[0], m.pivotLeftColPoints[0],
                                         m.pivotRightColPoints[0], m.collisions[0], -0.32f, 0.f, false);

    SpriteInfo* logRight = new SpriteInfo(&m.objects[44], m.pivotLeftPoints[44], m.pivotRightPoints[44],
                                           m.scaleCoeffs[44], m.widthCollisionCoeffs[44], m.showTerrainCoeffs[44],
                                           m.pivotLeftColPoints[44], m.pivotRightColPoints[44], m.collisions[44],
                                           1.2f, 0.f, false);

    SpriteInfo* logLeft = new SpriteInfo(&m.objects[44], m.pivotLeftPoints[44], m.pivotRightPoints[44],
                                         m.scaleCoeffs[44], m.widthCollisionCoeffs[44], m.showTerrainCoeffs[44],
                                         m.pivotLeftColPoints[44], m.pivotRightColPoints[44], m.collisions[44],
                                         -1.2f, 0.f, false);

    SpriteInfo* endFlag = new SpriteInfo(&m.objects[43], m.pivotLeftPoints[43], m.pivotRightPoints[43],
                                         m.scaleCoeffs[43], m.widthCollisionCoeffs[43],m.showTerrainCoeffs[43],
                                         m.pivotLeftColPoints[43], m.pivotRightColPoints[43], m.collisions[43],
                                         -1.35f, -1.6f, false);

    m.addSpriteInfo(504, flagger, Sprite_Position::NEAR_LEFT, false);

    m.addSpriteInfo(505, logLeft, Sprite_Position::NEAR_LEFT, false);
    m.addSpriteInfo(505, logRight, Sprite_Position::NEAR_RIGHT, false);
    m.addSpriteInfo(506, endFlag, Sprite_Position::NEAR_LEFT, false);

    m.addSpriteInfo(608, logLeft, Sprite_Position::NEAR_LEFT, false);
    m.addSpriteInfo(608, logRight, Sprite_Position::NEAR_RIGHT, false);
    m.addSpriteInfo(609, endFlag, Sprite_Position::NEAR_LEFT, false);

}

void Logger::updateSprite(Biome& m, const Sprite_Animated spriteAnimated){
    bool startMap = m.getStartBiome();
    bool goalMap = m.getGoalBiome();
    if (startMap || goalMap){
        if (spriteAnimated == Sprite_Animated::FLAGGER){
            switch (instance.status){
                case Flagger_Status::UPPING_FLAG:
                    instance.flagger_code_image++;
                    if (instance.flagger_code_image == instance.FLAGGER_FLAG)
                        instance.status = Flagger_Status::MOVING_FLAG;
                    break;
                case Flagger_Status::MOVING_FLAG:
                    if (instance.flagger_code_image < instance.FLAGGER_HAND){
                        if (instance.numIterations != instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.numIterations = 0;
                            instance.flagger_code_image++;
                        }
                    }
                    else {
                        if (instance.numIterations != instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.attemps++;
                            if (instance.attemps == instance.FLAGGER_CHANGE_STATUS){
                                instance.status = Flagger_Status::MOVING_HAND;
                                instance.flagger_code_image = instance.FLAGGER_HAND;
                                instance.attemps = 0;
                            }
                            else
                                instance.flagger_code_image = instance.FLAGGER_FLAG + 1;

                            instance.numIterations = 0;
                        }
                    }
                    break;
                case Flagger_Status::MOVING_HAND:
                    if (instance.flagger_code_image < instance.FLAGGER_BACK){
                        if (instance.numIterations != 2 * instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.numIterations = 0;
                            instance.flagger_code_image++;
                        }
                    }
                    else {
                        if (instance.numIterations != 2 * instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.attemps++;
                            if (instance.attemps == instance.FLAGGER_CHANGE_STATUS){
                                instance.totalTimes++;
                                if (instance.totalTimes == 3){
                                    instance.status = Flagger_Status::MOVING_BACK;
                                    instance.flagger_code_image = instance.FLAGGER_BACK + 1;
                                }
                                else {
                                    instance.status = Flagger_Status::MOVING_FLAG;
                                    instance.flagger_code_image = instance.FLAGGER_FLAG + 1;
                                    instance.attemps = 0;
                                }
                            }
                            else
                                instance.flagger_code_image = instance.FLAGGER_HAND + 1;

                            instance.numIterations = 0;
                        }
                    }
                    break;
                case Flagger_Status::MOVING_BACK:
                   if (instance.flagger_code_image < instance.FLAGGER_END_ANIMATION){
                        if (instance.numIterations != 4 * instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.numIterations = 0;
                            instance.flagger_code_image++;
                        }
                    }
                    else
                        instance.endFlaggerAnimation = true;
            }

            SpriteInfo* newSprite = new SpriteInfo(&m.objects[instance.flagger_code_image],
                                                   m.pivotLeftPoints[instance.flagger_code_image],
                                                   m.pivotRightPoints[instance.flagger_code_image],
                                                   m.scaleCoeffs[instance.flagger_code_image],
                                                   m.widthCollisionCoeffs[instance.flagger_code_image],
                                                   m.showTerrainCoeffs[instance.flagger_code_image],
                                                   m.pivotLeftColPoints[instance.flagger_code_image],
                                                   m.pivotRightColPoints[instance.flagger_code_image],
                                                   m.collisions[instance.flagger_code_image], -0.32f, 0.f, false);

            if (startMap)
                m.addSpriteInfo(306, newSprite, Sprite_Position::NEAR_LEFT, false);
            else
                m.addSpriteInfo(504, newSprite, Sprite_Position::NEAR_LEFT, false);
        }
        else {
            if (instance.semaphore_code_image <= instance.MAX_SEMAPHORE_CODE_IMAGE){
                SpriteInfo* newSprite = new SpriteInfo(&m.objects[instance.semaphore_code_image],
                                                       m.pivotLeftPoints[instance.semaphore_code_image],
                                                       m.pivotRightPoints[instance.semaphore_code_image],
                                                       m.scaleCoeffs[instance.semaphore_code_image],
                                                       m.widthCollisionCoeffs[instance.semaphore_code_image],
                                                       m.showTerrainCoeffs[instance.semaphore_code_image],
                                                       m.pivotLeftColPoints[instance.semaphore_code_image],
                                                       m.pivotRightColPoints[instance.semaphore_code_image],
                                                       m.collisions[instance.flagger_code_image], -0.8f, 0.f, false);

                m.addSpriteInfo(311, newSprite, Sprite_Position::NEAR_LEFT, false);
                instance.semaphore_code_image++;
            }
        }
    }
}

bool Logger::getEndFlaggerAnimation(){
    return instance.endFlaggerAnimation;
}

void Logger::close(){
    setLoggerStatus();
    instance.inputFlux.close();
    instance.outputFlux.close();
}



