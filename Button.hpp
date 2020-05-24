/******************************************************************************
 * @file    Button.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
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
