
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

    if (input.currentIndexResolution != 0){
        iconGame.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                          float(input.gameWindow.getSize().y) / SCREEN_0.second * 1.33f);
    }

    iconGame.setPosition((input.gameWindow.getSize().x - iconGame.getGlobalBounds().width) / 7.4f,
                         (input.gameWindow.getSize().y - iconGame.getGlobalBounds().height) / 2.0f);

    fontMenu.loadFromFile("Resources/Fonts/Bord.otf");

    logoZgz.setFont(fontMenu);
    logoZgz.setFillColor(sf::Color::White);
    logoZgz.setCharacterSize(static_cast<unsigned int>(int(120.0f * input.screenScaleX)));

    logoInfinityGames.setFont(fontMenu);
    logoInfinityGames.setFillColor(sf::Color::White);
    logoInfinityGames.setCharacterSize(static_cast<unsigned int>(int(65.0f * input.screenScaleX)));

    logoZgzCompleted.setString("zgz");
    logoZgzCompleted.setCharacterSize(static_cast<unsigned int>(int(65.0f * input.screenScaleX)));
    logoZgzCompleted.setFont(fontMenu);
    logoZgzCompleted.setFillColor(sf::Color::White);
    logoZgzCompleted.setPosition((input.gameWindow.getSize().x - logoZgzCompleted.getGlobalBounds().width) / 2.7f,
                                 input.gameWindow.getSize().y / 2.f - logoZgzCompleted.getGlobalBounds().height * 1.453f);

}

void MenuLogo::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escapePressed = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            startPressed = true;
            Audio::stop(Sfx::MENU_LOGO_DISPLAY);
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
        }
    }
}

void MenuLogo::draw(Input& input){

    Audio::play(Sfx::MENU_LOGO_DISPLAY, false);

    int i = 0;
    while (!startPressed && !escapePressed && i <= 300){

        handleEvent(input);

        if (i >= 160){
            logoZgz.setString("zgz");
        }
        else if (i >= 155){
            logoZgz.setString("zg");
        }
        else if (i >= 150){
            logoZgz.setString("z");
        }

        logoZgz.setPosition((input.gameWindow.getSize().x - logoZgzCompleted.getGlobalBounds().width) / 2.7f,
                            input.gameWindow.getSize().y / 2.f - logoZgz.getGlobalBounds().height * 1.453f);

        if (i >= 192){
            logoInfinityGames.setString("infinitgames");
        }
        else if (i >= 190){
            logoInfinityGames.setString("infinitgame");
        }
        else if (i >= 188){
            logoInfinityGames.setString("infinitgam");
        }
        else if (i >= 186){
            logoInfinityGames.setString("infinitga");
        }
        else if (i >= 184){
            logoInfinityGames.setString("infinitg");
        }
        else if (i >= 182){
            logoInfinityGames.setString("infinit");
        }
        else if (i >= 180){
            logoInfinityGames.setString("infini");
        }
        else if (i >= 178){
            logoInfinityGames.setString("infin");
        }
        else if (i >= 176){
            logoInfinityGames.setString("infi");
        }
        else if (i >= 174){
            logoInfinityGames.setString("inf");
        }
        else if (i >= 172){
            logoInfinityGames.setString("in");
        }
        else if (i >= 170){
            logoInfinityGames.setString("i");
        }

        logoInfinityGames.setPosition((input.gameWindow.getSize().x - logoZgzCompleted.getGlobalBounds().width) / 2.7f,
                                      input.gameWindow.getSize().y / 2.f + logoInfinityGames.getGlobalBounds().height * 0.21f);

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
