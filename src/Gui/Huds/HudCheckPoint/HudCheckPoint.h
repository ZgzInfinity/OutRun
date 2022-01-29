
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

#ifndef HUD_CHECKPOINT_H
#define HUD_CHECKPOINT_H

#include <SFML/Graphics.hpp>
#include "../../../Input/Input.h"


 enum class Hud_Texture_CheckPoint_Indicator : int {
    LAP_INDICATOR,
    EXTENDED_TIME_INDICATOR,
    __COUNT
};


enum class Hud_Text_CheckPoint_Indicator : int {
    CHECKPOINT_TEXT,
    LAP_TEXT,
    __COUNT
};


class HudCheckPoint {

    private:

        static HudCheckPoint instance;

        float minutes;

        float secs;

        float cents_second;

        std::array<sf::Texture, (int)Hud_Texture_CheckPoint_Indicator::__COUNT> hudTextures;

        std::array<sf::Sprite, (int)Hud_Texture_CheckPoint_Indicator::__COUNT> hudSprites;

        std::array<sf::Text, (int)Hud_Text_CheckPoint_Indicator::__COUNT> hudTexts;

        sf::Font hudIndicatorCheckPointText;

        static void loadHudCheckPointTextureIndicator(const Hud_Texture_CheckPoint_Indicator hudInd, const std::string& name);

        static void setScaleHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator& hudInd, const sf::Vector2f& scale);

        static void setPositionHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator& hudInd, const sf::Vector2f& pos);

        static void setTextHudCheckPointIndicator(const Hud_Text_CheckPoint_Indicator& hudInd, const std::string message, const sf::Vector2f& pos);

        static void setTextHudCheckPointIndicator(Input& input, const Hud_Text_CheckPoint_Indicator& hudInd, const std::string message);

    public:

        HudCheckPoint();

        static void loadHudCheckPoint();

        static void configureHudCheckPoint(Input& input);

        static void setHudCheckPoint(const float _minutes, const float _secs, const float _cents_second);

        static void setAllHudCheckPointIndicators(Input& input);

        static void drawHudCheckPoint(Input& input);
};

#endif // HUD_CHECKPOINT_H
