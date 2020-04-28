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

#define XINC 0.05f // x increment

/**
 * La información del vehículo está compuesta por su velocidad, aceleración, posición x, conjunto de texturas del
 * vehículo, sprite actual y acción actual y dirección actual.
 * Además, se incluyen las siguientes constantes para la lógica del vehículo:
 *      speedMul: multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
 *      maxSpeed: velocidad máxima
 *      halfMaxSpeed: la mitad de la velocidad máxima
 *      maxAcc: aceleración máxima
 *      accInc: incremento de la aceleración
 *      scale: escalado del sprite del vehículo
 *      maxCounterToChange: cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
 */
class Vehicle {
    const float speedMul, maxSpeed, halfMaxSpeed, maxAcc, accInc, scale;
    const int maxCounterToChange;

    float speed;
    float acceleration;
    float posX, posY, previousY, minScreenX, maxScreenX;

    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    int current_code_image;
    int counter_code_image; // Counter to change actual_code_image
    bool crashing; // True if crashing state is on

public:
    enum Elevation {
        UP,
        FLAT,
        DOWN
    };
    enum Action {
        NONE,
        BRAKE,
        ACCELERATE,
        CRASH
    };
    enum Direction {
        RIGHT,
        TURNLEFT,
        TURNRIGHT
    };

    /**
     * Inicializa el vehículo.
     * @param maxSpeed
     * @param speedMul multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
     * @param accInc incremento de la aceleración
     * @param scale escalado del sprite del vehículo
     * @param isPlayer
     * @param maxCounterToChange cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
     * @param vehicle nombre del vehículo
     * @param pX
     * @param pY
     */
    Vehicle(float maxSpeed, float speedMul, float accInc, float scale, bool isPlayer, int maxCounterToChange,
            const std::string &vehicle, float pX, float pY);

    /**
     * Establece la posición actual del vehículo.
     * @param pX
     * @param pY
     */
    void setPosition(float pX, float pY);

    /**
     * Devuelve la posición actual X.
     * @return
     */
    float getPosX() const;

    /**
     * Devuelve la posición actual Y.
     * @return
     */
    float getPosY() const;

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
     * Actualiza la lógica del vehículo de manera automática.
     * @param playerX
     * @param playerY
     * @param a
     * @param d
     */
    void autoControl(float playerX, float playerY, Action &a, Direction &d);

    /**
     * Actualiza el sprite del vehículo jugador y lo dibuja en la pantalla.
     * @param c
     * @param a
     * @param d
     * @param e
     */
    void drawPlayer(Config &c, const Action &a, const Direction &d, const Elevation &e);

    /**
     * Actualiza el sprite del vehículo enemigo y lo dibuja en la pantalla.
     * @param c
     * @param a
     * @param d
     * @param e
     * @param camX actual de la cámara
     */
    void drawEnemy(Config &c, const Action &a, const Direction &d, const Elevation &e, float camX);

    void setMinScreenX(float screenX);

    void setMaxScreenX(float screenX);

    float getMinScreenX() const;

    float getMaxScreenX() const;

    const Texture* getCurrentTexture() const;

    float getScale() const;
};


#endif //OUTRUN_VEHICLE_HPP
