/******************************************************************************
 * @file    Game.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef KEYWORD_MAPPER_H
#define KEYWORD_MAPPER_H

#include <iostream>
#include <SFML/Graphics.hpp>

const int KEYWORDS_MAPPED = 75;

using namespace std;
using namespace sf;

struct KeywordMapper {

    // Maps of the keywords available in the game
    Keyboard::Key mapperCodeKeyWord[KEYWORDS_MAPPED];
    string mapperIdKeyWord[KEYWORDS_MAPPED];

    KeywordMapper();

    int lookForIdKeyWord(const Keyboard::Key codeKey);
};

#endif // KEYWORD_MAPPER_H
