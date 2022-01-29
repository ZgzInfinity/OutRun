
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

#include "MenuPause.h"

MenuPause::MenuPause(const Map& m, const PlayerCar& p, const vector<TrafficCar*> trafficCars, const int _terrain) : Menu()
{
    optionSelected = 0;
    map = m;
    player = p;
    cars = trafficCars;
    terrain = _terrain;
}


void MenuPause::loadMenu(Input& input){

    shape.setPosition(0, 0);
    shape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
    shape.setFillColor(sf::Color(0, 0, 0, 200));

    pauseShape.setPosition(input.gameWindow.getSize().x / 2.f - 120.0f * input.screenScaleX,
                           input.gameWindow.getSize().y / 2.f - 180.0f * input.screenScaleX);

    pauseShape.setSize(sf::Vector2f(250.0f * input.screenScaleX, 400.0f * input.screenScaleX));
    pauseShape.setFillColor(sf::Color(0, 0, 0));
    pauseShape.setOutlineColor(sf::Color::Green);
    pauseShape.setOutlineThickness(5.0f * input.screenScaleX);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    textMenu.setString("PAUSE");
    textMenu.setFont(fontMenu);
    textMenu.setFillColor(sf::Color(214, 234, 12));
    textMenu.setOutlineColor(sf::Color(12, 72, 234));
    textMenu.setOutlineThickness(2.0f * input.screenScaleX);
    textMenu.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    textMenu.setPosition(input.gameWindow.getSize().x / 2.f - textMenu.getLocalBounds().width / 2.f,
                         input.gameWindow.getSize().y / 2.f - 150.0f * input.screenScaleX);

    // Buttons of the menu
    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 95.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 70.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "RESUME", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_HOVER, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 95.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f, 200.0f * input.screenScaleX,
                             30.0f * input.screenScaleX, fontMenu, "OPTIONS",
                             BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 95.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 70.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "HOME", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 95.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 140.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "EXIT", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);
}

void MenuPause::handleEvent(Input& input){
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
            }
        }
        else if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP))
            changeButtonSelected(true);
        else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN))
            changeButtonSelected(false);
    }
}

void MenuPause::draw(Input& input){

    Audio::pause(input.currentSoundtrack);

    // Until the start keyword is not pressed
    while (!startPressed && !escapePressed) {

        handleEvent(input);

        input.gameWindow.clear();
        map.renderMap(input, cars, player, gameStatus, true);
        player.drawPlayRound(input, true, terrain, false);
        HudRound::drawHudRound(input);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(pauseShape);
        input.gameWindow.draw(textMenu);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons) {
            menuButton.render(&input.gameWindow);
        }

        input.gameWindow.display();
        sf::sleep(sf::milliseconds(120));
    }
    if (optionSelected == 2){
        for (int i = 0; i <= 70; i++){
            shape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(shape);
            input.gameWindow.display();
            sf::sleep(sf::milliseconds(10));
        }
    }
}



State MenuPause::returnMenu(Input& input){
    if (startPressed){
        switch(optionSelected){
            case 0:
                Audio::play(input.currentSoundtrack, true);
                return State::PLAY_ROUND;
                break;
            case 1:
                 Audio::play(Soundtrack::OPTIONS, true);
                return State::OPTIONS;
                break;
            case 2:
                return State::START;
                break;
            case 3:
                sf::sleep(sf::milliseconds(Audio::getDurationSfx(Sfx::MENU_SELECTION_CONFIRM).asMilliseconds()));
                return State::EXIT;
        }
    }
    else if (escapePressed)
        return State::EXIT;
}


void MenuPause::changeButtonSelected(const bool& menuUpPressed){
    if (menuUpPressed){
        if (optionSelected != 0){
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != int(menuButtons.size() - 1)){
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}

void MenuPause::setGameStatus(State& _gameStatus){
    gameStatus = _gameStatus;
}



