
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
 * Implementation file of the module MenuControl
 */

#include "MenuControls.h"



/**
 * Default constructor
 */
MenuControls::MenuControls() : Menu(){
    // By default first controller hovered and anyone is selected
    optionSelected = 0;
    controlSelected = false;
}



/**
 * Change the option menu selected
 * @param menuUpPressed controls if the menu_up key has been or not pressed
 */
void MenuControls::changeButtonSelected(const bool menuUpPressed){
    // Menu up pressed
    if (menuUpPressed){
        if (optionSelected != 0){
            // Hover the lower controller option if it is not the first one
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 7].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != int(menuButtons.size() - 1) / 2){
            // Hover the upper controller option if it is not the last one
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}



/**
 * Update a game controller key of the game
 * @param input is the module that has all the configuration of the game
 * @param key is the new keyword entered by the player to be assigned to a game controller
 * @param action is the action whose controller has been changed by the player
 */
void MenuControls::updateGameControl(Input& input, const sf::Keyboard::Key& key, const Key& action){
    // Seth the new key
    input.set(action, key);

    // Replace the key name code associated to the new action controller
    menuButtons[optionSelected + 6].setTextButton(input.getKeyCodeName(key));
    menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
    menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_HOVER);
    controlSelected = !controlSelected;
    Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuControls::loadMenu(Input& input){

    // Set the background of the menu
    backgroundTexture.loadFromFile("Resources/Menus/OptionsMenu/icon.png");
    backgroundMenu = sf::IntRect(0, 0, input.gameWindow.getSize().x, input.gameWindow.getSize().y);

    background = sf::Sprite(backgroundTexture, backgroundMenu);
    background.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                        float(input.gameWindow.getSize().y) / SCREEN_0.second);

    // Set the main panel that contains the controllers
    texturePanel.loadFromFile("Resources/Menus/OptionsMenu/outrun.png");
    texturePanel.setRepeated(true);

    mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - 250.0f * input.screenScaleX);
    mainPanel.setSize(sf::Vector2f(710.0f * input.screenScaleX, 500.0f * input.screenScaleX));
    mainPanel.setOutlineColor(sf::Color(19, 186, 251));
    mainPanel.setOutlineThickness(5.0f * input.screenScaleX);
    mainPanel.setTexture(&texturePanel, true);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    // Set the menu title
    titleText.setString("CONTROLS");
    titleText.setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    titleText.setFont(fontMenu);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::Red);
    titleText.setOutlineColor(sf::Color(12, 12, 12));
    titleText.setOutlineThickness(3.0f * input.screenScaleX);
    titleText.setPosition((input.gameWindow.getSize().x / 2.f) - titleText.getLocalBounds().width / 2.f,
                          input.gameWindow.getSize().y / 2.f - 260.0f * input.screenScaleX);

    // Set the information text indicator
    informationText.setString("PRESS ENTER TO SELECT A CONTROL AND PRESS A KEY TO CHANGE THE CONTROL");
    informationText.setFont(fontMenu);
    informationText.setFillColor(sf::Color(10, 201, 235));
    informationText.setOutlineColor(sf::Color(3, 39, 8));
    informationText.setCharacterSize(static_cast<unsigned int>(int(15.0f * input.screenScaleX)));
    informationText.setOutlineThickness(3.0f * input.screenScaleX);
    informationText.setStyle(sf::Text::Bold);
    informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                input.gameWindow.getSize().y / 2.f - 185.0f * input.screenScaleX);

    /*
     * Load all the menu buttons
     */

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




/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuControls::handleEvent(Input& input){
    sf::Event event;
    // Detect the possible actions of the player
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            if (!escapePressed)
                // Close the game
                escapePressed = true;
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code != sf::Keyboard::Unknown)
        {
            // Control if the player has pressed a key and it has been recognized by the system
            if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                // Player cancels the menu and goes back to the options menu
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
            else if (controlSelected){
                // If a concrete controller is selected
                if (input.equal(Key::TURN_LEFT, event.key.code) || input.equal(Key::TURN_RIGHT, event.key.code) ||
                    input.equal(Key::ACCELERATE, event.key.code) || input.equal(Key::BRAKE, event.key.code) ||
                    input.equal(Key::UP_GEAR, event.key.code) || input.equal(Key::DOWN_GEAR, event.key.code))
                {
                    // Check if the key pressed is a key defined like a controller
                    Audio::play(Sfx::MENU_SELECTION_WRONG, false);
                }
                else {
                    // Available key (depending on the option selected replaces the current key by the new one)
                    switch (optionSelected){
                        case 0:
                            // Turn left
                            updateGameControl(input, event.key.code, Key::TURN_LEFT);
                            break;
                        case 1:
                            // Turn right
                            updateGameControl(input, event.key.code, Key::TURN_RIGHT);
                            break;
                        case 2:
                            // Acceleration
                            updateGameControl(input, event.key.code, Key::ACCELERATE);
                            break;
                        case 3:
                            // Braking
                            updateGameControl(input, event.key.code, Key::BRAKE);
                            break;
                        case 4:
                            // Up gear (manual transmission)
                            updateGameControl(input, event.key.code, Key::UP_GEAR);
                            break;
                        case 5:
                            // Down gear (manual transmission)
                            updateGameControl(input, event.key.code, Key::DOWN_GEAR);
                    }
                    // Update the configuration
                    input.modifiedConfig = true;
                }
            }
            else {
                if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP) && !controlSelected)
                    // Move the cursor to hover the upping controller
                    changeButtonSelected(true);
                else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN) && !controlSelected)
                    // Move the cursor to hover the downing controller
                    changeButtonSelected(false);
                else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                    if (!controlSelected)
                        // Select the hovered controller
                        Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                    else
                        // Stop selecting the hovered controller
                        Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

                    // Change the button status
                    controlSelected = !controlSelected;
                    menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_SELECTED);
                    menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_SELECTED);
                }
            }
        }
    }
}




/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuControls::draw(Input& input){

    // Draw the menu until the player exists (accept new configuration or cancel the menu)
    while (!escapePressed && !backPressed) {

        // Detect the actions of the player
        handleEvent(input);

        // Draw the menu with its components
        input.gameWindow.clear();
        input.gameWindow.draw(background);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(titleText);
        input.gameWindow.draw(informationText);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons)
            menuButton.render(&input.gameWindow);

        input.gameWindow.display();
        sf::sleep(sf::milliseconds(120));
    }
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuControls::returnMenu(Input& input){
    if (backPressed)
        // Gies back to the main options menu
        return State::OPTIONS;
    else if (escapePressed)
        // Close the game
        return State::EXIT;
}




