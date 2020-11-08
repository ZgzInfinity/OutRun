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

#ifndef OUTRUN_VEHICLE_HPP
#define OUTRUN_VEHICLE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Menu.hpp"

#define XINC 0.033f // x increment
#define ACC_INC 0.01f

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
        CRASH,
        BOOT
    };
    enum Direction {
        RIGHT,
        TURNLEFT,
        TURNRIGHT
    };

protected:
    const float maxSpeed, halfMaxSpeed, scale;
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
    Vehicle(float maxSpeed, float scale, int maxCounterToChange, float speed, float posX, float posY, float previousY,
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

    /**
     * Devuelve la coordenada mínima de pantalla en X que ocupa el vehículo.
     * @return
     */
    float getMinScreenX() const;

    /**
     * Devuelve la coordenada máxima de pantalla en X que ocupa el vehículo.
     * @return
     */
    float getMaxScreenX() const;

    /**
     * Devuelve la aceleración del vehículo.
     * @return
     */
    float getAcceleration() const;
};


#endif //OUTRUN_VEHICLE_HPP
