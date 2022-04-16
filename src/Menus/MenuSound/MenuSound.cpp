
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

#include "MenuSound.h"


MenuSound::MenuSound() : Menu()
{
    optionSelected = 0;
    controlSelected = false;
}

void MenuSound::loadMenu(Input& input){
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
    string volMusic = std::to_string(input.volumeMusic);
    string volSfx = std::to_string(input.volumeEffects);

    optionsText.setString("SOUND MENU");
    optionsText.setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    optionsText.setFont(fontMenu);
    optionsText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    optionsText.setFillColor(sf::Color::Red);
    optionsText.setOutlineColor(sf::Color(12, 12, 12));
    optionsText.setOutlineThickness(3.0f * input.screenScaleX);
    optionsText.setPosition((input.gameWindow.getSize().x / 2.f) - optionsText.getLocalBounds().width / 2.f,
                             input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleX);

    info.setString("PRESS ENTER TO SELECT A CONTROL AND PRESS A KEY TO CHANGE THE CONTROL");
    info.setFont(fontMenu);
    info.setFillColor(sf::Color(10, 201, 235));
    info.setOutlineColor(sf::Color(3, 39, 8));
    info.setCharacterSize(static_cast<unsigned int>(int(15.0f * input.screenScaleX)));
    info.setOutlineThickness(3.0f * input.screenScaleX);
    info.setStyle(sf::Text::Bold);
    info.setPosition(input.gameWindow.getSize().x / 2.f - info.getLocalBounds().width / 2.f,
                     input.gameWindow.getSize().y / 2.f - 125.0f * input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 40.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "MUSIC VOLUME", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_HOVER, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 30.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "EFFECTS VOLUME", BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 40.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             volMusic, BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_HOVER, input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 30.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             volSfx, BUTTON_IDLE_COLOR, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             ButtonState::BUTTON_IDLE,input.screenScaleX);
}


void MenuSound::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escapePressed = true;
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            backPressed = true;
        }
        else if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP) && !controlSelected){
            changeButtonSelected(true);
        }
        else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN) && !controlSelected){
            changeButtonSelected(false);
        }
        else if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT) && controlSelected){
            changeVolume(input, true);
        }
        else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT) && controlSelected){
            changeVolume(input, false);
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!controlSelected)
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            else
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

            controlSelected = !controlSelected;

            if (controlSelected){
                menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_SELECTED);
                menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_SELECTED);
            }
            else {
                menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
                menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_HOVER);
            }
        }
    }
}

void MenuSound::draw(Input& input){

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


State MenuSound::returnMenu(Input& input){
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
        return State::OPTIONS;
    }
    else if (escapePressed) {
        return State::EXIT;
    }
}


void MenuSound::changeButtonSelected(const bool& menuUpPressed){
    if (menuUpPressed){
        if (optionSelected != 0) {
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 3].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != 1) {
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}

void MenuSound::changeVolume(Input& input, const bool& menuLeft){
    float volumeMusicPct = Audio::getMusicVolume();
    float volumeSfxPct = Audio::getSfxVolume();
    int newVolMusic = 0, newVolSfx = 0;
    string volMusic, volSfx;
    if (menuLeft){
        switch (optionSelected) {
            case 0:
                volumeMusicPct = std::fmaxf(volumeMusicPct - 0.05f, 0.0f);
                Audio::setVolumeMusic(volumeMusicPct);
                volMusic = to_string((int)((Audio::getMusicVolume() + 0.005f) * 100));
                menuButtons[optionSelected + 2].setTextButton(volMusic);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                break;
            case 1:
                volumeSfxPct = std::fmaxf(volumeSfxPct - 0.05f, 0.0f);
                Audio::setVolumeSfx(volumeSfxPct);
                volSfx = to_string((int)((Audio::getSfxVolume() + 0.005f) * 100));
                menuButtons[optionSelected + 2].setTextButton(volSfx);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        switch (optionSelected) {
            case 0:
                volumeMusicPct = std::fminf(volumeMusicPct + 0.05f, 1.0f);
                Audio::setVolumeMusic(volumeMusicPct);
                newVolMusic = (int)((Audio::getMusicVolume() + 0.005f) * 100);
                volMusic = to_string(newVolMusic);
                menuButtons[optionSelected + 2].setTextButton(volMusic);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                break;
            case 1:
                volumeSfxPct = std::fminf(volumeSfxPct + 0.05f, 1.0f);
                Audio::setVolumeSfx(volumeSfxPct);
                newVolSfx = (int)((Audio::getSfxVolume() + 0.005f) * 100);
                volSfx = to_string(newVolSfx);
                menuButtons[optionSelected + 2].setTextButton(volSfx);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    input.volumeMusic = newVolMusic;
    input.volumeEffects = newVolSfx;
    input.modifiedinputig = true;
}

