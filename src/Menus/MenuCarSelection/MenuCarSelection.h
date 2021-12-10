
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

#ifndef MENU_CAR_SELECTION_H
#define MENU_CAR_SELECTION_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"

class MenuCarSelection : public Menu {

    private:

        sf::Font fontMenu;
        sf::Texture backgroundMenu, garageTexture;
        sf::Sprite mainMenu, vehicleCar, shadow;

        sf::RectangleShape shape, vehicleShape;
        sf::Text titleText, carDescription;
        sf::CircleShape leftTriangle, rightTriangle;

        sf::Texture shadowTexture;
        sf::Texture vehiclePlayerTextures[TOTAL_PLAYER_CARS][TOTAL_COLOR_TEXTURES];

        int colorCarSprite;

        bool cursorLeftPressed, cursorRightPressed;

        float factorResolution;

    public:

        MenuCarSelection();

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

        int hasPlayerCarSelected();

};

#endif // MENU_CAR_SELECTION_H
