
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

#ifndef MENU_OPTIONS_H
#define MENU_OPTIONS_H

#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"
#include "../../Gui/Huds/HudRound/HudRound.h"
#include "../../Logger/Logger.h"

class MenuOptions : public Menu {

    private:

        sf::Texture iconBackground, textureShape;
        sf::IntRect background;

        sf::Sprite sprite;
        sf::RectangleShape shape;

        vector<Button> menuButtons;

        sf::Font fontMenu;
        sf::Text optionsText, info;

        string difficulty, traffic, subMenu, access, saved;

        bool resized;
        bool controlSelected;
        bool pauseMode;

    public:

        MenuOptions();

        setPauseMode(const bool& pause);

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

        void changeButtonSelected(const bool& menuUpPressed);

        void changeDifficulty(Input& input, const sf::Event& event);

        void changeTrafficLevel(Input& input, const sf::Event& event);

        void changeResolution(Input& input, const sf::Event& event);

};

#endif // MENU_OPTIONS_H
