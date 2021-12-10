

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

#pragma once

#ifndef MENU_RANKING_H
#define MENU_RANKING_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Score/Score.h"

class MenuRanking : public Menu {

    private:

        sf::Texture backgroundMenu;
        sf::Sprite mainMenu;

        sf::Clock rankingTime, blinkStart;

        sf::RectangleShape blackShape;

        const sf::Time ranking_delay = sf::seconds(1.f);

        sf::Text rankingTitle, scoreIndicator, playerIndicator, recordIndicator,
                 timeCounter, scorePlayer, namePlayer, minutesPlayer, secondsPlayer,
                 centsPlayer, index, start;

        int time, offsetStartText, offsetRankingTitle, offsetTimeCounter, offsetScoreIndicator,
            offsetPlayerIndicator, offsetRecordIndicator, offsetIndexIndicator;

        float elapsed1, elapsed2, elapsed3, elapsed4;

        std::vector<Score> scoreRankingPlayer;

        int record;

        bool blink;

        int lettersIntroduced;

        std::string name;

        sf::Font fontMenu;

        int long long score;

        int minutes, secs, cents_second;

    public:

        MenuRanking(const int long long _score, const int _minutes, const int _secs, const int _cents_second);

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

};

#endif // MENU_RANKING_H

