
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
#define SPEED_MUL 90.0f
#define MAX_COUNTER 10
#define VEHICLE_DENSITY 3.0f // Greater than 0
#define VEHICLE_MIN_DISTANCE 5.0f // Minimum number of rectangles between enemies
#define DEL_VEHICLE 50.0f // Minimum number of rectangles behind the camera to delete the enemy

Game::Game(Config &c, Interface& interface) : player(MAX_SPEED, SPEED_MUL, ACC_INC, 1.25f, 0.9375f, MAX_COUNTER,
        "Ferrari", 0.0f, RECTANGLE), lastY(0), vehicleCrash(false), timeMul(1.0f), scoreMul(1.0f),
        goalMap(goalFlagger, goalEnd) {
    int nm = 0;
    const int times[] = {85, 58, 68, 50, 75, 69, 53, 54, 49, 48, 46, 42, 42, 41, 42};
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

    checkDifficulty(c); // Loads enemies and time

    Texture t;
    // Load the textures of the panel and assign them to their sprites
    for (int i = 1; i <= 6; i++){
        // Load the texture from the file
        t.loadFromFile("resources/GamePanel/" + to_string(i) + ".png");
        interface.textures.push_back(t);
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

    time = int(float(currentMap->getTime()) * timeMul);
    score = 0;
    level = 1;

    // Control if the player is still playing
    finalGame = false;
    inGame = false;
    onPause = false;
    comeFromOptions = false;
    blink = false;
    arrival = false;
    woman_delay = seconds(5.0f);
    traffic_delay = seconds(2.f);
    blink_delay = seconds(0.5f);
    bonus_delay = seconds(0.01f);

    Sprite s;
    for (int i = 0; i < 8; i++){
        s.setTexture(interface.textures[i], true);
        interface.sprites.push_back(s);
    }

    // Fill the matrix with the sprite maps
    for (int i = 0; i <= 4; i++){
        for(int j = 0; j <= i; j++){
            s.setTexture(interface.treeMap[i][j], true);
            interface.spriteMap[i][j] = s;
        }
    }
    prepareScreen(c, interface);
}

void Game::prepareScreen(Config &c, Interface& interface) {
    // Assign positions in the game console for the game panel indicators
    const float up = float(c.w.getSize().y) / 10.0f;

    // UP
    interface.sprites[0].scale(1.5f * c.screenScale, 1.5f * c.screenScale);
    const float separation = interface.sprites[0].getGlobalBounds().width / 3.0f;
    interface.sprites[0].setPosition(separation, up - interface.sprites[0].getGlobalBounds().height);
    float initial = separation + interface.sprites[0].getGlobalBounds().width + separation;

    // Initialize the HUD indicator of time
    interface.timeToPlay.setString("000");
    interface.timeToPlay.setFont(c.timeToPlay);
    interface.timeToPlay.setCharacterSize(int(50.0f * c.screenScale));
    interface.timeToPlay.setFillColor(Color::Yellow);
    interface.timeToPlay.setOutlineColor(Color::Black);
    interface.timeToPlay.setOutlineThickness(3.0f * c.screenScale);
    interface.timeToPlay.setPosition(initial, up - float(interface.timeToPlay.getCharacterSize()));
    initial += interface.timeToPlay.getGlobalBounds().width + separation;

    interface.sprites[1].scale(1.5f * c.screenScale, 1.5f * c.screenScale);
    interface.sprites[1].setPosition(initial, up - interface.sprites[1].getGlobalBounds().height);
    initial += interface.sprites[1].getGlobalBounds().width + separation;

    // Initialize the HUD indicator of score
    interface.textScore.setString(to_string(int(SCORE_BONIFICATION) * 100));
    interface.textScore.setFont(c.timeToPlay);
    interface.textScore.setCharacterSize(int(35.0f * c.screenScale));
    interface.textScore.setFillColor(Color(183, 164, 190));
    interface.textScore.setOutlineColor(Color::Black);
    interface.textScore.setOutlineThickness(3.0f * c.screenScale);
    interface.textScore.setPosition(initial, up - float(interface.textScore.getCharacterSize()));

    // Initialize the HUD indicator of lap time
    interface.textLap.setFont(c.timeToPlay);
    interface.textLap.setString("00' 00'' 00");
    interface.textLap.setCharacterSize(int(35.0f * c.screenScale));
    interface.textLap.setFillColor(Color(146, 194, 186));
    interface.textLap.setOutlineColor(Color::Black);
    interface.textLap.setOutlineThickness(3.0f * c.screenScale);
    initial = float(c.w.getSize().x) - separation - interface.textLap.getGlobalBounds().width;
    interface.textLap.setPosition(initial, up - float(interface.textLap.getCharacterSize()));

    interface.sprites[2].scale(1.5f * c.screenScale, 1.5f * c.screenScale);
    initial -= separation + interface.sprites[2].getGlobalBounds().width;
    interface.sprites[2].setPosition(initial, up - interface.sprites[2].getGlobalBounds().height);

    // DOWN
    interface.sprites[6].scale(2.f * c.screenScale, 1.5f * c.screenScale);
    const float down = float(c.w.getSize().y) - interface.sprites[6].getGlobalBounds().height * 1.5f;
    interface.sprites[6].setPosition(separation, float(c.w.getSize().y) - interface.sprites[6].getGlobalBounds().height * 1.25f);
    initial = separation + interface.sprites[6].getGlobalBounds().width / 2.0f;

    // Text
    interface.sText.setFont(c.speedVehicle);
    interface.sText.setString("000");
    interface.sText.setCharacterSize(int(70.0f * c.screenScale));
    interface.sText.setFillColor(Color(206, 73, 73));
    interface.sText.setOutlineColor(Color::Black);
    interface.sText.setOutlineThickness(3.0f * c.screenScale);
    interface.sText.setPosition(initial, down - float(interface.sText.getCharacterSize()));
    initial += interface.sText.getGlobalBounds().width;

    interface.sprites[3].scale(2.f * c.screenScale, 2.f * c.screenScale);
    interface.sprites[3].setPosition(initial, down - interface.sprites[3].getGlobalBounds().height);

    interface.sprites[7] = interface.spriteMap[0][0];
    interface.sprites[7].scale(2.f * c.screenScale, 2.f * c.screenScale);
    initial = float(c.w.getSize().x) - separation - interface.sprites[7].getGlobalBounds().width;
    interface.sprites[7].setPosition(initial, down - interface.sprites[7].getGlobalBounds().height);

    // Initialize the HUD stage indicator
    interface.textLevel.setFont(c.timeToPlay);
    interface.textLevel.setString("0");
    interface.textLevel.setCharacterSize(int(40.0f * c.screenScale));
    interface.textLevel.setFillColor(Color(146, 194, 186));
    interface.textLevel.setOutlineColor(Color::Black);
    interface.textLevel.setOutlineThickness(3.0f * c.screenScale);
    initial -= separation + interface.textLevel.getGlobalBounds().width;
    interface.textLevel.setPosition(initial, down - float(interface.textLevel.getCharacterSize()));

    interface.sprites[4].scale(1.5f * c.screenScale, 1.5f * c.screenScale);
    initial -= separation + interface.sprites[4].getGlobalBounds().width;
    interface.sprites[4].setPosition(initial, down - interface.sprites[4].getGlobalBounds().height);

    // Checkpoint
    interface.checkPoint.setFont(c.timeToPlay);
    interface.checkPoint.setString("CHECKPOINT!");
    interface.checkPoint.setCharacterSize(int(50.0f * c.screenScale));
    interface.checkPoint.setFillColor(Color::Yellow);
    interface.checkPoint.setOutlineColor(Color(12, 12, 12));
    interface.checkPoint.setOutlineThickness(3.0f * c.screenScale);
    initial = c.w.getSize().y / 3.0f + 0.25f * float(interface.checkPoint.getCharacterSize());
    interface.checkPoint.setPosition((float(c.w.getSize().x) - interface.checkPoint.getGlobalBounds().width) / 2.0f, c.w.getSize().y / 3.0f - float(interface.checkPoint.getCharacterSize()));

    interface.sprites[5].scale(1.5f * c.screenScale, 1.5f * c.screenScale);
    interface.sprites[5].setPosition((float(c.w.getSize().x) - interface.sprites[5].getGlobalBounds().width) / 2.0f, initial);
    initial += interface.sprites[5].getGlobalBounds().height * 1.25f;

    interface.textForLap.setFont(c.timeToPlay);
    interface.textForLap.setString("00' 00'' 00");
    interface.textForLap.setCharacterSize(int(35.0f * c.screenScale));
    interface.textForLap.setOutlineThickness(3.0f * c.screenScale);
    interface.textForLap.setPosition((float(c.w.getSize().x) - interface.textForLap.getGlobalBounds().width) / 2.0f, initial);

    interface.recordLap.setTexture(interface.textures[2], true);
    interface.recordLap.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    interface.recordLap.setPosition((float(c.w.getSize().x) + 1.25f * interface.textForLap.getGlobalBounds().width) / 2.0f, initial);

    // Game over indicator
    interface.gameOver.setFont(c.timeToPlay);
    interface.gameOver.setString("GAME OVER");
    interface.gameOver.setCharacterSize(int(60.0f * c.screenScale));
    interface.gameOver.setFillColor(Color::Yellow);
    interface.gameOver.setOutlineColor(Color(14, 29, 184));
    interface.gameOver.setOutlineThickness(3.0f * c.screenScale);
    interface.gameOver.setPosition((float(c.w.getSize().x) - interface.gameOver.getGlobalBounds().width) / 2.0f, (float(c.w.getSize().y) - float(interface.gameOver.getCharacterSize())) / 2.0f);

    // Bonification indicator
    interface.bonification.setFont(c.timeToPlay);
    interface.bonification.setString("BONUS POINTS!");
    interface.bonification.setCharacterSize(int(65.0f * c.screenScale));
    interface.bonification.setFillColor(Color::Yellow);
    interface.bonification.setOutlineColor(Color(12, 12, 12));
    interface.bonification.setOutlineThickness(3.0f * c.screenScale);
    float initialY = float(c.w.getSize().y) / 3.0f + float(interface.bonification.getCharacterSize());
    initial = (float(c.w.getSize().x) - interface.bonification.getGlobalBounds().width) / 2.0f;
    interface.bonification.setPosition(initial, float(c.w.getSize().y) / 3.0f - float(interface.bonification.getCharacterSize()));

    // Seconds arrival indicator
    interface.timeBonus.setFont(c.timeToPlay);
    interface.timeBonus.setString("000.0");
    interface.timeBonus.setCharacterSize(int(55.0f * c.screenScale));
    interface.timeBonus.setFillColor(Color::Yellow);
    interface.timeBonus.setOutlineColor(Color(12, 12, 12));
    interface.timeBonus.setOutlineThickness(3.0f * c.screenScale);
    initial -= interface.timeBonus.getLocalBounds().width;
    interface.timeBonus.setPosition(initial, initialY);
    initialY += float(interface.timeBonus.getCharacterSize());
    initial += 1.25f * interface.timeBonus.getLocalBounds().width;

    // Seconds arrival indicator
    interface.secondsIndicator.setFont(c.timeToPlay);
    interface.secondsIndicator.setString("SEC");
    interface.secondsIndicator.setCharacterSize(int(50.0f * c.screenScale));
    interface.secondsIndicator.setFillColor(Color(183, 164, 190));
    interface.secondsIndicator.setOutlineColor(Color::Black);
    interface.secondsIndicator.setOutlineThickness(3.0f * c.screenScale);
    interface.secondsIndicator.setPosition(initial, initialY - float(interface.secondsIndicator.getCharacterSize()));
    initial += separation + interface.secondsIndicator.getLocalBounds().width;

    // Cross indicator
    interface.crossSign.setFont(c.timeToPlay);
    interface.crossSign.setString("x");
    interface.crossSign.setCharacterSize(int(40.0f * c.screenScale));
    interface.crossSign.setFillColor(Color(232, 191, 157));
    interface.crossSign.setOutlineColor(Color::Black);
    interface.crossSign.setOutlineThickness(3.0f * c.screenScale);
    interface.crossSign.setPosition(initial, initialY - float(interface.crossSign.getCharacterSize()));
    initial += separation + interface.crossSign.getLocalBounds().width;

    // factor score multiplicator
    interface.scoreMultiply.setFont(c.timeToPlay);
    interface.scoreMultiply.setString(to_string((long long) SCORE_BONIFICATION));
    interface.scoreMultiply.setCharacterSize(int(55.0f * c.screenScale));
    interface.scoreMultiply.setFillColor(Color::Yellow);
    interface.scoreMultiply.setOutlineColor(Color(12, 12, 12));
    interface.scoreMultiply.setOutlineThickness(3.0f * c.screenScale);
    interface.scoreMultiply.setPosition(initial, initialY - float(interface.scoreMultiply.getCharacterSize()));
    initial += separation + interface.scoreMultiply.getLocalBounds().width;

    // Seconds arrival indicator
    interface.pointsIndicator.setFont(c.timeToPlay);
    interface.pointsIndicator.setString("PTS");
    interface.pointsIndicator.setCharacterSize(int(50.0f * c.screenScale));
    interface.pointsIndicator.setFillColor(Color(183, 164, 190));
    interface.pointsIndicator.setOutlineColor(Color::Black);
    interface.pointsIndicator.setOutlineThickness(3.0f * c.screenScale);
    interface.pointsIndicator.setPosition(initial, initialY - float(interface.pointsIndicator.getCharacterSize()));
}

void Game::checkDifficulty(Config &c) {
    int numCars = cars.size(); // Number of vehicles simultaneously
    time = int(float(time) / timeMul); // Restore original time

    float prevScoreMul = scoreMul;
    switch (c.level) {
        case PEACEFUL:
            numCars = 0;
            timeMul = 1.1f;
            scoreMul = 0.25f;
            break;
        case EASY:
            numCars = 5;
            timeMul = 1.1f;
            scoreMul = 0.5f;
            break;
        case NORMAL:
            numCars = 10;
            timeMul = 1.0f;
            scoreMul = 1.0f;
            break;
        case HARD:
            numCars = 15;
            timeMul = 0.9f;
            scoreMul = 1.5f;
            break;
        default:
            break;
    }
    if (prevScoreMul < scoreMul) // Keep the least multiplier
        scoreMul = prevScoreMul;

    // Vehicles
    cars.reserve(numCars);
    if (cars.size() > numCars) {
        while (cars.size() > numCars)
            cars.pop_back();
    }
    else if (cars.size() < numCars) {
        const int maxSprites = 6;
        const float vehicleScales[maxSprites] = {1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
        for (int i = cars.size(); i < numCars; i++) {
            Enemy v(MAX_SPEED, SPEED_MUL, vehicleScales[i % maxSprites], MAX_COUNTER,
                    "car" + to_string(1 + i % maxSprites), -RECTANGLE * DEL_VEHICLE * 3.0f);
            cars.push_back(v);
        }
    }

    time = int(float(time) * timeMul);
}

bool Game::isInGame() const {
    return inGame;
}

unsigned long Game::getScore() const {
    return score;
}

int Game::getMinutesTrip() const {
    return minutesTrip;
}

int Game::getSecsTrip() const {
    return secsTrip;
}

int Game::getCents_SecondTrip() const{
    return cents_secondTrip;
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

    while (!finalGame && !arrival && c.w.isOpen()) {
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

            interface.textures[6].loadFromFile("resources/GamePanel/7.png",
                                     IntRect(0, 0, (player.getRealSpeed() * 117.0f / MAX_SPEED * c.screenScale), 20.0f * c.screenScale));
            interface.sprites[6].setTexture(interface.textures[6], true);

            c.w.draw(interface.sText);

            // Update the score of the player if the player is not stopped
            if (player.getRealSpeed() > 0.0f) {
                // Add score
                score += int(player.getRealSpeed() * scoreMul);
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
                if (cents_second == 100) {
                    cents_second = 0;
                    secs++;
                    if (secs == 60) {
                        secs = 0;
                        minutes++;
                    }
                }
                cents_secondTrip++;
                if (cents_secondTrip == 100) {
                    cents_secondTrip = 0;
                    secsTrip++;
                    if (secsTrip == 60) {
                        secsTrip = 0;
                        minutesTrip++;
                    }
                }
                gameClockLap.restart();
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

    if (arrival){
        arrival = false;
        return RANKING;
    }

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

        return START;
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
    // slideCar
    c.effects[8]->stop();
    c.effects[8]->play();

    for (int i = (int) c.w.getSize().x / 2; !end; i -= 3) {
        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.drawInitialAnimation(c, float(i), end);
        c.w.display();
    }
    sleep(milliseconds(250));

    // Semaphore and flagger
    currentMap->incrementSpriteIndex(flagger, false, -1);
    int ms = 1000;

    currentMap->draw(c, cars);
    player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
    c.w.display();

    // comentaristIntro
    c.effects[11]->stop();
    c.effects[11]->play();
    sleep(c.effects[11]->getDuration() - c.effects[11]->getPlayingOffset());

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

    // GoalSound
    c.effects[27]->stop();
    c.effects[27]->play();

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

            score += int(scoreMul * SCORE_BONIFICATION / 10.0f); // Bonif. per dec.
            interface.textScore.setString(to_string(score));

            // Draw the time bonus
            interface.timeBonus.setString(to_string(seconds) + "." + to_string(decs_second));

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

    // slideCar
    c.effects[8]->stop();
    c.effects[8]->play();

    // claps
    c.effects[28]->stop();
    c.effects[28]->play();

    bonus.restart();
    sleep(milliseconds(20));

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

                score += int(scoreMul * SCORE_BONIFICATION / 10.0f); // Bonif. per dec.
                interface.textScore.setString(to_string(score));

                // Draw the time bonus
                interface.timeBonus.setString(to_string(seconds) + "." + to_string(decs_second));

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

                const Vector2f position = interface.sprites[7].getPosition();
                interface.sprites[7] = interface.spriteMap[mapId.first][mapId.second];
                interface.sprites[7].setPosition(position);
                interface.sprites[7].scale(2.f * c.screenScale, 2.f * c.screenScale);

                if (mapId.first < 4)
                    currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
                else {
                    goalMap.setColors(*currentMap);
                    currentMap->addNextMap(&goalMap);
                }
                // Update time when map changes
                time += int(float(currentMap->getTime()) * timeMul);

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
                timeCheck = time;
                // CheckPointEffect
                c.effects[23]->stop();
                c.effects[23]->play();
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
        arrival = true;
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
                        // WomanSound
                        c.effects[13]->stop();
                        c.effects[14]->stop();
                        c.effects[15]->stop();
                        c.effects[random_int(13, 15)]->play();
                        womanShot.restart();
                    }
                }
                if (distY <= 30.f && distX <= 1.2f){
                    // Thread with sound of the woman
                    elapsed8 = trafficCarSound.getElapsedTime().asSeconds();
                    if (elapsed8 - elapsed7 >= traffic_delay.asSeconds()){
                        // makeCarTrafficSound
                        c.effects[20]->stop();
                        c.effects[21]->stop();
                        c.effects[random_int(20, 21)]->play();
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
                // BeepSound
                c.effects[22]->stop();
                c.effects[22]->play();
             }
             else {
                interface.checkPoint.setFillColor(Color::Transparent);
                interface.checkPoint.setOutlineColor(Color::Transparent);
                interface.textForLap.setFillColor(Color::Transparent);
                interface.textForLap.setOutlineColor(Color::Transparent);
             }
             c.w.draw(interface.textForLap);
             c.w.draw(interface.checkPoint);

             if (timeCheck - time > 5){
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
    pauseShape.setPosition(c.w.getSize().x / 2.f - 120.0f * c.screenScale, c.w.getSize().y / 2.f - 180.0f * c.screenScale);
    pauseShape.setSize(sf::Vector2f(250.0f * c.screenScale, 350.0f * c.screenScale));
    pauseShape.setFillColor(Color(0, 0, 0));
    pauseShape.setOutlineColor(Color::Green);
    pauseShape.setOutlineThickness(5.0f * c.screenScale);

    Text textMenu;
    textMenu.setPosition(c.w.getSize().x / 2.f - 75.0f * c.screenScale, c.w.getSize().y / 2.f - 150.0f * c.screenScale);
    textMenu.setFont(c.options);
    textMenu.setFillColor(Color(214, 234, 12));
    textMenu.setOutlineColor(Color(12, 72, 234));
    textMenu.setOutlineThickness(2.0f * c.screenScale);
    textMenu.setCharacterSize(int(35.0f * c.screenScale));
    textMenu.setString("PAUSE");

    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Buttons of the menu
    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale, c.w.getSize().y / 2.f - 60.0f * c.screenScale, 200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                                 "Resume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale, c.w.getSize().y / 2.f + 10.0f * c.screenScale, 200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                                 "Options", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale, c.w.getSize().y / 2.f + 80.0f * c.screenScale, 200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
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
