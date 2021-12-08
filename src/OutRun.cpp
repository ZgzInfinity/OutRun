
/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>

using namespace std;

#include "Input/Input.h"
#include "Game/Game.h"

int main(){

    Input input;

    Game g = Game(input);

    g.run(input);

    /*
    sf::Image image;
    std::vector <sf::Image> images;

    int i;
    std::string path = "Resources/Vehicles/Ferrari/c";

    for (int i = 1; i <= 148; i++){
        if (!image.loadFromFile(path + to_string(i) + ".png"))
            return -1;
        else
            images.push_back(image);
    }

    int j = 1;

    for (sf::Image image : images){
        for (int i = 0; i <= image.getSize().x; i++){
            for (int j = 0; j <= image.getSize().y; j++){
                if (image.getPixel(i, j) == sf::Color(132, 0, 0)){
                    image.setPixel(i, j, sf::Color(0, 88, 104));
                }
                else if (image.getPixel(i, j) == sf::Color(164, 49, 49)){
                    image.setPixel(i, j, sf::Color(32, 104, 120));
                }
                else if (image.getPixel(i, j) == sf::Color(197, 82, 82)){
                    image.setPixel(i, j, sf::Color(64, 152, 168));
                }
                else if (image.getPixel(i, j) == sf::Color(214, 99, 99)){
                    image.setPixel(i, j, sf::Color(80, 168, 184));
                }
                else if (image.getPixel(i, j) == sf::Color(214, 132, 132)){
                    image.setPixel(i, j, sf::Color(112, 176, 184));
                }
                else if (image.getPixel(i, j) == sf::Color(214, 164, 164)){
                    image.setPixel(i, j, sf::Color(144, 184, 192));
                }
                else if (image.getPixel(i, j) == sf::Color(131, 0, 0)){
                    image.setPixel(i, j, sf::Color(0, 88, 104));
                }
                else if (image.getPixel(i, j) == sf::Color(213, 98, 98)){
                    image.setPixel(i, j, sf::Color(80, 168, 184));
                }
                else if (image.getPixel(i, j) == sf::Color(213, 164, 164)){
                    image.setPixel(i, j, sf::Color(144, 184, 192));
                }
                else if (image.getPixel(i, j) == sf::Color(213, 131, 131)){
                    image.setPixel(i, j, sf::Color(112, 176, 184));
                }
            }
        }

        if (!image.saveToFile("Prueba/c" + to_string(j) + ".png"))
            return -1;

        j++;
    }
    */

    return 0;
}
