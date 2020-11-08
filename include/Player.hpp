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

#ifndef OUTRUN_PLAYER_HPP
#define OUTRUN_PLAYER_HPP

#include "Vehicle.hpp"

class Player : public Vehicle {
    const float speedMul, maxAcc, accInc, scaleY;
    float acceleration, minCrashAcc, xDest;
    int inertia;

    sf::Sprite sprite;
    bool crashing; // True if crashing state is on
    bool smoking; // True if player generates smoke
    bool skidding; // True if player is skidding

    bool firstCrash, firstTurnLeft, firstTurnRight;

public:
    /**
     * Inicializa el vehículo del jugador.
     * @param maxSpeed
     * @param speedMul multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
     * @param accInc incremento de la aceleración
     * @param scaleX escalado del sprite del vehículo
     * @param scaleY escalado del sprite del vehículo
     * @param maxCounterToChange cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
     * @param vehicle nombre del vehículo
     * @param pX
     * @param pY
     */
    Player(float maxSpeed, float speedMul, float accInc, float scaleX, float scaleY, int maxCounterToChange,
           const std::string &vehicle, float pX, float pY);

    /**
     * Devuelve la posición previa Y.
     * @return
     */
    float getPreviousY() const;

    /**
     * Actualiza la lógica del choque y restablece la velocidad y aceleración.
     * @param vehicleCrash true si es un choque entre vehículos
     */
    void hitControl(bool vehicleCrash);

    /**
     * Devuelve true si la lógica de choque está en ejecución.
     * @return
     */
    bool isCrashing() const;

    /**
     * Devuelve la velocidad real del vehículo.
     * @return
     */
    float getRealSpeed() const;

    /**
     * Actualiza la lógica de la aceleración y frenado del vehículo.
     * @param c
     * @param hasGotOut indica si se ha salido del camino
     * @return
     */
    Action accelerationControl(Config &c, bool hasGotOut);

    /**
     * Actualiza la lógica de giro del vehículo.
     * @param c
     * @param curveCoefficient pertenece [-0.9, 0.9]
     * @return
     */
    Direction rotationControl(Config &c, float curveCoefficient);

    /**
     * Actualiza el sprite del vehículo jugador y lo dibuja en la pantalla.
     * @param c
     * @param a
     * @param d
     * @param e
     * @param enableSound
     */
    void draw(Config &c, const Action &a, const Direction &d, const Elevation &e, bool enableSound = true);

    /**
     * Dibuja la animación inicial en la pantalla y devuelve si ha acabado.
     * @param c
     * @param x
     * @param end
     */
    void drawInitialAnimation(Config &c, float x, bool &end);

    /**
     * Dibuja la animación final en la pantalla y devuelve si ha acabado.
     * @param c
     * @param step
     * @param end
     * @param smoke
     */
    void drawGoalAnimation(Config &c, int &step, bool &end, bool smoke = true);

    /**
     * Fuerza a que el coche esté echando humo o no.
     * @param smoke
     */
    void setSmoking(bool smoke);
};


#endif //OUTRUN_PLAYER_HPP
