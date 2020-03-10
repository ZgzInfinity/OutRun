
#ifndef FERRARI_H
#define FERRARI_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "Step.h"
#include "Player.h"
#include "IntervalCurve.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

/*
 * Class which represents the player
 */
class Ferrari : public Player {

    public:

        /**
         * Constructor of the class
         */
        Ferrari(char* pathFile);



        /**
         * Load the set of sprites of the player
         */
         void loadSpritesFromPath();



        /**
         * Draw the player sprite in the console render window
         * @param app is the console window game where the sprite is going to be drawn
         */
        void drawPlayer(RenderWindow& app);



        /**
         * Check if the player has to advance in the track
         * @param eventDetected is a boolean to control if an event has occurred
         */
        void advancePlayer(bool& eventDetected);



        /**
         * Get the coordinate of the payer in the axis X
         * @return the position of the Ferrari in the axis X
         */
         float getPlayerX();



         /**
          * Get the mode of collision of the Ferrari
          * @return the mode to show the collision of the Ferrari
          */
         int getModeCollision();



        /**
         * Control if the user has pressed the q keyword to turn to the left
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        inline void controlTurningPlayerLeftKeyboard(int& speed, bool& eventDetected, RenderWindow& app);



         /**
         * Control if the user has pressed the w keyword to turn to the right
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        inline void controlTurningPlayerRightKeyboard(int& speed, bool& eventDetected, RenderWindow& app);



        /**
         * Control if the user has pressed the q keyword to turn to the left
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        inline void controlTurningPlayerLeftMouse(int& speed, bool& eventDetected, RenderWindow& app);



        /**
         * Control if the user has pressed the w keyword to turn to the right
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        inline void controlTurningPlayerRightMouse(int& speed, bool& eventDetected, RenderWindow& app);



        /**
         * Control if the user has pressed the q keyword to increase the speed
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        inline void controlPlayerSpeed(int& speed, bool& eventDetected, RenderWindow& app);



        /**
         * Control if the user has pressed the q keyword to increase the speed
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        inline void controlPlayerBraking(int& speed, bool& eventDetected, RenderWindow& app);



        /**
         * Control if the player has done any of his possible actions
         * @param speed is the actual speed of the Ferrari of the player
         * @param eventDetected is a boolean to control if an event has occurred
         * @param app is the console window game where the sprite is going to be drawn
         */
        void controlActionPlayer(int& speed, bool& eventDetected, RenderWindow& app);



        /**
         * Control if the player has have collision with the nearest element of the map to him
         * @param nearestStep is the step of the scene where is located the nearest element to the player
         * @param lastPos is the last position of the Ferrari in the axis Y
         * @param pos is the current position of the Ferrari in the axis Y
         */
        bool controlPossibleCollision(Step& nearestSprite, int& lastPos, int& pos);



        /**
         * Control if there is there inertia force or not if the Ferrari is on a curve of the scene
         * @param onCurve is a boolean which represents if the Ferrari is on curve or not
         * @param curve is the possible curve of the scene where the Ferrari is currently now
         * @param speed is the actual speed of the Ferrari of the player
         */
        void controlInertiaForce(bool& onCurve, IntervalCurve& curve, int& speed);



        /**
         * Shows to the user how the Ferraris crushes
         */
        void collisionShow();

};

#endif // FERRARI_H
