/******************************************************************************
 * @file    Vehicle.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_VEHICLE_HPP
#define OUTRUN_VEHICLE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Menu.hpp"

/**
 * La información del vehículo está compuesta por su velocidad, aceleración, posición x, conjunto de texturas del
 * vehículo, sprite actual y acción actual y dirección actual.
 * Además, se incluyen las siguientes constantes para la lógica del vehículo:
 *      speedMul: multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
 *      maxAcc: aceleración máxima
 *      accInc: incremento de la aceleración
 *      scale: escalado del sprite del vehículo
 *      maxCounterToChange: cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
 */
class Vehicle {
    const float speedMul, maxAcc, accInc, scale;
    const int maxCounterToChange;

    float speed;
    float acceleration;
    float posX, posY, previousY, minScreenX, maxScreenX;

    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    int actual_code_image;
    int counter_code_image; // Counter to change actual_code_image
    enum Action {
        NONE,
        BRAKE,
        ACCELERATE
    };
    enum Direction {
        RIGHT,
        TURNLEFT,
        TURNRIGHT
    };

public:
    /**
     * Inicializa el vehículo.
     * @param maxSpeed
     * @param speedMul multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
     * @param accInc incremento de la aceleración
     * @param scale escalado del sprite del vehículo
     * @param numTextures
     * @param maxCounterToChange cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
     * @param vehicle nombre del vehículo
     */
    Vehicle(float maxSpeed, float speedMul, float accInc, float scale, int numTextures, int maxCounterToChange,
            const std::string &vehicle);

    /**
     * Establece la posición actual del vehículo y resetea la velocidad y la aceleración.
     * @param pX
     * @param pY
     */
    void resetPosition(float pX, float pY);

    /**
     * Devuelve la posición actual del vehículo.
     * @return {posX, posY}
     */
    std::pair<float, float> getPosition() const;

    /**
     * Devuelve la velocidad real del vehículo.
     * @return
     */
    float getRealSpeed() const;

    /**
     * Actualiza la lógica de la aceleración y frenado del vehículo.
     * @param c
     * @return
     */
    Action accelerationControl(Config &c);

    /**
     * Actualiza la lógica de giro del vehículo.
     * @param c
     * @return
     */
    Direction rotationControl(Config &c);

    /**
     * Actualiza el sprite del vehículo y lo dibuja en la pantalla.
     * @param c
     * @param a
     * @param d
     */
    void draw(Config &c, Action a, Direction d);

    /**
     *
     * @return
     */
    float getPosY() const;

    float getPreviousY() const;

    float getMinScreenX() const;

    float getMaxScreenX() const;
};


#endif //OUTRUN_VEHICLE_HPP
