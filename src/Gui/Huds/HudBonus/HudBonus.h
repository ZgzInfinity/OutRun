
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

#ifndef HUD_BONUS_H
#define HUD_BONUS_H

#include <SFML/Graphics.hpp>
#include "../../../Input/Input.h"

enum class Hud_Bonus_Text_Indicator : int {
    BONUS_TIME_POINTS_TEXT,
    BONUS_SECONDS_POINTS_TEXT,
    BONUS_SECONDS_TEXT,
    BONUS_CROSS_TEXT,
    BONUS_BONIFICATION_TEXT,
    BONUS_POINTS_TEXT,
    __COUNT
};


class HudBonus {

    private:

        static HudBonus instance;

        const std::string BONUS_POINTS_INDICATOR = "BONUS POINTS.";

        const std::string BONUS_SECONDS_INDICATOR = "SEC";

        const std::string BONUS_CROSS_SIGN_INDICATOR = "x";

        const std::string BONUS_POINTS_BONIFICATION_INDICATOR = "1000000";

        const std::string POINTS_INDICATOR = "PTS";

        std::array<sf::Text, (int)Hud_Bonus_Text_Indicator::__COUNT> hudTexts;

        sf::Font hudIndicatorText;

        int secondsBonus;

        int cents_secondBonus;

    public:

        HudBonus();

        static void loadHudBonus(Input& input);

        static void setTextHudBonusIndicator();

        static void setAllHudBonusIndicators(Input& input);

        static void setHudBonus(const int _secondsBonus, const int _cents_secondBonus);

        static void drawHudBonus(Input& input);

};

#endif // HUD_BONUS_H
