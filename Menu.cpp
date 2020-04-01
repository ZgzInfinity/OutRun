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

    camD = 0.84; // Camera depth
    renderLen = 300;
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

    // Loading the background texture
    backgroundMenu.loadFromFile("resources/MainMenu/LogoMain1.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0 , 0);
    mainMenu.setScale(2.9, 3.1);

    // Loading the texture of the game's name
    gameIcon.loadFromFile("resources/MainMenu/LogoMain2.png");
    nameGame.setTexture(gameIcon);
    nameGame.setPosition((c.w.getSize().x / 2.f) - 180, c.w.getSize().y / 2.f - 200);
    nameGame.setScale(2.0, 2.0);


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
            c.w.draw(nameGame);
            c.w.draw(textElements[0]);
            c.w.display();
            sleep(milliseconds(180));

            // Check if the start keyword has been pressed
            if (Keyboard::isKeyPressed(c.menuEnterKey)){
                // Pass to the second menu
                startPressed = true;
            }
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
            c.w.draw(nameGame);
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
        }
        // Return the state of the game
        return state;
    }
    return EXIT;
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

    // Load the soundtracks in the buffer to reproduce them
    for (int i = 0; i < NUM_SOUNDTRACKS; i++){
        // Loading the dial movements of the hand's driver
        switch(i){
            case 0:
                sound.loadFromFile("resources/Audio/MagicalSoundShower.wav");
                break;
            case 1:
                sound.loadFromFile("resources/Audio/PassingBreeze.wav");
                break;
            case 2:
                sound.loadFromFile("resources/Audio/SplashWave.wav");
        }
        c.reproductor.push_back(sound);
    }

    // Control if the Enter key is pressed
    bool startPressed = false;
    // Identifier of soundtrack to reproduce
    int currentSoundtrack = 0;

    c.soundtrackList.setBuffer(c.reproductor[currentSoundtrack]);
    c.soundtrackList.play();

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
            if (currentSoundtrack != 0){
                currentSoundtrack--;
                c.soundtrackList.stop();
                c.soundtrackList.setBuffer(c.reproductor[currentSoundtrack]);
                c.soundtrackList.play();
            }
        }
        else if (Keyboard::isKeyPressed(c.rightKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (currentSoundtrack != NUM_SOUNDTRACKS - 1){
                currentSoundtrack++;
                c.soundtrackList.stop();
                c.soundtrackList.setBuffer(c.reproductor[currentSoundtrack]);
                c.soundtrackList.play();
            }

        }

        // Load the texture of the soundtrack to display in the radio panel
        music.setTexture(textures[currentSoundtrack], true);

        // Get the dial movement to reproduce
        radio.setTexture(textures[currentSoundtrack + 3], true);

        // Get the hand movement of the driver
        hand.setTexture(textures[currentSoundtrack + 6], true);

        // Control the coordinates X and Y where display the title
        switch(currentSoundtrack){
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
    c.soundtrackList.stop();
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
