/******************************************************************************
 * @file    Random.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Abril 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_RANDOM_HPP
#define OUTRUN_RANDOM_HPP

#include "Vehicle.hpp"

Vehicle::Direction randomDirection();

float random_zero_one();

int random_zero_n(int n);

int random_int(int min, int max);

float random_float(float min, float max);

#endif //OUTRUN_RANDOM_HPP
