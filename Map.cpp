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

Map::SpriteInfo::SpriteInfo() {
    spriteNum = -1;
    offset = spriteMinX = spriteMaxX = 0.0f;
    repetitive = false;
}

Map::Line::Line() {
    curve = x = y = z = 0;
    mainColor = false;
}

void Map::addLine(float x, float y, float &z, float prevY, float curve, bool mainColor,
        const Map::SpriteInfo &spriteLeft, const Map::SpriteInfo &spriteRight) {
    float yInc = (y - prevY) / 5.0f; // 5 is total lines number will be added

    Line line, lineAux;
    line.x = x;
    line.y = prevY;
    line.mainColor = mainColor;
    line.curve = curve;

    lineAux = line; // without objects

    line.spriteLeft = spriteLeft;
    line.spriteRight = spriteRight;

    // For each normal line, 4 extras without objects for better visualization
    lineAux.z = z;
    z += SEGL;
    lineAux.y += yInc;
    lines.push_back(lineAux);

    lineAux.z = z;
    z += SEGL;
    lineAux.y += yInc;
    lines.push_back(lineAux);

    line.z = z;
    z += SEGL;
    lineAux.y += yInc;
    line.y = lineAux.y;
    lines.push_back(line);

    lineAux.z = z;
    z += SEGL;
    lineAux.y += yInc;
    lines.push_back(lineAux);

    lineAux.z = z;
    z += SEGL;
    lineAux.y += yInc;
    lines.push_back(lineAux);
}

Map::Line *Map::getLine(const int n) {
    if (n < lines.size() || next == nullptr)
        return &lines[n % lines.size()];
    else
        return &next->lines[(n - lines.size()) % next->lines.size()];
}

Map::Line Map::getLine(const int n) const {
    if (n < lines.size() || next == nullptr)
        return lines[n % lines.size()];
    else
        return next->lines[(n - lines.size()) % next->lines.size()];
}

Map::Line *Map::getPreviousLine(const int n) {
    if ((n > 0 && n - 1 < lines.size()) || next == nullptr)
        return &lines[(n - 1) % lines.size()];
    else
        return &next->lines[(n - 1 - lines.size()) % next->lines.size()];
}

Map::Line Map::getPreviousLine(const int n) const {
    if ((n > 0 && n - 1 < lines.size()) || next == nullptr)
        return lines[(n - 1) % lines.size()];
    else
        return next->lines[(n - 1 - lines.size()) % next->lines.size()];
}

void Map::addRandomLines(const float x, const float y, float &z, const int numLines, const vector<int> &objectIndexes) {
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> dist(0.1f, 0.9f);
    uniform_int_distribution<unsigned int> distObj(0, objectIndexes.size() - 1);
    uniform_int_distribution<int> distBool(0, 1);

    // Random elements in the map
    float c1 = dist(generator), c2 = -dist(generator);
    unsigned int fh1 = 0, fh2 = 0, sh1 = 0, sh2 = 0, ele = 0, maxEle = 0;
    if (numLines > 200) {
        fh1 = uniform_int_distribution<unsigned int>(0, numLines / 2 - 1)(generator);
        fh2 = uniform_int_distribution<unsigned int>(fh1, numLines / 2)(generator);
        sh1 = uniform_int_distribution<unsigned int>(numLines / 2, numLines - 1)(generator);
        sh2 = uniform_int_distribution<unsigned int>(sh1, numLines)(generator);
        ele = uniform_int_distribution<unsigned int>(0, numLines - 180)(generator);
    }

    lines.reserve(lines.size() + numLines);
    for (unsigned int i = 0; i < numLines; i++) {
        float yAux = y, curve = 0.0f;
        SpriteInfo spriteLeft, spriteRight;

        // Elevation
        if (i >= ele && i <= numLines - 180) // The last line will have the same Y as the first
            maxEle = i + 180;
        if (i >= ele && i <= maxEle)
            yAux += 5.0f * float(sin((i - ele) * M_PI / 180.0) * 1500.0);

        // Curves in the first half of the map
        if (i > fh1 && i < fh2) curve = c1;

        // Curves in the second half of the map
        if (i > sh1 && i < sh2) curve = c2;

        // Random objects
        if (!objectIndexes.empty() && dist(generator) > 0.66f) {
            if (distBool(generator) == 0) { // Left
                spriteLeft.spriteNum = objectIndexes[distObj(generator)];
                spriteLeft.offset = dist(generator);
            } else { // Right
                spriteRight.spriteNum = objectIndexes[distObj(generator)];
                spriteRight.offset = dist(generator);
            }
        }

        addLine(x, yAux, z, lines.empty() ? y : lines[lines.size() - 1].y, curve, i % 2, spriteLeft, spriteRight);
    }
}

void fileError() {
    cerr << "Error: Formato de fichero incorrecto." << endl;
    exit(1);
}

void Map::addLinesFromFile(float x, float y, float &z, const std::string &file) {
    ifstream fin(file);
    if (fin.is_open()) {
        bool comment = false, road = false, grass = false, end = false;
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
                        roadColor[0] = Color(stoi(buffer[0]), stoi(buffer[1]), stoi(buffer[2]));
                        roadColor[1] = Color(stoi(buffer[3]), stoi(buffer[4]), stoi(buffer[5]));
                        buffer.clear();
                        road = true;
                    }
                }
                else if (!grass) {
                    if (buffer.size() == 6) {
                        grassColor[0] = Color(stoi(buffer[0]), stoi(buffer[1]), stoi(buffer[2]));
                        grassColor[1] = Color(stoi(buffer[3]), stoi(buffer[4]), stoi(buffer[5]));
                        buffer.clear();
                        grass = true;
                    }
                }
                else if (buffer.size() > 1 && (s == "ROAD" || s == "CURVE" || s == "STRAIGHT" || s == "CLIMB" ||
                                               s == "FLAT" || s == "DROP" || s == "RANDOM" || s == "END")) {
                    if (buffer[0] == "ROAD") {
                        if (buffer.size() < 3) // Checkpoint
                            fileError();

                        float yAux;
                        SpriteInfo spriteLeft, spriteRight;

                        // Indexes and positions
                        yAux = sqrt(abs(elevationIndex * elevationCoeff));
                        if (elevationIndex < 0.0f)
                            yAux *= -1.0f;
                        yAux += y;

                        if (elevationCoeff > 0.0f)
                            elevationIndex++;
                        else if (elevationCoeff < 0.0f)
                            elevationIndex--;

                        lineIndex++;

                        // Objects
                        int i = 1; // Buffer index
                        if (buffer[i] != "-") { // Left object
                            if (buffer[i] == "+") {
                                spriteLeft.repetitive = true;
                                i++;
                            }
                            spriteLeft.spriteNum = stoi(buffer[i]) - 1;
                            i++;
                            if (buffer[i] != "-") {
                                spriteLeft.offset = stof(buffer[i]);
                                i++;
                            }
                        }
                        if (i >= buffer.size() || buffer[i] != "-") { // Checkpoint
                            fileError();
                        }
                        i++;
                        if (i < buffer.size() - 1) { // Right object
                            if (buffer[i] == "+") {
                                spriteRight.repetitive = true;
                                i++;
                            }
                            spriteRight.spriteNum = stoi(buffer[i]) - 1;
                            i++;
                            if (i < buffer.size() - 1) {
                                spriteRight.offset = stof(buffer[i]);
                                i++;
                            }
                        }
                        if (i != buffer.size() - 1) { // Checkpoint
                            fileError();
                        }

                        addLine(x, yAux, z, lines.empty() ? y : lines[lines.size() - 1].y, curveCoeff, lineIndex % 2,
                                spriteLeft, spriteRight);
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
                    else if (buffer[0] == "RANDOM") {
                        if (buffer.size() < 3)
                            fileError();

                        vector<int> objectIndexes;
                        for (int i = 2; i < buffer.size() - 1; i++)
                            objectIndexes.push_back(stoi(buffer[i]));

                        float currentY = y;
                        if (!lines.empty())
                            currentY = lines[lines.size() - 1].y;

                        addRandomLines(x, currentY, z, stoi(buffer[1]), objectIndexes);

                        y += lines[lines.size() - 1].y - currentY;
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
}

Map::Map(Config &c, const std::string &path, const std::string &bgName,
        const std::vector<std::string> &objectNames, bool random) : posX(0), posY(0), next(nullptr) {
    bg.loadFromFile(path + bgName);
    bg.setRepeated(true);

    int k = 0;
    vector<int> objectIndexes;
    objectIndexes.reserve(objectNames.size());
    objects.reserve(objectNames.size());
    hitCoeff.reserve(objectNames.size());
    for (const string &s : objectNames) {
        // Load indexes
        objectIndexes.push_back(k);
        k++;

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

    // Colors
    roadColor[0] = Color(107, 107, 107);
    roadColor[1] = Color(105, 105, 105);
    grassColor[0] = Color(16, 200, 16);
    grassColor[1] = Color(0, 154, 0);

    // Line generation
    float z = 0; // Line position
    if (random) { // Random generation
        addRandomLines(0, 0, z, 1000, objectIndexes);
    }
    else { // Predefined map
        addLinesFromFile(0, 0, z, path + "map.info");
    }

    if (lines.empty())
        fileError();
}

void Map::addNextMap(Map *map) {
    this->next = map;
    float yOffset = 0.0f;
    for (Line &l : lines)
        if (l.y > yOffset)
            yOffset = l.y;
    this->next->setOffset(yOffset);
}

void Map::setOffset(float yOffset) {
    for (Line &l : lines) {
        l.y += yOffset;
    }
}

void Map::updateView(pair<float, float> pos) {
    this->posX = pos.first;
    this->posY = pos.second;
}

void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW, float zOffset) {
    scale = camD / (1.0f + z + zOffset - camZ);
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
    object.spriteMinX = destX + (s.getGlobalBounds().width - s.getGlobalBounds().width * coeff[object.spriteNum]) / 2.0f;
    if (left && object.repetitive) {
        while (destX >= 0.0f) {
            destX -= s.getGlobalBounds().width;
            s.setPosition(destX, destY);
            w.draw(s);
        }

        object.spriteMinX = 0;
    }

    object.spriteMaxX = object.spriteMinX + s.getGlobalBounds().width * coeff[object.spriteNum];
    if (!left && object.repetitive) {
        while (destX <= w.getSize().x) {
            destX += s.getGlobalBounds().width;
            s.setPosition(destX, destY);
            w.draw(s);
        }

        if (coeff[object.spriteNum] != 0)
            object.spriteMaxX = w.getSize().x;
    }
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
    Line *l = getLine(int(posY)), *p;
    int N = lines.size();

    // Background
    drawQuad(c.w, grassColor[0], 0, 0, c.w.getSize().x, 0, c.w.getSize().y, c.w.getSize().x);
    Sprite sbg;
    sbg.setTexture(bg);
    sbg.setScale(Vector2f(2.0f * (float)c.w.getSize().x / bg.getSize().x, (float)c.w.getSize().y * BGS / bg.getSize().y));
    sbg.setPosition(0, 0);
    sbg.move(-sbg.getGlobalBounds().width / 3.0f - posX, 0);
    if (l->curve > 0.0f)
        sbg.move(XINC / 2.0f, 0);
    else if (l->curve < 0.0f)
        sbg.move(-XINC / 2.0f, 0);
    c.w.draw(sbg);

    int startPos = int(posY) % N;
    float camH = l->y + 1500.0f;

    float maxy = c.w.getSize().y;
    float x = 0, dx = 0;

    ///////draw road////////
    for (int n = startPos; n < startPos + c.renderLen; n++) {
        l = getLine(n);

        l->project(posX * ROADW - x, camH, float(startPos * SEGL), c.camD,
                  c.w.getSize().x, c.w.getSize().y, ROADW, n < N ? 0.0f : lines[lines.size() - 1].z);
        x += dx;
        dx += l->curve;

        l->clip = maxy;
        if (l->Y < maxy) {
            maxy = l->Y;

            Color grass, road;
            if (n < N || next == nullptr) {
                grass = grassColor[l->mainColor];
                road = roadColor[l->mainColor];
            }
            else {
                grass = next->grassColor[l->mainColor];
                road = next->roadColor[l->mainColor];
            }
            Color rumble = l->mainColor ? road : Color::White;
            Color dash = l->mainColor ? Color::White : road;

            p = getPreviousLine(n);

            // Draw grass
            drawQuad(c.w, grass, 0, int(p->Y), c.w.getSize().x, 0, int(l->Y), c.w.getSize().x);

            // Draw road
            const int x1 = int(p->X - p->W), y1 = int(p->Y), w1 = int(2.0f * p->W),
                      x2 = int(l->X - l->W), y2 = int(l->Y), w2 = int(2.0f * l->W),
                      rw1 = int(p->W * RUMBLECOEFF), rw2 = int(l->W * RUMBLECOEFF),
                      dw1 = rw1 / 2, dw2 = rw2 / 2;
            drawQuad(c.w, road, x1, y1, w1, x2, y2, w2);
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
        l = getLine(n);

        if (l->spriteLeft.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeff, l->spriteLeft, true);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeff, l->spriteLeft, true);
        }
        if (l->spriteRight.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeff, l->spriteRight, false);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeff, l->spriteRight, false);
        }
    }

}

bool Map::hasCrashed(const Config &c, float prevY, float currentY, float minX, float maxX) const {
    Line l;
    for (int n = int(posY); n < int(posY) + c.renderLen; n++) {
        l = getLine(n);

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
    return getLine(int(currentY)).curve;
}

Map::Elevation Map::getElevation(float currentY) const {
    const int n = int(currentY);
    const Line currentLine = getLine(n);
    const Line prevLine = getPreviousLine(n);
    if (n != 0 && abs(currentLine.y) > 1000 && currentLine.y > prevLine.y + 1.0f)
        return UP;
    else if (n != 0 && abs(currentLine.y) > 1000 && currentLine.y < prevLine.y - 1.0f)
        return DOWN;
    else
        return FLAT;
}

bool Map::isOver() const {
    return posY >= lines.size();
}
