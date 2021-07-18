
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

#ifndef MENU_GEARS_H
#define MENU_GEARS_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"

class MenuGears : public Menu {

    private:

        sf::Sprite bufferSprite;

        sf::Font fontMenu;
        sf::Texture backgroundMenu, gameIcon;
        sf::Sprite mainMenu, nameGame;

        sf::RectangleShape shape, blackShape;
        sf::Text titleText, totalTime;

        vector<sf::Texture> gameIcons;
        vector<sf::Sprite> nameGames;

        vector<sf::Texture> gearIcons;
        vector<sf::Sprite> gears;

        float offsetY;
        float offsetTitleText;
        float offsetTimeLapTexts;
        bool mainTextArrived;
        bool lapTextsArrived;
        bool carSpriteArrived;
        bool automaticMode;
        bool pressedKey;

    public:

        MenuGears();

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

        bool getAutomaticMode() const;

};

#endif // MENU_GEARS_H

