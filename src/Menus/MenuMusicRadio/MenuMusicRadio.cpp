
/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
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

#include "MenuMusicRadio.h"


MenuMusicRadio::MenuMusicRadio() : Menu()
{
    optionSelected = 0;
}

void MenuMusicRadio::loadMenu(Input& input){
    // Loading the background texture of the panel radio
    backgroundMusic.loadFromFile("Resources/Menus/MusicMenu/radioBackground.png");
    radioMenu.setTexture(backgroundMusic);
    radioMenu.setPosition(0, 0);
    radioMenu.setScale((float) input.gameWindow.getSize().x / backgroundMusic.getSize().x,
                       (float) input.gameWindow.getSize().y / backgroundMusic.getSize().y);

    // Load the titles of the soundtracks
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


void MenuMusicRadio::changeGameMusic(Input& input, const bool& menuLeft){
    // Control if the left or right cursor keys are pressed or not
    if (menuLeft) {
        // Up cursor pressed and change the soundtrack selected in the list
        if (input.currentSoundtrack != 1) {
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            Audio::stop(input.currentSoundtrack);
            input.currentSoundtrack--;
            Audio::play(input.currentSoundtrack, true);
        }
    }
    else {
        // Down cursor pressed and change the soundtrack selected in the list
        if (input.currentSoundtrack != Audio::NUM_SOUNDTRACKS) {
            Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
            Audio::stop(input.currentSoundtrack);
            input.currentSoundtrack++;
            Audio::play(input.currentSoundtrack, true);
        }
    }
}

void MenuMusicRadio::handleEvent(Input& input){
    sf::Event event;
    input.gameWindow.pollEvent(event);
    if (input.closed(event)){
        if (!escapePressed)
            escapePressed = true;
    }
    else {
        if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (!startPressed){
                startPressed = true;
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
            }
        }
        else if (input.pressed(Key::MENU_CANCEL, event) && input.held(Key::MENU_CANCEL)){
            if (!backPressed){
                backPressed = true;
                Audio::play(Sfx::MENU_SELECTION_BACK, false);
            }
        }
        else if (input.pressed(Key::MENU_LEFT, event) && input.held(Key::MENU_LEFT)){
            changeGameMusic(input, true);
        }
        else if (input.pressed(Key::MENU_RIGHT, event) && input.held(Key::MENU_RIGHT)){
            changeGameMusic(input, false);
        }
    }
}

void MenuMusicRadio::draw(Input& input){

    Audio::play(Sfx::MENU_SELECTION_MUSIC, false);
    Audio::play(Sfx::WAVE, true);
    Audio::play(input.currentSoundtrack, true);

    while (!startPressed && !escapePressed && !backPressed){

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
    Audio::stop(Sfx::WAVE);
    Audio::stop(input.currentSoundtrack);
}



State MenuMusicRadio::returnMenu(Input& input){
    if (startPressed){
        return State::LOADING;
    }
    else if (backPressed){
        return State::GEARS;
    }
    else if (escapePressed){
        return State::EXIT;
    }
}

