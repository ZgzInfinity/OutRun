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
#include "Menu.hpp"

class Map {
    sf::Texture bg; // Background

    // Objects
    std::vector<sf::Texture> objects;

    struct Line
    {
        float x,y,z; //3d center of line
        float X{},Y{},W{}; //screen coord
        float curve,spriteX,clip{},scale{},offset{};
        int spriteNum{};
        bool left{};

        Line();

        void project(float camX,float camY,float camZ, float camD, float width, float height, float rW);

        void drawSprite(sf::RenderWindow &w, const std::vector<sf::Texture> &objs);
    };
    std::vector<Line> lines;

    float posY;

public:
    Map(sf::RenderWindow &w, const std::string &bgName, const std::vector<std::string> &objectNames);

    void draw(Config &c, float camD,  float posX, float speed); // Relative position
};


#endif //OUTRUN_MAP_HPP
