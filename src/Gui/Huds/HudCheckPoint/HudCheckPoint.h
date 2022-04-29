
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
 * Interface file of the module HudCheckPoint
 */

#pragma once

#ifndef HUD_CHECKPOINT_H
#define HUD_CHECKPOINT_H

#include <SFML/Graphics.hpp>
#include "../../../Input/Input.h"



/**
 * Represents the texture indicators of the checkpoint hud
 */
enum class Hud_Texture_CheckPoint_Indicator : int {
    EXTENDED_TIME_INDICATOR,
    LAP_INDICATOR,
    __COUNT
};



/**
 * Represents the text indicators of the checkpoint hud
 */
enum class Hud_Text_CheckPoint_Indicator : int {
    CHECKPOINT_TEXT,
    LAP_TEXT,
    __COUNT
};



/**
 * Represents the checkpoint hud that appears when
 * the player completes a scenario
 */
class HudCheckPoint {

    private:

        // Static instance of the class
        static HudCheckPoint instance;

        // Minutes inverted in complete the scenario
        float minutes;

        // Seconds inverted in complete the scenario
        float secs;

        // Hundredths of second inverted in complete the scenario
        float cents_second;

        // Vector of textures to store the graphic indicators
        std::array<sf::Texture, (int)Hud_Texture_CheckPoint_Indicator::__COUNT> hudTextures;

        // Vector of sprites to store the textures
        std::array<sf::Sprite, (int)Hud_Texture_CheckPoint_Indicator::__COUNT> hudSprites;

        // Vector of texts to store the text indicators
        std::array<sf::Text, (int)Hud_Text_CheckPoint_Indicator::__COUNT> hudTexts;

        // Font of the text indicators
        sf::Font hudIndicatorCheckPointText;



        /**
         * Load a texture in the vector of textures of the hud
         * @param hudInd is the index position of the vector where the texture has to be stored
         * @param name is the path from where the texture has to be loaded
         */
        static void loadHudCheckPointTextureIndicator(const Hud_Texture_CheckPoint_Indicator hudInd, const std::string name);



        /**
         * Rescale a indicator of the hud (texture)
         * @param hudInd is the index of the indicator to be scaled in the screen
         * @param scale is a two components array that shows the new scale ratio of the new indicator
         */
        static void setScaleHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator hudInd, const sf::Vector2f scale);



        /**
         * Set the text content of a text indicator of the hud
         * @param hudInd is the index of the text indicator whose content is going to be updated
         * @param message is the new content of the text indicator
         */
        static void setTextHudCheckPointIndicator(const Hud_Text_CheckPoint_Indicator hudInd, const std::string message);



        /**
         * Locate a indicator of the hud (text) in a concrete position of the screen
         * @param hudInd is the index of the indicator to be located in the screen
         * @param pos is a two components array that shows where the indicator has to be located
         */
        static void setTextPositionHudCheckPointIndicator(const Hud_Text_CheckPoint_Indicator hudInd, const sf::Vector2f& pos);



        /**
         * Locate a indicator of the hud (texture) in a concrete position of the screen
         * @param hudInd is the index of the indicator to be located in the screen
         * @param pos is a two components array that shows where the indicator has to be located
         */
        static void setPositionHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator hudInd, const sf::Vector2f& pos);

    public:


        /**
         * Default constructor
         */
        HudCheckPoint();



        /**
         * Load all the checkpoint hud
         */
        static void loadHudCheckPoint();



        /**
         * Set the content, scale and positions of all the indicator that
         * compose the checkpoint hud
         * @param input is the module that has all the configuration of the game
         */
        static void configureHudCheckPoint(Input& input);



        /**
         * Set the content of the hud
         * @param _minutes are the minutes inverted in complete the scenario
         * @param _secs are the seconds inverted in complete the scenario
         * @param _cents_second are the hundredths of seconds inverted in complete the scenario
         */
        static void setHudCheckPoint(const float _minutes, const float _secs, const float _cents_second);



        /**
         * Draw the hud of the checkpoint in the screen
         * @param input is the module that has all the configuration of the game
         */
        static void drawHudCheckPoint(Input& input);
};

#endif // HUD_CHECKPOINT_H
