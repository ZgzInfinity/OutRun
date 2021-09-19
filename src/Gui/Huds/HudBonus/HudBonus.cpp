
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

#include "HudBonus.h"

HudBonus HudBonus::instance;

HudBonus::HudBonus(){
    secondsBonus = 0;
    cents_secondBonus = 0;
}


void HudBonus::loadHudBonus(Input& input){

    float separation = 50.f;
    instance.hudIndicatorText.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setString(instance.BONUS_POINTS_INDICATOR);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setCharacterSize(static_cast<unsigned int>(int(65.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setOutlineColor(sf::Color(12, 12, 12));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setOutlineThickness(3.0f * input.screenScaleX);

    float initialY = float(input.gameWindow.getSize().y) / 3.0f +
                     float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].getCharacterSize());

    float initial = (float(input.gameWindow.getSize().x) -
                     instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].getGlobalBounds().width) / 2.0f;

    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].setPosition(initial, float(input.gameWindow.getSize().y) / 3.0f -
                     float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT].getCharacterSize()));


    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setOutlineColor(sf::Color(12, 12, 12));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    initial -= instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].getLocalBounds().width;
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setPosition(initial / 1.7f, initialY);
    initialY += float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].getCharacterSize());
    initial += 1.25f * instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].getLocalBounds().width;


    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setString(instance.BONUS_SECONDS_INDICATOR);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setFillColor(sf::Color(183, 164, 190));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setOutlineThickness(3.0f * input.screenScaleX);

    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].setPosition(initial, initialY -
                     float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].getCharacterSize()));

    initial += separation + instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT].getLocalBounds().width;


    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setString(instance.BONUS_CROSS_SIGN_INDICATOR);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setFillColor(sf::Color(232, 191, 157));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setOutlineThickness(3.0f * input.screenScaleX);

    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].setPosition(initial, initialY -
                      float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].getCharacterSize()));

    initial += separation + instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT].getLocalBounds().width;


    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setString(instance.BONUS_POINTS_BONIFICATION_INDICATOR);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setOutlineColor(sf::Color(12, 12, 12));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setOutlineThickness(3.0f * input.screenScaleX);

    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].setPosition(initial, initialY -
                     float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].getCharacterSize()));

    initial += separation + instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT].getLocalBounds().width;


    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setString(instance.POINTS_INDICATOR);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setFillColor(sf::Color(183, 164, 190));
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].setPosition(initial, initialY -
                     float(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT].getCharacterSize()));
}

void HudBonus::setTextHudBonusIndicator(){
    instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT].setString(
                     to_string(instance.secondsBonus) + "." + to_string(instance.cents_secondBonus));
}

void HudBonus::setHudBonus(const int _secondsBonus, const int _cents_secondBonus){
    instance.secondsBonus = _secondsBonus;
    instance.cents_secondBonus = _cents_secondBonus;
}

void HudBonus::drawHudBonus(Input& input){
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_TIME_POINTS_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_POINTS_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_SECONDS_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_CROSS_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_BONIFICATION_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Bonus_Text_Indicator::BONUS_POINTS_TEXT]);
}

