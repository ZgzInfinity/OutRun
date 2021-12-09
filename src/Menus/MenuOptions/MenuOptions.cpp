
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

#include "MenuOptions.h"

MenuOptions::MenuOptions() : Menu()
{
    optionSelected = 0;
    resized = false;
    controlSelected = false;
}

MenuOptions::setPauseMode(const bool& pause){
    pauseMode = pause;
}

void MenuOptions::loadMenu(Input& input){
    menuButtons.clear();
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
                             input.gameWindow.getSize().y / 2.f - 130.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "DIFFICULTY", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             resized ? ButtonState::BUTTON_IDLE : ButtonState::BUTTON_HOVER,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 60.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "TRAFFIC", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 10.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "RESOLUTION", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             resized ? ButtonState::BUTTON_SELECTED : ButtonState::BUTTON_IDLE,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 80.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "SOUND", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 150.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "CONTROLS", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    // Option Inputs
    switch (input.difficulty) {
        case Level_Difficulty::EASY:
            difficulty = "EASY";
            break;
        case Level_Difficulty::HARD:
            difficulty = "HARD";
            break;
        default:
            difficulty = "NORMAL";
            break;
    }

    // Option Inputs
    switch (input.traffic) {
        case Level_Traffic::NONE:
            traffic = "EASY";
            break;
        case Level_Traffic::LOW:
            traffic = "HARD";
            break;
        case Level_Traffic::MEDIUM:
            traffic = "MEDIUM";
            break;
        default:
            traffic = "HIGH";
            break;
    }
    subMenu = "MENU", access = "PRESS ENTER", saved = "SAVED!";

    string resolution = input.currentIndexResolution == (int)Resolution::__COUNT ? "FULLSCREEN" :
        to_string(input.resolutions[input.currentIndexResolution].first) + "x" +
        to_string(input.resolutions[input.currentIndexResolution].second);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 130.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             difficulty, BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             resized ? ButtonState::BUTTON_IDLE : ButtonState::BUTTON_HOVER,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 60.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             traffic, BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 10.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             resolution, BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             resized ? ButtonState::BUTTON_SELECTED : ButtonState::BUTTON_IDLE,
                             input.screenScaleX);sf::Event event;

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 80.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             subMenu, BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 150.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             subMenu,BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);
}

void MenuOptions::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escapePressed = true;
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            if (!controlSelected){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                Audio::stop(Soundtrack::OPTIONS);
                if (!pauseMode)
                    Audio::play(Sfx::WIND, true);
            }
        }
        else if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP) && !controlSelected){
            changeButtonSelected(true);
        }
        else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN) && !controlSelected){
            changeButtonSelected(false);
        }
        else if ((input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT) && controlSelected) ||
                 (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT) && controlSelected))
        {
            switch (optionSelected) {
                case 0:
                    changeDifficulty(input, event);
                    break;
                case 1:
                    changeTrafficLevel(input, event);
                    break;
                case 2:
                    changeResolution(input, event);
                    loadMenu(input);
                    break;
            }
            input.modifiedinputig = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!controlSelected)
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            else
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

            controlSelected = !controlSelected;
            if (controlSelected){
                menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_SELECTED);
                menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_SELECTED);
            }
            else {
                menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
                menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_HOVER);
            }
            if (optionSelected > 2){
                startPressed = true;
            }
        }
    }
}

void MenuOptions::draw(Input& input){

    // Until the start keyword is not pressed
    while (!startPressed && !escapePressed && !backPressed) {

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



State MenuOptions::returnMenu(Input& input){
    if (startPressed){
        switch (optionSelected){
            case 3:
                return State::MUSIC_CONF;
                break;
            case 4:
                return State::CONTROLS;
                break;
        }
    }
    else if (backPressed){
        if (input.modifiedinputig){
            input.writeNewInput();
            Logger::setWidthScreen(input.gameWindow.getSize().x);
        }
        input.modifiedinputig = false;
        if (pauseMode){
            return State::PAUSE;
        }
        else {
            return State::GAME;
        }
    }
    else if (escapePressed) {
        return State::EXIT;
    }
}


void MenuOptions::changeButtonSelected(const bool& menuUpPressed){
    if (menuUpPressed){
        if (optionSelected != 0) {
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_IDLE);

            if (optionSelected == 2){
                menuButtons[optionSelected + 6].setTextButton(subMenu);
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != int(menuButtons.size() - 1) / 2) {
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 4].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    if (optionSelected > 2){
        switch (optionSelected){
            case 3:
                menuButtons[optionSelected + 5].setTextButton(access);
                menuButtons[optionSelected + 6].setTextButton(subMenu);
                break;
            case 4:
                menuButtons[optionSelected + 4].setTextButton(subMenu);
                menuButtons[optionSelected + 5].setTextButton(access);
                break;
        }
    }
}

void MenuOptions::changeDifficulty(Input& input, const sf::Event& event){
    if (event.key.code == input.get(Key::MENU_LEFT)){
        if (input.difficulty != Level_Difficulty::EASY) {
            if (input.difficulty == Level_Difficulty::NORMAL) {
                input.difficulty = Level_Difficulty::EASY;
                menuButtons[optionSelected + 5].setTextButton("EASY");
            }
            else if (input.difficulty == Level_Difficulty::HARD) {
                input.difficulty = Level_Difficulty::NORMAL;
                menuButtons[optionSelected + 5].setTextButton("NORMAL");
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else if (event.key.code == input.get(Key::MENU_RIGHT)){
        if (input.difficulty != Level_Difficulty::HARD) {
            if (input.difficulty == Level_Difficulty::EASY) {
                input.difficulty = Level_Difficulty::NORMAL;
                menuButtons[optionSelected + 5].setTextButton("NORMAL");
            }
            else if (input.difficulty == Level_Difficulty::NORMAL) {
                input.difficulty = Level_Difficulty::HARD;
                menuButtons[optionSelected + 5].setTextButton("HARD");
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}


void MenuOptions::changeTrafficLevel(Input& input, const sf::Event& event){
    if (event.key.code == input.get(Key::MENU_LEFT)){
        if (input.traffic != Level_Traffic::NONE) {
            if (input.traffic == Level_Traffic::LOW) {
                input.traffic = Level_Traffic::NONE;
                menuButtons[optionSelected + 5].setTextButton("NONE");
            }
            else if (input.traffic == Level_Traffic::MEDIUM) {
                input.traffic = Level_Traffic::LOW;
                menuButtons[optionSelected + 5].setTextButton("LOW");
            }
            else if (input.traffic == Level_Traffic::HIGH) {
                input.traffic = Level_Traffic::MEDIUM;
                menuButtons[optionSelected + 5].setTextButton("MEDIUM");
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else if (event.key.code == input.get(Key::MENU_RIGHT)){
        if (input.traffic != Level_Traffic::HIGH) {
            if (input.traffic == Level_Traffic::NONE) {
                input.traffic = Level_Traffic::LOW;
                menuButtons[optionSelected + 5].setTextButton("LOW");
            }
            else if (input.traffic == Level_Traffic::LOW) {
                input.traffic = Level_Traffic::MEDIUM;
                menuButtons[optionSelected + 5].setTextButton("MEDIUM");
            }
            else if (input.traffic == Level_Traffic::MEDIUM) {
                input.traffic = Level_Traffic::HIGH;
                menuButtons[optionSelected + 5].setTextButton("HIGH");
            }
        }
        Audio::play(Sfx::MENU_SELECTION_MOVE, false);
    }
}


void MenuOptions::changeResolution(Input& input, const sf::Event& event){
    if (event.key.code == input.get(Key::MENU_LEFT)){
        resized = true;
        if (input.currentIndexResolution > 0) {
            input.currentIndexResolution--;
            menuButtons[optionSelected + 5].setTextButton(to_string(input.resolutions[input.currentIndexResolution].first) + "x" +
                                                          to_string(input.resolutions[input.currentIndexResolution].second));

            // Modify the resolution of the screen
            input.gameWindow.create(sf::VideoMode(static_cast<unsigned int>(input.resolutions[input.currentIndexResolution].first),
                                                 static_cast<unsigned int>(input.resolutions[input.currentIndexResolution].second)), "Out Run",
                          sf::Style::Titlebar | sf::Style::Close);

            input.gameWindow.setFramerateLimit(FPS);
            input.gameWindow.setKeyRepeatEnabled(false);

            // Create a new screen with the new resolution
            input.gameWindow.setView(sf::View(sf::Vector2f(input.gameWindow.getSize().x / 2.0f, input.gameWindow.getSize().y / 2.0f),
                                             sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y)));

            input.screenScaleX = float(input.gameWindow.getSize().x) / float(SCREEN_0.first);
            input.screenScaleY = float(input.gameWindow.getSize().y) / float(SCREEN_0.second);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
            input.camD = (input.currentIndexResolution <= 1) ? 0.84 : 0.83;
        }
    }
    else if (event.key.code == input.get(Key::MENU_RIGHT)){
        if (input.currentIndexResolution < int(input.resolutions.size())) {
            resized = true;
            input.currentIndexResolution++;
            menuButtons[optionSelected + 5].setTextButton(input.currentIndexResolution < (int)Resolution::__COUNT ?
                                            to_string(input.resolutions[input.currentIndexResolution].first) + "x" +
                                            to_string(input.resolutions[input.currentIndexResolution].second) : "FULLSCREEN");

            // Control if there are more resolutions on the left
            if (input.currentIndexResolution < (int)Resolution::__COUNT) {
                input.gameWindow.create(sf::VideoMode(static_cast<unsigned int>(input.resolutions[input.currentIndexResolution].first),
                                                     static_cast<unsigned int>(input.resolutions[input.currentIndexResolution].second)),

                              "Out Run", sf::Style::Titlebar | sf::Style::Close);
            }
            else {

                input.gameWindow.create(sf::VideoMode(SCREEN_1.first, SCREEN_1.second), "Out Run", sf::Style::Fullscreen);
                input.currentIndexResolution = (int)Resolution::__COUNT;
            }

            input.gameWindow.setFramerateLimit(FPS);
            input.gameWindow.setKeyRepeatEnabled(false);

            input.gameWindow.setView(sf::View(sf::Vector2f(input.gameWindow.getSize().x / 2.0f, input.gameWindow.getSize().y / 2.0f),
                                             sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y)));

            // Calculation of the screen factor between the current resolution and the default resolution
            input.screenScaleX = float(input.gameWindow.getSize().x) / float(SCREEN_0.first);
            input.screenScaleY = float(input.gameWindow.getSize().y) / float(SCREEN_0.second);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
            input.camD = (input.currentIndexResolution <= 1) ? 0.84 : 0.83;
        }
    }
    if (pauseMode){
        HudRound::configureHudRound(input);
    }
}
