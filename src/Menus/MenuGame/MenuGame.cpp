
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

#include "MenuGame.h"

MenuGame::MenuGame() : Menu(){}

void MenuGame::loadMenu(Input& input){

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

    // Loading the texture of the game's name
    rowSelector.loadFromFile("Resources/Menus/MainMenu/row.png");
    row.setTexture(rowSelector);
    row.setScale(0.06f * input.screenScaleX, 0.06f * input.screenScaleX);
    row.setPosition((input.gameWindow.getSize().x / 2.f) - 100.0f * input.screenScaleX,
                    input.gameWindow.getSize().y / 2.f + 75.0f * input.screenScaleX);

    textElements[0].setString("START");
    textElements[0].setPosition((input.gameWindow.getSize().x / 2.f) - 50.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 70.0f * input.screenScaleX);
    textElements[0].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[0].setFont(fontMenu);
    textElements[0].setFillColor(sf::Color::Green);
    textElements[0].setOutlineColor(sf::Color::Black);
    textElements[0].setOutlineThickness(3.0f * input.screenScaleX);

    textElements[1].setString("OPTIONS");
    textElements[1].setPosition(input.gameWindow.getSize().x / 2.f - 50.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 120.0f * input.screenScaleX);
    textElements[1].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[1].setFont(fontMenu);
    textElements[1].setFillColor(sf::Color::Green);
    textElements[1].setOutlineColor(sf::Color::Black);
    textElements[1].setOutlineThickness(3.0f * input.screenScaleX);
}

void MenuGame::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            if (!escapePressed)
                escapePressed = true;
        }
        else {
            if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP)){
                if (optionSelected != 0){
                    optionSelected = 0;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN)){
                if (optionSelected != 1){
                    optionSelected = 1;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                if (!startPressed){
                    startPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                }
            }
            else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                if (!backPressed){
                    backPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_BACK, false);
                }
            }
        }
    }
}

void MenuGame::draw(Input& input){

    // Partial state of the game
    int j = 0, k = 0;
    optionSelected = 0;

    if (!Audio::isPlaying(Sfx::WIND)){
        Audio::play(Sfx::WIND, true);
    }

    // While the console input.gameWindow is opened
    while (!startPressed && !escapePressed && !backPressed) {

        handleEvent(input);

        switch (optionSelected){
            case 0:
                row.setPosition((input.gameWindow.getSize().x / 2.f) - 100.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 75.0f * input.screenScaleX);
                break;
            case 1:
                row.setPosition((input.gameWindow.getSize().x / 2.f) - 100.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 125.0f * input.screenScaleX);
                break;
        }

        // Show the press start title in the menu
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(nameGames[j]);
        for (int i = 0; i < ELEMENTS; i++){
            input.gameWindow.draw(textElements[i]);
        }
        input.gameWindow.draw(row);
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
    }
}



State MenuGame::returnMenu(Input& input){
    if (startPressed){
        switch(optionSelected){
            case 0:
                return State::GEARS;
                break;
            case 1:
                Audio::stop(Sfx::WIND);
                Audio::play(Soundtrack::OPTIONS, true);
                return State::OPTIONS;
        }
    }
    else if (backPressed){
        return State::START;
    }
    else if (escapePressed){
        return State::EXIT;
    }
}
