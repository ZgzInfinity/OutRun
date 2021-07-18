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
 * Module Button implementation file
 */

#include "Button.h"


using namespace std;
using namespace sf;



/**
 * Default constructor
 */
Button::Button(){}



Button::Button(float x, float y, float width, float height, Font &f,
               const string &text, Color idleColor, Color hoverColor,
               const sf::Color selectedColor, const ButtonState& stateButton, float screenScale)
{
    shape.setPosition(Vector2f(x, y));
    shape.setSize(Vector2f(width, height));
    shape.setOutlineColor(Color::Black);
    shape.setOutlineThickness(3.0f * screenScale);
    textButton.setString(text);
    textButton.setFont(f);
    textButton.setFillColor(Color::Blue);
    textButton.setCharacterSize(static_cast<unsigned int>(int(27.0f * screenScale)));

    idleColorButton = idleColor;
    hoverColorButton = hoverColor;
    selectedColorButton = selectedColor;
    setTextButton(text);
    setButtonState(stateButton);
    scale = screenScale;
}




void Button::setButtonState(const ButtonState& stateButton){
    state = stateButton;
    switch (state) {
        case ButtonState::BUTTON_IDLE:
            shape.setFillColor(idleColorButton);
            break;
        case ButtonState::BUTTON_HOVER:
            shape.setFillColor(hoverColorButton);
            break;
        case ButtonState::BUTTON_SELECTED:
            shape.setFillColor(selectedColorButton);
    }
}



void Button::render(sf::RenderWindow *app){
    app->draw(shape);
    app->draw(textButton);
}


void Button::setTextButton(const std::string &newString){
    textButton.setString(newString);
    textButton.setPosition(
        shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - textButton.getGlobalBounds().width / 2.f,
        shape.getPosition().y - (shape.getGlobalBounds().height / 2.f) + textButton.getGlobalBounds().height
    );
}
