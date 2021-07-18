
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

#include "Hud.h"

Hud Hud::instance;

Hud::Hud(){
    time = 0;
    score = 0;
    minutes = 0.f;
    secs = 0.f;
    cents_second;
    minutesTrip = 0.f;
    secsTrip = 0.f;
    cents_secondTrip = 0.f;
    level = 0;
}

void Hud::loadHudTextureIndicator(const Hud_Texture_Indicator hudInd , const std::string& name){
    sf::Texture indicatorTexture;
    if (indicatorTexture.loadFromFile(name)){
        instance.hudTextures[(int)hudInd] = indicatorTexture;
        instance.hudSprites[(int)hudInd].setTexture(instance.hudTextures[(int)hudInd], true);
    }
}

void Hud::loadHud(){

    loadHudTextureIndicator(Hud_Texture_Indicator::TIME_INDICATOR, "Resources/Hud/Time_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::SCORE_INDICATOR, "Resources/Hud/Score_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::LAP_INDICATOR, "Resources/Hud/Lap_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::SPEED_INDICATOR, "Resources/Hud/Speed_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR, "Resources/Hud/Speed_Motor_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::STAGE_INDICATOR, "Resources/Hud/Stage_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::GEAR_INDICATOR, "Resources/Hud/Gear_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::EXTENDED_TIME_INDICATOR, "Resources/Hud/Extended_play_Indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR, "Resources/Hud/Tree_level_1_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_2_INDICATOR, "Resources/Hud/Tree_level_2_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_3_INDICATOR, "Resources/Hud/Tree_level_3_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_4_INDICATOR, "Resources/Hud/Tree_level_4_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_5_INDICATOR, "Resources/Hud/Tree_level_5_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_6_INDICATOR, "Resources/Hud/Tree_level_6_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_7_INDICATOR, "Resources/Hud/Tree_level_7_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_8_INDICATOR, "Resources/Hud/Tree_level_8_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_9_INDICATOR, "Resources/Hud/Tree_level_9_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_10_INDICATOR, "Resources/Hud/Tree_level_10_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_11_INDICATOR, "Resources/Hud/Tree_level_11_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_12_INDICATOR, "Resources/Hud/Tree_level_12_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_13_INDICATOR, "Resources/Hud/Tree_level_13_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_14_INDICATOR, "Resources/Hud/Tree_level_14_indicator.png");
    loadHudTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_15_INDICATOR, "Resources/Hud/Tree_level_15_indicator.png");

    instance.hudIndicatorSpeed.loadFromFile("Resources/Fonts/digital.ttf");
    instance.hudIndicatorText.loadFromFile("Resources/Fonts/DisposableDroid.ttf");
}

void Hud::setScaleHudIndicator(const Hud_Texture_Indicator& hudInd, const sf::Vector2f& scale){
    instance.hudSprites[(int)hudInd].setScale(scale.x, scale.y);
}

void Hud::setPositionHudIndicator(const Hud_Texture_Indicator& hudInd, const sf::Vector2f& pos){
    instance.hudSprites[(int)hudInd].setPosition(pos.x, pos.y);
}

void Hud::setTextHudIndicator(const Hud_Text_Indicator& hudInd, const std::string message, const sf::Vector2f& pos){
    instance.hudTexts[(int)hudInd].setPosition(pos.x, pos.y);
    instance.hudTexts[(int)hudInd].setString(message);
}

void Hud::configureHud(Input& input){

    const float up = float(input.gameWindow.getSize().y) / 10.0f;
    float posX, posY;

    setScaleHudIndicator(Hud_Texture_Indicator::TIME_INDICATOR,
                         sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    const float separation = instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR].getGlobalBounds().width / 3.f;
    posX = separation;
    posY = up - 1.1f * instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posTimeIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::TIME_INDICATOR, posTimeIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    float initial = separation + instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR].getGlobalBounds().width + separation;
    posX = initial;
    posY = up - 1.1f * float(instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].getCharacterSize());
    sf::Vector2f posTimeTextIndicator(posX, posY);
    setTextHudIndicator(Hud_Text_Indicator::TIME_TEXT, "120", posTimeTextIndicator);


    setScaleHudIndicator(Hud_Texture_Indicator::SCORE_INDICATOR,
                         sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    posX = instance.hudSprites[(int)Hud_Texture_Indicator::SCORE_INDICATOR].getGlobalBounds().width * 3.f;
    posY = up - 1.1f * instance.hudSprites[(int)Hud_Texture_Indicator::SCORE_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posScoreIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::SCORE_INDICATOR, posScoreIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setCharacterSize(static_cast<unsigned int>(int(34.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setFillColor(sf::Color(183, 164, 190));
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX *= 2.f;
    posY = up - 1.15f * float(instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].getCharacterSize());
    sf::Vector2f posScoreTextIndicator(posX, posY);
    setTextHudIndicator(Hud_Text_Indicator::SCORE_TEXT, "0", posScoreTextIndicator);

    setScaleHudIndicator(Hud_Texture_Indicator::LAP_INDICATOR,
                         sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    posX = instance.hudSprites[(int)Hud_Texture_Indicator::LAP_INDICATOR].getGlobalBounds().width * 9.2f;
    posY = up - 1.1f * instance.hudSprites[(int)Hud_Texture_Indicator::LAP_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posLapIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::LAP_INDICATOR, posLapIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setCharacterSize(static_cast<unsigned int>(int(34.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) * 0.8f;
    posY = up - 1.15f * float(instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].getCharacterSize());
    sf::Vector2f posLapTextIndicator(posX, posY);
    setTextHudIndicator(Hud_Text_Indicator::LAP_TEXT, "00' 00'' 00", posLapTextIndicator);



    setScaleHudIndicator(Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR,
                         sf::Vector2f(2.f * input.screenScaleX, 1.5f * input.screenScaleX));

    const float lengthSpeed = instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR].getGlobalBounds().height;
    const float down = float(input.gameWindow.getSize().y) - lengthSpeed * 1.5f;
    initial = separation + instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR].getGlobalBounds().width;
    posX = separation;
    posY = float(input.gameWindow.getSize().y) - lengthSpeed * 1.25f;
    sf::Vector2f posSpeedMotorIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR, posSpeedMotorIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setFont(instance.hudIndicatorSpeed);
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setCharacterSize(static_cast<unsigned int>(int(70.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setFillColor(sf::Color(206, 73, 73));
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) / 7.8f;
    posY = down - float(instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].getCharacterSize());
    sf::Vector2f posSpeedTextIndicator(posX, posY);
    setTextHudIndicator(Hud_Text_Indicator::SPEED_TEXT, "0", posSpeedTextIndicator);


    setScaleHudIndicator(Hud_Texture_Indicator::SPEED_INDICATOR,
                         sf::Vector2f(2.f * input.screenScaleX, 2.f * input.screenScaleX));

    posX = initial / 1.6f;
    posY = down - instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posSpeedIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::SPEED_INDICATOR, posSpeedIndicator);




    setScaleHudIndicator(Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR,
                         sf::Vector2f(4.f * input.screenScaleX, 4.f * input.screenScaleX));

    float width = instance.hudSprites[(int)Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR].getGlobalBounds().width;
    float height = instance.hudSprites[(int)Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR].getGlobalBounds().height;
    initial = float(input.gameWindow.getSize().x) - separation - width;
    posX = initial;
    posY = down - height - 1.f;
    sf::Vector2f posTreeMapLevelIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR, posTreeMapLevelIndicator);


    setScaleHudIndicator(Hud_Texture_Indicator::STAGE_INDICATOR,
                         sf::Vector2f(2.7f * input.screenScaleX, 2.7f * input.screenScaleX));

    posX = float(input.gameWindow.getSize().x) * 0.7f;;
    posY = down - instance.hudSprites[(int)Hud_Texture_Indicator::STAGE_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posStageIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::STAGE_INDICATOR, posStageIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) * 0.85f;
    posY =  down - 1.09f * float(instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].getCharacterSize());
    sf::Vector2f posLevelTextIndicator(posX, posY);
    setTextHudIndicator(Hud_Text_Indicator::LEVEL_TEXT, "1", posLevelTextIndicator);

    setScaleHudIndicator(Hud_Texture_Indicator::GEAR_INDICATOR,
                         sf::Vector2f(2.7f * input.screenScaleX, 2.7f * input.screenScaleX));

    posX = float(input.gameWindow.getSize().x) * 0.7f;
    posY = down - instance.hudSprites[(int)Hud_Texture_Indicator::GEAR_INDICATOR].getGlobalBounds().height * 2.8f;
    sf::Vector2f posGearIndicator(posX, posY);
    setPositionHudIndicator(Hud_Texture_Indicator::GEAR_INDICATOR, posGearIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) * 0.85f;
    posY =  down - 2.15f * float(instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].getCharacterSize());
    sf::Vector2f posGearTextIndicator(posX, posY);
    setTextHudIndicator(Hud_Text_Indicator::GEAR_TEXT, "1", posGearTextIndicator);

}

void Hud::drawHud(Input& input){
    /*
    for (int i = 0; i < (int)Hud_Texture_Indicator::__COUNT; i++) {
        input.gameWindow.draw(instance.hudSprites[i]);
    }
    for (int i = 0; i < (int)Hud_Text_Indicator::__COUNT; i++) {
        input.gameWindow.draw(instance.hudTexts[i]);
    }
    */
    for (int i = 0; i < 6; i++) {
        input.gameWindow.draw(instance.hudSprites[i]);
    }
    input.gameWindow.draw(instance.hudSprites[20]);
    input.gameWindow.draw(instance.hudSprites[21]);

    for (int i = 0; i < 6; i++) {
        input.gameWindow.draw(instance.hudTexts[i]);
    }
}

