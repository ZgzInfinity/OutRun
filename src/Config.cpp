
#include "../include/Config.hpp"

/**
 * Assigns to the game the configuration read from the xml configuration file
 * @param difficulty is the difficulty level of the game
 * @param pixelArt controls if the graphics of the game have to be drawn with pixel art effect
 * @param fullScreen controls if the game is in full screen
 * @param axis_x is the width of the screen
 * @param axis_y is the height of the screen
 * @param controlLeft is the code of the key to move the vehicle to the left
 * @param controlLeft is the code of the key to move the vehicle to the right
 * @param controlAccelerate is the code of the key to accelerate the vehicle
 * @param controlSoundtrack is the code of the key to change the soundtrack of the game
 */
Config::Config(const Difficult difficulty, const bool pixelArt, const bool enabledAi, const bool fullScreen, const int axis_x,
               const int axis_y, const string controlLeft,const string controlRight, const string controlAccelerate,
               const string controlBrake, const int volEffects, const int volSoundtracks)
               : resolutions({SCREEN_DEFAULT, SCREEN_1, SCREEN_2, SCREEN_3, SCREEN_4, SCREEN_5}), camD(0.87), renderLen(450)
{
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

    // Calculate the factor of screen
    screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);

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

    // Store all the fonts of the text used by the game
    timeToPlay = initializeFontTimePlay();
    speedVehicle = initializeFontSpeed();
    options = initializeFontOptions();

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



State Config::graphicsMenu() {
    // Control if the start key is pressed or not
    bool startPressed = false;
    bool resized = false;

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
        Texture segaBackground, textureShape;
        segaBackground.loadFromFile("Resources/Menus/MenuOptions/segaIcon.png");
        segaBackground.setRepeated(true);
        textureShape.loadFromFile("Resources/Menus/MenuOptions/outrun.png");
        textureShape.setRepeated(true);

        IntRect background(0, 0, w.getSize().x, w.getSize().y);
        Sprite sprite(segaBackground, background);

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
        optionsText.setPosition(w.getSize().x / 2.f - 160.0f * screenScale, w.getSize().y / 2.f - 220.0f * screenScale);
        optionsText.setCharacterSize(static_cast<unsigned int>(int(35.0f * screenScale)));
        optionsText.setFont(options);
        optionsText.setStyle(Text::Bold | Text::Underlined);
        optionsText.setFillColor(Color::Red);

        // Option indicators

        menuButtons.emplace_back(w.getSize().x / 2.f - 270.0f * screenScale, w.getSize().y / 2.f - 70.0f * screenScale,
                                 200.0f * screenScale, 30.0f * screenScale, options,
                                 "Resolution", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1, screenScale);

        menuButtons.emplace_back(w.getSize().x / 2.f - 270.0f * screenScale, w.getSize().y / 2.f, 200.0f * screenScale,
                                 30.0f * screenScale, options,
                                 "Pixel art", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, screenScale);

        // Option configurations
        const string res = resIndex > -1 ? to_string(resolutions[resIndex].first) + "x" +
                                           to_string(resolutions[resIndex].second) : "FULLSCREEN";
        menuButtons.emplace_back(w.getSize().x / 2.f + 80.0f * screenScale, w.getSize().y / 2.f - 70.0f * screenScale,
                                 200.0f * screenScale, 30.0f * screenScale, options,
                                 res, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1, screenScale);

        menuButtons.emplace_back(w.getSize().x / 2.f + 80.0f * screenScale, w.getSize().y / 2.f, 200.0f * screenScale,
                                 30.0f * screenScale, options,
                                 enablePixelArt ? "ENABLED" : "DISABLED", Color(0, 255, 0), Color(255, 255, 0),
                                 Color(0, 255, 0), 0, screenScale);

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

            window.pollEvent(e);

            if (optionSelected == 0) {
                // Volume music
                // Check if left or right cursor keys have been pressed or not
                if (window.hasFocus() && Keyboard::isKeyPressed(leftKey)) {
                    if (resized) {
                        resized = false;
                    } else if (resIndex > -1) {
                        resIndex--;
                        menuButtons[optionSelected + 2].setTextButton(resIndex > -1 ?
                                                                      to_string(resolutions[resIndex].first) + "x" +
                                                                      to_string(resolutions[resIndex].second)
                                                                                    : "FULLSCREEN");
                        if (resIndex > -1) {
                            window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                                                    static_cast<unsigned int>(resolutions[resIndex].second)),
                                          "Out Run",
                                          Style::Titlebar | Style::Close);

                            fullScreen = false;
                        }
                        else {
                            window.create(VideoMode::getFullscreenModes()[0], "Out Run", Style::Fullscreen);
                            fullScreen = true;
                        }
                        window.setFramerateLimit(FPS);
                        window.setKeyRepeatEnabled(false);

                        isDefaultScreen = resIndex == 0;


                        window.setView(View(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f),
                                            Vector2f(window.getSize().x, window.getSize().y)));
                        w.create(static_cast<unsigned int>(window.getView().getSize().x),
                                 static_cast<unsigned int>(window.getView().getSize().y));

                        screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);
                        currentResized = true;
                        resized = true;
                    }
                } else if (window.hasFocus() && Keyboard::isKeyPressed(rightKey)) {
                    if (resized) {
                        resized = false;
                    }
                    if (resIndex < int(resolutions.size()) - 1 && !resized) {
                        resIndex++;
                        menuButtons[optionSelected + 2].setTextButton(to_string(resolutions[resIndex].first) + "x" +
                                                                      to_string(resolutions[resIndex].second));

                        window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                                                static_cast<unsigned int>(resolutions[resIndex].second)), "Out Run",
                                      Style::Titlebar | Style::Close);
                        window.setFramerateLimit(FPS);
                        window.setKeyRepeatEnabled(false);

                        isDefaultScreen = resIndex == 0;


                        window.setView(View(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f),
                                            Vector2f(window.getSize().x, window.getSize().y)));
                        w.create(static_cast<unsigned int>(window.getView().getSize().x),
                                 static_cast<unsigned int>(window.getView().getSize().y));

                        screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);
                        currentResized = true;
                        resized = true;
                        fullScreen = false;
                    }
                }
            } else {
                // Volume effects
                // Check if left or right cursor keys have been pressed or not
                if (window.hasFocus() && Keyboard::isKeyPressed(leftKey)) {
                    if (enablePixelArt) {
                        enablePixelArt = false;
                        menuButtons[optionSelected + 2].setTextButton("DISABLED");
                    }
                } else if (window.hasFocus() && Keyboard::isKeyPressed(rightKey)) {
                    if (!enablePixelArt) {
                        enablePixelArt = true;
                        menuButtons[optionSelected + 2].setTextButton("ENABLED");
                    }
                }
            }
            w.draw(sprite);
            w.draw(shape);
            w.draw(optionsText);

            // Show the buttons of the menu
            for (auto &menuButton : menuButtons) {
                menuButton.render(&w);
            }

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


sf::Font initializeFontTimePlay() {
    Font f;
    if (!f.loadFromFile("Resources/Fonts/DisposableDroid.ttf")) exit(1);
    return f;
}

sf::Font initializeFontSpeed() {
    Font f;
    if (!f.loadFromFile("Resources/Fonts/digital.ttf")) exit(1);
    return f;
}

sf::Font initializeFontOptions() {
    Font f;
    if (!f.loadFromFile("Resources/Fonts/needForSpeed.ttf")) exit(1);
    return f;
}




