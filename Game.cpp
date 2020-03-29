/******************************************************************************
 * @file    Game.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Game.hpp"
#include <string>

using namespace sf;
using namespace std;

Game::Game(Config &c) : mapId(make_pair(0, 0)), player(300.0f, 100.0f, 0.01f, 1.0f,
        40, 10, "Ferrari") {
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

        // Player update and draw
        player.draw(c, player.accelerationControl(c), player.rotationControl(c));

        // Draw speed
        string strSpeed = to_string(player.getRealSpeed());
        sText.setString(strSpeed.substr(0, strSpeed.find('.')));
        c.w.draw(sText);

        c.w.display();

        hitControl(c);
    }

    return EXIT;
}

void Game::mapControl(Config &c) {
    // Update camera
    maps[mapId.first][mapId.second].updateView(player.getPosition());

    // Draw map
    maps[mapId.first][mapId.second].draw(c);

    //TODO: Añadir bifurcaciones
}

void Game::hitControl(Config &c) {
    // Player hits with map object
    if (maps[mapId.first][mapId.second].hasCrashed(c, player.getPreviousY(), player.getPosY(), player.getMinScreenX(), player.getMaxScreenX())) {
        player.resetPosition(0.0f, player.getPosition().second - 1.0f);
    }
}
