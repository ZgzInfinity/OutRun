
#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

struct Interface {

    // HUD
    Text sText;

    // Vector of textures for represent the game panel
    vector<Texture> textures;

    // Vector of sprites for represent the game panel
    vector<Sprite> sprites;

    // Available time to arrive to the next checkpoint
    Text timeToPlay;

    // Score of the player displayed in the panel
    Text textScore;

    // Time of lap consumed
    Text textLap;

    // Level indicator
    Text textLevel;

    // Game over text
    Text gameOver;

    // Time inverted by the player for complete the game
    Text textForLap;

    // CheckPoint title
    Text checkPoint;

    // Final score after completing all the levels
    Text bonification;

    // Seconds arrival indicator
    Text secondsIndicator;

    // Seconds arrival indicator
    Text crossSign;

    // Seconds arrival indicator
    Text pointsIndicator;

    // Score factor to multiply
    Text scoreMultiply;

    // Time bonus to the player
    Text timeBonus;

    // Tree map with levels
    Texture treeMap[5][5];

    // Tree map with levels
    Sprite spriteMap[5][5];

    // Record lap of the pl
    Sprite recordLap;

    /*
     * Constructor of the interface
     */
    Interface();

};

#endif
