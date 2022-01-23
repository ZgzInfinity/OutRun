
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

#ifndef MENU_LOGO_H
#define MENU_LOGO_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Input/Input.h"

class MenuLogo : public Menu {

    private:

        sf::RectangleShape backgroundMenu;

        sf::Texture logoGame;

        sf::Sprite iconGame;

        sf::Font fontMenu;

        sf::Text logoZgz, logoInfinityGames, logoZgzCompleted;

        float offsetY, offsetY2;

    public:

        MenuLogo();

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

};

#endif // MENU_LOGO_H

