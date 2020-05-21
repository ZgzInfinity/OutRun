

#ifndef BUTTON_H
#define BUTTON_H


#include <iostream>
#include <cstring>
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

enum button_states {
    BUTTON_IDLE = 0,
    BUTTON_PRESSED,
    BUTTON_HOVER,
};

class Button {
    private:
        RectangleShape shape;
        Font fontButton;
        Text textButton;

        Color idleColorButton;
        Color hoverColorButton;
        Color activeColorButton;

        bool hover, pressed;
        int buttonState;

    public:

        const bool isPressed() const;

        const bool isHovered() const;

        Button();

        Button (float x, float y, float width, float height, Font& f, string text,
                Color idleColor, Color hoverColor, Color activeColor, int initialState, float screenScale);

        void getPressed (Button& otherButton);

        void setButtonState(button_states buttonState);

        void render (RenderTexture* app);

        void setTextButton(string newString);

        string getTextButton();

};


#endif // BUTTON_H
