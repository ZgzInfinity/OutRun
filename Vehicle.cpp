/******************************************************************************
 * @file    Vehicle.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Vehicle.hpp"
#include <cmath>

using namespace std;
using namespace sf;

Vehicle::Vehicle(float maxSpeed, float speedMul, float accInc, float scale, int numTextures,
                 int maxCounterToChange, const string &vehicle) : speedMul(speedMul), maxAcc(pow(maxSpeed / speedMul, 2.0f)),
                 accInc(accInc), scale(scale), maxCounterToChange(maxCounterToChange), speed(0), acceleration(0),
                 posX(0), posY(0), actual_code_image(1), counter_code_image(0) {
    for (int i = 1; i <= numTextures; i++) {
        Texture t;
        t.loadFromFile("resources/" + vehicle + "/c" + to_string(i) + ".png");
        t.setSmooth(true);
        t.setRepeated(false);
        textures.push_back(t);
    }
}

std::pair<float, float> Vehicle::getPosition() {
    return make_pair(posX, posY);
}

float Vehicle::getRealSpeed() {
    return speed * speedMul;
}

Vehicle::Action Vehicle::accelerationControl(Config &c) {
    Action a = NONE;

    if (Keyboard::isKeyPressed(c.brakeKey))
        a = BRAKE;

    if (a != BRAKE && Keyboard::isKeyPressed(c.accelerateKey)) {
        if (acceleration < maxAcc)
            acceleration += accInc;

        if (acceleration > maxAcc)
            acceleration = maxAcc;
    }
    else {
        float mul = 2.0f;
        if (a == BRAKE)
            mul = 4.0f;

        if (acceleration > 0.0f)
            acceleration -= accInc * mul;

        if (acceleration < 0.0f)
            acceleration = 0.0f;
    }

    if (a == NONE && acceleration > 0.0f)
        a = ACCELERATE;

    speed = sqrt(acceleration);
    posY += speed;

    return a;
}

Vehicle::Direction Vehicle::rotationControl(Config &c) {
    if (speed > 0.0f) {
        if (Keyboard::isKeyPressed(c.leftKey)) {
            posX -= 0.05f;
            return TURNLEFT;
        }

        if (Keyboard::isKeyPressed(c.rightKey)) {
            posX += 0.05f;
            return TURNRIGHT;
        }
    }

    return RIGHT;
}

void Vehicle::draw(Config &c, Vehicle::Action a, Vehicle::Direction d) {
    if (a != NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            if (speed > 0.0f)
                actual_code_image++;

            if (a == ACCELERATE) {
                if (d == TURNLEFT) {
                    if (actual_code_image < 10 || actual_code_image > 11)
                        actual_code_image = 10;
                }
                else if (d == TURNRIGHT) {
                    if (actual_code_image < 14 || actual_code_image > 15)
                        actual_code_image = 14;
                }
                else {
                    if (actual_code_image < 1 || actual_code_image > 4)
                        actual_code_image = 1;
                }
            }
            else if (a == BRAKE) {
                if (d == TURNLEFT) {
                    if (actual_code_image < 25 || actual_code_image > 28)
                        actual_code_image = 25;
                }
                else if (d == TURNRIGHT) {
                    if (actual_code_image < 33 || actual_code_image > 36)
                        actual_code_image = 33;
                }
                else {
                    if (actual_code_image < 21 || actual_code_image > 24)
                        actual_code_image = 21;
                }
            }
        }
        else {
            counter_code_image++;
        }
    }
    else {
        actual_code_image = 1;
    }

    sprite.setTexture(textures[actual_code_image - 1]);
    sprite.setScale(scale, scale);
    sprite.setPosition(((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f,
                        ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f);
    c.w.draw(sprite);
}
