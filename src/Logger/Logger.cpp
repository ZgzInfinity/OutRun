
/*
 * Copyright (c) 20222 Ruben Rodriguez
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
 * Implementation file of the module Logger
 */


#include "Logger.h"

// Declaration of the static instance
Logger Logger::instance;



/**
 * Default constructor
 */
Logger::Logger(){
    instance.firstTimeInvoked = true;
    setLoggerStatus();
}



/**
 * Set the logger status default configuration
 */
void Logger::setLoggerStatus(){
    // Set all the default status of the logger service
    instance.failDetected = false;
    instance.endFlaggerAnimation = false;
    instance.flagger_code_image = 0;
    instance.semaphore_code_image = 38;
    instance.attempts = 0;
    instance.numIterations = 0;
    instance.totalTimes = 0;
    instance.status = Flagger_Status::UPPING_FLAG;
    instance.linesOfBiome = 0;
}




/**
 * Set if the logger has detected any error
 * @param _failDetected is the possible error detected
 */
void Logger::setFailDetected(const bool _failDetected){
    instance.failDetected = _failDetected;
}



/**
 * Set the with of the screen
 * @param _widthScreen is the width screen
 */
void Logger::setWidthScreen(const int _widthScreen){
    instance.widthScreen = _widthScreen;
}



/**
 * Set the position in the axis y of the sprites (floatin sprites) in the biome
 * @param m is the biome whose sprites are going to be modified
 */
void Logger::setSpriteScreenY(Biome& m){
    // Check if the it is the starting biome
    if (m.startBiome)
        // Start flag
        m.setSpriteScreenY(310, float(instance.widthScreen * -1.6f / SCREEN_0.first));
    // Check if the it is the starting biome
    else if (m.goalBiome){
        // Goal flag
        m.setSpriteScreenY(706, float(instance.widthScreen * -1.6f / SCREEN_0.first));
        m.setSpriteScreenY(806, float(instance.widthScreen * -1.6f / SCREEN_0.first));
    }
    else {
        for (int i = 0; i < m.floatingSprites.size(); i++)
            // Floating sprites (tunnels)
            m.setSpriteScreenY(m.floatingSprites.at(i), float(instance.widthScreen * -1.45f / SCREEN_0.first));
    }
}



/**
 * Get if the logger has found any error
 */
bool Logger::getFailDetected(){
    return instance.failDetected;
}



/**
 * Get if the flagger has finished its animation
 */
bool Logger::getEndFlaggerAnimation(){
    return instance.endFlaggerAnimation;
}



/**
 * Returns true if a biome has a default configuration
 * file. Otherwise returns false
 * @param pathMapFile is the path of the biome's file to be checked
 */
bool Logger::checkMapFile(const std::string pathMapFile){
    // Check if it is the first time of the logger
    if (instance.firstTimeInvoked){
        // Open the file to write the log information in truncate mode
        instance.firstTimeInvoked = false;
        instance.outputFlux.open(instance.LOGGER_PATH_FILE, std::fstream::trunc);
    }
    else
        // Open the file to write the log information in append mode (at the end of the file)
        instance.outputFlux.open(instance.LOGGER_PATH_FILE, std::ios_base::app);

    // Fail detected if the writing file opening has finished in error
    if (!instance.outputFlux.is_open())
        instance.failDetected = true;

    // Open the file
    instance.inputFlux.open(pathMapFile);

    // Check if the file is opened
    if (instance.inputFlux.is_open()){
        // Success
        instance.outputFlux << "FILE MAP: " << pathMapFile << " FOUND." << std::endl;
        instance.column = 1;
        instance.row = 1;
    }
    else {
        // Error
        instance.outputFlux << "FILE MAP: " << pathMapFile << " DOES NOT EXIST." << std::endl;
        instance.failDetected = true;
    }
    return instance.failDetected;
}



/**
 * Returns true if there is a default configuration for the time
 * the type of road of the biome and stores it. Otherwise returns false
 * @param m is the biome where the values of time and kind of terrain
 * are going to be stored
 */
bool Logger::checkTimeTerrainRoad(Biome& m){
    std::string informationRead;

    // Read new information from the file
    instance.inputFlux >> informationRead;

    // Error because there is not global configuration
    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN GLOBAL_CONF NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        // Syntax error
        if (informationRead != "GLOBAL_CONF"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN GLOBAL_CONF BUT FOUND " << informationRead << "." << std::endl;

            return !instance.failDetected;
        }
        else {

            // Read the time configuration value
            instance.row++;
            instance.column = 1;

            instance.inputFlux >> informationRead;

            // Not found time value
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
                    // Not token time found
                    instance.column += 2;
                    instance.inputFlux >> informationRead;
                    if (instance.inputFlux.eof()){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". TIME VALUE NOT FOUND." << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        // Get the time value
                        if (std::regex_match(informationRead, instance.natural_number_regex)){
                            m.setTime(std::stoi(informationRead));
                        }
                        else {
                            // Token time value must be integer
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". TIME VALUE " << informationRead << " IS NOT A POSITIVE INTEGER." << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }

                instance.column = 1;
                instance.row++;

                // Read the token terrain
                instance.inputFlux >> informationRead;
                if (instance.inputFlux.eof()){
                    // Token terrain not found (finished file)
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". IDENTIFIER TOKEN TERRAIN: NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    // Token terrain not found
                    if (informationRead != "TERRAIN:"){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". EXPECTED IDENTIFIER TOKEN TERRAIN: BUT FOUND " << informationRead << "." << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        // Token terrain value not found
                        instance.column += 2;
                        instance.inputFlux >> informationRead;
                        if (instance.inputFlux.eof()){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". TERRAIN VALUE NOT FOUND." << std::endl;

                            return !instance.failDetected;
                        }
                        else {
                            // Terrain like time must be a natural integer
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

                        // Read the road terrain token
                        instance.inputFlux >> informationRead;

                        // Not found the token
                        if (instance.inputFlux.eof()){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". IDENTIFIER ROAD_TERRAIN: NOT FOUND." << std::endl;

                            return !instance.failDetected;
                        }
                        else {
                            // Error syntax token
                            if (informationRead != "ROAD_TERRAIN:"){
                                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                    instance.column << ". EXPECTED IDENTIFIER TOKEN ROAD_TERRAIN: BUT FOUND " << informationRead << "." << std::endl;

                                return !instance.failDetected;
                            }
                            else {
                                // Read the road terrain token value
                                instance.column += 2;
                                instance.inputFlux >> informationRead;
                                if (instance.inputFlux.eof()){
                                    // Not found
                                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                        instance.column << ". ROAD_TERRAIN VALUE NOT FOUND." << std::endl;

                                    return !instance.failDetected;
                                }
                                else {
                                    // The road terrain value must be also a natural integer number
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
    // The global configuration is correct
    instance.outputFlux << "GLOBAL_CONF PART CORRECTLY SPECIFIED." << std::endl;
    return instance.failDetected;
}



/**
 * Returns true if the color has been read and stored.
 * Otherwise returns false
 * @param colorRead is where the color is read is going to be stored
 */
bool Logger::readColor(sf::Color& colorRead){
    int red = 0, green = 0, blue = 0, alpha = 0, colorChannelsRead = 0;
    string informationRead, channelFailed;

    // Loop that read the three channels of the color RGB
    while (!instance.failDetected && colorChannelsRead < 4){
        // Read the channel
        instance.inputFlux >> informationRead;
        if (instance.inputFlux.eof()){
            // Bot found and determine which is the error
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
            // Read the color value of the channel
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
                        // Create the color to be stored
                        colorRead = sf::Color(red, green, blue, alpha);
                }
                else {
                    // Check the range of the color value
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
                // Color value channel must be a natural integer value
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
    // Return if there has been any kind of error
    return instance.failDetected;
}



/**
 * Returns true if the colors of the biome have been read and
 * stored. Otherwise returns false
 * @param m is the biome whose colors are going to be read and stored
 */
bool Logger::checkColors(Biome& m){
    std::string informationRead, colorToBeRead;
    std::vector<sf::Color> colorsOfBiome;
    sf::Color colorRead;

    instance.row++;
    instance.column = 1;

    // Read the colors of the biome
    instance.inputFlux >> informationRead;

    // Read the token colors indicator
    if (instance.inputFlux.eof()){
        // Not found
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN COLORS NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        // Syntax error
        if (informationRead != "COLORS"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN COLORS BUT FOUND " << informationRead << "." << std::endl;

            return !instance.failDetected;
        }
        else {
            // Read the different colors
            instance.row++;
            instance.column = 1;

            // Main colors of the scenario
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

                // Try to read the token color
                instance.inputFlux >> informationRead;
                if (instance.inputFlux.eof()){
                    // Not found
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". IDENTIFIER TOKEN " << colorToBeRead << " NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    // Syntax error of the token identifier
                    if (informationRead != colorToBeRead){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". EXPECTED IDENTIFIER TOKEN " << colorToBeRead << " BUT FOUND " << informationRead << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        // Read the color and stores it
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
            // Colors of the scenario are correct
            instance.row++;
            instance.column = 1;
            m.setColors(colorsOfBiome);
            instance.outputFlux << "COLORS PART CORRECTLY SPECIFIED." << std::endl;
            return instance.failDetected;
        }
    }
}



/**
 * Returns true if the a straight relief of the biome has been
 * read correctly. Otherwise returns false
 * @param m is the biome where the straight relief is going to be stored
 */
bool Logger::checkReliefStraight(Biome& m){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 5;

    bool mirror;
    int enter, hold, leave, numTracks, distM;

    // Read a straight relief part of the scenario
    while (!instance.failDetected && parametersRead < totalParametersToRead){

        // Read the new information and count the number of parameters read
        instance.inputFlux >> informationRead;
        parametersRead++;

        // Check final of file
        if (instance.inputFlux.eof()){

            // There are not enough parameters
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

            // Error
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". " << parameterFailed << " PARAMETER NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            // Parameter new read
            instance.column += 2;
            if (parametersRead == 4){
                // Check the range of the mirror value type
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
            // Read the rest of the parameters (all are natural integer numbers
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
                        // Check the number of tracks (from two to eight)
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". NUM TRACKS VALUE " << numTracks <<
                            " IS OUT F RANGE. MUST BE BETWEEN 2 AND 8." << std::endl;

                        return !instance.failDetected;
                    }
                }
            }
            else {
                // Any of the parameters read is not a natural integer number and notifies the error
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
    // Compute the number of road tracks and add the new part to the biome
    numTracks = m.computeRoadTracks(numTracks);
    m.addBiome(enter, hold, leave, 0, 0, mirror, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}



/**
 * Returns true if the a curve with a direction of the biome has been
 * read correctly. Otherwise returns false
 * @param m is the biome where the relief is going to be stored
 * @param leftDirection is the direction of the curve (true is left curve
 * and false is right curve
 */
bool Logger::checkReliefCurve(Biome& m, const bool leftDirection){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 7;

    bool mirror;
    int enter, hold, leave, numTracks, factor_length;
    float direction;

    // Read a curve relief part of the biome to be added
    while (!instance.failDetected && parametersRead < totalParametersToRead){

        // Read the new data and count the parameters to be read
        instance.inputFlux >> informationRead;
        parametersRead++;

        // Any parameter has been forgotten
        if (instance.inputFlux.eof()){

            std::string parameterFailed;

            // Write which parameter is not specified in the file
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
            // Read the values of the parameters an check the types
            instance.column += 2;
            if (parametersRead == 4){
                if (std::regex_match(informationRead, instance.float_number_regex)){
                    // Check the type of the curve direction coefficient (real number)
                    direction = std::stof(informationRead);

                    // Left curve (negative value)
                    if (leftDirection && direction >= 0.0f){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". LEFT CURVE DIRECTION " << direction << " MUST BE LOWER THAN ZERO. " << std::endl;

                        return !instance.failDetected;
                    }
                    // Right curve (positive value)
                    else if (!leftDirection && direction <= 0.0f){
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                            instance.column << ". RIGHT CURVE DIRECTION " << direction << " MUST BE HIGHER THAN ZERO. " << std::endl;

                        return !instance.failDetected;
                    }
                }
                else {
                    // Error type value
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". DIRECTION CURVE PARAMETER " << informationRead << " MUST BE FLOAT." << std::endl;

                    return !instance.failDetected;
                }
            }
            else if (parametersRead == 5){
                if (informationRead != "0" && informationRead != "1"){
                    // Check the mirror type value
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
                // Checkt the types of the rest of parameters (not curve and mirror values)
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
                        // Check the curve length (must be a positive number
                        factor_length = std::stoi(informationRead);
                        if (factor_length <= 0){
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". FACTOR LENGTH CANNOT BE NEGATIVE OR ZERO. " << std::endl;

                            return !instance.failDetected;
                        }
                    }
                }
                else {
                    // Any of the parameters has a wrong type and the error must be written in the log file
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
    // Compute the number of tracks and add the curve relief part to the scenario
    numTracks = m.computeRoadTracks(numTracks);
    m.addBiome(enter, hold * factor_length, leave, direction, 0, mirror, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}



/**
 * Returns true if the a hill of the relief of the biome has been
 * read correctly. Otherwise returns false
 * @param m is the biome where the relief is going to be stored
 */
bool Logger::checkReliefHillStraight(Biome& m){

    std::string informationRead;
    int parametersRead = 0, totalParametersToRead = 6;
    int enter, hold, leave, slope, numTracks, factor_length;

    // Read a hill straight to be added to the scenario
    while (!instance.failDetected && parametersRead < totalParametersToRead){

        // Read the data and count to the parameters
        instance.inputFlux >> informationRead;
        parametersRead++;

        // Any parameter is missed (specified which one)
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

            // Write the error
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". " << parameterFailed << " PARAMETER NOT FOUND." << std::endl;

            return !instance.failDetected;
        }
        else {
            instance.column += 2;
            if (parametersRead == 4){
                // Check the type of the slope coefficient
                if (std::regex_match(informationRead, instance.integer_number_regex))
                    // Positive value (up slope) and negative value (down slope)
                    slope = std::stoi(informationRead);
                else {
                    // Error
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". SLOPE PARAMETER " << informationRead << " MUST BE INTEGER." << std::endl;

                    return !instance.failDetected;
                }
            }
            else {
                // Check the type values of the rest parameters read and notify the possible error
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
                    // Notify the type error of the parameters
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
    // Compute the number of road tracks and add the straight hill to the scenario
    numTracks = m.computeRoadTracks(numTracks);
    m.addBiome(enter, hold * factor_length, leave, 0, slope, false, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}



/**
 * Returns true if the a curve hill with a direction of the biome has been
 * read correctly. Otherwise returns false
 * @param m is the biome where the relief is going to be stored
 * @param leftDirection is the direction of the curve hill (true is left curve
 * and false is right curve
 */
bool Logger::checkReliefHillCurve(Biome& m, const bool leftDirection){

    int parametersRead = 0, totalParametersToRead = 7;

    std::string informationRead;
    bool mirror;
    int enter, hold, leave, slope, numTracks;
    float direction;

    // Read the hill curve relief part and add it to the scenario
    while (!instance.failDetected && parametersRead < totalParametersToRead){

        // Read the data and count the parameters read
        instance.inputFlux >> informationRead;
        parametersRead++;

        // Missed parameters
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
            // Read the parameter coefficient value
            instance.column += 2;
            if (parametersRead == 4){
                if (std::regex_match(informationRead, instance.integer_number_regex))
                    // Slope value
                    slope = std::stoi(informationRead);
                else {
                    // Error type
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". SLOPE PARAMETER " << informationRead << " MUST BE INTEGER." << std::endl;

                    return !instance.failDetected;
                }
            }
            else if (parametersRead == 5){
                // Direction coefficient value
                if (std::regex_match(informationRead, instance.float_number_regex)){

                    // Check signed value to assign the type of curve
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
                    // Error of type coefficient (real number)
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". DIRECTION CURVE PARAMETER " << informationRead << " MUST BE FLOAT." << std::endl;

                    return !instance.failDetected;
                }
            }
            else if (parametersRead == 6){
                if (informationRead != "0" && informationRead != "1"){
                    // Mirror value parameter type error
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
                // Check the rest of type parameters
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
                    // Error must be written in log file
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
    // Compute the tracks and add the curve hill to the scenario
    numTracks = m.computeRoadTracks(numTracks);
    m.addBiome(enter, hold, enter, direction, slope, mirror, numTracks, instance.linesOfBiome);
    return instance.failDetected;
}



/**
 * Returns true if the a relief of the biome has been
 * read correctly. Otherwise returns false
 * @param m is the biome where the relief is going to be stored
 */
bool Logger::checkBiomeRelief(Biome& m){

    std::string informationRead;
    bool reliefProccessed = false;

    // Read the relief of the biome
    instance.inputFlux >> informationRead;

    // Relief token not found and error notified
    if (instance.inputFlux.eof()){
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". IDENTIFIER TOKEN START_RELIEF NOT FOUND." << std::endl;

        return !instance.failDetected;
    }
    else {
        // Syntax error
        if (informationRead != "START_RELIEF"){
            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                instance.column << ". EXPECTED IDENTIFIER TOKEN START_RELIEF BUT FOUND " << informationRead << "." << std::endl;

            return !instance.failDetected;
        }
        else {
            // Read the relief until the sprite section is detected
            instance.inputFlux >> informationRead;

            // No errors and sprites section has not been read
            while (!instance.failDetected && informationRead != "SPRITES"){

                instance.row++;
                instance.column = 1;

                // Try to read the new relief part
                if (instance.inputFlux.eof()){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". CANDIDATE IDENTIFIER TOKEN FOR RELIEF NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    // Determine the new relief part
                    if (informationRead != "STRAIGHT:" && informationRead != "CURVE_LEFT:" && informationRead != "CURVE_RIGHT:" &&
                        informationRead != "HILL_STRAIGHT:" && informationRead != "HILL_LEFT:" && informationRead != "HILL_RIGHT:")
                    {
                        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". EXPECTED ONE OF THIS RELIEF IDENTIFIER TOKENS (STRAIGHT:, CURVE_LEFT:, CURVE_RIGHT:"
                                        << " HILL_STRAIGHT:, HILL_LEFT:, HILL_RIGHT:. BUT FOUND " << informationRead << std::endl;

                        return !instance.failDetected;
                    }
                    else {
                        // Straight relief
                        if (informationRead == "STRAIGHT:"){
                            instance.failDetected = checkReliefStraight(m);
                        }
                        // Curve relief (left or right)
                        else if (informationRead == "CURVE_LEFT:" || informationRead == "CURVE_RIGHT:"){
                            bool leftDirection = (informationRead == "CURVE_LEFT:") ? true : false;
                            instance.failDetected = checkReliefCurve(m, leftDirection);
                        }
                        // Straight hill
                        else if (informationRead == "HILL_STRAIGHT:")
                            instance.failDetected = checkReliefHillStraight(m);
                        else {
                            // curve hill (left or right
                            bool leftDirection = (informationRead == "HILL_LEFT:") ? true : false;
                            instance.failDetected = checkReliefHillCurve(m, leftDirection);
                        }
                        // Relief part processed
                        if (!reliefProccessed)
                            reliefProccessed = true;
                    }
                }
                instance.inputFlux >> informationRead;
            }
            instance.row++;
            instance.column = 1;

            // Not relief section specified (sprites section directly
            if (!reliefProccessed){
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". SPRITES IDENTIFIER TOKEN FOUND BUT RELIEF PART IS TOTALLY EMPTY." << std::endl;

                return !instance.failDetected;
            }
            else {
                // Correct relief part written
                instance.outputFlux << "RELIEF PART CORRECTLY SPECIFIED." << std::endl;
                instance.row++;
                instance.column = 1;
                return instance.failDetected;
            }
        }
    }
}



/**
 * Load the sprites of the starting biome
 * @param m is the starting biome
 */
void Logger::loadStartBiomeSprites(Biome& m){

    /*
     *Public sprites of the beginning
     */
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

    // Flagger man
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

    /*
     * Palms at both sides of the road
     */

    SpriteInfo* palm1 = new SpriteInfo(&m.objects[35], m.pivotLeftPoints[35], m.pivotRightPoints[35], m.scaleCoeffs[35],
                                       m.widthCollisionCoeffs[35], m.showTerrainCoeffs[35], m.pivotLeftColPoints[35],
                                       m.pivotRightColPoints[35], m.collisions[35], -0.8f, 0.f, false);

    SpriteInfo* palm2 = new SpriteInfo(&m.objects[36], m.pivotLeftPoints[36], m.pivotRightPoints[36], m.scaleCoeffs[36],
                                       m.widthCollisionCoeffs[36], m.showTerrainCoeffs[36], m.pivotLeftColPoints[36],
                                       m.pivotRightColPoints[36], m.collisions[36], 0.8f, 0.f, false);

    // Start flag
    SpriteInfo* startFlag = new SpriteInfo(&m.objects[37], m.pivotLeftPoints[37], m.pivotRightPoints[37], m.scaleCoeffs[37],
                                           m.widthCollisionCoeffs[37], m.showTerrainCoeffs[37], m.pivotLeftColPoints[37],
                                           m.pivotRightColPoints[37], m.collisions[37], -1.33f,
                                           float(instance.widthScreen * -1.6f / SCREEN_0.first), false);


    // Semaphore
    SpriteInfo* trafficLightLeftPanel = new SpriteInfo(&m.objects[38], m.pivotLeftPoints[38], m.pivotRightPoints[38],
                                                       m.scaleCoeffs[38], m.widthCollisionCoeffs[38], m.showTerrainCoeffs[38],
                                                       m.pivotLeftColPoints[38], m.pivotRightColPoints[38], m.collisions[38],
                                                       -0.8f, 0.f, false);

    SpriteInfo* trafficLightRightPanel = new SpriteInfo(&m.objects[42], m.pivotLeftPoints[42], m.pivotRightPoints[42],
                                           m.scaleCoeffs[42], m.widthCollisionCoeffs[42], m.showTerrainCoeffs[42],
                                           m.pivotLeftColPoints[42], m.pivotRightColPoints[42], m.collisions[42],
                                           0.8f, 0.f, false);

    /*
     * Palms that hold the start flag
     */

    SpriteInfo* palm1Start = new SpriteInfo(&m.objects[35], m.pivotLeftPoints[35], m.pivotRightPoints[35],
                                           m.scaleCoeffs[35], m.widthCollisionCoeffs[35], m.showTerrainCoeffs[35],
                                           m.pivotLeftColPoints[35], m.pivotRightColPoints[35], m.collisions[35],
                                           -0.6f, 0.f, false);

    SpriteInfo* palm2Start = new SpriteInfo(&m.objects[36], m.pivotLeftPoints[36], m.pivotRightPoints[36],
                                           m.scaleCoeffs[36], m.widthCollisionCoeffs[36], m.showTerrainCoeffs[36],
                                           m.pivotLeftColPoints[36], m.pivotRightColPoints[36], m.collisions[36],
                                           0.6f, 0.f, false);


    /*
     * Add the sprites to the biome in the correct positions
     * to build the starting animation
     */

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



/**
 * Load the sprites of the goal biome
 * @param m is the goal biome
 */
void Logger::loadGoalBiomeSprites(Biome& m){

    float offsetPublic = 1.31f;
    int j = 250;

    /*
     * Add the final sprites in the goal animation
     * Public people in both sides of the road
     */

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

    // Set the logger status to start the flagger animation again
    setLoggerStatus();

    // Flagger
    SpriteInfo* flagger = new SpriteInfo(&m.objects[0], m.pivotLeftPoints[0], m.pivotRightPoints[0], m.scaleCoeffs[0],
                                         m.widthCollisionCoeffs[0], m.showTerrainCoeffs[0], m.pivotLeftColPoints[0],
                                         m.pivotRightColPoints[0], m.collisions[0], -0.32f, 0.f, false);

    /*
     * First and second log goal flag
     */

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

    // Add the flagger

    m.addSpriteInfo(504, flagger, Sprite_Position::NEAR_LEFT, false);

    // First log goal flag
    m.addSpriteInfo(505, logLeft, Sprite_Position::NEAR_LEFT, false);
    m.addSpriteInfo(505, logRight, Sprite_Position::NEAR_RIGHT, false);
    m.addSpriteInfo(506, endFlag, Sprite_Position::NEAR_LEFT, false);

    // Second log goal flag
    m.addSpriteInfo(608, logLeft, Sprite_Position::NEAR_LEFT, false);
    m.addSpriteInfo(608, logRight, Sprite_Position::NEAR_RIGHT, false);
    m.addSpriteInfo(609, endFlag, Sprite_Position::NEAR_LEFT, false);

}



/**
 * Returns true if the load of a the sprite of the biome
 * has been done correctly. Otherwise returns false
 * @param m is the biome where a sprite are going to be loaded
 * @param startPos is the initial position where the sprite is going to start to appear
 * @param endPos is the final position where the sprite is going to start to appear
 * @param incrementor is how much lines has to been added to the current one to insert another sprite
 * @param frequency controls if the sprite is going to be inserted or not
 * @param indexSpecified controls if the index of the sprite has been read in the file
 * @param spritesProcessed controls if all the information of the sprite has been read in the file
 * @param pos is the position in the road where the sprite is going to be inserted (far left, near left, center, near right, far right)
 * @param isCheckPoint controls if the sprite to be inserted is a checkpoint or not
 */
bool Logger::checkLevelBiomeSprite(Biome& m, const int startPos, const int endPos, const int incrementor, const int frequency,
                         bool& indexSpecified, bool& spritesProcessed, const int pos, const bool isCheckPoint){

    std::string informationRead;
    int idProp;
    float offsetX, offsetY;
    bool side;
    instance.column += 2;

    // Check if the index of the sprite to be inserted has been read
    if (indexSpecified){
        int parametersToRead = 4;
        // Starts to read the parameters of the sprite
        for (int i = 0; i < parametersToRead; i++){
            instance.inputFlux >> informationRead;
            // Missed parameters
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
                // Checks index of the sprite
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
                    // Check the side of the sprite (left or right road)
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
                    // Check the positions of the sprite in axis X and Y
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
        // Sprites section is completely empty
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". SPRITES PART IS TOTALLY EMPTY." << std::endl;

        return !instance.failDetected;
    }

    // Create the sprite structure
    SpriteInfo* newSprite = new SpriteInfo(&m.objects[idProp - 1], m.pivotLeftPoints[idProp - 1],
                                           m.pivotRightPoints[idProp - 1], m.scaleCoeffs[idProp - 1],
                                           m.widthCollisionCoeffs[idProp - 1], m.showTerrainCoeffs[idProp - 1],
                                           m.pivotLeftColPoints[idProp - 1], m.pivotRightColPoints[idProp - 1],
                                           m.collisions[idProp - 1], offsetX, offsetY, side);

    // Determine its position
    Sprite_Position spritePos = (pos == 0) ? Sprite_Position::FAR_LEFT : (pos == 1) ? Sprite_Position::NEAR_LEFT :
                                (pos == 2) ? Sprite_Position::CENTER : (pos == 3) ? Sprite_Position::FAR_RIGHT : Sprite_Position::NEAR_RIGHT;

    // Sprites that do not appear in intervals
    if (endPos == -1){
        m.addSpriteInfo(startPos, newSprite, spritePos, isCheckPoint);
        if (offsetY != 0.f)
            // Store the sprite and mark it as floating sprite (air)
            m.floatingSprites.push_back(startPos);
    }
    else {
        // Sprites that appear in intervals
        for (int i = startPos; i < endPos; i += incrementor){
            if (i % frequency == 0){
                m.addSpriteInfo(i, newSprite, spritePos, false);

                // Store the sprite and mark it as
                if (offsetY != 0.f)
                    m.floatingSprites.push_back(i);
            }
        }
    }
    instance.row++;
    instance.column = 1;

    // Sprites section processed
    if (!spritesProcessed)
        spritesProcessed = true;

    return instance.failDetected;
}



/**
 * Returns true if the load of all the sprites of the biome
 * has been done correctly. Otherwise returns false
 * @param m is the biome where all the sprites are going to be loaded
 */
bool Logger::checkLevelBiomeSprites(Biome& m){

    /*
     * Each interval or line has five times of position sprites
     * far left, near left, center, near right and far right and
     * there is only one type of sprite position for each interval
     */

    std::string informationRead;

    // Control flags to read the sprite
    bool isCheckPoint = false;
    bool spritesProcessed = false;
    bool indexSpecified = false, spriteFarLeft = false, spriteFarRight = false,
         spriteNearLeft = false, spriteNearRight = false, newInterval = false,
         spriteCenter = false;

    int startPos = 0, endPos = -1, incrementor = 0, frequency = 0;

    // Read new data
    instance.inputFlux >> informationRead;

    //Read until the end of file token is found
    while (!instance.failDetected && informationRead != "END_FILE"){

        // Sprites information specified in interval
        if (informationRead == "GROUP_LINES:"){
            if (indexSpecified){
                // No sprites index specified so there is error
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". INTERVAL OF LINES PROCESSED, SRITE_NEAR_LEFT: OR SPRITE_NEAR_RIGHT: TOKEN WAS EXPECTED." << std::endl;

                return !instance.failDetected;
            }
            else {
                // Preparing the sprite information to be read
                spriteFarLeft = false;
                spriteFarRight = false;
                spriteNearLeft = false;
                spriteNearRight = false;
                spriteCenter = false;
                newInterval = false;
                instance.column += 2;
                int parametersToRead = 4;

                // Reading the configuration parameters
                for (int i = 0; i < parametersToRead; i++){
                    instance.inputFlux >> informationRead;

                    // Possible missed parameters
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
                        // Read the parameter and check the type of value
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
                            // The type of the any of the parameters is wrong
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
        // Determine if the sprite is only on a concrete line (not interval and it is not a checkpoint)
        else if (informationRead == "LINE:" || informationRead == "LINE_CHECKPOINT:"){
            if (indexSpecified){
                // Index of the sprite unknown
                instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                    instance.column << ". INTERVAL OF LINES PROCESSED, SRITE_NEAR_LEFT OR SPRITE_NEAR_RIGHT TOKEN WAS EXPECTED." << std::endl;

                return !instance.failDetected;
            }
            else {
                // The sprite to insert is a checkpoint
                if (informationRead == "LINE_CHECKPOINT:")
                    isCheckPoint = true;

                // Prepare the configuration flags
                endPos = -1;
                spriteFarLeft = false;
                spriteFarRight = false;
                spriteNearLeft = false;
                spriteNearRight = false;
                spriteCenter = false;
                newInterval = false;
                instance.column += 2;
                instance.inputFlux >> informationRead;

                // Position of the scenario to be inserted is not found
                if (instance.inputFlux.eof()){
                    instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                        instance.column << ". INDEX CODE OF LINE NOT FOUND." << std::endl;

                    return !instance.failDetected;
                }
                else {
                    // Check the type of position value
                    if (std::regex_match(informationRead, instance.natural_number_regex)){
                        startPos = std::stoi(informationRead);

                        // Check the position of the sprite is inside the scenario
                        if (startPos >= instance.linesOfBiome){
                            // Index outside the biome
                            instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
                                instance.column << ". START INTERVAL VALUE " << startPos << " IS HIGHER OR EQUAL THAN "
                                                << "MAP SIZE " << instance.linesOfBiome << std::endl;

                            return !instance.failDetected;
                        }

                        instance.row++;
                        instance.column = 1;
                    }
                    else {
                        // Type error value
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
            // Check the position of the sprite (look at the codes) far left
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
            // Check the position of the sprite (look at the codes) near left
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
            // Check the position of the sprite (look at the codes) center
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
            // Check the position of the sprite (look at the codes) far right
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
            // Check the position of the sprite (look at the codes) near right
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
        // Prepare new sprite interval
        if (!newInterval){
            instance.inputFlux >> informationRead;
        }
    }
    if (spritesProcessed){
        // Sprites section completely read
        instance.outputFlux << "SPRITES PART CORRECTLY SPECIFIED." << std::endl;
        return instance.failDetected;
    }
    else {
        // Sprites section empty
        instance.outputFlux << "SYNTAX ERROR IN LINE " << instance.row << " AND COL " <<
            instance.column << ". SPRITE PART IS TOTALLY EMPTY." << std::endl;

        return !instance.failDetected;
    }
}



/**
 * Change the sprites status of a sprite from a concrete biome
 * @param is the biome where a sprite is going to be updated
 * @param spriteAnimated is the sprite which is going to be updated
 */
void Logger::updateSprite(Biome& m, const Sprite_Animated spriteAnimated){
    // Check if the scenario is the start or the goal
    bool startMap = m.getStartBiome();
    bool goalMap = m.getGoalBiome();
    if (startMap || goalMap){
        // Play the animations
        if (spriteAnimated == Sprite_Animated::FLAGGER){
            // Flagger sprite
            switch (instance.status){
                // Te flagger is upping the flag
                case Flagger_Status::UPPING_FLAG:
                    instance.flagger_code_image++;
                    if (instance.flagger_code_image == instance.FLAGGER_FLAG)
                        instance.status = Flagger_Status::MOVING_FLAG;
                    break;
                case Flagger_Status::MOVING_FLAG:
                    // Flagger starts to move the flag and repeats it many times
                    if (instance.flagger_code_image < instance.FLAGGER_HAND){
                        if (instance.numIterations != instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.numIterations = 0;
                            instance.flagger_code_image++;
                        }
                    }
                    else {
                        // Flagger stops moving the flag
                        if (instance.numIterations != instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.attempts++;
                            // Flagger starts to move the hand
                            if (instance.attempts == instance.FLAGGER_CHANGE_STATUS){
                                instance.status = Flagger_Status::MOVING_HAND;
                                instance.flagger_code_image = instance.FLAGGER_HAND;
                                instance.attempts = 0;
                            }
                            else
                                instance.flagger_code_image = instance.FLAGGER_FLAG + 1;

                            instance.numIterations = 0;
                        }
                    }
                    break;
                case Flagger_Status::MOVING_HAND:
                    // Next status is moving back
                    if (instance.flagger_code_image < instance.FLAGGER_BACK){
                        if (instance.numIterations != 2 * instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.numIterations = 0;
                            instance.flagger_code_image++;
                        }
                    }
                    else {
                        // Flagger stops moving the hand and goes back
                        if (instance.numIterations != 2 * instance.FLAGGER_CHANGE_ANIM)
                            instance.numIterations++;
                        else {
                            instance.attempts++;
                            if (instance.attempts == instance.FLAGGER_CHANGE_STATUS){
                                instance.totalTimes++;
                                if (instance.totalTimes == 3){
                                    instance.status = Flagger_Status::MOVING_BACK;
                                    instance.flagger_code_image = instance.FLAGGER_BACK + 1;
                                }
                                else {
                                    instance.status = Flagger_Status::MOVING_FLAG;
                                    instance.flagger_code_image = instance.FLAGGER_FLAG + 1;
                                    instance.attempts = 0;
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

            /*
             * Update the flagger status using its index and position
             */

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

            /*
             * Update the semaphore status using its index and position
             */

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



/**
 * Close the logger service
 */
void Logger::close(){
    setLoggerStatus();
    instance.inputFlux.close();
    instance.outputFlux.close();
}



