/******************************************************************************
 * @file    Button.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Button.hpp"

using namespace std;
using namespace sf;

Button::Button() = default;

Button::Button(float x, float y, float width, float height, Font &f,
               const string &text, Color idleColor, Color hoverColor, Color activeColor, int initialState,
               float screenScale) {
    shape.setPosition(Vector2f(x, y));
    shape.setSize(Vector2f(width, height));
    shape.setOutlineColor(Color::Black);
    shape.setOutlineThickness(3.0f * screenScale);
    textButton.setString(text);
    textButton.setFont(f);
    textButton.setFillColor(Color::Blue);
    textButton.setCharacterSize(static_cast<unsigned int>(int(12.0f * screenScale)));
    textButton.setPosition(
            shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - textButton.getGlobalBounds().width / 2.f,
            shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - textButton.getGlobalBounds().height / 2.f -
            5
    );

    // Store the possible colors of the button
    idleColorButton = idleColor;
    hoverColorButton = hoverColor;
    activeColorButton = activeColor;

    // Check the initial state of the button
    switch (initialState) {
        case 0:
            shape.setFillColor(idleColorButton);
            break;
        case 1:
            shape.setFillColor(hoverColorButton);
            break;
        case 2:
            shape.setFillColor(activeColorButton);
            break;
        default:
            break;
    }
}


void Button::setButtonState(button_states stateButton) {
    switch (stateButton) {
        case BUTTON_IDLE:
            shape.setFillColor(idleColorButton);
            break;
        case BUTTON_HOVER:
            shape.setFillColor(hoverColorButton);
            break;
        case BUTTON_PRESSED:
            shape.setFillColor(activeColorButton);
    }
}


void Button::render(RenderTexture *app) {
    app->draw(shape);
    app->draw(textButton);
}


void Button::setTextButton(const string &newString) {
    textButton.setString(newString);
    textButton.setPosition(
            shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - textButton.getGlobalBounds().width / 2.f,
            shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - textButton.getGlobalBounds().height / 2.f -
            5
    );
}


