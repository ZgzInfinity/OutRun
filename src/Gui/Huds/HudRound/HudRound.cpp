
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
 * Implementation file of the module HudRound
 */

#include "HudRound.h"

// Declare the static instance of the class
HudRound HudRound::instance;



/**
 * Load a texture in the vector of textures of the hud
 * @param hudInd is the index position of the vector where the texture has to be stored
 * @param name is the path from where the texture has to be loaded
 */
void HudRound::loadHudRoundTextureIndicator(const Hud_Texture_Indicator hudInd , const std::string name){
    sf::Texture indicatorTexture;
    // Read the texture if the file exits and stores it in the vector
    if (indicatorTexture.loadFromFile(name)){
        instance.hudTextures[(int)hudInd] = indicatorTexture;
        instance.hudSprites[(int)hudInd].setTexture(instance.hudTextures[(int)hudInd], true);
    }
}



/**
 * Rescale a indicator of the hud (texture)
 * @param hudInd is the index of the indicator to be scaled in the screen
 * @param scale is a two components array that shows the new scale ratio of the new indicator
 */
void HudRound::setScaleHudRoundIndicator(const Hud_Texture_Indicator hudInd, const sf::Vector2f& scale){
    // Modify the scale of the indicator
    instance.hudSprites[(int)hudInd].setScale(scale.x, scale.y);
}



/**
 * Locate a indicator of the hud (texture) in a concrete position of the screen
 * @param hudInd is the index of the indicator to be located in the screen
 * @param pos is a two components array that shows where the indicator has to be located
 */
void HudRound::setPositionHudRoundIndicator(const Hud_Texture_Indicator hudInd, const sf::Vector2f& pos){
        // Assign a position to the indicator (texture)
    instance.hudSprites[(int)hudInd].setPosition(pos.x, pos.y);
}



/**
 * Locate a indicator of the hud (text) in a concrete position with a concrete text in the screen
 * @param hudInd is the index of the indicator to be located in the screen
 * @param message is the new content of the text indicator
 * @param pos is a two components array that shows where the indicator has to be located
 */
void HudRound::setTextHudRoundIndicator(const Hud_Text_Indicator hudInd, const std::string message, const sf::Vector2f& pos){
    // Set the position and the message of the text indicator
    instance.hudTexts[(int)hudInd].setPosition(pos.x, pos.y);
    instance.hudTexts[(int)hudInd].setString(message);
}



/**
 * Set a indicator of the hud (text) with a concrete text message
 * @param input is the module that has all the configuration of the game
 * @param message is a two components array that shows where the indicator has to be located
 */
void HudRound::setTextHudRoundIndicator(Input& input, const Hud_Text_Indicator hudInd, const std::string message){
    // Get the position of the text indicator
    sf::Vector2f posSpeedTextIndicator = instance.hudTexts[(int)hudInd].getPosition();

    // Check if the text indicator is the speed or the score
    if (hudInd == Hud_Text_Indicator::SPEED_TEXT || hudInd == Hud_Text_Indicator::SCORE_TEXT)
    {
        // Calculation of the new position (text width grows to the right and must be aligned
        float posX;
        if (hudInd == Hud_Text_Indicator::SPEED_TEXT)
            posX = float(input.gameWindow.getSize().x) / 6.0f;
        else
            posX = instance.hudSprites[(int)Hud_Texture_Indicator::SCORE_INDICATOR].getGlobalBounds().width * 6.2f;

        // Set the new content in the position
        instance.hudTexts[(int)hudInd].setString(message);
        instance.hudTexts[(int)hudInd].setPosition(posX - instance.hudTexts[(int)hudInd].getLocalBounds().width, posSpeedTextIndicator.y);
    }
    else
        // Set the new text content but the position is the same
        instance.hudTexts[(int)hudInd].setString(message);
}



/**
 * Default constructor
 */
HudRound::HudRound(){
    // Initialize all the information
    time = 0;
    score = 0;
    minutes = 0.f;
    secs = 0.f;
    cents_second = 0.f;
    minutesTrip = 0.f;
    secsTrip = 0.f;
    cents_secondTrip = 0.f;
    speed = 0.f;
    maxSpeed = 0.f;
    level = 1;
    speedHud = 0.0f;
    treeMapPos = LEVEL_FACTOR;
}




/**
 * Load all the checkpoint hud
 */
void HudRound::loadHudRound(){

    /*
     * Load all the hud round indicators
     */

    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TIME_INDICATOR, "Resources/Huds/HudRound/Time_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::SCORE_INDICATOR, "Resources/Huds/HudRound/Score_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::LAP_INDICATOR, "Resources/Huds/HudRound/Lap_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::SPEED_INDICATOR, "Resources/Huds/HudRound/Speed_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR, "Resources/Huds/HudRound/Speed_Motor_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::STAGE_INDICATOR, "Resources/Huds/HudRound/Stage_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::GEAR_INDICATOR, "Resources/Huds/HudRound/Gear_Indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR, "Resources/Huds/HudRound/Tree_level_1_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_2_INDICATOR, "Resources/Huds/HudRound/Tree_level_2_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_3_INDICATOR, "Resources/Huds/HudRound/Tree_level_3_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_4_INDICATOR, "Resources/Huds/HudRound/Tree_level_4_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_5_INDICATOR, "Resources/Huds/HudRound/Tree_level_5_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_6_INDICATOR, "Resources/Huds/HudRound/Tree_level_6_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_7_INDICATOR, "Resources/Huds/HudRound/Tree_level_7_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_8_INDICATOR, "Resources/Huds/HudRound/Tree_level_8_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_9_INDICATOR, "Resources/Huds/HudRound/Tree_level_9_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_10_INDICATOR, "Resources/Huds/HudRound/Tree_level_10_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_11_INDICATOR, "Resources/Huds/HudRound/Tree_level_11_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_12_INDICATOR, "Resources/Huds/HudRound/Tree_level_12_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_13_INDICATOR, "Resources/Huds/HudRound/Tree_level_13_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_14_INDICATOR, "Resources/Huds/HudRound/Tree_level_14_indicator.png");
    loadHudRoundTextureIndicator(Hud_Texture_Indicator::TREE_LEVEL_15_INDICATOR, "Resources/Huds/HudRound/Tree_level_15_indicator.png");

    // Set the text fonts to write the text indicators
    instance.hudIndicatorSpeed.loadFromFile("Resources/Fonts/digital.ttf");
    instance.hudIndicatorText.loadFromFile("Resources/Fonts/DisposableDroid.ttf");
}



/**
 * Set all the indicators that compose the round hud
 * @param input is the module that has all the configuration of the game
 */
void HudRound::setAllHudRoundIndicators(Input& input){

    // Set the time and score text indicators
    setTextHudRoundIndicator(input, Hud_Text_Indicator::TIME_TEXT, std::to_string(instance.time));
    setTextHudRoundIndicator(input, Hud_Text_Indicator::SCORE_TEXT, std::to_string(instance.score));

    // Compose the new time o the lap
    std::string lap;
    lap = (instance.minutes < 10.f) ? "0" + to_string(int(instance.minutes)) + " '" : to_string(int(instance.minutes)) + " '";
    lap += (instance.secs < 10.f) ? "0" + to_string(int(instance.secs)) + " ''" : to_string(int(instance.secs)) + " ''";
    lap += (int(instance.cents_second * 100.f) < 10 ) ? "0" + to_string(int(instance.cents_second * 100.f)) : to_string(int(instance.cents_second * 100.f));
    setTextHudRoundIndicator(input, Hud_Text_Indicator::LAP_TEXT, lap);

    // Set the speed and level text indicators
    setTextHudRoundIndicator(input, Hud_Text_Indicator::SPEED_TEXT, std::to_string((int)instance.speed));
    setTextHudRoundIndicator(input, Hud_Text_Indicator::LEVEL_TEXT, std::to_string(instance.level));

    // Set the gear indicator
    string playerGear = (instance.gear == 1) ? "H" : "L";
    setTextHudRoundIndicator(input, Hud_Text_Indicator::GEAR_TEXT, playerGear);

    // Establish the new appearance of the speed grey bar indicator
    int width = int(instance.speedHud * 120.f / instance.maxSpeed);
    if (width > 0){
        instance.speedMotorIndicator.loadFromFile("Resources/Huds/HudRound/Speed_Motor_Indicator.png",
                            sf::IntRect(0, 0, width, int(20.0f * input.screenScaleX)));
        instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR].setTexture(instance.speedMotorIndicator, true);
    }
    else
        instance.speedHud = 0.0f;

    // Draw the speed bar indicator
    instance.hudSprites[instance.treeMapPos].setTexture(instance.hudTextures[instance.treeMapPos], true);
}




/**
 * Set the content, scale and positions of all the indicator that compose the round hud
 * @param input is the module that has all the configuration of the game
 */
void HudRound::configureHudRound(Input& input){

    /*
     * Set the position, scale and content of all
     * indicators (texts and sprites) of the round hud
     */

    const float up = float(input.gameWindow.getSize().y) / 10.0f;
    float posX, posY;

    setScaleHudRoundIndicator(Hud_Texture_Indicator::TIME_INDICATOR,
                              sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    const float separation = instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR].getGlobalBounds().width / 3.f;
    posX = separation;
    posY = up - 1.1f * instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posTimeIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::TIME_INDICATOR, posTimeIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    float initial = separation + instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR].getGlobalBounds().width + separation;
    posX = initial;
    posY = up - 1.1f * float(instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT].getCharacterSize());
    sf::Vector2f posTimeTextIndicator(posX, posY);
    setTextHudRoundIndicator(Hud_Text_Indicator::TIME_TEXT, std::to_string(instance.time), posTimeTextIndicator);


    setScaleHudRoundIndicator(Hud_Texture_Indicator::SCORE_INDICATOR,
                              sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    posX = instance.hudSprites[(int)Hud_Texture_Indicator::SCORE_INDICATOR].getGlobalBounds().width * 3.f;
    posY = up - 1.1f * instance.hudSprites[(int)Hud_Texture_Indicator::SCORE_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posScoreIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::SCORE_INDICATOR, posScoreIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setFillColor(sf::Color(183, 164, 190));
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX *= 2.f;
    posY = up - 1.21f * float(instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT].getCharacterSize());
    sf::Vector2f posScoreTextIndicator(posX, posY);
    setTextHudRoundIndicator(Hud_Text_Indicator::SCORE_TEXT, std::to_string(instance.score), posScoreTextIndicator);

    setScaleHudRoundIndicator(Hud_Texture_Indicator::LAP_INDICATOR,
                              sf::Vector2f(2.5f * input.screenScaleX, 2.5f * input.screenScaleX));

    posX = instance.hudSprites[(int)Hud_Texture_Indicator::LAP_INDICATOR].getGlobalBounds().width * 9.2f;
    posY = up - 1.1f * instance.hudSprites[(int)Hud_Texture_Indicator::LAP_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posLapIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::LAP_INDICATOR, posLapIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) * 0.8f;
    posY = up - 1.21f * float(instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT].getCharacterSize());
    sf::Vector2f posLapTextIndicator(posX, posY);

    std::string lap;
    lap = (instance.minutes < 10.f) ? "0" + to_string(int(instance.minutes)) + " '" : to_string(int(instance.minutes)) + " '";
    lap += (instance.secs < 10.f) ? "0" + to_string(int(instance.secs)) + " ''" : to_string(int(instance.secs)) + " ''";
    lap += (int(instance.cents_second * 100.f) < 10) ? "0" + to_string(int(instance.cents_second * 100.f)) : to_string(int(instance.cents_second * 100.f));
    setTextHudRoundIndicator(Hud_Text_Indicator::LAP_TEXT, lap, posLapTextIndicator);


    setScaleHudRoundIndicator(Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR,
                              sf::Vector2f(2.f * input.screenScaleX, 1.5f * input.screenScaleX));

    const float lengthSpeed = instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR].getGlobalBounds().height;
    const float down = float(input.gameWindow.getSize().y) - lengthSpeed * 1.5f;
    initial = separation + instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR].getGlobalBounds().width;
    posX = separation;
    posY = float(input.gameWindow.getSize().y) - lengthSpeed * 1.25f;
    sf::Vector2f posSpeedMotorIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR, posSpeedMotorIndicator);

    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setFont(instance.hudIndicatorSpeed);
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setCharacterSize(static_cast<unsigned int>(int(70.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setFillColor(sf::Color(206, 73, 73));
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) / 7.8f;
    posY = down - 1.05f * float(instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT].getCharacterSize());

    sf::Vector2f posSpeedTextIndicator = sf::Vector2f(posX, posY);
    setTextHudRoundIndicator(Hud_Text_Indicator::SPEED_TEXT, std::to_string((int)instance.speed), posSpeedTextIndicator);
    setScaleHudRoundIndicator(Hud_Texture_Indicator::SPEED_INDICATOR,
                              sf::Vector2f(2.f * input.screenScaleX, 2.f * input.screenScaleX));

    posX = float(input.gameWindow.getSize().x) / 5.5f;
    posY = down - 1.05f * instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posSpeedIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::SPEED_INDICATOR, posSpeedIndicator);


    for (int i = (int)Hud_Texture_Indicator::TREE_LEVEL_1_INDICATOR; i <= (int)Hud_Texture_Indicator::TREE_LEVEL_15_INDICATOR; i++){

        setScaleHudRoundIndicator(static_cast<Hud_Texture_Indicator>(i),
                                  sf::Vector2f(4.f * input.screenScaleX, 4.f * input.screenScaleX));

        float width = instance.hudSprites[i].getGlobalBounds().width;
        float height = instance.hudSprites[i].getGlobalBounds().height;
        initial = float(input.gameWindow.getSize().x) - separation - width;
        posX = initial;
        posY = down - height - 1.f;
        sf::Vector2f posTreeMapLevelIndicator(posX, posY);
        setPositionHudRoundIndicator(static_cast<Hud_Texture_Indicator>(i), posTreeMapLevelIndicator);
    }



    setScaleHudRoundIndicator(Hud_Texture_Indicator::STAGE_INDICATOR,
                              sf::Vector2f(2.7f * input.screenScaleX, 2.7f * input.screenScaleX));

    posX = float(input.gameWindow.getSize().x) * 0.7f;;
    posY = down - instance.hudSprites[(int)Hud_Texture_Indicator::STAGE_INDICATOR].getGlobalBounds().height;
    sf::Vector2f posStageIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::STAGE_INDICATOR, posStageIndicator);


    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) * 0.85f;
    posY =  down - 1.06f * float(instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT].getCharacterSize());
    sf::Vector2f posLevelTextIndicator(posX, posY);
    setTextHudRoundIndicator(Hud_Text_Indicator::LEVEL_TEXT, std::to_string(instance.level), posLevelTextIndicator);

    setScaleHudRoundIndicator(Hud_Texture_Indicator::GEAR_INDICATOR,
                              sf::Vector2f(2.7f * input.screenScaleX, 2.7f * input.screenScaleX));

    posX = float(input.gameWindow.getSize().x) * 0.7f;
    posY = down - instance.hudSprites[(int)Hud_Texture_Indicator::GEAR_INDICATOR].getGlobalBounds().height * 2.8f;
    sf::Vector2f posGearIndicator(posX, posY);
    setPositionHudRoundIndicator(Hud_Texture_Indicator::GEAR_INDICATOR, posGearIndicator);

    string playerGear = (instance.gear == 1) ? "H" : "L";
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setFillColor(sf::Color(146, 194, 186));
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setOutlineColor(sf::Color::Black);
    instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].setOutlineThickness(3.0f * input.screenScaleX);
    posX = float(input.gameWindow.getSize().x) * 0.85f;
    posY =  down - 2.15f * float(instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT].getCharacterSize());
    sf::Vector2f posGearTextIndicator(posX, posY);
    setTextHudRoundIndicator(Hud_Text_Indicator::GEAR_TEXT, playerGear, posGearTextIndicator);

    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setFont(instance.hudIndicatorText);
    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setCharacterSize(static_cast<unsigned int>(int(60.0f * input.screenScaleX)));
    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setString("GAME OVER");
    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setFillColor(sf::Color::Yellow);
    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setOutlineColor(sf::Color(14, 29, 184));
    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setOutlineThickness(3.0f * input.screenScaleX);

    posX = float(input.gameWindow.getSize().x / 2.f -
                 instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].getGlobalBounds().width / 2.0f);
    posY = float(input.gameWindow.getSize().y / 2.f -
                 instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].getCharacterSize() / 2.0f);

    sf::Vector2f posGameOverTextIndicator(posX, posY);
    instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT].setPosition(posGameOverTextIndicator);
}



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
void HudRound::setHudRound(const int _time, const long long int _score, const float _minutes, const float _secs,
                 const float _cents_second, const int _level, const int _treeMapPos, const bool checkPoint,
                 const int _gear, const float _speed, const float _maxSpeed)
{
    // Update the content of the hud
    instance.time = _time;
    instance.score = _score;
    instance.minutes = _minutes;
    instance.secs = _secs;
    instance.cents_second = _cents_second;
    instance.level = _level;
    instance.gear = _gear;
    instance.speed = _speed * HUD_SPEED_FACTOR;
    instance.maxSpeed = _maxSpeed;
    instance.speedHud = _speed;

    // Check if the player has passed the checkpoint to change the tree map level position panel
    if (checkPoint)
        instance.treeMapPos = _treeMapPos;
}



/**
 * Draw the round hud in the screen
 * @param input is the module that has all the configuration of the game
 */
void HudRound::drawHudRound(Input& input){

    /*
     * Draw all the hud round indicators (text and sprites)
     */

    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::TIME_INDICATOR]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::SCORE_INDICATOR]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::LAP_INDICATOR]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_INDICATOR]);

    if (instance.speedHud > 0.0f)
        input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::SPEED_MOTOR_INDICATOR]);

    input.gameWindow.draw(instance.hudSprites[instance.treeMapPos]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::STAGE_INDICATOR]);
    input.gameWindow.draw(instance.hudSprites[(int)Hud_Texture_Indicator::GEAR_INDICATOR]);

    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::TIME_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::SCORE_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::LAP_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::SPEED_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::LEVEL_TEXT]);
    input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::GEAR_TEXT]);

    if (instance.time == 0)
        input.gameWindow.draw(instance.hudTexts[(int)Hud_Text_Indicator::GAME_OVER_TEXT]);
}

