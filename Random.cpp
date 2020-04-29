/******************************************************************************
 * @file    Random.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Abril 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Random.hpp"
#include <random>

using namespace std;

static thread_local random_device rd;
static thread_local std::mt19937 generator(rd());
static thread_local uniform_real_distribution<float> dist(0.0f, 1.0f);

Vehicle::Direction randomDirection() {
    const float p = dist(generator);
    if (p <= 0.6f)
        return Vehicle::Direction::RIGHT;
    else if (p <= 0.8f)
        return Vehicle::Direction::TURNRIGHT;
    else
        return Vehicle::Direction::TURNLEFT;
}

float random_zero_one() {
    return dist(generator);
}

int random_zero_n(int n) {
    return uniform_int_distribution<>(0, n)(generator);
}

float random_float(float min, float max) {
    return uniform_real_distribution<float>(min, max)(generator);
}

