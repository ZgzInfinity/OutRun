
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

#include "MenuControls.h"

MenuControls::MenuControls() : Menu()
{
    optionSelected = 0;
    controlSelected = false;
}

void MenuControls::loadMenu(Input& input){

    iconBackground.loadFromFile("Resources/Menus/OptionsMenu/icon.png");
    background = sf::IntRect(0, 0, input.gameWindow.getSize().x, input.gameWindow.getSize().y);

    sprite = sf::Sprite(iconBackground, background);
    sprite.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                    float(input.gameWindow.getSize().y) / SCREEN_0.second);

    textureShape.loadFromFile("Resources/Menus/OptionsMenu/outrun.png");
    textureShape.setRepeated(true);

    shape.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                      input.gameWindow.getSize().y / 2.f - 250.0f * input.screenScaleX);
    shape.setSize(sf::Vector2f(710.0f * input.screenScaleX, 500.0f * input.screenScaleX));
    shape.setOutlineColor(sf::Color(19, 186, 251));
    shape.setOutlineThickness(5.0f * input.screenScaleX);
    shape.setTexture(&textureShape, true);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    optionsText.setString("CONTROLS");
    optionsText.setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    optionsText.setFont(fontMenu);
    optionsText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    optionsText.setFillColor(sf::Color::Red);
    optionsText.setOutlineColor(sf::Color(12, 12, 12));
    optionsText.setOutlineThickness(3.0f * input.screenScaleX);
    optionsText.setPosition((input.gameWindow.getSize().x / 2.f) - optionsText.getLocalBounds().width / 2.f,
                             input.gameWindow.getSize().y / 2.f - 260.0f * input.screenScaleX);

    info.setString("PRESS ENTER TO SELECT A CONTROL AND PRESS A KEY TO CHANGE THE CONTROL");
    info.setFont(fontMenu);
    info.setFillColor(sf::Color(10, 201, 235));
    info.setOutlineColor(sf::Color(3, 39, 8));
    info.setCharacterSize(static_cast<unsigned int>(int(15.0f * input.screenScaleX)));
    info.setOutlineThickness(3.0f * input.screenScaleX);
    info.setStyle(sf::Text::Bold);
    info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                     input.gameWindow.getSize().y / 2.f - 185.0f * input.screenScaleX);


    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 150.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getActionName(Key::TURN_LEFT), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                             BUTTON_SELECTED_COLOR, ButtonState::BUTTON_HOVER, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 80.f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getActionName(Key::TURN_RIGHT), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                             BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 10.f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getActionName(Key::ACCELERATE), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                             BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 60.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getActionName(Key::BRAKE), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                             BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 130.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getActionName(Key::UP_GEAR), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                             BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 200.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getActionName(Key::DOWN_GEAR), BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR,
                             BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 150.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getKeyCodeName(input.get(Key::TURN_LEFT)), BUTTON_IDLE_COLOR,
                             BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_HOVER,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 80.f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getKeyCodeName(input.get(Key::TURN_RIGHT)), BUTTON_IDLE_COLOR,
                             BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 10.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getKeyCodeName(input.get(Key::ACCELERATE)), BUTTON_IDLE_COLOR,
                             BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 60.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getKeyCodeName(input.get(Key::BRAKE)), BUTTON_IDLE_COLOR,
                             BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 130.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getKeyCodeName(input.get(Key::UP_GEAR)), BUTTON_IDLE_COLOR,
                             BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,ButtonState::BUTTON_IDLE,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 200.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             input.getKeyCodeName(input.get(Key::DOWN_GEAR)), BUTTON_IDLE_COLOR,
                             BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR, ButtonState::BUTTON_IDLE,
                             input.screenScaleX);
}

void MenuControls::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escapePressed = true;
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code != sf::Keyboard::Unknown)
        {
            if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
            else if (controlSelected){
                if (input.equal(Key::TURN_LEFT, event.key.code) || input.equal(Key::TURN_RIGHT, event.key.code) ||
                    input.equal(Key::ACCELERATE, event.key.code) || input.equal(Key::BRAKE, event.key.code) ||
                    input.equal(Key::UP_GEAR, event.key.code) || input.equal(Key::DOWN_GEAR, event.key.code))
                {
                    Audio::play(Sfx::MENU_SELECTION_WRONG, false);
                }
                else {
                    switch (optionSelected){
                        case 0:
                            updateGameControl(input, event.key.code, Key::TURN_LEFT);
                            break;
                        case 1:
                            updateGameControl(input, event.key.code, Key::TURN_RIGHT);
                            break;
                        case 2:
                            updateGameControl(input, event.key.code, Key::ACCELERATE);
                            break;
                        case 3:
                            updateGameControl(input, event.key.code, Key::BRAKE);
                            break;
                        case 4:
                            updateGameControl(input, event.key.code, Key::UP_GEAR);
                            break;
                        case 5:
                            updateGameControl(input, event.key.code, Key::DOWN_GEAR);
                    }
                    input.modifiedinputig = true;
                }
            }
            else {
                if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP) && !controlSelected){
                    changeButtonSelected(true);
                }
                else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN) && !controlSelected){
                    changeButtonSelected(false);
                }
                else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                    if (!controlSelected)
                        Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                    else
                        Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

                    controlSelected = !controlSelected;
                    menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_SELECTED);
                    menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_SELECTED);
                }
            }
        }
    }
}


void MenuControls::draw(Input& input){
    // Until the start keyword is not pressed
    while (!escapePressed && !backPressed) {

        handleEvent(input);

        input.gameWindow.clear();
        input.gameWindow.draw(sprite);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(optionsText);
        input.gameWindow.draw(info);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons) {
            menuButton.render(&input.gameWindow);
        }

        input.gameWindow.display();
        sf::sleep(sf::milliseconds(120));
    }
}



State MenuControls::returnMenu(Input& input){
    if (backPressed){
        return State::OPTIONS;
    }
    else if (escapePressed){
        return State::EXIT;
    }
}



void MenuControls::changeButtonSelected(const bool& menuUpPressed){
    if (menuUpPressed){
        if (optionSelected != 0) {
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 7].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != int(menuButtons.size() - 1) / 2) {
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}


void MenuControls::updateGameControl(Input& input, const sf::Keyboard::Key& key, const Key& action){
    input.set(action, key);
    menuButtons[optionSelected + 6].setTextButton(input.getKeyCodeName(key));
    menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
    menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_HOVER);
    controlSelected = !controlSelected;
    Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
}
