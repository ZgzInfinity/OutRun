
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

Game::Game(Config &c, Interface& interface) : player(MAX_SPEED, SPEED_MUL, ACC_INC, 1.0f, MAX_COUNTER, "Ferrari", 0.0f, RECTANGLE),
                                              lastY(0), vehicleCrash(false), goalMap(goalFlagger, goalEnd) {
    int nm = 0;
    int nobjects[] = {20, 28, 40, 15, 25, 29, 26, 31, 33, 30, 30, 30, 34, 39, 33}; // TODO: Más mapas
    for (int i = 0; i < 5; i++) {
        vector<Map> vm;
        for (int j = 0; j <= i; j++) {
            vector<string> objectNames;
            objectNames.reserve(nobjects[nm]);
            for (int no = 1; no <= nobjects[nm]; no++)
                objectNames.push_back(to_string(no) + ".png");

            Map m(c, "resources/map" + to_string(nm + 1) + "/", "bg.png", objectNames, false);
            vm.push_back(m);

            nm++;
        }
        maps.emplace_back(vm);
    }
    mapId = make_pair(0, 0);
    currentMap = &maps[mapId.first][mapId.second];
    currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);

    // Vehicles
    cars.reserve(MAX_VEHICLES);
    const int maxSprites = 6;
    const float vehicleScales[maxSprites] = {1.25f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    for (int i = 0; i < MAX_VEHICLES; i++) {
        Enemy v(MAX_SPEED, SPEED_MUL, vehicleScales[i % maxSprites], MAX_COUNTER,
                "car" + to_string(1 + i % maxSprites), -RECTANGLE);
        cars.push_back(v);
    }

    Texture t;
    Sprite s;
    // Load the textures of the panel and assign them to their sprites
    for (int i = 1; i <= 6; i++){
        // Load the texture from the file
        t.loadFromFile("resources/GamePanel/" + to_string(i) + ".png");
        interface.textures.push_back(t);
    }

    for (int i = 0; i < 7; i++){
        s.setTexture(interface.textures[i], true);
        interface.sprites.push_back(s);
    }

    // Assign positions in the game console for the game panel indicators
    for (int i = 0; i < 7; i++){
        switch(i){
            case 0: // TODO: Usar porcentajes, no restas y sumas, para así permitir distintas resoluciones de pantalla
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f - 430, c.w.getSize().y / 2.f - 330);
                interface.sprites[i].scale(1.5f, 1.5f);
                break;
            case 1:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f - 200, c.w.getSize().y / 2.f - 330);
                interface.sprites[i].scale(1.5f, 1.5f);
                break;
            case 2:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f + 150 , c.w.getSize().y / 2.f - 323);
                interface.sprites[i].scale(1.5f, 1.5f);
                break;
            case 3:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f - 300 , c.w.getSize().y / 2.f + 260);
                interface.sprites[i].scale(2.f, 2.f);
                break;
            case 4:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f + 370 , c.w.getSize().y / 2.f + 270);
                interface.sprites[i].scale(2.f, 2.f);
                break;
            case 5:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f + 160 , c.w.getSize().y / 2.f + 284);
                interface.sprites[i].scale(1.5f, 1.5f);
                break;
            case 6:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f - 420 , c.w.getSize().y / 2.f + 320);
                interface.sprites[i].scale(2.f, 1.5f);
        }
    }

    // Text
    interface.sText.setFillColor(Color(206, 73, 73));
    interface.sText.setOutlineColor(Color::Black);
    interface.sText.setOutlineThickness(3);
    interface.sText.setCharacterSize(70);
    interface.sText.setFont(c.speedVehicle);

    // Initialize the HUD indicator of time
    time = MAX_TIME;
    interface.timeToPlay.setString(to_string(time));
    interface.timeToPlay.setFont(c.timeToPlay);
    interface.timeToPlay.setPosition(c.w.getSize().x / 2.f - 310, c.w.getSize().y / 2.f - 340);
    interface.timeToPlay.setCharacterSize(50);
    interface.timeToPlay.setFillColor(Color::Yellow);
    interface.timeToPlay.setOutlineColor(Color::Black);
    interface.timeToPlay.setOutlineThickness(3);

    // Initialize the HUD indicator of score
    score = 0;
    interface.textScore.setString(to_string(score));
    interface.textScore.setFont(c.timeToPlay);
    interface.textScore.setPosition(c.w.getSize().x / 2.f + 40, c.w.getSize().y / 2.f - 325);
    interface.textScore.setCharacterSize(35);
    interface.textScore.setFillColor(Color(183, 164, 190));
    interface.textScore.setOutlineColor(Color::Black);
    interface.textScore.setOutlineThickness(3);

    // Initialize the HUD indicator of lap time
    interface.textLap.setFont(c.timeToPlay);
    interface.textLap.setPosition(c.w.getSize().x / 2.f + 250, c.w.getSize().y / 2.f - 325);
    interface.textLap.setCharacterSize(35);
    interface.textLap.setFillColor(Color(146, 194, 186));
    interface.textLap.setOutlineColor(Color::Black);
    interface.textLap.setOutlineThickness(3);

    // Initialize the HUD stage indicator
    level = 1;
    interface.textLevel.setFont(c.timeToPlay);
    interface.textLevel.setPosition(c.w.getSize().x / 2.f + 305, c.w.getSize().y / 2.f + 268);
    interface.textLevel.setCharacterSize(40);
    interface.textLevel.setFillColor(Color(146, 194, 186));
    interface.textLevel.setOutlineColor(Color::Black);
    interface.textLevel.setOutlineThickness(3);

    // Game over indicator
    interface.gameOver.setFont(c.timeToPlay);
    interface.gameOver.setPosition(c.w.getSize().x / 2.f - 125, c.w.getSize().y / 2.f - 50);
    interface.gameOver.setString("GAME OVER");
    interface.gameOver.setCharacterSize(60);
    interface.gameOver.setFillColor(Color::Yellow);
    interface.gameOver.setOutlineColor(Color(14, 29, 184));
    interface.gameOver.setOutlineThickness(3);

    // Control if the player is still playing
    finalGame = false;
    inGame = false;
    onPause = false;
}


bool Game::isInGame(){
    return inGame;
}



State Game::play(Config &c, Interface& interface) {
    c.themes[c.currentSoundtrack]->play();

    if (!inGame) {
        inGame = true;
        initialAnimation(c);
    }

    c.w.setKeyRepeatEnabled(false);

    // Time to update the clock counter
    Time shot_delayTime = seconds(1.0);

    // Time to update the clock counter lap
    Time shot_delayLap = seconds(0.1);

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    Vehicle::Action action;
    Vehicle::Direction direction;

    State status;

    while (!finalGame && c.w.isOpen()) {
        updateAndDraw(c, action, direction);

        if (!finalGame) {
            Event e{};
            while (c.w.pollEvent(e)) {
                if (e.type == Event::Closed)
                    c.w.close();
            }

            if (Keyboard::isKeyPressed(c.menuKey) || onPause){
                // Pause the game
                c.effects[1]->stop();
                c.effects[1]->play();
                status = pause(c, interface, action, direction);

                // Control the exit of the game
                if (status == OPTIONS){
                    finalGame = true;
                }
                else if (status == START){
                    finalGame = true;
                }
                else if (status == GAME){

                }
            }

            // Draw speed
            string strSpeed = to_string(player.getRealSpeed());
            interface.sText.setString(strSpeed.substr(0, strSpeed.find('.')));
            interface.sText.setPosition((float) (c.w.getSize().x / 2.f) - 310 - interface.sText.getLocalBounds().width,
                              (float) c.w.getSize().y / 2.f + 240);

            interface.textures[6].loadFromFile("resources/GamePanel/7.png",
                                     IntRect(0, 0, ((int) player.getRealSpeed() * 117 / MAX_SPEED), 20));
            interface.sprites[6].setTexture(interface.textures[6], true);

            c.w.draw(interface.sText);

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

            interface.timeToPlay.setString(to_string(time));
            interface.textScore.setString(to_string(score));
            interface.textLap.setString(lap);
            interface.textLevel.setString(to_string(level));

            // Draw the panel indicators
            for (int i = 0; i < 6; i++) {
                c.w.draw(interface.sprites[i]);
            }

            if (player.getRealSpeed() > 0) {
                c.w.draw(interface.sprites[6]);
            }

            c.w.draw(interface.timeToPlay);
            c.w.draw(interface.textScore);
            c.w.draw(interface.textLap);
            c.w.draw(interface.textLevel);
            c.w.display();

            // Check if the player has time to continue
            if (time == 0) {
                // Final game
                finalGame = true;
            }
        }
    }

    finalGame = false;

    if (status != OPTIONS && status != START) {
        // Draw the game over text in the console window
        c.w.draw(interface.gameOver);
        c.w.display();

        bool startPressed = false;
        while (!startPressed)
            startPressed = Keyboard::isKeyPressed(c.menuEnterKey);

        return EXIT;
    }
    else if (status == START){
        inGame = false;
    }
    return status;
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
        player.drawInitialAnimation(c, float(i), end);
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

void Game::goalAnimation(Config &c) {
    // Hide enemies
    for (Enemy &v : cars)
        v.setPosition(v.getPosX(), -RECTANGLE);

    player.setSmoking(false);
    int increment = 0;
    float currentTime = gameClockTime.getElapsedTime().asMilliseconds();
    while (int(player.getPosY()) < goalEnd) {
        // Update camera
        currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.setPosition(player.getPosX(), player.getPosY() + 1);
        player.draw(c, Vehicle::Action::ACCELERATE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
        c.w.display();

        // Flager animation
        if (gameClockTime.getElapsedTime().asMilliseconds() - currentTime >= 200.0f) {
            if (increment >= 5) {
                increment = 0;
                currentMap->incrementSpriteIndex(goalFlagger, false, -5);
            }
            else {
                currentMap->incrementSpriteIndex(goalFlagger, false);
                increment++;
            }

            currentTime = gameClockTime.getElapsedTime().asMilliseconds();
        }

        // TODO: Ir mostrando bonus y tal
    }

    // Car animation
    int step = 0;
    bool end = false;
    while (!end) {
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.drawGoalAnimation(c, step, end);
        c.w.display();
    }
}

void Game::updateAndDraw(Config &c, Vehicle::Action& action, Vehicle::Direction &direction) {
    // Update camera
    currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

    if (currentMap->isGoalMap()) {
        goalAnimation(c);
        finalGame = true;
    }
    else if (currentMap->isOver()) {
        if (currentMap->getNext() != nullptr) {
            // Update player and vehicle positions
            player.setPosition(player.getPosX() + currentMap->getOffsetX(), player.getPosY() - currentMap->getMaxY());
            for (Enemy &v : cars)
                v.setPosition(v.getPosX(), v.getPosY() - currentMap->getMaxY());

            const bool isInitMap = currentMap->isInitMap();
            currentMap = currentMap->getNext();
            if (!isInitMap) {
                time = MAX_TIME; // Update time when map changes
                level++;
                // TODO: Actualizar esquema del nivel actual

                // Update fork maps
                if (currentMap == &maps[mapId.first + 1][mapId.second + 1])
                    mapId.second++;
                mapId.first++;
                cout << "Map: " << mapId.first << ", " << mapId.second << endl; // TODO: Borrar
                if (mapId.first < 4)
                    currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
                else {
                    goalMap.setColors(*currentMap);
                    currentMap->addNextMap(&goalMap);
                }
            }
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
            if (currentMap->inFork(v.getPosY()) || v.getPosY() + DEL_VEHICLE < currentMap->getCamY()) {
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
        action = Vehicle::CRASH;
        direction = Vehicle::RIGHT;

        if (!player.isCrashing()) { // If not has crashed
            action = player.accelerationControl(c, currentMap->hasGotOut(player.getPosX(), player.getPosY()));
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

State Game::pause(Config& c, Interface& i, const Vehicle::Action& a, const Vehicle::Direction &d) {
    // Start the pause menu of the game
    vector<Button> menuButtons;

    // Game in pause
    onPause = true;

    // Stop the music of the level
    c.themes[c.currentSoundtrack]->pause();

    RectangleShape shape;
    shape.setPosition(0 , 0);
    shape.setSize(Vector2f(c.w.getSize().x, c.w.getSize().y));
    shape.setFillColor(Color(0, 0, 0, 200));

    RectangleShape pauseShape;
    pauseShape.setPosition(c.w.getSize().x / 2.f - 120 , c.w.getSize().y / 2.f - 180);
    pauseShape.setSize(sf::Vector2f(250 , 350));
    pauseShape.setFillColor(Color(0, 0, 0));
    pauseShape.setOutlineColor(Color::Green);
    pauseShape.setOutlineThickness(5);

    Text textMenu;
    textMenu.setPosition(c.w.getSize().x / 2.f - 75 , c.w.getSize().y / 2.f - 150);
    textMenu.setFont(c.options);
    textMenu.setFillColor(Color(214, 234, 12));
    textMenu.setOutlineColor(Color(12, 72, 234));
    textMenu.setOutlineThickness(2);
    textMenu.setCharacterSize(35);
    textMenu.setString("PAUSE");

    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Buttons of the menu
    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 95, c.w.getSize().y / 2.f - 60, 200, 30, c.options,
                                 "Resume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 95, c.w.getSize().y / 2.f + 10, 200, 30, c.options,
                                 "Options", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    menuButtons.push_back(Button(c.w.getSize().x / 2.f - 95, c.w.getSize().y / 2.f + 80, 200, 30, c.options,
                                 "Quit", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0));

    while (!startPressed){

        // Detect the possible events
        Event e{};
        while (c.w.pollEvent(e)) {
            if (e.type == Event::Closed){
                c.w.close();
            }
        }

        // Check if the up or down cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuDownKey)){
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1)){
                // Change the color appearance of both buttons
                c.effects[0]->stop();
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
            }
        }
        else if (Keyboard::isKeyPressed(c.menuUpKey)){
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0){
                c.effects[0]->stop();
                c.effects[0]->play();
                optionSelected--;
                // Change the color appearance of both buttons
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
            }
        }
        else if (Keyboard::isKeyPressed(c.menuEnterKey)){
            startPressed = true;
        }

        // Draw the map
        currentMap->draw(c, cars);

        // Draw the vehicle of the player
        player.draw(c, a, d, currentMap->getElevation(player.getPosY()));

        c.w.draw(i.sText);
        c.w.draw(i.textLap);
        c.w.draw(i.textLevel);
        c.w.draw(i.textScore);
        c.w.draw(i.timeToPlay);

        for (Sprite s : i.sprites){
            c.w.draw(s);
        }

        c.w.draw(shape);
        c.w.draw(pauseShape);
        c.w.draw(textMenu);

        for (Button b : menuButtons){
            b.render(&c.w);
        }

        c.w.display();
        sleep(milliseconds(180));
    }

    c.effects[2]->stop();
    c.effects[2]->play();

    // Check the option selected by the user
    switch(optionSelected){
        case 0:
            // Resume button selected and reanudate the music
            c.themes[c.currentSoundtrack]->play();
            onPause = false;
            return GAME;
        case 1:
            // Options button selected
            return OPTIONS;
        case 2:
            // Quit button selected
            onPause = false;
            return START;
    }
}
