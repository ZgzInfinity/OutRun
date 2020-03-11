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

#define MAXCOUNT 15

#define PLAYERTEXTURES 40
#define PLAYERSCALE 1.0f

#define ACC 1.075f
#define BRC 1.85f
#define SPM 200
#define ACM 500

using namespace sf;
using namespace std;

Game::Game(Config &c) : speed(0), acceleration(0), posX(0), mapId(make_pair(0, 0)), actual_code_image(1),
counter_code_image(0) {
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
        t.loadFromFile("resources/Ferrari/c" + to_string(i) + ".png");
        t.setSmooth(true);
        t.setRepeated(false);
        playerTextures.push_back(t);
    }

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

        drawPlayer(c, accelerationControl(c), rotationControl(c));

        c.w.display();
    }

    return EXIT;
}

void Game::mapControl(Config &c) {
    maps[mapId.first][mapId.second].draw(c, c.camD, posX, speed);
    //TODO: Añadir bifurcaciones
}

Game::Action Game::accelerationControl(Config &c) {
    Action a = NONE;

    //TODO Ejemplo de aceleración y frenado, dibujar y adaptar al coche
    if (Keyboard::isKeyPressed(c.brakeKey)) {
        speed -= pow(2.0f, 1.0f / (BRC * BRC)) - 1.0f;
        acceleration -= pow(2.0f, (ACC * ACC)) - 1.0f;

        a = BRAKE;
    }
    else if (Keyboard::isKeyPressed(c.accelerateKey)) {
        acceleration += pow(2.0f, 1.0f / (ACC * ACC)) - 1.0f;

        speed += pow(2.0f, 1.0f / (acceleration * acceleration)) - 1.0f;
    }
    else {
        speed -= pow(2.0f, 1.0f / (BRC * BRC * BRC)) - 1.0f;
        acceleration -= pow(2.0f, (ACC * ACC)) - 1.0f;
    }

    if (speed <= 0.0f)
        speed = 0.0f;
    else if (a == NONE)
        a = ACCELERATE;

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

    return a;
}

Game::Direction Game::rotationControl(Config &c) {
    if (speed > 0.0f) {
        if (Keyboard::isKeyPressed(c.leftKey)) {
            posX--;
            return TURNLEFT;
        }

        if (Keyboard::isKeyPressed(c.rightKey)) {
            posX++;
            return TURNRIGHT;
        }
    }

    return RIGHT;
}

void Game::drawPlayer(Config &c, Game::Action a, Game::Direction d) {
    if (a != NONE && counter_code_image >= MAXCOUNT) {
        counter_code_image = 0;

        if (speed > 0.0f)
            actual_code_image++;

        if (a == ACCELERATE) {
            if (d == TURNLEFT) {
                if (actual_code_image < 5 || actual_code_image > 12)
                    actual_code_image = 5;
            }
            else if (d == TURNRIGHT) {
                if (actual_code_image < 13 || actual_code_image > 20)
                    actual_code_image = 13;
            }
            else {
                if (actual_code_image < 1 || actual_code_image > 4)
                    actual_code_image = 1;
            }
        }
        else if (a == BRAKE) {
            if (d == TURNLEFT) {
                if (actual_code_image < 25 || actual_code_image > 32)
                    actual_code_image = 25;
            }
            else if (d == TURNRIGHT) {
                if (actual_code_image < 33 || actual_code_image > 40)
                    actual_code_image = 33;
            }
            else {
                if (actual_code_image < 21 || actual_code_image > 24)
                    actual_code_image = 21;
            }
        }
    }
    else {
        counter_code_image++;
    }

    sPlayer.setTexture(playerTextures[actual_code_image - 1]);
    sPlayer.setScale(PLAYERSCALE, PLAYERSCALE);
    sPlayer.setPosition(((float)c.w.getSize().x)/2.0f - sPlayer.getGlobalBounds().width / 2.0f,
                        ((float)c.w.getSize().y) * c.camD - sPlayer.getGlobalBounds().height / 2.0f);
    c.w.draw(sPlayer);
}
