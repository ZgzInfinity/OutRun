/******************************************************************************
 * @file    Random.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_RANDOM_HPP
#define OUTRUN_RANDOM_HPP

/**
 * Devuelve un número aleatorio entre cero y uno.
 * @return
 */
float random_zero_one();

/**
 * Devuelve un número aleatorio entre cero y n.
 * @return
 */
int random_zero_n(int n);

/**
 * Devuelve un número aleatorio entre min y max.
 * @param min
 * @param max
 * @return
 */
int random_int(int min, int max);

/**
 * Devuelve un número aleatorio entre min y max.
 * @param min
 * @param max
 * @return
 */
float random_float(float min, float max);

#endif //OUTRUN_RANDOM_HPP
