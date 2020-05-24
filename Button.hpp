/******************************************************************************
 * @file    Button.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

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

    Button();

    Button(float x, float y, float width, float height, sf::Font &f, const std::string &text,
           sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int initialState, float screenScale);

    void setButtonState(button_states buttonState);

    void render(sf::RenderTexture *app);

    void setTextButton(const std::string &newString);

};


#endif // BUTTON_H
