
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
 * Interface file of the module HudBonus
 */

#pragma once

#ifndef HUD_BONUS_H
#define HUD_BONUS_H

#include <SFML/Graphics.hpp>
#include "../../../Input/Input.h"



/**
 * Represents the text indicators of the hud
 */
enum class Hud_Bonus_Text_Indicator : int {
    BONUS_TIME_POINTS_TEXT,
    BONUS_SECONDS_POINTS_TEXT,
    BONUS_SECONDS_TEXT,
    BONUS_CROSS_TEXT,
    BONUS_BONIFICATION_TEXT,
    BONUS_POINTS_TEXT,
    __COUNT
};



/**
 * Represents the hud that appears when the
 * player finishes a game round
 */
class HudBonus {

    private:

        // Static instance of the class
        static HudBonus instance;

        /*
         * Text indicators of the hud
         */

        const std::string BONUS_POINTS_INDICATOR = "BONUS POINTS.";

        const std::string BONUS_SECONDS_INDICATOR = "SEC";

        const std::string BONUS_CROSS_SIGN_INDICATOR = "x";

        const std::string BONUS_POINTS_BONIFICATION_INDICATOR = "1000000";

        const std::string POINTS_INDICATOR = "PTS";

        // Vector of texts
        std::array<sf::Text, (int)Hud_Bonus_Text_Indicator::__COUNT> hudTexts;

        // Font of the texts that compose the hud
        sf::Font hudIndicatorText;

        // Seconds of bonus
        int secondsBonus;

        // Tenths of seconds of bonus
        int tenths_secondBonus;


    public:



        /**
         * Default constructor
         */
        HudBonus();


        /**
         * Load the bonus hud
         * @param input is the module that has all the configuration of the game
         */
        static void loadHudBonus(Input& input);



        /**
         * Set the content of the hud
         * @param _secs are the seconds of bonus given to the player
         * @param _tenths_second are the tenths of seconds of bonus given to the player
         */
        static void setHudBonus(const int _secondsBonus, const int _tenths_second);



        /**
         * Set the text contents of the text indicators
         */
        static void setTextHudBonusIndicator();



        /**
         * Draw the hud of the bonus in the screen
         * @param input is the module that has all the configuration of the game
         */
        static void drawHudBonus(Input& input);

};

#endif // HUD_BONUS_H
