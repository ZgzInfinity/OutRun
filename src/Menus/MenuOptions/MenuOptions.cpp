
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
 * Implementation file of the module MenuOptions
 */

#include "MenuOptions.h"



/**
 * Default constructor
 */
MenuOptions::MenuOptions() : Menu(){
    // First option hovered and not selected
    optionSelected = 0;
    resized = false;
    controlSelected = false;
}



/**
 * Set the game in pause mode
 * @param _pauseMode controls if the game is in pause mode or not
 */
void MenuOptions::setPauseMode(const bool _pauseMode){
    pauseMode = _pauseMode;
}



/**
 * Change the option menu selected
 * @param menuUpPressed controls if the menu_up key has been or not pressed
 */
void MenuOptions::changeButtonSelected(const bool menuUpPressed){
    // Menu up pressed
    if (menuUpPressed){
        if (optionSelected != 0){
            // Hover the lower option configuration (it is not the first one)
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 6].setButtonState(ButtonState::BUTTON_IDLE);

            // Check if the button selected is the controllers sub-menu
            if (optionSelected == 2)
                // Inform the player of the sub-menu
                menuButtons[optionSelected + 6].setTextButton(subMenu);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != int(menuButtons.size() - 1) / 2) {
            // Hover the upper option configuration (it is not the last one)
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 4].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    // Check if the option selected is the sound or controller sub-menus
    if (optionSelected > 2){
        switch (optionSelected){
            case 3:
                // Sound menu
                menuButtons[optionSelected + 5].setTextButton(access);
                menuButtons[optionSelected + 6].setTextButton(subMenu);
                break;
            case 4:
                // Controller menu
                menuButtons[optionSelected + 4].setTextButton(subMenu);
                menuButtons[optionSelected + 5].setTextButton(access);
                break;
        }
    }
}



/**
 * Change the difficulty level of the game
 * @param input is the module that has all the configuration of the game
 * @param event is the current event of the player that has been detected
 */
void MenuOptions::changeDifficulty(Input& input, const sf::Event& event){
    // Check if the player has pressed the left navigation key
    if (event.key.code == input.get(Key::MENU_LEFT)){
        if (input.difficulty != Level_Difficulty::EASY) {
            // Modify the difficulty if the current option is not the first one (EASY)
            if (input.difficulty == Level_Difficulty::NORMAL) {
                // From NORMAL to EASY
                input.difficulty = Level_Difficulty::EASY;
                menuButtons[optionSelected + 5].setTextButton("EASY");
            }
            else if (input.difficulty == Level_Difficulty::HARD) {
                // From HARD to NORMAL
                input.difficulty = Level_Difficulty::NORMAL;
                menuButtons[optionSelected + 5].setTextButton("NORMAL");
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else if (event.key.code == input.get(Key::MENU_RIGHT)){
        // Modify the difficulty if the current option is not the first one (HARD)
        if (input.difficulty != Level_Difficulty::HARD) {
            if (input.difficulty == Level_Difficulty::EASY) {
                // From EASY to NORMAL
                input.difficulty = Level_Difficulty::NORMAL;
                menuButtons[optionSelected + 5].setTextButton("NORMAL");
            }
            else if (input.difficulty == Level_Difficulty::NORMAL) {
                // From NORMAL to HARD
                input.difficulty = Level_Difficulty::HARD;
                menuButtons[optionSelected + 5].setTextButton("HARD");
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}



/**
 * Change the traffic level of the game
 * @param input is the module that has all the configuration of the game
 * @param event is the current event of the player that has been detected
 */
void MenuOptions::changeTrafficLevel(Input& input, const sf::Event& event){
        // Check if the player has pressed the left navigation key
    if (event.key.code == input.get(Key::MENU_LEFT)){
        if (input.traffic != Level_Traffic::LOW) {
            // Modify the traffic level if the current option is not the first one (LOW)
            if (input.traffic == Level_Traffic::MEDIUM) {
                // From MEDIUM to LOW
                input.traffic = Level_Traffic::LOW;
                menuButtons[optionSelected + 5].setTextButton("LOW");
            }
            else if (input.traffic == Level_Traffic::HIGH) {
                // From HIGH to MEDIUM
                input.traffic = Level_Traffic::MEDIUM;
                menuButtons[optionSelected + 5].setTextButton("MEDIUM");
            }
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else if (event.key.code == input.get(Key::MENU_RIGHT)){
        // Check if the player has pressed the right navigation key
        if (input.traffic != Level_Traffic::HIGH) {
            // Modify the traffic level if the current option is not the first one (HIGH)
            if (input.traffic == Level_Traffic::LOW) {
                // From LOW to MEDIUM
                input.traffic = Level_Traffic::MEDIUM;
                menuButtons[optionSelected + 5].setTextButton("MEDIUM");
            }
            else if (input.traffic == Level_Traffic::MEDIUM) {
                // From MEDIUM to HIGH
                input.traffic = Level_Traffic::HIGH;
                menuButtons[optionSelected + 5].setTextButton("HIGH");
            }
        }
        Audio::play(Sfx::MENU_SELECTION_MOVE, false);
    }
}



/**
 * Change the resolution of the game
 * @param input is the module that has all the configuration of the game
 * @param event is the current event of the player that has been detected
 */
void MenuOptions::changeResolution(Input& input, const sf::Event& event){
    // Check if the player has pressed the left navigation key
    if (event.key.code == input.get(Key::MENU_LEFT)){
        resized = true;
        if (input.currentIndexResolution > 0){
            // Not the first resolution is selected
            input.currentIndexResolution--;
            menuButtons[optionSelected + 5].setTextButton(to_string(input.resolutions[input.currentIndexResolution].first) + "x" +
                                                          to_string(input.resolutions[input.currentIndexResolution].second));

            // Modify the resolution of the screen
            input.gameWindow.create(sf::VideoMode(static_cast<unsigned int>(input.resolutions[input.currentIndexResolution].first),
                                                 static_cast<unsigned int>(input.resolutions[input.currentIndexResolution].second)), "Out Run",
                          sf::Style::Titlebar | sf::Style::Close);

            // Set the properties of the window
            input.gameWindow.setFramerateLimit(FPS);
            input.gameWindow.setKeyRepeatEnabled(false);
            input.gameWindow.setMouseCursorVisible(false);

            // Create a new screen view with the new resolution
            input.gameWindow.setView(sf::View(sf::Vector2f(input.gameWindow.getSize().x / 2.0f, input.gameWindow.getSize().y / 2.0f),
                                             sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y)));

            input.screenScaleX = float(input.gameWindow.getSize().x) / float(SCREEN_0.first);
            input.screenScaleY = float(input.gameWindow.getSize().y) / float(SCREEN_0.second);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
            input.camD = (input.currentIndexResolution <= 1) ? 0.84 : 0.83;
        }
    }
    else if (event.key.code == input.get(Key::MENU_RIGHT)){
        // Check if the player has pressed the right navigation key
        if (input.currentIndexResolution < int(input.resolutions.size())){
            // Not is selected the full screen resolution
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
                // Full screen resolution
                input.gameWindow.create(sf::VideoMode(SCREEN_1.first, SCREEN_1.second), "Out Run", sf::Style::Fullscreen);
                input.currentIndexResolution = (int)Resolution::__COUNT;
            }

            // Set the properties of the window
            input.gameWindow.setFramerateLimit(FPS);
            input.gameWindow.setKeyRepeatEnabled(false);
            input.gameWindow.setMouseCursorVisible(false);

            // Create a new screen view with the new resolution
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
        // If the game is in pause reload the huds to adapt them to the new screen resolution
        HudRound::configureHudRound(input);
        HudCheckPoint::configureHudCheckPoint(input);
    }
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuOptions::loadMenu(Input& input){

    // Clear the buttons
    menuButtons.clear();

    // Set background of the menu
    backgroundTexture.loadFromFile("Resources/Menus/OptionsMenu/icon.png");
    backgroundRect = sf::IntRect(0, 0, input.gameWindow.getSize().x, input.gameWindow.getSize().y);

    background = sf::Sprite(backgroundTexture, backgroundRect);
    background.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                        float(input.gameWindow.getSize().y) / SCREEN_0.second);

    // Set the main panel of the menu
    panelTexture.loadFromFile("Resources/Menus/OptionsMenu/outrun.png");
    panelTexture.setRepeated(true);

    panel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                      input.gameWindow.getSize().y / 2.f - 250.0f * input.screenScaleX);
    panel.setSize(sf::Vector2f(710.0f * input.screenScaleX, 500.0f * input.screenScaleX));
    panel.setOutlineColor(sf::Color(19, 186, 251));
    panel.setOutlineThickness(5.0f * input.screenScaleX);
    panel.setTexture(&panelTexture, true);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    // Set the title of the menu
    titleText.setString("CONTROLS");
    titleText.setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    titleText.setFont(fontMenu);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::Red);
    titleText.setOutlineColor(sf::Color(12, 12, 12));
    titleText.setOutlineThickness(3.0f * input.screenScaleX);
    titleText.setPosition((input.gameWindow.getSize().x / 2.f) - titleText.getLocalBounds().width / 2.f,
                          input.gameWindow.getSize().y / 2.f - 260.0f * input.screenScaleX);

    // Set the information of the menu
    informationText.setString("PRESS ENTER TO SELECT A CONTROL AND PRESS A KEY TO CHANGE THE CONTROL");
    informationText.setFont(fontMenu);
    informationText.setFillColor(sf::Color(10, 201, 235));
    informationText.setOutlineColor(sf::Color(3, 39, 8));
    informationText.setCharacterSize(static_cast<unsigned int>(int(15.0f * input.screenScaleX)));
    informationText.setOutlineThickness(3.0f * input.screenScaleX);
    informationText.setStyle(sf::Text::Bold);
    informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                input.gameWindow.getSize().y / 2.f - 175.0f * input.screenScaleX);

    // Set the status of the buttons
    sf::Color buttonColor = (pauseMode) ? BUTTON_LOCKED_COLOR : BUTTON_IDLE_COLOR;

    /*
     * Add the left buttons
     */

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 130.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "DIFFICULTY", buttonColor, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             resized ? ButtonState::BUTTON_IDLE : ButtonState::BUTTON_HOVER,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f - 270.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 60.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             "TRAFFIC", buttonColor, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
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

    // Determine the difficulty indicator on the game depending on the difficulty level
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

    // Determine the traffic indicator on the game depending on the traffic level
    switch (input.traffic) {
        case Level_Traffic::LOW:
            traffic = "LOW";
            break;
        case Level_Traffic::MEDIUM:
            traffic = "MEDIUM";
            break;
        case Level_Traffic::HIGH:
            traffic = "HIGH";
            break;
    }

    // Determine the sub-menu indicators
    subMenu = "MENU", access = "PRESS ENTER", saved = "SAVED!";

    // Compute the screen resolution
    string resolution = input.currentIndexResolution == (int)Resolution::__COUNT ? "FULLSCREEN" :
        to_string(input.resolutions[input.currentIndexResolution].first) + "x" +
        to_string(input.resolutions[input.currentIndexResolution].second);

    /*
     * Add the right buttons
     */

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 130.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             difficulty, buttonColor, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
                             resized ? ButtonState::BUTTON_IDLE : ButtonState::BUTTON_HOVER,
                             input.screenScaleX);

    menuButtons.emplace_back(input.gameWindow.getSize().x / 2.f + 80.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 60.0f * input.screenScaleX,
                             200.0f * input.screenScaleX, 30.0f * input.screenScaleX, fontMenu,
                             traffic, buttonColor, BUTTON_HOVER_COLOR, BUTTON_SELECTED_COLOR,
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



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuOptions::handleEvent(Input& input){
    sf::Event event;
    // Detect possible actions of the player
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            if (!escapePressed)
                // Game closed
                escapePressed = true;
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            // Player cancels the menu
            if (!controlSelected){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                Audio::stop(Soundtrack::OPTIONS);
                if (!pauseMode)
                    // If a game round is not in curse goes to the main menu
                    Audio::play(Sfx::WIND, true);
            }
        }
        else if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP) && !controlSelected)
            // Change the option menu hovered (move up)
            changeButtonSelected(true);
        else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN) && !controlSelected)
            // Change the option menu hovered (move down)
            changeButtonSelected(false);
        else if ((input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT) && controlSelected) ||
                 (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT) && controlSelected))
        {
            // Check the option selected (move left or right)
            switch (optionSelected) {
                case 0:
                    if (!pauseMode)
                        // Change the difficulty level (not available during a game round)
                        changeDifficulty(input, event);
                    break;
                case 1:
                    if (!pauseMode)
                            // Change the traffic level (not available during a game round)
                        changeTrafficLevel(input, event);
                    break;
                case 2:
                    // Change the resolution of the screen and reload the GUI
                    changeResolution(input, event);
                    loadMenu(input);
                    break;
            }
            input.modifiedConfig = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            // Player presses the start key
            if (!pauseMode || (pauseMode && optionSelected >= 2)){
                // Check if the game was is pause or if the player selected sound or controller sub-menus
                if (!controlSelected)
                    Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                else
                    Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

                controlSelected = !controlSelected;

                // Change the status of the buttons
                if (controlSelected){
                    menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_SELECTED);
                    menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_SELECTED);
                }
                else {
                    menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
                    menuButtons[optionSelected + 5].setButtonState(ButtonState::BUTTON_HOVER);
                }
                if (optionSelected > 2)
                    // Redirects to the sub-menu selected
                    startPressed = true;
            }
        }
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuOptions::draw(Input& input){

    // Draw the options menu until the menu is closed
    while (!startPressed && !escapePressed && !backPressed) {

        // Detect the possible actions of the player
        handleEvent(input);

        // Draw the menu
        input.gameWindow.clear();
        input.gameWindow.draw(background);
        input.gameWindow.draw(panel);
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
State MenuOptions::returnMenu(Input& input){
    // Check if the player has pressed the start key
    if (startPressed){
        switch (optionSelected){
            case 3:
                // Goes to music sound configuration menu
                return State::MUSIC_CONF;
                break;
            case 4:
                // Goes to controller menu
                return State::CONTROLS;
                break;
        }
    }
    else if (backPressed){
        // Player exists the options menu
        if (input.modifiedConfig){
            // If there is any configuration change it is changed
            input.writeNewInput();
            Logger::setWidthScreen(input.gameWindow.getSize().x);
        }
        input.modifiedConfig = false;
        // Check if the game is paused
        if (pauseMode)
            // Goes to pause menu
            return State::PAUSE;
        else
            // Goes to main menu
            return State::GAME;
    }
    else if (escapePressed)
        // Close the game
        return State::EXIT;
}
