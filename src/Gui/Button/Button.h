
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


 /*
 * Module Button interface file
 */

#pragma once


#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <cstring>
#include "SFML/Graphics.hpp"


/*
 * States of the button
 */
enum class ButtonState : int {
    BUTTON_IDLE,
    BUTTON_HOVER,
    BUTTON_SELECTED,
    __COUNT
};



/**
 * Represents the buttons of the game's GUI
 */
class Button {

private:

    sf::RectangleShape shape;
    sf::Text textButton;
    ButtonState state;
    float scale;

    sf::Color idleColorButton, hoverColorButton,
    selectedColorButton, fontColorButton;

public:



    /**
     * Default constructor
     */
    Button();



    Button(float x, float y, float width, float height, sf::Font &f,
           const std::string &text, sf::Color idleColor, sf::Color hoverColor,
           const sf::Color selectedColor, const ButtonState& stateButton, float screenScale);



    void setButtonState(const ButtonState& stateButton);




    void render(sf::RenderWindow *app);



    void setTextButton(const std::string &newString);
};

#endif // BUTTON_H
