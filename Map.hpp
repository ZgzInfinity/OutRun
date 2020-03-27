/******************************************************************************
 * @file    Map.cpp
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
 * largo del mapa (objects).
 * El suelo está formado por un número limitado de rectángulos horizontales (lines) que tendrán dibujado el trozo de
 * carretera que les corresponda y objetos en los laterales.
 * Dependiendo de la posición en el mapa (posY) el primer rectángulo visible será uno u otro y los objetos se verán
 * más grandes si están más cerca de la pantalla o más pequeños si están más lejos.
 */
class Map {
    sf::Texture bg; // Background

    // Objects
    std::vector<sf::Texture> objects;

    /**
     * Rectángulo horizontal cuyo centro está ubicado en las coordenadas (x, y, z), es decir, puede tener elevación (z).
     * En el centro del rectángulo estará la carretera que puede tener una curvatura dependiendo del coeficiente de
     * curvatura (curve) que tenga.
     * Además, puede contener un objeto en el lateral de la carretera (left or not left) con un offset en x. Este objeto
     * corresponde a objects[spriteNum].
     */
    struct Line {
        float x, y, z; // 3d center of line
        float X{}, Y{}, W{}; // screen coord
        float curve, spriteX, clip{}, scale{}, offset{};
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
         */
        void drawSprite(sf::RenderWindow &w, const std::vector<sf::Texture> &objs);
    };
    std::vector<Line> lines;

    float posY;

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
     * Dibuja el fragmento del mapa actual en la pantalla dadas la configuración, la profundidad de la cámara, la
     * posición x del mapa y la velocidad actual de movimiento. Esta velocidad actualizará la posición Y global del mapa.
     * @param c
     * @param camD
     * @param posX
     * @param speed
     */
    void draw(Config &c, float camD,  float posX, float speed); // Relative position
};


#endif //OUTRUN_MAP_HPP
