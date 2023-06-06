
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
 * Implementation file of the module MenuCredits
 */

#include "MenuCredits.h"



/**
 * Default constructor
 */
MenuCredits::MenuCredits() : Menu(){
    // Assign all the default information
    offsetY = 700.f;
    offsetTitleText = -1.0f;
    offsetDescriptionMenuText = 4.0f;
    mainTextArrived = false;
    descriptionTextArrived = false;

    // Load all the credits of the game
    Credit::loadAll();
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuCredits::loadMenu(Input& input){

    // Prepare the background of the menu
    backgroundMenu.loadFromFile("Resources/Menus/MainMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    // Set the font of the menu
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");


    for (int i = 2; i <= 7; i++) {
        // Load the game icon textures
        gameIcon.loadFromFile("Resources/Menus/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    for (int i = 0; i < 6; i++) {
        // Set the game icon textures in the vector of sprites
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((input.gameWindow.getSize().x / 2.f) - 180.0f * input.screenScaleX,
                             input.gameWindow.getSize().y / 2.f - 200.0f * input.screenScaleY);
        nameGame.setScale(2.0f * input.screenScaleX, 2.0f * input.screenScaleY);
        nameGames.push_back(nameGame);
    }

    // Set the main panel of the menu
    mainPanel.setSize(sf::Vector2f(720.0f * input.screenScaleX, 490.0f * input.screenScaleY));
    mainPanel.setOutlineColor(sf::Color::Green);
    mainPanel.setOutlineThickness(5.0f * input.screenScaleX);
    mainPanel.setFillColor(sf::Color(0, 0, 0, 200));

    // Title of the menu
    titleText.setString("CREDITS");
    titleText.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    titleText.setFont(fontMenu);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the black panel to result the credits
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x, input.gameWindow.getSize().y));

    // Set the game technical indicator
    gameDetailText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    gameDetailText.setFont(fontMenu);
    gameDetailText.setFillColor(sf::Color::White);
    gameDetailText.setOutlineColor(sf::Color::Black);
    gameDetailText.setOutlineThickness(5.0f * input.screenScaleX);
    gameDetailText.setStyle(sf::Text::Bold);

    // Set the information right indicator
    informationText.setCharacterSize(static_cast<unsigned int>(int(19.0f * input.screenScaleX)));
    informationText.setFont(fontMenu);
    informationText.setFillColor(sf::Color::White);
    informationText.setOutlineColor(sf::Color::Black);
    informationText.setOutlineThickness(5.0f * input.screenScaleX);
    informationText.setStyle(sf::Text::Bold);

    // Set the right indicator
    rightText.setCharacterSize(static_cast<unsigned int>(int(25.0f * input.screenScaleX)));
    rightText.setFont(fontMenu);
    rightText.setFillColor(sf::Color::White);
    rightText.setOutlineColor(sf::Color::Black);
    rightText.setOutlineThickness(5.0f * input.screenScaleX);
    rightText.setStyle(sf::Text::Bold);
}




/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuCredits::handleEvent(Input& input){
    sf::Event event;
    // Check if the player has done any action
    while (input.gameWindow.pollEvent(event)){
        if (input.closed(event)){
            // Closes the game window
            if (!escapePressed)
                escapePressed = true;
        }
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            // Press start key
            if (!startPressed){
                // Only lets one pressing
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                Audio::stop(Soundtrack::CREDITS);
            }
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            // Press escape key
            if (!backPressed){
                // Only lets one pressing
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_BACK, false);
                Audio::stop(Soundtrack::CREDITS);
            }
        }
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuCredits::draw(Input& input){

    // Preparing the changing icons game animation
    int j = 0, k = 0;
    Audio::play(Sfx::MENU_PANEL_DISPLAY, false);
    Audio::play(Soundtrack::CREDITS, true);

    // Initial animation that introduce the credit panel
    while(!mainTextArrived && !descriptionTextArrived){

        // Detect actions of the player
        sf::Event event;
        input.gameWindow.pollEvent(event);

        // Creation of the panel rectangle of the menu
        mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 360.0f * input.screenScaleX,
                              input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);

        // Title indicator
        titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

        // Control if the title menu has arrived to the destiny position
        if (offsetTitleText < 1.f)
            offsetTitleText += 0.1;
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

        // Draw the menu components
        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(nameGames[j]);
        input.gameWindow.draw(mainPanel);
        input.gameWindow.draw(titleText);
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

    /*
     * Initial animation finished
     */

    // Starting credit ad
    int numAnimation = 1;


    // Play the credits animation until it finishes or player exists the menu
    while (!startPressed && !escapePressed && !backPressed && numAnimation <= 7) {

        // Counter of time to display the credit ad
        int i = 0;

        // Play the credit ad until it finishes or the time runs out
        while (!startPressed && !escapePressed &&
               !backPressed && i <= TIME_CREDIT_SCREEN)
        {

            // Display the animation
            input.gameWindow.clear();
            input.gameWindow.draw(mainMenu);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(mainPanel);

            // Determine which animation to show
            switch(numAnimation){
                case 1:
                    // Menu title
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

                    input.gameWindow.draw(titleText);

                    // Show the technical information of the game (author, year, ....)
                    for (int j = 0; j < (int)Game_Detail::__COUNT; j++){
                        gameDetailText.setString(Credit::getGameDetail(j));
                        gameDetailText.setPosition(input.gameWindow.getSize().x / 2.f - gameDetailText.getLocalBounds().width / 2.f,
                                                   input.gameWindow.getSize().y / 2.f - 140.f + (60.f * j) * input.screenScaleY);

                        input.gameWindow.draw(gameDetailText);
                    }
                    break;
                case 2:
                    // Title
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

                    input.gameWindow.draw(titleText);

                    // Show the resource from where the sprites, soundtracks and sfx have been obtained
                    for (int j = 0; j < (int)Game_Acknowlegment::__COUNT; j++){
                        rightText.setString(Credit::getGameAcknowlegment(j));
                        rightText.setPosition(input.gameWindow.getSize().x / 2.f - rightText.getLocalBounds().width / 2.f,
                                              input.gameWindow.getSize().y / 2.f - 120.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(rightText);
                    }
                    break;
                case 3:
                    // Title of the menu
                    titleText.setString("SOUNDTRACKS");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

                    input.gameWindow.draw(titleText);

                    /*
                     * Show the games with its develop enterprise whose soundtracks have been used
                     */

                    for (int j = 0; j < (int)Soundtrack_Credit::__COUNT; j++){
                        informationText.setString(Credit::getSoundtrackCredit(j));
                        informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                                    input.gameWindow.getSize().y / 2.f - 125.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(informationText);

                        informationText.setString(Credit::getSoundtrackEnterpriseCredit(j));
                        informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                                    input.gameWindow.getSize().y / 2.f - 95.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(informationText);
                    }
                    break;
                case 4:
                    // Menu title
                    titleText.setString("SOUNDTRACKS");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                          input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);
                    input.gameWindow.draw(titleText);

                    /*
                     * Show the games with its develop enterprise whose soundtracks have been used
                     * (there are two because there are to much to be displayed in only one ad)
                     */

                    for (int j = 0; j < (int)Soundtrack_Credit_Two::__COUNT; j++){
                        informationText.setString(Credit::getSoundtrackCreditTwo(j));
                        informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                                    input.gameWindow.getSize().y / 2.f - 125.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(informationText);

                        informationText.setString(Credit::getSoundtrackEnterpriseCreditTwo(j));
                        informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                                    input.gameWindow.getSize().y / 2.f - 95.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(informationText);
                    }
                    break;
                case 5:
                    // Menu title
                    titleText.setString("SFX");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                            input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

                    input.gameWindow.draw(titleText);

                    /*
                     * Show the games with its develop enterprise whose sfx have been used
                     */

                    for (int j = 0; j < (int)Soundtrack_Credit::__COUNT; j++){
                        informationText.setString(Credit::getSfxCredit(j));
                        informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                                    input.gameWindow.getSize().y / 2.f - 125.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(informationText);

                        informationText.setString(Credit::getSfxEnterpriseCredit(j));
                        informationText.setPosition(input.gameWindow.getSize().x / 2.f - informationText.getLocalBounds().width / 2.f,
                                                    input.gameWindow.getSize().y / 2.f - 95.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(informationText);
                    }
                    break;
                case 6:

                    /*
                     * Show the games with its develop enterprise whose sprites have been used
                     */

                    titleText.setString("SPRITES");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                            input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Sprite_Enterprise::__COUNT; j++){
                        rightText.setString(Credit::getSpriteEnterpriseCredit(j));
                        rightText.setPosition(input.gameWindow.getSize().x / 2.f - rightText.getLocalBounds().width / 2.f,
                                              input.gameWindow.getSize().y / 2.f - 70.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(rightText);
                    }
                    break;
                case 7:

                    /*
                     * Show the acknowledgments to the games in which this game has been inspired
                     */

                    titleText.setString("ACKNOWLEDGMENTS");
                    titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                            input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

                    input.gameWindow.draw(titleText);

                    for (int j = 0; j < (int)Acknowledgments::__COUNT; j++){
                        rightText.setString(Credit::getAcknowledgments(j));
                        rightText.setPosition(input.gameWindow.getSize().x / 2.f - rightText.getLocalBounds().width / 2.f,
                                              input.gameWindow.getSize().y / 2.f - 70.f + (120.f * j) * input.screenScaleY);

                        input.gameWindow.draw(rightText);
                    }
            }
            input.gameWindow.display();
            handleEvent(input);
            i++;
        }
        // Increment the animation to be displayed
        numAnimation++;
    }

    // Reset the intro panel animation
    mainTextArrived = false;
    descriptionTextArrived = false;

    // Check if the player has exit the credit menu
    if (!escapePressed){

        // Play the hide sound
        Audio::play(Sfx::MENU_PANEL_HIDE, false);

        // Play the hiding panel animation
        while(!escapePressed && !mainTextArrived && !descriptionTextArrived){

            // Detect possible actions of the player
            handleEvent(input);

            // Creation of the panel rectangle of the menu with the tile
            mainPanel.setPosition((input.gameWindow.getSize().x / 2.f) - 350.0f * input.screenScaleX,
                                  input.gameWindow.getSize().y / 2.f - offsetY * input.screenScaleY);

            titleText.setPosition(((input.gameWindow.getSize().x / 2.f) - (titleText.getLocalBounds().width / 2.f)) * offsetTitleText,
                                    input.gameWindow.getSize().y / 2.f - 240.0f * input.screenScaleY);

            // Control if the title menu has arrived to the destiny position (initial position)
            if (offsetTitleText > -1.f)
                offsetTitleText -= 0.1;
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

            // Draw the menu components
            input.gameWindow.clear();
            input.gameWindow.draw(mainMenu);
            input.gameWindow.draw(nameGames[j]);
            input.gameWindow.draw(mainPanel);
            input.gameWindow.draw(titleText);
            input.gameWindow.display();

            sf::sleep(sf::milliseconds(50));
        }
    }
    if (!escapePressed)
        // Check if the player has canceled the credit menu and the music is stopped
        Audio::stop(Soundtrack::CREDITS);
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuCredits::returnMenu(Input& input){
    if (escapePressed)
        // Game closed
        return State::EXIT;
    else
        // Credits animation finished or canceled (goes to main menu)
        return State::START;
}
