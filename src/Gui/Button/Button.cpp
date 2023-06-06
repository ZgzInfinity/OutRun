
/*
 * Copyright (c) 2023 Ruben Rodriguez
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
 * Implementation file of the module Button
 */

#include "Button.h"


using namespace std;
using namespace sf;



/**
 * Default constructor
 */
Button::Button(){}



/**
 * Constructor of the button
 * @param _posX is the coordinate in axis x of the button in the screen
 * @param _posY is the coordinate in axis y of the button in the screen
 * @param _width is the width dimension of the button
 * @param _height is the height dimension of the button
 * @param _fontText is the font of the text
 * @param _text is the content of the button
 * @param _idleColor is the normal color of the button
 * @param _hoverColor is the color of the button hovered
 * @param _activeColor is the color of the button pressed
 * @param _initialState is the state code of the button
 * @param _screenScale is the factor of resolution of the screen
 */
Button::Button(const float _posX, const float _posY, const float _width, const float _height, sf::Font& _fontText,
               const std::string _text, const sf::Color _idleColor, const sf::Color _hoverColor,
               const sf::Color _selectedColor, const ButtonState& _stateButton, const float _screenScale)
{
    // Assign all the information to the new button
    shape.setPosition(Vector2f(_posX, _posY));
    shape.setSize(Vector2f(_width, _height));
    shape.setOutlineColor(Color::Black);
    shape.setOutlineThickness(3.0f * _screenScale);
    textButton.setString(_text);
    textButton.setFont(_fontText);
    textButton.setFillColor(Color::Blue);
    textButton.setCharacterSize(static_cast<unsigned int>(int(27.0f * _screenScale)));

    idleColorButton = _idleColor;
    hoverColorButton = _hoverColor;
    selectedColorButton = _selectedColor;
    setTextButton(_text);
    setButtonState(_stateButton);
    scale = _screenScale;
}



/**
 * Updates the state of the button
 * @param buttonState is the new state of the button to change
 */
void Button::setButtonState(const ButtonState& stateButton){
    // Change the status of the button and the color
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



/**
 * Draws the button
 * @param app is the console window of the game
 */
void Button::render(sf::RenderWindow *app){
    app->draw(shape);
    app->draw(textButton);
}


/**
 * Initialize the text content of the button
 * @param newString is the content text of the button
 */
void Button::setTextButton(const std::string newString){
    // Update the text content of the button in the center of it
    textButton.setString(newString);
    textButton.setPosition(
        shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - textButton.getGlobalBounds().width / 2.f,
        shape.getPosition().y - (shape.getGlobalBounds().height / 2.f) + textButton.getGlobalBounds().height
    );
}
