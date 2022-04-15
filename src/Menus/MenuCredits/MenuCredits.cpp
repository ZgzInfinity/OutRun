
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

#include "MenuCredits.h"

MenuCredits::MenuCredits() : Menu(){
    offsetY = 700.f;
    offsetTitleText = -1.0f;
    offsetTimeLapTexts = 4.0f;
    mainTextArrived = false;
    lapTextsArrived = false;

    Credit::loadAll();
}


void MenuCredits::loadMenu(Input& input){

    // Loading the background texture
    backgroundMenu.loadFromFile("Resources/Menus/MainMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    for (int i = 2; i <= 7; i++) {
        // Loading the texture of the game's name
        gameIcon.loadFromFile("Resources/Menus/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    for (int i = 0; i < 6; i++) {
        // Loading the texture of the game's name
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 180.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleY);
        nameGame.setScale(2.0f * input.screenScaleX, 2.0f * input.screenScaleY);
        nameGames.push_back(nameGame);
    }

    shape.setSize(sf::Vector2f(720.0f * input.screenScaleX, 490.0f * input.screenScaleY));
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(5.0f * input.screenScaleX);
    shape.setFillColor(sf::Color(0, 0, 0, 200));

    // Main Text of the menu
    titleText.setString("CREDITS");
    titleText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    titleText.setFont(fontMenu);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3.0f * input.screenScaleX);

    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

    gameDetailText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    gameDetailText.setFont(fontMenu);
    gameDetailText.setFillColor(sf::Color::White);
    gameDetailText.setOutlineColor(sf::Color::Black);
    gameDetailText.setOutlineThickness(5.0f * input.screenScaleX);
    gameDetailText.setStyle(sf::Text::Bold);

    info.setCharacterSize(static_cast<unsigned int>(int(19.0f * input.screenScaleX)));
    info.setFont(fontMenu);
    info.setFillColor(sf::Color::White);
    info.setOutlineColor(sf::Color::Black);
    info.setOutlineThickness(5.0f * input.screenScaleX);
    info.setStyle(sf::Text::Bold);

    right.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    right.setFont(fontMenu);
    right.setFillColor(sf::Color::White);
    right.setOutlineColor(sf::Color::Black);
    right.setOutlineThickness(5.0f * input.screenScaleX);
    right.setStyle(sf::Text::Bold);
}


void MenuCredits::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            if (!escapePressed)
                escapePressed = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!startPressed){
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                Audio::stop(Soundtrack::CREDITS);
            }
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            if (!backPressed){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_BACK, false);
                Audio::stop(Soundtrack::CREDITS);
            }
        }
    }
}

void MenuCredits::draw(Input& input){

    int j = 0, k = 0;
    Audio::play(Sfx::MENU_PANEL_DISPLAY, false);
    Audio::play(Soundtrack::CREDITS, true);

    // Until start key is pressed
    while(!mainTextArrived && !lapTextsArrived){

        sf::Event event;
        input.gameWindow.pollEvent(event);

        // Creation of the panel rectangle of the menu
        shape.setPosition((input.gameWindow.getSize().x / 2.f) - 360.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);

        titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

        if (offsetTitleText < 1.f){
            offsetTitleText += 0.1;
        }
        else {
            mainTextArrived = true;
        }

        if (offsetTimeLapTexts > 1.1f){
            offsetTimeLapTexts -= 0.15;
        }
        else {
            lapTextsArrived = true;
        }

        if (offsetY > 250.f){
            offsetY -= 35.f;
        }

        // Show the press start title in the menu
        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(nameGames[j]);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(titleText);
        input.gameWindow.display();

        if (j < (int) nameGames.size() - 1){
            if (k == 10){
                j++;
                k = 0;
            }
            else {
                k++;
            }
        }
        else {
            j = 0;
        }

        sf::sleep(sf::milliseconds(50));
    }

    int numAnimation = 1;

    while (!startPressed && !escapePressed && !backPressed && numAnimation <= 7) {

        // Wait until the time pass displaying the info
        int i = 0;
        while (!startPressed && !escapePressed &&
               !backPressed && i <= TIME_CREDIT_SCREEN)
        {

            // Display the animation
            input.gameWindow.clear();
            input.gameWindow.draw(mainMenu);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(shape);

            switch(numAnimation){
                case 1:
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Game_Detail::__COUNT; j++){
                        gameDetailText.setString(Credit::getGameDetail(j));
                        gameDetailText.setPosition(input.gameWindow.getSize().x / 2.f - gameDetailText.getLocalBounds().width / 2.f,
                                                   input.gameWindow.getSize().y / 2.f - 140.f + (60.f * j) * input.screenScaleY);
                        input.gameWindow.draw(gameDetailText);
                    }
                    break;
                case 2:
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Game_Acknowlegment::__COUNT; j++){
                        right.setString(Credit::getGameAcknowlegment(j));
                        right.setPosition(input.gameWindow.getSize().x / 2.f - right.getLocalBounds().width / 2.f,
                                          input.gameWindow.getSize().y / 2.f - 120.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(right);
                    }
                    break;
                case 3:
                    titleText.setString("SOUNDTRACKS");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Soundtrack_Credit::__COUNT; j++){
                        info.setString(Credit::getSoundtrackCredit(j));
                        info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                                         input.gameWindow.getSize().y / 2.f - 125.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(info);

                        info.setString(Credit::getSoundtrackEnterpriseCredit(j));
                        info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                                         input.gameWindow.getSize().y / 2.f - 95.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(info);
                    }
                    break;
                case 4:
                    titleText.setString("SOUNDTRACKS");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Soundtrack_Credit_Two::__COUNT; j++){
                        info.setString(Credit::getSoundtrackCreditTwo(j));
                        info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                                         input.gameWindow.getSize().y / 2.f - 125.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(info);

                        info.setString(Credit::getSoundtrackEnterpriseCreditTwo(j));
                        info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                                         input.gameWindow.getSize().y / 2.f - 95.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(info);
                    }
                    break;
                case 5:
                    titleText.setString("SFX");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                            input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Soundtrack_Credit::__COUNT; j++){
                        info.setString(Credit::getSfxCredit(j));
                        info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                                         input.gameWindow.getSize().y / 2.f - 125.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(info);

                        info.setString(Credit::getSfxEnterpriseCredit(j));
                        info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                                         input.gameWindow.getSize().y / 2.f - 95.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(info);
                    }
                    break;
                case 6:
                    titleText.setString("SPRITES");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                            input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Sprite_Enterprise::__COUNT; j++){
                        right.setString(Credit::getSpriteEnterpriseCredit(j));
                        right.setPosition(input.gameWindow.getSize().x / 2.f - right.getLocalBounds().width / 2.f,
                                          input.gameWindow.getSize().y / 2.f - 70.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(right);
                    }
                    break;
                case 7:
                    titleText.setString("ACKNOWLEDGMENTS");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                            input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Acknowledgments::__COUNT; j++){
                        right.setString(Credit::getAcknowledgments(j));
                        right.setPosition(input.gameWindow.getSize().x / 2.f - right.getLocalBounds().width / 2.f,
                                          input.gameWindow.getSize().y / 2.f - 70.f + (120.f * j) * input.screenScaleY);
                        input.gameWindow.draw(right);
                    }
            }
            input.gameWindow.display();
            handleEvent(input);
            i++;
        }
        // Increment the animation to be displayed
        numAnimation++;
    }


    mainTextArrived = false;
    lapTextsArrived = false;

    // Until start key is pressed
    if (!escapePressed){

        Audio::play(Sfx::MENU_PANEL_HIDE, false);

        while(!escapePressed && !mainTextArrived && !lapTextsArrived){

            handleEvent(input);

            // Creation of the panel rectangle of the menu
            shape.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX, input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);

            titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                    input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

            if (offsetTitleText > -1.f){
                offsetTitleText -= 0.1;
            }
            else {
                mainTextArrived = true;
            }

            if (offsetTimeLapTexts < 4.0f){
                offsetTimeLapTexts += 0.15;
            }
            else {
                lapTextsArrived = true;
            }

            if (offsetY < 850.f){
                offsetY += 35.f;
            }

            if (j < (int) nameGames.size() - 1){
                if (k == 10){
                    j++;
                    k = 0;
                }
                else {
                    k++;
                }
            }
            else {
                j = 0;
            }

            // Show the press start title in the menu
            input.gameWindow.clear();
            input.gameWindow.draw(mainMenu);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(shape);
            input.gameWindow.draw(titleText);
            input.gameWindow.display();

            sf::sleep(sf::milliseconds(50));
        }
    }
    if (!escapePressed){
        Audio::stop(Soundtrack::CREDITS);
    }
}



State MenuCredits::returnMenu(Input& input){
    if (escapePressed) {
        return State::EXIT;
    }
    else {
        return State::START;
    }
}
