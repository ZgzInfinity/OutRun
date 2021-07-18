
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

#include "MenuGears.h"

MenuGears::MenuGears() : Menu(){
    offsetY = 700.f;
    offsetTitleText = -1.0f;
    offsetTimeLapTexts = 4.0f;
    mainTextArrived = false;
    lapTextsArrived = false;
    startPressed = false;
    automaticMode = true;
    pressedKey = false;
    escapePressed = false;
}

void MenuGears::loadMenu(Input& input){

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

    for (int i = 1; i <= 4; i++) {
        // Loading the texture of the game's name
        gameIcon.loadFromFile("Resources/Menus/GearMenu/Gear" + to_string(i) + ".png");
        gearIcons.push_back(gameIcon);
    }

    for (int i = 0; i <= 3; i++) {
        // Loading the texture of the game's name
        nameGame.setTexture(gearIcons[i], true);
        nameGame.setScale(1.5f * input.screenScaleX, 1.5f * input.screenScaleY);
        if (i < 2){
            nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 280.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 65.0f * input.screenScaleY);
        }
        else {
            nameGame.setPosition((input.gameWindow.getSize().x / 2.f) + 50.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 65.0f * input.screenScaleY);
        }
        gears.push_back(nameGame);
    }

    shape.setSize(sf::Vector2f(710.0f * input.screenScaleX, 490.0f * input.screenScaleY));
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(5.0f * input.screenScaleX);
    shape.setFillColor(sf::Color(0, 0, 0, 200));

    // Main Text of the menu
    titleText.setString("GEARS MENU");
    titleText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    titleText.setFont(fontMenu);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3.0f * input.screenScaleX);

     // Main Text of the menu
    totalTime.setString("CHOOSE TRANSMISSION");
    totalTime.setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    totalTime.setFont(fontMenu);
    totalTime.setStyle(sf::Text::Bold);
    totalTime.setFillColor(sf::Color(64, 147, 225));
    totalTime.setOutlineColor(sf::Color::Black);
    totalTime.setOutlineThickness(3.0f * input.screenScaleX);

    // Creation of the panel rectangle of the menu
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
}


void MenuGears::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escapePressed = true;
        }
        else {
            if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT)){
                if (optionSelected != 0){
                    pressedKey = true;
                    automaticMode = true;
                    optionSelected = 0;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT)){
                if (optionSelected != 1){
                    pressedKey = true;
                    automaticMode = false;
                    optionSelected = 1;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                startPressed = true;
                Audio::stop(Sfx::WIND);
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
            else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_BACK, false);
            }
        }
    }
}


void MenuGears::draw(Input& input){

    int j = 0, k = 0;
    Audio::play(Sfx::MENU_PANEL_DISPLAY, false);

    if (!Audio::isPlaying(Sfx::WIND)){
        Audio::play(Sfx::WIND, true);
    }

    // Until start key is pressed
    while(!mainTextArrived && !lapTextsArrived)
    {

        sf::Event event;
        input.gameWindow.pollEvent(event);

        // Creation of the panel rectangle of the menu
        shape.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);


        titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleY);

        totalTime.setPosition(((input.gameWindow.getSize().x / 2.f) - (totalTime.getLocalBounds().width / 2.f)) * offsetTimeLapTexts,
                                input.gameWindow.getSize().y / 2.f - 140.0f * input.screenScaleY);

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
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(nameGames[j]);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(titleText);
        input.gameWindow.draw(totalTime);
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

    optionSelected = 0;

    // While the start key is not pressed
    while (!startPressed && !backPressed && !escapePressed){

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

        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(nameGames[j]);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(titleText);
        input.gameWindow.draw(totalTime);

        // Default Input
        if (!pressedKey || optionSelected == 0){
            input.gameWindow.draw(gears[optionSelected]);
            input.gameWindow.draw(gears[optionSelected + 3]);
        }
        else if (optionSelected == 1){
            input.gameWindow.draw(gears[optionSelected]);
            input.gameWindow.draw(gears[optionSelected + 1]);
        }

        input.gameWindow.display();
        sf::sleep(sf::milliseconds(50));
        handleEvent(input);
    }

    mainTextArrived = false;
    lapTextsArrived = false;

    // Until start key is pressed
    if (!escapePressed){
        Audio::play(Sfx::MENU_PANEL_HIDE, false);

        while(!escapePressed && !mainTextArrived && !lapTextsArrived){

            // Creation of the panel rectangle of the menu
            shape.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                              input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);

            titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                    input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleY);

            totalTime.setPosition(((input.gameWindow.getSize().x / 2.f) - (totalTime.getLocalBounds().width / 2.f)) * offsetTimeLapTexts,
                                    input.gameWindow.getSize().y / 2.f - 140.0f * input.screenScaleY);

            // Detect the possible events
            handleEvent(input);

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
            input.gameWindow.draw(totalTime);
            input.gameWindow.draw(titleText);
            input.gameWindow.display();

            sf::sleep(sf::milliseconds(50));
            handleEvent(input);
        }

        if (!escapePressed && !backPressed){
            for (int i = 0; i <= 70; i++){
                blackShape.setFillColor(sf::Color(0, 0, 0, i));
                input.gameWindow.draw(blackShape);
                input.gameWindow.display();
                sf::sleep(sf::milliseconds(20));
            }
        }
    }
}


State MenuGears::returnMenu(Input& input){
    if (escapePressed){
        return State::EXIT;
    }
    else if (backPressed){
        return State::GAME;
    }
    else {
        return State::MUSIC;
    }
}


bool MenuGears::getAutomaticMode() const {
    return automaticMode;
}


