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

    Direction currentDirection, calculatedPath;
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
     * Actualiza la lógica del vehículo de manera automática para el movimiento actual.
     *
     * La agresividad de la IA está dentro de la configuración. Es un valor entre 0 y 100 que indica la probabilidad de
     * que la IA actúe en este movimiento.
     *
     *      Si la agresividad de la IA es 0, el movimiento será como en el juego original, es decir, el coche seguirá
     *      a velocidad constante sin salirse de la carretera y siguiendo una trayectoria recta o con giro (elegida
     *      de manera aleatoria) y sin ser influenciado por el jugador.
     *
     *      Si la agresividad de la IA es 100, el movimiento será controlado por la IA y se verá influenciado por el
     *      jugador de la siguiente manera:
     *          Si el vehículo no se encuentra en la trayectoria del jugador, realiza un giro para acercarse a la
     *          trayectoria del jugador.
     *          Si el vehículo se encuentra en la trayectoria del jugador y está por detrás suyo, acelera para intentar
     *          chocarse con el jugaodr.
     *          Si el vehículo se encuentra en la trayectoria del jugador y está por delante suyo, frena para intentar
     *          chocarse con el jugaodr.
     *
     *      Si la agresividad de la IA se encuentra entre 0 y 100, realiza una de las dos acciones descritas (original
     *      o IA) con probabilidad p = (agresividad / 100) de que actúe la IA y p' = (1 - p) de que sea como en el
     *      original.
     *
     * @param c
     * @param playerPosX
     * @param playerPosY
     */
    void autoControl(Config &c, float playerPosX, float playerPosY);

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
