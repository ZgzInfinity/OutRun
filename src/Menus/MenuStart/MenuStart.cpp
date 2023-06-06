
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
 * Implementation file of the module MenuStart
 */

#include "MenuStart.h"



/**
 * Default constructor
 */
MenuStart::MenuStart() : Menu(){
    // The elements of the menu are displayed by default
    blink = true;
}



/**
 * Set the start menu configuration flags
 * @param _firstLoad controls if it the first load of the menu
 * @param _newGame controls if it is a new game round
 */
void MenuStart::setMenuStart(const bool _firstLoad, const bool _newGame){
    firstLoad = _firstLoad;
    newGame = _newGame;
}



/**
 * Get if it is the first time that the menu has been loaded
 */
bool MenuStart::getFirstLoad() const {
    return firstLoad;
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuStart::loadMenu(Input& input){

    // Load the menu background
    backgroundMenu.loadFromFile("Resources/Menus/MainMenu/LogoMain1.png");
    background.setTexture(backgroundMenu);
    background.setPosition(0, 0);
    background.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                        (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    // Load the game icon textures
    for (int i = 2; i <= 7; i++) {
        // Loading the texture of the game's name
        gameIcon.loadFromFile("Resources/Menus/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    // Set the game icon textures in the vector of sprites
    for (int i = 0; i < 6; i++) {
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 180.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleY);
        nameGame.setScale(2.0f * input.screenScaleX, 2.0f * input.screenScaleY);
        nameGames.push_back(nameGame);
    }

    /*
     * Set the text indicators of the menu
     */

    textElements[0].setString("PRESS START");
    textElements[0].setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    textElements[0].setFont(fontMenu);
    textElements[0].setFillColor(sf::Color::Green);
    textElements[0].setOutlineColor(sf::Color::Black);
    textElements[0].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[0].setPosition((input.gameWindow.getSize().x - textElements[0].getGlobalBounds().width) / 2.f,
                                input.gameWindow.getSize().y / 2.f + 50.0f * (input.gameWindow.getSize().y / SCREEN_0.second));

    textElements[1].setString("SELECT: ENTER KEY");
    textElements[1].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[1].setFont(fontMenu);
    textElements[1].setFillColor(sf::Color::Green);
    textElements[1].setOutlineColor(sf::Color::Black);
    textElements[1].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[1].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 130.0f * input.screenScaleY);

    textElements[2].setString("MOVE: ARROW KEYS");
    textElements[2].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[2].setFont(fontMenu);
    textElements[2].setFillColor(sf::Color::Green);
    textElements[2].setOutlineColor(sf::Color::Black);
    textElements[2].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[2].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 180.0f * input.screenScaleY);

    textElements[3].setString("BACK / PAUSE: ESC KEY");
    textElements[3].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[3].setFont(fontMenu);
    textElements[3].setFillColor(sf::Color::Green);
    textElements[3].setOutlineColor(sf::Color::Black);
    textElements[3].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[3].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 230.0f * input.screenScaleY);

    textElements[4].setString("SPACE: GAME CREDITS");
    textElements[4].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[4].setFont(fontMenu);
    textElements[4].setFillColor(sf::Color::Green);
    textElements[4].setOutlineColor(sf::Color::Black);
    textElements[4].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[4].setPosition(input.gameWindow.getSize().x / 15.f,
                                input.gameWindow.getSize().y / 2.f + 280.0f * input.screenScaleY);


    textElements[5].setString("ZGZ INFINITGAMES");
    textElements[5].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[5].setFont(fontMenu);
    textElements[5].setFillColor(sf::Color::Green);
    textElements[5].setOutlineColor(sf::Color::Black);
    textElements[5].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[5].setPosition(input.gameWindow.getSize().x * 0.7f,
                                input.gameWindow.getSize().y / 2.f + 230.0f * input.screenScaleY);

    textElements[6].setString("2023");
    textElements[6].setCharacterSize(static_cast<unsigned int>(int(30.0f * input.screenScaleX)));
    textElements[6].setFont(fontMenu);
    textElements[6].setFillColor(sf::Color::Green);
    textElements[6].setOutlineColor(sf::Color::Black);
    textElements[6].setOutlineThickness(3.0f * input.screenScaleX);
    textElements[6].setPosition(input.gameWindow.getSize().x * 0.79f,
                                input.gameWindow.getSize().y / 2.f + 280.0f * input.screenScaleY);

    // Set the black rectangle to make the darkness transition
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

}



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuStart::handleEvent(Input& input){
    sf::Event event;
    // Detect actions of the player
    input.gameWindow.pollEvent(event);
    if (input.closed(event)){
        // Check if the game is closed
        if (!escapePressed)
            escapePressed = true;
    }
    else {
        if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            // Check start key is pressed
            if (!startPressed){
                // Only one time
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                optionSelected = 0;
            }
        }
        else if (input.pressed(Key::MENU_CREDITS, event) && input.held(Key::MENU_CREDITS)){
            // Check if the credits key has been pressed
            if (!startPressed){
                // Only one time
                startPressed = true;
                optionSelected = 1;
                Audio::stop(Sfx::WIND);
                Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            }
        }
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuStart::draw(Input& input){

    // Configure the blinking effect with the clock
    int j = 0, k = 0;
    sf::Time blink_delay = sf::seconds(1.0);
    blinkClcok.restart().asSeconds();
    elapsedBlink = blinkClcok.restart().asSeconds();

    // Play the wind sound
    if (!Audio::isPlaying(Sfx::WIND))
        Audio::play(Sfx::WIND, true);

    // Draw the darkness animation
    if (firstLoad || newGame){
        for (int i = 255; i >= 0; i -= 5){

            // Detect possible actions of the player
            handleEvent(input);
            elapsedHide = blinkClcok.getElapsedTime().asSeconds();

            // Compute the blink effect
            if (elapsedHide - elapsedBlink >= blink_delay.asSeconds()) {
                blink = !blink;
                blinkClcok.restart();
            }

            if (blink) {
                // Display the menu components
                textElements[0].setFillColor(sf::Color::Green);
                textElements[0].setOutlineColor(sf::Color::Black);
            }
            else {
                // Hide the menu components
                textElements[0].setFillColor(sf::Color::Transparent);
                textElements[0].setOutlineColor(sf::Color::Transparent);
            }

            // Show the press start title in the menu with all the text indicators
            input.gameWindow.draw(background);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(textElements[0]);
            input.gameWindow.draw(textElements[1]);
            input.gameWindow.draw(textElements[2]);
            input.gameWindow.draw(textElements[3]);
            input.gameWindow.draw(textElements[4]);
            input.gameWindow.draw(textElements[5]);
            input.gameWindow.draw(textElements[6]);

            // Draw the dark rectangle to make the darkness transition
            blackShape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(blackShape);
            input.gameWindow.display();

            // Change the game icon texture to be displayed
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

    // While the menu is been played
    while (!startPressed && !escapePressed) {

        // Detect the actions of the player
        handleEvent(input);
        elapsedHide = blinkClcok.getElapsedTime().asSeconds();

        // Compute the blinking effect
        if (elapsedHide - elapsedBlink >= blink_delay.asSeconds()) {
            blink = !blink;
            blinkClcok.restart();
        }


        if (blink) {
            // Display the menu components
            textElements[0].setFillColor(sf::Color::Green);
            textElements[0].setOutlineColor(sf::Color::Black);
        }
        else {
            // Hide the menu components
            textElements[0].setFillColor(sf::Color::Transparent);
            textElements[0].setOutlineColor(sf::Color::Transparent);
        }

        // Show the press start title in the menu with all the text indicators
        input.gameWindow.draw(background);
        input.gameWindow.draw(nameGames[j]);
        for (int index = 0; index < ELEMENTS; index++)
            input.gameWindow.draw(textElements[index]);

        // Display the menu
        input.gameWindow.display();

        // Determine the game icon texture to be displayed
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
 * @param input is the module that has all the configuration of the game
 */
State MenuStart::returnMenu(Input& input){
    // Check if the player has pressed the start key
    if (startPressed){
        firstLoad = false;
        // Depending on the option menu selected chose a menu or other
        switch (optionSelected){
            case 0:
                // Game selection mode
                return State::GAME;
                break;
            case 1:
                // Credits menu
                return State::CREDITS;
        }
    }
    else
        // Close the menu
        return State::EXIT;
}
