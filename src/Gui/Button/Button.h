
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
 * Interface file of the module Audio
 */

#pragma once


#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <cstring>
#include "SFML/Graphics.hpp"



/**
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

        // Rectangle of the button
        sf::RectangleShape shape;

        // Text of the button
        sf::Text textButton;

        // Status of the button
        ButtonState state;


        float scale;

        // Color of the button when is not hovered and not selected
        sf::Color idleColorButton;

        // Color of the button when it is hovered
        sf::Color hoverColorButton;

        // Color of the button when it is selected
        sf::Color selectedColorButton;

        // Color of the text of the button
        sf::Color fontColorButton;

    public:



        /**
         * Default constructor
         */
        Button();



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
        Button(const float _posX, const float _posY, const float _width, const float _height, sf::Font& _fontText,
               const std::string _text, const sf::Color _idleColor, const sf::Color _hoverColor,
               const sf::Color _selectedColor, const ButtonState& _stateButton, const float _screenScale);



        /**
         * Updates the state of the button
         * @param buttonState is the new state of the button to change
         */
        void setButtonState(const ButtonState& stateButton);



        /**
         * Draws the button
         * @param app is the console window of the game
         */
        void render(sf::RenderWindow *app);



        /**
         * Initialize the text content of the button
         * @param newString is the content text of the button
         */
        void setTextButton(const std::string newString);
};

#endif // BUTTON_H
