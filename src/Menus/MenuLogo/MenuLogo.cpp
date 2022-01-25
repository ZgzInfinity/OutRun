
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

#include "MenuLogo.h"

MenuLogo::MenuLogo() : Menu(){}

void MenuLogo::loadMenu(Input& input){

    backgroundMenu.setPosition(0, 0);
    backgroundMenu.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
    backgroundMenu.setFillColor(sf::Color::Black);

    logoGame.loadFromFile("Resources/Intro/Icon.png");
    iconGame.setTexture(logoGame, true);

    if (input.currentIndexResolution == (int)Resolution::SCREEN_0){
        iconGame.setScale(1.6f, 1.6f);
        offsetY = 2.0f;
        offsetY2 = 1.853f;
    }
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_1 ||
             input.currentIndexResolution == (int)Resolution::__COUNT)
    {
        iconGame.setScale(1.74f, 1.74f);
        offsetY = 2.0f;
        offsetY2 = 1.857f;
    }
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_2){
        iconGame.setScale(2.27f, 2.27f);
        offsetY = 1.98f;
        offsetY2 = 1.873f;
    }
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_3){
        iconGame.setScale(2.39f, 2.39f);
        offsetY = 1.98f;
        offsetY2 = 1.863f;
    }


    iconGame.setPosition((input.gameWindow.getSize().x - iconGame.getGlobalBounds().width) / 4.5f,
                         (input.gameWindow.getSize().y - iconGame.getGlobalBounds().height) / offsetY);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    logoZgz.setFont(fontMenu);
    logoZgz.setFillColor(sf::Color::White);
    logoZgz.setCharacterSize(static_cast<unsigned int>(int(140.0f * input.screenScaleX)));

    logoInfinityGames.setFont(fontMenu);
    logoInfinityGames.setFillColor(sf::Color::White);
    logoInfinityGames.setCharacterSize(static_cast<unsigned int>(int(80.0f * input.screenScaleX)));

    logoZgzCompleted.setString("ZGZ");
    logoZgzCompleted.setCharacterSize(static_cast<unsigned int>(int(80.0f * input.screenScaleX)));
    logoZgzCompleted.setFont(fontMenu);
    logoZgzCompleted.setFillColor(sf::Color::White);
    logoZgzCompleted.setPosition((input.gameWindow.getSize().x - logoZgzCompleted.getGlobalBounds().width) / 2.7f,
                                 input.gameWindow.getSize().y / 2.f - logoZgzCompleted.getGlobalBounds().height * offsetY2);

}

void MenuLogo::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            if (!escapePressed)
                escapePressed = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!startPressed){
                startPressed = true;
                Audio::stop(Sfx::MENU_LOGO_DISPLAY);
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            }
        }
    }
}

void MenuLogo::draw(Input& input){

    Audio::play(Sfx::MENU_LOGO_DISPLAY, false);

    int i = 0;
    while (!startPressed && !escapePressed && i <= 300){

        handleEvent(input);

        if (i >= 160){
            logoZgz.setString("ZGZ");
        }
        else if (i >= 155){
            logoZgz.setString("ZG");
        }
        else if (i >= 150){
            logoZgz.setString("Z");
        }

        logoZgz.setPosition((input.gameWindow.getSize().x - logoZgzCompleted.getGlobalBounds().width) / 2.3f,
                            input.gameWindow.getSize().y / 2.f - logoZgz.getGlobalBounds().height * offsetY2);

        if (i >= 192){
            logoInfinityGames.setString("INFINITGAMES");
        }
        else if (i >= 190){
            logoInfinityGames.setString("INFINITGAME");
        }
        else if (i >= 188){
            logoInfinityGames.setString("INFINITGAM");
        }
        else if (i >= 186){
            logoInfinityGames.setString("INFINITGA");
        }
        else if (i >= 184){
            logoInfinityGames.setString("INFINITG");
        }
        else if (i >= 182){
            logoInfinityGames.setString("INFINIT");
        }
        else if (i >= 180){
            logoInfinityGames.setString("INFINI");
        }
        else if (i >= 178){
            logoInfinityGames.setString("INFIN");
        }
        else if (i >= 176){
            logoInfinityGames.setString("INFI");
        }
        else if (i >= 174){
            logoInfinityGames.setString("INF");
        }
        else if (i >= 172){
            logoInfinityGames.setString("IN");
        }
        else if (i >= 170){
            logoInfinityGames.setString("I");
        }

        logoInfinityGames.setPosition((input.gameWindow.getSize().x - logoZgzCompleted.getGlobalBounds().width) / 2.3f,
                                      input.gameWindow.getSize().y / 2.09f + logoInfinityGames.getGlobalBounds().height * 0.21f);

        input.gameWindow.clear();

        // Draw the logo of the game
        if (i >= 40){
            input.gameWindow.draw(iconGame);
        }
        if (i >= 60){
            input.gameWindow.draw(logoZgz);
        }
        if (i >= 170){
            input.gameWindow.draw(logoInfinityGames);
        }

        input.gameWindow.display();
        i++;
    }

    backgroundMenu.setPosition(0, 0);
    backgroundMenu.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

    // Draw the landscape animation
    int j = 0;
    while (!startPressed && !escapePressed && j <= 255){

        handleEvent(input);

        backgroundMenu.setFillColor(sf::Color(0, 0, 0, j));

        input.gameWindow.clear();
        input.gameWindow.draw(iconGame);
        input.gameWindow.draw(logoZgz);
        input.gameWindow.draw(logoInfinityGames);
        input.gameWindow.draw(backgroundMenu);
        input.gameWindow.display();

        j += 5;
    }
}


State MenuLogo::returnMenu(Input& input){
    if (startPressed || (!startPressed && !escapePressed)){
        return State::START;
    }
    else {
        return State::EXIT;
    }
}
