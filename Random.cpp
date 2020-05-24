/******************************************************************************
 * @file    Random.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Random.hpp"
#include <random>

using namespace std;

static thread_local random_device rd;
static thread_local std::mt19937 generator(rd());
static thread_local uniform_real_distribution<float> dist(0.0f, 1.0f);

float random_zero_one() {
    return dist(generator);
}

int random_zero_n(int n) {
    return uniform_int_distribution<>(0, n)(generator);
}

int random_int(int min, int max) {
    return uniform_int_distribution<int>(min, max)(generator);
}

float random_float(float min, float max) {
    return uniform_real_distribution<float>(min, max)(generator);
}

