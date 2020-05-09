
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
#define MAX_COUNTER 10
#define MAX_VEHICLES 10 // Number of vehicles simultaneously
#define VEHICLE_DENSITY 3.0f // Greater than 0
#define VEHICLE_MIN_DISTANCE 5.0f // Minimum number of rectangles between enemies
#define DEL_VEHICLE 50.0f // Minimum number of rectangles behind the camera to delete the enemy


void comentaristIntro(Config& c){
     c.effects[11]->stop();
     c.effects[11]->play();
     this_thread::sleep_for(chrono::milliseconds(5000));
}

void slideCar(Config& c){
    c.effects[8]->stop();
    c.effects[8]->play();
}

void makeWomanSound(Config& c, int sound){
    c.effects[sound - 1]->stop();
    c.effects[sound - 1]->play();
    sleep(c.effects[sound - 1]->getDuration());
}


void makeCarTrafficSound(Config& c, int sound){
    c.effects[sound - 1]->stop();
    c.effects[sound - 1]->play();
    sleep(c.effects[sound - 1]->getDuration());
}

void makeBeepSound(Config& c){
    c.effects[22]->stop();
    c.effects[22]->play();
    sleep(c.effects[22]->getDuration());
}

void makeCheckPointEffect(Config& c){
    c.effects[23]->stop();
    c.effects[23]->play();
    sleep(c.effects[23]->getDuration());
}

void makeGoalSound(Config& c){
    c.effects[27]->stop();
    c.effects[27]->play();
    sleep(c.effects[27]->getDuration());
    c.effects[28]->stop();
    c.effects[28]->play();
    sleep(c.effects[28]->getDuration());
}

void claps(Config& c){
    c.effects[29]->stop();
    c.effects[29]->play();
    sleep(c.effects[29]->getDuration());
}

Game::Game(Config &c, Interface& interface) : player(MAX_SPEED, SPEED_MUL, ACC_INC, 1.25f, 0.9375f, MAX_COUNTER,
        "Ferrari", 0.0f, RECTANGLE), lastY(0), vehicleCrash(false), goalMap(goalFlagger, goalEnd) {
    int nm = 0;
    const int times[] = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
    const int nobjects[] = {20, 28, 40, 15, 25, 29, 26, 31, 33, 30, 30, 30, 34, 39, 33};
    for (int i = 0; i < 5; i++) {
        vector<Map> vm;
        for (int j = 0; j <= i; j++) {
            vector<string> objectNames;
            objectNames.reserve(nobjects[nm]);
            for (int no = 1; no <= nobjects[nm]; no++)
                objectNames.push_back(to_string(no) + ".png");

            Map m(c, "resources/map" + to_string(nm + 1) + "/", "bg.png", objectNames, false, times[nm]);
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
    const float vehicleScales[maxSprites] = {1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    for (int i = 0; i < MAX_VEHICLES; i++) {
        Enemy v(MAX_SPEED, SPEED_MUL, vehicleScales[i % maxSprites], MAX_COUNTER,
                "car" + to_string(1 + i % maxSprites), -RECTANGLE * DEL_VEHICLE * 3.0f);
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

    for (int i = 0; i < 8; i++){
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
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f + 160 , c.w.getSize().y / 2.f + 284);
                interface.sprites[i].scale(1.5f, 1.5f);
                break;
            case 5:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f - 170 , c.w.getSize().y / 2.f - 180);
                interface.sprites[i].scale(1.5f, 1.5f);
                break;
            case 6:
                interface.sprites[i].setPosition(c.w.getSize().x / 2.f - 420 , c.w.getSize().y / 2.f + 320);
                interface.sprites[i].scale(2.f, 1.5f);
                break;
            default:
                break;
        }
    }

    // Code of first Map
    int idFirstMap = 8;

    // Fill the matrix with the tree maps
    for (int i = 0; i <= 4; i++){
        for(int j = 0; j <= i; j++){
            t.loadFromFile("resources/GamePanel/" + to_string(idFirstMap) + ".png");
            interface.treeMap[i][j] = t;
            idFirstMap++;
        }
    }

    // Fill the matrix with the sprite maps
    for (int i = 0; i <= 4; i++){
        for(int j = 0; j <= i; j++){
            s.setTexture(interface.treeMap[i][j], true);
            interface.spriteMap[i][j] = s;
        }
    }

    interface.recordLap.setTexture(interface.textures[2], true);
    interface.recordLap.setPosition(c.w.getSize().x / 2.f - 50 , c.w.getSize().y / 2.f - 180);
    interface.recordLap.setScale(1.5f, 1.5f);

    interface.sprites[7] = interface.spriteMap[0][0];
    interface.sprites[7].setPosition(c.w.getSize().x / 2.f + 370 , c.w.getSize().y / 2.f + 270);
    interface.sprites[7].scale(2.f, 2.f);

    // Text
    interface.sText.setFillColor(Color(206, 73, 73));
    interface.sText.setOutlineColor(Color::Black);
    interface.sText.setOutlineThickness(3);
    interface.sText.setCharacterSize(70);
    interface.sText.setFont(c.speedVehicle);

    // Initialize the HUD indicator of time
    time = currentMap->getTime();
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

    // CheckPoint indicator
    interface.checkPoint.setFont(c.timeToPlay);
    interface.checkPoint.setPosition(c.w.getSize().x / 2.f - 120, c.w.getSize().y / 2.f - 235);
    interface.checkPoint.setString("CHECKPOINT!");
    interface.checkPoint.setCharacterSize(50);
    interface.checkPoint.setFillColor(Color::Yellow);
    interface.checkPoint.setOutlineColor(Color(12, 12, 12));
    interface.checkPoint.setOutlineThickness(3);

    // Bonification indicator
    interface.bonification.setFont(c.timeToPlay);
    interface.bonification.setPosition(c.w.getSize().x / 2.f - 190, c.w.getSize().y / 2.f - 235);
    interface.bonification.setString("BONUS POINTS!");
    interface.bonification.setCharacterSize(65);
    interface.bonification.setFillColor(Color::Yellow);
    interface.bonification.setOutlineColor(Color(12, 12, 12));
    interface.bonification.setOutlineThickness(3);

    // Seconds arrival indicator
    interface.secondsIndicator.setFont(c.timeToPlay);
    interface.secondsIndicator.setPosition(c.w.getSize().x / 2.f - 160, c.w.getSize().y / 2.f - 130);
    interface.secondsIndicator.setString("SEC");
    interface.secondsIndicator.setCharacterSize(50);
    interface.secondsIndicator.setFillColor(Color(183, 164, 190));
    interface.secondsIndicator.setOutlineColor(Color::Black);
    interface.secondsIndicator.setOutlineThickness(3);

    // Cross indicator
    interface.crossSign.setFont(c.timeToPlay);
    interface.crossSign.setPosition(c.w.getSize().x / 2.f - 50, c.w.getSize().y / 2.f - 120);
    interface.crossSign.setString("x");
    interface.crossSign.setCharacterSize(40);
    interface.crossSign.setFillColor(Color(232, 191, 157));
    interface.crossSign.setOutlineColor(Color::Black);
    interface.crossSign.setOutlineThickness(3);

    // factor score multiplicator
    interface.scoreMultiply.setFont(c.timeToPlay);
    interface.scoreMultiply.setPosition(c.w.getSize().x / 2.f - 10, c.w.getSize().y / 2.f - 135);
    interface.scoreMultiply.setString("1000000");
    interface.scoreMultiply.setCharacterSize(55);
    interface.scoreMultiply.setFillColor(Color::Yellow);
    interface.scoreMultiply.setOutlineColor(Color(12, 12, 12));
    interface.scoreMultiply.setOutlineThickness(3);

    // Seconds arrival indicator
    interface.pointsIndicator.setFont(c.timeToPlay);
    interface.pointsIndicator.setPosition(c.w.getSize().x / 2.f + 200, c.w.getSize().y / 2.f - 130);
    interface.pointsIndicator.setString("PTS");
    interface.pointsIndicator.setCharacterSize(50);
    interface.pointsIndicator.setFillColor(Color(183, 164, 190));
    interface.pointsIndicator.setOutlineColor(Color::Black);
    interface.pointsIndicator.setOutlineThickness(3);

    // Seconds arrival indicator
    interface.timeBonus.setFont(c.timeToPlay);
    interface.timeBonus.setCharacterSize(55);
    interface.timeBonus.setFillColor(Color::Yellow);
    interface.timeBonus.setOutlineColor(Color(12, 12, 12));
    interface.timeBonus.setOutlineThickness(3);

    interface.textForLap.setFont(c.timeToPlay);
    interface.textForLap.setPosition(c.w.getSize().x / 2.f - 80, c.w.getSize().y / 2.f - 140);
    interface.textForLap.setCharacterSize(35);
    interface.textForLap.setOutlineThickness(3);


    // Control if the player is still playing
    finalGame = false;
    inGame = false;
    onPause = false;
    comeFromOptions = false;
    blink = false;
    woman_delay = seconds(5.0f);
    traffic_delay = seconds(2.f);
    blink_delay = seconds(0.5f);
    bonus_delay = seconds(0.01f);
}


bool Game::isInGame() const {
    return inGame;
}



State Game::play(Config &c, Interface& interface) {

    if (!inGame) {
        inGame = true;
        initialAnimation(c);
    }

    c.w.setKeyRepeatEnabled(false);

    // Time to update the clock counter
    Time shot_delayTime = seconds(1.0);

    // Time to update the clock counter lap
    Time shot_delayLap = seconds(0.01);

    gameClockTime.restart().asSeconds();
    elapsed1 = gameClockTime.restart().asSeconds();

    gameClockLap.restart();
    elapsed3 = gameClockLap.getElapsedTime().asSeconds();

    womanShot.restart();
    elapsed5 = womanShot.getElapsedTime().asSeconds();

    trafficCarSound.restart();
    elapsed7 = trafficCarSound.getElapsedTime().asSeconds();

    blinkTime.restart();
    elapsed9 = blinkTime.getElapsedTime().asSeconds();

    Vehicle::Action action;
    Vehicle::Direction direction;

    State status;

    while (!finalGame && c.w.isOpen()) {
        updateAndDraw(c, interface, action, direction);

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
                    comeFromOptions = true;
                    finalGame = true;
                }
                else if (status == START){
                    finalGame = true;
                    inGame = false;
                    return START;
                }
                else if (status == GAME){
                    c.themes[c.currentSoundtrack]->play();
                }
            }

            // Draw speed
            string strSpeed = to_string(player.getRealSpeed());
            interface.sText.setString(strSpeed.substr(0, strSpeed.find('.')));
            interface.sText.setPosition((float) (c.w.getSize().x / 2.f) - 310 - interface.sText.getLocalBounds().width,
                              (float) c.w.getSize().y / 2.f + 240);

            interface.textures[6].loadFromFile("resources/GamePanel/7.png",
                                     IntRect(0, 0, (int(player.getRealSpeed() * 117.0f / MAX_SPEED)), 20));
            interface.sprites[6].setTexture(interface.textures[6], true);

            c.w.draw(interface.sText);

            // Update the score of the player if the player is not stopped
            if (player.getRealSpeed() > 0.0f) {
                // Add score
                score += int(player.getRealSpeed());
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
                cents_second++;
                gameClockLap.restart();
                if (cents_second == 100) {
                    cents_second = 0;
                    secs++;
                    if (secs == 60) {
                        secs = 0;
                        minutes++;
                    }
                }
            }

            // Update the indicators
            string lap;
            lap += (minutes < 10) ? "0" + to_string(minutes) + " '" : to_string(minutes) + " ''";
            lap += (secs < 10) ? "0" + to_string(secs) + " ''" : to_string(secs) + " ''";
            lap += (cents_second < 10) ? "0" + to_string(cents_second) : to_string(cents_second);

            interface.timeToPlay.setString(to_string(time));
            interface.textScore.setString(to_string(score));
            interface.textLap.setString(lap);
            interface.textLevel.setString(to_string(level));

            // Draw the panel indicators
            for (int i = 0; i < 5; i++) {
                c.w.draw(interface.sprites[i]);
            }

            if (player.getRealSpeed() > 0) {
                c.w.draw(interface.sprites[6]);
            }

            c.w.draw(interface.sprites[7]);
            c.w.draw(interface.timeToPlay);
            c.w.draw(interface.textScore);
            c.w.draw(interface.textLap);
            c.w.draw(interface.textLevel);
            c.w.display();

            if (time == 10){
                c.effects[25]->stop();
                c.effects[25]->play();
            }

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
        c.themes[c.currentSoundtrack]->stop();
        c.effects[24]->stop();
        c.effects[24]->play();
        c.w.display();

        bool startPressed = false;
        while (!startPressed)
            startPressed = Keyboard::isKeyPressed(c.menuEnterKey);

        return EXIT;
    }
    return status;
}

void Game::initialAnimation(Config &c) {

    c.themes[c.currentSoundtrack]->stop();
    int flagger, semaphore;
    Map *initMap = new Map(*currentMap, flagger, semaphore);
    initMap->addNextMap(currentMap);
    currentMap = initMap;

    // Prepare car
    bool end = false;

    sleep(milliseconds(70));
    thread slide(slideCar, ref(c));

    for (int i = (int) c.w.getSize().x / 2; !end; i -= 3) {
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.drawInitialAnimation(c, float(i), end);
        c.w.display();
    }
    sleep(milliseconds(250));
    slide.join();

    // Semaphore and flagger
    currentMap->incrementSpriteIndex(flagger, false, -1);
    int ms = 1000;

    thread comentarist(comentaristIntro, ref(c));
    currentMap->draw(c, cars);
    player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
    c.w.display();
    comentarist.join();

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
        if (ms > 0) {
            sleep(milliseconds(ms));
            c.effects[4]->stop();
            c.effects[4]->play();
        }
        currentMap->incrementSpriteIndex(semaphore, false);
        c.effects[5]->stop();
        c.effects[5]->play();
    }
    currentMap->incrementSpriteIndex(flagger, false, -1);

    c.themes[c.currentSoundtrack]->play();
}

void Game::goalAnimation(Config &c, Interface& interface) {
    // Stop music level
    c.themes[c.currentSoundtrack]->stop();

    thread (makeGoalSound, ref(c)).detach();

    // Hide enemies
    for (Enemy &v : cars)
        v.setPosition(v.getPosX(), -RECTANGLE);

    player.setSmoking(false);
    int increment = 0;
    float currentTime = gameClockTime.getElapsedTime().asMilliseconds();

    // Bonus seconds
    int decsTime = time * 10;
    int seconds = decsTime / 10;
    int decs_second = decsTime % 10;

    bonus.restart();
    elapsed11 = bonus.getElapsedTime().asSeconds(); // TODO: Esta marca solo se actualiza aquí ?????

    // Draw the time bonus
    interface.timeBonus.setString(to_string(seconds) + "." + to_string(decs_second));
    interface.timeBonus.setPosition((float) (c.w.getSize().x / 2.f) - 170 - interface.timeBonus.getLocalBounds().width,
                                    (float) c.w.getSize().y / 2.f - 135);

    while (int(player.getPosY()) < goalEnd) {
        // Update camera
        currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.setPosition(player.getPosX(), player.getPosY() + 1);
        player.draw(c, Vehicle::Action::ACCELERATE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()), false);

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

        elapsed12 = bonus.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed12 - elapsed11 >= bonus_delay.asSeconds()) {
            // Decrement one Tenth of a second
            decsTime--;

            seconds = decsTime / 10;
            // Decs per second
            decs_second = decsTime % 10;

            score += SCORE_BONIFICATION / 10; // Bonif. per dec.
            interface.textScore.setString(to_string(score));

            // Draw the time bonus
            interface.timeBonus.setString(to_string(seconds) + "." + to_string(decs_second));
            interface.timeBonus.setPosition((float) (c.w.getSize().x / 2.f) - 170 - interface.timeBonus.getLocalBounds().width,
                                            (float) c.w.getSize().y / 2.f - 135);

            bonus.restart();
        }

        // Draw the bonus indicators
        c.w.draw(interface.timeBonus);
        c.w.draw(interface.bonification);
        c.w.draw(interface.secondsIndicator);
        c.w.draw(interface.crossSign);
        c.w.draw(interface.scoreMultiply);
        c.w.draw(interface.pointsIndicator);
        c.w.draw(interface.timeBonus);

        // Draw speed
        string strSpeed = to_string(player.getRealSpeed());
        interface.sText.setString(strSpeed.substr(0, strSpeed.find('.')));
        interface.sText.setPosition((float) (c.w.getSize().x / 2.f) - 310 - interface.sText.getLocalBounds().width,
                          (float) c.w.getSize().y / 2.f + 240);

        // Draw the panel indicators
        for (int i = 0; i < 5; i++) {
            c.w.draw(interface.sprites[i]);
        }

        if (player.getRealSpeed() > 0) {
            c.w.draw(interface.sprites[6]);
        }

        c.w.draw(interface.sprites[7]);
        c.w.draw(interface.timeToPlay);
        c.w.draw(interface.textScore);
        c.w.draw(interface.textLap);
        c.w.draw(interface.textLevel);

        c.w.draw(interface.sText);
        c.w.display();
    }

    // Car animation
    int step = 0, lastStep;
    bool firstEnd = false, end = false;

    thread(slideCar, ref(c)).detach();
    thread(claps, ref(c)).detach();

    bonus.restart();

    while (!end) {
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);

        if (!firstEnd) {
            player.drawGoalAnimation(c, step, end);
        }
        else {
            player.drawGoalAnimation(c, lastStep, end, false);
        }

        if (end && !firstEnd){
            firstEnd = true;
        }
        if (firstEnd){
            lastStep = step - 1;
        }

        if (decsTime > 0){
            elapsed12 = bonus.getElapsedTime().asSeconds();

            // Check if a second has passed between both timestamps
            if (elapsed12 - elapsed11 >= bonus_delay.asSeconds()) {
                // Decrement one Tenth of a second
                decsTime--;

                seconds = decsTime / 10;
                // Decs per second
                decs_second = decsTime % 10;

                score += SCORE_BONIFICATION / 10; // Bonif. per dec.
                interface.textScore.setString(to_string(score));

                // Draw the time bonus
                interface.timeBonus.setString(to_string(seconds) + "." + to_string(decs_second));
                interface.timeBonus.setPosition((float) (c.w.getSize().x / 2.f) - 170 - interface.timeBonus.getLocalBounds().width,
                                                (float) c.w.getSize().y / 2.f - 135);

                bonus.restart();
            }

            // Draw the bonus indicators
            c.w.draw(interface.timeBonus);
            c.w.draw(interface.bonification);
            c.w.draw(interface.secondsIndicator);
            c.w.draw(interface.crossSign);
            c.w.draw(interface.scoreMultiply);
            c.w.draw(interface.pointsIndicator);
            c.w.draw(interface.timeBonus);

            // Draw speed
            string strSpeed = to_string(player.getRealSpeed());
            interface.sText.setString(strSpeed.substr(0, strSpeed.find('.')));
            interface.sText.setPosition((float) (c.w.getSize().x / 2.f) - 310 - interface.sText.getLocalBounds().width,
                              (float) c.w.getSize().y / 2.f + 240);

            // Draw the panel indicators
            for (int i = 0; i < 5; i++) {
                c.w.draw(interface.sprites[i]);
            }

            if (player.getRealSpeed() > 0) {
                c.w.draw(interface.sprites[6]);
            }

            c.w.draw(interface.sprites[7]);
            c.w.draw(interface.timeToPlay);
            c.w.draw(interface.textScore);
            c.w.draw(interface.textLap);
            c.w.draw(interface.textLevel);

            c.w.draw(interface.sText);
            end = false;
        }
        c.w.display();
    }
}


void Game::updateAndDraw(Config &c, Interface& interface, Vehicle::Action& action, Vehicle::Direction &direction) {
    // Update camera
    currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

    if (currentMap->isOver()) {
        if (currentMap->getNext() != nullptr) {
            // Update player and vehicle positions
            player.setPosition(player.getPosX() + currentMap->getOffsetX(), player.getPosY() - currentMap->getMaxY());
            for (Enemy &v : cars)
                v.setPosition(v.getPosX(), v.getPosY() - currentMap->getMaxY());

            const bool isInitMap = currentMap->isInitMap();
            currentMap = currentMap->getNext();
            if (!isInitMap && !currentMap->isGoalMap()) {
                level++;
                // Update fork maps
                if (currentMap == &maps[mapId.first + 1][mapId.second + 1])
                    mapId.second++;
                mapId.first++;

                interface.sprites[7] = interface.spriteMap[mapId.first][mapId.second];
                interface.sprites[7].setPosition(c.w.getSize().x / 2.f + 370 , c.w.getSize().y / 2.f + 270);
                interface.sprites[7].scale(2.f, 2.f);

                if (mapId.first < 4)
                    currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
                else {
                    goalMap.setColors(*currentMap);
                    currentMap->addNextMap(&goalMap);
                }
                // Update time when map changes
                time += currentMap->getTime();

                // Update the indicators
                if (!checkPoint){
                    string lap;
                    lap += (minutes < 10) ? "0" + to_string(minutes) + " '" : to_string(minutes) + " ''";
                    lap += (secs < 10) ? "0" + to_string(secs) + " ''" : to_string(secs) + " ''";
                    lap += (cents_second < 10) ? "0" + to_string(cents_second) : to_string(cents_second);
                    interface.textForLap.setString(lap);

                    // Initialize to zero the time
                    cents_second = 0;
                    secs = 0;
                    minutes= 0;
                }
                checkPoint = true;
                thread(makeCheckPointEffect, ref(c)).detach();
            }
            currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

            lastY = currentMap->getCamY();
        }
        else {
            finalGame = true;
        }
    }

    if (currentMap->isGoalMap()) {
        goalAnimation(c, interface);
        finalGame = true;
    }

    if (!finalGame) {
        // Update and prepare cars to draw
        if (lastY <= currentMap->getCamY() + float(c.renderLen))
            lastY = currentMap->getCamY() + float(c.renderLen);
        for (Enemy &v : cars) {
            if (currentMap->inFork(v.getPosY())) {
                v.setPosition(v.getPosX(), -RECTANGLE * DEL_VEHICLE * 3.0f);
            }
            else if (v.getPosY() + DEL_VEHICLE < currentMap->getCamY()) {
                v.update(lastY, lastY + float(c.renderLen) / VEHICLE_DENSITY);
                lastY = v.getPosY() + VEHICLE_MIN_DISTANCE * RECTANGLE;
            }

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
                                                player.getMinScreenX(), player.getMaxScreenX(),
                                                crashPos);

            if (crash || vehicleCrash) {
                player.setPosition(player.getPosX(), crashPos);
                player.hitControl(vehicleCrash);
                action = Vehicle::CRASH;
                direction = Vehicle::RIGHT;

                player.draw(c, action, direction, currentMap->getElevation(player.getPosY()));
            }
        }

        for (Enemy &v : cars)
            v.autoControl(c, player.getPosX(), player.getPosY());

        // Check if enemies are displayed on the screen
        for (Enemy &v : cars) {
            float distX, distY;
            bool visible = v.isVisible(c, currentMap->getCamY(), player.getPosX(), player.getPosY(), distX, distY);
            if (visible) {
                if (distY <= 20.f && distX <= 0.3f){
                    // Thread with sound of the woman
                    elapsed6 = womanShot.getElapsedTime().asSeconds();
                    if (elapsed6 - elapsed5 >= woman_delay.asSeconds()){
                        int code = random_int(14, 16);
                        thread (makeWomanSound, ref(c), code).detach();
                        womanShot.restart();
                    }
                }
                if (distY <= 30.f && distX <= 1.2f){
                    // Thread with sound of the woman
                    elapsed8 = trafficCarSound.getElapsedTime().asSeconds();
                    if (elapsed8 - elapsed7 >= traffic_delay.asSeconds()){
                        int code = random_int(21, 22);
                        thread (makeCarTrafficSound, ref(c), code).detach();
                        trafficCarSound.restart();
                    }
                }
            }
        }
        if (checkPoint){
             elapsed10 = blinkTime.getElapsedTime().asSeconds();
             if (elapsed10 - elapsed9 >= blink_delay.asSeconds()){
                blink = !blink;
                blinkTime.restart();
             }
             if (blink){
                interface.checkPoint.setFillColor(Color::Yellow);
                interface.checkPoint.setOutlineColor(Color(14, 29, 184));
                interface.textForLap.setFillColor(Color(146, 194, 186));
                interface.textForLap.setOutlineColor(Color::Black);
                c.w.draw(interface.sprites[6]);
                c.w.draw(interface.recordLap);
                thread(makeBeepSound, ref(c)).detach();
             }
             else {
                interface.checkPoint.setFillColor(Color::Transparent);
                interface.checkPoint.setOutlineColor(Color::Transparent);
                interface.textForLap.setFillColor(Color::Transparent);
                interface.textForLap.setOutlineColor(Color::Transparent);
             }
             c.w.draw(interface.textForLap);
             c.w.draw(interface.checkPoint);

             if (currentMap->getTime() - time > 5){
                checkPoint = false;
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
    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95, c.w.getSize().y / 2.f - 60, 200, 30, c.options,
                                 "Resume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95, c.w.getSize().y / 2.f + 10, 200, 30, c.options,
                                 "Options", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95, c.w.getSize().y / 2.f + 80, 200, 30, c.options,
                                 "Quit", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0);


    // Draw the map
    currentMap->draw(c, cars);

    // Draw the vehicle of the player
    player.draw(c, a, d, currentMap->getElevation(player.getPosY()), false);

    c.w.draw(i.sText);
    c.w.draw(i.textLap);
    c.w.draw(i.textLevel);
    c.w.draw(i.textScore);
    c.w.draw(i.timeToPlay);

    for (const Sprite& s : i.sprites){
        c.w.draw(s);
    }

    c.w.draw(shape);

    while (!startPressed) {
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
            if (comeFromOptions){
                comeFromOptions = false;
            }
            else {
                startPressed = true;
            }
        }

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
            onPause = false;
            return GAME;
        case 1:
            // Options button selected
            return OPTIONS;
        default:
            // Quit button selected
            onPause = false;
            return START;
    }
}
