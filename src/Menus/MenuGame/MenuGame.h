
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

#ifndef MENU_GAME_H
#define MENU_GAME_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"

class MenuGame : public Menu {

    private:

        sf::Texture backgroundMenu, gameIcon, rowSelector;
        sf::Sprite mainMenu, nameGame, row;

        vector<sf::Texture> gameIcons;
        vector<sf::Sprite> nameGames;

        // Options of the main menu
        const static int ELEMENTS = 2;
        sf::Text textElements[ELEMENTS];

        sf::Font fontMenu;

    public:

        MenuGame();

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

};

#endif // MENU_GAME_H

