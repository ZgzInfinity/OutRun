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

    /**
     * Devuelve true si pos corresponde al vehículo actual. En caso de que sea true, también devuelve la posición Y
     * donde han colisionado.
     * @param c
     * @param currentY
     * @param prevY
     * @param minX
     * @param maxX
     * @param crashPos
     * @return
     */
    bool hasCrashed(const Config &c, float prevY, float currentY, float minX, float maxX, float &crashPos) const;

    /**
     * Devuelve true si el coche se muestra en pantalla y la distancia al jugador, sino devuelve false.
     * @param c
     * @param minY
     * @param playerX
     * @param playerY
     * @param distanceX
     * @param distanceY
     * @return
     */
    bool isVisible(const Config &c, float minY, float playerX, float playerY, float &distanceX, float &distanceY) const;
};


#endif //OUTRUN_ENEMY_HPP
