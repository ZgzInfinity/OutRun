
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
 * Implementation file of the module MenuMusicRadio
 */

#include "MenuMusicRadio.h"



/**
 * Default constructor
 */
MenuMusicRadio::MenuMusicRadio() : Menu(){
    // Default soundtrack to be played
    optionSelected = 0;
}



/**
 * Changes the soundtrack to be listened in the menu
 * @param menuLeftPressed controls if the move left key has been pressed or not
 */
void MenuMusicRadio::changeGameMusic(Input& input, const bool& menuLeft){
    // Control if the left or right cursor keys are pressed or not
    if (menuLeft) {
        // Change the soundtrack selected in the list (left changing)
        if (input.currentSoundtrack != 1) {
            // The current soundtrack is not the first one
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            Audio::stop(input.currentSoundtrack);
            input.currentSoundtrack--;
            Audio::play(input.currentSoundtrack, true);
        }
    }
    else {
        // Change the soundtrack selected in the list (right changing)
        if (input.currentSoundtrack != Audio::NUM_SOUNDTRACKS) {
            // The current soundtrack is not the last one
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            Audio::stop(input.currentSoundtrack);
            input.currentSoundtrack++;
            Audio::play(input.currentSoundtrack, true);
        }
    }
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuMusicRadio::loadMenu(Input& input){

    // Loading the background texture of the panel radio
    backgroundMusic.loadFromFile("Resources/Menus/MusicMenu/radioBackground.png");
    radioMenu.setTexture(backgroundMusic);
    radioMenu.setPosition(0, 0);
    radioMenu.setScale((float) input.gameWindow.getSize().x / backgroundMusic.getSize().x,
                       (float) input.gameWindow.getSize().y / backgroundMusic.getSize().y);

    // Load the titles of the soundtracks
    sf::Texture t;
    for (int i = 1; i <= Audio::NUM_SOUNDTRACKS; i++) {
        // Loading the icon texture
        t.loadFromFile("Resources/Menus/MusicMenu/soundtrack" + to_string(i) + ".png");
        musicTitleTextures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 1; i <= Audio::NUM_SOUNDTRACKS; i++) {
        // Loading the icon texture
        t.loadFromFile("Resources/Menus/MusicMenu/radio" + to_string(i) + ".png");
        radioTextures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 1; i <= (Audio::NUM_SOUNDTRACKS / 3); i++) {
        // Loading the icon texture
        t.loadFromFile("Resources/Menus/MusicMenu/hand" + to_string(i) + ".png");
        handTextures.push_back(t);
    }
}



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuMusicRadio::handleEvent(Input& input){
    sf::Event event;
    // Detect possible actions of the player
    input.gameWindow.pollEvent(event);
    if (input.closed(event)){
        // Close the game
        if (!escapePressed)
            escapePressed = true;
    }
    else {
        if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            // Soundtrack has been selected
            if (!startPressed){
                // Only can be select once
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            // The player returns back (cancel)
            if (!backPressed){
                // Only can be selected once
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_BACK, false);
            }
        }
        else if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT))
            // Change the soundtrack list to the left and plays the new song
            changeGameMusic(input, true);
        else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT))
            // Change the soundtrack list to the right and plays the new song
            changeGameMusic(input, false);
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuMusicRadio::draw(Input& input){

    // Select a default soundtracks and play it with the intro sfx
    input.currentSoundtrack = 1;
    Audio::play(Sfx::MENU_SELECTION_MUSIC, false);
    Audio::play(Sfx::WAVE, true);
    Audio::play(input.currentSoundtrack, true);

    // The music radio menu is in course
    while (!startPressed && !escapePressed && !backPressed){

        // Detect the possible actions of the player
        handleEvent(input);

        // Load the texture of the soundtrack to display in the radio panel
        music.setTexture(musicTitleTextures[input.currentSoundtrack - 1], true);
        music.setScale(3.0f * input.screenScaleX, 3.f * input.screenScaleX);

        // Get the dial movement to reproduce
        radio.setTexture(radioTextures[input.currentSoundtrack - 1], true);
        radio.setScale(3.f * input.screenScaleX, 3.f * float(input.gameWindow.getSize().y) / SCREEN_0.second);

        // Get the hand movement of the driver
        hand.setTexture(handTextures[(input.currentSoundtrack - 1) % 3], true);
        hand.setScale(3.f * input.screenScaleX, 3.f * float(input.gameWindow.getSize().y) / SCREEN_0.second);

        // Control the coordinates X and Y where display the title
        music.setPosition((input.gameWindow.getSize().x - music.getGlobalBounds().width) / 2.f,
                          (input.gameWindow.getSize().y - music.getGlobalBounds().height) / 2.f);
        radio.setPosition((input.gameWindow.getSize().x - radio.getGlobalBounds().width * 0.5f) / 2.0f,
                          input.gameWindow.getSize().y * 0.8725f - radio.getGlobalBounds().height);
        hand.setPosition((input.gameWindow.getSize().x - hand.getGlobalBounds().width * 0.66f) / 2.0f,
                         input.gameWindow.getSize().y - hand.getGlobalBounds().height);

        // Show the title of the soundtrack
        input.gameWindow.draw(radioMenu);
        input.gameWindow.draw(music);
        input.gameWindow.draw(radio);
        input.gameWindow.draw(hand);
        input.gameWindow.display();
    }

    // Stop the sfx and the soundtrack selected
    Audio::stop(Sfx::WAVE);
    Audio::stop(input.currentSoundtrack);
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 */
State MenuMusicRadio::returnMenu(Input& input){
    if (startPressed)
        // Loading animation
        return State::LOADING;
    else if (backPressed)
        // Vehicle menu (canceled)
        return State::VEHICLE;
    else if (escapePressed)
        // Game closed
        return State::EXIT;
}

