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
    State state = ANIMATION;

    while (c.w.isOpen() && state != EXIT) {
        Game engine(c);

        if (state == START)
            state = startMenu(c);

        while (c.w.isOpen() && state != START) {
            switch (state) {
                case ANIMATION: {
                    state = introAnimation(c);
                    break;
                }
                case MUSIC: {
                    state = selectMusicSoundtrack(c);
                    break;
                }
                case OPTIONS: {
                    bool inGame = engine.isInGame();
                    state = optionsMenu(c, inGame);
                    engine.checkDifficulty(c);
                    break;
                }
                case GAME: {
                    state = engine.play(c);
                    break;
                }
                case RANKING: {
                    unsigned long scorePlayer = engine.getScore();
                    int minutes = engine.getMinutesTrip();
                    int secs = engine.getSecsTrip();
                    int cents_Second = engine.getCents_SecondTrip();
;                   state = rankingMenu(c, scorePlayer, minutes, secs, cents_Second);
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
}
