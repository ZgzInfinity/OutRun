
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
 * Implementation file of the module MenuGears
 */

#include "MenuGears.h"



/**
 * Default constructor
 * @param _playerCarSelected controls if the player has selected a color for the car
 */
MenuGears::MenuGears(const int _playerCarSelected) : Menu(){
    // Assign all the default configuration
    offsetY = 700.f;
    offsetTitleMenuText = -1.0f;
    offsetDescriptionMenuText = 4.0f;
    mainTextArrived = false;
    descriptionTextArrived = false;
    startPressed = false;
    automaticMode = true;
    pressedKey = false;
    escapePressed = false;
    playerCarSelected = _playerCarSelected;
}



/**
 * Get if the player has selected automatic mode or not
 */
bool MenuGears::getAutomaticMode() const {
    return automaticMode;
}


/**
 * Set the start menu configuration flags
 * @param _firstLoad controls if it the first load of the menu
 * @param _newGame controls if it is a new game round
 */
void MenuGears::setMenuGears(const bool _firstLoad, const bool _newGame){
    firstLoad = _firstLoad;
    newGame = _newGame;
}



/**
 * Get if it is the first time that the menu has been loaded
 */
bool MenuGears::getFirstLoad() const {
    return firstLoad;
}



/**
 * Set the game mode to be played
 * @param _gameMode is the default game mode to be played
 */
void MenuGears::setGameMode(const GameMode _gameMode){
    gameMode = _gameMode;
}



/**
 * Get the game mode selected by the player
 */
GameMode MenuGears::getGameMode() const {
    return gameMode;
}


/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuGears::loadMenu(Input& input){

    // Loading the background of the menu
    backgroundMenu.loadFromFile("Resources/Menus/MainMenu/LogoMain1.png");
    background.setTexture(backgroundMenu);
    background.setPosition(0, 0);
    background.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    // Load the font of the text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    for (int i = 2; i <= 7; i++) {
        // Loading the texture of the game icon
        gameIcon.loadFromFile("Resources/Menus/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    for (int i = 0; i < 6; i++) {
        // Loading the texture of the game icon
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 180.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleY);
        nameGame.setScale(2.0f * input.screenScaleX, 2.0f * input.screenScaleY);
        nameGames.push_back(nameGame);
    }

    for (int i = 1; i <= 4; i++) {
        // Loading the textures of the gears
        gameIcon.loadFromFile("Resources/Menus/GearMenu/Gear" + to_string(i) + ".png");
        gearIcons.push_back(gameIcon);
    }

    for (int i = 0; i <= 3; i++) {
        // Loading the sprite of the gears
        nameGame.setTexture(gearIcons[i], true);
        nameGame.setScale(1.5f * input.screenScaleX, 1.5f * input.screenScaleY);
        if (i < 2)
            nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 280.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 65.0f * input.screenScaleY);
        else
            nameGame.setPosition((input.gameWindow.getSize().x / 2.f) + 50.0f * input.screenScaleX,
                                 input.gameWindow.getSize().y / 2.f - 65.0f * input.screenScaleY);
        gears.push_back(nameGame);
    }

    // Prepare the main panel
    mainPanel.setSize(sf::Vector2f(710.0f * input.screenScaleX, 490.0f * input.screenScaleY));
    mainPanel.setOutlineColor(sf::Color::Green);
    mainPanel.setOutlineThickness(5.0f * input.screenScaleX);
    mainPanel.setFillColor(sf::Color(0, 0, 0, 200));

    // Main title text of the menu
    titleMenuText.setString("GEARS MENU");
    titleMenuText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    titleMenuText.setFont(fontMenu);
    titleMenuText.setStyle(sf::Text::Bold);
    titleMenuText.setFillColor(sf::Color::White);
    titleMenuText.setOutlineColor(sf::Color::Black);
    titleMenuText.setOutlineThickness(3.0f * input.screenScaleX);

     // Description text of the menu
    descriptionMenuText.setString("CHOOSE TRANSMISSION");
    descriptionMenuText.setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    descriptionMenuText.setFont(fontMenu);
    descriptionMenuText.setStyle(sf::Text::Bold);
    descriptionMenuText.setFillColor(sf::Color(64, 147, 225));
    descriptionMenuText.setOutlineColor(sf::Color::Black);
    descriptionMenuText.setOutlineThickness(3.0f * input.screenScaleX);

    // Creation of the panel rectangle of the menu
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));
}



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuGears::handleEvent(Input& input){
    sf::Event event;
    // Detect actions of the player
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            // Game closed
            if (!escapePressed)
                escapePressed = true;
        }
        else {
            if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT)){
                // Detect if player selects the automatic transmission
                if (optionSelected != 0){
                    // The manual transmission was selected
                    pressedKey = true;
                    automaticMode = true;
                    optionSelected = 0;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT)){
                // Detect if player selects the manual transmission
                if (optionSelected != 1){
                    // The automatic transmission was selected
                    pressedKey = true;
                    automaticMode = false;
                    optionSelected = 1;
                    Audio::play(Sfx::MENU_SELECTION_MOVE, false);
                }
            }
            else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
                // Control if the player selects a transmission (automatic or manual) and the intro panel animation has entered
                if (mainTextArrived && descriptionTextArrived && !startPressed){
                    // Only once
                    startPressed = true;
                    Audio::stop(Sfx::WIND);
                    Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                }
            }
            else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
                // Control if the player cancels the transmission selection and the intro panel animation has entered
                if (mainTextArrived && descriptionTextArrived && !backPressed){
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
void MenuGears::draw(Input& input){

    // Prepare the icon game animation
    int j = 0, k = 0;

    // Play wind sound (coming from car selection menu)
    if (!Audio::isPlaying(Sfx::WIND))
        Audio::play(Sfx::WIND, true);

    // There is not color car selected
    if (playerCarSelected == -1){
        // Makes the darkness transition effect
        for (int i = 255; i >= 0; i -= 5){
            handleEvent(input);

            input.gameWindow.clear();
            input.gameWindow.draw(background);
            input.gameWindow.draw(nameGames[j]);

            blackShape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(blackShape);
            input.gameWindow.display();
        }
    }

    // Play the sfx panel
    Audio::play(Sfx::MENU_PANEL_DISPLAY, false);

    // Introduction of the gear panel transmission starts
    while(!mainTextArrived && !descriptionTextArrived){

        // Detect the events of the player
        sf::Event event;
        input.gameWindow.pollEvent(event);

        /*
         * Draw the main panel with the title and the description of the menu (moving animation)
         */

        mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                              input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);


        titleMenuText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleMenuText.getLocalBounds().width / 2.f)) * offsetTitleMenuText,
                                input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleY);

        descriptionMenuText.setPosition(((input.gameWindow.getSize().x / 2.f) - (descriptionMenuText.getLocalBounds().width / 2.f)) * offsetDescriptionMenuText,
                                        input.gameWindow.getSize().y / 2.f - 140.0f * input.screenScaleY);

        // Control if the title menu has arrived to the destiny position
        if (offsetTitleMenuText < 1.f)
            offsetTitleMenuText += 0.1;
        else
            mainTextArrived = true;

        // Control if the description menu has arrived to the destiny position
        if (offsetDescriptionMenuText > 1.1f)
            offsetDescriptionMenuText -= 0.15;
        else
            descriptionTextArrived = true;

        // Controls if the main panel menu has arrived to the destiny position
        if (offsetY > 250.f)
            offsetY -= 35.f;

        /*
         * Draw the menu
         */

        input.gameWindow.clear();
        input.gameWindow.draw(background);
        input.gameWindow.draw(nameGames[j]);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(titleMenuText);
        input.gameWindow.draw(descriptionMenuText);
        input.gameWindow.display();

        // Change the game texture icon
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

        sf::sleep(sf::milliseconds(50));
    }

    optionSelected = 0;
    k = 0;

    // Control the transmission selection
    while (!startPressed && !backPressed && !escapePressed){

        // Change the game texture icon
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

        /*
         * Draw the menu
         */

        input.gameWindow.clear();
        input.gameWindow.draw(background);
        input.gameWindow.draw(nameGames[j]);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(titleMenuText);
        input.gameWindow.draw(descriptionMenuText);

        // Control if the player has pressed a key
        if (!pressedKey || optionSelected == 0){
            // Automatic transmission selected
            input.gameWindow.draw(gears[optionSelected]);
            input.gameWindow.draw(gears[optionSelected + 3]);
        }
        else if (optionSelected == 1){
            // Manual transmission selected
            input.gameWindow.draw(gears[optionSelected]);
            input.gameWindow.draw(gears[optionSelected + 1]);
        }

        input.gameWindow.display();
        sf::sleep(sf::milliseconds(50));
        handleEvent(input);
    }

    // Control the hiding animation menu
    mainTextArrived = false;
    descriptionTextArrived = false;
    k = 0;

    // Check if the player has not canceled the transmission selection
    if (!escapePressed){

        // Play the hide sx
        Audio::play(Sfx::MENU_PANEL_HIDE, false);

        // Hiding panel animation
        while(!escapePressed && !mainTextArrived && !descriptionTextArrived){

            /*
             * Draw the main panel with the title and the description of the menu (moving animation)
             */

            mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                                  input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);

            titleMenuText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleMenuText.getLocalBounds().width / 2.f)) * offsetTitleMenuText,
                                    input.gameWindow.getSize().y / 2.f - 230.0f * input.screenScaleY);

            descriptionMenuText.setPosition(((input.gameWindow.getSize().x / 2.f) - (descriptionMenuText.getLocalBounds().width / 2.f)) * offsetDescriptionMenuText,
                                            input.gameWindow.getSize().y / 2.f - 140.0f * input.screenScaleY);

            // Detect the possible events
            handleEvent(input);

             // Control if the title menu has arrived to the destiny position (initial position)
            if (offsetTitleMenuText > -1.f)
                offsetTitleMenuText -= 0.1;
            else
                mainTextArrived = true;

             // Control if the description menu has arrived to the destiny position (initial position)
            if (offsetDescriptionMenuText < 4.0f)
                offsetDescriptionMenuText += 0.15;
            else
                descriptionTextArrived = true;

            // Controls if the main panel menu has arrived to the destiny position (initial position)
            if (offsetY < 850.f)
                offsetY += 35.f;

            // Change the game texture icon
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

            /*
             * Draw the menu
             */

            input.gameWindow.clear();
            input.gameWindow.draw(background);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(mainPanel);
            input.gameWindow.draw(descriptionMenuText);
            input.gameWindow.draw(titleMenuText);
            input.gameWindow.display();

            sf::sleep(sf::milliseconds(50));
            handleEvent(input);
        }

        // Make darkness transition before passing to the color car selection menu
        if (!escapePressed && !backPressed){
            for (int i = 0; i <= 70; i++){
                blackShape.setFillColor(sf::Color(0, 0, 0, i));
                input.gameWindow.draw(blackShape);
                input.gameWindow.display();
                sf::sleep(sf::milliseconds(20));
            }
        }
    }
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuGears::returnMenu(Input& input){
    if (escapePressed)
        // Game closed
        return State::EXIT;
    else if (backPressed)
        // Menu canceled
        return State::MODE;
    else
        // Gear transmission selected
        return State::VEHICLE;

}


