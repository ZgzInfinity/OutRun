/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
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

#ifndef KEYWORD_MAPPER_H
#define KEYWORD_MAPPER_H

#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>

const int KEYWORDS_MAPPED = 75;

using namespace std;
using namespace sf;

struct KeywordMapper {


    // Maps of the keywords available in the game
    sf::Keyboard::Key mapperCodeKeyWord[KEYWORDS_MAPPED];
    std::string mapperIdKeyWord[KEYWORDS_MAPPED];



    /**
     * Constructor por defecto.
     */
    KeywordMapper();



    /**
     * Returns the numeric code of key identified by its name
     * if it exists. Otherwise returns a negative number
     * @param key is a key identified by its name
     */
    int lookForKeyBoard(const string key);



    /**
     * Returns the numeric code of key identified by its name
     * if it exists. Otherwise returns a negative number
     * @param key is a key identified by its name
     */
    int lookForKeyBoardId(const Keyboard::Key key);

};

#endif // KEYWORD_MAPPER_H
