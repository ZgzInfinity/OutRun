
#include "../include/Ferrari.h"

Ferrari::Ferrari(char* pathFile) : Player(pathFile){};



/**
 * Draw the player sprite in the console render window
 * @param app is the console window game where the sprite is going to be drawn
 */
void Ferrari::drawPlayer(RenderWindow& app){
    this->offset = (mode == 0 && actual_code_image <= 35) ? offset += 10 : offset += 5;
    this->offset = (mode == 1 && actual_code_image <= 46) ? offset += 20 : offset;
    if (mode == -1) offset = 0;
    playerSprite.setPosition(Vector2f(370.f, HEIGHT - 220.f - offset));
    app.draw(playerSprite);
}



/**
 * Load the set of sprites of the player
 */
void Ferrari::loadSpritesFromPath(){
    // Document xml where the document is going to be parsed
    xml_document<> doc;
    file<> file("Configuration/Vehicles/Ferrari.xml");
    // Parsing the content of file
    doc.parse<0>(file.data());

    // Get the principal node of the file
    xml_node<> *nodePlayer = doc.first_node();

    // Loop in order to iterate all the children of the principal node
    for (xml_node<> *child = nodePlayer->first_node(); child; child = child->next_sibling()){
        // Check if the actual node is the controller of the paths of the sprites
        if ((string)child->name() == "SpritePaths"){
            // Loop for iterate throughout the path files and add then to the vector
            for (xml_node<> * pathNode = child->first_node(); pathNode; pathNode = pathNode->next_sibling()){
                // Add the texture to the vector
                if (t.loadFromFile(string(filePath) + pathNode->value())){
                    // Increment the textures read
                    textures.push_back(t);
                }
            }
        }
        else {
            // Error the tag has not been found
            cerr << "The file must have the tag SpritePaths defined" << endl;
            exit(12);
        }
    }
}



/**
 * Check if the player has to advance in the track
 * @param eventDetected is a boolean to control if an event has occurred
 */
void Ferrari::advancePlayer(bool& eventDetected){
    // Eliminate this event detection
    if (!eventDetected){
        if (actual_code_image > 4){
            // First advance sprite loaded
            actual_code_image = 1;
        }
        else {
            if (actual_code_image != 4){
                actual_code_image++;
            }
            else {
                actual_code_image = 1;
            }
        }
        playerSprite.setTexture(textures[actual_code_image - 1]);
    }
    else {
        // Elimination of the last event registered
        eventDetected = false;
    }
}




/**
 * Get the coordinate of the payer in the axis X
 * @return the position of the Ferrari in the axis X
 */
float Ferrari::getPlayerX(){
    return playerX;
}




/**
 * Get the mode of collision of the Ferrari
 * @return the mode to show the collision of the Ferrari
 */
int Ferrari::getModeCollision(){
    return mode;
}



/**
 * Control if the user has pressed the w keyword to turn to the right
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
inline void Ferrari::controlTurningPlayerLeftKeyboard(int& speed, bool& eventDetected, RenderWindow& app){
    // Check if key left pressed
    if (Keyboard::isKeyPressed(Keyboard::Q)){
        // Check if the motorbike can be moved or not spite of pressing the key
        if (playerX - 0.1 >= BORDER_LIMIT_ROAD_LEFT){
            // Check if the motorbike is outside the road
            if (playerX >= BORDER_ROAD_LEFT && playerX <= BORDER_ROAD_RIGHT){
                // Advance more
                playerX -= 0.1;
            }
            else {
                 playerX -= 0.035;
            }
        }
        // Change the texture
        if (actual_code_image < 5 || (actual_code_image >= 13 && actual_code_image <= 24) ||
           (actual_code_image >= 33 && actual_code_image <= 40))
        {
            actual_code_image = 5;
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image >= 5 && actual_code_image <= 12){
            // Increment the actual code of the sprite
            if (actual_code_image != 12){
                // Increment the texture of the sprite
                actual_code_image++;
            }
            else {
                // Change sprite while the motorbike is turning to left
                actual_code_image --;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        // Register event
        eventDetected = true;
    }
}



/**
 * Control if the user has pressed the q keyword to turn to the left
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
/**
 * Control if the user has pressed the q keyword to turn to the left
 * @param speed is the actual speed of the motorbike of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
inline void Ferrari::controlTurningPlayerRightKeyboard(int& speed, bool& eventDetected, RenderWindow& app){
    // Check if key right pressed
    if (Keyboard::isKeyPressed(Keyboard::W)){
        // Check if the motorbike can be moved or not spite of pressing the key
        if (playerX + 0.1 <= BORDER_LIMIT_ROAD_RIGHT){
            // Check if the motorbike is outside the road
            if (playerX >= BORDER_ROAD_LEFT && playerX <= BORDER_ROAD_RIGHT){
                // Advance more
                playerX += 0.1;
            }
            else {
                 playerX += 0.035;
            }
        }
        // Change the texture
        if (actual_code_image < 13 || (actual_code_image >= 21 && actual_code_image <= 31)){
            actual_code_image = 13;
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image <= 20){
            // Increment the actual code of the sprite
            if (actual_code_image != 20){
                // Increment the texture of the sprite
                actual_code_image++;
            }
            else {
                // Change sprite while the motorbike is turning to left
                actual_code_image --;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        // Register event
        eventDetected = true;
    }
}



/**
 * Control if the user has pressed the q keyword to turn to the left
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
inline void Ferrari::controlTurningPlayerLeftMouse(int& speed, bool& eventDetected, RenderWindow& app){
    // Get the x coordinate of the mouse in the window
    int coordinateX = sf::Mouse::getPosition(app).x;
    // Check if key right pressed
    if (coordinateX < (int)app.getSize().x / 3  ){
        // Check if the motorbike can be moved or not spite of pressing the key
        if (playerX - 0.1 >= BORDER_LIMIT_ROAD_LEFT){
            // Check if the motorbike is outside the road
            if (playerX >= BORDER_ROAD_LEFT && playerX <= BORDER_ROAD_RIGHT){
                // Advance more
                playerX -= 0.1;
            }
            else {
                 playerX -= 0.035;
            }
        }
        // Change the texture
        if (actual_code_image < 5 || (actual_code_image >= 13 && actual_code_image <= 24) ||
           (actual_code_image >= 33 && actual_code_image <= 40))
        {
            actual_code_image = 5;
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image >= 5 && actual_code_image <= 12){
            // Increment the actual code of the sprite
            if (actual_code_image != 12){
                // Increment the texture of the sprite
                actual_code_image++;
            }
            else {
                // Change sprite while the motorbike is turning to left
                actual_code_image--;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        // Register event
        eventDetected = true;
    }
}



/**
 * Control if the user has pressed the w keyword to turn to the right
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
inline void Ferrari::controlTurningPlayerRightMouse(int& speed, bool& eventDetected, RenderWindow& app){
    // Get the x coordinate of the mouse in the window
    int coordinateX = sf::Mouse::getPosition(app).x;
    // Check if key right pressed
    if (coordinateX > 2 * (int)(app.getSize().x / 3)){
         // Check if the motorbike can be moved or not spite of pressing the key
        if (playerX + 0.1 <= BORDER_LIMIT_ROAD_RIGHT){
            // Check if the motorbike is outside the road
            if (playerX >= BORDER_ROAD_LEFT && playerX <= BORDER_ROAD_RIGHT){
                // Advance more
                playerX += 0.1;
            }
            else {
                 playerX += 0.035;
            }
        }
        // Change the texture
        if (actual_code_image < 13 || (actual_code_image >= 21 && actual_code_image <= 31)){
            actual_code_image = 13;
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image <= 20){
            // Increment the actual code of the sprite
            if (actual_code_image != 20){
                // Increment the texture of the sprite
                actual_code_image++;
            }
            else {
                // Change sprite while the motorbike is turning to left
                actual_code_image--;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        // Register event
        eventDetected = true;
    }
}



/**
 * Control if the user has pressed the q keyword to increase the speed
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
inline void Ferrari::controlPlayerSpeed(int& speed, bool& eventDetected, RenderWindow& app){
    // Check if the user is accelerating
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up))){
        // Control about not acceleration if the Ferrari goes in the grass
        if (playerX >= BORDER_ROAD_LEFT && playerX <= BORDER_ROAD_RIGHT){
            // Increment the speed because it is inside the road
            if (speed <= MAX_SPEED){
                // Increment of the speed
                speed += SPEED_INCREMENT + SPEED_INCREMENT;
            }
        }
        else {
            // Increment the speed because it is outside the road
            if (speed >= INITIAL_SPEED){
                // Decrement of the speed
                speed -= SPEED_INCREMENT - SPEED_INCREMENT;
            }
        }
        // Check if the key to turn left is pressed
        if (!eventDetected){
            // Check the type of control of the Ferrari
            if (typeControl == 0){
                 // Check if the key to turn to the right is pressed
                 controlTurningPlayerRightKeyboard(speed, eventDetected, app);
            }
            else {
                // Check if the mouse has been moved to turn to the right
                controlTurningPlayerRightMouse(speed, eventDetected, app);
            }
            return;
        }
        // Check if the key to turn left is pressed
        if (!eventDetected){
            // Check the type of control of the Ferrari
            if (typeControl == 0){
                 // Check if the key to turn to the left is pressed
                 controlTurningPlayerLeftKeyboard(speed, eventDetected, app);
            }
            else {
                // Check if the mouse has been moved to turn to the left
                controlTurningPlayerLeftMouse(speed, eventDetected, app);
            }
            return;
        }
        // Change the sprite;
        actual_code_image = 1;
        // Set the texture from the file
        playerSprite.setTexture(textures[actual_code_image - 1]);
    }
}




/**
 * Control if the user has pressed the q keyword to increase the speed
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
inline void Ferrari::controlPlayerBraking(int& speed, bool& eventDetected, RenderWindow& app){
// Check if the user is braking
    if (Keyboard::isKeyPressed(Keyboard::Down)){
        // Check more events
        if (!eventDetected){
            // Control if first the user has accelerated
            controlPlayerSpeed(speed, eventDetected, app);
        }
        // Selection of the correct sprite of the motorbike
        if (actual_code_image >= 1 && actual_code_image <= 4){
            actual_code_image += 20;
        }
        else if (actual_code_image >= 21 && actual_code_image <= 24){
            if (actual_code_image != 24){
                actual_code_image++;
            }
            else {
                actual_code_image--;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image >= 5 && actual_code_image <= 12){
            actual_code_image += 20;
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image >= 13 && actual_code_image <= 20){
            actual_code_image += 20;
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image >= 25 && actual_code_image <= 32){
            // Increment the actual code of the sprite
            if (actual_code_image != 32){
                // Increment the texture of the sprite
                actual_code_image++;
            }
            else {
                // Change sprite while the motorbike is turning to left
                actual_code_image--;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        else if (actual_code_image >= 33 && actual_code_image <= 40){
            // Increment the actual code of the sprite
            if (actual_code_image != 40){
                // Increment the texture of the sprite
                actual_code_image++;
            }
            else {
                // Change sprite while the motorbike is turning to left
                actual_code_image--;
            }
            // Set the texture from the file
            playerSprite.setTexture(textures[actual_code_image - 1]);
        }
        // Reduce the speed
        if (speed > INITIAL_SPEED){
            // Increment of the speed
            speed -= SPEED_INCREMENT;
        }
        // Detect event
        eventDetected = true;
    }
}



/**
 * Control if the player has done any of his possible actions
 * @param speed is the actual speed of the Ferrari of the player
 * @param eventDetected is a boolean to control if an event has occurred
 * @param app is the console window game where the sprite is going to be drawn
 */
void Ferrari::controlActionPlayer(int& speed, bool& eventDetected, RenderWindow& app){
    if (typeControl == 0){
        // Keyword
        // Check if W keyword has been pressed to turn to the right
        controlTurningPlayerRightKeyboard(speed, eventDetected, app);

        // Check if Q keyword has been pressed to turn to the left
        controlTurningPlayerLeftKeyboard(speed, eventDetected, app);
    }
    else {
        // Mouse
        // Check if the mouse has has been moved to turn to the right
        controlTurningPlayerRightMouse(speed, eventDetected, app);

        // Check if the mouse has has been moved to turn to the left
        controlTurningPlayerLeftMouse(speed, eventDetected, app);
    }

    // Check if the Up keyword has been pressed to increase the speed
    controlPlayerSpeed(speed, eventDetected, app);

    //Check if the E keyword has been pressed to brake the Ferrari
    controlPlayerBraking(speed, eventDetected, app);

    // Check if any event has been registered
    if (!eventDetected){
        // Reduce the speed
        if (speed > INITIAL_SPEED){
            // Increment of the speed
            speed -= SPEED_INCREMENT;
        }
    }
}



/**
 * Control if the player has have collision with the nearest element of the map to him
 * @param nearestStep is the step of the scene where is located the nearest element to the player
 * @param lastPos is the last position of the Ferrari in the axis Y
 * @param pos is the current position of the Ferrari in the axis Y
 */
bool Ferrari::controlPossibleCollision(Step& nearestStep, int& lastPos, int& pos){
    // Calculation of the distance
    float distance = nearestStep.spriteX - playerX;
    // Check the sign of the offset
    if (distance > 0){
        // Right
        // Check if the coordinates in the axis X are more less equal to have a collision
        if (distance >= nearestStep.offset - 0.2 && distance <= nearestStep.offset + 0.075){
            // Check if there is a direct collision or indirect collision in axis Y
            if ((pos == nearestStep.spriteY) || (lastPos <= nearestStep.spriteY && nearestStep.spriteY <= pos)){
                // Collision
                return true;
            }
            else {
                // There is not collision in spite the coordinates in axis X are equal
                return false;
            }
        }
        // There is not collision because the coordinates in axis X are different
        return false;
    }
    else {
        // Left
        // Check if the coordinates in the axis X are more less equal to have a collision
        if (distance >= nearestStep.offset - 0.075 && distance <= nearestStep.offset + 0.2){
            // Check if there is a direct collision or indirect collision in axis Y
            if ((pos == nearestStep.spriteY) || (lastPos <= nearestStep.spriteY && nearestStep.spriteY <= pos)){
                // Collision
                return true;
            }
            else {
                // There is not collision in spite the coordinates in axis X are equal
                return false;
            }
        }
        // There is not collision because the coordinates in axis X are different
        return false;
    }
}



/**
 * Control if there is there inertia force or not if the Ferrari is on a curve of the scene
 * @param onCurve is a boolean which represents if the Ferrari is on curve or not
 * @param curve is the possible curve of the scene where the Ferrari is currently now
 * @param speed is the actual speed of the Ferrari of the player
 */
void Ferrari::controlInertiaForce(bool& onCurve, IntervalCurve& curve, int& speed){
    // Check if there has to appear inertia force
    if (onCurve){
        // The Ferrari is on a curve of the scene
        onCurve = false;
        // Check the direction of the curve
        if (curve.directionCurve > 0.f){
            // Check if the Ferrari
            if (speed >= MEDIUM_SPEED){
                // Ferrari goes to the left when it is a right curve
                playerX -= 0.075;
            }
            else if (speed >= CONTROL_SPEED && speed < MEDIUM_SPEED) {
                playerX -= 0.045;
            }
            else {
                playerX -= 0.015;
            }
        }
        else {
            // Check if the Ferrari
            if (speed >= MEDIUM_SPEED){
                // Ferrari goes to the left when it is a right curve
                playerX += 0.075;
            }
            else if (speed >= CONTROL_SPEED && speed < MEDIUM_SPEED) {
                playerX += 0.045;
            }
            else {
                playerX += 0.015;
            }
        }
    }
}



/**
 * Shows to the user how the Ferraris crushes
 */
void Ferrari::collisionShow(){
}






