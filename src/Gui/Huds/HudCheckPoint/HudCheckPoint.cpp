
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

}

void HudCheckPoint::loadHudCheckPoint(){
    loadHudCheckPointTextureIndicator(Hud_Texture_CheckPoint_Indicator::LAP_INDICATOR, "Resources/Huds/HudCheckPoint/Lap_Indicator.png");
    loadHudCheckPointTextureIndicator(Hud_Texture_CheckPoint_Indicator::EXTENDED_TIME_INDICATOR, "Resources/Huds/HudCheckPoint/Extended_play_Indicator.png");

    instance.hudIndicatorCheckPointText.loadFromFile("Resources/Fonts/DisposableDroid.ttf");
}

void HudCheckPoint::setScaleHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator& hudInd, const sf::Vector2f& scale){

}

void HudCheckPoint::setPositionHudCheckPointIndicator(const Hud_Texture_CheckPoint_Indicator& hudInd, const sf::Vector2f& pos){

}

void HudCheckPoint::setTextHudCheckPointIndicator(const Hud_Text_CheckPoint_Indicator& hudInd, const std::string message, const sf::Vector2f& pos){

}

void HudCheckPoint::setTextHudCheckPointIndicator(Input& input, const Hud_Text_CheckPoint_Indicator& hudInd, const std::string message){

}

void HudCheckPoint::setAllHudCheckPointIndicators(Input& input){

}

void HudCheckPoint::configureHudCheckPoint(Input& input){

}

void HudCheckPoint::setHudCheckPoint(const float _minutes, const float _secs, const float _cents_second){
    instance.minutes = _minutes;
    instance.secs = _secs;
    instance.cents_second = _cents_second;
}

void HudCheckPoint::drawHudCheckPoint(Input& input){

}


