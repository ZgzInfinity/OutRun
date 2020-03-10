/******************************************************************************
 * @file    main.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Game.hpp"

using namespace sf;
using namespace std;

int main() {
    Config c;
    Game engine(c);
    State state = START;

    while (c.w.isOpen()) {
        switch (state) {
            case START: {
                state = startMenu(c);
                break;
            }
            case GAME: {
                state = engine.play(c);
                break;
            }
            case PAUSE: {
                state = pauseMenu(c);
                break;
            }
            case END: {
                state = endMenu(c);
                break;
            }
            default: {
                c.w.close();
                break;
            }
        }
    }
}