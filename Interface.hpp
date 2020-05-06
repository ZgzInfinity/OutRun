
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


    /*
     * Constructor of the interface
     */
    Interface();

};

#endif
