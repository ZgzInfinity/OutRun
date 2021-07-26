
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

#pragma once

#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include "../../Input/Input.h"


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


class Hud {

    private:

        static Hud instance;

        int time;

        long long int score;

        float minutes;

        float secs;

        float cents_second;

        float minutesTrip;

        float secsTrip;

        float cents_secondTrip;

        int level;

        int gear;

        float speed, speedHud;

        float maxSpeed;

        std::array<sf::Texture, (int)Hud_Texture_Indicator::__COUNT> hudTextures;

        std::array<sf::Sprite, (int)Hud_Texture_Indicator::__COUNT> hudSprites;

        std::array<sf::Text, (int)Hud_Text_Indicator::__COUNT> hudTexts;

        sf::Font hudIndicatorSpeed, hudIndicatorText;

        sf::Texture speedMotorIndicator;

        static void loadHudTextureIndicator(const Hud_Texture_Indicator hudInd, const std::string& name);

        static void setScaleHudIndicator(const Hud_Texture_Indicator& hudInd, const sf::Vector2f& scale);

        static void setPositionHudIndicator(const Hud_Texture_Indicator& hudInd, const sf::Vector2f& pos);

        static void setTextHudIndicator(const Hud_Text_Indicator& hudInd, const std::string message, const sf::Vector2f& pos);

        static void setTextHudIndicator(const Hud_Text_Indicator& hudInd, const std::string message);

    public:

        Hud();

        static void loadHud();

        static void configureHud(Input& input);

        static void setHud(const int _time, const long long int _score, const float _minutes,
                           const float _secs, const float _cents_second, const int _level,
                           const int _gear, const float _speed, const float _maxSpeed);

        static void setAllHudIndicators(Input& input);

        static void drawHud(Input& input);
};

#endif // HUD_H
