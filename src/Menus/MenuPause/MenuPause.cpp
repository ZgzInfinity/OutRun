
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
 * Implementation file of the module MenuPause
 */

#include "MenuPause.h"




/**
 * Default constructor
 * @param _m is the scenario to be drawn in the screen
 * @param _p is the player car
 * @param _trafficCars is a vector that stores all the traffic cars of the game round
 * @param _terrain is the type of terrain outside the road in
 * the scenario (SAND, GRASS, SNOW or MUD) presented with values (1..4)
 */
MenuPause::MenuPause(const Map& _m, const PlayerCar& _p, const vector<TrafficCar*> _trafficCars, const int _terrain) : Menu()
{
    // Assign all the information
    optionSelected = 0;
    m = _m;
    player = _p;
    cars = _trafficCars;
    terrain = _terrain;
}



/**
 * Set the game status of the game
 * @param _gameStatus is the current status of the game
 */
void MenuPause::setGameStatus(const State _gameStatus){
    gameStatus = _gameStatus;
}


/**
 * Set the game mode selected by the player
 * @param _gameMode is the game mode selected by the player
 */
void MenuPause::setGameMode(const GameMode _gameMode){
    gameMode = _gameMode;
}

/**
 * Change the option menu selected
 * @param menuUpPressed controls if the menu_up key has been or not pressed
 */
void MenuPause::changeButtonSelected(const bool menuUpPressed){
    // Check if the player has pressed the key move up the option selected
    if (menuUpPressed){
        if (optionSelected != 0){
            // The previous option hovered was not the first one
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        // Check if the player has pressed the key move down the option selected
        if (optionSelected != int(menuButtons.size() - 1)){
            // The previous option hovered was not the last one
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuPause::loadMenu(Input& input){

    // set the black screen of the menu
    blackScreen.setPosition(0, 0);
    blackScreen.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
    blackScreen.setFillColor(sf::Color(0, 0, 0, 200));

    // Set the main panel of the menu that is going to content all the buttons
    pausePanel.setPosition(input.gameWindow.getSize().x / 2.f - 120.0f * input.screenScaleX,
                           input.gameWindow.getSize().y / 2.f - 180.0f * input.screenScaleX);

    pausePanel.setSize(sf::Vector2f(250.0f * input.screenScaleX, 400.0f * input.screenScaleX));
    pausePanel.setFillColor(sf::Color(0, 0, 0));
    pausePanel.setOutlineColor(sf::Color::Green);
    pausePanel.setOutlineThickness(5.0f * input.screenScaleX);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    titleTextMenu.setString("PAUSE");
    titleTextMenu.setFont(fontMenu);
    titleTextMenu.setFillColor(sf::Color(214, 234, 12));
    titleTextMenu.setOutlineColor(sf::Color(12, 72, 234));
    titleTextMenu.setOutlineThickness(2.0f * input.screenScaleX);
    titleTextMenu.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    titleTextMenu.setPosition(input.gameWindow.getSize().x / 2.f - titleTextMenu.getLocalBounds().width / 2.f,
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



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuPause::handleEvent(Input& input){
    sf::Event event;
    // Check the possible actions of the player
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            // Game closed
            if (!escapePressed)
                escapePressed = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!startPressed){
                // Start key pressed
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
        }
        else if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP))
            // Move up and hover the upper option
            changeButtonSelected(true);
        else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN))
            // Move down and hover the lower option
            changeButtonSelected(false);
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuPause::draw(Input& input){

    // Stop the current soundtrack
    Audio::pause(input.currentSoundtrack);

    // Play the menu
    while (!startPressed && !escapePressed) {

        // Detect the action of the player
        handleEvent(input);

        // Clear the screen and draw all the menu components
        input.gameWindow.clear();
        m.renderMap(input, cars, player, gameStatus, gameMode, true);
        player.drawPlayRound(input, true, terrain, false);
        HudRound::drawHudRound(input);
        input.gameWindow.draw(blackScreen);
        input.gameWindow.draw(pausePanel);
        input.gameWindow.draw(titleTextMenu);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons)
            menuButton.render(&input.gameWindow);

        input.gameWindow.display();
        sf::sleep(sf::milliseconds(120));
    }

    // Check if START option has been pressed
    if (optionSelected == 2){
        // Draw the darkness transition
        for (int i = 0; i <= 70; i++){
            blackScreen.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(blackScreen);
            input.gameWindow.display();
            sf::sleep(sf::milliseconds(10));
        }
    }
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuPause::returnMenu(Input& input){
    // Check if the player pressed an option
    if (startPressed){
        switch(optionSelected){
            case 0:
                // Continue playing
                Audio::play(input.currentSoundtrack, true);
                return State::PLAY_ROUND;
                break;
            case 1:
                // Options menu
                Audio::play(Soundtrack::OPTIONS, true);
                return State::OPTIONS;
                break;
            case 2:
                // Main menu
                Audio::stop(input.currentSoundtrack);
                return State::START;
                break;
            case 3:
                // Close the game
                sf::sleep(sf::milliseconds(Audio::getDurationSfx(Sfx::MENU_SELECTION_CONFIRM).asMilliseconds()));
                return State::EXIT;
        }
    }
    else if (escapePressed)
        // Game has been closed
        return State::EXIT;
}



