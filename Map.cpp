/******************************************************************************
 * @file    Map.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <random>
#include <fstream>
#include "Map.hpp"

using namespace std;
using namespace sf;

#define BGS 0.525F // Background size
#define SEGL 100 // Segment length
#define ROADW 2000 // Road Width
#define RUMBLECOEFF 0.1f // Ruble size = Road size * Rumble coeff

void fileError() {
    cerr << "Error: Formato de fichero incorrecto." << endl;
    exit(1);
}

Map::Map(Config &c, const std::string &path, const std::string &bgName,
        const std::vector<std::string> &objectNames, bool random) : posX(0), posY(0) {
    bg.loadFromFile(path + bgName);
    bg.setRepeated(true);

    objects.reserve(objectNames.size());
    hitCoeff.reserve(objectNames.size());
    for (const string &s : objectNames) {
        // Load textures
        Texture t;
        t.loadFromFile(path + s);
        t.setSmooth(true);
        objects.push_back(t);

        // Load hit percentage from center
        ifstream fin(path + s + ".info");
        float coeff = 1.0f;
        if (fin.is_open()) {
            fin >> coeff;
            fin.close();
        }
        hitCoeff.push_back(coeff);
    }

    float z = 0; // Line position

    if (random) { // Random generation
        random_device rd;
        mt19937 generator(rd());
        uniform_real_distribution<float> dist(0.1f, 0.9f);
        uniform_int_distribution<unsigned int> distObj(0, objects.size() - 1);
        uniform_int_distribution<int> distBool(0, 1);

        const int maxLines = 4000;

        // Random elements in the map
        float c1 = dist(generator), c2 = -dist(generator);
        unsigned int fh1, fh2, sh1, sh2, ele, maxEle = 0;
        fh1 = uniform_int_distribution<unsigned int>(0, maxLines / 2 - 1)(generator);
        fh2 = uniform_int_distribution<unsigned int>(fh1, maxLines / 2)(generator);
        sh1 = uniform_int_distribution<unsigned int>(maxLines / 2, maxLines - 1)(generator);
        sh2 = uniform_int_distribution<unsigned int>(sh1, maxLines)(generator);
        ele = uniform_int_distribution<unsigned int>(0, maxLines - 180)(generator);

        lines.reserve(maxLines);
        for (unsigned int i = 0; i < maxLines; i++) {
            Line line, lineAux;

            // Colors
            line.road = i % 2 ? Color(107, 107, 107) : Color(105, 105, 105);
            line.grass = i % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
            line.drawDash = i % 2;

            // Elevation
            if (i >= ele && i <= maxLines - 180) // The last line will have the same Y as the first
                maxEle = i + 180;
            if (i >= ele && i <= maxEle)
                line.y = float(sin((i - ele) * M_PI / 180.0) * 1500.0);

            lineAux = line; // without objects

            // Curves in the first half of the map
            if (i > fh1 && i < fh2) line.curve = c1;

            // Curves in the second half of the map
            if (i > sh1 && i < sh2) line.curve = c2;

            // Random objects
            if (dist(generator) > 0.66f) {
                if (distBool(generator) == 0) { // Left
                    line.spriteLeft.spriteNum = distObj(generator);
                    line.spriteLeft.offset = dist(generator);
                }
                else { // Right
                    line.spriteRight.spriteNum  = distObj(generator);
                    line.spriteRight.offset = dist(generator);
                }
            }

            // For each normal line, 4 extras without objects for better visualization
            lineAux.z = z;
            z += SEGL;
            lines.push_back(lineAux);

            lineAux.z = z;
            z += SEGL;
            lines.push_back(lineAux);

            line.z = z;
            z += SEGL;
            lines.push_back(line);

            lineAux.z = z;
            z += SEGL;
            lines.push_back(lineAux);

            lineAux.z = z;
            z += SEGL;
            lines.push_back(lineAux);
        }
    }
    else { // Predefined map
        ifstream fin(path + "map.info");
        if (fin.is_open()) {
            bool comment = false, road = false, grass = false, end = false;

            Color roadRGB[2], grassRGB[2];
            float curveCoeff = 0.0f, elevationCoeff = 0.0f, elevationIndex = 0.0f;
            unsigned int lineIndex = 0;

            vector<string> buffer;
            string s;
            while (!end && !fin.eof()) {
                fin >> s;

                if (s.size() >= 2 && s[0] == '/' && s[1] == '*')
                    comment = true;
                if (comment) {
                    if (s.size() >= 2 && s[s.size() - 1] == '/' && s[s.size() - 2] == '*')
                        comment = false;
                }
                else {
                    buffer.push_back(s);

                    if (!road) {
                        if (buffer.size() == 6) {
                            roadRGB[0] = Color(stoi(buffer[0]), stoi(buffer[1]), stoi(buffer[2]));
                            roadRGB[1] = Color(stoi(buffer[3]), stoi(buffer[4]), stoi(buffer[5]));
                            buffer.clear();
                            road = true;
                        }
                    }
                    else if (!grass) {
                        if (buffer.size() == 6) {
                            grassRGB[0] = Color(stoi(buffer[0]), stoi(buffer[1]), stoi(buffer[2]));
                            grassRGB[1] = Color(stoi(buffer[3]), stoi(buffer[4]), stoi(buffer[5]));
                            buffer.clear();
                            grass = true;
                        }
                    }
                    else if (buffer.size() > 1 && (s == "ROAD" || s == "CURVE" || s == "STRAIGHT" || s == "CLIMB" ||
                             s == "FLAT" || s == "DROP" || s == "END")) {
                        if (buffer[0] == "ROAD") {
                            if (buffer.size() < 3) // Checkpoint
                                fileError();

                            Line line, lineAux;

                            // Colors
                            line.road = roadRGB[lineIndex % 2];
                            line.grass = grassRGB[lineIndex % 2];
                            line.drawDash = lineIndex % 2;

                            // Indexes and positions
                            line.y = sqrt(abs(elevationIndex * elevationCoeff));
                            if (elevationIndex < 0.0f)
                                line.y *= 1.0f;

                            if (elevationCoeff > 0.0f)
                                elevationIndex++;
                            else if (elevationCoeff < 0.0f)
                                elevationIndex--;

                            lineAux = line; // without objects

                            line.curve = curveCoeff;
                            lineIndex++;

                            // Objects
                            int i = 1; // Buffer index
                            if (buffer[i] != "-") { // Left object
                                if (buffer[i] == "+") {
                                    line.spriteLeft.repetitive = true;
                                    i++;
                                }
                                line.spriteLeft.spriteNum = stoi(buffer[i]) - 1;
                                i++;
                                if (buffer[i] != "-") {
                                    line.spriteLeft.offset = stof(buffer[i]);
                                    i++;
                                }
                            }
                            if (i >= buffer.size() || buffer[i] != "-") { // Checkpoint
                                fileError();
                            }
                            i++;
                            if (i < buffer.size() - 1) { // Right object
                                if (buffer[i] == "+") {
                                    line.spriteRight.repetitive = true;
                                    i++;
                                }
                                line.spriteRight.spriteNum = stoi(buffer[i]) - 1;
                                i++;
                                if (i < buffer.size() - 1) {
                                    line.spriteRight.offset = stof(buffer[i]);
                                    i++;
                                }
                            }
                            if (i != buffer.size() - 1) { // Checkpoint
                                fileError();
                            }

                            // For each normal line, 4 extras without objects for better visualization
                            lineAux.z = z;
                            z += SEGL;
                            lines.push_back(lineAux);

                            lineAux.z = z;
                            z += SEGL;
                            lines.push_back(lineAux);

                            line.z = z;
                            z += SEGL;
                            lines.push_back(line);

                            lineAux.z = z;
                            z += SEGL;
                            lines.push_back(lineAux);

                            lineAux.z = z;
                            z += SEGL;
                            lines.push_back(lineAux);
                        }
                        else if (buffer[0] == "CURVE") {
                            if (buffer.size() < 3)
                                fileError();
                            curveCoeff = stof(buffer[1]);
                        }
                        else if (buffer[0] == "STRAIGHT") {
                            if (buffer.size() < 2)
                                fileError();
                            curveCoeff = 0.0f;
                        }
                        else if (buffer[0] == "CLIMB") {
                            if (buffer.size() < 3)
                                fileError();
                            elevationCoeff = stof(buffer[1]);
                        }
                        else if (buffer[0] == "FLAT") {
                            if (buffer.size() < 2)
                                fileError();
                            elevationCoeff = 0.0f;
                        }
                        else if (buffer[0] == "DROP") {
                            if (buffer.size() < 3)
                                fileError();
                            elevationCoeff = -stof(buffer[1]);
                        }
                        else if (buffer[0] == "END") {
                            end = true;
                        }

                        buffer.clear();
                        buffer.push_back(s);
                    }
                }
            }
            fin.close();
        }

        if (lines.empty())
            fileError();

        // This is in case there are too few lines
        const unsigned long moreLines = c.renderLen - lines.size();
        const unsigned long N = lines.size();
        for (unsigned long i = 1; i <= moreLines; i++) {
            z += SEGL;
            Line l = lines[(N - i) % N];
            l.z = z;
            lines.push_back(l);
        }
    }
}

void Map::updateView(pair<float, float> pos) {
    this->posX = pos.first;
    this->posY = pos.second;
}

void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2) {
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1, y1));
    shape.setPoint(1, Vector2f(x2, y2));
    shape.setPoint(2, Vector2f(x2 + w2, y2));
    shape.setPoint(3, Vector2f(x1 + w1, y1));
    w.draw(shape);
}

void Map::draw(Config &c) {
    int N = lines.size();

    // Background
    drawQuad(c.w, lines[0].grass, 0, 0, c.w.getSize().x, 0, c.w.getSize().y, c.w.getSize().x);
    Sprite sbg;
    sbg.setTexture(bg);
    sbg.setScale(Vector2f(2.0f * (float)c.w.getSize().x / bg.getSize().x, (float)c.w.getSize().y * BGS / bg.getSize().y));
    sbg.setPosition(0, 0);
    sbg.move(-sbg.getGlobalBounds().width / 3.0f - posX, 0);
    if (lines[int(posY) % N].curve > 0.0f)
        sbg.move(XINC / 2.0f, 0);
    else if (lines[int(posY) % N].curve < 0.0f)
        sbg.move(-XINC / 2.0f, 0);
    c.w.draw(sbg);

    int startPos = int(posY) % N;
    float camH = lines[startPos].y + 1500.0f;

    float maxy = c.w.getSize().y;
    float x = 0, dx = 0;

    ///////draw road////////
    for (int n = startPos; n < startPos + c.renderLen; n++) {
        Line &l = lines[n % N];
        l.project(posX * ROADW - x, camH, float(startPos * SEGL - (n >= N ? N * SEGL : 0)), c.camD,
                  c.w.getSize().x, c.w.getSize().y, ROADW);
        x += dx;
        dx += l.curve;

        l.clip = maxy;
        if (l.Y < maxy) {
            maxy = l.Y;

            Color rumble = l.drawDash ? l.road : Color::White;
            Color dash = l.drawDash ? Color::White : l.road;

            Line p = lines[(n - 1) % N]; //previous line

            // Draw grass
            drawQuad(c.w, l.grass, 0, int(p.Y), c.w.getSize().x, 0, int(l.Y), c.w.getSize().x);

            // Draw road
            const int x1 = int(p.X - p.W), y1 = int(p.Y), w1 = int(2.0f * p.W),
                      x2 = int(l.X - l.W), y2 = int(l.Y), w2 = int(2.0f * l.W),
                      rw1 = int(p.W * RUMBLECOEFF), rw2 = int(l.W * RUMBLECOEFF),
                      dw1 = rw1 / 2, dw2 = rw2 / 2;
            drawQuad(c.w, l.road, x1, y1, w1, x2, y2, w2);
            drawQuad(c.w, rumble, x1, y1, rw1, x2, y2, rw2); // Left rumble
            drawQuad(c.w, rumble, x1 + w1 - rw1, y1, rw1, x2 + w2 - rw2, y2, rw2); // Right rumble
            drawQuad(c.w, dash, x1 + rw1, y1, dw1, x2 + rw2, y2, dw2); // First left dash
            drawQuad(c.w, dash, x1 + w1 - rw1 - dw1, y1, dw1, x2 + w2 - rw2 - dw2, y2, dw2); // First right dash
            drawQuad(c.w, dash, x1 + int(float(w1) * 0.333f), y1, dw1, x2 + int(float(w2) * 0.333f), y2, dw2); // Second left dash
            drawQuad(c.w, dash, x1 + int(float(w1) * 0.666f), y1, dw1, x2 + int(float(w2) * 0.666f), y2, dw2); // Second right dash
        }
    }

    ////////draw objects////////
    for (int n = startPos + c.renderLen; n > startPos; n--) {
        if (lines[n % N].spriteLeft.spriteNum > -1)
            lines[n % N].drawSprite(c.w, objects, hitCoeff, lines[n % N].spriteLeft, true);
        if (lines[n % N].spriteRight.spriteNum > -1)
            lines[n % N].drawSprite(c.w, objects, hitCoeff, lines[n % N].spriteRight, false);
    }

}

Map::SpriteInfo::SpriteInfo() {
    spriteNum = -1;
    offset = spriteMinX = spriteMaxX = 0.0f;
    repetitive = false;
}

Map::Line::Line() {
    curve = x = y = z = 0;
    drawDash = false;
}

void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW) {
    scale = camD / (1.0f + z - camZ);
    X = (1.0f + scale * (x - camX)) * width / 2.0f;
    Y = (1.0f - scale * (y - camY)) * height / 2.0f;
    W = scale * rW  * width / 2.0f;
}

void Map::Line::drawSprite(RenderWindow &w, const vector<Texture> &objs, const vector<float> &coeff, SpriteInfo &object,
        bool left) {
    Sprite s(objs[object.spriteNum]);
    const int width = s.getTextureRect().width;
    const int h = s.getTextureRect().height;

    float destY = Y + 4.0f;
    float destW = float(width) * W / 266.0f;
    float destH = float(h) * W / 266.0f;

    destY += destH * (-1.0f); //offsetY

    float clipH = destY + destH - clip;
    if (clipH < 0)
        clipH = 0;

    if (clipH >= destH) return;
    s.setTextureRect(IntRect(0, 0, width, float(h) - float(h) * clipH / destH));
    s.setScale(destW / float(width), destH / float(h));

    float destX = X + W + object.offset * s.getGlobalBounds().width; // Right road side
    if (left)
        destX = X - W - s.getGlobalBounds().width - object.offset * s.getGlobalBounds().width; // Left road side
    s.setPosition(destX, destY);
    w.draw(s);

    // Repetitive drawing
    if (left && object.repetitive) {
        while (destX >= 0.0f) {
            destX -= s.getGlobalBounds().width;
            s.setPosition(destX, destY);
            w.draw(s);
        }

        object.spriteMinX = 0;
    }
    else {
        object.spriteMinX = destX +
                (s.getGlobalBounds().width - s.getGlobalBounds().width * coeff[object.spriteNum]) / 2.0f;
    }

    if (!left && object.repetitive) {
        while (destX <= w.getSize().x) {
            destX += s.getGlobalBounds().width;
            s.setPosition(destX, destY);
            w.draw(s);
        }

        object.spriteMaxX = w.getSize().x;
    }
    else {
        object.spriteMaxX = object.spriteMinX + s.getGlobalBounds().width * coeff[object.spriteNum];
    }
}

bool Map::hasCrashed(const Config &c, float prevY, float currentY, float minX, float maxX) const {
    const int N = lines.size();
    for (int n = int(posY); n < int(posY) + c.renderLen; n++) {
        const Line &l = lines[n % N];
        if (l.spriteLeft.spriteNum != -1 && l.spriteLeft.spriteMinX != l.spriteLeft.spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= l.spriteLeft.spriteMinX && minX <= l.spriteLeft.spriteMaxX) ||
                 (maxX >= l.spriteLeft.spriteMinX && maxX <= l.spriteLeft.spriteMaxX) ||
                 (l.spriteLeft.spriteMinX >= minX && l.spriteLeft.spriteMinX <= maxX) ||
                 (l.spriteLeft.spriteMaxX >= minX && l.spriteLeft.spriteMaxX <= maxX))) // x matches
            return true;
        if (l.spriteRight.spriteNum != -1 && l.spriteRight.spriteMinX != l.spriteRight.spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= l.spriteRight.spriteMinX && minX <= l.spriteRight.spriteMaxX) ||
                 (maxX >= l.spriteRight.spriteMinX && maxX <= l.spriteRight.spriteMaxX) ||
                 (l.spriteRight.spriteMinX >= minX && l.spriteRight.spriteMinX <= maxX) ||
                 (l.spriteRight.spriteMaxX >= minX && l.spriteRight.spriteMaxX <= maxX))) // x matches
            return true;
    }
    return false;
}

bool Map::hasGotOut(float currentX) const {
    return abs(currentX) > 1.0f;
}

float Map::getCurveCoefficient(float currentY) const {
    return lines[int(currentY) % lines.size()].curve;
}

Map::Elevation Map::getElevation(float currentY) const {
    const int N = lines.size();
    const int n = int(currentY);
    const Line &currentLine = lines[n % N];
    const Line &prevLine = lines[(n - 1) % N];
    if (n % N != 0 && abs(currentLine.y) > 1000 && currentLine.y > prevLine.y + 1.0f)
        return UP;
    else if (n % N != 0 && abs(currentLine.y) > 1000 && currentLine.y < prevLine.y - 1.0f)
        return DOWN;
    else
        return FLAT;
}
