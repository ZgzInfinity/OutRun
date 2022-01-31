
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

#include "HudCheckPoint.h"

HudCheckPoint HudCheckPoint::instance;

HudCheckPoint::HudCheckPoint(){
    instance.minutes = 0;
    instance.secs = 0;
    instance.cents_second = 0;
}

void HudCheckPoint::loadHudCheckPointTextureIndicator(const Hud_Texture_CheckPoint_Indicator hudInd , const std::string& name){
    sf::Texture indicatorTexture;
    if (indicatorTexture.loadFromFile(name)){
        instance.hudTextures[(int)hudInd] = indicatorTexture;
        instance.hudSprites[(int)hudInd].setTexture(instance.hudTextures[(int)hudInd], true);
    }
}

void HudCheckPoint::loadHudCheckPoint(){
    loadHudCheckPointTextureIndicator(Hud_Texture_CheckPoint_Indicator::EXTENDED_TIME_INDICATOR,
                                      "Resources/Huds/HudCheckPoint/Extended_play_Indicator.png");

    loadHudCheckPointTextureIndicator(Hud_Texture_CheckPoint_Indicator::LAP_INDICATOR, "Resources/Huds/HudCheckPoint/Lap_Indicator.png");

    instance.hudIndicatorCheckPointText.loadFromFile("Resources/Fonts/DisposableDroid.ttf");
}

void HudCheckPoint::setScaleHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator& hudInd, const sf::Vector2f& scale){
    instance.hudSprites[(int)hudInd].setScale(scale.x, scale.y);
}


void HudCheckPoint::setTextHudCheckPointIndicator(const Hud_Text_CheckPoint_Indicator& hudInd, const std::string message)
{
    instance.hudTexts[(int)hudInd].setString(message);
}

void HudCheckPoint::setTextPositionHudCheckPointIndicator(const Hud_Text_CheckPoint_Indicator& hudInd, const sf::Vector2f& pos)
{
    instance.hudTexts[(int)hudInd].setPosition(pos.x, pos.y);
}

void HudCheckPoint::setPositionHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator& hudInd, const sf::Vector2f& pos)
{
    instance.hudSprites[(int)hudInd].setPosition(pos.x, pos.y);
}

void HudCheckPoint::configureHudCheckPoint(Input& input){

    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT].setFont(instance.hudIndicatorCheckPointText);
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT].setCharacterSize(static_cast<unsigned int>(int(52.f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    instance.setTextHudCheckPointIndicator(Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT, "CHECKPOINT!");
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT].setOutlineColor(sf::Color(14, 29, 184));

    float posX = input.gameWindow.getSize().x / 2.f -
                 instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT].getGlobalBounds().width / 2.0f;
    float posY = input.gameWindow.getSize().y / 2.f - 190.0f * input.screenScaleX;

    sf::Vector2f posCheckPointTextIndicator(posX * 1.005f, posY);
    setTextPositionHudCheckPointIndicator(Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT, posCheckPointTextIndicator);


    setScaleHudCheckPointIndicator(Hud_Texture_CheckPoint_Indicator::EXTENDED_TIME_INDICATOR,
                                   sf::Vector2f(2.62f * input.screenScaleX, 2.62f * input.screenScaleX));

    posX = input.gameWindow.getSize().x / 2.f -
           instance.hudSprites[(int)Hud_Texture_CheckPoint_Indicator::EXTENDED_TIME_INDICATOR].getGlobalBounds().width / 2.0f;
    posY = input.gameWindow.getSize().y / 2.f - 120.0f * input.screenScaleX;

    sf::Vector2f posCheckPointExtendedTimeIndicator(posX, posY);
    setPositionHudCheckPointIndicator(Hud_Texture_CheckPoint_Indicator::EXTENDED_TIME_INDICATOR, posCheckPointExtendedTimeIndicator);

    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].setFont(instance.hudIndicatorCheckPointText);
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].setOutlineThickness(3.0f * input.screenScaleX);

    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].setOutlineColor(sf::Color::Black);


    posX = input.gameWindow.getSize().x / 2.f -
           instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].getGlobalBounds().width / 2.0f;
    posY = input.gameWindow.getSize().y / 2.f + 30.0f * input.screenScaleX;

    sf::Vector2f posCheckPointTextLapIndicator(posX, posY);
    setTextPositionHudCheckPointIndicator(Hud_Text_CheckPoint_Indicator::LAP_TEXT, posCheckPointTextLapIndicator);


    setScaleHudCheckPointIndicator(Hud_Texture_CheckPoint_Indicator::LAP_INDICATOR,
                                   sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    posX = float((input.gameWindow.getSize().x) +
                 instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT].getGlobalBounds().width / 2.0f) / 2.4f;

    posY = input.gameWindow.getSize().y / 2.f - 10.0f * input.screenScaleX;
    sf::Vector2f posCheckPointLapIndicator(posX, posY);
    setPositionHudCheckPointIndicator(Hud_Texture_CheckPoint_Indicator::LAP_INDICATOR, posCheckPointLapIndicator);
}

void HudCheckPoint::setHudCheckPoint(const float _minutes, const float _secs, const float _cents_second){
    instance.minutes = _minutes;
    instance.secs = _secs;
    instance.cents_second = _cents_second;

    std::string lap;
    lap = (instance.minutes < 10.f) ? "0" + to_string(int(instance.minutes)) + " '" : to_string(int(instance.minutes)) + " '";
    lap += (instance.secs < 10.f) ? "0" + to_string(int(instance.secs)) + " ''" : to_string(int(instance.secs)) + " ''";
    lap += (int(instance.cents_second * 100.f) < 10 ) ? "0" + to_string(int(instance.cents_second * 100.f)) : to_string(int(instance.cents_second * 100.f));
    instance.setTextHudCheckPointIndicator(Hud_Text_CheckPoint_Indicator::LAP_TEXT, lap);
}

void HudCheckPoint::drawHudCheckPoint(Input& input){
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::CHECKPOINT_TEXT]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_CheckPoint_Indicator::EXTENDED_TIME_INDICATOR]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_CheckPoint_Indicator::LAP_INDICATOR]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_CheckPoint_Indicator::LAP_TEXT]);
}


