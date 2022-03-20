
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

#include <atomic>
#include <SFML/Graphics.hpp>
#include "../Menu/Menu.h"
#include "../../Globals.h"
#include "../../Gui/Button/Button.h"

class MenuLoading : public Menu {

    private:

        sf::Texture iconBackground, textureShape;
        sf::IntRect background;

        sf::Sprite sprite;
        sf::RectangleShape shape;

        vector<Button> menuButtons;
        vector<string> points;

        sf::Font fontMenu;
        sf::Text controlsText, loadingText, pointText;

        int totalPoints, offsetText;

        bool automaticMode;

    public:

        MenuLoading(const bool& _automaticMode);

        void loadMenu(Input& input) override;

        void handleEvent(Input& input) override;

        void draw(Input& input) override;

        State returnMenu(Input& input) override;

};


