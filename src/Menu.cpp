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

#include "../include/Menu.hpp"
#include "../include/Svg.hpp"

#include <memory>

using namespace std;
using namespace sf;


State introAnimation(Config &c) {
    // Load the game effects
    for (int i = 1; i <= 41; i++) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }
        unique_ptr<Music> effect = make_unique<Music>();
        effect->openFromFile("Resources/SoundEffects/" + to_string(i) + ".ogg");
        effect->setVolume(c.volumeEffects);
        c.effects.push_back(move(effect));
    }
    c.effects[29]->setLoop(true);
    c.effects[37]->setLoop(true);


    /* Create image */
    sfc::SVGImage img;

    /* Don't show debug lines */
    img.setMode(sfc::DrawMode::NORMAL);

    /* Load SVG image from file */
    img.loadFromFile("Resources/Intro/Icon.svg");

    /* Rasterize image & save it to file */
    img.rasterize().saveToFile("Resources/Intro/Icon.png");

     // Vector of images with the logo of Sega
    Texture t;
    Sprite icon;

    // Loading the icon texture
    t.loadFromFile("Resources/Intro/Icon.png");

    // Load the texture in the sprite reseting the last texture
    icon.setTexture(t, true);
    if (!c.isDefaultScreen){
        float axis_x = float(c.w.getSize().x) / SCREEN_DEFAULT_X;
        float axis_y = float(c.w.getSize().y) / SCREEN_DEFAULT_Y;
        icon.setScale(axis_x, axis_y * 1.33f);
    }

    // Locate the icon in the center of the screen
    icon.setPosition((c.w.getSize().x - icon.getGlobalBounds().width) / 7.4f,
                     (c.w.getSize().y - icon.getGlobalBounds().height) / 2.0f);

    Font f;
    f.loadFromFile("Resources/Fonts/Bord.otf");

    // Options of the main menu
    Text logoText1;
    logoText1.setFillColor(Color::White);
    logoText1.setOutlineColor(Color::Black);
    logoText1.setOutlineThickness(3.0f * c.screenScale);

    // Options of the main menu
    Text logoText2;
    logoText2.setFillColor(Color::White);
    logoText2.setOutlineColor(Color::Black);
    logoText2.setOutlineThickness(3.0f * c.screenScale);

    Text zgzText;
    zgzText.setString("zgz");
    zgzText.setCharacterSize(static_cast<unsigned int>(int(65.0f * c.screenScale)));
    zgzText.setFont(f);
    zgzText.setFillColor(Color::White);
    zgzText.setOutlineColor(Color::Black);
    zgzText.setOutlineThickness(3.0f * c.screenScale);
    zgzText.setPosition((c.w.getSize().x - zgzText.getGlobalBounds().width) / 2.4f,
                         c.w.getSize().y / 2.f - zgzText.getGlobalBounds().height * 1.375f);

    // Load the music soundtracks of the game
    for (int i = 0; i <= 5; i++) {
        unique_ptr<Music> music = make_unique<Music>();
        music->openFromFile("Resources/Soundtrack/" + to_string(i) + ".ogg");
        music->setVolume(c.volumeMusic);
        music->setLoop(true);
        c.themes.push_back(move(music));
    }

    c.effects[39]->stop();
    c.effects[39]->play();

    for (int i = 0; i <= 300; i++){

        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Clear the screen and draw the icon
        if (i >= 160){
            logoText1.setString("zgz");
        }
        else if (i>= 155){
            logoText1.setString("zg");
        }
        else if (i >= 150){
            logoText1.setString("z");
        }

        logoText1.setCharacterSize(static_cast<unsigned int>(int(110.0f * c.screenScale)));
        logoText1.setFont(f);
        logoText1.setPosition((c.w.getSize().x - zgzText.getGlobalBounds().width) / 2.4f,
                               c.w.getSize().y / 2.f - logoText1.getGlobalBounds().height * 1.375f);

        if (i >= 192){
            logoText2.setString("infinitgames");
        }
        else if (i >= 190){
            logoText2.setString("infinitgame");
        }
        else if (i >= 188){
            logoText2.setString("infinitgam");
        }
        else if (i >= 186){
            logoText2.setString("infinitga");
        }
        else if (i >= 184){
            logoText2.setString("infinitg");
        }
        else if (i >= 182){
            logoText2.setString("infinit");
        }
        else if (i >= 180){
            logoText2.setString("infini");
        }
        else if (i >= 178){
            logoText2.setString("infin");
        }
        else if (i >= 176){
            logoText2.setString("infi");
        }
        else if (i>= 174){
            logoText2.setString("inf");
        }
        else if (i>= 172){
            logoText2.setString("in");
        }
        else if (i>= 170){
            logoText2.setString("i");
        }

        logoText2.setCharacterSize(static_cast<unsigned int>(int(65.0f * c.screenScale)));
        logoText2.setFont(f);
        logoText2.setPosition((c.w.getSize().x - zgzText.getGlobalBounds().width) / 2.4f,
                               c.w.getSize().y / 2.f + logoText2.getGlobalBounds().height * 0.21f);

        c.w.clear();

        if (i >= 40){
            c.w.draw(icon);
        }
        if (i >= 60){
            c.w.draw(logoText1);
        }
        if (i >= 170){
            c.w.draw(logoText2);
        }

        // Show the logos in the console
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
    }

    RectangleShape blackShape;
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(c.w.getSize().x, c.w.getSize().y));

    // Draw the landscape animation
    for (int j = 0; j <= 255; j += 5){

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        c.w.clear();
        c.w.draw(icon);
        c.w.draw(logoText1);
        c.w.draw(logoText2);

        blackShape.setFillColor(Color(0, 0, 0, j));
        c.w.draw(blackShape);

        // Show the logos in the console
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
    }

    return START;
}

State startMenu(Config &c) {
    c.window.setView(View(Vector2f(c.window.getSize().x / 2.0f, c.window.getSize().y / 2.0f),
                          Vector2f(c.window.getSize().x, c.window.getSize().y)));
    c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
               static_cast<unsigned int>(c.window.getView().getSize().y));
    c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);

    const int ELEMENTS = 8;

    bool startPressed = false;

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
    backgroundMenu.loadFromFile("Resources/Menus/MainMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) c.w.getSize().x / backgroundMenu.getSize().x,
                      (float) c.w.getSize().y / backgroundMenu.getSize().y);

    for (int i = 2; i <= 7; i++) {
        // Loading the texture of the game's name
        gameIcon.loadFromFile("Resources/Menus/MainMenu/LogoMain" + to_string(i) + ".png");
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
    rowSelector.loadFromFile("Resources/Menus/MainMenu/row.png");
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

    textElements[4].setString("ZGZ INFINITGAMES");
    textElements[4].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[4].setFont(c.timeToPlay);
    textElements[4].setFillColor(Color::Green);
    textElements[4].setOutlineColor(Color::Black);
    textElements[4].setOutlineThickness(3.0f * c.screenScale);
    float initialX = c.w.getSize().x - 1.1f * textElements[4].getGlobalBounds().width, initialY =
            c.w.getSize().y - 2.0f * float(textElements[4].getCharacterSize());
    textElements[4].setPosition(initialX / 1.065f, initialY);

    initialX = textElements[4].getGlobalBounds().width;
    textElements[5].setString("EXIT / PAUSE: ESC KEY");
    textElements[5].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[5].setFont(c.timeToPlay);
    textElements[5].setFillColor(Color::Green);
    textElements[5].setOutlineColor(Color::Black);
    textElements[5].setOutlineThickness(3.0f * c.screenScale);
    textElements[5].setPosition(initialX / 3.4f, initialY);

    initialY -= 2.0f * textElements[5].getGlobalBounds().height;
    textElements[6].setString("MOVE: ARROW KEYS");
    textElements[6].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[6].setFont(c.timeToPlay);
    textElements[6].setFillColor(Color::Green);
    textElements[6].setOutlineColor(Color::Black);
    textElements[6].setOutlineThickness(3.0f * c.screenScale);
    textElements[6].setPosition(initialX / 3.4f, initialY);

    textElements[3].setString("2020");
    textElements[3].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[3].setFont(c.timeToPlay);
    textElements[3].setFillColor(Color::Green);
    textElements[3].setOutlineColor(Color::Black);
    textElements[3].setOutlineThickness(3.0f * c.screenScale);
    textElements[3].setPosition(initialX + 8.2f * textElements[3].getGlobalBounds().width, initialY);

    initialY -= 2.0f * textElements[6].getGlobalBounds().height;
    textElements[7].setString("SELECT: ENTER KEY");
    textElements[7].setCharacterSize(static_cast<unsigned int>(int(30.0f * c.screenScale)));
    textElements[7].setFont(c.timeToPlay);
    textElements[7].setFillColor(Color::Green);
    textElements[7].setOutlineColor(Color::Black);
    textElements[7].setOutlineThickness(3.0f * c.screenScale);
    textElements[7].setPosition(initialX / 3.4f, initialY);

    RectangleShape blackShape;
    blackShape.setPosition(0, 0);
    blackShape.setSize(sf::Vector2f(c.w.getSize().x, c.w.getSize().y));

    // Partial state of the game
    State state = START;

    // Code of sprite to display
    int j = 0, k = 0;

    float elapsed1, elapsed2;
    Clock blinkClcok;
    Time blink_delay = seconds(1.0);

    blinkClcok.restart().asSeconds();
    elapsed1 = blinkClcok.restart().asSeconds();
    bool blink = true;

    c.effects[37]->play();

    // Draw the landscape animation
    for (int i = 255; i >= 0; i -= 5){

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

        blackShape.setFillColor(Color(0, 0, 0, i));
        c.w.draw(blackShape);

        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        if (j < (int) nameGames.size() - 1){
            if (k == 10){
                j++;
                k = 0;
            }
            else {
                k++;
            }
        }
        else {
            j = 0;
        }
    }

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

            // Check if the start keyword has been pressed
            if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
                // Pass to the second menu
                startPressed = true;
                c.effects[1]->stop();
                c.effects[1]->play();
                // sleep(milliseconds(50));
            } else if (c.window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Escape))
                return EXIT;

            if (j < (int) nameGames.size() - 1){
                if (k == 10){
                    j++;
                    k = 0;
                }
                else {
                    k++;
                }
            }
            else {
                j = 0;
            }
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
            if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuUpKey)) {
                // Up cursor pressed
                if (state != MUSIC) {
                    c.effects[0]->stop();
                    c.effects[0]->play();
                    row.setPosition((c.w.getSize().x / 2.f) - 100.0f * c.screenScale,
                                    c.w.getSize().y / 2.f + 75.0f * c.screenScale);
                    state = MUSIC;
                }
            } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuDownKey)) {
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
            if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
                // Pass to the second menu
                startPressed = true;
                c.effects[1]->stop();
                c.effects[1]->play();
            }

            if (j < (int) nameGames.size() - 1){
                if (k == 10){
                    j++;
                    k = 0;
                }
                else {
                    k++;
                }
            }
            else {
                j = 0;
            }
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

        c.effects[37]->stop();
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
    Texture iconBackground, textureShape;
    iconBackground.loadFromFile("Resources/Menus/MenuOptions/icon.png");
    IntRect background(0, 0, c.w.getSize().x, c.w.getSize().y);

    Sprite sprite(iconBackground, background);
    float axis_x = float(c.w.getSize().x) / SCREEN_DEFAULT_X;
    float axis_y = float(c.w.getSize().y) / SCREEN_DEFAULT_Y;
    sprite.setScale(axis_x, axis_y);

    textureShape.loadFromFile("Resources/Menus/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

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
        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuDownKey)) {
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
        } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuUpKey)) {
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
        while (c.window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Space) && !Keyboard::isKeyPressed(Keyboard::Enter)) {
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
        if (c.window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Escape)) {
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

    // Loading the background texture
    Texture iconBackground, textureShape;
    iconBackground.loadFromFile("Resources/Menus/MenuOptions/icon.png");
    IntRect background(0, 0, c.w.getSize().x, c.w.getSize().y);

    Sprite sprite(iconBackground, background);
    float axis_x = float(c.w.getSize().x) / SCREEN_DEFAULT_X;
    float axis_y = float(c.w.getSize().y) / SCREEN_DEFAULT_Y;
    sprite.setScale(axis_x, axis_y);

    textureShape.loadFromFile("Resources/Menus/MenuOptions/outrun.png");
    textureShape.setRepeated(true);

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

        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuDownKey)) {
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
        } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuUpKey)) {
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
            if (c.window.hasFocus() && Keyboard::isKeyPressed(c.leftKey)) {
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
            } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.rightKey)) {
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
            if (c.window.hasFocus() && Keyboard::isKeyPressed(c.leftKey)) {
                if (c.volumeEffects != 0) {
                    c.volumeEffects--;
                    for (int i = 0; i <= 40; i++) {
                        c.effects[i]->setVolume(float(c.volumeEffects));
                    }
                    c.effects[0]->stop();
                    c.effects[0]->play();
                    menuButtons[optionSelected + 2].setTextButton((to_string(c.volumeEffects)));
                }
            } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.rightKey)) {
                if (c.volumeEffects != 100) {
                    c.volumeEffects++;
                    for (int i = 0; i <= 40; i++) {
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
        if (c.window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Escape)) {
            // Change the controllers of the car
            startPressed = true;
            c.modifiedConfig = true;
            c.effects[2]->stop();
            c.effects[2]->play();
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
        Texture iconBackground, textureShape;
        iconBackground.loadFromFile("Resources/Menus/MenuOptions/icon.png");
        IntRect background(0, 0, c.w.getSize().x, c.w.getSize().y);

        Sprite sprite(iconBackground, background);
        float axis_x = float(c.w.getSize().x) / SCREEN_DEFAULT_X;
        float axis_y = float(c.w.getSize().y) / SCREEN_DEFAULT_Y;
        sprite.setScale(axis_x, axis_y);


        textureShape.loadFromFile("Resources/Menus/MenuOptions/outrun.png");
        textureShape.setRepeated(true);

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

        const string submenu = "Menu", access = "Press Enter", saved = "Saved!";

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
            if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuDownKey)) {
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
            } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuUpKey)) {
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
                    if (c.window.hasFocus() && Keyboard::isKeyPressed(c.leftKey)) {
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
                            c.modifiedConfig = true;
                        }
                    } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.rightKey)) {
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
                            c.modifiedConfig = true;
                        }
                    }
                    break;
                case 1:
                    // AI aggressiveness level
                    // Check if left or right cursor keys have been pressed or not
                    if (c.window.hasFocus() && Keyboard::isKeyPressed(c.leftKey)) {
                        if (c.enableAI) {
                            c.enableAI = false;
                            menuButtons[optionSelected + 5].setTextButton("DISABLED");
                            c.modifiedConfig = true;
                        }
                    }
                    else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.rightKey)) {
                        if (!c.enableAI) {
                            c.enableAI = true;
                            menuButtons[optionSelected + 5].setTextButton("ENABLED");
                            c.modifiedConfig = true;
                        }
                    }

                    menuButtons[optionSelected + 6].setTextButton(submenu);
                    break;
                case 2:
                    // Change the volume of the game
                    menuButtons[optionSelected + 5].setTextButton(access);

                    // Check if left or right cursor keys have been pressed or not
                    if (Keyboard::isKeyPressed(c.menuEnterKey)) {
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
                    if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
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
                    if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
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
            if (c.window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Escape)) {
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

    // Update the configuration of the game
    if (c.modifiedConfig){

        const string path = "Resources/Settings/Settings.info";

        KeywordMapper kM = KeywordMapper();

        // Get the keyword to control the turn to the left
        int index = kM.lookForKeyBoardId(c.leftKey);
        string controlLeft = kM.mapperIdKeyWord[index];

        // Get the keyword to control the turn to the right
        index = kM.lookForKeyBoardId(c.rightKey);
        string controlRight = kM.mapperIdKeyWord[index];

        // Get the keyword to control the acceleration
        index = kM.lookForKeyBoardId(c.accelerateKey);
        string controlAccelerate = kM.mapperIdKeyWord[index];

        // Get the keyword to control the brake
        index = kM.lookForKeyBoardId(c.brakeKey);
        string controlBrake = kM.mapperIdKeyWord[index];

        if (c.resIndex == -1){
            c.fullScreen = true;
        }
        else {
            c.fullScreen = false;
        }

        // Update the file with the new configuration
        storeNewConfiguration(path, c.level, c.enableAI, c.volumeMusic, c.volumeEffects, c.enablePixelArt, c.fullScreen,
                              c.resolutions[c.resIndex].first, c.resolutions[c.resIndex].second, controlLeft, controlRight,
                              controlAccelerate, controlBrake);

        // Make possible more modifications
        c.modifiedConfig = false;
    }

    if (inGame) {
        return GAME;
    } else {
        return startMenu(c);
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
    backgroundMusic.loadFromFile("Resources/Menus/MusicMenu/radioBackground.png");
    radioMenu.setTexture(backgroundMusic);
    radioMenu.setPosition(0, 0);
    radioMenu.setScale((float) c.w.getSize().x / backgroundMusic.getSize().x,
                       (float) c.w.getSize().y / backgroundMusic.getSize().y);

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++) {
        // Loading the icon texture
        t.loadFromFile("Resources/Menus/MusicMenu/soundtrack" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++) {
        // Loading the icon texture
        t.loadFromFile("Resources/Menus/MusicMenu/radio" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Load the titles of the soundtracks
    for (int i = 0; i < NUM_SOUNDTRACKS - 1; i++) {
        // Loading the icon texture
        t.loadFromFile("Resources/Menus/MusicMenu/hand" + to_string(i + 1) + ".png");
        textures.push_back(t);
    }

    // Control if the Enter key is pressed
    bool startPressed = false;

    // Stop sounds
    c.themes[0]->stop();
    c.themes[1]->stop();
    c.themes[2]->stop();

    sleep(milliseconds(10));
    c.effects[40]->stop();
    c.effects[40]->play();

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
        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.leftKey)) {
            // Up cursor pressed and change the soundtrack selected in the list
            if (c.currentSoundtrack != 1) {
                c.effects[1]->stop();
                c.effects[1]->play();
                c.themes[c.currentSoundtrack]->stop();
                c.currentSoundtrack--;
                c.themes[c.currentSoundtrack]->play();
            }
        } else if (c.window.hasFocus() && Keyboard::isKeyPressed(c.rightKey)) {
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
        music.setScale(3.0f * c.screenScale, 3.f * c.screenScale);

        // Get the dial movement to reproduce
        radio.setTexture(textures[c.currentSoundtrack + 2], true);
        radio.setScale(3.f * c.screenScale, 3.f * float(c.w.getSize().y) / SCREEN_DEFAULT_Y);

        // Get the hand movement of the driver
        hand.setTexture(textures[c.currentSoundtrack + 5], true);
        hand.setScale(3.f * c.screenScale, 3.f * float(c.w.getSize().y) / SCREEN_DEFAULT_Y);

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
        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
            startPressed = true;
            c.effects[2]->stop();
            c.effects[2]->play();
        }
    }
    c.effects[29]->stop();
    c.themes[0]->stop();
    return GAME;
}



State rankingMenu(Config &c, const unsigned long scorePlayerGame, const int minutes, const int secs, const int cents_Second) {
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
    rankingBackground.loadFromFile("Resources/Menus/RankingMenu/bg.png");
    Sprite palm_trees(rankingBackground);
    palm_trees.setScale((float) c.w.getSize().x / rankingBackground.getSize().x,
                        (float) c.w.getSize().y / rankingBackground.getSize().y);

    // Get the best seventh out runners
    vector<Score> scoreRankingPlayer = getGlobalScores(c);

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

            for (int i = record; i < 6 && i < (int)scoreRankingPlayer.size(); i++) {
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
                } else if (c.window.hasFocus() && event.type == Event::KeyPressed) {
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
        if (c.window.hasFocus() && Keyboard::isKeyPressed(c.menuEnterKey)) {
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
        saveNewRecord(c, scoreRankingPlayer, s);
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



/**
 * Updates the current configuration of the game by a new one modifying
 * the configuration file
 * @param path is the path the configuration file
 * @param difficulty is the level of difficulty selected by the player
 * @param volumeSoundtracks is the volume of the soundtracks selected by the player
 * @param volumeEffects is the volume of the sound effects selected by the player
 * @param pixelArt controls if the graphics must be rendered with pixel art effect or not
 * @param fullScreen controls if the game must be load in full screen or not
 * @param axis_x is the width of the window's game
 * @param axis_y is the height of the window's game
 * @param controlLeft is the key selected by the player to turn the vehicle to the left
 * @param controlRight is the key selected by the player to turn the vehicle to the right
 * @param controlAccelerate is the key selected by the player to accelerate the vehicle
 * @param controlBrake is the key selected by the player to brake the vehicle
 * @param controlSoundtrack is the key selected by the player to change the soundtrack of the game
 */
void storeNewConfiguration(const string path, const Difficult difficulty, const bool enabledAi, const int volumeSoundtracks,
                           const int volumeEffects, const bool pixelArt, const bool fullScreen, const int axis_x, const int axis_y,
                           const string controlLeft, const string controlRight, const string controlAccelerate, const string controlBrake)
{
    //  New file which stores the new configuration of the game
    ofstream f(path);
    // Check if the file has been opened correctly
    if (f.is_open()){
        f << "DIFFICULTY: ";
        switch (difficulty){
            case PEACEFUL:
                f << "Peaceful" << endl;
                break;
            case EASY:
                f << "Easy" << endl;
                break;
            case NORMAL:
                f << "Normal" << endl;
                break;
            case HARD:
                f << "Hard" << endl;
        }

        f << "AI: ";
        if (enabledAi){
            f << "Enabled" << endl;
        }
        else {
            f << "Disabled" << endl;
        }

        f << "VOLUME_SOUNDTRACKS: " << volumeSoundtracks << endl;
        f << "VOLUME_EFFECTS: " << volumeEffects << endl;

        f << "PIXEL_ART: ";
        if (pixelArt){
            f << "Enabled" << endl;
        }
        else {
            f << "Disabled" << endl;
        }

        f << "FULL_SCREEN: ";
        if (fullScreen){
            f << "Enabled" << endl;
        }
        else {
            f << "Disabled" << endl;
        }

        f << "RESOLUTION_X: " << axis_x << endl;
        f << "RESOLUTION_Y: " << axis_y << endl;

        f << "CONTROLLER_LEFT: " << controlLeft << endl;
        f << "CONTROLLER_RIGHT: " << controlRight << endl;
        f << "CONTROLLER_ACCELERATE: " << controlAccelerate << endl;
        f << "CONTROLLER_BRAKE: " << controlBrake << endl;
    }
    f.close();
}
