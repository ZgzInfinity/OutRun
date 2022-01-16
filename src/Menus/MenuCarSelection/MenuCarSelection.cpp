
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

#include "MenuCarSelection.h"

MenuCarSelection::MenuCarSelection() : Menu(){
    optionSelected = 0;
    colorCarSprite = 0;
    cursorLeftPressed = false;
    cursorRightPressed = false;
    factorResolution = 300.f;
}

void MenuCarSelection::loadMenu(Input& input){

    if (input.currentIndexResolution >= 2 && input.currentIndexResolution <= 3)
        factorResolution = 225.f;

    // Loading the background texture
    backgroundMenu.loadFromFile("Resources/Menus/CarSelectionMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");
    std::string path = "";

    sf::Texture playerCarTexture;
    for (int i = 0; i < TOTAL_PLAYER_CARS; i++){
        path = "Resources/Vehicles/PlayerCar/Ferraris/Ferrari" + std::to_string(i + 1) + "/c";
        for (int j = 0; j < TOTAL_COLOR_TEXTURES; j++){
            playerCarTexture.loadFromFile(path + std::to_string(121 + j) + ".png");
            vehiclePlayerTextures[i][j] = playerCarTexture;
        }
    }

    shape.setSize(sf::Vector2f(710.0f * input.screenScaleX, 490.0f * input.screenScaleY));
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(5.0f * input.screenScaleX);
    shape.setFillColor(sf::Color(0, 0, 0, 200));
    shape.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                      input.gameWindow.getSize().y / 2.f - 250.f * input.screenScaleY);

    // Main Text of the menu
    titleText.setString("CAR SELECTION MENU");
    titleText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    titleText.setFont(fontMenu);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3.0f * input.screenScaleX);
    titleText.setPosition((input.gameWindow.getSize().x / 2.f) - titleText.getLocalBounds().width / 2.f,
                          input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleY);

    carDescription.setString("PRESS LEFT OR RIGHT CURSOR KEYS TO CHANGE THE CAR");
    carDescription.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    carDescription.setFont(fontMenu);
    carDescription.setStyle(sf::Text::Bold | sf::Text::Underlined);
    carDescription.setFillColor(sf::Color::White);
    carDescription.setOutlineColor(sf::Color::Black);
    carDescription.setOutlineThickness(5.0f * input.screenScaleX);
    carDescription.setPosition(input.gameWindow.getSize().x / 2.f - carDescription.getLocalBounds().width / 2.f,
                               input.gameWindow.getSize().y / 2.f - 150.0f * input.screenScaleY);


    garageTexture.loadFromFile("Resources/Menus/CarSelectionMenu/Garage.png");
    garageTexture.setRepeated(true);

    shadowTexture.loadFromFile("Resources/Menus/CarSelectionMenu/Shadow.png");
    shadowTexture.setRepeated(true);
    shadow.setTexture(shadowTexture, true);
    shadow.setScale(2.75f * input.screenScaleX, 2.75f * input.screenScaleY);
    shadow.setPosition(input.gameWindow.getSize().x / 2.f - shadow.getLocalBounds().width / 2.f * shadow.getScale().x,
                       input.gameWindow.getSize().y / 2.f + 135.f * input.screenScaleY);

    vehicleShape.setTexture(&garageTexture, true);
    vehicleShape.setSize(sf::Vector2f(310.0f * input.screenScaleX, factorResolution * input.screenScaleX));
    vehicleShape.setOutlineColor(sf::Color(237, 160, 26));
    vehicleShape.setOutlineThickness(5.0f * input.screenScaleX);
    vehicleShape.setPosition((input.gameWindow.getSize().x / 2.f) - vehicleShape.getLocalBounds().width / 2.f + 5.f,
                             input.gameWindow.getSize().y / 2.f - 80.0f * input.screenScaleY);

    leftTriangle = sf::CircleShape(30 * input.screenScaleX, 3);
    leftTriangle.setFillColor(sf::Color::Red);
    leftTriangle.setOutlineColor(sf::Color::Black);
    leftTriangle.setOutlineThickness(2.0f * input.screenScaleX);
    leftTriangle.setRotation(30);
    leftTriangle.setPosition(input.gameWindow.getSize().x / 2.f - 200.f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f + 20.0f * input.screenScaleY);

    rightTriangle = sf::CircleShape(30 * input.screenScaleX, 3);
    rightTriangle.setFillColor(sf::Color::Green);
    rightTriangle.setOutlineColor(sf::Color::Black);
    rightTriangle.setOutlineThickness(2.0f * input.screenScaleX);
    rightTriangle.setRotation(90);
    rightTriangle.setPosition(input.gameWindow.getSize().x / 2.f + 220.0f * input.screenScaleX,
                              input.gameWindow.getSize().y / 2.f + 31.0f * input.screenScaleY);

    vehicleCar.setTexture(vehiclePlayerTextures[0][0], true);
    vehicleCar.setScale(1.6f * input.screenScaleX, 1.6f * input.screenScaleY);
    vehicleCar.setPosition(input.gameWindow.getSize().x / 2.f - vehicleCar.getLocalBounds().width / 2.f * vehicleCar.getScale().x,
                           input.gameWindow.getSize().y / 2.f + 90.f * input.screenScaleY);
}


void MenuCarSelection::handleEvent(Input& input){
    sf::Event event;
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            escapePressed = true;
        }
        else {
            if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT)){
                if (optionSelected != 0){
                    optionSelected--;
                    cursorLeftPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT)){
                if (optionSelected < TOTAL_PLAYER_CARS - 1){
                    optionSelected++;
                    cursorRightPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
            else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_BACK, false);
            }
        }
    }
}


void MenuCarSelection::draw(Input& input){

    Audio::play(Soundtrack::VEHICLE_SELECTION, true);

    Audio::play(Sfx::BLONDE_WOMAN_CHOOSE_CAR, false);


    while (!startPressed && !backPressed && !escapePressed){

        handleEvent(input);
        colorCarSprite = (colorCarSprite < TOTAL_COLOR_TEXTURES - 1) ? colorCarSprite + 1 : 0;
        vehicleCar.setTexture(vehiclePlayerTextures[optionSelected][colorCarSprite], true);

        switch(optionSelected){
            case 0:
                leftTriangle.setFillColor(sf::Color::Red);
                rightTriangle.setFillColor(sf::Color::Green);
                break;
            case TOTAL_PLAYER_CARS - 1:
                leftTriangle.setFillColor(sf::Color::Green);
                rightTriangle.setFillColor(sf::Color::Red);
                break;
            default:
                leftTriangle.setFillColor(sf::Color::Green);
                rightTriangle.setFillColor(sf::Color::Green);
        }

        if (cursorLeftPressed){
            leftTriangle.setFillColor(sf::Color::Yellow);
            rightTriangle.setFillColor(sf::Color::Green);
            cursorLeftPressed = false;
        }

        if (cursorRightPressed){
            rightTriangle.setFillColor(sf::Color::Yellow);
            leftTriangle.setFillColor(sf::Color::Green);
            cursorRightPressed = false;
        }

        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(shape);
        input.gameWindow.draw(titleText);
        input.gameWindow.draw(carDescription);
        input.gameWindow.draw(vehicleShape);
        input.gameWindow.draw(shadow);
        input.gameWindow.draw(vehicleCar);
        input.gameWindow.draw(leftTriangle);
        input.gameWindow.draw(rightTriangle);
        input.gameWindow.display();

        sf::sleep(sf::milliseconds(100));
    }
    Audio::stop(Soundtrack::VEHICLE_SELECTION);
    sf::sleep(Audio::getDurationSfx(Sfx::MENU_SELECTION_CONFIRM));
}

State MenuCarSelection::returnMenu(Input& input){
    if (escapePressed)
        return State::EXIT;
    else if (backPressed)
        return State::GEARS;
    else if (startPressed)
        return State::MUSIC;
}


int MenuCarSelection::hasPlayerCarSelected(){
    if (!backPressed)
        return optionSelected;
    else
        return -1;
}
