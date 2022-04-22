
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

#include "MenuRanking.h"

MenuRanking::MenuRanking(const int long long _score, const int _minutes,
                         const int _secs, const int _cents_second) : Menu()
{
    score = _score;
    minutes = _minutes;
    secs = _secs;
    cents_second = _cents_second;

    time = TIME_RANKING_MENU;
    blink = false;

    lettersIntroduced = 0;
    offsetStartText = 0;
    offsetRankingTitle = 0;
    offsetTimeCounter = 0;
    offsetScoreIndicator = 0;
    offsetPlayerIndicator = 0;
    offsetRecordIndicator = 0;
    offsetIndexIndicator = 5.5f;

    name = "_";
}

void MenuRanking::loadMenu(Input& input){

    if (input.currentIndexResolution == (int)Resolution::SCREEN_1 ||
        input.currentIndexResolution == (int)Resolution::__COUNT)
        offsetStartText = 60;
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_2)
        offsetStartText = 70;
    else if (input.currentIndexResolution == (int)Resolution::SCREEN_3)
        offsetStartText = 100;

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

    scoreRankingPlayer = getGlobalScores(input);
    record = isNewRecord(scoreRankingPlayer, score);

    backgroundMenu.loadFromFile("Resources/Menus/RankingMenu/rankingBackground.png");
    mainMenu.setTexture(backgroundMenu);
    mainMenu.setPosition(0, 0);
    mainMenu.setScale((float) input.gameWindow.getSize().x / backgroundMenu.getSize().x,
                      (float) input.gameWindow.getSize().y / backgroundMenu.getSize().y);

    fontMenu.loadFromFile("Resources/Fonts/DisposableDroid.ttf");


    rankingTitle.setFont(fontMenu);
    rankingTitle.setPosition(input.gameWindow.getSize().x / 4.f, input.gameWindow.getSize().y / 17.f - offsetRankingTitle);
    rankingTitle.setString("BEST OUTRUNNERS");
    rankingTitle.setCharacterSize(static_cast<unsigned int>(int(65.0f * input.screenScaleX)));
    rankingTitle.setFillColor(sf::Color::Yellow);
    rankingTitle.setOutlineColor(sf::Color(12, 12, 12));
    rankingTitle.setOutlineThickness(3.0f * input.screenScaleX);

    scoreIndicator.setFont(fontMenu);
    scoreIndicator.setPosition(input.gameWindow.getSize().x / 8.f, input.gameWindow.getSize().y / 6.0f - offsetScoreIndicator);
    scoreIndicator.setString("SCORE");
    scoreIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    scoreIndicator.setFillColor(sf::Color(146, 194, 186));
    scoreIndicator.setOutlineColor(sf::Color(12, 12, 12));
    scoreIndicator.setOutlineThickness(3.0f * input.screenScaleX);

    playerIndicator.setFont(fontMenu);
    playerIndicator.setPosition(input.gameWindow.getSize().x / 2.2f, input.gameWindow.getSize().y / 6.0f - offsetPlayerIndicator);
    playerIndicator.setString("NAME");
    playerIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    playerIndicator.setFillColor(sf::Color(146, 194, 186));
    playerIndicator.setOutlineColor(sf::Color(12, 12, 12));
    playerIndicator.setOutlineThickness(3.0f * input.screenScaleX);

    recordIndicator.setFont(fontMenu);
    recordIndicator.setPosition((input.gameWindow.getSize().x / 2.f) * 1.5f, input.gameWindow.getSize().y / 6.0f - offsetRecordIndicator);
    recordIndicator.setString("RECORD");
    recordIndicator.setCharacterSize(static_cast<unsigned int>(int(50.0f * input.screenScaleX)));
    recordIndicator.setFillColor(sf::Color(146, 194, 186));
    recordIndicator.setOutlineColor(sf::Color(12, 12, 12));
    recordIndicator.setOutlineThickness(3.0f * input.screenScaleX);

    timeCounter.setFont(fontMenu);
    timeCounter.setCharacterSize(static_cast<unsigned int>(int(62.0f * input.screenScaleX)));
    timeCounter.setString(to_string(time));
    timeCounter.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width,
                            input.gameWindow.getSize().y / 15.7f - offsetTimeCounter);
    timeCounter.setFillColor(sf::Color::Red);
    timeCounter.setOutlineColor(sf::Color(12, 12, 12));
    timeCounter.setOutlineThickness(3.0f * input.screenScaleX);

    scorePlayer.setFont(fontMenu);
    scorePlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    scorePlayer.setFillColor(sf::Color(146, 194, 186));
    scorePlayer.setOutlineColor(sf::Color::Black);
    scorePlayer.setOutlineThickness(3.0f * input.screenScaleX);

    namePlayer.setFont(fontMenu);
    namePlayer.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    namePlayer.setFillColor(sf::Color(146, 194, 186));
    namePlayer.setOutlineColor(sf::Color(12, 12, 12));
    namePlayer.setOutlineThickness(3.0f * input.screenScaleX);

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

    index.setFont(fontMenu);
    index.setCharacterSize(static_cast<unsigned int>(int(35.0f * input.screenScaleX)));
    index.setFillColor(sf::Color(180, 130, 211));
    index.setOutlineColor(sf::Color::Black);
    index.setOutlineThickness(3.0f * input.screenScaleX);

    start.setCharacterSize(static_cast<unsigned int>(int(40.0f * input.screenScaleX)));
    start.setFont(fontMenu);
    start.setFillColor(sf::Color::Green);
    start.setOutlineColor(sf::Color::Black);
    start.setOutlineThickness(3.0f * input.screenScaleX);
}

void MenuRanking::handleEvent(Input& input){
    sf::Event event;
    while(input.gameWindow.pollEvent(event)){
        if (input.closed(event))
            escapePressed = true;
        else if (input.pressed(Key::MENU_ACCEPT, event) && input.held(Key::MENU_ACCEPT)){
            if (record == -1 || (record != -1 && lettersIntroduced == 3)){
                Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                startPressed = true;
            }
        }
        else if (input.isKeypressedEvent(event)){
            if (lettersIntroduced < 3 && input.pressedKeyIsValidLetter(event)){
                lettersIntroduced++;
                string keyLetter = input.getKeyCodeName(event.key.code);
                if (name == "_") {
                    name = keyLetter;
                    Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                }
                else {
                    if (lettersIntroduced == 3) {
                        name = name.substr(0, name.size() - 1);
                        name += keyLetter;
                        Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);
                    }
                    else {
                        name += keyLetter + "_";
                        Audio::play(Sfx::MENU_SELECTION_CHOOSE, false);
                    }
                }
            }
            else
                Audio::play(Sfx::MENU_SELECTION_WRONG, false);
        }
    }
}

void MenuRanking::draw(Input& input){

    rankingTime.restart();

    elapsed1 = rankingTime.getElapsedTime().asSeconds();
    elapsed3 = blinkStart.getElapsedTime().asSeconds();

    Audio::play(Soundtrack::LAST_WAVE, true);

    if (record != 1)
        Audio::play(Sfx::HALL_FAME, false);

    while (time > 0 && !escapePressed && !startPressed){

        handleEvent(input);

        // Get the actual time
        elapsed2 = rankingTime.getElapsedTime().asSeconds();

        // Check if a second has passed between both timestamps
        if (elapsed2 - elapsed1 >= ranking_delay.asSeconds()) {
            // Draw time
            time--;
            rankingTime.restart();
            timeCounter.setString(to_string(time));
            timeCounter.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - timeCounter.getLocalBounds().width,
                                    input.gameWindow.getSize().y / 15.7f - offsetRankingTitle);
        }

        input.gameWindow.clear();
        input.gameWindow.draw(mainMenu);
        input.gameWindow.draw(rankingTitle);
        input.gameWindow.draw(timeCounter);
        input.gameWindow.draw(scoreIndicator);
        input.gameWindow.draw(playerIndicator);
        input.gameWindow.draw(recordIndicator);

        // There is no new record
        if (record == -1) {

            start.setString("PRESS START!");
            start.setPosition(input.gameWindow.getSize().x / 2.f - start.getLocalBounds().width /2.f,
                              (input.gameWindow.getSize().y / 4.5f) + 400.0f + offsetStartText);

            // There is not a new record
            for (int i = 1; i <= 7; i++) {

                index.setString(to_string(i) + ".");
                index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                                  (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                scorePlayer.setString(to_string(scoreRankingPlayer[i - 1].score));
                scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                namePlayer.setString(scoreRankingPlayer[i - 1].name);
                namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i - 1].minutes) + "'");
                minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                if (scoreRankingPlayer[i - 1].secs >= 10)
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i - 1].secs) + "''");
                else
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i - 1].secs) + "''");

                secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));


                if (scoreRankingPlayer[i - 1].cents_second >= 10)
                    centsPlayer.setString(to_string(scoreRankingPlayer[i - 1].cents_second));
                else
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i - 1].cents_second));

                centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * float(i));

                input.gameWindow.draw(index);
                input.gameWindow.draw(scorePlayer);
                input.gameWindow.draw(namePlayer);
                input.gameWindow.draw(minutesPlayer);
                input.gameWindow.draw(secondsPlayer);
                input.gameWindow.draw(centsPlayer);
            }
        }
        else {

            if (lettersIntroduced != 3) {
                start.setString("ENTER YOUR NAME!");
                start.setPosition(input.gameWindow.getSize().x / 2.f - start.getLocalBounds().width /2.f,
                              (input.gameWindow.getSize().y / 4.5f) + 400.0f + offsetStartText);
            }
            else {
                start.setString("PRESS START!");
                start.setPosition(input.gameWindow.getSize().x / 2.f - start.getLocalBounds().width /2.f,
                              (input.gameWindow.getSize().y / 4.5f) + 400.0f + offsetStartText);
            }

            for (int i = 0; i <= record - 1; i++) {

                index.setString(to_string(i + 1) + ".");
                index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                                  (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                if (scoreRankingPlayer[i].secs >= 10)
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                else
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");

                secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));


                if (scoreRankingPlayer[i].cents_second >= 10)
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                else
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));

                centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 1));

                input.gameWindow.draw(index);
                input.gameWindow.draw(scorePlayer);
                input.gameWindow.draw(namePlayer);
                input.gameWindow.draw(minutesPlayer);
                input.gameWindow.draw(secondsPlayer);
                input.gameWindow.draw(centsPlayer);
            }

            int offset = (record == 0) ? 1 : record + 1;

            index.setString(to_string(record + 1) + ".");
            index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                              (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float)offset);

            // Show the actual player
            scorePlayer.setString(to_string(score));
            scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                    (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            namePlayer.setString(name);
            namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                   (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            minutesPlayer.setString(to_string(minutes) + "'");
            minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                      (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            if (secs >= 10)
                secondsPlayer.setString(to_string(secs) + "''");
            else
                secondsPlayer.setString("0" + to_string(secs) + "''");

            secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                      (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);


            if (cents_second >= 10)
                centsPlayer.setString(to_string(cents_second));
            else
                centsPlayer.setString("0" + to_string(cents_second));

            centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                    (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) offset);

            input.gameWindow.draw(index);
            input.gameWindow.draw(scorePlayer);
            input.gameWindow.draw(namePlayer);
            input.gameWindow.draw(minutesPlayer);
            input.gameWindow.draw(secondsPlayer);
            input.gameWindow.draw(centsPlayer);

            // Show the rest of out runners

            for (int i = record; i < 6 && i < (int)scoreRankingPlayer.size(); i++) {

                index.setString(to_string(i + 2) + ".");
                index.setPosition((input.gameWindow.getSize().x / 13.f) - index.getLocalBounds().width,
                              (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                scorePlayer.setString(to_string(scoreRankingPlayer[i].score));
                scorePlayer.setPosition((input.gameWindow.getSize().x / 3.9f) - scorePlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                namePlayer.setString(scoreRankingPlayer[i].name);
                namePlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.13f - namePlayer.getLocalBounds().width,
                                       (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                minutesPlayer.setString(to_string(scoreRankingPlayer[i].minutes) + "'");
                minutesPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.57f - minutesPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                if (scoreRankingPlayer[i].secs >= 10)
                    secondsPlayer.setString(to_string(scoreRankingPlayer[i].secs) + "''");
                else
                    secondsPlayer.setString("0" + to_string(scoreRankingPlayer[i].secs) + "''");

                secondsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.7f - secondsPlayer.getLocalBounds().width,
                                          (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));


                if (scoreRankingPlayer[i].cents_second >= 10)
                    centsPlayer.setString(to_string(scoreRankingPlayer[i].cents_second));
                else
                    centsPlayer.setString("0" + to_string(scoreRankingPlayer[i].cents_second));

                centsPlayer.setPosition((input.gameWindow.getSize().x / 2.f) * 1.8f - centsPlayer.getLocalBounds().width,
                                        (input.gameWindow.getSize().y / offsetIndexIndicator) + 50.0f * input.screenScaleX * (float) (i + 2));

                input.gameWindow.draw(index);
                input.gameWindow.draw(scorePlayer);
                input.gameWindow.draw(namePlayer);
                input.gameWindow.draw(minutesPlayer);
                input.gameWindow.draw(secondsPlayer);
                input.gameWindow.draw(centsPlayer);
            }
        }

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

    Audio::stop(Soundtrack::LAST_WAVE);

    if (time <= 0)
        Audio::play(Sfx::MENU_SELECTION_CONFIRM, false);

    if (!escapePressed){
        input.gameWindow.draw(timeCounter);
        blackShape.setPosition(0, 0);
        blackShape.setSize(sf::Vector2f(input.gameWindow.getSize().x,
                                        input.gameWindow.getSize().y));

        for (int i = 0; i <= 70; i++){
            blackShape.setFillColor(sf::Color(0, 0, 0, i));
            input.gameWindow.draw(blackShape);
            input.gameWindow.display();
            sf::sleep(sf::milliseconds(20));
        }
    }
}


State MenuRanking::returnMenu(Input& input){
    if (escapePressed)
        return State::EXIT;
    else if (startPressed){
        if (record != -1) {
            if (lettersIntroduced == 3){
                Score s = Score(score, name, minutes, secs, cents_second);
                saveNewRecord(input, scoreRankingPlayer, s);
            }
        }
    }
    return State::START;
}

