
/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */



/*
 * Implementation file of the module MenuRanking
 */

#include "MenuRanking.h"



/**
 * Default constructor
 * @param _score is the total score reached by the player after finishing a game round
 * @param _minutes are the total minutes elapsed by the player to complete the game round
 * @param _secs are the total seconds elapsed by the player to complete the game round
 * @param _cents_seconds are the total hundredths of second elapsed by the player to complete the game round
 */
MenuRanking::MenuRanking(const int long long _score, const int _minutes,
                         const int _secs, const int _cents_second) : Menu()
{

    /*
     * Assign all the default configuration
     */

    score = _score;
    minutes = _minutes;
    secs = _secs;
    cents_second = _cents_second;

    time = TIME_RANKING_MENU;
    blink = false;

    lettersIntroduced = 0;
    offsetStartIndicator = 0;
    offsetRankingTitle = 0;
    offsetTimeCounter = 0;
    offsetScoreIndicator = 0;
    offsetPlayerIndicator = 0;
    offsetRecordIndicator = 0;
    offsetIndexIndicator = 5.5f;

    name = "_";
}



/**
 * Load the menu with all its configuration
 * @param input is the module that has all the configuration of the game
 */
void MenuRanking::loadMenu(Input& input){

    /*
     * Compute the offset position in axis Y of the
     * start indicator text depending on the screen resolution
     */

    if (input.currentIndexResolution == (int)Resolution::SCREEN_1 ||
        input.currentIndexResolution == (int)Resolution::__COUNT)
        offsetStartIndicator = 60;
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_2)
        offsetStartIndicator = 70;
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_3)
        offsetStartIndicator = 100;

    /*
     * Configure the offset of the rest of indicators using the screen resolution
     */


    if (input.currentIndexResolution >= (int)Resolution::SCREEN_2 &&
        input.currentIndexResolution < (int)Resolution::__COUNT)
    {
        offsetRankingTitle = 50;
        offsetTimeCounter = 50;
        offsetScoreIndicator = 40;
        offsetPlayerIndicator = 40;
        offsetRecordIndicator = 40;
        offsetIndexIndicator = 8.f;
    }

    // Get the scores for the current traffic and difficulty levels
    scoreRankingPlayer = getGlobalScores(input);

    // Check if the player has beaten a new record
    record = isNewRecord(scoreRankingPlayer, score);

    // Prepare the bakcground of the ranking menu
    backgroundMenu.loadFromFile("Resources/Menus/RankingMenu/rankingBackground.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    // Load the font of the menu for text indicators
    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");

    // Set the title of the menu
    rankingTitle.setFont(fontMenu);
    rankingTitle.setPosition(input.gameWindow.getSize().x / 4.f, input.gameWindow.getSize().y / 17.f - offsetRankingTitle);
    rankingTitle.setString("BEST OUTRUNNERS");
    rankingTitle.setCharacterSize(static_cast<unsigned int>(int(65.0f * input.screenScaleX)));
    rankingTitle.setFillColor(sf::Color::Yellow);
    rankingTitle.setOutlineColor(sf::Color(12, 12, 12));
    rankingTitle.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the score indicator
    scoreIndicator.setFont(fontMenu);
    scoreIndicator.setPosition(input.gameWindow.getSize().x / 8.f, input.gameWindow.getSize().y / 6.0f - offsetScoreIndicator);
    scoreIndicator.setString("SCORE");
    scoreIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    scoreIndicator.setFillColor(sf::Color(146, 194, 186));
    scoreIndicator.setOutlineColor(sf::Color(12, 12, 12));
    scoreIndicator.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the player name indicator
    playerIndicator.setFont(fontMenu);
    playerIndicator.setPosition(input.gameWindow.getSize().x / 2.2f, input.gameWindow.getSize().y / 6.0f - offsetPlayerIndicator);
    playerIndicator.setString("NAME");
    playerIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    playerIndicator.setFillColor(sf::Color(146, 194, 186));
    playerIndicator.setOutlineColor(sf::Color(12, 12, 12));
    playerIndicator.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the record indicator
    recordIndicator.setFont(fontMenu);
    recordIndicator.setPosition((input.gameWindow.getSize().x / 2.f) * 1.5f, input.gameWindow.getSize().y / 6.0f - offsetRecordIndicator);
    recordIndicator.setString("RECORD");
    recordIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    recordIndicator.setFillColor(sf::Color(146, 194, 186));
    recordIndicator.setOutlineColor(sf::Color(12, 12, 12));
    recordIndicator.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the time counter
    timeCounter.setFont(fontMenu);
    timeCounter.setCharacterSize(static_cast<unsigned int>(int(62.0f * input.screenScaleX)));
    timeCounter.setString(to_string(time));
    timeCounter.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width,
                            input.gameWindow.getSize().y / 15.7f - offsetTimeCounter);
    timeCounter.setFillColor(sf::Color::Red);
    timeCounter.setOutlineColor(sf::Color(12, 12, 12));
    timeCounter.setOutlineThickness(3.0f * input.screenScaleX);

    // Prepare the score of the player
    scorePlayer.setFont(fontMenu);
    scorePlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    scorePlayer.setFillColor(sf::Color(146, 194, 186));
    scorePlayer.setOutlineColor(sf::Color::Black);
    scorePlayer.setOutlineThickness(3.0f * input.screenScaleX);

    // Prepare the name of the player
    namePlayer.setFont(fontMenu);
    namePlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    namePlayer.setFillColor(sf::Color(146, 194, 186));
    namePlayer.setOutlineColor(sf::Color(12, 12, 12));
    namePlayer.setOutlineThickness(3.0f * input.screenScaleX);

    /*
     * Prepare the total time elapsed by the player to complete the game round
     * (minutes, seconds, hundredths of seconds)
     */

    minutesPlayer.setFont(fontMenu);
    minutesPlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    minutesPlayer.setFillColor(sf::Color(146, 194, 186));
    minutesPlayer.setOutlineColor(sf::Color::Black);
    minutesPlayer.setOutlineThickness(3.0f * input.screenScaleX);

    secondsPlayer.setFont(fontMenu);
    secondsPlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    secondsPlayer.setFillColor(sf::Color(146, 194, 186));
    secondsPlayer.setOutlineColor(sf::Color::Black);
    secondsPlayer.setOutlineThickness(3.0f * input.screenScaleX);

    centsPlayer.setFont(fontMenu);
    centsPlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    centsPlayer.setFillColor(sf::Color(146, 194, 186));
    centsPlayer.setOutlineColor(sf::Color::Black);
    centsPlayer.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the ndex indicator
    index.setFont(fontMenu);
    index.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    index.setFillColor(sf::Color(180, 130, 211));
    index.setOutlineColor(sf::Color::Black);
    index.setOutlineThickness(3.0f * input.screenScaleX);

    // Set the start indicator
    start.setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    start.setFont(fontMenu);
    start.setFillColor(sf::Color::Green);
    start.setOutlineColor(sf::Color::Black);
    start.setOutlineThickness(3.0f * input.screenScaleX);
}



/**
 * Detect an action of the player and executes it
 * @param input is the module that has all the configuration of the game
 */
void MenuRanking::handleEvent(Input& input){
    sf::Event event;
    // Detect the events of the player
    while(input.gameWindow.pollEvent(event)){
        if (input.closed(event))
            // Close the game
            escapePressed = true;
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            // Player presses key and there is not record (exists the ranking menu)
            if (record == -1 || (record != -1 && lettersIntroduced == 3)){
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                startPressed = true;
            }
        }
        else if (input.isKeypressedEvent(event)){
            // There is record (detect key pressed events
            if (lettersIntroduced < 3 && input.pressedKeyIsValidLetter(event)){
                // Check if there are still keys to be introduced and they are valid
                lettersIntroduced++;
                string keyLetter = input.getKeyCodeName(event.key.code);
                if (name == "_") {
                    // First letter introduced
                    name = keyLetter;
                    Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                }
                else {
                    if (lettersIntroduced == 3) {
                        // Total letters introduced (player name completed
                        name = name.substr(0, name.size() - 1);
                        name += keyLetter;
                        Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                    }
                    else {
                        // Only 2 of 3 letters have been introduced
                        name += keyLetter + "_";
                        Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                    }
                }
            }
            else
                // Wrong key tried to be inserted
                Audio::play(Sfx::MENU_SELECTION_WRONG, false);
        }
    }
}



/**
 * Draw the menu in the screen
 * @param input is the module that has all the configuration of the game
 */
void MenuRanking::draw(Input& input){

    // Compute the internal clocks
    rankingTime.restart();
    elapsed1 = rankingTime.getElapsedTime().asSeconds();
    elapsed3 = blinkStart.getElapsedTime().asSeconds();

    // Play the last wave sound
    Audio::play(Soundtrack::LAST_WAVE, true);

    // There is no record
    if (record != 1)
        Audio::play(Sfx::HALL_FAME, false);

    // Display the ranking menu until time elapses and player does not exist
    while (time > 0 && !escapePressed && !startPressed){

        // Detect the actions of the player
        handleEvent(input);

        // Get the actual time
        elapsed2 = rankingTime.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed2 - elapsed1 >= ranking_delay.asSeconds()) {
            // Decrease the time counter
            time--;
            rankingTime.restart();
            timeCounter.setString(to_string(time));
            timeCounter.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width,
                                    input.gameWindow.getSize().y / 15.7f - offsetRankingTitle);
        }

        /*
         * Draw all the menu components except the player scores
         */

        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(rankingTitle);
        input.gameWindow.draw(timeCounter);
        input.gameWindow.draw(scoreIndicator);
        input.gameWindow.draw(playerIndicator);
        input.gameWindow.draw(recordIndicator);

        // There is no new record
        if (record == -1) {

            // Draw the start indicator
            start.setString("PRESS START!");
            start.setPosition(input.gameWindow.getSize().x / 2.f - start.getLocalBounds().width /2.f,
                              (input.gameWindow.getSize().y / 4.5f) + 400.0f + offsetStartIndicator);

            // There is not a new record
            for (int i = 1; i <= 7; i++) {

                /*
                * Draw the player score data
                */

                // Index
                index.setString(to_string(i) + ".");
                index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                                  (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                // Score
                scorePlayer.setString(to_string(scoreRankingPlayer[i - 1].score));
                scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                // Player name
                namePlayer.setString(scoreRankingPlayer[i - 1].name);
                namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                // Minutes
                minutesPlayer.setString(to_string(scoreRankingPlayer[i - 1].minutes) + "'");
                minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                // Seconds
                if (scoreRankingPlayer[i - 1].secs >= 10)
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i - 1].secs) + "''");
                else
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i - 1].secs) + "''");

                secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                // Hundredths of second
                if (scoreRankingPlayer[i - 1].cents_second >= 10)
                    centsPlayer.setString(to_string(scoreRankingPlayer[i - 1].cents_second));
                else
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i - 1].cents_second));

                centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));


                /*
                 * Draw the score of the player
                 */

                input.gameWindow.draw(index);
                input.gameWindow.draw(scorePlayer);
                input.gameWindow.draw(namePlayer);
                input.gameWindow.draw(minutesPlayer);
                input.gameWindow.draw(secondsPlayer);
                input.gameWindow.draw(centsPlayer);
            }
        }
        else {
            // There is a record
            if (lettersIntroduced != 3){
                // Draw the Enter your name indicator (substituted after by start)
                start.setString("ENTER YOUR NAME!");
                start.setPosition(input.gameWindow.getSize().x / 2.f - start.getLocalBounds().width /2.f,
                              (input.gameWindow.getSize().y / 4.5f) + 400.0f + offsetStartIndicator);
            }
            else {
                // Draw the start indicator
                start.setString("PRESS START!");
                start.setPosition(input.gameWindow.getSize().x / 2.f - start.getLocalBounds().width /2.f,
                              (input.gameWindow.getSize().y / 4.5f) + 400.0f + offsetStartIndicator);
            }

            // Draw all the player records that higher than the new record
            for (int i = 0; i <= record - 1; i++) {

                //Index
                index.setString(to_string(i + 1) + ".");
                index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                                  (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                // Score
                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                // Player name
                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                // Minutes
                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                // Seconds
                if (scoreRankingPlayer[i].secs >= 10)
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                else
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");

                secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                // Hundredths of second
                if (scoreRankingPlayer[i].cents_second >= 10)
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                else
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));

                centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));


                /*
                 * Draw the rest of menu components
                 */

                input.gameWindow.draw(index);
                input.gameWindow.draw(scorePlayer);
                input.gameWindow.draw(namePlayer);
                input.gameWindow.draw(minutesPlayer);
                input.gameWindow.draw(secondsPlayer);
                input.gameWindow.draw(centsPlayer);
            }

            // Compute the offset if there is a record to have space for the record player
            int offset = (record == 0) ? 1 : record + 1;

            // Index of the player
            index.setString(to_string(record + 1) + ".");
            index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                              (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float)offset);

            // Score of the new player
            scorePlayer.setString(to_string(score));
            scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                    (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            // New player name
            namePlayer.setString(name);
            namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                   (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            // Minutes of the new player
            minutesPlayer.setString(to_string(minutes) + "'");
            minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                      (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            // Seconds of the new player
            if (secs >= 10)
                secondsPlayer.setString(to_string(secs) + "''");
            else
                secondsPlayer.setString("0" + to_string(secs) + "''");

            secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                      (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            // Hundredths of second of the new player
            if (cents_second >= 10)
                centsPlayer.setString(to_string(cents_second));
            else
                centsPlayer.setString("0" + to_string(cents_second));

            centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                    (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);


            /*
             * Draw the menu components
             */

            input.gameWindow.draw(index);
            input.gameWindow.draw(scorePlayer);
            input.gameWindow.draw(namePlayer);
            input.gameWindow.draw(minutesPlayer);
            input.gameWindow.draw(secondsPlayer);
            input.gameWindow.draw(centsPlayer);

            // Show the rest of the runners after the new record player
            for (int i = record; i < 6 && i < (int)scoreRankingPlayer.size(); i++) {

                // Index
                index.setString(to_string(i + 2) + ".");
                index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                              (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                // Score
                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                // Player name
                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                // Minutes
                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                // Seconds
                if (scoreRankingPlayer[i].secs >= 10)
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                else
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");

                secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                // Hundredths of second
                if (scoreRankingPlayer[i].cents_second >= 10)
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                else
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));

                centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));


                /*
                 * Draw the menu components
                 */

                input.gameWindow.draw(index);
                input.gameWindow.draw(scorePlayer);
                input.gameWindow.draw(namePlayer);
                input.gameWindow.draw(minutesPlayer);
                input.gameWindow.draw(secondsPlayer);
                input.gameWindow.draw(centsPlayer);
            }
        }

        // Compute the blinking effect of the ranking menu
        elapsed4 = blinkStart.getElapsedTime().asSeconds();
        if (elapsed4 - elapsed3 >= ranking_delay.asSeconds()) {
            blink = !blink;
            blinkStart.restart();
        }

        if (blink) {
            start.setFillColor(sf::Color::Green);
            start.setOutlineColor(sf::Color::Black);
        }
        else {
            start.setFillColor(sf::Color::Transparent);
            start.setOutlineColor(sf::Color::Transparent);
        }

        input.gameWindow.draw(start);
        input.gameWindow.display();
    }

    // Stop the soundtrack
    Audio::stop(Soundtrack::LAST_WAVE);

    // Stop the menu ranking
    if (time <= 0)
        Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

    if (!escapePressed){
        // Check if the start key has not been pressed
        input.gameWindow.draw(timeCounter);
        blackShape.setPosition(0, 0);
        blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                        input.gameWindow.getSize().y));

        // Make the darkness transition
        for (int i = 0; i <= 70; i++){
            blackShape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(blackShape);
            input.gameWindow.display();
            sf::sleep(sf::milliseconds(20));
        }
    }
}



/**
 * Return the next status of the game after and option of the menu
 * has been selected by the player
 * @param input is the module that has all the configuration of the game
 */
State MenuRanking::returnMenu(Input& input){
    if (escapePressed)
        // Game closed
        return State::EXIT;
    else if (startPressed){
        // Start key has been pressed
        if (record != -1){
            // Check if there is record
            if (lettersIntroduced == 3){
                // If the name has been entered it is going to be stored
                Score s = Score(score, name, minutes, secs, cents_second);
                saveNewRecord(input, scoreRankingPlayer, s);
            }
        }
    }
    // Returns to the start menu
    return State::START;
}

