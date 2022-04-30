
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
 * Interface file of the module HudRound
 */

#pragma once

#ifndef HUD_ROUND_H
#define HUD_ROUND_H

#include <SFML/Graphics.hpp>
#include "../../../Input/Input.h"



/**
 * Represents the texture indicators of the round hud
 */
enum class Hud_Texture_Indicator : int {
    TIME_INDICATOR,
    SCORE_INDICATOR,
    LAP_INDICATOR,
    SPEED_MOTOR_INDICATOR,
    SPEED_INDICATOR,
    TREE_LEVEL_1_INDICATOR,
    TREE_LEVEL_2_INDICATOR,
    TREE_LEVEL_3_INDICATOR,
    TREE_LEVEL_4_INDICATOR,
    TREE_LEVEL_5_INDICATOR,
    TREE_LEVEL_6_INDICATOR,
    TREE_LEVEL_7_INDICATOR,
    TREE_LEVEL_8_INDICATOR,
    TREE_LEVEL_9_INDICATOR,
    TREE_LEVEL_10_INDICATOR,
    TREE_LEVEL_11_INDICATOR,
    TREE_LEVEL_12_INDICATOR,
    TREE_LEVEL_13_INDICATOR,
    TREE_LEVEL_14_INDICATOR,
    TREE_LEVEL_15_INDICATOR,
    STAGE_INDICATOR,
    GEAR_INDICATOR,
    EXTENDED_TIME_INDICATOR,
    __COUNT
};



/**
 * Represents the text indicators of the round hud
 */
enum class Hud_Text_Indicator : int {
    TIME_TEXT,
    SCORE_TEXT,
    LAP_TEXT,
    SPEED_TEXT,
    LEVEL_TEXT,
    GEAR_TEXT,
    GAME_OVER_TEXT,
    __COUNT
};



/**
 * Represents the round hud that appears when
 * the player playing a game round
 */
class HudRound {

    private:

        // Static instance of the class
        static HudRound instance;

        // Time to complete the scenario
        int time;

        // Score of the player
        long long int score;

        // Minutes of the lap in the scenario
        float minutes;

        // Seconds of the lap in the scenario
        float secs;

        // Hundredths of seconds of the lap in the scenario
        float cents_second;

        // Minutes spent since the player started the game round
        float minutesTrip;

        // Seconds spent since the player started the game round
        float secsTrip;

        // Hundredths of seconds spent since the player started the game round
        float cents_secondTrip;

        // Current level where the player is driving
        int level;

        // Gear of the car
        int gear;

        // Position of the level in the tree map panel (upper right corner)
        int treeMapPos;

        // Speed of the car
        float speed;

        // Speed factor multiplier to rescale the speed indicator in km/h
        float speedHud;

        // Maximum speed reached by the player car
        float maxSpeed;

        // Vector of textures to store the graphic indicators
        std::array<sf::Texture, (int)Hud_Texture_Indicator::__COUNT> hudTextures;

        // Vector of sprites to store the textures
        std::array<sf::Sprite, (int)Hud_Texture_Indicator::__COUNT> hudSprites;

        // Vector of texts to store the text indicators
        std::array<sf::Text, (int)Hud_Text_Indicator::__COUNT> hudTexts;

        // Fonts of the hud to write the texts
        sf::Font hudIndicatorSpeed, hudIndicatorText;

        // Texture of the speed motor indicator (grey bar)
        sf::Texture speedMotorIndicator;



        /**
         * Load a texture in the vector of textures of the hud
         * @param hudInd is the index position of the vector where the texture has to be stored
         * @param name is the path from where the texture has to be loaded
         */
        static void loadHudRoundTextureIndicator(const Hud_Texture_Indicator hudInd, const std::string name);



        /**
         * Rescale a indicator of the hud (texture)
         * @param hudInd is the index of the indicator to be scaled in the screen
         * @param scale is a two components array that shows the new scale ratio of the new indicator
         */
        static void setScaleHudRoundIndicator(const Hud_Texture_Indicator hudInd, const sf::Vector2f& scale);



        /**
         * Locate a indicator of the hud (texture) in a concrete position of the screen
         * @param hudInd is the index of the indicator to be located in the screen
         * @param pos is a two components array that shows where the indicator has to be located
         */
        static void setPositionHudRoundIndicator(const Hud_Texture_Indicator hudInd, const sf::Vector2f& pos);



        /**
         * Locate a indicator of the hud (text) in a concrete position with a concrete text in the screen
         * @param hudInd is the index of the indicator to be located in the screen
         * @param message is the new content of the text indicator
         * @param pos is a two components array that shows where the indicator has to be located
         */
        static void setTextHudRoundIndicator(const Hud_Text_Indicator hudInd, const std::string message, const sf::Vector2f& pos);



        /**
         * Set a indicator of the hud (text) with a concrete text message
         * @param input is the module that has all the configuration of the game
         * @param hudInd is the index of the indicator to be located in the screen
         * @param message is a two components array that shows where the indicator has to be located
         */
        static void setTextHudRoundIndicator(Input& input, const Hud_Text_Indicator hudInd, const std::string message);

    public:



        /**
         * Default constructor
         */
        HudRound();



        /**
         * Load all the checkpoint hud
         */
        static void loadHudRound();



        /**
         * Set the content, scale and positions of all the indicator that compose the round hud
         * @param input is the module that has all the configuration of the game
         */
        static void configureHudRound(Input& input);



        /**
         * Set all the indicators that compose the round hud
         * @param input is the module that has all the configuration of the game
         */
        static void setAllHudRoundIndicators(Input& input);



        /**
         * Set the content of the hud
         * @param _time is the time that the player has to complete the scenario
         * @param _scores is the score of the player
         * @param _minutes are the minutes spent since the player started the scenario
         * @param _secs are are the seconds spent since the player started the scenario
         * @param _cents_second are the hundredths of seconds spent since the player started the scenario
         * @param _level is the level where the playing is driving
         * @param _treeMapPos is the level location in the tree map (right hud panel)
         * @param _checkPoint controls if the checkpoint animation has to be drawn in the screen
         * @param _gear is the gear that the player is currently using (up or down)
         * @param _speed is the speed of the player car
         * @param _maxSpeed is the maximum speed that the player car can reach
         */
        static void setHudRound(const int _time, const long long int _score, const float _minutes,
                                const float _secs, const float _cents_second, const int _level, const int _treeMapPos,
                                const bool _checkPoint, const int _gear, const float _speed, const float _maxSpeed);

        /**
         * Draw the round hud in the screen
         * @param input is the module that has all the configuration of the game
         */
        static void drawHudRound(Input& input);
};

#endif // HUD_ROUND_H
