
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

#ifndef MENU_CONTROLS_H
#define MENU_CONTROLS_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"

class MenuControls : public Menu {

    private:

        sf::Texture iconBackground, textureShape;
        sf::IntRect background;

        sf::Sprite sprite;
        sf::RectangleShape shape;

        vector<Button> menuButtons;

        sf::Font fontMenu;
        sf::Text optionsText, info;

        bool controlSelected;

    public:

        MenuControls();

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

        void updateGameControl(Input& input, const sf::Keyboard::Key& key, const Key& action);

        void changeButtonSelected(const bool& menuUpPressed);

};

 #endif // MENU_CONTROLS_H

