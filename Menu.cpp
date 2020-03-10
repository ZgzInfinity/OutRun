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

using namespace sf;

Config::Config() {
    w.create(VideoMode(1366, 700), "Outrun Racing!", Style::Titlebar | Style::Close);
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
}

void changeConfig(Config &c) {

}

State startMenu(Config &c) {
    c.w.clear();

    int selected = 0;
    const int elements = 2;
    Text text[elements];
    text[0].setString("START");
    text[1].setString("EXIT");

    while (c.w.isOpen()) {
        Event e{};
        while (c.w.pollEvent(e)) {
            if (e.type == Event::Closed)
                c.w.close();
        }

        if (Keyboard::isKeyPressed(c.menuUpKey)) {
            if (selected > 0)
                selected--;
        }
        else if (Keyboard::isKeyPressed(c.menuDownKey)) {
            if (selected < elements - 1)
                selected++;
        }
        else if (Keyboard::isKeyPressed(c.menuEnterKey)) {
            if (selected == 0)
                return GAME;
            else if (selected == 1)
                return EXIT;
        }

        for (int i = 0; i < elements; i++) {
            if (i == selected)
                text[i].setFillColor(Color::Yellow);
            else
                text[i].setFillColor(Color::White);

            text[i].setCharacterSize(TEXTSIZE);
            text[i].setPosition(c.w.getSize().x / 2.0f - (float) TEXTSIZE,
                    ((float) (TEXTSIZE * i)) + c.w.getSize().y / 3.0f);
            text[i].setFont(c.font);
            c.w.draw(text[i]);
        }

        c.w.display();
    }

    return EXIT;
}

State pauseMenu(Config &c) {
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
