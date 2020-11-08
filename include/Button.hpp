/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
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

#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <cstring>
#include "SFML/Graphics.hpp"

enum button_states {
    BUTTON_IDLE = 0,
    BUTTON_PRESSED,
    BUTTON_HOVER,
};

class Button {
private:
    sf::RectangleShape shape;
    sf::Text textButton;

    sf::Color idleColorButton;
    sf::Color hoverColorButton;
    sf::Color activeColorButton;

public:

    /**
     * Constructor por defecto.
     */
    Button();

    /**
     * Constructor del botón.
     * @param x
     * @param y
     * @param width
     * @param height
     * @param f
     * @param text
     * @param idleColor
     * @param hoverColor
     * @param activeColor
     * @param initialState
     * @param screenScale
     */
    Button(float x, float y, float width, float height, sf::Font &f, const std::string &text,
           sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int initialState, float screenScale);

    /**
     * Determina el estado del botón.
     * @param buttonState
     */
    void setButtonState(button_states buttonState);

    /**
     * Dibuja el botón.
     * @param app
     */
    void render(sf::RenderTexture *app);

    /**
     * Establece el texto del botón.
     * @param newString
     */
    void setTextButton(const std::string &newString);
};


#endif // BUTTON_H
