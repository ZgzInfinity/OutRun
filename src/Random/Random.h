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



 /*
 * Module Random interface file
 */

#ifndef OUTRUN_RANDOM_HPP
#define OUTRUN_RANDOM_HPP



/**
 * Returns a random number between zero and one
 * @return
 */
float random_zero_one();



/**
 * Returns a random number between zero and n
 * @param n is the upper bound number to generate the random number
 * @return
 */
int random_zero_n(int n);



/**
 * Returns a integer random number between a lower bound and an upper bound numbers
 * @param min is the lower bound number of the interval
 * @param max is the upper bound number of the interval
 * @return
 */
int random_int(int min, int max);



/**
 * Returns a float random number between a lower bound and an upper bound numbers
 * @param min is the lower bound number of the interval
 * @param max is the upper bound number of the interval
 * @return
 */
float random_float(float min, float max);


#endif //OUTRUN_RANDOM_HPP
