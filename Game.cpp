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

#define MAX_SPEED 300.0f
#define SPEED_MUL 100.0f
#define ACC_INC 0.01f
#define MAX_COUNTER 10
#define MAX_VEHICLES 10 // Number of vehicles simultaneously
#define VEHICLE_DENSITY 3.0f // Greater than 0
#define VEHICLE_MIN_DISTANCE 5.0f // Minimum number of rectangles between enemies
#define DEL_VEHICLE 50.0f // Minimum number of rectangles behind the camera to delete the enemy

Game::Game(Config &c) : player(MAX_SPEED, SPEED_MUL, ACC_INC, 1.0f, MAX_COUNTER, "Ferrari", 0.0f, RECTANGLE),
                        lastY(0), vehicleCrash(false) {
    int nm = 15;
    int nobjects[] = {6, 15, 15, 40, 0, 25, 0, 26, 0, 0, 0, 0, 0, 0, 0, 33}; // TODO: Más mapas
    for (int i = 0; i < 5; i++) {
        vector<Map> vm;
        for (int j = 0; j <= i; j++) {
            vector<string> objectNames;
            objectNames.reserve(nobjects[nm]);
            for (int no = 1; no <= nobjects[nm]; no++)
                objectNames.push_back(to_string(no) + ".png");

            Map m(c, "resources/map" + to_string(nm) + "/", "bg.png", objectNames, false);
            vm.push_back(m);

            // nm++; // TODO: Añadir más mapas y descomentar
        }
        maps.emplace_back(vm);

        // nm++; // TODO: Añadir más mapas y borrar línea
        // nm = nm % 3; // TODO: Añadir más mapas y borrar línea
    }
    mapId = make_pair(0, 0);
    currentMap = &maps[mapId.first][mapId.second];
    currentMap->addNextMap(&maps[mapId.first + 1][mapId.second]); // TODO: Añadir bifurcación
    isInitMap = true;

    // Vehicles
    cars.reserve(MAX_VEHICLES);
    const int maxSprites = 1; // TODO: Añadir más vehículos
    for (int i = 0; i < MAX_VEHICLES; i++) {
        Enemy v(MAX_SPEED, SPEED_MUL, 1.25f, MAX_COUNTER, "car" + to_string(1 + i % maxSprites), -RECTANGLE);
        cars.push_back(v);
    }

    Texture t;
    Sprite s;
    // Load the textures of the panel and assign them to their sprites
    for (int i = 1; i <= 6; i++){
        // Load the texture from the file
        t.loadFromFile("resources/GamePanel/" + to_string(i) + ".png");
        textures.push_back(t);
    }

    for (int i = 0; i < 7; i++){
        s.setTexture(textures[i], true);
        sprites.push_back(s);
    }

    // Text
    sText.setFillColor(Color(206, 73, 73));
    sText.setOutlineColor(Color::Black);
    sText.setOutlineThickness(3);
    sText.setCharacterSize(70);
    sText.setFont(c.speedVehicle);

    // Initialize the HUD indicator of time
    time = MAX_TIME;
    timeToPlay.setString(to_string(time));
    timeToPlay.setFont(c.timeToPlay);
    timeToPlay.setPosition(c.w.getSize().x / 2.f - 310, c.w.getSize().y / 2.f - 340);
    timeToPlay.setCharacterSize(50);
    timeToPlay.setFillColor(Color::Yellow);
    timeToPlay.setOutlineColor(Color::Black);
    timeToPlay.setOutlineThickness(3);

    // Initialize the HUD indicator of score
    score = 0;
    textScore.setString(to_string(score));
    textScore.setFont(c.timeToPlay);
    textScore.setPosition(c.w.getSize().x / 2.f + 40, c.w.getSize().y / 2.f - 325);
    textScore.setCharacterSize(35);
    textScore.setFillColor(Color(183, 164, 190));
    textScore.setOutlineColor(Color::Black);
    textScore.setOutlineThickness(3);

    // Initialize the HUD indicator of lap time
    textLap.setFont(c.timeToPlay);
    textLap.setPosition(c.w.getSize().x / 2.f + 250, c.w.getSize().y / 2.f - 325);
    textLap.setCharacterSize(35);
    textLap.setFillColor(Color(146, 194, 186));
    textLap.setOutlineColor(Color::Black);
    textLap.setOutlineThickness(3);

    // Initialize the HUD stage indicator
    level = 1;
    textLevel.setFont(c.timeToPlay);
    textLevel.setPosition(c.w.getSize().x / 2.f + 305, c.w.getSize().y / 2.f + 268);
    textLevel.setCharacterSize(40);
    textLevel.setFillColor(Color(146, 194, 186));
    textLevel.setOutlineColor(Color::Black);
    textLevel.setOutlineThickness(3);

    // Game over indicator
    gameOver.setFont(c.timeToPlay);
    gameOver.setPosition(c.w.getSize().x / 2.f - 125, c.w.getSize().y / 2.f - 50);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(60);
    gameOver.setFillColor(Color::Yellow);
    gameOver.setOutlineColor(Color(14, 29, 184));
    gameOver.setOutlineThickness(3);

    // Control if the player is still playing
    finalGame = false;
}

State Game::play(Config &c) {
    c.themes[c.currentSoundtrack]->play();

    initialAnimation(c); // TODO: Aquí es el lugar correcto, o primero se tiene que mostrar la velocidad y tal??

    c.w.setKeyRepeatEnabled(false);

    // Time to update the clock counter
    Time shot_delayTime = seconds(1.0);

    // Time to update the clock counter lap
    Time shot_delayLap = seconds(0.1);
    float elapsed1, elapsed2, elapsed3, elapsed4;

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    // Assign positions in the game console for the game panel indicators
    for (int i = 0; i < 7; i++){
        switch(i){
            case 0: // TODO: Usar porcentajes, no restas y sumas, para así permitir distintas resoluciones de pantalla
                sprites[i].setPosition(c.w.getSize().x / 2.f - 430, c.w.getSize().y / 2.f - 330);
                sprites[i].scale(1.5f, 1.5f);
                break;
            case 1:
                sprites[i].setPosition(c.w.getSize().x / 2.f - 200, c.w.getSize().y / 2.f - 330);
                sprites[i].scale(1.5f, 1.5f);
                break;
            case 2:
                sprites[i].setPosition(c.w.getSize().x / 2.f + 150 , c.w.getSize().y / 2.f - 323);
                sprites[i].scale(1.5f, 1.5f);
                break;
            case 3:
                sprites[i].setPosition(c.w.getSize().x / 2.f - 300 , c.w.getSize().y / 2.f + 260);
                sprites[i].scale(2.f, 2.f);
                break;
            case 4:
                sprites[i].setPosition(c.w.getSize().x / 2.f + 370 , c.w.getSize().y / 2.f + 270);
                sprites[i].scale(2.f, 2.f);
                break;
            case 5:
                sprites[i].setPosition(c.w.getSize().x / 2.f + 160 , c.w.getSize().y / 2.f + 284);
                sprites[i].scale(1.5f, 1.5f);
                break;
            case 6:
                sprites[i].setPosition(c.w.getSize().x / 2.f - 420 , c.w.getSize().y / 2.f + 320);
                sprites[i].scale(2.f, 1.5f);
        }
    }

    while (!finalGame && c.w.isOpen()) {
        updateAndDraw(c);

        if (!finalGame) {
            Event e{};
            while (c.w.pollEvent(e)) {
                if (e.type == Event::Closed)
                    c.w.close();
            }

            if (Keyboard::isKeyPressed(c.menuKey))
                return PAUSE;

            // Draw speed
            string strSpeed = to_string(player.getRealSpeed());
            sText.setString(strSpeed.substr(0, strSpeed.find('.')));
            sText.setPosition((float) (c.w.getSize().x / 2.f) - 310 - sText.getLocalBounds().width,
                              (float) c.w.getSize().y / 2.f + 240);

            textures[6].loadFromFile("resources/GamePanel/7.png",
                                     IntRect(0, 0, ((int) player.getRealSpeed() * 117 / MAX_SPEED), 20));
            sprites[6].setTexture(textures[6], true);

            c.w.draw(sText);

            // Update the score of the player if the player is not stopped
            if (player.getRealSpeed() > 0.f) {
                // Add score
                score++;
            }

            // Get the actual time
            elapsed2 = gameClockTime.getElapsedTime().asSeconds();

            // Check if a second has passed between both timestamps
            if (elapsed2 - elapsed1 >= shot_delayTime.asSeconds()) {
                // Draw time
                time--;
                gameClockTime.restart();
            }

            // Get the actual time
            elapsed4 = gameClockLap.getElapsedTime().asSeconds();

            // Check if a tenth of second has passed between both timestamps
            if (elapsed4 - elapsed3 >= shot_delayLap.asSeconds()) {
                decs_second++;
                gameClockLap.restart();
                if (decs_second == 100) {
                    decs_second = 0;
                    secs++;
                    if (secs == 60) {
                        secs = 0;
                        minutes++;
                    }
                }
            }

            // Update the indicators
            string lap = "";
            lap = (minutes < 10) ? lap + "0" + to_string(minutes) + " '" : lap + to_string(minutes) + " ''";
            lap = (secs < 10) ? lap + "0" + to_string(secs) + " ''" : lap + to_string(secs) + " ''";
            lap = (decs_second < 10) ? lap + "0" + to_string(decs_second) : lap + to_string(decs_second);

            timeToPlay.setString(to_string(time));
            textScore.setString(to_string(score));
            textLap.setString(lap);
            textLevel.setString(to_string(level));

            // Draw the panel indicators
            for (int i = 0; i < 6; i++) {
                c.w.draw(sprites[i]);
            }

            if (player.getRealSpeed() > 0) {
                c.w.draw(sprites[6]);
            }

            c.w.draw(timeToPlay);
            c.w.draw(textScore);
            c.w.draw(textLap);
            c.w.draw(textLevel);
            c.w.display();

            // Check if the player has time to continue
            if (time == 0) {
                // Final game
                finalGame = true;
            }
        }
    }
    // Draw the game over text in the console window
    c.w.draw(gameOver);
    c.w.display();

    //system("pause"); // TODO: No usar system(), "pause" puede no existir. Se ha cambiado por el siguiente bucle:
    bool startPressed = false;
    while (!startPressed)
        startPressed = Keyboard::isKeyPressed(c.menuEnterKey);

    return EXIT;
}

void Game::initialAnimation(Config &c) {
    int flagger, semaphore;
    Map *initMap = new Map(*currentMap, flagger, semaphore);
    initMap->addNextMap(currentMap);
    currentMap = initMap;

    // Prepare car
    bool end = false;
    for (int i = (int) c.w.getSize().x / 2; !end; i -= 3) {
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.drawAnimation(c, float(i), end);
        c.w.display();
    }
    sleep(milliseconds(200));

    // Semaphore and flagger
    currentMap->incrementSpriteIndex(flagger, false, -1);
    int ms = 1000;
    for (int i = 0; i < 3; i++) {
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
        c.w.display();

        // Flagger
        if (i == 2) {
            for (; ms > 0; ms -= 200) {
                sleep(milliseconds(200));

                currentMap->incrementSpriteIndex(flagger, false);

                // Draw map
                c.w.clear();
                currentMap->draw(c, cars);
                player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
                c.w.display();
            }
        }

        // Change semaphore state
        if (ms > 0)
            sleep(milliseconds(ms));
        currentMap->incrementSpriteIndex(semaphore, false);
    }
    currentMap->incrementSpriteIndex(flagger, false, -1);
}

void Game::updateAndDraw(Config &c) {
    // Update camera
    currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

    if (currentMap->isOver()) {
        // TODO: Añadir bifurcación
        if (!isInitMap)
            mapId.first++;
        if (isInitMap || mapId.first < maps.size()) {
            if (isInitMap)
                isInitMap = false;

            // Update player and vehicle positions
            player.setPosition(player.getPosX(), player.getPosY() - currentMap->getMaxY());
            for (Vehicle &v : cars)
                v.setPosition(v.getPosX(), v.getPosY() - currentMap->getMaxY());

            currentMap = &maps[mapId.first][mapId.second];
            if (mapId.first < maps.size() - 1)
                currentMap->addNextMap(&maps[mapId.first + 1][mapId.second]);
            currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

            lastY = currentMap->getCamY();
        }
        else {
            finalGame = true;
        }
    }

    if (!finalGame) {
        // Update and prepare cars to draw
        if (lastY <= currentMap->getCamY() + float(c.renderLen))
            lastY = currentMap->getCamY() + float(c.renderLen);
        for (Enemy &v : cars) {
            if (v.getPosY() + DEL_VEHICLE < currentMap->getCamY()) {
                v.update(lastY, lastY + float(c.renderLen) / VEHICLE_DENSITY);
                lastY = v.getPosY() + VEHICLE_MIN_DISTANCE * RECTANGLE;
            }

            v.autoControl();
            float posY = v.getPosY();
            v.draw(c, currentMap->getElevation(posY), currentMap->getCamX());
        }

        // Draw map with cars
        c.w.clear();
        currentMap->draw(c, cars);

        // Player update and draw
        Vehicle::Action action = Vehicle::CRASH;
        Vehicle::Direction direction = Vehicle::RIGHT;

        if (!player.isCrashing()) { // If not has crashed
            action = player.accelerationControl(c, currentMap->hasGotOut(player.getPosX()));
            direction = player.rotationControl(c, currentMap->getCurveCoefficient(player.getPosY()));
        }
        else {
            player.hitControl(vehicleCrash);
        }

        player.draw(c, action, direction, currentMap->getElevation(player.getPosY()));

        if (!player.isCrashing()) {
            vehicleCrash = false;
            float crashPos;
            bool crash = currentMap->hasCrashed(c, player.getPreviousY(), player.getPosY(), player.getMinScreenX(),
                                                player.getMaxScreenX(), crashPos);
            if (!crash)
                for (int i = 0; !vehicleCrash && i < cars.size(); i++)
                    vehicleCrash = cars[i].hasCrashed(c, player.getPreviousY(), player.getPosY(),
                                                      player.getMinScreenX(),
                                                      player.getMaxScreenX(), crashPos);
            if (crash || vehicleCrash) {
                player.setPosition(player.getPosX(), crashPos);
                player.hitControl(vehicleCrash);
                action = Vehicle::CRASH;
                direction = Vehicle::RIGHT;

                player.draw(c, action, direction, currentMap->getElevation(player.getPosY()));
            }
        }
    }
}
