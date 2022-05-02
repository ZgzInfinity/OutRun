
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
 * Interface file of the module Logger
 */

#pragma once

#include <regex>
#include <fstream>
#include "../Scene/Biome/Biome.h"
#include "../Random/Random.h"

#ifndef LOGGER_H
#define LOGGER_H

class Biome;



/**
 * Represents the log service of the game
 * Load the biomes, control the animations of the game
 * and warns the player if there is any kind of error
 */
class Logger {

    public:


        /**
         * Status of the flagger animation
         */
        enum class Flagger_Status : int {
            UPPING_FLAG,
            MOVING_FLAG,
            MOVING_HAND,
            MOVING_BACK
        };


        /*
         * Identifier constants to represent the points
         * of transition of status in the flagger animation
         */

        static constexpr const int FLAGGER_FLAG = 2;

        static constexpr const int FLAGGER_HAND = 9;

        static constexpr const int FLAGGER_BACK = 11;

        static constexpr const int FLAGGER_END_ANIMATION = 13;

        static constexpr const int FLAGGER_CHANGE_STATUS = 10;

        static constexpr const int FLAGGER_CHANGE_ANIM = 5;

        static constexpr const int MAX_SEMAPHORE_CODE_IMAGE = 41;

        // Path of the file where the logger writes the context of the game
        const std::string LOGGER_PATH_FILE = "Resources/Logger/Logger.txt";

        /*
         * Regular expressions
         */

        const std::regex natural_number_regex{"[0-9]+"};

        const std::regex float_number_regex{"-?[0-9]+.?[0-9]*"};

        const std::regex integer_number_regex{"-?[0-9]+"};

        // Static instance of the class
        static Logger instance;

        // Column of the file where there is a syntax error
        int column;

        // Row of the file where there is a syntax error
        int row;

        // Index code of the semaphore sprite to draw in the semaphore animation
        int semaphore_code_image;

        // Index code of the semaphore sprite to draw in the flagger animation
        int flagger_code_image;

        // Number of attempts done by the flagger to move the flag
        int attempts;

        // Total iterations to execute the next the flag moving
        int  numIterations;

        // Total iterations to execute the flag moving
        int totalTimes;

        // Total lines of the biome without counting the fork
        int linesOfBiome;

        // Width of the screen
        float widthScreen;

        // Control if there has been any type of error
        bool failDetected;

        // Control if it is the first time that logger is invoked to do anything
        bool firstTimeInvoked;

        // Control if the flagger has finished the animation
        bool endFlaggerAnimation;

        // Previous status of the flagger animation
        Flagger_Status previousStatus;

        // Current status of the flagger animation
        Flagger_Status status;

        // Reading flux to read the biome files
        std::ifstream inputFlux;

        // Writing flux to write the status of the game
        std::ofstream outputFlux;



        /**
         * Default constructor
         */
        Logger();



        /**
         * Set the logger status default configuration
         */
        void static setLoggerStatus();



        /**
         * Set if the logger has detected any error
         * @param _failDetected is the possible error detected
         */
        void static setFailDetected(const bool _failDetected);



        /**
         * Set the with of the screen
         * @param _widthScreen is the width screen
         */
        void static setWidthScreen(const int _widthScreen);



        /**
         * Set the position in the axis y of the sprites (floatin sprites) in the biome
         * @param m is the biome whose sprites are going to be modified
         */
        void static setSpriteScreenY(Biome& m);



        /**
         * Get if the logger has found any error
         */
        bool static getFailDetected();



        /**
         * Get if the flagger has finished its animation
         */
        bool static getEndFlaggerAnimation();



        /**
         * Returns true if a biome has a default configuration
         * file. Otherwise returns false
         * @param pathMapFile is the path of the biome's file to be checked
         */
        bool static checkMapFile(const std::string pathMapFile);



        /**
         * Returns true if there is a default configuration for the time
         * the type of road of the biome and stores it. Otherwise returns false
         * @param m is the biome where the values of time and kind of terrain
         * are going to be stored
         */
        bool static checkTimeTerrainRoad(Biome& m);



        /**
         * Returns true if the color has been read and stored.
         * Otherwise returns false
         * @param colorRead is where the color is read is going to be stored
         */
        bool static readColor(sf::Color& colorRead);



        /**
         * Returns true if the colors of the biome have been read and
         * stored. Otherwise returns false
         * @param m is the biome whose colors are going to be read and stored
         */
        bool static checkColors(Biome& m);



        /**
         * Returns true if the a straight relief of the biome has been
         * read correctly. Otherwise returns false
         * @param m is the biome where the straight relief is going to be stored
         */
        bool static checkReliefStraight(Biome& m);



        /**
         * Returns true if the a curve with a direction of the biome has been
         * read correctly. Otherwise returns false
         * @param m is the biome where the relief is going to be stored
         * @param leftDirection is the direction of the curve (true is left curve
         * and false is right curve
         */
        bool static checkReliefCurve(Biome& m, const bool leftDirection);



        /**
         * Returns true if the a hill of the relief of the biome has been
         * read correctly. Otherwise returns false
         * @param m is the biome where the relief is going to be stored
         */
        bool static checkReliefHillStraight(Biome& m);



        /**
         * Returns true if the a curve hill with a direction of the biome has been
         * read correctly. Otherwise returns false
         * @param m is the biome where the relief is going to be stored
         * @param leftDirection is the direction of the curve hill (true is left curve
         * and false is right curve
         */
        bool static checkReliefHillCurve(Biome& m, const bool leftDirection);



        /**
         * Returns true if the a relief of the biome has been
         * read correctly. Otherwise returns false
         * @param m is the biome where the relief is going to be stored
         */
        bool static checkBiomeRelief(Biome& m);



        /**
         * Load the sprites of the starting biome
         * @param m is the starting biome
         */
        void static loadStartBiomeSprites(Biome& m);



        /**
         * Load the sprites of the goal biome
         * @param m is the goal biome
         */
        void static loadGoalBiomeSprites(Biome& m);



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
        bool static checkLevelBiomeSprite(Biome& m, const int startPos, const int endPos, const int incrementor, const int frequency,
                                          bool& indexSpecified, bool& spritesProcessed, const int pos, const bool isCheckPoint);



        /**
         * Returns true if the load of all the sprites of the biome
         * has been done correctly. Otherwise returns false
         * @param m is the biome where all the sprites are going to be loaded
         */
        bool static checkLevelBiomeSprites(Biome& m);




        /**
         * Change the sprites status of a sprite from a concrete biome
         * @param is the biome where a sprite is going to be updated
         * @param spriteAnimated is the sprite which is going to be updated
         */
        void static updateSprite(Biome& m, const Sprite_Animated spriteAnimated);



        /**
         * Close the logger service
         */
        void static close();
};

#endif // LOGGER_H

