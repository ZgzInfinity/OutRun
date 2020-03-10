/******************************************************************************
 * @file    Map.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_MAP_HPP
#define OUTRUN_MAP_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Map {
    sf::Texture bg; // Background

    // Objects
    std::vector<sf::Texture> objects;

    struct Line
    {
        float x,y,z; //3d center of line
        float X,Y,W; //screen coord
        float curve,spriteX,clip,scale;
        sf::Sprite sprite;

        Line();

        void project(int camX,int camY,int camZ, float camD, int width, int height, int rW);

        void drawSprite(sf::RenderWindow &app);
    };
    std::vector<Line> lines;

    const int roadW = 2000; // Road Width
    const int segL = 200; // Segment length

    int posY;

public:
    Map(sf::RenderWindow &w, const std::string &bgName, const std::vector<std::string> &objectNames);

    void draw(sf::RenderWindow &w, float camD,  int posX, float speed); // Relative position
};


#endif //OUTRUN_MAP_HPP
