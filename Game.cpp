/******************************************************************************
 * @file    Engine.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Game.hpp"
#include <string>
#include <cmath>

#define PLAYERTEXTURES 1
#define PLAYERSCALE 3.0f

#define ACC 1.075f
#define BRC 1.85f
#define SPM 200
#define ACM 500

using namespace sf;
using namespace std;

Game::Game(Config &c) : speed(0), acceleration(0), posX(0), mapId(make_pair(0, 0)) {
    int nm = 0;
    int nobjects[] = {6}; // TODO: Más mapas
    //for (int i = 0; i < 5; i++) {
    for (int i = 0; i < 1; i++) { // TODO: Borrar línea y descomentar la de arriba
        vector<Map> vm;
        for (int j = 0; j <= i; j++) {
            vector<string> objectNames;
            objectNames.reserve(nobjects[nm]);
            for (int no = 1; no <= nobjects[nm]; no++)
                objectNames.push_back("resources/map" + to_string(nm) + "/" + to_string(no) + ".png");

            Map m(c.w, "resources/map" + to_string(nm) + "/bg.png", objectNames);
            vm.push_back(m);

            nm++;
        }

        maps.push_back(vm);
    }

    // Player
    for (int i = 1; i <= PLAYERTEXTURES; i++) {
        Texture t;
        t.loadFromFile("resources/player/" + to_string(i) + ".png");
        t.setSmooth(true);
        t.setRepeated(false);
        playerTextures.push_back(t);
    }
    sPlayer.setTexture(playerTextures[0]);
    sPlayer.setScale(PLAYERSCALE, PLAYERSCALE);
    sPlayer.setPosition(((float)c.w.getSize().x)/2.0f - sPlayer.getGlobalBounds().width / 2.0f,
            ((float)c.w.getSize().y) * c.camD - sPlayer.getGlobalBounds().height / 2.0f);

    // Text
    sText.setFillColor(Color::White);
    sText.setCharacterSize(50);
    sText.setPosition((float) sText.getCharacterSize(), (float) c.w.getSize().y - (float) sText.getCharacterSize());
    sText.setFont(c.font);
}

State Game::play(Config &c) {
    while (c.w.isOpen()) {
        c.w.clear();
        mapControl(c);

        Event e{};
        while (c.w.pollEvent(e)) {
            if (e.type == Event::Closed)
                c.w.close();
        }

        if (Keyboard::isKeyPressed(c.menuKey))
            return PAUSE;

        accelerationControl(c);
        rotationControl(c);

        // Draw car
        c.w.draw(sPlayer);

        c.w.display();
    }

    return EXIT;
}

void Game::mapControl(Config &c) {
    maps[mapId.first][mapId.second].draw(c, c.camD, posX, speed);
    //TODO: Añadir bifurcaciones
}

void Game::accelerationControl(Config &c) {
    //TODO Ejemplo de aceleración y frenado, dibujar y adaptar al coche
    if (Keyboard::isKeyPressed(c.brakeKey)) {
        speed -= pow(2.0f, 1.0f / (BRC * BRC)) - 1.0f;
        acceleration -= pow(2.0f, (ACC * ACC)) - 1.0f;
        //TODO sPlayer
    }
    else if (Keyboard::isKeyPressed(c.accelerateKey)) {
        acceleration += pow(2.0f, 1.0f / (ACC * ACC)) - 1.0f;

        speed += pow(2.0f, 1.0f / (acceleration * acceleration)) - 1.0f;

        //TODO sPlayer
    }
    else {
        speed -= pow(2.0f, 1.0f / (BRC * BRC * BRC)) - 1.0f;
        acceleration -= pow(2.0f, (ACC * ACC)) - 1.0f;
    }
    if (speed < 0.0f)
        speed = 0.0f;
    if (acceleration < 0.0f || speed < 0.5f)
        acceleration = 0.0f;
    if (speed > SPM)
        speed = SPM;
    if (acceleration > ACM)
        acceleration = ACM;
    //TODO END

    string strSpeed = to_string(speed * 100);
    sText.setString(strSpeed.substr(0, strSpeed.find('.')));
    c.w.draw(sText);
}

void Game::rotationControl(Config &c) {
    if (Keyboard::isKeyPressed(c.leftKey)) {
        posX--;
        //TODO sPlayer
    }
    if (Keyboard::isKeyPressed(c.rightKey)) {
        posX++;
        //TODO sPlayer
    }
}
