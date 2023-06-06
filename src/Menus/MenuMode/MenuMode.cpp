
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
 * Implementation file of the module MenuMode
 */

#include "MenuMode.h"



/**
 * Default constructor
 */
MenuMode::MenuMode() : Menu(){}



/**
 * Set the game mode to be played
 * @param _gameMode is the default game mode to be played
 */
void MenuMode::setGameMode(const GameMode _gameMode){
    gameMode = _gameMode;
}



/**
 * Get the game mode selected by the player
 */
GameMode MenuMode::getGameMode() const {
    return gameMode;
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuMode::loadMenu(Input& input){

    // Prepare the background of the menu
    backgroundMenu.loadFromFile("Resources/Menus/MainMenu/LogoMain1.png");
    background.setTexture(backgroundMenu);
    background.setPosition(0, 0);
    background.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                        (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    // Load the font of the menu
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    for (int i = 2; i <= 7; i++) {
        // Loading the texture of the game's name
        gameIcon.loadFromFile("Resources/Menus/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    for (int i = 0; i < 6; i++) {
        // Loading the texture of the game's name
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 180.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleY);
        nameGame.setScale(2.0f * input.screenScaleX, 2.0f * input.screenScaleY);
        nameGames.push_back(nameGame);
    }

    //  Load the row selector
    rowSelector.loadFromFile("Resources/Menus/MainMenu/row.png");
    row.setTexture(rowSelector);
    row.setScale(0.06f * input.screenScaleX, 0.06f * input.screenScaleX);
    row.setPosition((input.gameWindow.getSize().x / 2.f) - 160.0f * input.screenScaleX,
                    input.gameWindow.getSize().y / 2.f + 25.0f * input.screenScaleX);

    /*
     * Prepare all the texts indicators of the menu
     */

    textElements[0].setString("ORIGINAL MODE");
    textElements[0].setPosition((input.gameWindow.getSize().x / 2.f) - 80.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 20.0f * input.screenScaleX);
    textElements[0].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[0].setFont(fontMenu);
    textElements[0].setFillColor(sf::Color::Green);
    textElements[0].setOutlineColor(sf::Color::Black);
    textElements[0].setOutlineThickness(3.0f * input.screenScaleX);

    textElements[1].setString("CONTINUOUS MODE");
    textElements[1].setPosition(input.gameWindow.getSize().x / 2.f - 80.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 70.0f * input.screenScaleX);
    textElements[1].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[1].setFont(fontMenu);
    textElements[1].setFillColor(sf::Color::Green);
    textElements[1].setOutlineColor(sf::Color::Black);
    textElements[1].setOutlineThickness(3.0f * input.screenScaleX);

    textElements[2].setString("SURVIVAL MODE");
    textElements[2].setPosition(input.gameWindow.getSize().x / 2.f - 80.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 120.0f * input.screenScaleX);
    textElements[2].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[2].setFont(fontMenu);
    textElements[2].setFillColor(sf::Color::Green);
    textElements[2].setOutlineColor(sf::Color::Black);
    textElements[2].setOutlineThickness(3.0f * input.screenScaleX);

}



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuMode::handleEvent(Input& input){
    sf::Event event;
    // Check the actions of the player
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            // Close the game
            if (!escapePressed)
                escapePressed = true;
        }
        else {
            if (input.pressed(Key::MENU_UP, event) && input.held(Key::MENU_UP)){
                // Move the row selector in up direction
                if (optionSelected != 0){
                    // The row was pointing the options tag
                    optionSelected--;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_DOWN, event) && input.held(Key::MENU_DOWN)){
                // Move the row selector in down direction
                if (optionSelected != 2){
                    // The row was pointing the start tag
                    optionSelected++;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                // Player presses start key
                if (!startPressed){
                    // Only one time
                    startPressed = true;
                    Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                }
            }
            else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                // Player presses escape key (cancel)
                if (!backPressed){
                    // Only one time
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
void MenuMode::draw(Input& input){

    // Partial state of the game
    int j = 0, k = 0;
    optionSelected = 0;

    // Play the wind sfx
    if (!Audio::isPlaying(Sfx::WIND))
        Audio::play(Sfx::WIND, true);

    // Game in curse
    while (!startPressed && !escapePressed && !backPressed) {

        // Detect the player actions
        handleEvent(input);

        // Determine the movement of the player selector
        switch (optionSelected){
            case 0:
                // Up position selected
                row.setPosition((input.gameWindow.getSize().x / 2.f) - 160.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 25.0f * input.screenScaleX);
                break;
            case 1:
                // Down position selected
                row.setPosition((input.gameWindow.getSize().x / 2.f) - 160.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 75.0f * input.screenScaleX);
                break;
            case 2:
                // Down position selected
                row.setPosition((input.gameWindow.getSize().x / 2.f) - 160.0f * input.screenScaleX,
                                input.gameWindow.getSize().y / 2.f + 125.0f * input.screenScaleX);
        }

        // Draw the menu completed in the screen
        input.gameWindow.draw(background);
        input.gameWindow.draw(nameGames[j]);
        for (int i = 0; i < ELEMENTS; i++)
            input.gameWindow.draw(textElements[i]);

        input.gameWindow.draw(row);
        input.gameWindow.display();

        // Change the texture of the game icon
        if (j < (int) nameGames.size() - 1){
            if (k == 10){
                j++;
                k = 0;
            }
            else
                k++;
        }
        else
            j = 0;
    }
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 */
State MenuMode::returnMenu(Input& input){
    // Check if the start key has been pressed
    if (startPressed){
        // Control the game mode selected by the player
        switch(optionSelected){
            case 0:
                gameMode = GameMode::ORIGINAL_MODE;
                break;
            case 1:
                gameMode = GameMode::CONTINUOUS_MODE;
                break;
            case 2:
                gameMode = GameMode::SURVIVAL_MODE;
        }
        // Start option selected
        return State::GEARS;
    }
    else if (backPressed)
        // Goes to the main menu of the game
        return State::START;
    else if (escapePressed)
        // Close the game
        return State::EXIT;
}
