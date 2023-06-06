
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
 * Implementation file of the module MenuSound
 */

#include "MenuSound.h"



/**
 * Default constructor
 */
MenuSound::MenuSound() : Menu(){
    // By default the first option is hovered and any one is selected
    optionSelected = 0;
    controlSelected = false;
}



/**
 * Change the option menu selected
 * @param menuUpPressed controls if the menu_up key has been or not pressed
 */
void MenuSound::changeButtonSelected(const bool menuUpPressed){
    // Menu up pressed
    if (menuUpPressed){
        if (optionSelected != 0){
            // First option selected (soundtracks option selected)
            optionSelected--;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 3].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
    else {
        if (optionSelected != 1){
            // Second option selected (sfx option selected)
            optionSelected++;
            menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected - 1].setButtonState(ButtonState::BUTTON_IDLE);
            menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_HOVER);
            menuButtons[optionSelected + 1].setButtonState(ButtonState::BUTTON_IDLE);
            Audio::play(Sfx::MENU_SELECTION_MOVE, false);
        }
    }
}



/**
 * Change the level volume of the option selected
 * @param menuLeftPressed controls if the menu_left key has been or not pressed
 */
void MenuSound::changeVolume(Input& input, const bool menuLeftPressed){
    // Get the current volumes
    float volumeMusicPct = Audio::getMusicVolume();
    float volumeSfxPct = Audio::getSfxVolume();
    int newVolMusic = 0, newVolSfx = 0;
    string volMusic, volSfx;
    if (menuLeftPressed){
        // Reduce volume
        switch (optionSelected) {
            case 0:
                // Soundtracks
                volumeMusicPct = std::fmaxf(volumeMusicPct - 0.05f, 0.0f);
                Audio::setVolumeMusic(volumeMusicPct);
                newVolMusic = (int)((Audio::getMusicVolume() + 0.005f) * 100);
                volMusic = to_string(newVolMusic);
                menuButtons[optionSelected + 2].setTextButton(volMusic);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                input.volumeMusic = newVolMusic;
                break;
            case 1:
                // Sfx
                volumeSfxPct = std::fmaxf(volumeSfxPct - 0.05f, 0.0f);
                Audio::setVolumeSfx(volumeSfxPct);
                newVolSfx = (int)((Audio::getSfxVolume() + 0.005f) * 100);
                volSfx = to_string(newVolSfx);
                menuButtons[optionSelected + 2].setTextButton(volSfx);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                input.volumeEffects = newVolSfx;
        }
    }
    else {
        // Add volume
        switch (optionSelected) {
            case 0:
                // Soundtracks
                volumeMusicPct = std::fminf(volumeMusicPct + 0.05f, 1.0f);
                Audio::setVolumeMusic(volumeMusicPct);
                newVolMusic = (int)((Audio::getMusicVolume() + 0.005f) * 100);
                volMusic = to_string(newVolMusic);
                menuButtons[optionSelected + 2].setTextButton(volMusic);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                input.volumeMusic = newVolMusic;
                break;
            case 1:
                // Sfx
                volumeSfxPct = std::fminf(volumeSfxPct + 0.05f, 1.0f);
                Audio::setVolumeSfx(volumeSfxPct);
                newVolSfx = (int)((Audio::getSfxVolume() + 0.005f) * 100);
                volSfx = to_string(newVolSfx);
                menuButtons[optionSelected + 2].setTextButton(volSfx);
                Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                input.volumeEffects = newVolSfx;
        }
    }

    // Update the new volume values
    input.modifiedConfig = true;
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuSound::loadMenu(Input& input){

    // Clear the vector of buttons
    menuButtons.clear();

    // Prepare the background of the menu
    background.loadFromFile("Resources/Menus/OptionsMenu/icon.png");
    backgroundMenu = sf::IntRect(0, 0, input.gameWindow.getSize().x, input.gameWindow.getSize().y);

    backGroundSprite = sf::Sprite(background, backgroundMenu);
    backGroundSprite.setScale(float(input.gameWindow.getSize().x) / SCREEN_0.first,
                              float(input.gameWindow.getSize().y) / SCREEN_0.second);

    // Prepare the main panel of the menu with all the buttons
    panel.loadFromFile("Resources/Menus/OptionsMenu/outrun.png");
    panel.setRepeated(true);

    mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                          input.gameWindow.getSize().y / 2.f - 250.0f * input.screenScaleX);
    mainPanel.setSize(sf::Vector2f(710.0f * input.screenScaleX, 500.0f * input.screenScaleX));
    mainPanel.setOutlineColor(sf::Color(19, 186, 251));
    mainPanel.setOutlineThickness(5.0f * input.screenScaleX);
    mainPanel.setTexture(&panel, true);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    // Get the volumes of the game (soundtracks and sfx)
    string volMusic = std::to_string(input.volumeMusic);
    string volSfx = std::to_string(input.volumeEffects);

    // Create the menu title
    titleMenuText.setString("SOUND MENU");
    titleMenuText.setCharacterSize(static_cast<unsigned int>(int(55.0f * input.screenScaleX)));
    titleMenuText.setFont(fontMenu);
    titleMenuText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleMenuText.setFillColor(sf::Color::Red);
    titleMenuText.setOutlineColor(sf::Color(12, 12, 12));
    titleMenuText.setOutlineThickness(3.0f * input.screenScaleX);
    titleMenuText.setPosition((input.gameWindow.getSize().x / 2.f) - titleMenuText.getLocalBounds().width / 2.f,
                              input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleX);

    // Create the menu infoMenuTextrmation text
    infoMenuText.setString("PRESS ENTER TO SELECT A CONTROL AND PRESS A KEY TO CHANGE THE CONTROL");
    infoMenuText.setFont(fontMenu);
    infoMenuText.setFillColor(sf::Color(10, 201, 235));
    infoMenuText.setOutlineColor(sf::Color(3, 39, 8));
    infoMenuText.setCharacterSize(static_cast<unsigned int>(int(15.0f * input.screenScaleX)));
    infoMenuText.setOutlineThickness(3.0f * input.screenScaleX);
    infoMenuText.setStyle(sf::Text::Bold);
    infoMenuText.setPosition(input.gameWindow.getSize().x / 2.f - infoMenuText.getLocalBounds().width / 2.f,
                             input.gameWindow.getSize().y / 2.f - 125.0f * input.screenScaleX);

    /*
     * Load the menu buttons and push them to the vector
     */

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



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuSound::handleEvent(Input& input){
    sf::Event event;
    // Detect possible actions of the player
    while (input.gameWindow.pollEvent(event)){
        // Game window closed
        if (input.closed(event))
            escapePressed = true;
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            // Check if the player exists from the menu (cancel)
            Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            backPressed = true;
        }
        else if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP) && !controlSelected)
            // Change option menu selected to soundtracks
            changeButtonSelected(true);
        else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN) && !controlSelected)
            // Change option menu selected to sfx
            changeButtonSelected(false);
        else if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT) && controlSelected)
            // Modify volume (less volume)
            changeVolume(input, true);
        else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT) && controlSelected)
            // Modify volume (more volume)
            changeVolume(input, false);
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            // Select an option to be modified
            if (!controlSelected)
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            else
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

            controlSelected = !controlSelected;

            // Change the color and status of the button when it is selected
            if (controlSelected){
                // Option selected
                menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_SELECTED);
                menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_SELECTED);
            }
            else {
                // Option hovered
                menuButtons[optionSelected].setButtonState(ButtonState::BUTTON_HOVER);
                menuButtons[optionSelected + 2].setButtonState(ButtonState::BUTTON_HOVER);
            }
        }
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuSound::draw(Input& input){

    // Draw the menu
    while (!escapePressed && !backPressed) {

        // Detect actions of the player
        handleEvent(input);

        // Clear the screen and draw all the elements
        input.gameWindow.clear();
        input.gameWindow.draw(backGroundSprite);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(titleMenuText);
        input.gameWindow.draw(infoMenuText);

        // Draw all the buttons
        for (auto &menuButton : menuButtons) {
            menuButton.render(&input.gameWindow);
        }

        // Display the menu
        input.gameWindow.display();
        sf::sleep(sf::milliseconds(120));
    }
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuSound::returnMenu(Input& input){
    // Player selects an option to configure
    if (backPressed)
        // Goes back to options (cancel)
        return State::OPTIONS;
    else if (escapePressed)
        // Close the game
        return State::EXIT;
}

