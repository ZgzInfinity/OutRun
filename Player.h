
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "Step.h"
#include "IntervalCurve.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;
using namespace rapidxml;

const int SPEED_INCREMENT = 50;
const int INITIAL_SPEED = 200;
const int MAX_SPEED = 1000;
const int MEDIUM_SPEED = INITIAL_SPEED + MAX_SPEED / 2;
const int CONTROL_SPEED = INITIAL_SPEED + MEDIUM_SPEED / 2;

const float BORDER_LIMIT_ROAD_LEFT = -1.4;
const float BORDER_LIMIT_ROAD_RIGHT = 1.4;


const float BORDER_ROAD_LEFT = -1.15;
const float BORDER_ROAD_RIGHT = 1.15;

/*
 * Class which represents the player
 */
class Player {

     protected:

        Texture t;

        // Code of the actual image texture
        int actual_code_image;

        // Position of the main character
        float playerX;

        // How to control the motorbike
        int typeControl = 0;

        // Sprite of the player motorbike
        Sprite playerSprite;

        // Mode to show the collision of the motorbike by default
        int mode;

        // Accumulator to the coordinate of the axis Y to make collision better
        int offset;

        // Offset added if the collision is with the second type
        int increment;

        // File path with the sprites
        char* filePath;

        // Vector of textures of the player
        vector<Texture> textures;

    public:



        /**
         * Constructor of the class
         */
        Player(char* pathFile){
            // Assign principal variables
            filePath = pathFile;
            // Store actual code of the image
            actual_code_image = 1;
            // Initializing the position of the player in the axis X
            playerX = 0.f;
            // Mode to show the collision of the motorbike by default
            mode = -1;
            // Accumulator to the coordinate of the axis Y to make collision better
            offset = 0;
        }



        /**
         * Load the set of sprites of the player
         */
         virtual void loadSpritesFromPath() = 0;



        /**
         * Draw the player sprite in the console render window
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual void drawPlayer(RenderWindow& app) = 0;



        /**
         * Check if the player has to advance in the track
         * @param eventDetected is a boolean to control if an event has occurred
         */
        virtual void advancePlayer(bool& eventDetected) = 0;



        /**
         * Get the coordinate of the payer in the axis X
         * @return the position of the motorbike in the axis X
         */
         virtual float getPlayerX() = 0;



         /**
          * Get the mode of collision of the motorbike
          * @return the mode to show the collision of the motorbike
          */
         virtual int getModeCollision() = 0;



        /**
         * Control if the user has pressed the q keyword to turn to the left
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual inline void controlTurningPlayerLeftKeyboard(int& speed, bool& eventDetected, RenderWindow& app) = 0;



         /**
         * Control if the user has pressed the w keyword to turn to the right
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual inline void controlTurningPlayerRightKeyboard(int& speed, bool& eventDetected, RenderWindow& app) = 0;



        /**
         * Control if the user has pressed the q keyword to turn to the left
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual inline void controlTurningPlayerLeftMouse(int& speed, bool& eventDetected, RenderWindow& app) = 0;



        /**
         * Control if the user has pressed the w keyword to turn to the right
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual inline void controlTurningPlayerRightMouse(int& speed, bool& eventDetected, RenderWindow& app) = 0;



        /**
         * Control if the user has pressed the q keyword to increase the speed
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual inline void controlPlayerSpeed(int& speed, bool& eventDetected, RenderWindow& app) = 0;



        /**
         * Control if the user has pressed the q keyword to increase the speed
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual inline void controlPlayerBraking(int& speed, bool& eventDetected, RenderWindow& app) = 0;



        /**
         * Control if the player has done any of his possible actions
         * @param speed is the actual speed of the motorbike of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        virtual void controlActionPlayer(int& speed, bool& eventDetected, RenderWindow& app) = 0;



        /**
         * Control if the player has have collision with the nearest element of the map to him
         * @param nearestStep is the step of the scene where is located the nearest element to the player
         * @param lastPos is the last position of the motorbike in the axis Y
         * @param pos is the current position of the motorbike in the axis Y
         */
        virtual bool controlPossibleCollision(Step& nearestSprite, int& lastPos, int& pos) = 0;



        /**
         * Control if there is there inertia force or not if the motorbike is on a curve of the scene
         * @param onCurve is a boolean which represents if the motorbike is on curve or not
         * @param curve is the possible curve of the scene where the motorbike is currently now
         * @param speed is the actual speed of the motorbike of the player
         */
        virtual void controlInertiaForce(bool& onCurve, IntervalCurve& curve, int& speed) = 0;



        /**
         * Shows to the user how the motorbikes crushes
         */
        virtual void collisionShow() = 0;

};


#endif
