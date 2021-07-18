

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

#ifndef MENU_CREDITS_H
#define MENU_CREDITS_H

#include <SFML/Graphics.hpp>
#include "Credit.h"
#include "../Menu/Menu.h"

class MenuCredits : public Menu {

    private:

        sf::Font fontMenu;
        sf::Texture backgroundMenu, gameIcon;
        sf::Sprite mainMenu, nameGame;

        sf::RectangleShape shape, blackShape;
        sf::Text titleText, gameDetailText, right, info;

        vector<sf::Texture> gameIcons;
        vector<sf::Sprite> nameGames;

        float offsetY;
        float offsetTitleText;
        float offsetTimeLapTexts;
        bool mainTextArrived;
        bool lapTextsArrived;

    public:

        MenuCredits();

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

};

#endif // MENU_CREDITS_H

