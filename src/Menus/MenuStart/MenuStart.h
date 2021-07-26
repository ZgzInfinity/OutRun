
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

#ifndef MENU_START_H
#define MENU_START_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"

class MenuStart : public Menu {

    private:

        sf::Texture backgroundMenu, gameIcon, rowSelector;
        sf::Sprite mainMenu, nameGame;

        vector<sf::Texture> gameIcons;
        vector<sf::Sprite> nameGames;

        // Options of the main menu
        const static int ELEMENTS = 7;
        sf::Text textElements[ELEMENTS];

        sf::Font fontMenu;
        sf::RectangleShape blackShape;

        float elapsedBlink, elapsedHide;
        sf::Clock blinkClcok;
        bool blink;
        bool firstLoad;
        bool pauseMode;
        bool newGame;

    public:

        MenuStart();

        void setMenuStart(const bool& first, const bool& pause, const bool& newRound);

        bool getFirstLoad() const;

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

};

#endif // MENU_START_H

