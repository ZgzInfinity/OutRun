
/*
 * Copyright (c) 2023 Ruben Rodriguez
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
 * Implementation file of the module MenuLoading
 */

#include "MenuLoading.h"



/**
 * Default constructor
 * @param _automaticMode controls if the player
 * has selected automatic transmission in order to show the controllers
 */
MenuLoading::MenuLoading(const bool& _automaticMode) : Menu()
{
    automaticMode = _automaticMode;
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuLoading::loadMenu(Input& input){

    // Prepare the background of the menu
    iconBackground.loadFromFile("Resources/Menus/OptionsMenu/icon.png");
    background = sf::IntRect(0, 0, input.gameWindow.getSize().x, input.gameWindow.getSize().y);

    sprite = sf::Sprite(iconBackground, background);
    sprite.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                    float(input.gameWindow.getSize().y) / SCREEN_0.second);

    // Set the main panel of the menu that contains the controllers
    panel.loadFromFile("Resources/Menus/OptionsMenu/outrun.png");
    panel.setRepeated(true);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    // Set the title of the menu
    controlsText.setString("CONTROLS");
    controlsText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    controlsText.setFont(fontMenu);
    controlsText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    controlsText.setFillColor(sf::Color::Red);
    controlsText.setOutlineColor(sf::Color(12, 12, 12));
    controlsText.setOutlineThickness(3.0f * input.screenScaleX);

    // Establish the positions of the menu components using the screen resolution
    if (input.currentIndexResolution <= 1){

        // Main panel wit the controllers
        mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 300.0f * input.screenScaleX,
                              input.gameWindow.getSize().y / 2.f - 250.0f * input.screenScaleX);

        mainPanel.setSize(sf::Vector2f(610.0f * input.screenScaleX, 450.0f * input.screenScaleX));

        // Title
        controlsText.setPosition(input.gameWindow.getSize().x / 2.f - 90.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 220.0f * input.screenScaleX);
    }
    else {

        // Main panel wit the controllers
        mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 300.0f * input.screenScaleX,
                              input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleX);

        mainPanel.setSize(sf::Vector2f(610.0f * input.screenScaleX, 390.0f * input.screenScaleX));

        // Title
        controlsText.setPosition(input.gameWindow.getSize().x / 2.f - 90.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 203.0f * input.screenScaleX);
    }

    // Set the position and number of points to be drawn depending on the screen size
    if (input.currentIndexResolution <= (int)Resolution::SCREEN_1 || input.currentIndexResolution == (int)Resolution::__COUNT){
        totalPoints = 25;
        offsetText = 235;
    }
    else {
        totalPoints = 35;
        offsetText = 205;
    }

    // Prepare the loading text indicator
    loadingText.setString("NOW LOADING");
    loadingText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    loadingText.setFont(fontMenu);
    loadingText.setStyle(sf::Text::Bold);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setOutlineColor(sf::Color::Black);
    loadingText.setOutlineThickness(5.0f * input.screenScaleX);
    loadingText.setPosition((input.gameWindow.getSize().x / 7.f) - loadingText.getLocalBounds().width / 2.f,
                            input.gameWindow.getSize().y / 2.f + offsetText * input.screenScaleX);

    // Configure the main panel
    mainPanel.setOutlineColor(sf::Color(19, 186, 251));
    mainPanel.setOutlineThickness(5.0f * input.screenScaleX);
    mainPanel.setTexture(&panel, true);

    // Fill the vector of points
    points = vector<string>(totalPoints, ".");

    if (automaticMode){

        /*
         * Set the buttons with the controllers used in automatic gear transmission
         */

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

        /*
         * Set the buttons with the controllers used in manual gear transmission
         */

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



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuLoading::handleEvent(Input& input){
    sf::Event event;
    // Detect the actions of the player
    input.gameWindow.pollEvent(event);
    if (input.closed(event))
        // Game closed
        escapePressed = true;
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuLoading::draw(Input& input){

    // Prepare the points counter and play the loading sound
    int i = 0;
    Audio::play(Soundtrack::LOADING, true);

    // Menu loading in course
    while (!escapePressed &&  i < totalPoints){

        // Detect the actions of the player
        handleEvent(input);

        // Draw the elements of the menu
        input.gameWindow.draw(sprite);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(controlsText);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons)
            menuButton.render(&input.gameWindow);

        // Draw the loading text indicator
        input.gameWindow.draw(loadingText);

        // Draw the loading points
        int j = 0;
        while (!escapePressed && j <= i){

            // Detect possible actions of the player
            handleEvent(input);

            // Draw the point (each time more close to the right side of the screen)
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

        // Check if the player has not closed the game
        if (!escapePressed){
            input.gameWindow.display();
            sleep(sf::milliseconds(450));
            i++;
        }
    }

    // Stop the loading sound
    Audio::stop(Soundtrack::LOADING);
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuLoading::returnMenu(Input& input){
    if (escapePressed)
        // Game closed
        return State::EXIT;
    else
        // Start the game round
        return State::PREPARE_ROUND;
}
