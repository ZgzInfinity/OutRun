
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


bool Logger::checkMapFile(const std::string& pathMapFile){
    instance.inputFlux.open(pathMapFile);
    if (instance.inputFlux.is_open())
        instance.outputFlux << "MAP 1" << std::endl;
    else {
        instance.outputFlux << "FILE MAP: " << pathMapFile << " DOES NOT EXITS." << std::endl;
        instance.failDetected = true;
    }
    return instance.failDetected;
}


bool Logger::checkTimeAndTerrain(Map& m){
    std::string informationRead;
    instance.inputFlux >> informationRead;
    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN TIME: NOT FOUND" << std::endl;

        return !instance.failDetected;
    }
    else {
        if (informationRead != "TIME:"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN TIME: BUT FOUND " << informationRead << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            instance.inputFlux >> informationRead;
            if (instance.inputFlux.eof()){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". TIME VALUE NOT FOUND" << std::endl;

                return !instance.failDetected;
            }
            else {
                if (std::regex_match(informationRead, natural_number_regex)){
                    m.setTime(std::stoi(informationRead));
                }
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". TIME VALUE " << informationRead << " IS NOT A POSITIVE INTEGER" << std::endl;

                    return !instance.failDetected;
                }
            }
        }
    }
    instance.row++;
    instance.column = 0;
    instance.inputFlux >> informationRead;
    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN TERRAIN: NOT FOUND" << std::endl;

        return !instance.failDetected;
    }
    else {
        if (informationRead != "TERRAIN:"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN TERRAIN: BUT FOUND " << informationRead << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            instance.inputFlux >> informationRead;
            if (instance.inputFlux.eof()){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". TERRAIN VALUE NOT FOUND" << std::endl;

                return !instance.failDetected;
            }
            else {
                if (std::regex_match(informationRead, natural_number_regex)){
                    m.setTerrain(std::stoi(informationRead));
                }
                else {
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". TERRAIN VALUE " << informationRead << " IS NOT A POSITIVE INTEGER" << std::endl;

                    return !instance.failDetected;
                }
            }
        }
    }
    instance.row++;
    instance.column = 0;
    return instance.failDetected;
}

bool Logger::checkColors(Map& m){
    std::string informationRead;
    instance.inputFlux >> informationRead;
    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN COLOR_BACKGROUND: NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        if (informationRead != "COLOR_BACKGROUND:"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN COLOR_BACKGROUND: BUT FOUND " << informationRead << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            sf::Color sky;
            readColor(sky);
        }
    }
}


bool Logger::readColor(sf::Color& colorRead){
    int red, green, blue, alpha, colorChannelsRead = 0;
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
            if (std::regex_match(informationRead, natural_number_regex)){
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
                                        << " IS OUT OF RANGE. MUST BE BETWEEN 0 AND 255" << std::endl;

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
                    instance.column << ". " << channelFailed << " IS NOT A POSITIVE INTEGER " << std::endl;

                return !instance.failDetected;
            }
        }
    }
    return instance.failDetected;
}
