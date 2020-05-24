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
// Screen
#define SCREEN_DEFAULT make_pair(SCREEN_DEFAULT_X, SCREEN_DEFAULT_Y)
// HD
#define SCREEN_1 make_pair(SCREEN_HD_X, SCREEN_HD_Y)
// HD+
#define SCREEN_2 make_pair(1366, 768)
// FULL HD
#define SCREEN_3 make_pair(1920, 1080)
// QHD
#define SCREEN_4 make_pair(2560, 1440)
// UHD
#define SCREEN_5 make_pair(3840, 2160)

using namespace std;
using namespace sf;

Config::Config() : resolutions({SCREEN_DEFAULT, SCREEN_1, SCREEN_2, SCREEN_3, SCREEN_4, SCREEN_5}), resIndex(0),
                   isDefaultScreen(true), camD(0.87), renderLen(300) {
    window.create(VideoMode(static_cast<unsigned int>(resolutions[resIndex].first),
                            static_cast<unsigned int>(resolutions[resIndex].second)), "Out Run",
                  Style::Titlebar | Style::Close);
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    if (isDefaultScreen)
        window.setView(View(Vector2f(SCREEN_DEFAULT_X / 4.0f, SCREEN_DEFAULT_Y / 4.0f),
                            Vector2f(SCREEN_DEFAULT_X / 2.0f, SCREEN_DEFAULT_Y / 2.0f)));
    else
        window.setView(View(Vector2f(SCREEN_HD_X / 4.0f, SCREEN_HD_Y / 4.0f),
                            Vector2f(SCREEN_HD_X / 2.0f, SCREEN_HD_Y / 2.0f)));
    w.create(static_cast<unsigned int>(window.getView().getSize().x),
             static_cast<unsigned int>(window.getView().getSize().y));

    Image i;
    i.loadFromFile("resources/Icon/OutRun.png");
    window.setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());

    screenScale = float(w.getSize().x) / float(SCREEN_DEFAULT_X);

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

    volumeEffects = 100;
    volumeMusic = 80;

    level = NORMAL;

    modifiedConfig = false;

    currentSoundtrack = 0;

    maxAggressiveness = 0.0f;
    enableAI = false;

    enablePixelArt = true;
}

State introAnimation(Config &c) {
    // Load the game effects
    for (int i = 1; i <= 30; i++) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }
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

    // Iterate throughout all the icons of sega
    for (int i = 1; i < NUM_SEGA_ICONS; i++) {

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }
        // Loading the icon texture
        t.loadFromFile("resources/SegaAnimation/segaLogo" + to_string(i) + ".png");
        segaIcons.push_back(t);
        // Load the texture in the sprite reseting the last texture
        segaIcon.setTexture(segaIcons.at(static_cast<unsigned long>(i - 1)), true);
        segaIcon.setScale(c.screenScale, c.screenScale);
        if (i == 1) {
            segaIcon.setPosition((c.w.getSize().x - segaIcon.getGlobalBounds().width) / 2.0f,
                                 (c.w.getSize().y - segaIcon.getGlobalBounds().height) / 2.0f);
        }
        c.w.draw(segaIcon);
        // Show the logos in the console
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        // Sleep the process to see the menu icons correctly
        sleep(milliseconds(40));
    }
    c.effects[26]->play();

    // Load the music soundtracks of the game
    for (int i = 0; i <= 5; i++) {
        unique_ptr<Music> music = make_unique<Music>();
        music->openFromFile("resources/Soundtrack/" + to_string(i) + ".ogg");
        music->setVolume(90);
        music->setLoop(true);
        c.themes.push_back(move(music));
    }

    return START;
}

State startMenu(Config &c, bool startPressed) {
    c.window.setView(View(Vector2f(c.window.getSize().x / 2.0f, c.window.getSize().y / 2.0f),
                          Vector2f(c.window.getSize().x, c.window.getSize().y)));
    c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
               static_cast<unsigned int>(c.window.getView().getSize().y));
    c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);

    const int ELEMENTS = 8;

    // Clean the console window
    c.w.clear();
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    Texture backgroundMenu, gameIcon, rowSelector;
    Sprite mainMenu, nameGame, row;

    vector<Texture> gameIcons;
    vector<Sprite> nameGames;

    // Loading the background texture
    backgroundMenu.loadFromFile("resources/MainMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) c.w.getSize().x / backgroundMenu.getSize().x,
                      (float) c.w.getSize().y / backgroundMenu.getSize().y);

    for (int i = 2; i <= 7; i++) {
        // Loading the texture of the game's name
        gameIcon.loadFromFile("resources/MainMenu/LogoMain" + to_string(i) + ".png");
        gameIcons.push_back(gameIcon);
    }

    for (int i = 0; i < 6; i++) {
        // Loading the texture of the game's name
        nameGame.setTexture(gameIcons[i], true);
        nameGame.setPosition((c.w.getSize().x / 2.f) - 180.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 200.0f * c.screenScale);
        nameGame.setScale(2.0f * c.screenScale, 2.0f * c.screenScale);
        nameGames.push_back(nameGame);
    }

    // Loading the texture of the game's name
    rowSelector.loadFromFile("resources/MainMenu/row.png");
    row.setTexture(rowSelector);
    row.setScale(0.06f * c.screenScale, 0.06f * c.screenScale);
    row.setPosition((c.w.getSize().x / 2.f) - 100.0f * c.screenScale, c.w.getSize().y / 2.f + 75.0f * c.screenScale);

    // Options of the main menu
    Text textElements[ELEMENTS];
    textElements[0].setString("PRESS ENTER KEY");
    textElements[0].setCharacterSize(static_cast<unsigned int>(int(40.0f * c.screenScale)));
    textElements[0].setFont(c.timeToPlay);
    textElements[0].setFillColor(Color::Green);
    textElements[0].setOutlineColor(Color::Black);
    textElements[0].setOutlineThickness(3.0f * c.screenScale);
    textElements[0].setPosition((c.w.getSize().x - textElements[0].getGlobalBounds().width) / 2.f,
                                c.w.getSize().y / 2.f + 100.0f * c.screenScale);

    textElements[1].setString("START");
    textElements[1].setPosition((c.w.getSize().x / 2.f) - 50.0f * c.screenScale,
                                c.w.getSize().y / 2.f + 70.0f * c.screenScale);
    textElements[1].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[1].setFont(c.timeToPlay);
    textElements[1].setFillColor(Color::Green);
    textElements[1].setOutlineColor(Color::Black);
    textElements[1].setOutlineThickness(3.0f * c.screenScale);

    textElements[2].setString("OPTIONS");
    textElements[2].setPosition(c.w.getSize().x / 2.f - 50.0f * c.screenScale,
                                c.w.getSize().y / 2.f + 120.0f * c.screenScale);
    textElements[2].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[2].setFont(c.timeToPlay);
    textElements[2].setFillColor(Color::Green);
    textElements[2].setOutlineColor(Color::Black);
    textElements[2].setOutlineThickness(3.0f * c.screenScale);

    textElements[4].setString("©SEGA");
    textElements[4].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[4].setFont(c.timeToPlay);
    textElements[4].setFillColor(Color::Green);
    textElements[4].setOutlineColor(Color::Black);
    textElements[4].setOutlineThickness(3.0f * c.screenScale);
    float initialX = c.w.getSize().x - 2.0f * textElements[4].getGlobalBounds().width, initialY =
            c.w.getSize().y - 2.0f * float(textElements[4].getCharacterSize());
    textElements[4].setPosition(initialX, initialY);

    textElements[3].setString("1986");
    textElements[3].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[3].setFont(c.timeToPlay);
    textElements[3].setFillColor(Color::Green);
    textElements[3].setOutlineColor(Color::Black);
    textElements[3].setOutlineThickness(3.0f * c.screenScale);
    textElements[3].setPosition(initialX - 1.5f * textElements[3].getGlobalBounds().width, initialY);

    initialX = textElements[4].getGlobalBounds().width;
    textElements[5].setString("EXIT / PAUSE: ESC KEY");
    textElements[5].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[5].setFont(c.timeToPlay);
    textElements[5].setFillColor(Color::Green);
    textElements[5].setOutlineColor(Color::Black);
    textElements[5].setOutlineThickness(3.0f * c.screenScale);
    textElements[5].setPosition(initialX, initialY);

    initialY -= 2.0f * textElements[5].getGlobalBounds().height;
    textElements[6].setString("MOVE: ARROW KEYS");
    textElements[6].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[6].setFont(c.timeToPlay);
    textElements[6].setFillColor(Color::Green);
    textElements[6].setOutlineColor(Color::Black);
    textElements[6].setOutlineThickness(3.0f * c.screenScale);
    textElements[6].setPosition(initialX, initialY);

    initialY -= 2.0f * textElements[6].getGlobalBounds().height;
    textElements[7].setString("SELECT: ENTER KEY");
    textElements[7].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[7].setFont(c.timeToPlay);
    textElements[7].setFillColor(Color::Green);
    textElements[7].setOutlineColor(Color::Black);
    textElements[7].setOutlineThickness(3.0f * c.screenScale);
    textElements[7].setPosition(initialX, initialY);

    // Partial state of the game
    State state = START;

    // Change the background texture
    c.w.draw(mainMenu);

    // Code of sprite to display
    int j = 0;

    float elapsed1, elapsed2;
    Clock blinkClcok;
    Time blink_delay = seconds(1.0);

    blinkClcok.restart().asSeconds();
    elapsed1 = blinkClcok.restart().asSeconds();
    bool blink = true;

    // While the console window is opened
    while (c.window.isOpen()) {
        // While the ENTER keyword is not pressed
        while (!startPressed) {
            // Detect the possible events
            Event e{};
            while (c.window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    return EXIT;
                }
            }


            elapsed2 = blinkClcok.getElapsedTime().asSeconds();

            // Change the color of the main text
            if (elapsed2 - elapsed1 >= blink_delay.asSeconds()) {
                blink = !blink;
                blinkClcok.restart();
            }
            if (blink) {
                textElements[0].setFillColor(Color::Green);
                textElements[0].setOutlineColor(Color::Black);
            } else {
                textElements[0].setFillColor(Color::Transparent);
                textElements[0].setOutlineColor(Color::Transparent);
            }

            // Show the press start title in the menu
            c.w.draw(mainMenu);
            c.w.draw(nameGames[j]);
            c.w.draw(textElements[0]);
            c.w.draw(textElements[3]);
            c.w.draw(textElements[4]);
            c.w.draw(textElements[5]);
            c.w.draw(textElements[6]);
            c.w.draw(textElements[7]);
            bufferSprite.setTexture(c.w.getTexture(), true);
            c.w.display();
            c.window.draw(bufferSprite);
            c.window.display();
            // sleep(milliseconds(180));

            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)) {
                // Pass to the second menu
                startPressed = true;
                c.effects[1]->stop();
                c.effects[1]->play();
                // sleep(milliseconds(50));
            } else if (Keyboard::isKeyPressed(Keyboard::Escape))
                return EXIT;

            j = (j < (int) nameGames.size() - 1) ? j + 1 : 0;

        }

        // Control the second menu
        startPressed = false;
        state = MUSIC;
        sleep(milliseconds(200));

        // While the ENTER keyword is not pressed
        while (!startPressed) {
            // Detect the possible events
            Event e{};
            while (c.window.pollEvent(e))
                if (e.type == Event::Closed)
                    return EXIT;

            // Control if the up or down cursor keys are pressed or not
            if (Keyboard::isKeyPressed(c.menuUpKey)) {
                // Up cursor pressed
                if (state != MUSIC) {
                    c.effects[0]->stop();
                    c.effects[0]->play();
                    row.setPosition((c.w.getSize().x / 2.f) - 100.0f * c.screenScale,
                                    c.w.getSize().y / 2.f + 75.0f * c.screenScale);
                    state = MUSIC;
                }
            } else if (Keyboard::isKeyPressed(c.menuDownKey)) {
                // Down cursor pressed
                if (state != OPTIONS) {
                    c.effects[0]->stop();
                    c.effects[0]->play();
                    row.setPosition((c.w.getSize().x / 2.f) - 100.0f * c.screenScale,
                                    c.w.getSize().y / 2.f + 125.0f * c.screenScale);
                    state = OPTIONS;
                }
            } else if (Keyboard::isKeyPressed(Keyboard::Escape))
                return EXIT;

            // Show the menu with the starting and options indicators
            c.w.draw(mainMenu);

            c.w.draw(nameGames[j]);
            c.w.draw(textElements[1]);
            c.w.draw(textElements[2]);
            c.w.draw(textElements[3]);
            c.w.draw(textElements[4]);
            c.w.draw(textElements[5]);
            c.w.draw(textElements[6]);
            c.w.draw(textElements[7]);
            c.w.draw(row);
            bufferSprite.setTexture(c.w.getTexture(), true);
            c.w.display();
            c.window.draw(bufferSprite);
            c.window.display();
            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)) {
                // Pass to the second menu
                startPressed = true;
                c.effects[1]->stop();
                c.effects[1]->play();
            }

            j = (j < (int) nameGames.size() - 1) ? j + 1 : 0;
        }
        // Return the state of the game
        c.effects[0]->stop();

        if (c.enablePixelArt) {
            if (c.isDefaultScreen)
                c.window.setView(View(Vector2f(SCREEN_DEFAULT_X / 4.0f, SCREEN_DEFAULT_Y / 4.0f),
                                      Vector2f(SCREEN_DEFAULT_X / 2.0f, SCREEN_DEFAULT_Y / 2.0f)));
            else
                c.window.setView(View(Vector2f(SCREEN_HD_X / 4.0f, SCREEN_HD_Y / 4.0f),
                                      Vector2f(SCREEN_HD_X / 2.0f, SCREEN_HD_Y / 2.0f)));
            c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
                       static_cast<unsigned int>(c.window.getView().getSize().y));
            c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);
        }

        return state;
    }
    return EXIT;
}

State changeCarControllers(Config &c) {
    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    KeywordMapper kM = KeywordMapper();

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    bufferSprite.setTexture(c.w.getTexture(), true);
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    // Loading the background texture
    Texture segaBackground, textureShape;
    segaBackground.loadFromFile("resources/MenuOptions/segaIcon.png");
    segaBackground.setRepeated(true);
    textureShape.loadFromFile("resources/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x, c.w.getSize().y);
    Sprite sprite(segaBackground, background);

    RectangleShape shape;
    shape.setPosition((c.w.getSize().x / 2.f) - 300.0f * c.screenScale, c.w.getSize().y / 2.f - 250.0f * c.screenScale);
    shape.setSize(sf::Vector2f(610.0f * c.screenScale, 500.0f * c.screenScale));
    shape.setOutlineColor(Color(19, 186, 251));
    shape.setOutlineThickness(5.0f * c.screenScale);
    shape.setTexture(&textureShape, true);

    vector<Button> menuButtons;

    // Main Text of the menu
    Text optionsText;
    optionsText.setString("CONTROLLERS");
    optionsText.setPosition(c.w.getSize().x / 2.f - 160.0f * c.screenScale,
                            c.w.getSize().y / 2.f - 220.0f * c.screenScale);
    optionsText.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    optionsText.setFont(c.options);
    optionsText.setStyle(Text::Bold | Text::Underlined);
    optionsText.setFillColor(Color::Red);

    Text info1;
    info1.setString("Hold down Space to select a controller");
    info1.setFillColor(Color(10, 201, 235));
    info1.setOutlineColor(Color(3, 39, 8));
    info1.setOutlineThickness(3.0f * c.screenScale);
    info1.setCharacterSize(static_cast<unsigned int>(int(15.0f * c.screenScale)));
    info1.setStyle(Text::Bold);
    info1.setPosition(c.w.getSize().x / 2.f - 235.0f * c.screenScale, c.w.getSize().y / 2.f - 160.0f * c.screenScale);
    info1.setFont(c.options);
    c.w.draw(info1);

    Text info2;
    info2.setString("Then press a key to change its configuration");
    info2.setFillColor(Color(10, 201, 235));
    info2.setOutlineColor(Color(3, 39, 8));
    info2.setCharacterSize(static_cast<unsigned int>(int(15.0f * c.screenScale)));
    info2.setOutlineThickness(3.0f * c.screenScale);
    info2.setStyle(Text::Bold);
    info2.setPosition(c.w.getSize().x / 2.f - 265.0f * c.screenScale, c.w.getSize().y / 2.f - 120.0f * c.screenScale);
    info2.setFont(c.options);
    c.w.draw(info2);

    // Option indicators

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Turning left", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale, c.w.getSize().y / 2.f,
                             200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                             "Turning right", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Acceleration", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 140.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Brake", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

    // Option configurations

    int code;
    code = kM.mapperCodeKeyWord[c.leftKey];
    menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1,
                             c.screenScale);

    code = kM.mapperCodeKeyWord[c.rightKey];
    menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale, c.w.getSize().y / 2.f,
                             200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                             kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                             c.screenScale);

    code = kM.mapperCodeKeyWord[c.accelerateKey];
    menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                             c.screenScale);

    code = kM.mapperCodeKeyWord[c.brakeKey];
    menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 140.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             kM.mapperIdKeyWord[code], Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                             c.screenScale);


    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Until the start keyword is not pressed
    while (!startPressed) {
        c.w.draw(sprite);
        c.w.draw(shape);
        c.w.draw(optionsText);
        c.w.draw(info1);
        c.w.draw(info2);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons) {
            menuButton.render(&c.w);
        }

        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(milliseconds(120));
        c.effects[0]->stop();

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }
        if (Keyboard::isKeyPressed(c.menuDownKey)) {
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1) / 2) {
                // Change the color appearance of both buttons
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 4].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 3].setButtonState(BUTTON_IDLE);
            }
        } else if (Keyboard::isKeyPressed(c.menuUpKey)) {
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0) {
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
            c.window.waitEvent(e);
            if (e.type == Event::KeyPressed && e.key.code != -1 && e.key.code != Keyboard::Enter &&
                e.key.code != Keyboard::Space) {
                // Modify the option parameter if it's necessary
                switch (optionSelected) {
                    case 0:
                        if (kM.mapperCodeKeyWord[e.key.code] == c.rightKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey ||
                            kM.mapperCodeKeyWord[e.key.code] == c.brakeKey) {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        } else {
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
                            kM.mapperCodeKeyWord[e.key.code] == c.brakeKey) {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        } else {
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
                            kM.mapperCodeKeyWord[e.key.code] == c.brakeKey) {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        } else {
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
                            kM.mapperCodeKeyWord[e.key.code] == c.accelerateKey) {
                            c.effects[3]->stop();
                            c.effects[3]->play();
                        } else {
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

        // Check if left or right cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)) {
            // Change the controllers of the car
            startPressed = true;
            c.modifiedConfig = true;
            c.effects[2]->stop();
            c.effects[2]->play();
        }
    }
    return OPTIONS;
}

State soundMenu(Config &c, const bool &inGame) {
    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    bufferSprite.setTexture(c.w.getTexture(), true);
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    // Loading the background texture
    Texture segaBackground, textureShape;
    segaBackground.loadFromFile("resources/MenuOptions/segaIcon.png");
    segaBackground.setRepeated(true);
    textureShape.loadFromFile("resources/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

    IntRect background(0, 0, c.w.getSize().x, c.w.getSize().y);
    Sprite sprite(segaBackground, background);

    RectangleShape shape;
    shape.setPosition((c.w.getSize().x / 2.f) - 300.0f * c.screenScale, c.w.getSize().y / 2.f - 250.0f * c.screenScale);
    shape.setSize(sf::Vector2f(610.0f * c.screenScale, 500.0f * c.screenScale));
    shape.setOutlineColor(Color(19, 186, 251));
    shape.setOutlineThickness(5.0f * c.screenScale);
    shape.setTexture(&textureShape, true);

    vector<Button> menuButtons;

    // Main Text of the menu
    Text optionsText;
    optionsText.setString("SOUND MENU");
    optionsText.setPosition(c.w.getSize().x / 2.f - 160.0f * c.screenScale,
                            c.w.getSize().y / 2.f - 220.0f * c.screenScale);
    optionsText.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    optionsText.setFont(c.options);
    optionsText.setStyle(Text::Bold | Text::Underlined);
    optionsText.setFillColor(Color::Red);

    // Option indicators

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Music volume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale, c.w.getSize().y / 2.f,
                             200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                             "Effects volume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                             c.screenScale);

    // Option configurations
    menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             to_string(c.volumeMusic), Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1,
                             c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale, c.w.getSize().y / 2.f,
                             200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                             to_string(c.volumeEffects), Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                             c.screenScale);


    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Until the start keyword is not pressed
    while (!startPressed) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        if (Keyboard::isKeyPressed(c.menuDownKey)) {
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1) / 2) {
                // Change the color appearance of both buttons
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 2].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
            }
        } else if (Keyboard::isKeyPressed(c.menuUpKey)) {
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0) {
                // Change the color appearance of both buttons
                c.effects[0]->play();
                optionSelected--;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
                menuButtons[optionSelected + 2].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 3].setButtonState(BUTTON_IDLE);
            }
        }

        if (optionSelected == 0) {
            // Volume music
            // Check if left or right cursor keys have been pressed or not
            if (Keyboard::isKeyPressed(c.leftKey)) {
                if (c.volumeMusic != 0) {
                    c.volumeMusic--;
                    for (int i = 0; i <= 5; i++) {
                        c.themes[i]->setVolume(float(c.volumeMusic));
                        if (i == 0) {
                            c.themes[i]->pause();
                            c.themes[i]->play();
                        }
                    }
                    menuButtons[optionSelected + 2].setTextButton((to_string(c.volumeMusic)));
                }
            } else if (Keyboard::isKeyPressed(c.rightKey)) {
                if (c.volumeMusic != 100) {
                    c.volumeMusic++;
                    for (int i = 0; i <= 5; i++) {
                        c.themes[i]->setVolume(float(c.volumeMusic));
                        if (i == 0) {
                            c.themes[i]->pause();
                            c.themes[i]->play();
                        }
                    }
                    menuButtons[optionSelected + 2].setTextButton((to_string(c.volumeMusic)));
                }
            }
        } else {
            // Volume effects
            // Check if left or right cursor keys have been pressed or not
            if (Keyboard::isKeyPressed(c.leftKey)) {
                if (c.volumeEffects != 0) {
                    c.volumeEffects--;
                    for (int i = 0; i <= 29; i++) {
                        c.effects[i]->setVolume(float(c.volumeEffects));
                    }
                    c.effects[0]->stop();
                    c.effects[0]->play();
                    menuButtons[optionSelected + 2].setTextButton((to_string(c.volumeEffects)));
                }
            } else if (Keyboard::isKeyPressed(c.rightKey)) {
                if (c.volumeEffects != 100) {
                    c.volumeEffects++;
                    for (int i = 0; i <= 29; i++) {
                        c.effects[i]->setVolume(float(c.volumeEffects));
                    }
                    c.effects[0]->stop();
                    c.effects[0]->play();
                    menuButtons[optionSelected + 2].setTextButton((to_string(c.volumeEffects)));
                }
            }
        }
        c.w.draw(sprite);
        c.w.draw(shape);
        c.w.draw(optionsText);

        // Show the buttons of the menu
        for (auto &menuButton : menuButtons) {
            menuButton.render(&c.w);
        }

        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(milliseconds(120));
        c.effects[0]->stop();

        // Check if left or right cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)) {
            // Change the controllers of the car
            startPressed = true;
            c.modifiedConfig = true;
            c.effects[2]->stop();
            c.effects[2]->play();
        }
    }
    return OPTIONS;
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
        segaBackground.loadFromFile("resources/MenuOptions/segaIcon.png");
        segaBackground.setRepeated(true);
        textureShape.loadFromFile("resources/MenuOptions/outrun.png");
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
            if (Keyboard::isKeyPressed(menuDownKey)) {
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
            } else if (Keyboard::isKeyPressed(menuUpKey)) {
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
                if (Keyboard::isKeyPressed(leftKey)) {
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
                        } else {
                            window.create(VideoMode::getFullscreenModes()[0], "Out Run", Style::Fullscreen);
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
                } else if (Keyboard::isKeyPressed(rightKey)) {
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
                    }
                }
            } else {
                // Volume effects
                // Check if left or right cursor keys have been pressed or not
                if (Keyboard::isKeyPressed(leftKey)) {
                    if (enablePixelArt) {
                        enablePixelArt = false;
                        menuButtons[optionSelected + 2].setTextButton("DISABLED");
                    }
                } else if (Keyboard::isKeyPressed(rightKey)) {
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
            if (Keyboard::isKeyPressed(menuEnterKey)) {
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

State optionsMenu(Config &c, const bool &inGame) {
    c.window.setView(View(Vector2f(c.window.getSize().x / 2.0f, c.window.getSize().y / 2.0f),
                          Vector2f(c.window.getSize().x, c.window.getSize().y)));
    c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
               static_cast<unsigned int>(c.window.getView().getSize().y));
    c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);

    // Control if the start key is pressed or not
    bool startPressed = false;

    while (!startPressed) {

        // Clean the console window
        c.w.clear(Color(0, 0, 0));
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        c.themes[0]->play();

        // Loading the background texture
        Texture segaBackground, textureShape;
        segaBackground.loadFromFile("resources/MenuOptions/segaIcon.png");
        segaBackground.setRepeated(true);
        textureShape.loadFromFile("resources/MenuOptions/outrun.png");
        textureShape.setRepeated(true);

        IntRect background(0, 0, c.w.getSize().x, c.w.getSize().y);
        Sprite sprite(segaBackground, background);

        RectangleShape shape;
        shape.setPosition((c.w.getSize().x / 2.f) - 350.0f * c.screenScale,
                          c.w.getSize().y / 2.f - 250.0f * c.screenScale);
        shape.setSize(sf::Vector2f(710.0f * c.screenScale, 500.0f * c.screenScale));
        shape.setOutlineColor(Color(19, 186, 251));
        shape.setOutlineThickness(5.0f * c.screenScale);
        shape.setTexture(&textureShape, true);

        vector<Button> menuButtons;

        // Main Text of the menu
        Text optionsText;
        optionsText.setString("OPTIONS");
        optionsText.setPosition((c.w.getSize().x / 2.f) - 90.0f * c.screenScale,
                                c.w.getSize().y / 2.f - 230.0f * c.screenScale);
        optionsText.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
        optionsText.setFont(c.options);
        optionsText.setStyle(Text::Bold | Text::Underlined);
        optionsText.setFillColor(Color::Red);

        // Option indicators

        menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                 c.w.getSize().y / 2.f - 130.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 "Difficulty", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1,
                                 c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                 c.w.getSize().y / 2.f - 60.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 "enemies AI", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                                 c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                 c.w.getSize().y / 2.f + 10.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 "Sound", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                 c.w.getSize().y / 2.f + 80.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 "Graphics", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                 c.w.getSize().y / 2.f + 150.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 "Controllers", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                                 c.screenScale);

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

        const string submenu = "Menu", access = "Press C", saved = "Saved!";

        menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                 c.w.getSize().y / 2.f - 130.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 difficulty, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1, c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                 c.w.getSize().y / 2.f - 60.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 c.enableAI ? "ENABLED" : "DISABLED", Color(0, 255, 0), Color(255, 255, 0),
                                 Color(0, 255, 0),
                                 0, c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                 c.w.getSize().y / 2.f + 10.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 submenu, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                 c.w.getSize().y / 2.f + 80.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 submenu, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

        menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                 c.w.getSize().y / 2.f + 150.0f * c.screenScale, 200.0f * c.screenScale,
                                 30.0f * c.screenScale, c.options,
                                 submenu, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

        // Control the option selected by the user
        int optionSelected = 0;

        // Until the start keyword is not pressed
        while (!startPressed) {
            // Detect the possible events
            Event e{};
            while (c.window.pollEvent(e))
                if (e.type == Event::Closed)
                    return EXIT;

            // Check if the up or down cursor keys have been pressed or not
            if (Keyboard::isKeyPressed(c.menuDownKey)) {
                // Up cursor pressed and change the soundtrack selected in the list
                if (optionSelected != int(menuButtons.size() - 1) / 2) {
                    // Change the color appearance of both buttons
                    c.effects[0]->play();
                    optionSelected++;
                    menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                    menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
                    menuButtons[optionSelected + 5].setButtonState(BUTTON_HOVER);
                    menuButtons[optionSelected + 4].setButtonState(BUTTON_IDLE);
                }
            } else if (Keyboard::isKeyPressed(c.menuUpKey)) {
                // Down cursor pressed and change the soundtrack selected in the list
                if (optionSelected != 0) {
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
            switch (optionSelected) {
                case 0:
                    // Check if left or right cursor keys have been pressed or not
                    if (Keyboard::isKeyPressed(c.leftKey)) {
                        if (c.level != PEACEFUL) {
                            // Change the difficult level
                            if (c.level == EASY) {
                                c.level = PEACEFUL;
                                menuButtons[optionSelected + 5].setTextButton("Peaceful");
                            }
                            if (c.level == NORMAL) {
                                c.level = EASY;
                                menuButtons[optionSelected + 5].setTextButton("Easy");
                            } else if (c.level == HARD) {
                                c.level = NORMAL;
                                menuButtons[optionSelected + 5].setTextButton("Normal");
                            }
                        }
                    } else if (Keyboard::isKeyPressed(c.rightKey)) {
                        if (c.level != HARD) {
                            // Change the difficult level
                            if (c.level == PEACEFUL) {
                                c.level = EASY;
                                menuButtons[optionSelected + 5].setTextButton("Easy");
                            }
                            if (c.level == EASY) {
                                c.level = NORMAL;
                                menuButtons[optionSelected + 5].setTextButton("Normal");
                            } else if (c.level == NORMAL) {
                                c.level = HARD;
                                menuButtons[optionSelected + 5].setTextButton("Hard");
                            }
                        }
                    }
                    break;
                case 1:
                    // AI aggressiveness level
                    // Check if left or right cursor keys have been pressed or not
                    if (Keyboard::isKeyPressed(c.leftKey)) {
                        if (c.enableAI) {
                            c.enableAI = false;
                            menuButtons[optionSelected + 5].setTextButton("DISABLED");
                        }
                    } else if (Keyboard::isKeyPressed(c.rightKey)) {
                        if (!c.enableAI) {
                            c.enableAI = true;
                            menuButtons[optionSelected + 5].setTextButton("ENABLED");
                        }
                    }

                    menuButtons[optionSelected + 6].setTextButton(submenu);
                    break;
                case 2:
                    // Change the volume of the game
                    menuButtons[optionSelected + 5].setTextButton(access);

                    // Check if left or right cursor keys have been pressed or not
                    if (Keyboard::isKeyPressed(Keyboard::C)) {
                        c.effects[1]->stop();
                        c.effects[1]->play();
                        State status = soundMenu(c, inGame);
                        if (status == EXIT) {
                            return status;
                        }
                        if (c.modifiedConfig) {
                            menuButtons[optionSelected + 5].setTextButton(saved);
                        }
                    }

                    menuButtons[optionSelected + 6].setTextButton(submenu);
                    break;
                case 3:
                    // Change the graphics of the game
                    menuButtons[optionSelected + 5].setTextButton(access);

                    // Check if left or right cursor keys have been pressed or not
                    if (Keyboard::isKeyPressed(Keyboard::C)) {
                        c.effects[1]->stop();
                        c.effects[1]->play();
                        State status = c.graphicsMenu();
                        if (status == EXIT) {
                            return status;
                        }
                        if (c.modifiedConfig) {
                            menuButtons[optionSelected + 5].setTextButton(saved);
                            shape.setPosition((c.w.getSize().x / 2.f) - 350.0f * c.screenScale,
                                              c.w.getSize().y / 2.f - 250.0f * c.screenScale);
                            shape.setSize(sf::Vector2f(710.0f * c.screenScale, 500.0f * c.screenScale));
                            optionsText.setPosition((c.w.getSize().x / 2.f) - 90.0f * c.screenScale,
                                                    c.w.getSize().y / 2.f - 230.0f * c.screenScale);
                            optionsText.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));

                            menuButtons.clear();

                            menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f - 130.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     "Difficulty", Color(0, 255, 0), Color(255, 255, 0),
                                                     Color(0, 255, 0), 0, c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f - 60.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     "enemies AI", Color(0, 255, 0), Color(255, 255, 0),
                                                     Color(0, 255, 0), 0, c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f + 10.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     "Sound", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                                                     c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f + 80.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     "Graphics", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0),
                                                     1, c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f - 270.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f + 150.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     "Controllers", Color(0, 255, 0), Color(255, 255, 0),
                                                     Color(0, 255, 0), 0, c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f - 130.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     difficulty, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0),
                                                     0, c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f - 60.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     c.enableAI ? "ENABLED" : "DISABLED", Color(0, 255, 0),
                                                     Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f + 10.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     submenu, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                                                     c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f + 80.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     submenu, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1,
                                                     c.screenScale);

                            menuButtons.emplace_back(c.w.getSize().x / 2.f + 80.0f * c.screenScale,
                                                     c.w.getSize().y / 2.f + 150.0f * c.screenScale,
                                                     200.0f * c.screenScale,
                                                     30.0f * c.screenScale, c.options,
                                                     submenu, Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0,
                                                     c.screenScale);
                        }
                    }

                    menuButtons[optionSelected + 4].setTextButton(submenu);
                    menuButtons[optionSelected + 6].setTextButton(submenu);
                    break;
                case 4:
                    // Change the controllers of the game
                    menuButtons[optionSelected + 5].setTextButton(access);

                    // Check if left or right cursor keys have been pressed or not
                    if (Keyboard::isKeyPressed(Keyboard::C)) {
                        // Change the controllers of the car
                        c.effects[1]->stop();
                        c.effects[1]->play();
                        State status = changeCarControllers(c);
                        if (status == EXIT) {
                            return status;
                        }
                        if (c.modifiedConfig) {
                            menuButtons[optionSelected + 5].setTextButton(saved);
                        }
                    }

                    menuButtons[optionSelected + 4].setTextButton(submenu);
                    break;
                default:
                    break;
            }

            c.w.draw(sprite);
            c.w.draw(shape);
            c.w.draw(optionsText);

            // Show the buttons of the menu
            for (auto &menuButton : menuButtons) {
                menuButton.render(&c.w);
            }

            bufferSprite.setTexture(c.w.getTexture(), true);
            c.w.display();
            c.window.draw(bufferSprite);
            c.window.display();
            sleep(milliseconds(120));
            c.effects[0]->stop();

            // Check if left or right cursor keys have been pressed or not
            if (Keyboard::isKeyPressed(c.menuEnterKey)) {
                // Change the controllers of the car
                startPressed = true;
                c.effects[2]->stop();
                c.effects[2]->play();
            }
        }
        c.themes[0]->stop();

        if (c.enablePixelArt) {
            if (c.isDefaultScreen)
                c.window.setView(View(Vector2f(SCREEN_DEFAULT_X / 4.0f, SCREEN_DEFAULT_Y / 4.0f),
                                      Vector2f(SCREEN_DEFAULT_X / 2.0f, SCREEN_DEFAULT_Y / 2.0f)));
            else
                c.window.setView(View(Vector2f(SCREEN_HD_X / 4.0f, SCREEN_HD_Y / 4.0f),
                                      Vector2f(SCREEN_HD_X / 2.0f, SCREEN_HD_Y / 2.0f)));
            c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
                       static_cast<unsigned int>(c.window.getView().getSize().y));
            c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);
        }
    }

    if (inGame) {
        return GAME;
    } else {
        return startMenu(c, true);
    }
}

State selectMusicSoundtrack(Config &c) {
    // Clean the console window
    c.w.clear();
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

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
    radioMenu.setPosition(0, 0);
    radioMenu.setScale((float) c.w.getSize().x / backgroundMusic.getSize().x,
                       (float) c.w.getSize().y / backgroundMusic.getSize().y);

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++) {
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/soundtrack" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++) {
        // Loading the icon texture
        t.loadFromFile("resources/MusicMenu/radio" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++) {
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
    while (!startPressed) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e))
            if (e.type == Event::Closed)
                return EXIT;

        // Control if the left or right cursor keys are pressed or not
        if (Keyboard::isKeyPressed(c.leftKey)) {
            // Up cursor pressed and change the soundtrack selected in the list
            if (c.currentSoundtrack != 1) {
                c.effects[1]->stop();
                c.effects[1]->play();
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack--;
                c.themes[c.currentSoundtrack]->play();
            }
        } else if (Keyboard::isKeyPressed(c.rightKey)) {
            // Down cursor pressed and change the soundtrack selected in the list
            if (c.currentSoundtrack != NUM_SOUNDTRACKS - 1) {
                c.effects[1]->stop();
                c.effects[1]->play();
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack++;
                c.themes[c.currentSoundtrack]->play();
            }
        }

        // Load the texture of the soundtrack to display in the radio panel
        music.setTexture(textures[c.currentSoundtrack - 1], true);
        music.setScale(c.screenScale, c.screenScale);

        // Get the dial movement to reproduce
        radio.setTexture(textures[c.currentSoundtrack + 2], true);
        radio.setScale(c.screenScale, float(c.w.getSize().y) / SCREEN_DEFAULT_Y);

        // Get the hand movement of the driver
        hand.setTexture(textures[c.currentSoundtrack + 5], true);
        hand.setScale(c.screenScale, float(c.w.getSize().y) / SCREEN_DEFAULT_Y);

        // Control the coordinates X and Y where display the title
        music.setPosition((c.w.getSize().x - music.getGlobalBounds().width) / 2.f,
                          (c.w.getSize().y - music.getGlobalBounds().height) / 2.f);
        radio.setPosition((c.w.getSize().x - radio.getGlobalBounds().width * 0.5f) / 2.0f,
                          c.w.getSize().y * 0.8725f - radio.getGlobalBounds().height);
        hand.setPosition((c.w.getSize().x - hand.getGlobalBounds().width * 0.66f) / 2.0f,
                         c.w.getSize().y - hand.getGlobalBounds().height);

        // Show the title of the soundtrack
        c.w.draw(radioMenu);
        c.w.draw(music);
        c.w.draw(radio);
        c.w.draw(hand);
        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        sleep(milliseconds(80));

        // Check if the keyword Enter has been pressed or not
        if (Keyboard::isKeyPressed(c.menuEnterKey)) {
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


State
rankingMenu(Config &c, const unsigned long scorePlayerGame, const int minutes, const int secs, const int cents_Second) {
    c.window.setView(View(Vector2f(c.window.getSize().x / 2.0f, c.window.getSize().y / 2.0f),
                          Vector2f(c.window.getSize().x, c.window.getSize().y)));
    c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
               static_cast<unsigned int>(c.window.getView().getSize().y));
    c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);

    c.effects[6]->stop();
    // Clean the console window
    c.w.clear(Color(0, 0, 0));
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

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
    rankingTitle.setCharacterSize(static_cast<unsigned int>(int(65.0f * c.screenScale)));
    rankingTitle.setFillColor(Color::Yellow);
    rankingTitle.setOutlineColor(Color(12, 12, 12));
    rankingTitle.setOutlineThickness(3.0f * c.screenScale);

    Text scoreIndicator;
    scoreIndicator.setFont(c.timeToPlay);
    scoreIndicator.setPosition(c.w.getSize().x / 8.f, c.w.getSize().y / 6.0f);
    scoreIndicator.setString("SCORE");
    scoreIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    scoreIndicator.setFillColor(Color(146, 194, 186));
    scoreIndicator.setOutlineColor(Color(12, 12, 12));
    scoreIndicator.setOutlineThickness(3.0f * c.screenScale);

    Text playerIndicator;
    playerIndicator.setFont(c.timeToPlay);
    playerIndicator.setPosition(c.w.getSize().x / 2.2f, c.w.getSize().y / 6.0f);
    playerIndicator.setString("NAME");
    playerIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    playerIndicator.setFillColor(Color(146, 194, 186));
    playerIndicator.setOutlineColor(Color(12, 12, 12));
    playerIndicator.setOutlineThickness(3.0f * c.screenScale);

    Text recordIndicator;
    recordIndicator.setFont(c.timeToPlay);
    recordIndicator.setPosition((c.w.getSize().x / 2.f) * 1.5f, c.w.getSize().y / 6.0f);
    recordIndicator.setString("RECORD");
    recordIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    recordIndicator.setFillColor(Color(146, 194, 186));
    recordIndicator.setOutlineColor(Color(12, 12, 12));
    recordIndicator.setOutlineThickness(3.0f * c.screenScale);

    Text timeCounter;
    timeCounter.setFont(c.timeToPlay);
    timeCounter.setCharacterSize(static_cast<unsigned int>(int(62.0f * c.screenScale)));
    timeCounter.setString(to_string(time));
    timeCounter.setPosition((c.w.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width,
                            c.w.getSize().y / 15.7f);
    timeCounter.setFillColor(Color::Red);
    timeCounter.setOutlineColor(Color(12, 12, 12));
    timeCounter.setOutlineThickness(3.0f * c.screenScale);

    Text scorePlayer;
    scorePlayer.setFont(c.timeToPlay);
    scorePlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    scorePlayer.setFillColor(Color(146, 194, 186));
    scorePlayer.setOutlineColor(Color::Black);
    scorePlayer.setOutlineThickness(3.0f * c.screenScale);

    Text namePlayer;
    namePlayer.setFont(c.timeToPlay);
    namePlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    namePlayer.setFillColor(Color(146, 194, 186));
    namePlayer.setOutlineColor(Color(12, 12, 12));
    namePlayer.setOutlineThickness(3.0f * c.screenScale);

    Text minutesPlayer;
    minutesPlayer.setFont(c.timeToPlay);
    minutesPlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    minutesPlayer.setFillColor(Color(146, 194, 186));
    minutesPlayer.setOutlineColor(Color::Black);
    minutesPlayer.setOutlineThickness(3.0f * c.screenScale);

    Text secondsPlayer;
    secondsPlayer.setFont(c.timeToPlay);
    secondsPlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    secondsPlayer.setFillColor(Color(146, 194, 186));
    secondsPlayer.setOutlineColor(Color::Black);
    secondsPlayer.setOutlineThickness(3.0f * c.screenScale);

    Text centsPlayer;
    centsPlayer.setFont(c.timeToPlay);
    centsPlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    centsPlayer.setFillColor(Color(146, 194, 186));
    centsPlayer.setOutlineColor(Color::Black);
    centsPlayer.setOutlineThickness(3.0f * c.screenScale);

    Text index;
    index.setFont(c.timeToPlay);
    index.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    index.setFillColor(Color(180, 130, 211));
    index.setOutlineColor(Color::Black);
    index.setOutlineThickness(3.0f * c.screenScale);

    Text start;
    start.setCharacterSize(static_cast<unsigned int>(int(45.0f * c.screenScale)));
    start.setFont(c.timeToPlay);
    start.setFillColor(Color::Green);
    start.setOutlineColor(Color::Black);
    start.setOutlineThickness(3.0f * c.screenScale);

    // Loading the background texture
    Texture rankingBackground;
    rankingBackground.loadFromFile("resources/RankingMenu/bg.png");
    Sprite palm_trees(rankingBackground);
    palm_trees.setScale((float) c.w.getSize().x / rankingBackground.getSize().x,
                        (float) c.w.getSize().y / rankingBackground.getSize().y);

    // Get the best seventh out runners
    vector<Score> scoreRankingPlayer = getGlobalScores();

    // Check if there is a new record
    int record = isNewRecord(scoreRankingPlayer, scorePlayerGame);
    bool startPressed = false;
    bool blink = false;
    int lettersIntroduced = 0;
    string name = "_";

    rankingTime.restart();
    elapsed1 = rankingTime.getElapsedTime().asSeconds();
    elapsed3 = blinkStart.getElapsedTime().asSeconds();

    c.effects[29]->play();
    Event e{};

    while (time > 0 && !startPressed) {

        // Get the actual time
        elapsed2 = rankingTime.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed2 - elapsed1 >= ranking_delay.asSeconds()) {
            // Draw time
            time--;
            rankingTime.restart();
            timeCounter.setString(to_string(time));
            timeCounter.setPosition((c.w.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width,
                                    c.w.getSize().y / 15.7f);
        }

        c.w.draw(palm_trees);
        c.w.draw(rankingTitle);
        c.w.draw(timeCounter);
        c.w.draw(scoreIndicator);
        c.w.draw(playerIndicator);
        c.w.draw(recordIndicator);

        // There is no new record
        if (record == -1) {

            start.setString("PRESS START!");
            start.setPosition(c.w.getSize().x / 2.5f, (c.w.getSize().y / 4.5f) + 400.0f);

            // There is not a new record
            for (int i = 1; i <= 7; i++) {

                index.setString(to_string(i) + ".");
                index.setPosition((c.w.getSize().x / 13.f) - index.getLocalBounds().width,
                                  (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * float(i));

                scorePlayer.setString(to_string(scoreRankingPlayer[i - 1].score));
                scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * float(i));

                namePlayer.setString(scoreRankingPlayer[i - 1].name);
                namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * float(i));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i - 1].minutes) + "'");
                minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * float(i));

                if (scoreRankingPlayer[i - 1].secs >= 10) {
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i - 1].secs) + "''");
                } else {
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i - 1].secs) + "''");
                }
                secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * float(i));


                if (scoreRankingPlayer[i - 1].cents_second >= 10) {
                    centsPlayer.setString(to_string(scoreRankingPlayer[i - 1].cents_second));
                } else {
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i - 1].cents_second));
                }
                centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * float(i));

                c.w.draw(index);
                c.w.draw(scorePlayer);
                c.w.draw(namePlayer);
                c.w.draw(minutesPlayer);
                c.w.draw(secondsPlayer);
                c.w.draw(centsPlayer);
            }
            while (c.window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    return EXIT;
                }
            }
        } else {

            if (lettersIntroduced != 3) {
                start.setString("ENTER YOUR NAME!");
                start.setPosition(c.w.getSize().x / 3.0f, (c.w.getSize().y / 4.5f) + 400.0f);
            } else {
                start.setString("PRESS START!");
                start.setPosition(c.w.getSize().x / 2.5f, (c.w.getSize().y / 4.5f) + 400.0f);
            }

            // There is a new record
            // Show all the out runners with a higher score

            for (int i = 0; i <= record - 1; i++) {

                index.setString(to_string(i) + ".");
                index.setPosition((c.w.getSize().x / 13.f) - index.getLocalBounds().width,
                                  (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) i);

                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 1));

                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 1));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 1));

                if (scoreRankingPlayer[i].secs >= 10) {
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                } else {
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");
                }
                secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 1));


                if (scoreRankingPlayer[i].cents_second >= 10) {
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                } else {
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));
                }
                centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 1));

                c.w.draw(scorePlayer);
                c.w.draw(namePlayer);
                c.w.draw(minutesPlayer);
                c.w.draw(secondsPlayer);
                c.w.draw(centsPlayer);
            }

            int offset = (record == 0) ? 1 : record + 1;

            // Show the actual player
            scorePlayer.setString(to_string(scorePlayerGame));
            scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                    (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) offset);

            namePlayer.setString(name);
            namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                   (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) offset);

            minutesPlayer.setString(to_string(minutes) + "'");
            minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                      (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) offset);

            if (secs >= 10) {
                secondsPlayer.setString(to_string(secs) + "''");
            } else {
                secondsPlayer.setString("0" + to_string(secs) + "''");
            }
            secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                      (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) offset);


            if (cents_Second >= 10) {
                centsPlayer.setString(to_string(cents_Second));
            } else {
                centsPlayer.setString("0" + to_string(cents_Second));
            }
            centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                    (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) offset);

            c.w.draw(scorePlayer);
            c.w.draw(namePlayer);
            c.w.draw(minutesPlayer);
            c.w.draw(secondsPlayer);
            c.w.draw(centsPlayer);

            // Show the rest of out runners

            for (int i = record; i < 6 && i < scoreRankingPlayer.size(); i++) {
                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((c.w.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 2));

                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((c.w.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 2));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((c.w.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 2));

                if (scoreRankingPlayer[i].secs >= 10) {
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                } else {
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");
                }
                secondsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 2));


                if (scoreRankingPlayer[i].cents_second >= 10) {
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                } else {
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));
                }
                centsPlayer.setPosition((c.w.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (c.w.getSize().y / 4.5f) + 50.0f * c.screenScale * (float) (i + 2));

                c.w.draw(scorePlayer);
                c.w.draw(namePlayer);
                c.w.draw(minutesPlayer);
                c.w.draw(secondsPlayer);
                c.w.draw(centsPlayer);
            }
        }
        elapsed4 = blinkStart.getElapsedTime().asSeconds();
        if (elapsed4 - elapsed3 >= ranking_delay.asSeconds()) {
            blink = !blink;
            blinkStart.restart();
        }
        if (blink) {
            start.setFillColor(Color::Green);
            start.setOutlineColor(Color::Black);
        } else {
            start.setFillColor(Color::Transparent);
            start.setOutlineColor(Color::Transparent);
        }

        c.w.draw(start);
        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        if (lettersIntroduced != 3 && record != -1) {
            // while there are pending events...
            Event event{};
            while (c.window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    return EXIT;
                } else if (event.type == Event::KeyPressed) {
                    // Get code of the key
                    int code = event.key.code;
                    // Check if the key pressed is a letter or not
                    if (code >= 0 && code <= 25) {
                        lettersIntroduced++;
                        string keyLetter = kM.mapperIdKeyWord[code];
                        if (name == "_") {
                            name = keyLetter;
                            c.effects[1]->stop();
                            c.effects[1]->play();
                        } else {
                            if (lettersIntroduced == 3) {
                                name = name.substr(0, name.size() - 1);
                                name += keyLetter;
                                c.effects[2]->stop();
                                c.effects[2]->play();
                            } else {
                                name += keyLetter + "_";
                                c.effects[1]->stop();
                                c.effects[1]->play();
                            }
                        }
                    } else {
                        c.effects[3]->stop();
                        c.effects[3]->play();
                    }
                }
            }
        }
        if (Keyboard::isKeyPressed(c.menuEnterKey)) {
            startPressed = true;
        }
    }

    // Store the record
    if (record != -1) {
        // If the was record and the name is uncompleted
        if (lettersIntroduced != 3) {
            name = "   ";
        }
        Score s = Score(scorePlayerGame, name, minutes, secs, cents_Second);
        saveNewRecord(scoreRankingPlayer, s);
    }

    c.effects[2]->stop();
    c.effects[2]->play();
    c.effects[29]->stop();
    c.themes[4]->stop();

    if (c.enablePixelArt) {
        if (c.isDefaultScreen)
            c.window.setView(View(Vector2f(SCREEN_DEFAULT_X / 4.0f, SCREEN_DEFAULT_Y / 4.0f),
                                  Vector2f(SCREEN_DEFAULT_X / 2.0f, SCREEN_DEFAULT_Y / 2.0f)));
        else
            c.window.setView(View(Vector2f(SCREEN_HD_X / 4.0f, SCREEN_HD_Y / 4.0f),
                                  Vector2f(SCREEN_HD_X / 2.0f, SCREEN_HD_Y / 2.0f)));
        c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
                   static_cast<unsigned int>(c.window.getView().getSize().y));
        c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);
    }

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

sf::Font initializeFontOptions() {
    Font f;
    if (!f.loadFromFile("resources/fonts/needForSpeed.ttf")) exit(1);
    return f;
}
