/******************************************************************************
 * @file    Player.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_PLAYER_HPP
#define OUTRUN_PLAYER_HPP

#include "Vehicle.hpp"

class Player : public Vehicle {
    const float speedMul, halfMaxSpeed, maxAcc, accInc;
    float acceleration;

    sf::Sprite sprite;
    bool crashing; // True if crashing state is on

public:
    /**
     * Inicializa el vehículo del jugador.
     * @param maxSpeed
     * @param speedMul multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
     * @param accInc incremento de la aceleración
     * @param scale escalado del sprite del vehículo
     * @param maxCounterToChange cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
     * @param vehicle nombre del vehículo
     * @param pX
     * @param pY
     */
    Player(float maxSpeed, float speedMul, float accInc, float scale, int maxCounterToChange,
            const std::string &vehicle, float pX, float pY);

    /**
     * Devuelve la posición previa Y.
     * @return
     */
    float getPreviousY() const;

    /**
     * Actualiza la lógica del choque y restablece la velocidad y aceleración.
     */
    void hitControl();

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
     */
    void draw(Config &c, const Action &a, const Direction &d, const Elevation &e);
};


#endif //OUTRUN_PLAYER_HPP
