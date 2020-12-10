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

#ifndef OUTRUN_BUTTON_H
#define OUTRUN_BUTTON_H

#include <iostream>
#include <cstring>
#include "SFML/Graphics.hpp"


using namespace sf;
using namespace std;

/*
 * States of the button
 */
enum button_states {
    // Normal state of the button
    BUTTON_IDLE = 0,
    // Button hovered with the mouse or with the row cursor keywords
    BUTTON_HOVER,
};



/**
 * Represents the buttons of the game's GUI
 */
class Button {

private:

    // Rectangle of the button
    RectangleShape shape;
    // Text content of the button
    Text textButton;

    // Colors of the button
    Color idleColorButton, hoverColorButton, fontColorButton;

public:



    /**
     * Default constructor
     */
    Button();



    /**
     * Constructor of the button
     * @param x is the coordinate in axis x of the button in the screen
     * @param y is the coordinate in axis y of the button in the screen
     * @param width is the width dimension of the button
     * @param height is the height dimension of the button
     * @param f is the font of the text
     * @param text is the content of the button
     * @param idleColor is the normal color of the button
     * @param hoverColor is the color of the button hovered
     * @param initialState is the state code of the button
     * @param screenScale is the factor of resolution of the screen
     */
    Button(float x, float y, float width, float height, sf::Font &f, const std::string &text,
           Color idleColor, Color hoverColor, int initialState, float screenScale);



    /**
     * Updates the state of the button
     * @param buttonState is the new state of the button to change
     */
    void setButtonState(button_states stateButton);



    /**
     * Draws the button
     * @param app is the console window of the game
     */
    void render(RenderTexture *app);



    /**
     * Initialize the text content of the button
     * @param newString is the content text of the button
     */
    void setTextButton(const string &newString);
};


#endif // OUTRUN_BUTTON_H
