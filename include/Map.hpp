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


/*
 * Module Map interface file
 */

#ifndef OUTRUN_MAP_HPP
#define OUTRUN_MAP_HPP

#include <cmath>
#include <cstring>


using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Menu.hpp"
#include "Enemy.hpp"

#define PRE_POS 2
#define FORK_RADIUS 20.0f


const int RECTANGLE = PRE_POS * 2 + 1; // the number of lines that form a rectangle



/**
 * Map that contains the texture of the landscape (bg) and the texture of the objects that will be shown at the
 * map length (objects). In addition each object will have a coefficient that will indicate the total percentage of the object with
 * The one that can be hit starting from the center [0, 1]. This is useful for objects whose base is less than the cup.
 * The floor is formed by a limited number of horizontal rectangles (lines) that will have drawn the piece of
 * road that corresponds to them and objects in the sides.
 * Depending on the position of the camera (posX, posY) the first visible rectangle will be one or the other and the objects will
 * you will see larger if you are closer to the screen or smaller if you are farther away
 */
class Map {


    /**
     * Information of an object corresponding to objects [spriteNum] if spriteNum != -1, with an offset in x.
     * If repetitive is true the object will be repeated to the edge of the screen.
     */
    struct SpriteInfo {

        // Identifier of the sprite element
        int spriteNum;

        // Distance to the road
        float offset;

        // Minimum position in axis x
        float spriteMinX;

        // Maximum position in axis x
        float spriteMaxX;

        // It is the space from spriteMinX to another minX side
        float spriteToSideX;

        // Controls if the sprite has to be drawn repeatedly or not
        bool repetitive;

        // Controls if it is a line of checkPint
        bool checkPoint;


        /**
         * Initialize the sprite
         */
        SpriteInfo();

    };

public:

    /**
     * Horizontal rectangle whose center is located in the coordinates (x, y, z), that is, it can have elevation (z).
     * In the center of the rectangle will be the road that can have a curvature depending on the coefficient of
     * curvature (curve) you have.
     * In addition, it can contain an object on each side of the road (spriteLeft, spriteRight).
     */
    struct Line {

        // Center of the rectangle in the axes x, y and z
        float x, y, z;

        // Screen coordinates in the screen
        float X{}, Y{}, W{};

        // Direction curve
        float curve;

        // Height in the axis y
        float clip;

        // Scale factor to draw the map element
        float scale;

        // Controls the type of color to draw
        bool mainColor;

        // Map elements in the left and in the tight
        SpriteInfo spriteFarLeft, spriteNearLeft, spriteRight;

        // Background position
        float bgX;

        // Offset in the axes x and y of the background
        float offsetX, yOffsetX;



        /**
         * Default constructor
         */
        explicit Line();



        /**
         * Sets the coordinates on the screen that correspond to the rectangle and its scale. This function must be
         * call to update the rectangle if the position of the map has been changed and.
         * @param camX is the coordinate of the rectangle in axis X
         * @param camY is the coordinate of the rectangle in axis Y
         * @param camZ is the coordinate of the rectangle in axis Z
         * @param camD is the deep of the rectangle in the screen
         * @param width is the width dimension of the rectangle
         * @param height is the height dimension of the rectangle
         * @param rW is the with of the road
         * @param zOffset is the offset in the axis Z
         */
        void project(float camX, float camY, float camZ, float camD, float width, float height, float rW, float zOffset);



        /**
         * Coefficient aliases to control the types of collision
         */
        enum HitCoeffType {
            HIT_CENTER,
            HIT_LEFT,
            HIT_RIGHT,
            HIT_SIDES
        };



        /**
         * Draw the map element in the screen of the game
         * @param w is the console window of the game
         * @param objs is a vector with all the map elements textures
         * @param hitCoeff is a vector with all the hits coefficients
         * @param hitCoeffType is the hit coefficient of the element to draw
         * @param scaleCoeff is the scaling factor of the element to draw
         * @param object is the element to draw in the screen
         * @param left control if the object has to be drawn on the left or on the right of the screen
         */
        void drawSprite(sf::RenderTexture &w, const std::vector<sf::Texture> &objs, const std::vector<float> &hitCoeff,
                        const std::vector<HitCoeffType> &hitCoeffType, const std::vector<float> &scaleCoeff,
                        SpriteInfo &object, bool left) const;

    };

    // Circles info for forks (with different centers):
    // Circle 1: y = -sqrt(r² - x²) + r
    // Circle 2: y = sqrt(r² - (x - a)²) + b

    // A value for C1
    const float aOffsetX = sqrt(2.0f) * FORK_RADIUS;
    // B value for C1
    const float bOffsetX = FORK_RADIUS - sqrt(2.0f) * FORK_RADIUS;
    // X increment
    const float xChange = static_cast<const float>(FORK_RADIUS * sin(0.75f * M_PI));

    // Background
    Texture bg;

    // Vector with the backgrounds for the transition map
    vector<Texture> backgroundMaps;

    // Objects of the map
    vector<sf::Texture> objects;
    vector<float> hitCoeffs;
    vector<Line::HitCoeffType> hitCoeffTypes;
    vector<float> scaleCoeffs;
    vector<Line> lines;

    // Colors
    Color roadColor[2], grassColor[2], rumbleColor, dashColor;

    // Last colors of the map in the transition
    Color roadColorPrev[2], grassColorPrev[2], rumbleColorPrev[2], dashColorPrev[2];

    // Camera
    float posX, posY;

    // Next map
    Map *next, *nextRight;

    // Control if the map is the starting map
    bool initMap;

    // Control if the map is the goal map
    bool goalMap;

    // Maximum time to complete the map
    int maxTime;

    // Type of terrain of the map
    int terrain;

    // Total lines of the map without an with fork added
    int totalLines, totalLinesWithoutFork;

    // Transition color
    int colorThreshold;

    // Line of the checkpoint indicators
    int checkPointLine;


    /**
     * Return the line of the landscape with index n
     * @param n is the index of a landscape's line
     * @return
     */
    Line *getLine(int n);



    /**
     * Return the line of the landscape with index n
     * @param n is the index of a landscape's line
     * @return
     */
    Line getLine(int n) const;



    /**
     * Returns the previous line of the landscape to the line with index n
     * @param is the index of a landscape's line
     * @return
     */
    Line *getPreviousLine(int n);



    /**
     * Returns the previous line of the landscape to the line with index n
     * @param is the index of a landscape's line
     * @return
     */
    Line getPreviousLine(int n) const;



    /**
     * Add a new line of the landscape but not from the file
     * @param x is the actual coordinate 3d of the map in the axis x
     * @param y is the actual coordinate 3d of the map in the axis y
     * @param z is the actual coordinate 3d of the map in the axis z
     * @param prevY is the coordinate 3d of the map in the axis y of the last line added
     * @param curve represents the coefficient radius of the line to add to the map
     * @param mainColor shows in which color the line is going to be painted
     * @param spriteLeft is the map element located near and on the left of the road
     * @param spriteNearLeft is the map element located near and on the left of the road
     * @param spriteRight is the map element located near and on the right of the road
     * @param bgX is the background position of the map in axis x
     * @param offsetX is the controller offset to make the bifurcations
     * @param offsetInc is partial offset to increment the bifurcation direction
     */
    void addLine(float x, float y, float &z, float prevY, float curve, bool mainColor, const SpriteInfo &spriteFarLeft,
                 const SpriteInfo &spriteNearLeft, const SpriteInfo &spriteRight, float &bgX, float &offsetX, float offsetInc = 0.0f);



    /**
     * Add rectangles from the instructions to the map from (x, y, z). Update z for a new line.
     * @param x is the coordinate of the first rectangle in the axis x
     * @param y is the coordinate of the first rectangle in the axis y
     * @param z is the coordinate of the first rectangle in the axis z
     * @param bgX is the position in axis x of the background
     * @param instructions is a vector that contains all the information of the lines of the file that contains the map
     */
    void addLines(float x, float y, float &z, float &bgX, const std::vector<std::vector<std::string>> &instructions);




    /**
     * Load all the textures of the map elements located in the landscape
     * @param path is the path of the file which contains the configuration of the landscape
     * @param objectNames is a vector which contains the identifiers of the map elements
     * @param objectIndexes is a vector which contains the code ids of the map elements
     */
    void loadObjects(const std::string &path, const std::vector<std::string> &objectNames, std::vector<int> &objectIndexes);



public:


    // Creates a map with a landscape given the name of the image file and with some objects given the names of
    // image files. The map content must be in the path. If random is true the
    // map in a random way, on the other hand if random is false the map is created from the file map.info, which is
    // described below:
    // - Comments can be included anywhere, starting with /* and ending with */
    // - First you must indicate the two colors of the road, the two colors of the ground, the color of the shoulder (rumble)
    // and that of the discontinuous lines (dash), in RGB and separated by spaces.
    // - In second place, the route is indicated with objects, curves and elevations.
    // - Random fragments of the same way can be included:
    // RANDOM n o_1 o_2 ... o_x
    // Where n indicates the number of fragments and o_i indicates the index of the object that may contain (#.png). By
    // default no object.
    // - Finally to indicate the end of the map you must include END.
    // - File example:
    //
    // /* Map 1 */
    //
    // 107 107 107 /* Road RGB color 1 */
    // 105 105 105 /* Road RGB color 2 */
    // 16 200 16 /* Grass RGB color 1 */
    // 0 154 0 /* Grass RGB color 2 */
    // 255 255 255 /* RGB color rumble */
    // 255 255 255 /* Dash RGB color */
    //
    // /*
    // * ROAD:
    // * 1º Optional: + ;Indicates that it is an object repeated infinitely to the left. By default it is not repeated.
    // * 2º Optional: # ;Indicates the index of the object placed to the left of the road (#.png). By default there is no object.
    // * 3º Optional: #.# ;Indicates the offset from the edge of the road to the object. Default is 0.0.
    // * 4º Required: - ;Represents the road
    // * 5º Optional: + ;Indicates that it is an object repeated infinitely to the right. By default it is not repeated.
    // * 6º Optional: # ;Indicates the index of the object placed to the right of the road (#.png). By default there is no object.
    // * 7º Optional: #.# ;Indicates the offset from the edge of the road to the object. Default is 0.0.
    // */
    // ROAD -
    // ROAD 5 -
    // CURVE 0.5 /* Curve with index between -0.9 and 0.9, negative if it is to the left and positive if it is to the right */
    // ROAD - 5
    // ROAD 5 0.0 - 5 2.0
    // STRAIGHT /* Straight
    // ROAD + 5 -
    // CLIMB 1.0 /* Rise with index between 0.0 and MAX_FLOAT */
    // ROAD - + 5 -1.0
    // FLAT /* Flat, no climbs or descents */
    // ROAD -
    // ROAD -
    // RANDOM 10 5 4 /* Adds 10 random fragments that can contain the objects 5 and 4 */
    // ROAD -
    // ROAD -
    // ROAD -
    // DROP 1.0 /* Drop with index between 0.0 and MAX_FLOAT */
    // ROAD -
    // ROAD -
    // END /* Required */
    //


    /**
     * Creates a landscape reading its configuration file
     * @param c is the configuration of the game
     * @param path is the path of the file that contains the configuration of the landscape
     * @param bgName is the path of the image that represents the background of the landscape
     * @param objectNames is a vector with the aliases of the different map elements
     * @param random controls if the map has to be generated from a file or randomly
     * @param time is the time available to complete the landscape
     */
    Map(Config &c, const std::string &path, const std::string &bgName, const std::vector<std::string> &objectNames, bool random, int time);



    /**
     * Creates a straight flat map which represents the starting point of a map
     * @param map is the map to be displayed
     * @param flagger is the flagger position while is announcing the start
     * @param semaphore is the color of the semaphore in the starting
     */
    Map(const Map &map, int &flagger, int &semaphore);



    /**
     * Creates a straight flat map which represents the goal point of a map
     * @param flagger is the flagger position while is announcing the goal
     * @param goalEnd is position of the goal in the final map
     */
    Map(int &flagger, int &goalEnd);



    /**
     * Initialize the colors of the map
     * @param map is the map whose colors are going to be initialized
     */
    void setColors(const Map &map);



    /**
     * Increase the code id of the map element of the step if it exits
     * @param step is the actual step of the map with the map elements
     * @param right controls if there are more map elements of the step
     * @param increment is how to code id has to be increased
     */
    void incrementSpriteIndex(int line, bool right, int increment = 1);



    /**
     * Add a map to follow the actual
     * @param map is the scenario to be added
     */
    void addNextMap(Map *map);



    /**
     * Create a fork between the actual map and its neighbors
     * @param left is the left map of the fork
     * @param right is the right map of the fork
     */
    void addFork(Map *left, Map *right);



    /**
     * Adds the offset to the map to be displayed correctly
     * @param yOffset is the offset to add
     */
    void setOffset(float yOffset);



    /**
     * Updates the position of the camera in the map
     * @param pX is the new coordinate in the axis x of the camera
     * @param pY is the new coordinate in the axis y of the camera
     */
    void updateView(float pX, float pY);



    /**
     * Returns the coordinate x of the camera
     * @return
     */
    float getCamX() const;



    /**
     * Returns the coordinate y of the camera
     * @return
     */
    float getCamY() const;



    /**
     * Draws the percent of map visible with all the traffic cars in it
     * @param c is the configuration of the player
     * @param vehicles is the vector with all the traffic cars
     */
    void draw(Config &c, std::vector<Enemy> &vehicles, float playerPosX, float playerPosY, bool inFork, Map* left, Map* right);



    /**
     * Returns true if the vehicle has crashed with any element of the map or with its borders
     * @param c is the configuration of the player
     * @param prevY is the previous position of the player in the axis y of the map
     * @param currentY is the actual position of the player in the axis y of the map
     * @param currentX is the actual position of the player in the axis x of the map
     * @param minX is the minimum threshold to detect a collision
     * @param maxX is the maximum threshold to detect a collision
     * @param crashPos is the possible position of crash detected
     * @return
     */
    bool hasCrashed(const Config &c, float prevY, float currentY, float currentX, float minX, float maxX,
                    float &crashPos) const;



   /**
     * Returns true if the actual position in axis x is outside the road.
     * Otherwise returns false
     * @param currentX is the position on axis x
     * @param currentY is the position on axis y
     * @return
     */
    bool hasGotOut(float currentX, float currentY) const;



    /**
     * Returns the curvature coefficient corresponding to the currentY rectangle. The coefficient is positive if the
     * curve is to the right, negative if it's to the left and 0 if it's a straight line
     * @param currentY currentY is the current of the landscape where is the player
     * @return
     */
    float getCurveCoefficient(float currentY) const;



   /**
     * Returns the elevation corresponding to the currentY rectangle based on the previous rectangle.
     * @param currentY is the current of the map where is the player
     * @return
     */
    Vehicle::Elevation getElevation(float currentY) const;



    /**
     * Returns true if the map has been finished.
     * Otherwise returns false
     * @return
     */
    bool isOver() const;



    /**
     * Returns the height of the map
     * @return
     */
    float getMaxY() const;



    /**
     * Returns the offset of the road in the axis x.
     * @return
     */
    float getOffsetX() const;



    /**
     * Returns true if the player is on a fork. Otherwise returns false
     * @param currentY is the coordinate in axis y which to be tested to know
     *        if the player is on a fork or not
     * @return
     */
    bool inFork(float currentY) const;



    /**
     * Returns the next map or null if it is the last one
     * @return
     */
    Map *getNext() const;



    /**
     * Returns true if is the init map. Otherwise returns false
     * @return
     */
    bool isInitMap() const;



    /**
     * Returns true if is the goal landscape. Otherwise returns false
     * @return
     */
    bool isGoalMap() const;



    /**
     * Returns the time to complete the map
     * @return
     */
    int getTime() const;



    /**
     * Returns the kind of terrain of the landscape
     * @return
     */
    int getTerrain() const;



    /**
     * Return the line of the possible checkpoint in the map
     * @return
     */
    int getCheckPointLine() const;



    /**
     * Return number of lines of the map without fork
     * @return
     */
    int getLinesWithoutFork()const;

};


#endif //OUTRUN_MAP_HPP
