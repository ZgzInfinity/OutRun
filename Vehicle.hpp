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
 */
class Vehicle {
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

protected:
    const float maxSpeed, scale;
    const int maxCounterToChange;

    float speed;
    float posX, posY, previousY, minScreenX, maxScreenX;

    std::vector<sf::Texture> textures;
    int current_code_image;
    int counter_code_image; // Counter to change actual_code_image

public:
     /**
      * Inicializa el vehículo.
      * @param maxSpeed
      * @param scale
      * @param maxCounterToChange
      * @param speed
      * @param posX
      * @param posY
      * @param previousY
      * @param minScreenX
      * @param maxScreenX
      * @param vehicle
      * @param numTextures
      * @param currentCodeImage
      * @param counterCodeImage
      */
    Vehicle(float maxSpeed, float scale, int maxCounterToChange, float speed, float posX, float posY,  float previousY,
            float minScreenX, float maxScreenX, const std::string &vehicle, int numTextures, int currentCodeImage,
            int counterCodeImage);

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

    float getMinScreenX() const;

    float getMaxScreenX() const;
};


#endif //OUTRUN_VEHICLE_HPP
