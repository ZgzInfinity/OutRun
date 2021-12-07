
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

#include "MenuLoading.h"


MenuLoading::MenuLoading(const bool& _automaticMode) : Menu()
{
    automaticMode = _automaticMode;
}


void MenuLoading::loadMenu(Input& input){

    iconBackground.loadFromFile("Resources/Menus/MenuOptions/icon.png");
    background = sf::IntRect(0, 0, input.gameWindow.getSize().x, input.gameWindow.getSize().y);

    sprite = sf::Sprite(iconBackground, background);
    sprite.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                    float(input.gameWindow.getSize().y) / SCREEN_0.second);

    textureShape.loadFromFile("Resources/Menus/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    controlsText.setString("CONTROLS");
    controlsText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    controlsText.setFont(fontMenu);
    controlsText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    controlsText.setFillColor(sf::Color::Red);
    controlsText.setOutlineColor(sf::Color(12, 12, 12));
    controlsText.setOutlineThickness(3.0f * input.screenScaleX);

    // Establish the dimensions depending on if the game is played manually or automatically
    if (input.currentIndexResolution <= 1){
        shape.setPosition((input.gameWindow.getSize().x / 2.f) - 300.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - 250.0f * input.screenScaleX);

        shape.setSize(sf::Vector2f(610.0f * input.screenScaleX, 450.0f * input.screenScaleX));

        controlsText.setPosition(input.gameWindow.getSize().x / 2.f - 90.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 220.0f * input.screenScaleX);
    }
    else {
        shape.setPosition((input.gameWindow.getSize().x / 2.f) - 300.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleX);

        shape.setSize(sf::Vector2f(610.0f * input.screenScaleX, 390.0f * input.screenScaleX));

        controlsText.setPosition(input.gameWindow.getSize().x / 2.f - 90.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 203.0f * input.screenScaleX);
    }

    if (input.currentIndexResolution <= (int)Resolution::SCREEN_1 || input.currentIndexResolution == (int)Resolution::__COUNT){
        totalPoints = 25;
        offsetText = 235;
    }
    else {
        totalPoints = 35;
        offsetText = 205;
    }

    // Menu text
    loadingText.setString("NOW LOADING");
    loadingText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    loadingText.setFont(fontMenu);
    loadingText.setStyle(sf::Text::Bold);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setOutlineColor(sf::Color::Black);
    loadingText.setOutlineThickness(5.0f * input.screenScaleX);
    loadingText.setPosition((input.gameWindow.getSize().x / 7.f) - loadingText.getLocalBounds().width / 2.f,
                            input.gameWindow.getSize().y / 2.f + offsetText * input.screenScaleX);

    shape.setOutlineColor(sf::Color(19, 186, 251));
    shape.setOutlineThickness(5.0f * input.screenScaleX);
    shape.setTexture(&textureShape, true);

    points = vector<string>(totalPoints, ".");

    if (automaticMode){
        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 130.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::TURN_LEFT), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 50.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::TURN_RIGHT), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 30.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::ACCELERATE), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 110.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::BRAKE), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 130.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::TURN_LEFT)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 50.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::TURN_RIGHT)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 30.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::ACCELERATE)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 110.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::BRAKE)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);
    }
    else {
        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 135.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::TURN_LEFT), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 80.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::TURN_RIGHT), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 25.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::ACCELERATE), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 30.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::BRAKE), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 85.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::UP_GEAR), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 140.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getActionName(Key::DOWN_GEAR), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                                 BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 135.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::TURN_LEFT)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 80.f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::TURN_RIGHT)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 25.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::ACCELERATE)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 30.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::BRAKE)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 85.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::UP_GEAR)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);

        menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f + 140.0f * input.screenScaleX,
                                 200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                                 input.getKeyCodeName(input.get(Key::DOWN_GEAR)), BUTTON_IDLE_COLOR,
                                 BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                                 input.screenScaleX);
    }
}

void MenuLoading::handleEvent(Input& input){
    sf::Event event;
    input.gameWindow.pollEvent(event);
    if (input.closed(event)){
        escapePressed = true;
    }
}

void MenuLoading::draw(Input& input){

    int i = 0;
    Audio::play(Soundtrack::LOADING, true);

    while (!escapePressed &&  i < totalPoints){

        handleEvent(input);

        // Draw the elements of the menu
        input.gameWindow.draw(sprite);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(controlsText);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons) {
            menuButton.render(&input.gameWindow);
        }

        input.gameWindow.draw(loadingText);

        // Draw the loading points
        int j = 0;
        while (!escapePressed && j <= i){

            handleEvent(input);

            pointText.setString(points[j]);
            pointText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
            pointText.setFont(fontMenu);
            pointText.setStyle(sf::Text::Bold);
            pointText.setFillColor(sf::Color::White);
            pointText.setOutlineColor(sf::Color::Black);
            pointText.setOutlineThickness(5.0f * input.screenScaleX);
            pointText.setPosition((input.gameWindow.getSize().x / 7.f) + loadingText.getLocalBounds().width / 2.f + 25.f * (j + 1),
                                   input.gameWindow.getSize().y / 2.f + offsetText * input.screenScaleX);

            input.gameWindow.draw(pointText);
            j++;
        }

        if (!escapePressed){
            input.gameWindow.display();
            sleep(sf::milliseconds(450));
            i++;
        }
    }
    Audio::stop(Soundtrack::LOADING);
}


State MenuLoading::returnMenu(Input& input){
    if (escapePressed){
        return State::EXIT;
    }
    else {
        return State::PREPARE_ROUND;
    }
}
