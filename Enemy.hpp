/******************************************************************************
 * @file    Enemy.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_ENEMY_HPP
#define OUTRUN_ENEMY_HPP

#include "Vehicle.hpp"

class Enemy : public Vehicle {
    const float oriX;

    Direction currentDirection;
    int current_direction_counter, max_direction_counter;

public:
    /**
     * Inicializa el vehículo rival.
     * @param maxSpeed
     * @param speedMul multiplicador de la velocidad que multiplicado por speed obtiene la velocidad real
     * @param scale escalado del sprite del vehículo
     * @param maxCounterToChange cuando counter_code_image llega a maxCounterToChange se actualiza el sprite
     * @param vehicle nombre del vehículo
     * @param pY
     */
    Enemy(float maxSpeed, float speedMul, float scale, int maxCounterToChange, const std::string &vehicle, float pY);

    /**
     * Actualiza la lógica del vehículo de manera automática.
     */
    void autoControl();

    /**
     * Inicializa el estado del vehículo.
     * @param iniPos
     * @param endPos
     */
    void update(float iniPos, float endPos);

    /**
     * Actualiza el sprite del vehículo enemigo y lo dibuja en la pantalla.
     * @param c
     * @param e
     * @param camX actual de la cámara
     */
    void draw(Config &c, const Elevation &e, float camX);

    void setMinScreenX(float screenX);

    void setMaxScreenX(float screenX);

    const Texture* getCurrentTexture() const;

    float getScale() const;
};


#endif //OUTRUN_ENEMY_HPP
