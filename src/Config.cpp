/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
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
 * Module Configuration implementation file
 */


#include "../include/Config.hpp"



/**
 * Assigns to the game the configuration read from the xml configuration file
 * @param exitsSettings controls if there is a default configuration of the game
 * @param difficulty is the difficulty level of the game
 * @param pixelArt controls if the graphics of the game have to be drawn with pixel art effect
 * @param fullScreen controls if the game is in full screen
 * @param axis_x is the width of the screen
 * @param axis_y is the height of the screen
 * @param controlLeft is the code of the key to move the vehicle to the left
 * @param controlLeft is the code of the key to move the vehicle to the right
 * @param controlAccelerate is the code of the key to accelerate the vehicle
 * @param controlSoundtrack is the code of the key to change the soundtrack of the game
 * @param controlUpGear is the code of the key to change to a higher gear
 * @param controlLowGear is the code of the key to change to a lower gear
 */
Config::Config(const bool exitsSettings, const Difficult difficulty, const bool pixelArt, const bool enabledAi, const bool fullScreen, const int axis_x,
               const int axis_y, const string controlLeft,const string controlRight, const string controlAccelerate,
               const string controlBrake, const string controlUpGear, const string controlLowGear, const int volEffects, const int volSoundtracks)
               : resolutions({SCREEN_DEFAULT, SCREEN_1, SCREEN_2, SCREEN_3, SCREEN_4, SCREEN_5}), camD(0.87), renderLen(450)
{

    // Check if exits a default configuration for the game
    if (!exitsSettings){
        // Create a default configuration for the game
        string path = "Resources/Settings/Settings.info";
        writeDefaultConfiguration(path);

        resIndex = 0;

        // Create the screen with not full screen resolution
        window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                                static_cast<unsigned int>(resolutions[resIndex].second)), "Out Run",
                        Style::Titlebar | Style::Close);

        volumeEffects = 100;
        volumeMusic = 100;


        // Default configuration of the keys
        leftKey = Keyboard::Left;
        rightKey = Keyboard::Right;
        accelerateKey = Keyboard::LControl;
        brakeKey = Keyboard::LAlt;
        upGearKey = Keyboard::Z;
        lowGearKey = Keyboard::RControl;

        // Store the difficult of the game
        level = NORMAL;

        // By default any aspect of the configuration has been modified
        modifiedConfig = false;

        // Default level of AI aggressiveness
        maxAggressiveness = 0.0f;

        // AI always active
        enableAI = true;

        // Assigns the pixel art flag
        enablePixelArt = true;

        // Default screen size
        isDefaultScreen = true;
    }
    else {

        // Check if the screen is in default resolution or not
        if (axis_x == SCREEN_DEFAULT_X && axis_y == SCREEN_DEFAULT_Y){
            isDefaultScreen = true;
            resIndex = 0;
        }
        else {
            isDefaultScreen = false;
        }

        // Establish the index of the resolution currently used in the vector
        if (axis_x == SCREEN_1.first && axis_y == SCREEN_1.second){
            resIndex = 1;
        }
        else if (axis_x == SCREEN_2.first && axis_y == SCREEN_2.second){
            resIndex = 2;
        }
        else if (axis_x == SCREEN_3.first && axis_y == SCREEN_3.second){
            resIndex = 3;
        }
        else if (axis_x == SCREEN_4.first && axis_y == SCREEN_5.second){
            resIndex = 4;
        }
        else if (axis_x == SCREEN_4.first && axis_y == SCREEN_5.second){
            resIndex = 5;
        }

        // Check if it's full screen or not
        if (!fullScreen){
            // Create the screen with not full screen resolution
            window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                                    static_cast<unsigned int>(resolutions[resIndex].second)), "Out Run",
                          Style::Titlebar | Style::Close);
        }
        else {
            // Create a screen with full screen resolution
            window.create(VideoMode::getFullscreenModes()[0], "Out Run", Style::Fullscreen);
            resIndex = -1;
        }

        volumeEffects = volEffects;
        volumeMusic = volSoundtracks;

        // Initialize the keyword mapper
        int index;
        KeywordMapper kM = KeywordMapper();

        // Look for the keyboard to control the turning left
        index = kM.lookForKeyBoard(controlLeft);
        leftKey = kM.mapperCodeKeyWord[index];

        // Look for the keyboard to control the turning right
        index = kM.lookForKeyBoard(controlRight);
        rightKey = kM.mapperCodeKeyWord[index];

        // Look for the keyboard to control the acceleration
        index = kM.lookForKeyBoard(controlAccelerate);
        accelerateKey = kM.mapperCodeKeyWord[index];

        // Look for the keyboard to control the braking
        index = kM.lookForKeyBoard(controlBrake);
        brakeKey = kM.mapperCodeKeyWord[index];

        // Look for the keyboard to change to a higher gear
        index = kM.lookForKeyBoard(controlUpGear);
        upGearKey = kM.mapperCodeKeyWord[index];

        // Look for the keyboard to change to a lower gear
        index = kM.lookForKeyBoard(controlLowGear);
        lowGearKey = kM.mapperCodeKeyWord[index];

        // Store the difficult of the game
        level = difficulty;

        // By default any aspect of the configuration has been modified
        modifiedConfig = false;

        // Default level of AI aggressiveness
        maxAggressiveness = 0.0f;

        // AI always active
        enableAI = enabledAi;

        // Assigns the pixel art flag
        enablePixelArt = pixelArt;
    }

    // Set the FPS of the game
    window.setFramerateLimit(FPS);

    // Available repeated keys interruptions
    window.setKeyRepeatEnabled(false);

    // Creation of the view
    w.create(static_cast<unsigned int>(window.getView().getSize().x),
             static_cast<unsigned int>(window.getView().getSize().y));

    // Create the view to display the elements of the game
    window.setView(View(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f),
                          Vector2f(window.getSize().x, window.getSize().y)));
    w.create(static_cast<unsigned int>(window.getView().getSize().x),
               static_cast<unsigned int>(window.getView().getSize().y));

    // Assign icon to the screen of the game
    Image i;
    i.loadFromFile("Resources/Icon/OutRun.png");
    window.setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());

    // Calculate the factor of screen
    screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);

    // Assign the key controllers
    menuKey = Keyboard::Escape;
    menuUpKey = Keyboard::Up;
    menuDownKey = Keyboard::Down;
    menuEnterKey = Keyboard::Enter;

    // Store all the fonts of the text used by the game
    timeToPlay = initializeFontTimePlay();
    speedVehicle = initializeFontSpeed();
    options = initializeFontMenus();
}



/**
 * Represents on the screen the graphics menu and returns to options menu
 * @return
 */
State Config::graphicsMenu() {
    // Control if the start key is pressed or not
    bool startPressed = false;
    bool resized = false;

    // While the start button has not been pressed
    while (!startPressed) {
        bool currentResized = false;

        // Clean the console window
        w.clear(Color(0, 0, 0));
        Sprite bufferSprite(w.getTexture());
        w.display();
        window.draw(bufferSprite);
        window.display();

        // Clean the console window
        w.clear(Color(0, 0, 0));
        bufferSprite.setTexture(w.getTexture(), true);
        w.display();
        window.draw(bufferSprite);
        window.display();

        // Loading the background texture
        Texture iconBackground, textureShape;
        iconBackground.loadFromFile("Resources/Menus/MenuOptions/icon.png");
        IntRect background(0, 0, w.getSize().x, w.getSize().y);

        // Adapt the graphics menu to the different screen resolutions
        Sprite sprite(iconBackground, background);
        float axis_x = float(w.getSize().x) / SCREEN_DEFAULT_X;
        float axis_y = float(w.getSize().y) / SCREEN_DEFAULT_Y;
        sprite.setScale(axis_x, axis_y);

        textureShape.loadFromFile("Resources/Menus/MenuOptions/outrun.png");
        textureShape.setRepeated(true);

        RectangleShape shape;
        shape.setPosition((w.getSize().x / 2.f) - 300.0f * screenScale, w.getSize().y / 2.f - 250.0f * screenScale);
        shape.setSize(sf::Vector2f(610.0f * screenScale, 500.0f * screenScale));
        shape.setOutlineColor(Color(19, 186, 251));
        shape.setOutlineThickness(5.0f * screenScale);
        shape.setTexture(&textureShape, true);

        vector<Button> menuButtons;

        // Main Text of the menu
        Text optionsText;
        optionsText.setString("GRAPHICS MENU");
        optionsText.setCharacterSize(static_cast<unsigned int>(int(55.0f * screenScale)));
        optionsText.setFont(options);
        optionsText.setStyle(Text::Bold | Text::Underlined);
        optionsText.setFillColor(Color::Red);
        optionsText.setOutlineColor(Color(12, 12, 12));
        optionsText.setOutlineThickness(3.0f * screenScale);
        optionsText.setPosition((w.getSize().x / 2.f) - optionsText.getLocalBounds().width / 2.f,
                                 w.getSize().y / 2.f - 230.0f * screenScale);

        // Option indicators
        menuButtons.emplace_back(w.getSize().x / 2.f - 270.0f * screenScale, w.getSize().y / 2.f - 70.0f * screenScale,
                                 200.0f * screenScale, 30.0f * screenScale, options,
                                 "Resolution", Color(0, 255, 0), Color(255, 255, 0), 1, screenScale);

        menuButtons.emplace_back(w.getSize().x / 2.f - 270.0f * screenScale, w.getSize().y / 2.f, 200.0f * screenScale,
                                 30.0f * screenScale, options,
                                 "Pixel art", Color(0, 255, 0), Color(255, 255, 0), 0, screenScale);

        // Option configurations
        const string res = resIndex > -1 ? to_string(resolutions[resIndex].first) + "x" +
                                           to_string(resolutions[resIndex].second) : "FULLSCREEN";
        menuButtons.emplace_back(w.getSize().x / 2.f + 80.0f * screenScale, w.getSize().y / 2.f - 70.0f * screenScale,
                                 200.0f * screenScale, 30.0f * screenScale, options,
                                 res, Color(0, 255, 0), Color(255, 255, 0), 1, screenScale);

        menuButtons.emplace_back(w.getSize().x / 2.f + 80.0f * screenScale, w.getSize().y / 2.f, 200.0f * screenScale,
                                 30.0f * screenScale, options,
                                 enablePixelArt ? "ENABLED" : "DISABLED", Color(0, 255, 0), Color(255, 255, 0), 0, screenScale);

        // Control the option selected by the user
        int optionSelected = 0;

        // Until the start keyword is not pressed
        while (!startPressed && !currentResized) {
            // Detect the possible events
            Event e{};
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    return EXIT;
                }
            }

            window.pollEvent(e);
            if (window.hasFocus() && Keyboard::isKeyPressed(menuDownKey)) {
                // Up cursor pressed and change the soundtrack selected in the list
                if (optionSelected != int(menuButtons.size() - 1) / 2) {
                    // Change the color appearance of both buttons
                    effects[0]->play();
                    optionSelected++;
                    menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                    menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
                    menuButtons[optionSelected + 2].setButtonState(BUTTON_HOVER);
                    menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
                }
            } else if (window.hasFocus() && Keyboard::isKeyPressed(menuUpKey)) {
                // Down cursor pressed and change the soundtrack selected in the list
                if (optionSelected != 0) {
                    // Change the color appearance of both buttons
                    effects[0]->play();
                    optionSelected--;
                    menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                    menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
                    menuButtons[optionSelected + 2].setButtonState(BUTTON_HOVER);
                    menuButtons[optionSelected + 3].setButtonState(BUTTON_IDLE);
                }
            }

            // Receive an event of the screen
            window.pollEvent(e);

            if (optionSelected == 0) {
                // Screen resolutions
                // Check if left or left cursor keys have been pressed or not
                if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Left)) {
                    if (resized) {
                        // The screen has not been resized during the game before
                        resized = false;
                    } else if (resIndex > -1) {
                        // Modify the resolution of the screen
                        resIndex--;
                        menuButtons[optionSelected + 2].setTextButton(resIndex > -1 ?
                                                                      to_string(resolutions[resIndex].first) + "x" +
                                                                      to_string(resolutions[resIndex].second)
                                                                                    : "FULLSCREEN");

                        // Control if there are more resolutions on the left
                        if (resIndex > -1) {
                            window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                                                    static_cast<unsigned int>(resolutions[resIndex].second)),
                                          "Out Run",
                                          Style::Titlebar | Style::Close);
                        }
                        else {
                            // Create a new screen with the new resolution
                            window.create(VideoMode::getFullscreenModes()[0], "Out Run", Style::Fullscreen);
                        }

                        window.setFramerateLimit(FPS);
                        window.setKeyRepeatEnabled(false);

                        isDefaultScreen = resIndex == 0;

                        // Define the screen to represent correctly the interface
                        window.setView(View(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f),
                                            Vector2f(window.getSize().x, window.getSize().y)));
                        w.create(static_cast<unsigned int>(window.getView().getSize().x),
                                 static_cast<unsigned int>(window.getView().getSize().y));

                        // Calculation of the screen factor between the current resolution and the default resolution
                        screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);
                        currentResized = true;
                        resized = true;
                    }
                }
                // Check if the right cursor keys have been pressed or not
                else if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Right)) {
                    if (resized) {
                        // The screen has not been resized during the game before
                        resized = false;
                    }
                    if (resIndex < int(resolutions.size()) - 1 && !resized) {
                        resIndex++;
                        menuButtons[optionSelected + 2].setTextButton(to_string(resolutions[resIndex].first) + "x" +
                                                                      to_string(resolutions[resIndex].second));

                        // Modify the resolution of the screen
                        window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                                                static_cast<unsigned int>(resolutions[resIndex].second)), "Out Run",
                                      Style::Titlebar | Style::Close);
                        window.setFramerateLimit(FPS);
                        window.setKeyRepeatEnabled(false);

                        isDefaultScreen = resIndex == 0;

                        // Create a new screen with the new resolution
                        window.setView(View(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f),
                                            Vector2f(window.getSize().x, window.getSize().y)));
                        w.create(static_cast<unsigned int>(window.getView().getSize().x),
                                 static_cast<unsigned int>(window.getView().getSize().y));

                        screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);
                        currentResized = true;
                        resized = true;
                    }
                }
            } else {
                // Pixel art
                // Check if left or right cursor keys have been pressed or not
                if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Left)) {
                    if (enablePixelArt) {
                        enablePixelArt = false;
                        menuButtons[optionSelected + 2].setTextButton("DISABLED");
                    }
                } else if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Right)) {
                    if (!enablePixelArt) {
                        enablePixelArt = true;
                        menuButtons[optionSelected + 2].setTextButton("ENABLED");
                    }
                }
            }

            // Draw the elements of the menu
            w.draw(sprite);
            w.draw(shape);
            w.draw(optionsText);

            // Show the buttons of the menu
            for (auto &menuButton : menuButtons) {
                menuButton.render(&w);
            }

            // Display the menu in the screen
            bufferSprite.setTexture(w.getTexture(), true);
            w.display();
            window.draw(bufferSprite);
            window.display();
            sleep(milliseconds(120));
            effects[0]->stop();

            // Check if left or right cursor keys have been pressed or not
            if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Escape)) {
                // Change the controllers of the car
                startPressed = true;
                modifiedConfig = true;
                effects[2]->stop();
                effects[2]->play();
            }
        }
    }
    return OPTIONS;
}



/**
 * Write a default configuration for the game
 * @param path is the path with the default configuration of the game
 */
void Config::writeDefaultConfiguration(const string path){
        //  New file which stores the new configuration of the game
    ofstream f(path);
    // Check if the file has been opened correctly
    if (f.is_open()){
        // Write all the configuration of the game
        f << "DIFFICULTY: Normal" << endl;
        f << "AI: Enabled" << endl;
        f << "VOLUME_SOUNDTRACKS: 100" << endl;
        f << "VOLUME_EFFECTS: 100" << endl;
        f << "PIXEL_ART: Enabled" << endl;
        f << "FULL_SCREEN: Disabled" << endl;
        f << "RESOLUTION_X: 921" << endl;
        f << "RESOLUTION_Y: 691" << endl;
        f << "CONTROLLER_LEFT: LEFT CURSOR" << endl;
        f << "CONTROLLER_RIGHT: RIGHT CURSOR" << endl;
        f << "CONTROLLER_ACCELERATE: LCTRL" << endl;
        f << "CONTROLLER_BRAKE: LALT" << endl;
        f << "CONTROLLER_UP_GEAR: Z" << endl;
        f << "CONTROLLER_LOW_GEAR: RCTRL" << endl;
    }
    // Close the flux of the configuration file
    f.close();
}



/**
 * Returns the font used to write the time in the elapsed time panel
 * @return
 */
Font initializeFontTimePlay() {
    Font f;
    if (!f.loadFromFile("Resources/Fonts/DisposableDroid.ttf")) exit(1);
    return f;
}



/**
 * Returns the font used to represent the HUD during the game
 * @return
 */
Font initializeFontSpeed() {
    Font f;
    if (!f.loadFromFile("Resources/Fonts/digital.ttf")) exit(1);
    return f;
}



/**
 * Returns the font used to represent all the text indicators in
 * the animations of the game
 * @return
 */
Font initializeFontMenus() {
    Font f;
    if (!f.loadFromFile("Resources/Fonts/Hetikademo.otf")) exit(1);
    return f;
}




