/******************************************************************************
 * @file    Player.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_PLAYER_HPP
#define OUTRUN_PLAYER_HPP

#include <thread>
#include <mutex>
#include "Vehicle.hpp"

class Player : public Vehicle {
    const float speedMul, halfMaxSpeed, maxAcc, accInc, scaleY;
    float acceleration, minCrashAcc, xDest;

    sf::Sprite sprite;
    bool crashing; // True if crashing state is on
    bool smoking; // True if player generates smoke
    bool goingFast;

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
     */
    void draw(Config &c, const Action &a, const Direction &d, const Elevation &e);

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
     */
    void drawGoalAnimation(Config &c, int &step, bool &end);

    /**
     * Fuerza a que el coche esté echando humo o no.
     * @param smoke
     */
    void setSmoking(bool smoke);
};


#endif //OUTRUN_PLAYER_HPP
