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

#ifndef OUTRUN_SCORE_HPP
#define OUTRUN_SCORE_HPP

#include <string>
#include <vector>
#include "Config.hpp"

/**
 * Representa un récord de puntuación (score), con el nombre del jugador (name) y su marca de tiempo.
 */
struct Score {
    unsigned long score;
    std::string name;
    int minutes;
    int secs;
    int cents_second;

    /**
     * Constructor por defecto.
     * @param score
     * @param name
     * @param minutes
     * @param secs
     * @param centsSecond
     */
    Score(unsigned long score, const std::string &name, int minutes, int secs, int centsSecond);
};

/**
 * Devuelve todos los récords de puntuación ordenados de mayor a menor puntuación, es decir, el mejor récord estará en
 * la posición 0. El tamaño del vector está comprendido entre 0 (no hay ningún récord) y 7 (hay 7 récords en las
 * posiciones 0 a 6).
 * @return
 */
std::vector<Score> getGlobalScores(Config& c);

/**
 * Dados todos los récords de puntuación ordenados de mayor a menor puntuación y una puntuación score, devuelve la
 * posición del nuevo récord (0 a 6) o -1 si no es un nuevo récord.
 * @param globalScores
 * @param score
 * @return
 */
int isNewRecord(const std::vector<Score> &globalScores, unsigned long score);

/**
 * Dados todos los récords de puntuación ordenados de mayor a menor puntuación y un nuevo récord, añade el nuevo récord
 * de puntuación y devuelve true si ha sido almacenado con éxito y false en caso contrario.
 * @param globalScores
 * @param newRecord
 * @return
 */
bool saveNewRecord(Config& c, const std::vector<Score> &globalScores, const Score &newRecord);

#endif //OUTRUN_SCORE_HPP
