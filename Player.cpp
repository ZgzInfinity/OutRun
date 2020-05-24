/******************************************************************************
 * @file    Player.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "Player.hpp"
#include "Random.hpp"
#include <cmath>

using namespace std;
using namespace sf;

#define PLAYER_TEXTURES 136
#define MAX_INERTIA 10

Player::Player(float maxSpeed, float speedMul, float accInc, float scaleX, float scaleY, int maxCounterToChange,
        const string &vehicle, float pX, float pY) : Vehicle(maxSpeed / speedMul, scaleX, maxCounterToChange, 0.0f, pX, pY, pY, 0, 0,
                       vehicle, PLAYER_TEXTURES, 1, 0), speedMul(speedMul),
                       maxAcc(pow(maxSpeed / speedMul, 2.0f)), accInc(accInc), scaleY(scaleY), acceleration(0),
                       minCrashAcc(0), xDest(0), inertia(0), crashing(false), smoking(false), skidding(false),
                       firstCrash(true), firstTurnLeft(true), firstTurnRight(true) {}

float Player::getPreviousY() const {
    return previousY;
}

void Player::hitControl(const bool vehicleCrash) {
    crashing = true;
    smoking = false;
    skidding = false;

    if (!vehicleCrash) {
        if (posX > XINC)
            posX -= XINC;
        else if (posX < -XINC)
            posX += XINC;
    }
    else {
        if (minCrashAcc <= 0.0f) { // Only first time
            minCrashAcc = acceleration * 0.5f; // In case of car crash, acc will be drop 1/2. Otherwise it will be drop to 0.

            if (posX < 0.0f)
                xDest = 0.8f;
            else
                xDest = -0.8f;
        }

        if (posX > xDest)
            posX -= XINC;
        else if (posX < xDest)
            posX += XINC;

        if (posX > 0.8f)
            posX = 0.8f;
        else if (posX < -0.8f)
            posX = -0.8f;
    }

    acceleration -= accInc * 2.5f;
    if (speed > 1.333f * halfMaxSpeed) // Reduces hit time
        acceleration -= accInc * 7.5f;
    else if (speed > halfMaxSpeed)
        acceleration -= accInc * 5.0f;
    else if (speed > 0.5f * halfMaxSpeed)
        acceleration -= accInc * 2.5f;

    if (acceleration < 0.0f)
        acceleration = 0.0f;

    speed = sqrt(acceleration);

    previousY = posY;
    if (vehicleCrash && abs(posX) != 0.8f)
        posY -= speed / 2.0f;
    if (!vehicleCrash && (posX <= -XINC || posX >= XINC))
        posY -= speed / 6.0f;

    if (acceleration <= minCrashAcc) {
        acceleration = minCrashAcc;
        speed = sqrt(acceleration);
        crashing = false;
        minCrashAcc = 0.0f;
        xDest = 0.0f;
        inertia = 0;

        previousY = posY;
    }
}

bool Player::isCrashing() const {
    return crashing;
}

float Player::getRealSpeed() const {
    return speed * speedMul;
}

Vehicle::Action Player::accelerationControl(Config &c, bool hasGotOut) {
    Action a = NONE;
    smoking = false;
    float previousAcc = acceleration;

    if (Keyboard::isKeyPressed(c.brakeKey))
        a = BRAKE;

    if (a != BRAKE && Keyboard::isKeyPressed(c.accelerateKey)) {
        if (hasGotOut) {
            if (acceleration < maxAcc / 4.5f)
                acceleration += accInc / 3.0f;
            else
                acceleration -= accInc * 1.5f;
        }
        else {
            if (acceleration < maxAcc)
                acceleration += accInc;
        }

        if (acceleration > maxAcc)
            acceleration = maxAcc;

        smoking = acceleration < maxAcc * 0.1f;
    }
    else {
        float mul = 2.0f;
        if (a == BRAKE)
            mul *= 2.0f;
        if (hasGotOut)
            mul *= 1.5f;

        if (acceleration > 0.0f)
            acceleration -= accInc * mul;

        if (acceleration < 0.0f)
            acceleration = 0.0f;
    }

    if (previousAcc == 0.0f && acceleration > 0.0f)
        a = BOOT;
    else if (a == NONE && acceleration > 0.0f)
        a = ACCELERATE;

    speed = sqrt(acceleration);
    if (speed > 0.0f) {
        previousY = posY;
        posY += speed;
    }

    return a;
}

Vehicle::Direction Player::rotationControl(Config &c, float curveCoefficient) {
    skidding = false;

    if (speed > 0.0f) {
        posX -= XINC * curveCoefficient * sqrt(speed) * speed / maxSpeed;
        if (abs(curveCoefficient) >= 0.33f && speed >= 0.66f * maxSpeed)
            skidding = true;

        if (Keyboard::isKeyPressed(c.leftKey)) {
            if (inertia > -MAX_INERTIA)
                inertia--;

            if (inertia < 0) {
                if (curveCoefficient > 0.0f)
                    skidding = false;

                if (speed < halfMaxSpeed)
                    posX -= 1.5f * XINC * speed / maxSpeed;
                else if (curveCoefficient == 0.0f)
                    posX -= 1.25f * XINC * speed / maxSpeed;
                else
                    posX -= XINC * speed / maxSpeed;

                return TURNLEFT;
            }
        }
        else if (Keyboard::isKeyPressed(c.rightKey)) {
            if (inertia < MAX_INERTIA)
                inertia++;

            if (inertia > 0) {
                if (curveCoefficient < 0.0f)
                    skidding = false;

                if (speed < halfMaxSpeed)
                    posX += 1.5f * XINC * speed / maxSpeed;
                else if (curveCoefficient == 0.0f)
                    posX += 1.25f * XINC * speed / maxSpeed;
                else
                    posX += XINC * speed / maxSpeed;

                return TURNRIGHT;
            }
        }
        else if (inertia > 0) {
            inertia--;
        }
        else if (inertia < 0) {
            inertia++;
        }

        skidding = false;
    }

    return RIGHT;
}

void Player::draw(Config &c, const Action &a, const Direction &d, const Elevation &e, bool enableSound) {
    // Sound effects
    if (a != CRASH)
        firstCrash = true;

    if (enableSound) {
        if (speed > 0.0f) {
            if (a == BOOT) {
                // Acceleration sound
                c.effects[12]->stop();
                c.effects[12]->play();
            }
            if (c.effects[6]->getStatus() != SoundSource::Playing) {
                // Engine sound
                c.effects[6]->stop();
                c.effects[6]->play();
            }
            c.effects[6]->setVolume((33.0f + 67.0f * speed / maxSpeed) * c.volumeEffects / 100.f);
            if (skidding && c.effects[8]->getStatus() != SoundSource::Playing) {
                // Skidding sound
                c.effects[8]->stop();
                c.effects[8]->play();
            }
        }
        else {
            c.effects[12]->stop();
            c.effects[6]->stop();
            c.effects[8]->stop();
        }

        if (a == CRASH && firstCrash) {
            firstCrash = false;
            c.effects[17]->stop();
            c.effects[18]->stop();
            c.effects[19]->stop();
            c.effects[17]->play();
            c.effects[random_int(18, 19)]->play();
        }
    }
    else {
        c.effects[12]->stop();
        c.effects[6]->stop();
        c.effects[8]->stop();
        c.effects[17]->stop();
        c.effects[18]->stop();
        c.effects[19]->stop();
    }

    // Draw
    if (d != TURNLEFT)
        firstTurnLeft = true;
    if (d != TURNRIGHT)
        firstTurnRight = true;

    if (a != NONE) {
        if (counter_code_image >= maxCounterToChange) {
            counter_code_image = 0;

            if (speed > 0.0f)
                current_code_image++;

            if (textures.size() == PLAYER_TEXTURES) {
                if (a == ACCELERATE || a == BOOT) {
                    if (e == FLAT) {
                        if (d == RIGHT) {
                            if (current_code_image < 1 || current_code_image > 4)
                                current_code_image = 1;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 5 || current_code_image > 8)
                                    current_code_image = 5;
                                if (current_code_image == 8)
                                    firstTurnLeft = false;
                            }
                            else {
                                if (current_code_image < 9 || current_code_image > 12)
                                    current_code_image = 9;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 13 || current_code_image > 16)
                                    current_code_image = 13;
                                if (current_code_image == 16)
                                    firstTurnRight = false;
                            }
                            else {
                                if (current_code_image < 17 || current_code_image > 20)
                                    current_code_image = 17;
                            }
                        }
                    }
                    else if (e == UP) {
                        if (d == RIGHT) {
                            if (current_code_image < 21 || current_code_image > 24)
                                current_code_image = 21;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 25 || current_code_image > 28)
                                    current_code_image = 25;
                                if (current_code_image == 28)
                                    firstTurnLeft = false;
                            }
                            else {
                                if (current_code_image < 29 || current_code_image > 32)
                                    current_code_image = 29;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 33 || current_code_image > 36)
                                    current_code_image = 33;
                                if (current_code_image == 36)
                                    firstTurnRight = false;
                            }
                            else {
                                if (current_code_image < 37 || current_code_image > 40)
                                    current_code_image = 37;
                            }
                        }
                    }
                    else { // Down
                        if (d == RIGHT) {
                            if (current_code_image < 41 || current_code_image > 44)
                                current_code_image = 41;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 45 || current_code_image > 48)
                                    current_code_image = 45;
                                if (current_code_image == 48)
                                    firstTurnLeft = false;
                            }
                            else {
                                if (current_code_image < 49 || current_code_image > 52)
                                    current_code_image = 49;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 53 || current_code_image > 56)
                                    current_code_image = 53;
                                if (current_code_image == 56)
                                    firstTurnRight = false;
                            }
                            else {
                                if (current_code_image < 57 || current_code_image > 60)
                                    current_code_image = 57;
                            }
                        }
                    }
                }
                else if (a == BRAKE) {
                    if (e == FLAT) {
                        if (d == RIGHT) {
                            if (current_code_image < 61 || current_code_image > 64)
                                current_code_image = 61;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 65 || current_code_image > 68)
                                    current_code_image = 65;
                                if (current_code_image == 68)
                                    firstTurnLeft = false;
                            }
                            else {
                                if (current_code_image < 69 || current_code_image > 72)
                                    current_code_image = 69;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 73 || current_code_image > 76)
                                    current_code_image = 73;
                                if (current_code_image == 76)
                                    firstTurnRight = false;
                            }
                            else {
                                if (current_code_image < 77 || current_code_image > 80)
                                    current_code_image = 77;
                            }
                        }
                    }
                    else if (e == UP) {
                        if (d == RIGHT) {
                            if (current_code_image < 81 || current_code_image > 84)
                                current_code_image = 81;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 85 || current_code_image > 88)
                                    current_code_image = 85;
                                if (current_code_image == 88)
                                    firstTurnLeft = false;
                            }
                            else {
                                if (current_code_image < 89 || current_code_image > 92)
                                    current_code_image = 89;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 93 || current_code_image > 96)
                                    current_code_image = 93;
                                if (current_code_image == 96)
                                    firstTurnRight = false;
                            }
                            else {
                                if (current_code_image < 97 || current_code_image > 100)
                                    current_code_image = 97;
                            }
                        }
                    }
                    else { // Down
                        if (d == RIGHT) {
                            if (current_code_image < 101 || current_code_image > 104)
                                current_code_image = 101;
                        }
                        else if (d == TURNLEFT) {
                            if (firstTurnLeft) {
                                if (current_code_image < 105 || current_code_image > 108)
                                    current_code_image = 105;
                                if (current_code_image == 108)
                                    firstTurnLeft = false;
                            }
                            else {
                                if (current_code_image < 109 || current_code_image > 112)
                                    current_code_image = 109;
                            }
                        }
                        else { // Turn right
                            if (firstTurnRight) {
                                if (current_code_image < 113 || current_code_image > 116)
                                    current_code_image = 113;
                                if (current_code_image == 116)
                                    firstTurnRight = false;
                            }
                            else {
                                if (current_code_image < 117 || current_code_image > 120)
                                    current_code_image = 117;
                            }
                        }
                    }
                }
                else {
                    // Crash
                    if (posX > 0.0f) {
                        if (posX > XINC) {
                            if (current_code_image < 121 || current_code_image > 124)
                                current_code_image = 121;
                        }
                        else {
                            if (current_code_image < 129 || current_code_image > 130)
                                current_code_image = 129;
                        }
                    }
                    else {
                        if (posX < -XINC) {
                            if (current_code_image < 125 || current_code_image > 128)
                                current_code_image = 125;
                        }
                        else {
                            if (current_code_image < 131 || current_code_image > 132)
                                current_code_image = 131;
                        }
                    }
                }
            }
        }
        else {
            counter_code_image++;
        }
    }
    else {
        current_code_image = 1;
    }

    sprite.setTexture(textures[current_code_image - 1], true);
    sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
    minScreenX = ((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
    maxScreenX = minScreenX + sprite.getGlobalBounds().width;
    sprite.setPosition(minScreenX, ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f);
    c.w.draw(sprite);

    if (smoking || skidding) {
        const float j = sprite.getPosition().y + sprite.getGlobalBounds().height;
        sprite.setTexture(textures[132 + current_code_image % 4], true);
        sprite.setScale(4.0f * c.screenScale, 4.0f * c.screenScale);
        sprite.setPosition(((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width,
                j - sprite.getGlobalBounds().height);
        c.w.draw(sprite);
        sprite.setPosition(((float)c.w.getSize().x) / 2.0f , j - sprite.getGlobalBounds().height);
        c.w.draw(sprite);
    }
}

void Player::drawInitialAnimation(Config &c, float x, bool &end) {
    if (textures.size() == PLAYER_TEXTURES) {
        if (counter_code_image >= maxCounterToChange) {
            current_code_image++;
            counter_code_image = 0;
        }
        if (current_code_image < 132 || current_code_image > 135)
            current_code_image = 132;

        int index = 125;
        if (float(x) < ((float)c.w.getSize().x) * 0.45f)
            index = 124;
        end = float(x) < ((float)c.w.getSize().x) * 0.4f || x >= c.w.getSize().x;

        // Vehicle
        sprite.setTexture(textures[index], true);
        sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
        sprite.setPosition(x, ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f);
        c.w.draw(sprite);

        // Smoke
        float i = x - sprite.getGlobalBounds().width / 3, j = sprite.getPosition().y + sprite.getGlobalBounds().height;
        while (i < (float)c.w.getSize().x) {
            index = current_code_image;
            sprite.setTexture(textures[index], true);
            sprite.setScale(4.0f * c.screenScale, 4.0f * c.screenScale);
            sprite.setPosition(i, j - sprite.getGlobalBounds().height);
            c.w.draw(sprite);

            i += sprite.getGlobalBounds().width;
        }

        if (end) {
            current_code_image = 0;
            counter_code_image = 0;
        }
        else {
            counter_code_image++;
        }
    }
}

void Player::drawGoalAnimation(Config &c, int &step, bool &end, bool smoke) {
    if (textures.size() == PLAYER_TEXTURES) {
        if (counter_code_image >= maxCounterToChange) {
            current_code_image++;
            counter_code_image = 0;
            step++;
        }
        if (current_code_image < 132 || current_code_image > 135)
            current_code_image = 132;

        int index = 126;
        if (step < 2)
            index = 124;
        else if (step < 4)
            index = 125;

        // Vehicle
        sprite.setTexture(textures[index], true);
        sprite.setScale(scale * c.screenScale, scaleY * c.screenScale);
        minScreenX = ((float)c.w.getSize().x) / 2.0f - sprite.getGlobalBounds().width / 2.0f;
        maxScreenX = minScreenX + sprite.getGlobalBounds().width;
        sprite.setPosition(minScreenX, ((float)c.w.getSize().y) * c.camD - sprite.getGlobalBounds().height / 2.0f);
        c.w.draw(sprite);

        // Smoke
        if (smoke) {
            float i = minScreenX - sprite.getGlobalBounds().width / 3, j =
                    sprite.getPosition().y + sprite.getGlobalBounds().height;
            while (i < (float) c.w.getSize().x) {
                index = current_code_image;
                sprite.setTexture(textures[index], true);
                sprite.setScale(4.0f * c.screenScale, 4.0f * c.screenScale);
                sprite.setPosition(i, j - sprite.getGlobalBounds().height);
                c.w.draw(sprite);

                i += sprite.getGlobalBounds().width;
            }
        }

        if (step >= 6)
            end = true;

        if (end) {
            current_code_image = 0;
            counter_code_image = 0;
        }
        else {
            counter_code_image++;
        }
    }
}

void Player::setSmoking(bool smoke) {
    smoking = smoke;
}
