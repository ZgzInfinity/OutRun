/******************************************************************************
 * @file    Menu.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Menu.hpp"

#define TEXTSIZE 50
#define FPS 60


Config::Config() {
    w.create(VideoMode(921, 691), "Outrun Racing!", Style::Titlebar | Style::Close);
    w.setFramerateLimit(FPS);

    menuKey = Keyboard::Escape;
    menuUpKey = Keyboard::Up;
    menuDownKey = Keyboard::Down;
    menuEnterKey = Keyboard::Enter;
    accelerateKey = Keyboard::LControl;
    brakeKey = Keyboard::LAlt;
    leftKey = Keyboard::Left;
    rightKey = Keyboard::Right;

    font = arial();

    timeToPlay = initializeFontTimePlay();

    speedVehicle = initializeFontSpeed();

    camD = 0.84; // Camera depth
    renderLen = 300;


    for (int i = 0; i < 3; i++){
        // Load the music soundtracks of the game
        unique_ptr<Music> music = unique_ptr<Music>(new Music());
        switch(i){
            case 0:
                music->openFromFile("resources/Audio/MagicalSoundShower.wav");
                break;
            case 1:
                music->openFromFile("resources/Audio/PassingBreeze.wav");
                break;
            case 2:
                music->openFromFile("resources/Audio/SplashWave.wav");
        }
        music->setVolume(100);
        music->setLoop(true);
        themes.push_back(std::move(music));
    }
}

void changeConfig(Config &c) {

}



State introAnimation(Config& c){
    // Vector of images with the logo of Sega
    Texture t;
    vector<Texture> segaIcons;
    Sprite segaIcon;

    // Initialize the position of the sega sprite
    segaIcon.setPosition(340, 300);

    // Iterate throughout all the icons of sega
    for (int i = 1; i < NUM_SEGA_ICONS; i++){
        // Loading the icon texture
        t.loadFromFile("resources/SegaAnimation/segaLogo" + to_string(i) + ".png");
        segaIcons.push_back(t);
        // Load the texture in the sprite reseting the last texture
        segaIcon.setTexture(segaIcons.at(i - 1), true);
        c.w.draw(segaIcon);
        // Show the logos in the console
        c.w.display();
        // Sleep the process to see the menu icons correctly
        sleep(milliseconds(35));
    }
    // Final of animation
    return START;
}



State startMenu(Config &c) {

    const int ELEMENTS = 4;

    // Clean the console window
    c.w.clear();
    c.w.display();

    Texture backgroundMenu, gameIcon, rowSelector;
    Sprite mainMenu, nameGame, row;

    vector<Texture> gameIcons;
    vector<Sprite> nameGames;

    // Loading the background texture
    backgroundMenu.loadFromFile("resources/MainMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0 , 0);
    mainMenu.setScale(2.9, 3.1);

    for (int i = 2; i <= 7; i++){
        // Loading the texture of the game's name
        gameIcon.loadFromFile("resources/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    for (int i = 0; i < 6; i++){
        // Loading the texture of the game's name
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((c.w.getSize().x / 2.f) - 180, c.w.getSize().y / 2.f - 200);
        nameGame.setScale(2.0, 2.0);
        nameGames.push_back(nameGame);
    }


    // Loading the texture of the game's name
    rowSelector.loadFromFile("resources/MainMenu/row.png");
    row.setTexture(rowSelector);
    row.setScale(0.06, 0.06);
    row.setPosition((c.w.getSize().x / 2.f) - 100, c.w.getSize().y / 2.f + 75);

    // Options of the main menu
    Text textElements[ELEMENTS];
    textElements[0].setString("PRESS START BUTTON");
    textElements[0].setPosition((c.w.getSize().x / 2.f) - 150, c.w.getSize().y / 2.f + 100);
    textElements[0].setCharacterSize(30);
    textElements[0].setFont(c.font);
    textElements[0].setFillColor(Color::Green);
    textElements[0].setOutlineColor(Color::Black);
    textElements[0].setOutlineThickness(3);

    textElements[1].setString("START");
    textElements[1].setPosition((c.w.getSize().x / 2.f) - 50, c.w.getSize().y / 2.f + 70);
    textElements[1].setCharacterSize(30);
    textElements[1].setFont(c.font);
    textElements[1].setFillColor(Color::Green);
    textElements[1].setOutlineColor(Color::Black);
    textElements[1].setOutlineThickness(3);

    textElements[2].setString("OPTIONS");
    textElements[2].setPosition((c.w.getSize().x / 2.f) - 50, c.w.getSize().y / 2.f + 120);
    textElements[2].setCharacterSize(30);
    textElements[2].setFont(c.font);
    textElements[2].setFillColor(Color::Green);
    textElements[2].setOutlineColor(Color::Black);
    textElements[2].setOutlineThickness(3);


    // Partial state of the game
    State state = START;

    // Change the background texture
    c.w.draw(mainMenu);

    // Control for press the enter key
    bool startPressed = false;

    // Code of sprite to display
    int j = 0;

    // While the console window is opened
    while (c.w.isOpen()) {
        // Detect the possible events
        Event e{};
        while (c.w.pollEvent(e)) {
            if (e.type == Event::Closed){
                c.w.close();
            }
        }

        // While the ENTER keyword is not pressed
        while (!startPressed){

            // Change the color of the main text
            if (textElements[0].getFillColor() == Color::Green){
                // Change color to transparent
                textElements[0].setFillColor(Color::Transparent);
                textElements[0].setOutlineColor(Color::Transparent);
            }
            else {
                // Change color to transparent
                textElements[0].setFillColor(Color::Green);
                textElements[0].setOutlineColor(Color::Black);
            }
            // Show the press start title in the menu
            c.w.draw(mainMenu);
            c.w.draw(nameGames[j]);
            c.w.draw(textElements[0]);
            c.w.display();
            sleep(milliseconds(180));

            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)){
                // Pass to the second menu
                startPressed = true;
            }

            j = ( j < (int)nameGames.size() - 1) ? j + 1 : 0;

        }

        // Control the second menu
        startPressed = false;
        sleep(milliseconds(50));
        state = MUSIC;

        // While the ENTER keyword is not pressed
        while (!startPressed){
            // Control if the up or down cursor keys are pressed or not
            if (Keyboard::isKeyPressed(c.menuUpKey)){
                // Up cursor pressed
                row.setPosition((c.w.getSize().x / 2.f) - 100, c.w.getSize().y / 2.f + 75);
                state = MUSIC;
            }
            else if (Keyboard::isKeyPressed(c.menuDownKey)){
                // Down cursor pressed7
                row.setPosition((c.w.getSize().x / 2.f) - 100, c.w.getSize().y / 2.f + 125);
                state = OPTIONS;
            }

            // Show the menu with the starting and options indicators
            c.w.draw(mainMenu);

            c.w.draw(nameGames[j]);
            c.w.draw(textElements[1]);
            c.w.draw(textElements[2]);
            c.w.draw(row);
            c.w.display();
            sleep(milliseconds(180));

            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)){
                // Pass to the second menu
                startPressed = true;
            }

            j = ( j < (int)nameGames.size() - 1) ? j + 1 : 0;
        }
        // Return the state of the game
        return state;
    }
    return EXIT;
}



void changeCarControllers(Config& c){

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    c.w.display();

    KeywordMapper kM = KeywordMapper();

    // Loading the background texture
    Texture segaBackground;
    segaBackground.loadFromFile("resources/MenuOptions/background.png");
    segaBackground.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x,  c.w.getSize().y);
    Sprite sprite(segaBackground, background);

    RectangleShape shape;
    shape.setPosition((c.w.getSize().x / 2.f) - 330, c.w.getSize().y / 2.f - 180);
    shape.setSize(sf::Vector2f(670, 375));
    shape.setOutlineColor(Color( 19, 186, 251 ));
    shape.setOutlineThickness(5);
    shape.setFillColor(Color(10, 10, 12));

    // Main Text of the menu
    Text optionsText;
    optionsText.setString("CONTROLLERS");
    optionsText.setPosition((c.w.getSize().x / 2.f) - 90, c.w.getSize().y / 2.f - 170);
    optionsText.setCharacterSize(25);
    optionsText.setFont(c.font);
    optionsText.setFillColor(Color::Red);

    // Option indicators

    // Main Text of the menu
    Text controlLeft;
    controlLeft.setString("Turning left");
    controlLeft.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f - 120);
    controlLeft.setCharacterSize(25);
    controlLeft.setFont(c.font);
    controlLeft.setFillColor(Color::Yellow);

    // Main Text of the menu
    Text controlRight;
    controlRight.setString("Turning right");
    controlRight.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f - 40);
    controlRight.setCharacterSize(20);
    controlRight.setFont(c.font);
    controlRight.setFillColor(Color::White);

    // Main Text of the menu
    Text controlAccelerate;
    controlAccelerate.setString("Acceleration");
    controlAccelerate.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f + 40);
    controlAccelerate.setCharacterSize(20);
    controlAccelerate.setFont(c.font);
    controlAccelerate.setFillColor(Color::White);

    // Main Text of the menu
    Text controlBrake;
    controlBrake.setString("Brake");
    controlBrake.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f + 120);
    controlBrake.setCharacterSize(20);
    controlBrake.setFont(c.font);
    controlBrake.setFillColor(Color::White);

    // Default keyword configuration
    Text leftOption;
    leftOption.setString("LCRTL");
    leftOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f - 120);
    leftOption.setCharacterSize(25);
    leftOption.setFont(c.font);
    leftOption.setFillColor(Color::Yellow);

    Text rightOption;
    rightOption.setString("LALT");
    rightOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f - 40);
    rightOption.setCharacterSize(20);
    rightOption.setFont(c.font);
    rightOption.setFillColor(Color::White);

    Text accelerateOption;
    accelerateOption.setString("UP");
    accelerateOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f + 40);
    accelerateOption.setCharacterSize(20);
    accelerateOption.setFont(c.font);
    accelerateOption.setFillColor(Color::White);

    Text brakeOption;
    brakeOption.setString("DOWN");
    brakeOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f + 120);
    brakeOption.setCharacterSize(20);
    brakeOption.setFont(c.font);
    brakeOption.setFillColor(Color::White);

    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 1;

    // Until the start keyword is not pressed
    while (!startPressed){

        // Check if the up or down cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuDownKey)){
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != NUM_CONTROLLERS){
                optionSelected++;
            }
        }
        else if (Keyboard::isKeyPressed(c.menuUpKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 1){
                optionSelected--;
            }
        }

        // Modify the option parameter if it's necessary
        Event e;
        switch (optionSelected){
            case 1:
                // Left control
                controlLeft.setCharacterSize(25);
                leftOption.setCharacterSize(25);
                controlLeft.setFillColor(Color::Yellow);
                leftOption.setFillColor(Color::Yellow);
                controlRight.setCharacterSize(20);
                rightOption.setCharacterSize(20);
                controlRight.setFillColor(Color::White);
                rightOption.setFillColor(Color::White);

                while (c.w.pollEvent(e)) {
                    // Check if any keyword has been pressed or not
                    if (e.type == Event::KeyPressed && e.key.code != Keyboard::Up && e.key.code != Keyboard::Down){
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (e.key.code != -1){
                            if (kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.brakeKey)
                            {
                                int indexCode = kM.lookForIdKeyWord(c.leftKey);
                                leftOption.setString("SELECTED!");
                                c.w.draw(sprite);
                                c.w.draw(shape);
                                c.w.draw(optionsText);
                                c.w.draw(controlLeft);
                                c.w.draw(controlRight);
                                c.w.draw(controlAccelerate);
                                c.w.draw(controlBrake);

                                c.w.draw(leftOption);
                                c.w.draw(rightOption);
                                c.w.draw(accelerateOption);
                                c.w.draw(brakeOption);
                                c.w.display();
                                sleep(milliseconds(200));
                                leftOption.setString(kM.mapperIdKeyWord[indexCode]);
                            }
                            else {
                                leftOption.setString(kM.mapperIdKeyWord[e.key.code]);
                                c.leftKey = kM.mapperCodeKeyWord[e.key.code];
                            }
                        }
                        else {
                            leftOption.setString("UNKNOWN!");
                        }
                    }
                }
                break;
            case 2:
                // Right control
                controlLeft.setCharacterSize(20);
                leftOption.setCharacterSize(20);
                controlLeft.setFillColor(Color::White);
                leftOption.setFillColor(Color::White);
                controlRight.setCharacterSize(25);
                rightOption.setCharacterSize(25);
                controlRight.setFillColor(Color::Yellow);
                rightOption.setFillColor(Color::Yellow);
                controlAccelerate.setCharacterSize(20);
                accelerateOption.setCharacterSize(20);
                controlAccelerate.setFillColor(Color::White);
                accelerateOption.setFillColor(Color::White);

                while (c.w.pollEvent(e)) {
                    if (e.type == Event::KeyPressed && e.key.code != Keyboard::Up && e.key.code != Keyboard::Down){
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (e.key.code != -1){
                            if (kM.mapperCodeKeyWord[e.key.code] == c.leftKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.brakeKey)
                            {
                                int indexCode = kM.lookForIdKeyWord(c.rightKey);
                                rightOption.setString("SELECTED!");
                                c.w.draw(sprite);
                                c.w.draw(shape);
                                c.w.draw(optionsText);
                                c.w.draw(controlLeft);
                                c.w.draw(controlRight);
                                c.w.draw(controlAccelerate);
                                c.w.draw(controlBrake);

                                c.w.draw(leftOption);
                                c.w.draw(rightOption);
                                c.w.draw(accelerateOption);
                                c.w.draw(brakeOption);
                                c.w.display();
                                sleep(milliseconds(200));
                                rightOption.setString(kM.mapperIdKeyWord[indexCode]);
                            }
                            else {
                                rightOption.setString(kM.mapperIdKeyWord[e.key.code]);
                                c.rightKey = kM.mapperCodeKeyWord[e.key.code];
                            }
                        }
                        else {
                            rightOption.setString("UNKNOWN!");
                        }
                    }
                }
                break;
            case 3:
                // Volume music
                controlRight.setCharacterSize(20);
                rightOption.setCharacterSize(20);
                controlRight.setFillColor(Color::White);
                rightOption.setFillColor(Color::White);
                controlAccelerate.setCharacterSize(25);
                accelerateOption.setCharacterSize(25);
                controlAccelerate.setFillColor(Color::Yellow);
                accelerateOption.setFillColor(Color::Yellow);
                controlBrake.setCharacterSize(20);
                brakeOption.setCharacterSize(20);
                controlBrake.setFillColor(Color::White);
                brakeOption.setFillColor(Color::White);

                while (c.w.pollEvent(e)) {
                    if (e.type == Event::KeyPressed && e.key.code != Keyboard::Left && e.key.code != Keyboard::Right &&
                        e.key.code != Keyboard::Up && e.key.code != Keyboard::Down){
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (e.key.code != -1){
                            if (kM.mapperCodeKeyWord[e.key.code] == c.leftKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.brakeKey)
                            {
                                int indexCode = kM.lookForIdKeyWord(c.accelerateKey);
                                accelerateOption.setString("SELECTED!");
                                c.w.draw(sprite);
                                c.w.draw(shape);
                                c.w.draw(optionsText);
                                c.w.draw(controlLeft);
                                c.w.draw(controlRight);
                                c.w.draw(controlAccelerate);
                                c.w.draw(controlBrake);

                                c.w.draw(leftOption);
                                c.w.draw(rightOption);
                                c.w.draw(accelerateOption);
                                c.w.draw(brakeOption);
                                c.w.display();
                                sleep(milliseconds(200));
                                accelerateOption.setString(kM.mapperIdKeyWord[indexCode]);
                            }
                            else {
                                accelerateOption.setString(kM.mapperIdKeyWord[e.key.code]);
                                c.accelerateKey = kM.mapperCodeKeyWord[e.key.code];
                            }
                        }
                        else {
                            accelerateOption.setString("UNKNOWN!");
                        }
                    }
                }
                break;
            case 4:
                // Effect music
                controlAccelerate.setCharacterSize(20);
                accelerateOption.setCharacterSize(20);
                controlAccelerate.setFillColor(Color::White);
                accelerateOption.setFillColor(Color::White);
                controlBrake.setCharacterSize(25);
                brakeOption.setCharacterSize(25);
                controlBrake.setFillColor(Color::Yellow);
                brakeOption.setFillColor(Color::Yellow);

                while (c.w.pollEvent(e)) {
                    if (e.type == Event::KeyPressed && e.key.code != Keyboard::Left && e.key.code != Keyboard::Right &&
                        e.key.code != Keyboard::Up && e.key.code != Keyboard::Down){
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (e.key.code != -1){
                            if (kM.mapperCodeKeyWord[e.key.code] == c.leftKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                                kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey)
                            {
                                int indexCode = kM.lookForIdKeyWord(c.brakeKey);
                                brakeOption.setString("SELECTED!");
                                c.w.draw(sprite);
                                c.w.draw(shape);
                                c.w.draw(optionsText);
                                c.w.draw(controlLeft);
                                c.w.draw(controlRight);
                                c.w.draw(controlAccelerate);
                                c.w.draw(controlBrake);

                                c.w.draw(leftOption);
                                c.w.draw(rightOption);
                                c.w.draw(accelerateOption);
                                c.w.draw(brakeOption);
                                c.w.display();
                                sleep(milliseconds(200));
                                brakeOption.setString(kM.mapperIdKeyWord[indexCode]);
                            }
                            else {
                                brakeOption.setString(kM.mapperIdKeyWord[e.key.code]);
                                c.brakeKey = kM.mapperCodeKeyWord[e.key.code];
                            }
                        }
                        else {
                            brakeOption.setString("UNKNOWN!");
                        }
                    }
                }
        }
        c.w.draw(sprite);
        c.w.draw(shape);
        c.w.draw(optionsText);
        c.w.draw(controlLeft);
        c.w.draw(controlRight);
        c.w.draw(controlAccelerate);
        c.w.draw(controlBrake);

        c.w.draw(leftOption);
        c.w.draw(rightOption);
        c.w.draw(accelerateOption);
        c.w.draw(brakeOption);

        c.w.display();
        sleep(milliseconds(120));

        // Check if left or right cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)){
            // Change the controllers of the car
            startPressed = true;
            c.modifiedConfig = true;
        }
    }
}



State optionsMenu(Config& c){

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    c.w.display();

    // Loading the background texture
    Texture segaBackground;
    segaBackground.loadFromFile("resources/MenuOptions/background.png");
    segaBackground.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x,  c.w.getSize().y);
    Sprite sprite(segaBackground, background);

    RectangleShape shape;
    shape.setPosition((c.w.getSize().x / 2.f) - 330, c.w.getSize().y / 2.f - 180);
    shape.setSize(sf::Vector2f(670, 375));
    shape.setOutlineColor(Color( 19, 186, 251 ));
    shape.setOutlineThickness(5);
    shape.setFillColor(Color(10, 10, 12));

    // Main Text of the menu
    Text optionsText;
    optionsText.setString("OPTIONS");
    optionsText.setPosition((c.w.getSize().x / 2.f) - 60, c.w.getSize().y / 2.f - 170);
    optionsText.setCharacterSize(25);
    optionsText.setFont(c.font);
    optionsText.setFillColor(Color::Red);

    // Option indicators

    // Main Text of the menu
    Text difficultText;
    difficultText.setString("Difficulty");
    difficultText.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f - 130);
    difficultText.setCharacterSize(25);
    difficultText.setFont(c.font);
    difficultText.setFillColor(Color::Yellow);

    // Main Text of the menu
    Text trafficText;
    trafficText.setString("Traffic");
    trafficText.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f - 60);
    trafficText.setCharacterSize(20);
    trafficText.setFont(c.font);
    trafficText.setFillColor(Color::White);

    // Main Text of the menu
    Text volumeText;
    volumeText.setString("Music volume");
    volumeText.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f + 10);
    volumeText.setCharacterSize(20);
    volumeText.setFont(c.font);
    volumeText.setFillColor(Color::White);

    // Main Text of the menu
    Text effectText;
    effectText.setString("Volume effect");
    effectText.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f + 80);
    effectText.setCharacterSize(20);
    effectText.setFont(c.font);
    effectText.setFillColor(Color::White);

    // Main Text of the menu
    Text controllers;
    controllers.setString("Controllers");
    controllers.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f + 150);
    controllers.setCharacterSize(20);
    controllers.setFont(c.font);
    controllers.setFillColor(Color::White);

    // Option configurations

    Text difficultOption;
    difficultOption.setString("Easy");
    difficultOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f - 130);
    difficultOption.setCharacterSize(25);
    difficultOption.setFont(c.font);
    difficultOption.setFillColor(Color::Yellow);

    Text trafficOption;
    trafficOption.setString("Yes");
    trafficOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f - 60);
    trafficOption.setCharacterSize(20);
    trafficOption.setFont(c.font);
    trafficOption.setFillColor(Color::White);

    Text volumeOption;
    volumeOption.setString("100%");
    volumeOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f + 10);
    volumeOption.setCharacterSize(20);
    volumeOption.setFont(c.font);
    volumeOption.setFillColor(Color::White);

    Text effectOption;
    effectOption.setString("100%");
    effectOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f + 80);
    effectOption.setCharacterSize(20);
    effectOption.setFont(c.font);
    effectOption.setFillColor(Color::White);

    Text controllerOption;
    controllerOption.setString("Default");
    controllerOption.setPosition((c.w.getSize().x / 2.f) + 130, c.w.getSize().y / 2.f + 150);
    controllerOption.setCharacterSize(20);
    controllerOption.setFont(c.font);
    controllerOption.setFillColor(Color::White);

    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 1;

    // Until the start keyword is not pressed
    while (!startPressed){

        // Check if the up or down cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuDownKey)){
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != NUM_OPTIONS){
                optionSelected++;
            }
        }
        else if (Keyboard::isKeyPressed(c.menuUpKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 1){
                optionSelected--;
            }
        }

        // Modify the option parameter if it's necessary
        switch (optionSelected){
            case 1:
                // Difficult level
                difficultText.setCharacterSize(25);
                difficultOption.setCharacterSize(25);
                difficultText.setFillColor(Color::Yellow);
                difficultOption.setFillColor(Color::Yellow);
                trafficText.setCharacterSize(20);
                trafficOption.setCharacterSize(20);
                trafficText.setFillColor(Color::White);
                trafficOption.setFillColor(Color::White);

                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.level != EASY){
                        // Change the difficult level to medium or easy
                        if (c.level == MEDIUM){
                            c.level = EASY;
                            difficultOption.setString("Easy");
                        }
                        else if (c.level == HARD){
                            c.level = MEDIUM;
                            difficultOption.setString("Medium");
                        }
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.level != HARD){
                        // Change the difficult level to medium or easy
                        if (c.level == EASY){
                            c.level = MEDIUM;
                            difficultOption.setString("Medium");
                        }
                        else if (c.level == MEDIUM){
                            c.level = HARD;
                            difficultOption.setString("Hard");
                        }
                    }
                }
                break;
            case 2:
                // Traffic control
                difficultText.setCharacterSize(20);
                difficultOption.setCharacterSize(20);
                difficultText.setFillColor(Color::White);
                difficultOption.setFillColor(Color::White);
                trafficText.setCharacterSize(25);
                trafficOption.setCharacterSize(25);
                trafficText.setFillColor(Color::Yellow);
                trafficOption.setFillColor(Color::Yellow);
                volumeText.setCharacterSize(20);
                volumeOption.setCharacterSize(20);
                volumeText.setFillColor(Color::White);
                volumeOption.setFillColor(Color::White);

                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (!c.trafficControl){
                       c.trafficControl = true;
                       trafficOption.setString("Yes");
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.trafficControl){
                       c.trafficControl = false;
                       trafficOption.setString("No");
                    }
                }
                break;
            case 3:
                // Volume music
                trafficText.setCharacterSize(20);
                trafficOption.setCharacterSize(20);
                trafficText.setFillColor(Color::White);
                trafficOption.setFillColor(Color::White);
                volumeText.setCharacterSize(25);
                volumeOption.setCharacterSize(25);
                volumeText.setFillColor(Color::Yellow);
                volumeOption.setFillColor(Color::Yellow);
                effectText.setCharacterSize(20);
                effectOption.setCharacterSize(20);
                effectText.setFillColor(Color::White);
                effectOption.setFillColor(Color::White);

                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.volumeMusic != 0){
                       c.volumeMusic--;
                       volumeOption.setString(to_string(c.volumeMusic) + "%");
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.volumeMusic != 100){
                        c.volumeMusic++;
                        volumeOption.setString(to_string(c.volumeMusic) + "%");
                    }
                }
                break;
            case 4:
                // Effect music
                volumeText.setCharacterSize(20);
                volumeOption.setCharacterSize(20);
                volumeText.setFillColor(Color::White);
                volumeOption.setFillColor(Color::White);
                effectText.setCharacterSize(25);
                effectOption.setCharacterSize(25);
                effectText.setFillColor(Color::Yellow);
                effectOption.setFillColor(Color::Yellow);
                controllers.setCharacterSize(20);
                controllerOption.setCharacterSize(20);
                controllers.setFillColor(Color::White);
                controllerOption.setFillColor(Color::White);
                if (c.modifiedConfig){
                    controllerOption.setString("Saved!");
                }
                else {
                    controllerOption.setString("Default");
                }

                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.volumeEffects != 0){
                       c.volumeEffects--;
                       effectOption.setString(to_string(c.volumeEffects) + "%");
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.volumeEffects != 100){
                        c.volumeEffects++;
                        effectOption.setString(to_string(c.volumeEffects) + "%");
                    }
                }
                break;
            case 5:
                // Controllers of the car
                effectText.setCharacterSize(20);
                effectOption.setCharacterSize(20);
                effectText.setFillColor(Color::White);
                effectOption.setFillColor(Color::White);
                controllers.setCharacterSize(25);
                controllerOption.setCharacterSize(25);
                controllers.setFillColor(Color::Yellow);
                controllerOption.setFillColor(Color::Yellow);

                // Change the controllers of the game
                controllerOption.setString("Press C!");

                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(Keyboard::C)){
                    // Change the controllers of the car
                    changeCarControllers(c);

                    if (c.modifiedConfig){
                        controllerOption.setString("Saved!");
                    }
                }
        }

        c.w.draw(sprite);
        c.w.draw(shape);
        c.w.draw(optionsText);
        c.w.draw(difficultText);
        c.w.draw(trafficText);
        c.w.draw(volumeText);
        c.w.draw(effectText);
        c.w.draw(controllers);

        c.w.draw(difficultOption);
        c.w.draw(trafficOption);
        c.w.draw(volumeOption);
        c.w.draw(effectOption);
        c.w.draw(controllerOption);

        c.w.display();
        sleep(milliseconds(120));


        // Check if left or right cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)){
            // Change the controllers of the car
            startPressed = true;
        }
    }
    return GAME;
}




State selectMusicSoundtrack(Config &c){
    // Clean the console window
    c.w.clear();
    c.w.display();

    Texture backgroundMusic;
    Sprite radioMenu;

    // List of available soundtracks
    Texture t;
    vector<Texture> textures;
    // Sprite to see the title of the music
    Sprite music;

    // Sprite to see the dial movement of the radio
    Sprite radio;

    // Sprite to see the hand movements of the driver
    Sprite hand;

    // Buffer to reproduce the soundtracks
    SoundBuffer sound;

    // Loading the background texture of the panel radio
    backgroundMusic.loadFromFile("resources/MusicMenu/radioBackground.png");
    radioMenu.setTexture(backgroundMusic);
    radioMenu.setPosition(0 , 0);
    radioMenu.setScale(2.9, 3.1);

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS; i++){
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/soundtrack" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS; i++){
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/radio" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS; i++){
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/hand" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Control if the Enter key is pressed
    bool startPressed = false;

    c.themes[c.currentSoundtrack]->play();

    // Control until the Enter key is pressed
    while (!startPressed){

        // Detect the possible events
        Event e{};
        while (c.w.pollEvent(e)) {
            if (e.type == Event::Closed){
                c.w.close();
            }
        }

        // Control if the left or right cursor keys are pressed or not
        if (Keyboard::isKeyPressed(c.leftKey)){
            // Up cursor pressed and change the soundtrack selected in the list
            if (c.currentSoundtrack != 0){
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack--;
                c.themes[c.currentSoundtrack]->play();
            }
        }
        else if (Keyboard::isKeyPressed(c.rightKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (c.currentSoundtrack != NUM_SOUNDTRACKS - 1){
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack++;
                c.themes[c.currentSoundtrack]->play();
            }

        }

        // Load the texture of the soundtrack to display in the radio panel
        music.setTexture(textures[c.currentSoundtrack], true);

        // Get the dial movement to reproduce
        radio.setTexture(textures[c.currentSoundtrack + 3], true);

        // Get the hand movement of the driver
        hand.setTexture(textures[c.currentSoundtrack + 6], true);

        // Control the coordinates X and Y where display the title
        switch(c.currentSoundtrack){
            case 0:
                // First soundtrack
                music.setPosition((c.w.getSize().x / 2.f) - 300, c.w.getSize().y / 2.f - 70);
                radio.setPosition((c.w.getSize().x / 2.f) - 88, c.w.getSize().y / 2.f + 170);
                hand.setPosition((c.w.getSize().x / 2.f) - 120, c.w.getSize().y / 2.f + 170);
                break;
            case 1:
                // Second soundtrack
                music.setPosition((c.w.getSize().x / 2.f) - 230, c.w.getSize().y / 2.f - 70);
                radio.setPosition((c.w.getSize().x / 2.f) - 88, c.w.getSize().y / 2.f + 170);
                hand.setPosition((c.w.getSize().x / 2.f) - 120, c.w.getSize().y / 2.f + 170);
                break;
            case 2:
                // Third soundtrack
                music.setPosition((c.w.getSize().x / 2.f) - 200, c.w.getSize().y / 2.f - 70);
                radio.setPosition((c.w.getSize().x / 2.f) - 88, c.w.getSize().y / 2.f + 170);
                hand.setPosition((c.w.getSize().x / 2.f) - 120, c.w.getSize().y / 2.f + 170);
        }

        // Show the title of the soundtrack
        c.w.draw(radioMenu);
        c.w.draw(music);
        c.w.draw(radio);
        c.w.draw(hand);
        c.w.display();

        sleep(milliseconds(180));

        // Check if the keyword Enter has been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)){
            startPressed = true;
        }
    }
    c.themes[c.currentSoundtrack]->stop();
    return GAME;
}



State pauseMenu(Config& c) {
    return EXIT;
}

State endMenu(Config &c) {
    return EXIT;
}

sf::Font arial() {
    Font f;
    if (!f.loadFromFile("resources/fonts/arial.ttf")) exit(1);
    return f;
}


sf::Font initializeFontTimePlay() {
    Font f;
    if (!f.loadFromFile("resources/fonts/DisposableDroid.ttf")) exit(1);
    return f;
}



sf::Font initializeFontSpeed() {
    Font f;
    if (!f.loadFromFile("resources/fonts/digital.ttf")) exit(1);
    return f;
}

