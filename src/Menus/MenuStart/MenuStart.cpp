
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

#include "MenuStart.h"

MenuStart::MenuStart() : Menu(){
    blink = true;
}


void MenuStart::setMenuStart(const bool& first, const bool& newRound){
    firstLoad = first;
    newGame = newRound;
}

bool MenuStart::getFirstLoad() const {
    return firstLoad;
}


void MenuStart::loadMenu(Input& input){

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

    textElements[0].setString("PRESS START");
    textElements[0].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    textElements[0].setFont(fontMenu);
    textElements[0].setFillColor(sf::Color::Green);
    textElements[0].setOutlineColor(sf::Color::Black);
    textElements[0].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[0].setPosition((input.gameWindow.getSize().x - textElements[0].getGlobalBounds().width) / 2.f,
                                input.gameWindow.getSize().y / 2.f + 50.0f * (input.gameWindow.getSize().y / SCREEN_0.second));

    textElements[1].setString("SELECT: ENTER KEY");
    textElements[1].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[1].setFont(fontMenu);
    textElements[1].setFillColor(sf::Color::Green);
    textElements[1].setOutlineColor(sf::Color::Black);
    textElements[1].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[1].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 130.0f * input.screenScaleY);

    textElements[2].setString("MOVE: ARROW KEYS");
    textElements[2].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[2].setFont(fontMenu);
    textElements[2].setFillColor(sf::Color::Green);
    textElements[2].setOutlineColor(sf::Color::Black);
    textElements[2].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[2].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 180.0f * input.screenScaleY);

    textElements[3].setString("BACK / PAUSE: ESC KEY");
    textElements[3].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[3].setFont(fontMenu);
    textElements[3].setFillColor(sf::Color::Green);
    textElements[3].setOutlineColor(sf::Color::Black);
    textElements[3].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[3].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 230.0f * input.screenScaleY);

    textElements[4].setString("SPACE: GAME CREDITS");
    textElements[4].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[4].setFont(fontMenu);
    textElements[4].setFillColor(sf::Color::Green);
    textElements[4].setOutlineColor(sf::Color::Black);
    textElements[4].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[4].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 280.0f * input.screenScaleY);


    textElements[5].setString("ZGZ INFINITGAMES");
    textElements[5].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[5].setFont(fontMenu);
    textElements[5].setFillColor(sf::Color::Green);
    textElements[5].setOutlineColor(sf::Color::Black);
    textElements[5].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[5].setPosition(input.gameWindow.getSize().x * 0.7f,
                                input.gameWindow.getSize().y / 2.f + 230.0f * input.screenScaleY);

    textElements[6].setString("2022");
    textElements[6].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[6].setFont(fontMenu);
    textElements[6].setFillColor(sf::Color::Green);
    textElements[6].setOutlineColor(sf::Color::Black);
    textElements[6].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[6].setPosition(input.gameWindow.getSize().x * 0.79f,
                                input.gameWindow.getSize().y / 2.f + 280.0f * input.screenScaleY);

    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

}

void MenuStart::handleEvent(Input& input){
    sf::Event event;
    input.gameWindow.pollEvent(event);
    if (input.closed(event)){
        if (!escapePressed)
            escapePressed = true;
    }
    else {
        if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!startPressed){
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                optionSelected = 0;
            }
        }
        else if (input.pressed(Key::MENU_CREDITS, event) && input.held(Key::MENU_CREDITS)){
            if (!startPressed){
                startPressed = true;
                optionSelected = 1;
                Audio::stop(Sfx::WIND);
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            }
        }
    }
}

void MenuStart::draw(Input& input){

    int j = 0, k = 0;
    sf::Time blink_delay = sf::seconds(1.0);
    blinkClcok.restart().asSeconds();
    elapsedBlink = blinkClcok.restart().asSeconds();

    if (!Audio::isPlaying(Sfx::WIND)){
        Audio::play(Sfx::WIND, true);
    }

    // Draw the landscape animation
    if (firstLoad || newGame){
        for (int i = 255; i >= 0; i -= 5){

            handleEvent(input);

            elapsedHide = blinkClcok.getElapsedTime().asSeconds();

            // Change the color of the main text
            if (elapsedHide - elapsedBlink >= blink_delay.asSeconds()) {
                blink = !blink;
                blinkClcok.restart();
            }
            if (blink) {
                textElements[0].setFillColor(sf::Color::Green);
                textElements[0].setOutlineColor(sf::Color::Black);
            }
            else {
                textElements[0].setFillColor(sf::Color::Transparent);
                textElements[0].setOutlineColor(sf::Color::Transparent);
            }

            // Show the press start title in the menu
            input.gameWindow.draw(mainMenu);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(textElements[0]);
            input.gameWindow.draw(textElements[1]);
            input.gameWindow.draw(textElements[2]);
            input.gameWindow.draw(textElements[3]);
            input.gameWindow.draw(textElements[4]);
            input.gameWindow.draw(textElements[5]);
            input.gameWindow.draw(textElements[6]);

            blackShape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(blackShape);
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

    // While the console input.gameWindow is opened
    while (!startPressed && !escapePressed) {

        handleEvent(input);
        elapsedHide = blinkClcok.getElapsedTime().asSeconds();

        // Change the color of the main text
        if (elapsedHide - elapsedBlink >= blink_delay.asSeconds()) {
            blink = !blink;
            blinkClcok.restart();
        }
        if (blink) {
            textElements[0].setFillColor(sf::Color::Green);
            textElements[0].setOutlineColor(sf::Color::Black);
        }
        else {
            textElements[0].setFillColor(sf::Color::Transparent);
            textElements[0].setOutlineColor(sf::Color::Transparent);
        }

        // Show the press start title in the menu
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(nameGames[j]);
        for (int index = 0; index < ELEMENTS; index++){
            input.gameWindow.draw(textElements[index]);
        }
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


State MenuStart::returnMenu(Input& input){
    if (startPressed){
        firstLoad = false;
        switch (optionSelected){
            case 0:
                return State::GAME;
                break;
            case 1:
                return State::CREDITS;
        }
    }
    else {
        return State::EXIT;
    }
}
