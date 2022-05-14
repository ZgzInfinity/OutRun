
/*
 * Copyright (c) 2022 Ruben Rodriguez
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



/*
 * Implementation file of the module MenuCarSelection
 */

#include "MenuCarSelection.h"



/**
 * Default constructor
 */
MenuCarSelection::MenuCarSelection() : Menu(){
    // Not car selected and original color displayed by default
    optionSelected = 0;
    colorCarSprite = 0;
    cursorLeftPressed = false;
    cursorRightPressed = false;
    factorResolution = 300.f;
}




/**
 * Returns if the player has selected a color car or not
 */
int MenuCarSelection::hasPlayerCarSelected(){
    if (!backPressed)
        // Color car selected
        return optionSelected;
    else
        // Color car not selected
        return -1;
}




/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuCarSelection::loadMenu(Input& input){

    // Set the car sprite size depending on the screen resolution
    if (input.currentIndexResolution >= 2 && input.currentIndexResolution <= 3)
        factorResolution = 225.f;

    // Loading the background
    backgroundMenu.loadFromFile("Resources/Menus/CarSelectionMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    // Load the font text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");
    std::string path = "";

    // Loas all the textures of the car in different colors
    sf::Texture playerCarTexture;
    for (int i = 0; i < TOTAL_PLAYER_CARS; i++){
        path = "Resources/Vehicles/PlayerCar/Ferraris/Ferrari" + std::to_string(i + 1) + "/c";
        for (int j = 0; j < TOTAL_COLOR_TEXTURES; j++){
            playerCarTexture.loadFromFile(path + std::to_string(121 + j) + ".png");
            vehiclePlayerTextures[i][j] = playerCarTexture;
        }
    }

    //Set the main panel of the menu
    mainPanel.setSize(sf::Vector2f(710.0f * input.screenScaleX, 500.0f * input.screenScaleY));
    mainPanel.setOutlineColor(sf::Color::Yellow);
    mainPanel.setOutlineThickness(5.0f * input.screenScaleX);
    mainPanel.setFillColor(sf::Color(0, 0, 0, 200));
    mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - 250.f * input.screenScaleY);

    // Prepare the title of the menu
    titleMenuText.setString("CAR SELECTION MENU");
    titleMenuText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    titleMenuText.setFont(fontMenu);
    titleMenuText.setStyle(sf::Text::Bold);
    titleMenuText.setFillColor(sf::Color::White);
    titleMenuText.setOutlineColor(sf::Color::Black);
    titleMenuText.setOutlineThickness(3.0f * input.screenScaleX);
    titleMenuText.setPosition((input.gameWindow.getSize().x / 2.f) - titleMenuText.getLocalBounds().width / 2.f,
                              input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleY);

    // Set the information indicator of the menu
    carDescriptionText.setString("PRESS LEFT OR RIGHT CURSOR KEYS TO CHANGE THE CAR");
    carDescriptionText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    carDescriptionText.setFont(fontMenu);
    carDescriptionText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    carDescriptionText.setFillColor(sf::Color::White);
    carDescriptionText.setOutlineColor(sf::Color::Black);
    carDescriptionText.setOutlineThickness(5.0f * input.screenScaleX);
    carDescriptionText.setPosition(input.gameWindow.getSize().x / 2.f - carDescriptionText.getLocalBounds().width / 2.f,
                                   input.gameWindow.getSize().y / 2.f - 150.0f * input.screenScaleY);

    // Set the garage texture
    garageTexture.loadFromFile("Resources/Menus/CarSelectionMenu/Garage.png");
    garageTexture.setRepeated(true);

    // Prepare the shadow texture projected by the car
    shadowTexture.loadFromFile("Resources/Menus/CarSelectionMenu/Shadow.png");
    shadowTexture.setRepeated(true);
    shadow.setTexture(shadowTexture, true);
    shadow.setScale(2.75f * input.screenScaleX, 2.75f * input.screenScaleY);
    shadow.setPosition(input.gameWindow.getSize().x / 2.f - shadow.getLocalBounds().width / 2.f * shadow.getScale().x,
                       input.gameWindow.getSize().y / 2.f + 135.f * input.screenScaleY);

    //Set the vehicle sub-panel
    vehiclePanel.setTexture(&garageTexture, true);
    vehiclePanel.setSize(sf::Vector2f(310.0f * input.screenScaleX, factorResolution * input.screenScaleX));
    vehiclePanel.setOutlineColor(sf::Color(237, 160, 26));
    vehiclePanel.setOutlineThickness(5.0f * input.screenScaleX);
    vehiclePanel.setPosition((input.gameWindow.getSize().x / 2.f) - vehiclePanel.getLocalBounds().width / 2.f + 5.f,
                             input.gameWindow.getSize().y / 2.f - 80.0f * input.screenScaleY);

    /*
     * Prepare the left and right selection triangle indicators
     */

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

    // Set the sprite of the car
    vehicleCar.setTexture(vehiclePlayerTextures[0][0], true);
    vehicleCar.setScale(1.6f * input.screenScaleX, 1.6f * input.screenScaleY);
    vehicleCar.setPosition(input.gameWindow.getSize().x / 2.f - vehicleCar.getLocalBounds().width / 2.f * vehicleCar.getScale().x,
                           input.gameWindow.getSize().y / 2.f + 90.f * input.screenScaleY);
}



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuCarSelection::handleEvent(Input& input){
    sf::Event event;
    // Detect the possible player actions
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            // Game window closed
            if (!escapePressed)
                escapePressed = true;
        }
        else {
            if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT)){
                // Player changes color to the left
                if (optionSelected != 0){
                    // Only available if the current colors is not the first one
                    optionSelected--;
                    cursorLeftPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT)){
                // Player changes color to the right
                if (optionSelected < TOTAL_PLAYER_CARS - 1){
                    // Only available if the current colors is not the last one
                    optionSelected++;
                    cursorRightPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                // Player selects car color
                if (!startPressed){
                    // Only once
                    startPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                }
            }
            else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                // PLayer cancels the color car selection
                if (!backPressed){
                    // Only once
                    backPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_BACK, false);
                }
            }
        }
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuCarSelection::draw(Input& input){

    // Play the music car selection and the choose car sfx
    Audio::play(Soundtrack::VEHICLE_SELECTION, true);
    Audio::play(Sfx::BLONDE_WOMAN_CHOOSE_CAR, false);

    // Car selection menu displayed
    while (!startPressed && !backPressed && !escapePressed){

        // Detect possible actions of the player
        handleEvent(input);

        // Draw the sprite of the car (spinning effect animation)
        colorCarSprite = (colorCarSprite < TOTAL_COLOR_TEXTURES - 1) ? colorCarSprite + 1 : 0;
        vehicleCar.setTexture(vehiclePlayerTextures[optionSelected][colorCarSprite], true);

        // Check the color car currently selected
        switch(optionSelected){
            case 0:
                // The first one is selected (left triangle is red)
                leftTriangle.setFillColor(sf::Color::Red);
                rightTriangle.setFillColor(sf::Color::Green);
                break;
            case TOTAL_PLAYER_CARS - 1:
                // The last one is selected (right triangle is red)
                leftTriangle.setFillColor(sf::Color::Green);
                rightTriangle.setFillColor(sf::Color::Red);
                break;
            default:
                // Intermediate option is selected
                leftTriangle.setFillColor(sf::Color::Green);
                rightTriangle.setFillColor(sf::Color::Green);
        }

        // Check move left action done
        if (cursorLeftPressed){
            // Set the triangle indicator color to yellow (pressing button effect)
            leftTriangle.setFillColor(sf::Color::Yellow);
            rightTriangle.setFillColor(sf::Color::Green);
            cursorLeftPressed = false;
        }

        // Check move right action done
        if (cursorRightPressed){
            // Set the triangle indicator color to yellow (pressing button effect)
            rightTriangle.setFillColor(sf::Color::Yellow);
            leftTriangle.setFillColor(sf::Color::Green);
            cursorRightPressed = false;
        }

        /*
         * Draw the menu with all the indicators
         */

        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(titleMenuText);
        input.gameWindow.draw(carDescriptionText);
        input.gameWindow.draw(vehiclePanel);
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



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuCarSelection::returnMenu(Input& input){
    if (escapePressed)
        // Game window closed
        return State::EXIT;
    else if (backPressed)
        // Goes back to the gears menu
        return State::GEARS;
    else if (startPressed)
        // Goes to the radio music selection menu
        return State::MUSIC;
}




