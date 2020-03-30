/******************************************************************************
 * @file    Map.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#ifndef OUTRUN_MAP_HPP
#define OUTRUN_MAP_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Menu.hpp"

/**
 * Mapa que contiene la textura del paisaje (bg) y la textura de los objetos que se generarán de manera aleatoria a lo
 * largo del mapa (objects). Además cada objeto tendrá un coeficiente que indicará el porcentaje total del objeto con
 * el que se puede chocar empezando desde el centro [0, 1]. Esto es útil para objetos cuya base es menor a la copa.
 * El suelo está formado por un número limitado de rectángulos horizontales (lines) que tendrán dibujado el trozo de
 * carretera que les corresponda y objetos en los laterales.
 * Dependiendo de la posición de la cámara (posX, posY) el primer rectángulo visible será uno u otro y los objetos se
 * verán más grandes si están más cerca de la pantalla o más pequeños si están más lejos.
 */
class Map {
    sf::Texture bg; // Background

    // Objects
    std::vector<sf::Texture> objects;
    std::vector<float> hitCoeff;

    /**
     * Rectángulo horizontal cuyo centro está ubicado en las coordenadas (x, y, z), es decir, puede tener elevación (z).
     * En el centro del rectángulo estará la carretera que puede tener una curvatura dependiendo del coeficiente de
     * curvatura (curve) que tenga.
     * Además, puede contener un objeto en el lateral de la carretera (left or not left) si spriteNum != -1 con un
     * offset en x. Este objeto corresponde a objects[spriteNum].
     */
    struct Line {
        float x, y, z; // 3d center of line
        float X{}, Y{}, W{}; // screen coord
        float curve, spriteX, clip{}, scale{}, offset{}, spriteMinX{}, spriteMaxX{};
        int spriteNum{};
        bool left{};

        /**
         * Construye un rectángulo horizontal con una carretera con curvatura y altura aleatorias, y puede contener un
         * objeto aleatorio.
         */
        Line();

        /**
         * Establece las coordenadas en la pantalla que corresponen al rectángulo y su escala. Esta función debe ser
         * llamada para actualizar el rectángulo si se ha variado la posición del mapa y.
         * @param camX
         * @param camY
         * @param camZ
         * @param camD
         * @param width
         * @param height
         * @param rW
         */
        void project(float camX, float camY, float camZ, float camD, float width, float height, float rW);


        /**
         * Dibuja el rectángulo en la pantalla. Esta función debe ser llamada después de project().
         * @param w
         * @param objs
         * @param coeff
         */
        void drawSprite(sf::RenderWindow &w, const std::vector<sf::Texture> &objs, const std::vector<float> &coeff);
    };
    std::vector<Line> lines;

    float posX, posY;

public:
    /**
     * Crea un mapa con un paisaje dado el nombre del fichero de la imagen y con unos objetos dados los nombres de los
     * ficheros de las imágenes.
     * @param w
     * @param bgName
     * @param objectNames
     */
    Map(sf::RenderWindow &w, const std::string &bgName, const std::vector<std::string> &objectNames);

    /**
     * Establece la posición de la cámara.
     * @param pos = {x, y}, donde x = 0 es el medio de la carretera, y >= 0 AND y <= MAXLINES
     */
    void updateView(std::pair<float, float> pos);

    /**
     * Dibuja el fragmento del mapa actual dada la posición de la cámara establecida con la función updateView().
     * @param c
     */
    void draw(Config &c);

    /**
     * Devuelve true si pos corresponde a algún objeto del fragmento del mapa actual.
     * @param c
     * @param actualY
     * @param prevY
     * @param minX
     * @param maxX
     * @return
     */
    bool hasCrashed(const Config &c, float prevY, float actualY, float minX, float maxX) const;
};


#endif //OUTRUN_MAP_HPP
