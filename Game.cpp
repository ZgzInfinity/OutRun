/******************************************************************************
 * @file    Game.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Game.hpp"
#include <fstream>

using namespace sf;
using namespace std;

#define MAX_SPEED 300.0f
#define SPEED_MUL 70.0f
#define MAX_COUNTER 10
#define VEHICLE_DENSITY 3.0f // Greater than 0
#define VEHICLE_MIN_DISTANCE 5.0f // Minimum number of rectangles between enemies
#define DEL_VEHICLE 50.0f // Minimum number of rectangles behind the camera to delete the enemy

Game::Game(Config &c) : player(MAX_SPEED, SPEED_MUL, ACC_INC, 1.25f, 0.9375f, MAX_COUNTER,
                               "Ferrari", 0.0f, RECTANGLE), lastY(0), vehicleCrash(false), timeMul(1.0f),
                        scoreMul(1.0f), timeAI(0.0f),
                        goalMap(goalFlagger, goalEnd) {
    int nm = 0;
    const int times[] = {85, 58, 68, 50, 75, 69, 53, 54, 49, 48, 46, 42, 42, 41, 42};
    const int nobjects[] = {20, 28, 40, 15, 25, 29, 26, 31, 33, 30, 30, 30, 34, 39, 33};
    for (int i = 0; i < 5; i++) {
        vector<Map> vm;
        for (int j = 0; j <= i; j++) {
            vector<string> objectNames;
            objectNames.reserve((unsigned long) nobjects[nm]);
            for (int no = 1; no <= nobjects[nm]; no++)
                objectNames.push_back(to_string(no) + ".png");

            Map m(c, "resources/map" + to_string(nm + 1) + "/", "bg.png", objectNames, false, times[nm]);
            vm.push_back(m);

            nm++;
        }
        maps.emplace_back(vm);
    }
    mapId = make_pair(0, 0);

    // Back door
    int bdTime = 0;
    ifstream fin("resources/backdoor.info");
    if (fin.is_open()) {
        int bdMap = 1;
        while (!fin.eof()) {
            string s;
            fin >> s;
            if (s == "MAP:" && !fin.eof()) {
                fin >> bdMap;
                if (bdMap > 0 && bdMap <= 15) {
                    for (int i = 1; i < bdMap; i++) {
                        if (mapId.second < mapId.first) {
                            mapId.second++;
                        } else {
                            mapId.second = 0;
                            mapId.first++;
                        } // Level????????????????????????
                    }
                }
            } else if (s == "TIME:" && !fin.eof()) {
                fin >> bdTime;
            }
        }
        fin.close();
    }

    currentMap = &maps[mapId.first][mapId.second];
    if (mapId.first < 4)
        currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
    else {
        goalMap.setColors(*currentMap);
        currentMap->addNextMap(&goalMap);
    }

    Texture t;
    // Load the textures of the panel and assign them to their sprites
    for (int i = 1; i <= 6; i++) {
        // Load the texture from the file
        t.loadFromFile("resources/GamePanel/" + to_string(i) + ".png");
        textures.push_back(t);
    }

    // Code of first Map
    int idFirstMap = 8;

    // Fill the matrix with the tree maps
    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= i; j++) {
            t.loadFromFile("resources/GamePanel/" + to_string(idFirstMap) + ".png");
            treeMap[i][j] = t;
            idFirstMap++;
        }
    }

    time = int(float(currentMap->getTime()) * timeMul) + bdTime;
    score = 0;
    level = mapId.first + 1;

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

    checkDifficulty(c); // Loads enemies and time
}

void Game::drawHUD(Config &c) {
    Sprite s;

    // Assign positions in the game console for the game panel indicators
    const float up = float(c.w.getSize().y) / 10.0f;

    // UP
    s.setTexture(textures[0], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    const float separation = s.getGlobalBounds().width / 3.0f;
    s.setPosition(separation, up - 1.1f * s.getGlobalBounds().height);
    float initial = separation + s.getGlobalBounds().width + separation;
    c.w.draw(s);

    // Initialize the HUD indicator of time
    // Available time to arrive to the next checkpoint
    Text timeToPlay;
    timeToPlay.setString("000");
    timeToPlay.setFont(c.timeToPlay);
    timeToPlay.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    timeToPlay.setFillColor(Color::Yellow);
    timeToPlay.setOutlineColor(Color::Black);
    timeToPlay.setOutlineThickness(3.0f * c.screenScale);
    timeToPlay.setPosition(initial, up - 1.1f * float(timeToPlay.getCharacterSize()));
    timeToPlay.setString(to_string(time));
    c.w.draw(timeToPlay);
    initial += timeToPlay.getGlobalBounds().width + separation;

    s.setTexture(textures[1], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    s.setPosition(initial, up - s.getGlobalBounds().height);
    c.w.draw(s);
    initial += s.getGlobalBounds().width + separation;

    // Initialize the HUD indicator of score
    // Score of the player displayed in the panel
    Text textScore;
    textScore.setString(to_string(int(SCORE_BONIFICATION) * 100));
    textScore.setFont(c.timeToPlay);
    textScore.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textScore.setFillColor(Color(183, 164, 190));
    textScore.setOutlineColor(Color::Black);
    textScore.setOutlineThickness(3.0f * c.screenScale);
    textScore.setPosition(initial, up - 1.1f * float(textScore.getCharacterSize()));
    textScore.setString(to_string(score));
    c.w.draw(textScore);

    // Initialize the HUD indicator of lap time
    // Time of lap consumed
    Text textLap;
    textLap.setFont(c.timeToPlay);
    textLap.setString("00' 00'' 00");
    textLap.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textLap.setFillColor(Color(146, 194, 186));
    textLap.setOutlineColor(Color::Black);
    textLap.setOutlineThickness(3.0f * c.screenScale);
    initial = float(c.w.getSize().x) - separation - textLap.getGlobalBounds().width;
    textLap.setPosition(initial, up - 1.1f * float(textLap.getCharacterSize()));

    s.setTexture(textures[2], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    initial -= separation + s.getGlobalBounds().width;
    s.setPosition(initial, up - s.getGlobalBounds().height);

    textLap.setString(lap);
    c.w.draw(textLap);
    c.w.draw(s);

    // DOWN
    textures[6].loadFromFile("resources/GamePanel/7.png");
    s.setTexture(textures[6], true);
    s.setScale(2.f * c.screenScale, 1.5f * c.screenScale);
    const float down = float(c.w.getSize().y) - s.getGlobalBounds().height * 1.5f;
    s.setPosition(separation, float(c.w.getSize().y) - s.getGlobalBounds().height * 1.25f);
    initial = separation + s.getGlobalBounds().width / 4.0f;
    textures[6].loadFromFile("resources/GamePanel/7.png",
                             IntRect(0, 0, static_cast<int>(player.getRealSpeed() * 117.0f / MAX_SPEED * c.screenScale),
                                     static_cast<int>(20.0f * c.screenScale)));
    s.setTexture(textures[6], true);
    if (player.getRealSpeed() > 0.0f)
        c.w.draw(s);

    // Text
    // HUD
    Text sText;
    sText.setFont(c.speedVehicle);
    sText.setString("0001");
    sText.setCharacterSize(static_cast<unsigned int>(int(70.0f * c.screenScale)));
    sText.setFillColor(Color(206, 73, 73));
    sText.setOutlineColor(Color::Black);
    sText.setOutlineThickness(3.0f * c.screenScale);
    sText.setPosition(initial - (sText.getLocalBounds().width * 0.26f), down - float(sText.getCharacterSize()));
    initial += sText.getGlobalBounds().width;
    string strSpeed = to_string(player.getRealSpeed());
    sText.setString(strSpeed.substr(0, strSpeed.find('.')));
    c.w.draw(sText);

    s.setTexture(textures[3], true);
    s.setScale(2.f * c.screenScale, 2.f * c.screenScale);
    s.setPosition(initial / 1.2f, down - s.getGlobalBounds().height);
    c.w.draw(s);

    s.setTexture(treeMap[mapId.first][mapId.second], true);
    s.setScale(2.f * c.screenScale, 2.f * c.screenScale);
    initial = float(c.w.getSize().x) - separation - s.getGlobalBounds().width;
    s.setPosition(initial, down - s.getGlobalBounds().height);
    c.w.draw(s);

    // Initialize the HUD stage indicator
    // Level indicator
    Text textLevel;
    textLevel.setFont(c.timeToPlay);
    textLevel.setString("0");
    textLevel.setCharacterSize(static_cast<unsigned int>(int(40.0f * c.screenScale)));
    textLevel.setFillColor(Color(146, 194, 186));
    textLevel.setOutlineColor(Color::Black);
    textLevel.setOutlineThickness(3.0f * c.screenScale);
    initial -= separation + textLevel.getGlobalBounds().width;
    textLevel.setPosition(initial, down - 1.1f * float(textLevel.getCharacterSize()));
    textLevel.setString(to_string(level));
    c.w.draw(textLevel);

    s.setTexture(textures[4], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    initial -= separation + s.getGlobalBounds().width;
    s.setPosition(initial, down - s.getGlobalBounds().height);
    c.w.draw(s);
}

void Game::drawCheckpoint(Config &c, bool visible) {
    Sprite s;

    // CheckPoint title
    Text checkPointTitle;
    checkPointTitle.setFont(c.timeToPlay);
    checkPointTitle.setString("CHECKPOINT!");
    checkPointTitle.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));

    checkPointTitle.setOutlineThickness(3.0f * c.screenScale);
    float initial = c.w.getSize().y / 3.0f + 0.25f * float(checkPointTitle.getCharacterSize());
    checkPointTitle.setPosition((float(c.w.getSize().x) - checkPointTitle.getGlobalBounds().width) / 2.0f,
                                c.w.getSize().y / 3.0f - float(checkPointTitle.getCharacterSize()));

    // Time inverted by the player for complete the game
    s.setTexture(textures[5], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    s.setPosition((float(c.w.getSize().x) - s.getGlobalBounds().width) / 2.0f, initial);
    initial += s.getGlobalBounds().height * 1.25f;

    if (visible) {
        checkPointTitle.setFillColor(Color::Yellow);
        checkPointTitle.setOutlineColor(Color(14, 29, 184));
        c.w.draw(s);
        c.w.draw(checkPointTitle);

    } else {
        checkPointTitle.setFillColor(Color::Transparent);
        checkPointTitle.setOutlineColor(Color::Transparent);
    }

    Text textForLap;
    textForLap.setFont(c.timeToPlay);
    textForLap.setString("00' 00'' 00");
    textForLap.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textForLap.setOutlineThickness(3.0f * c.screenScale);

    s.setTexture(textures[2], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    textForLap.setPosition(
            (float(c.w.getSize().x) - textForLap.getGlobalBounds().width - s.getGlobalBounds().width) / 1.7f,
            initial + 1.1f * s.getGlobalBounds().height);
    s.setPosition((float(c.w.getSize().x) + textForLap.getGlobalBounds().width / 2.0f) / 2.4f, initial);

    if (visible) {
        textForLap.setFillColor(Color(146, 194, 186));
        textForLap.setOutlineColor(Color::Black);
        c.w.draw(s);
    } else {
        textForLap.setFillColor(Color::Transparent);
        textForLap.setOutlineColor(Color::Transparent);
    }

    textForLap.setString(lapCheckPoint);
    c.w.draw(textForLap);
}

void Game::drawGameOver(Config &c) {
    // Game over text
    Text gameOver;
    gameOver.setFont(c.timeToPlay);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(static_cast<unsigned int>(int(60.0f * c.screenScale)));
    gameOver.setFillColor(Color::Yellow);
    gameOver.setOutlineColor(Color(14, 29, 184));
    gameOver.setOutlineThickness(3.0f * c.screenScale);
    gameOver.setPosition((float(c.w.getSize().x) - gameOver.getGlobalBounds().width) / 2.0f,
                         (float(c.w.getSize().y) - float(gameOver.getCharacterSize())) / 2.0f);
    c.w.draw(gameOver);
}

void Game::drawBonus(Config &c, int seconds, int decs_second) {
    // Only for separation
    Sprite s;
    s.setTexture(textures[0], true);
    s.setScale(1.5f * c.screenScale, 1.5f * c.screenScale);
    const float separation = s.getGlobalBounds().width / 3.0f;

    // Final score after completing all the levels
    Text bonification;
    bonification.setFont(c.timeToPlay);
    bonification.setString("BONUS POINTS!");
    bonification.setCharacterSize(static_cast<unsigned int>(int(65.0f * c.screenScale)));
    bonification.setFillColor(Color::Yellow);
    bonification.setOutlineColor(Color(12, 12, 12));
    bonification.setOutlineThickness(3.0f * c.screenScale);
    float initialY = float(c.w.getSize().y) / 3.0f + float(bonification.getCharacterSize());
    float initial = (float(c.w.getSize().x) - bonification.getGlobalBounds().width) / 2.0f;
    bonification.setPosition(initial, float(c.w.getSize().y) / 3.0f - float(bonification.getCharacterSize()));
    c.w.draw(bonification);

    // Time bonus to the player
    Text timeBonus;
    timeBonus.setFont(c.timeToPlay);
    timeBonus.setString("000.0");
    timeBonus.setCharacterSize(static_cast<unsigned int>(int(55.0f * c.screenScale)));
    timeBonus.setFillColor(Color::Yellow);
    timeBonus.setOutlineColor(Color(12, 12, 12));
    timeBonus.setOutlineThickness(3.0f * c.screenScale);
    initial -= timeBonus.getLocalBounds().width;
    timeBonus.setPosition(initial, initialY);
    initialY += float(timeBonus.getCharacterSize());
    initial += 1.25f * timeBonus.getLocalBounds().width;
    timeBonus.setString(to_string(seconds) + "." + to_string(decs_second));
    c.w.draw(timeBonus);

    // Seconds arrival indicator
    Text secondsIndicator;
    secondsIndicator.setFont(c.timeToPlay);
    secondsIndicator.setString("SEC");
    secondsIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    secondsIndicator.setFillColor(Color(183, 164, 190));
    secondsIndicator.setOutlineColor(Color::Black);
    secondsIndicator.setOutlineThickness(3.0f * c.screenScale);
    secondsIndicator.setPosition(initial, initialY - float(secondsIndicator.getCharacterSize()));
    initial += separation + secondsIndicator.getLocalBounds().width;
    c.w.draw(secondsIndicator);

    // Seconds arrival indicator
    Text crossSign;
    crossSign.setFont(c.timeToPlay);
    crossSign.setString("x");
    crossSign.setCharacterSize(static_cast<unsigned int>(int(40.0f * c.screenScale)));
    crossSign.setFillColor(Color(232, 191, 157));
    crossSign.setOutlineColor(Color::Black);
    crossSign.setOutlineThickness(3.0f * c.screenScale);
    crossSign.setPosition(initial, initialY - float(crossSign.getCharacterSize()));
    initial += separation + crossSign.getLocalBounds().width;
    c.w.draw(crossSign);

    // Score factor to multiply
    Text scoreMultiply;
    scoreMultiply.setFont(c.timeToPlay);
    scoreMultiply.setString(to_string((long long) SCORE_BONIFICATION));
    scoreMultiply.setCharacterSize(static_cast<unsigned int>(int(55.0f * c.screenScale)));
    scoreMultiply.setFillColor(Color::Yellow);
    scoreMultiply.setOutlineColor(Color(12, 12, 12));
    scoreMultiply.setOutlineThickness(3.0f * c.screenScale);
    scoreMultiply.setPosition(initial, initialY - float(scoreMultiply.getCharacterSize()));
    initial += separation + scoreMultiply.getLocalBounds().width;
    c.w.draw(scoreMultiply);

    // Seconds arrival indicator
    Text pointsIndicator;
    pointsIndicator.setFont(c.timeToPlay);
    pointsIndicator.setString("PTS");
    pointsIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * c.screenScale)));
    pointsIndicator.setFillColor(Color(183, 164, 190));
    pointsIndicator.setOutlineColor(Color::Black);
    pointsIndicator.setOutlineThickness(3.0f * c.screenScale);
    pointsIndicator.setPosition(initial, initialY - float(pointsIndicator.getCharacterSize()));
    c.w.draw(pointsIndicator);
}

void Game::checkDifficulty(Config &c) {
    int numCars = static_cast<int>(cars.size()); // Number of vehicles simultaneously
    time = int(float(time - int(timeAI)) / timeMul); // Restore original time

    float prevScoreMul = scoreMul;
    switch (c.level) {
        case PEACEFUL:
            numCars = 0;
            timeMul = 1.1f;
            scoreMul = 0.25f;
            if (c.enableAI)
                c.maxAggressiveness = 0.0f;
            break;
        case EASY:
            numCars = 5;
            timeMul = 1.1f;
            scoreMul = 0.5f;
            if (c.enableAI)
                c.maxAggressiveness = 0.25f;
            break;
        case NORMAL:
            numCars = 10;
            timeMul = 1.0f;
            scoreMul = 1.0f;
            if (c.enableAI)
                c.maxAggressiveness = 0.5f;
            break;
        case HARD:
            numCars = 15;
            timeMul = 0.9f;
            scoreMul = 1.5f;
            if (c.enableAI)
                c.maxAggressiveness = 0.75f;
            break;
        default:
            break;
    }
    if (prevScoreMul < scoreMul) // Keep the least multiplier
        scoreMul = prevScoreMul;

    if (!c.enableAI)
        c.maxAggressiveness = 0.0f;

    // Vehicles
    cars.reserve(static_cast<unsigned long>(numCars));
    if (cars.size() > numCars) {
        while (cars.size() > numCars)
            cars.pop_back();
    } else if (cars.size() < numCars) {
        const int maxSprites = 6;
        const float vehicleScales[maxSprites] = {1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
        for (int i = static_cast<int>(cars.size()); i < numCars; i++) {
            Enemy v(MAX_SPEED, SPEED_MUL, vehicleScales[i % maxSprites], MAX_COUNTER,
                    "car" + to_string(1 + i % maxSprites), -RECTANGLE * DEL_VEHICLE * 3.0f);
            cars.push_back(v);
        }
    }

    for (Enemy &v : cars)
        v.setAI(c.maxAggressiveness);

    time = int(float(time) * timeMul);
    timeAI = !cars.empty() ? float(time) * c.maxAggressiveness / 3.0f : 0.0f;
    time += int(timeAI);

    if (time < 0)
        time = 0;
}

bool Game::isInGame() const {
    return inGame;
}

unsigned long Game::getScore() const {
    return static_cast<unsigned long>(score);
}

float Game::getMinutesTrip() const {
    return minutesTrip;
}

float Game::getSecsTrip() const {
    return secsTrip;
}

float Game::getCents_SecondTrip() const {
    return cents_secondTrip;
}

State Game::play(Config &c) {
    if (!inGame) {
        inGame = true;
        initialAnimation(c);
    }

    c.window.setKeyRepeatEnabled(false);

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

    while (!finalGame && !arrival && c.window.isOpen()) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e))
            if (e.type == Event::Closed)
                return EXIT;

        updateAndDraw(c, action, direction);

        if (!finalGame) {
            if (Keyboard::isKeyPressed(c.menuKey) || onPause) {
                // Pause the game
                c.effects[1]->stop();
                c.effects[1]->play();
                status = pause(c, action, direction);

                // Control the exit of the game
                if (status == OPTIONS) {
                    comeFromOptions = true;
                    finalGame = true;
                } else if (status == START) {
                    finalGame = true;
                    inGame = false;
                    return START;
                } else if (status == GAME) {
                    c.themes[c.currentSoundtrack]->play();
                } else if (status == EXIT) {
                    return EXIT;
                }
                gameClockLap.restart();
            }

            // Update the score of the player if the player is not stopped
            if (player.getRealSpeed() > 0.0f) {
                // Add score
                score += int(player.getRealSpeed() * scoreMul);
            }

            // Get the actual time
            elapsed4 = gameClockLap.getElapsedTime().asSeconds();

            // Check if a tenth of second has passed between both timestamps
            if (elapsed4 - elapsed3 >= shot_delayLap.asSeconds()) {
                cents_second += elapsed4;
                if (cents_second >= 1.f) {
                    cents_second -= 1.f;
                    secs++;
                    if (secs == 60.f) {
                        secs = 0;
                        minutes++;
                    }
                }
                cents_secondTrip += elapsed4;
                if (cents_secondTrip >= 1.f) {
                    cents_secondTrip -= 1.f;
                    secsTrip++;
                    if (secsTrip == 60.f) {
                        secsTrip = 0;
                        minutesTrip++;
                    }
                }
                gameClockLap.restart();
            }

            // Get the actual time
            elapsed2 = gameClockTime.getElapsedTime().asSeconds();

            // Check if a second has passed between both timestamps
            if (elapsed2 - elapsed1 >= shot_delayTime.asSeconds()) {
                // Draw time
                time--;
                gameClockTime.restart();
            }

            // Update the indicators
            lap = (minutes < 10) ? "0" + to_string(int(minutes)) + " '" : to_string(int(minutes)) + " ''";
            lap += (secs < 10) ? "0" + to_string(int(secs)) + " ''" : to_string(int(secs)) + " ''";
            lap += to_string(int(cents_second * 100.f));

            drawHUD(c);

            Sprite bufferSprite(c.w.getTexture());
            c.w.display();
            c.window.draw(bufferSprite);
            c.window.display();

            if (time == 10) {
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

    if (arrival) {
        arrival = false;
        return RANKING;
    }

    if (status != OPTIONS && status != START) {
        // Draw the game over text in the console window
        drawGameOver(c);
        c.themes[c.currentSoundtrack]->stop();
        c.effects[24]->stop();
        c.effects[24]->play();
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(c.effects[24]->getDuration());

        bool startPressed = false;
        c.themes[5]->play();
        while (!startPressed) {

            // Detect the possible events
            Event e{};
            while (c.window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    return EXIT;
                }
            }

            startPressed = Keyboard::isKeyPressed(c.menuEnterKey);
        }
        c.themes[5]->stop();
        c.effects[2]->stop();
        c.effects[2]->play();
        return START;
    }
    return status;
}

State Game::initialAnimation(Config &c) {
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

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.drawInitialAnimation(c, float(i), end);
        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
    }

    // Detect the possible events
    Event e{};
    while (c.window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            return EXIT;
        }
    }


    sleep(milliseconds(250));

    // Semaphore and flagger
    currentMap->incrementSpriteIndex(flagger, false, -1);
    int ms = 1000;

    currentMap->draw(c, cars);
    player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();

    // Detect the possible events
    while (c.window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            return EXIT;
        }
    }


    // comentaristIntro
    c.effects[11]->stop();
    c.effects[11]->play();
    sleep(c.effects[11]->getDuration() - c.effects[11]->getPlayingOffset());

    for (int i = 0; i < 3; i++) {

        // Detect the possible events
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT, currentMap->getElevation(player.getPosY()));
        bufferSprite.setTexture(c.w.getTexture(), true);
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();

        // Flagger
        if (i == 2) {
            for (; ms > 0; ms -= 200) {
                sleep(milliseconds(200));

                currentMap->incrementSpriteIndex(flagger, false);

                // Draw map
                c.w.clear();
                currentMap->draw(c, cars);
                player.draw(c, Vehicle::Action::NONE, Vehicle::Direction::RIGHT,
                            currentMap->getElevation(player.getPosY()));
                bufferSprite.setTexture(c.w.getTexture(), true);
                c.w.display();
                c.window.draw(bufferSprite);
                c.window.display();
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
    return GAME;
}

State Game::goalAnimation(Config &c) {
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

    while (int(player.getPosY()) < goalEnd) {

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Update camera
        currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);
        player.setPosition(player.getPosX(), player.getPosY() + 1);
        player.draw(c, Vehicle::Action::ACCELERATE, Vehicle::Direction::RIGHT,
                    currentMap->getElevation(player.getPosY()), false);

        // Flager animation
        if (gameClockTime.getElapsedTime().asMilliseconds() - currentTime >= 200.0f) {
            if (increment >= 5) {
                increment = 0;
                currentMap->incrementSpriteIndex(goalFlagger, false, -5);
            } else {
                currentMap->incrementSpriteIndex(goalFlagger, false);
                increment++;
            }
            currentTime = gameClockTime.getElapsedTime().asMilliseconds();
        }

        elapsed12 = bonus.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed12 - elapsed11 >= bonus_delay.asSeconds()) {
            // Decrement one Tenth of a second

            if (decsTime > 0) {
                decsTime--;

                seconds = decsTime / 10;
                // Decs per second
                decs_second = decsTime % 10;
            }

            score += int(scoreMul * SCORE_BONIFICATION / 10.0f); // Bonif. per dec.

            bonus.restart();
        }

        // Draw the bonus indicators
        drawBonus(c, seconds, decs_second);
        drawHUD(c);

        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
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

        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Draw map
        c.w.clear();
        currentMap->draw(c, cars);

        if (!firstEnd) {
            player.drawGoalAnimation(c, step, end);
        } else {
            player.drawGoalAnimation(c, lastStep, end, false);
        }

        if (end && !firstEnd) {
            firstEnd = true;
        }
        if (firstEnd) {
            lastStep = step - 1;
        }

        if (decsTime > 0) {
            elapsed12 = bonus.getElapsedTime().asSeconds();

            // Check if a second has passed between both timestamps
            if (elapsed12 - elapsed11 >= bonus_delay.asSeconds()) {
                // Decrement one Tenth of a second
                decsTime--;

                seconds = decsTime / 10;
                // Decs per second
                decs_second = decsTime % 10;

                score += int(scoreMul * SCORE_BONIFICATION / 10.0f); // Bonif. per dec.

                bonus.restart();
            }

            // Draw the bonus indicators
            drawBonus(c, seconds, decs_second);

            // Draw speed
            drawHUD(c);

            end = false;
        }

        // Draw the bonus indicators
        drawBonus(c, seconds, decs_second);

        // Draw speed
        drawHUD(c);

        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
    }
    Sprite bufferSprite(c.w.getTexture());
    c.w.display();
    c.window.draw(bufferSprite);
    c.window.display();
    sleep(c.effects[27]->getDuration());
    return RANKING;
}


void Game::updateAndDraw(Config &c, Vehicle::Action &action, Vehicle::Direction &direction) {
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

                if (mapId.first < 4)
                    currentMap->addFork(&maps[mapId.first + 1][mapId.second], &maps[mapId.first + 1][mapId.second + 1]);
                else {
                    goalMap.setColors(*currentMap);
                    currentMap->addNextMap(&goalMap);
                }
                // Update time when map changes
                time += int(float(currentMap->getTime()) * timeMul);

                // Update the indicators
                if (!checkPoint) {
                    lapCheckPoint = (minutes < 10) ? "0" + to_string(int(minutes)) + " '" : to_string(int(minutes)) +
                                                                                            " ''";
                    lapCheckPoint += (secs < 10) ? "0" + to_string(int(secs)) + " ''" : to_string(int(secs)) + " ''";
                    lapCheckPoint += to_string(int(cents_second * 100.f));

                    // Initialize to zero the time
                    cents_second = 0;
                    secs = 0;
                    minutes = 0;
                }
                checkPoint = true;
                timeCheck = time;
                // CheckPointEffect
                c.effects[23]->stop();
                c.effects[23]->play();
            }
            currentMap->updateView(player.getPosX(), player.getPosY() - RECTANGLE);

            lastY = currentMap->getCamY();
        } else {
            finalGame = true;
        }
    }

    if (currentMap->isGoalMap()) {
        State status = goalAnimation(c);
        if (status == EXIT) {
            exit(1);
        }
        arrival = true;
    }

    if (!finalGame) {
        // Update and prepare cars to draw
        if (lastY <= currentMap->getCamY() + float(c.renderLen))
            lastY = currentMap->getCamY() + float(c.renderLen);
        for (Enemy &v : cars) {
            if (currentMap->inFork(v.getPosY())) {
                v.setPosition(v.getPosX(), -RECTANGLE * DEL_VEHICLE * 3.0f);
            } else if (v.getPosY() + DEL_VEHICLE < currentMap->getCamY()) {
                v.update(lastY, lastY + float(c.renderLen) / VEHICLE_DENSITY, c.maxAggressiveness);
                lastY = v.getPosY() + VEHICLE_MIN_DISTANCE * RECTANGLE;
            }

            float posY = v.getPosY();
            v.draw(currentMap->getElevation(posY), currentMap->getCamX());
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
        } else {
            player.hitControl(vehicleCrash);
        }

        player.draw(c, action, direction, currentMap->getElevation(player.getPosY()));

        if (!player.isCrashing()) {
            vehicleCrash = false;
            float crashPos;
            bool crash = currentMap->hasCrashed(c, player.getPreviousY(), player.getPosY(), player.getPosX(),
                                                player.getMinScreenX(), player.getMaxScreenX(), crashPos);
            if (!crash)
                for (int i = 0; !vehicleCrash && i < cars.size(); i++)
                    vehicleCrash = cars[i].hasCrashed(player.getPreviousY(), player.getPosY(),
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
                if (distY <= 20.f && distX <= 0.3f) {
                    // Thread with sound of the woman
                    elapsed6 = womanShot.getElapsedTime().asSeconds();
                    if (elapsed6 - elapsed5 >= woman_delay.asSeconds()) {
                        // WomanSound
                        c.effects[13]->stop();
                        c.effects[14]->stop();
                        c.effects[15]->stop();
                        c.effects[random_int(13, 15)]->play();
                        womanShot.restart();
                    }
                }
                if (distY <= 30.f && distX <= 1.2f) {
                    // Thread with sound of the woman
                    elapsed8 = trafficCarSound.getElapsedTime().asSeconds();
                    if (elapsed8 - elapsed7 >= traffic_delay.asSeconds()) {
                        // makeCarTrafficSound
                        c.effects[20]->stop();
                        c.effects[21]->stop();
                        c.effects[random_int(20, 21)]->play();
                        trafficCarSound.restart();
                    }
                }
            }
        }
        if (checkPoint) {
            elapsed10 = blinkTime.getElapsedTime().asSeconds();
            if (elapsed10 - elapsed9 >= blink_delay.asSeconds()) {
                blink = !blink;
                blinkTime.restart();
            }
            if (blink) {
                drawCheckpoint(c, true);
                // BeepSound
                c.effects[22]->stop();
                c.effects[22]->play();
            } else {
                drawCheckpoint(c, false);
            }

            if (timeCheck - time > 5) {
                checkPoint = false;
            }
        }
    }
}

State Game::pause(Config &c, const Vehicle::Action &a, const Vehicle::Direction &d) {
    c.w.clear();

    // Draw the map
    currentMap->draw(c, cars);

    // Draw the vehicle of the player
    player.draw(c, a, d, currentMap->getElevation(player.getPosY()), false);

    drawHUD(c);

    c.w.display();
    const Texture bgTexture(c.w.getTexture());
    Sprite bgSprite(bgTexture);
    bgSprite.setScale(float(c.window.getSize().x) / float(c.w.getSize().x),
                      float(c.window.getSize().y) / float(c.w.getSize().y));

    c.w.clear();
    c.window.setView(View(Vector2f(c.window.getSize().x / 2.0f, c.window.getSize().y / 2.0f),
                          Vector2f(c.window.getSize().x, c.window.getSize().y)));
    c.w.create(static_cast<unsigned int>(c.window.getView().getSize().x),
               static_cast<unsigned int>(c.window.getView().getSize().y));
    c.screenScale = float(c.w.getSize().x) / float(SCREEN_DEFAULT_X);

    // Start the pause menu of the game
    vector<Button> menuButtons;

    // Game in pause
    onPause = true;

    // Stop the music of the level
    c.themes[c.currentSoundtrack]->pause();

    RectangleShape shape;
    shape.setPosition(0, 0);
    shape.setSize(Vector2f(c.w.getSize().x, c.w.getSize().y));
    shape.setFillColor(Color(0, 0, 0, 200));

    RectangleShape pauseShape;
    pauseShape.setPosition(c.w.getSize().x / 2.f - 120.0f * c.screenScale,
                           c.w.getSize().y / 2.f - 180.0f * c.screenScale);
    pauseShape.setSize(sf::Vector2f(250.0f * c.screenScale, 400.0f * c.screenScale));
    pauseShape.setFillColor(Color(0, 0, 0));
    pauseShape.setOutlineColor(Color::Green);
    pauseShape.setOutlineThickness(5.0f * c.screenScale);

    Text textMenu;
    textMenu.setPosition(c.w.getSize().x / 2.f - 75.0f * c.screenScale, c.w.getSize().y / 2.f - 150.0f * c.screenScale);
    textMenu.setFont(c.options);
    textMenu.setFillColor(Color(214, 234, 12));
    textMenu.setOutlineColor(Color(12, 72, 234));
    textMenu.setOutlineThickness(2.0f * c.screenScale);
    textMenu.setCharacterSize(static_cast<unsigned int>(int(35.0f * c.screenScale)));
    textMenu.setString("PAUSE");

    // Control if the start key is pressed or not
    bool startPressed = false;

    // Control the option selected by the user
    int optionSelected = 0;

    // Buttons of the menu
    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale,
                             c.w.getSize().y / 2.f - 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Resume", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 1, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale, c.w.getSize().y / 2.f,
                             200.0f * c.screenScale, 30.0f * c.screenScale, c.options,
                             "Options", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 70.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Home", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

    menuButtons.emplace_back(c.w.getSize().x / 2.f - 95.0f * c.screenScale,
                             c.w.getSize().y / 2.f + 140.0f * c.screenScale, 200.0f * c.screenScale,
                             30.0f * c.screenScale, c.options,
                             "Exit", Color(0, 255, 0), Color(255, 255, 0), Color(0, 255, 0), 0, c.screenScale);

    while (!startPressed) {
        // Detect the possible events
        Event e{};
        while (c.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return EXIT;
            }
        }

        // Check if the up or down cursor keys have been pressed or not
        if (Keyboard::isKeyPressed(c.menuDownKey)) {
            // Up cursor pressed and change the soundtrack selected in the list
            if (optionSelected != int(menuButtons.size() - 1)) {
                // Change the color appearance of both buttons
                c.effects[0]->stop();
                c.effects[0]->play();
                optionSelected++;
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected - 1].setButtonState(BUTTON_IDLE);
            }
        } else if (Keyboard::isKeyPressed(c.menuUpKey)) {
            // Down cursor pressed and change the soundtrack selected in the list
            if (optionSelected != 0) {
                c.effects[0]->stop();
                c.effects[0]->play();
                optionSelected--;
                // Change the color appearance of both buttons
                menuButtons[optionSelected].setButtonState(BUTTON_HOVER);
                menuButtons[optionSelected + 1].setButtonState(BUTTON_IDLE);
            }
        } else if (Keyboard::isKeyPressed(c.menuEnterKey)) {
            if (comeFromOptions) {
                comeFromOptions = false;
            } else {
                startPressed = true;
            }
        }

        c.w.draw(bgSprite);

        c.w.draw(shape);

        c.w.draw(pauseShape);
        c.w.draw(textMenu);

        for (Button b : menuButtons) {
            b.render(&c.w);
        }

        Sprite bufferSprite(c.w.getTexture());
        c.w.display();
        c.window.draw(bufferSprite);
        c.window.display();
        sleep(milliseconds(100));
    }

    c.effects[2]->stop();
    c.effects[2]->play();

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

    // Check the option selected by the user
    switch (optionSelected) {
        case 0:
            // Resume button selected and reanudate the music
            onPause = false;
            return GAME;
        case 1:
            // Options button selected
            return OPTIONS;
        case 2:
            // Quit button selected
            onPause = false;
            return START;
        default:
            return EXIT;
    }
}
