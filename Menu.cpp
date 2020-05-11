/******************************************************************************
 * @file    Menu.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Menu.hpp"

#include <memory>

#define FPS 60

using namespace std;
using namespace sf;

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

    timeToPlay = initializeFontTimePlay();

    speedVehicle = initializeFontSpeed();

    options = initializeFontOptions();

    camD = 0.84; // Camera depth
    renderLen = 300;

    volumeEffects = 100;
    volumeMusic = 80;

    level = NORMAL;

    modifiedConfig = false;

    currentSoundtrack = 0;

    aggressiveness = 0;
}

State introAnimation(Config& c){
    // Load the game effects
    for (int i = 1; i <= 30; i++){
        unique_ptr<Music> effect = make_unique<Music>();
        effect->openFromFile("resources/SoundEffects/" + to_string(i) + ".ogg");
        effect->setVolume(100);
        c.effects.push_back(move(effect));
    }
    c.effects[29]->setLoop(true);

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
        sleep(milliseconds(40));
    }
    c.effects[26]->play();

    // Load the music soundtracks of the game
    for (int i = 0; i <= 4; i++){
        unique_ptr<Music> music = make_unique<Music>();
        music->openFromFile("resources/Soundtrack/" + to_string(i) + ".ogg");
        music->setVolume(90);
        music->setLoop(true);
        c.themes.push_back(move(music));
    }

    return START;
}

State startMenu(Config &c, bool startPressed) {
    const int ELEMENTS = 5;

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
    textElements[0].setPosition((c.w.getSize().x / 2.f) - 120, c.w.getSize().y / 2.f + 100);
    textElements[0].setCharacterSize(30);
    textElements[0].setFont(c.timeToPlay);
    textElements[0].setFillColor(Color::Green);
    textElements[0].setOutlineColor(Color::Black);
    textElements[0].setOutlineThickness(3);

    textElements[1].setString("START");
    textElements[1].setPosition((c.w.getSize().x / 2.f) - 50, c.w.getSize().y / 2.f + 70);
    textElements[1].setCharacterSize(30);
    textElements[1].setFont(c.timeToPlay);
    textElements[1].setFillColor(Color::Green);
    textElements[1].setOutlineColor(Color::Black);
    textElements[1].setOutlineThickness(3);

    textElements[2].setString("OPTIONS");
    textElements[2].setPosition(c.w.getSize().x / 2.f - 50, c.w.getSize().y / 2.f + 120);
    textElements[2].setCharacterSize(30);
    textElements[2].setFont(c.timeToPlay);
    textElements[2].setFillColor(Color::Green);
    textElements[2].setOutlineColor(Color::Black);
    textElements[2].setOutlineThickness(3);

    textElements[3].setString("1986");
    textElements[3].setPosition(c.w.getSize().x / 2.f + 270, c.w.getSize().y / 2.f + 290);
    textElements[3].setCharacterSize(30);
    textElements[3].setFont(c.timeToPlay);
    textElements[3].setFillColor(Color::Green);
    textElements[3].setOutlineColor(Color::Black);
    textElements[3].setOutlineThickness(3);

    textElements[4].setString("©SEGA");
    textElements[4].setPosition(c.w.getSize().x / 2.f + 350, c.w.getSize().y / 2.f +290);
    textElements[4].setCharacterSize(30);
    textElements[4].setFont(c.timeToPlay);
    textElements[4].setFillColor(Color::Green);
    textElements[4].setOutlineColor(Color::Black);
    textElements[4].setOutlineThickness(3);

    // Partial state of the game
    State state = START;

    // Change the background texture
    c.w.draw(mainMenu);

    // Code of sprite to display
    int j = 0;

    // While the console window is opened
    while (c.w.isOpen()) {

        // Detect the possible events
        Event e{};
        c.w.pollEvent(e);
        if (e.type == Event::Closed){
            c.w.close();
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
            c.w.draw(textElements[3]);
            c.w.draw(textElements[4]);
            c.w.display();
            sleep(milliseconds(180));

            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)){
                // Pass to the second menu
                startPressed = true;
                c.effects[1]->stop();
                c.effects[1]->play();
                sleep(milliseconds(50));
            }
            else if (Keyboard::isKeyPressed(Keyboard::Escape))
                return EXIT;

            j = ( j < (int)nameGames.size() - 1) ? j + 1 : 0;

        }

        // Control the second menu
        startPressed = false;
        sleep(milliseconds(200));
        state = MUSIC;

        // While the ENTER keyword is not pressed
        while (!startPressed){

            // Detect the possible events
            Event e{};
            c.w.waitEvent(e);
            if (e.type == Event::Closed){
                c.w.close();
            }

            // Control if the up or down cursor keys are pressed or not
            if (Keyboard::isKeyPressed(c.menuUpKey)){
                // Up cursor pressed
                if (state != MUSIC){
                    c.effects[0]->play();
                    row.setPosition((c.w.getSize().x / 2.f) - 100, c.w.getSize().y / 2.f + 75);
                    state = MUSIC;
                }
            }
            else if (Keyboard::isKeyPressed(c.menuDownKey)){
                // Down cursor pressed
                if (state != OPTIONS){
                    c.effects[0]->play();
                    row.setPosition((c.w.getSize().x / 2.f) - 100, c.w.getSize().y / 2.f + 125);
                    state = OPTIONS;
                }
            }

            // Show the menu with the starting and options indicators
            c.w.draw(mainMenu);

            c.w.draw(nameGames[j]);
            c.w.draw(textElements[1]);
            c.w.draw(textElements[2]);
            c.w.draw(textElements[3]);
            c.w.draw(textElements[4]);
            c.w.draw(row);
            c.w.display();
            sleep(milliseconds(180));
            c.effects[0]->stop();

            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)){
                // Pass to the second menu
                startPressed = true;
                c.effects[1]->stop();
                c.effects[1]->play();
            }

            j = ( j < (int)nameGames.size() - 1) ? j + 1 : 0;
        }
        // Return the state of the game
        c.effects[0]->stop();
        return state;
    }
    return EXIT;
}

void changeCarControllers(Config& c){

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    c.w.display();

    KeywordMapper kM = KeywordMapper();

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    c.w.display();

    // Loading the background texture
    Texture segaBackground, textureShape;
    segaBackground.loadFromFile("resources/MenuOptions/segaIcon.png");
    segaBackground.setRepeated(true);
    textureShape.loadFromFile("resources/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x,  c.w.getSize().y);
    Sprite sprite(segaBackground, background);

    RectangleShape shape;
    shape.setPosition((c.w.getSize().x / 2.f) - 300, c.w.getSize().y / 2.f - 250);
    shape.setSize(sf::Vector2f(610, 500));
    shape.setOutlineColor(Color( 19, 186, 251 ));
    shape.setOutlineThickness(5);
    shape.setTexture(&textureShape, true);

    vector<Button> menuButtons;

    // Main Text of the menu
    Text optionsText;
    optionsText.setString("CONTROLLERS");
    optionsText.setPosition(c.w.getSize().x / 2.f - 160, c.w.getSize().y / 2.f - 220);
    optionsText.setCharacterSize(35);
    optionsText.setFont(c.options);
    optionsText.setStyle(Text::Bold | Text::Underlined);
    optionsText.setFillColor(Color::Red);

    Text info1;
    info1.setString("Hold down Space to select a controller");
    info1.setFillColor(Color(10, 201, 235));
    info1.setOutlineColor(Color(3, 39, 8));
    info1.setOutlineThickness(3);
    info1.setCharacterSize(15);
    info1.setStyle(Text::Bold);
    info1.setPosition(c.w.getSize().x / 2.f - 200, c.w.getSize().y / 2.f - 160);
    info1.setFont(c.options);
    c.w.draw(info1);

    Text info2;
    info2.setString("Then press a key to change its configuration");
    info2.setFillColor(Color(10, 201, 235));
    info2.setOutlineColor(Color(3, 39, 8));
    info2.setCharacterSize(15);
    info2.setOutlineThickness(3);
    info2.setStyle(Text::Bold);
    info2.setPosition(c.w.getSize().x / 2.f - 265, c.w.getSize().y / 2.f - 120);
    info2.setFont(c.options);
    c.w.draw(info2);

    // Option indicators

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f - 70, 200, 30, c.options,
                                 "Turning left", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f, 200, 30, c.options,
                                 "Turning right", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f + 70, 200, 30, c.options,
                                 "Acceleration", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f + 140, 200, 30, c.options,
                                 "Brake", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    // Option configurations

    int code;
    code = kM.mapperCodeKeyWord[c.leftKey];
    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f - 70, 200, 30, c.options,
                                 kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1));

    code = kM.mapperCodeKeyWord[c.rightKey];
    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f, 200, 30, c.options,
                                 kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    code = kM.mapperCodeKeyWord[c.accelerateKey];
    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f + 70, 200, 30, c.options,
                                 kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    code = kM.mapperCodeKeyWord[c.brakeKey];
    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f + 140, 200, 30, c.options,
                                 kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));


    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Until the start keyword is not pressed
    while (!startPressed){
        Event e{};
        c.w.waitEvent(e);
        if (Keyboard::isKeyPressed(c.menuDownKey)){
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1) / 2){
                // Change the color appearance of both buttons
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 4].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 3].setButtonState(BUTTON_IDLE);
            }
        }
        else if (Keyboard::isKeyPressed(c.menuUpKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0){
                // Change the color appearance of both buttons
                c.effects[0]->play();
                optionSelected--;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 4].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 5].setButtonState(BUTTON_IDLE);
            }
        }
        while (Keyboard::isKeyPressed(Keyboard::Space) && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            // Check if any keyword has been pressed or not
            c.w.waitEvent(e);
            if (e.type == Event::KeyPressed && e.key.code != -1 && e.key.code != Keyboard::Enter && e.key.code != Keyboard::Space){
                // Modify the option parameter if it's necessary
                switch (optionSelected){
                    case 0:
                        if (kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.brakeKey)
                        {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        }
                        else {
                            menuButtons[optionSelected + 4].setTextButton(kM.mapperIdKeyWord[e.key.code]);
                            c.leftKey = kM.mapperCodeKeyWord[e.key.code];
                            c.effects[1]->stop();
                            c.effects[1]->play();
                        }
                        break;
                    case 1:
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (kM.mapperCodeKeyWord[e.key.code] == c.leftKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.brakeKey)
                        {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        }
                        else {
                            menuButtons[optionSelected + 4].setTextButton(kM.mapperIdKeyWord[e.key.code]);
                            c.rightKey = kM.mapperCodeKeyWord[e.key.code];
                            c.effects[1]->stop();
                            c.effects[1]->play();
                        }
                        break;
                    case 2:
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (kM.mapperCodeKeyWord[e.key.code] == c.leftKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.brakeKey)
                        {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        }
                        else {
                            menuButtons[optionSelected + 4].setTextButton(kM.mapperIdKeyWord[e.key.code]);
                            c.accelerateKey = kM.mapperCodeKeyWord[e.key.code];
                            c.effects[1]->stop();
                            c.effects[1]->play();
                        }
                        break;
                    case 3:
                        // Get the code of the keyword if it's not the up pr down cursor keys
                        if (kM.mapperCodeKeyWord[e.key.code] == c.leftKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey)
                        {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        }
                        else {
                            menuButtons[optionSelected + 4].setTextButton(kM.mapperIdKeyWord[e.key.code]);
                            c.brakeKey = kM.mapperCodeKeyWord[e.key.code];
                            c.effects[1]->stop();
                            c.effects[1]->play();
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        c.w.draw(sprite);
        c.w.draw(shape);
        c.w.draw(optionsText);
        c.w.draw(info1);
        c.w.draw(info2);

        // Show the buttons of the menu
        for (int i = 0; i < (int)menuButtons.size(); i++){
            menuButtons.at(i).render(&c.w);
        }

        c.w.display();
        sleep(milliseconds(120));
        c.effects[0]->stop();

        // Check if left or right cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)){
            // Change the controllers of the car
            startPressed = true;
            c.modifiedConfig = true;
            c.effects[2]->stop();
            c.effects[2]->play();
        }
    }
}

State optionsMenu(Config& c, const bool& inGame){
    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    c.w.display();

    c.themes[0]->play();

    // Loading the background texture
    Texture segaBackground, textureShape;
    segaBackground.loadFromFile("resources/MenuOptions/segaIcon.png");
    segaBackground.setRepeated(true);
    textureShape.loadFromFile("resources/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x,  c.w.getSize().y);
    Sprite sprite(segaBackground, background);

    RectangleShape shape;
    shape.setPosition((c.w.getSize().x / 2.f) - 350, c.w.getSize().y / 2.f - 250);
    shape.setSize(sf::Vector2f(710, 535));
    shape.setOutlineColor(Color( 19, 186, 251 ));
    shape.setOutlineThickness(5);
    shape.setTexture(&textureShape, true);

    vector<Button> menuButtons;

    // Main Text of the menu
    Text optionsText;
    optionsText.setString("OPTIONS");
    optionsText.setPosition((c.w.getSize().x / 2.f) - 90, c.w.getSize().y / 2.f - 230);
    optionsText.setCharacterSize(35);
    optionsText.setFont(c.options);
    optionsText.setStyle(Text::Bold | Text::Underlined);
    optionsText.setFillColor(Color::Red);

    // Option indicators

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f - 130, 200, 30, c.options,
                                 "Difficulty", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f - 60, 200, 30, c.options,
                                 "AI aggressiveness", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f + 10, 200, 30, c.options,
                                 "Music volume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f + 80, 200, 30, c.options,
                                 "Volume effect", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 270, c.w.getSize().y / 2.f + 150, 200, 30, c.options,
                                 "Controllers", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    // Option configurations

    string difficulty;
    switch (c.level) {
        case PEACEFUL:
            difficulty = "Peaceful";
            break;
        case EASY:
            difficulty = "Easy";
            break;
        case HARD:
            difficulty = "Hard";
            break;
        default:
            difficulty = "Normal";
            break;
    }

    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f - 130, 200, 30, c.options,
                                 difficulty, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f - 60, 200, 30, c.options,
                                 to_string(c.aggressiveness), Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f + 10, 200, 30, c.options,
                                 to_string(c.volumeMusic), Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f + 80, 200, 30, c.options,
                                 to_string(c.volumeEffects), Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f + 80, c.w.getSize().y / 2.f + 150, 200, 30, c.options,
                                 "Default", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));


    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Until the start keyword is not pressed
    while (!startPressed){
        // Check if the up or down cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuDownKey)){
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1) / 2){
                // Change the color appearance of both buttons
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 5].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 4].setButtonState(BUTTON_IDLE);
            }
        }
        else if (Keyboard::isKeyPressed(c.menuUpKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0){
                c.effects[0]->play();
                optionSelected--;
                // Change the color appearance of both buttons
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 5].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 6].setButtonState(BUTTON_IDLE);
            }
        }


        // Modify the option parameter if it's necessary
        switch (optionSelected){
            case 0:
                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.level != PEACEFUL){
                        // Change the difficult level
                        if (c.level == EASY) {
                            c.level = PEACEFUL;
                            menuButtons[optionSelected + 5].setTextButton("Peaceful");
                        }
                        if (c.level == NORMAL) {
                            c.level = EASY;
                             menuButtons[optionSelected + 5].setTextButton("Easy");
                        }
                        else if (c.level == HARD) {
                            c.level = NORMAL;
                            menuButtons[optionSelected + 5].setTextButton("Normal");
                        }
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.level != HARD){
                        // Change the difficult level
                        if (c.level == PEACEFUL) {
                            c.level = EASY;
                            menuButtons[optionSelected + 5].setTextButton("Easy");
                        }
                        if (c.level == EASY) {
                            c.level = NORMAL;
                            menuButtons[optionSelected + 5].setTextButton("Normal");
                        }
                        else if (c.level == NORMAL){
                            c.level = HARD;
                            menuButtons[optionSelected + 5].setTextButton("Hard");
                        }
                    }
                }
                break;
            case 1:
                // AI aggressiveness level
                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.aggressiveness >= 5){
                        c.aggressiveness -= 5;
                        menuButtons[optionSelected + 5].setTextButton((to_string(c.aggressiveness)));
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.aggressiveness <= 95){
                        c.aggressiveness += 5;
                        menuButtons[optionSelected + 5].setTextButton((to_string(c.aggressiveness)));
                    }
                }
                break;
            case 2:
                // Volume music
                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.volumeMusic != 0){
                       c.volumeMusic--;
                       menuButtons[optionSelected + 5].setTextButton((to_string(c.volumeMusic)));
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.volumeMusic != 100){
                        c.volumeMusic++;
                        menuButtons[optionSelected + 5].setTextButton((to_string(c.volumeMusic)));
                    }
                }
                break;
            case 3:
                // Volume effects
                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(c.leftKey)){
                    if (c.volumeEffects != 0){
                       c.volumeEffects--;
                       menuButtons[optionSelected + 5].setTextButton((to_string(c.volumeEffects)));
                    }
                }
                else if (Keyboard::isKeyPressed(c.rightKey)){
                    if (c.volumeEffects != 100){
                        c.volumeEffects++;
                        menuButtons[optionSelected + 5].setTextButton((to_string(c.volumeEffects)));
                    }
                }
                if (c.modifiedConfig){
                    menuButtons[optionSelected + 6].setTextButton("Saved!");
                }
                else {
                    menuButtons[optionSelected + 6].setTextButton("Default");
                }
                break;
            case 4:
                // Change the controllers of the game
                menuButtons[optionSelected + 5].setTextButton("Press C!");

                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(Keyboard::C)){
                    // Change the controllers of the car
                    c.effects[1]->stop();
                    c.effects[1]->play();
                    changeCarControllers(c);
                    if (c.modifiedConfig){
                         menuButtons[optionSelected + 5].setTextButton("Saved!");
                    }
                }
        }

        c.w.draw(sprite);
        c.w.draw(shape);
        c.w.draw(optionsText);

        // Show the buttons of the menu
        for (int i = 0; i < (int)menuButtons.size(); i++){
            menuButtons.at(i).render(&c.w);
        }

        c.w.display();
        sleep(milliseconds(120));
        c.effects[0]->stop();

        // Check if left or right cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)){
            // Change the controllers of the car
            startPressed = true;
            c.effects[1]->stop();
            c.effects[1]->play();
        }
    }
    c.themes[0]->stop();
    if (inGame){
        return GAME;
    }
    else {
        return startMenu(c, true);
    }
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
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++){
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/soundtrack" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++){
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/radio" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++){
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/hand" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Control if the Enter key is pressed
    bool startPressed = false;

    // Stop sounds
    c.themes[0]->stop();
    c.themes[1]->stop();
    c.themes[2]->stop();

    c.currentSoundtrack = 1;
    c.themes[c.currentSoundtrack]->play();

    c.effects[29]->play();

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
            if (c.currentSoundtrack != 1){
                c.effects[1]->stop();
                c.effects[1]->play();
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack--;
                c.themes[c.currentSoundtrack]->play();
            }
        }
        else if (Keyboard::isKeyPressed(c.rightKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (c.currentSoundtrack != NUM_SOUNDTRACKS - 1){
                c.effects[1]->stop();
                c.effects[1]->play();
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack++;
                c.themes[c.currentSoundtrack]->play();
            }
        }

        // Load the texture of the soundtrack to display in the radio panel
        music.setTexture(textures[c.currentSoundtrack - 1], true);

        // Get the dial movement to reproduce
        radio.setTexture(textures[c.currentSoundtrack + 2], true);

        // Get the hand movement of the driver
        hand.setTexture(textures[c.currentSoundtrack + 5], true);

        // Control the coordinates X and Y where display the title
        switch(c.currentSoundtrack){
            case 1:
                // First soundtrack
                music.setPosition((c.w.getSize().x / 2.f) - 300, c.w.getSize().y / 2.f - 70);
                radio.setPosition((c.w.getSize().x / 2.f) - 88, c.w.getSize().y / 2.f + 170);
                hand.setPosition((c.w.getSize().x / 2.f) - 120, c.w.getSize().y / 2.f + 170);
                break;
            case 2:
                // Second soundtrack
                music.setPosition((c.w.getSize().x / 2.f) - 230, c.w.getSize().y / 2.f - 70);
                radio.setPosition((c.w.getSize().x / 2.f) - 88, c.w.getSize().y / 2.f + 170);
                hand.setPosition((c.w.getSize().x / 2.f) - 120, c.w.getSize().y / 2.f + 170);
                break;
            case 3:
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
            c.effects[2]->stop();
            c.effects[2]->play();
        }
    }
    c.effects[29]->stop();
    c.themes[0]->stop();
    return GAME;
}

State endMenu(Config &c) {
    return EXIT;
}


State rankingMenu(Config& c, const unsigned long scorePlayerGame, const int minutes, const int secs, const int cents_Second){

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    c.w.display();

    KeywordMapper kM = KeywordMapper();

    c.themes[c.currentSoundtrack]->stop();

    Clock rankingTime, blinkStart;

    Time ranking_delay = seconds(1.f);

    int time = 28;
    float elapsed1, elapsed2, elapsed3, elapsed4;

    c.themes[4]->play();

    Text rankingTitle;
    rankingTitle.setFont(c.timeToPlay);
    rankingTitle.setPosition(c.w.getSize().x / 4.f, c.w.getSize().y / 17.f);
    rankingTitle.setString("BEST OUTRUNNERS");
    rankingTitle.setCharacterSize(65);
    rankingTitle.setFillColor(Color::Yellow);
    rankingTitle.setOutlineColor(Color(12, 12, 12));
    rankingTitle.setOutlineThickness(3);

    Text scoreIndicator;
    scoreIndicator.setFont(c.timeToPlay);
    scoreIndicator.setPosition(c.w.getSize().x / 8.f, c.w.getSize().y / 6.0f);
    scoreIndicator.setString("SCORE");
    scoreIndicator.setCharacterSize(50);
    scoreIndicator.setFillColor(Color(146, 194, 186));
    scoreIndicator.setOutlineColor(Color(12, 12, 12));
    scoreIndicator.setOutlineThickness(3);

    Text playerIndicator;
    playerIndicator.setFont(c.timeToPlay);
    playerIndicator.setPosition(c.w.getSize().x / 2.2f, c.w.getSize().y / 6.0f);
    playerIndicator.setString("NAME");
    playerIndicator.setCharacterSize(50);
    playerIndicator.setFillColor(Color(146, 194, 186));
    playerIndicator.setOutlineColor(Color(12, 12, 12));
    playerIndicator.setOutlineThickness(3);

    Text recordIndicator;
    recordIndicator.setFont(c.timeToPlay);
    recordIndicator.setPosition((c.w.getSize().x / 2.f) * 1.5f, c.w.getSize().y / 6.0f);
    recordIndicator.setString("RECORD");
    recordIndicator.setCharacterSize(50);
    recordIndicator.setFillColor(Color(146, 194, 186));
    recordIndicator.setOutlineColor(Color(12, 12, 12));
    recordIndicator.setOutlineThickness(3);

    Text timeCounter;
    timeCounter.setFont(c.timeToPlay);
    timeCounter.setCharacterSize(62);
    timeCounter.setString(to_string(time));
    timeCounter.setPosition((c.w.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width, c.w.getSize().y / 15.7f);
    timeCounter.setFillColor(Color::Red);
    timeCounter.setOutlineColor(Color(12, 12, 12));
    timeCounter.setOutlineThickness(3);

    Text scorePlayer;
    scorePlayer.setFont(c.timeToPlay);
    scorePlayer.setCharacterSize(35);
    scorePlayer.setFillColor(Color(146, 194, 186));
    scorePlayer.setOutlineColor(Color::Black);
    scorePlayer.setOutlineThickness(3);

    Text namePlayer;
    namePlayer.setFont(c.timeToPlay);
    namePlayer.setCharacterSize(35);
    namePlayer.setFillColor(Color(146, 194, 186));
    namePlayer.setOutlineColor(Color(12, 12, 12));
    namePlayer.setOutlineThickness(3);

    Text minutesPlayer;
    minutesPlayer.setFont(c.timeToPlay);
    minutesPlayer.setCharacterSize(35);
    minutesPlayer.setFillColor(Color(146, 194, 186));
    minutesPlayer.setOutlineColor(Color::Black);
    minutesPlayer.setOutlineThickness(3);

    Text secondsPlayer;
    secondsPlayer.setFont(c.timeToPlay);
    secondsPlayer.setCharacterSize(35);
    secondsPlayer.setFillColor(Color(146, 194, 186));
    secondsPlayer.setOutlineColor(Color::Black);
    secondsPlayer.setOutlineThickness(3);

    Text centsPlayer;
    centsPlayer.setFont(c.timeToPlay);
    centsPlayer.setCharacterSize(35);
    centsPlayer.setFillColor(Color(146, 194, 186));
    centsPlayer.setOutlineColor(Color::Black);
    centsPlayer.setOutlineThickness(3);

    Text index;
    index.setFont(c.timeToPlay);
    index.setCharacterSize(35);
    index.setFillColor(Color(180, 130, 211));
    index.setOutlineColor(Color::Black);
    index.setOutlineThickness(3);

    Text start;
    start.setCharacterSize(45);
    start.setFont(c.timeToPlay);
    start.setFillColor(Color::Green);
    start.setOutlineColor(Color::Black);
    start.setOutlineThickness(3);

    // Loading the background texture
    Texture rankingBackground;
    rankingBackground.loadFromFile("resources/RankingMenu/bg.png");
    rankingBackground.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x,  c.w.getSize().y);
    Sprite palm_trees(rankingBackground, background);

    // Get the best seventh out runners
    vector<Score> scoreRankingPlayer = getGlobalScores();

    // Check if there is a new record
    int record = isNewRecord(scoreRankingPlayer, scorePlayerGame);
    bool shownPlayers = false;
    bool startPressed = false;
    bool blink = false;
    int lettersIntroduced = 0;
    string name = "_";

    rankingTime.restart();
    elapsed1 = rankingTime.getElapsedTime().asSeconds();

    c.effects[29]->play();

    while (time > 0 && !startPressed){

        // Get the actual time
        elapsed2 = rankingTime.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed2 - elapsed1 >= ranking_delay.asSeconds()) {
            // Draw time
            time--;
            rankingTime.restart();
            timeCounter.setString(to_string(time));
            timeCounter.setPosition((c.w.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width, c.w.getSize().y / 15.7f);
        }

        c.w.draw(palm_trees);
        c.w.draw(rankingTitle);
        c.w.draw(timeCounter);
        c.w.draw(scoreIndicator);
        c.w.draw(playerIndicator);
        c.w.draw(recordIndicator);

        // There is no new record
        if (record == -1){

            start.setString("PRESS START!");
            start.setPosition(c.w.getSize().x / 2.5f, (c.w.getSize().y / 4.5f) + 400);

            // There is not a new record
            for (int i = 1; i <= 7; i++){

                index.setString(to_string(i) + ".");
                index.setPosition((c.w.getSize().x / 13.f) - index.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);

                scorePlayer.setString(to_string(scoreRankingPlayer[i -1].score));
                scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);

                namePlayer.setString(scoreRankingPlayer[i -1].name);
                namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);

                minutesPlayer.setString(to_string(scoreRankingPlayer[i -1].minutes) + "'");
                minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);

                if (scoreRankingPlayer[i -1].secs >= 10){
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i -1].secs) + "''");
                }
                else {
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i -1].secs) + "''");
                }
                secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);


                if (scoreRankingPlayer[i -1].cents_second >= 10){
                    centsPlayer.setString(to_string(scoreRankingPlayer[i -1].cents_second));
                }
                else {
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i -1].cents_second));
                }
                centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);

                c.w.draw(index);
                c.w.draw(scorePlayer);
                c.w.draw(namePlayer);
                c.w.draw(minutesPlayer);
                c.w.draw(secondsPlayer);
                c.w.draw(centsPlayer);
            }
        }
        else {

            if (lettersIntroduced != 3){
                start.setString("ENTER YOUR NAME!");
                start.setPosition(c.w.getSize().x / 3.5f, (c.w.getSize().y / 4.5f) + 400);
            }
            else {
                start.setString("PRESS START!");
                start.setPosition(c.w.getSize().x / 2.5f, (c.w.getSize().y / 4.5f) + 400);
            }

            // There is a new record
            // Show all the out runners with a higher score

            for (int i = 0; i <= record - 1; i++ && record != 0){

                index.setString(to_string(i) + ".");
                index.setPosition((c.w.getSize().x / 13.f) - index.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * i);

                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 1));

                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 1));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 1));

                if (scoreRankingPlayer[i].secs >= 10){
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                }
                else {
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");
                }
                secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 1));


                if (scoreRankingPlayer[i].cents_second >= 10){
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                }
                else {
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));
                }
                centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 1));

                c.w.draw(scorePlayer);
                c.w.draw(namePlayer);
                c.w.draw(minutesPlayer);
                c.w.draw(secondsPlayer);
                c.w.draw(centsPlayer);
            }

            int offset = (record == 0) ? 1 : record + 1;

            // Show the actual player
            scorePlayer.setString(to_string(scorePlayerGame));
            scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * offset);

            namePlayer.setString(name);
            namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * offset);

            minutesPlayer.setString(to_string(minutes) + "'");
            minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * offset);

            if (secs >= 10){
                secondsPlayer.setString(to_string(secs) + "''");
            }
            else {
                secondsPlayer.setString("0" + to_string(secs) + "''");
            }
            secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * offset);


            if (cents_Second >= 10){
                centsPlayer.setString(to_string(cents_Second));
            }
            else {
                centsPlayer.setString("0" + to_string(cents_Second));
            }
            centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * offset);

            c.w.draw(scorePlayer);
            c.w.draw(namePlayer);
            c.w.draw(minutesPlayer);
            c.w.draw(secondsPlayer);
            c.w.draw(centsPlayer);

            // Show the rest of out runners

            for (int i = record; i < 6; i++){
                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 2));

                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 2));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 2));

                if (scoreRankingPlayer[i].secs >= 10){
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                }
                else {
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");
                }
                secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 2));


                if (scoreRankingPlayer[i].cents_second >= 10){
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                }
                else {
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));
                }
                centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width, (c.w.getSize().y / 4.5f) + 50 * (i + 2));

                c.w.draw(scorePlayer);
                c.w.draw(namePlayer);
                c.w.draw(minutesPlayer);
                c.w.draw(secondsPlayer);
                c.w.draw(centsPlayer);
            }
        }
        elapsed4 = blinkStart.getElapsedTime().asSeconds();
        if (elapsed4 - elapsed3 >= ranking_delay.asSeconds()){
            blink = !blink;
            blinkStart.restart();
        }
        if (blink){
            start.setFillColor(Color::Green);
            start.setOutlineColor(Color::Black);
        }
        else {
            start.setFillColor(Color::Transparent);
            start.setOutlineColor(Color::Transparent);
        }

        c.w.draw(start);
        c.w.display();

        if (lettersIntroduced != 3){
            // while there are pending events...
            Event event;
            while (c.w.pollEvent(event)){
                if (event.type == Event::KeyPressed){
                    // Get code of the key
                    int code = event.key.code;
                    // Check if the key pressed is a letter or not
                    if (code >= 0 && code <= 25){
                        lettersIntroduced++;
                        string keyLetter = kM.mapperIdKeyWord[code];
                        if (name == "_"){
                            name = keyLetter;
                            c.effects[1]->stop();
                            c.effects[1]->play();
                        }
                        else {
                            if (lettersIntroduced == 3){
                                name = name.substr(0, name.size() - 1);
                                name += keyLetter;
                                c.effects[2]->stop();
                                c.effects[2]->play();
                            }
                            else {
                                name += keyLetter + "_";
                                c.effects[1]->stop();
                                c.effects[1]->play();
                            }
                        }
                    }
                    else {
                        c.effects[3]->stop();
                        c.effects[3]->play();
                    }
                }
            }
        }

        if (Keyboard::isKeyPressed(c.menuEnterKey)){
            startPressed = true;
        }
    }

    // Store the record
    if (record != -1){
        // If the was record and the name is uncompleted
        if (lettersIntroduced != 3){
            name = "   ";
        }
        Score s = Score(scorePlayerGame, name, minutes, secs, cents_Second);
        saveNewRecord(scoreRankingPlayer, s);
    }

    c.effects[2]->stop();
    c.effects[2]->play();
    c.effects[29]->stop();
    c.themes[4]->stop();

    return START;
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

sf::Font initializeFontOptions(){
    Font f;
    if (!f.loadFromFile("resources/fonts/needForSpeed.ttf")) exit(1);
    return f;
}
