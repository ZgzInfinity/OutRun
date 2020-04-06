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
#define SEGL 200 // Segment length
#define ROADW 2000 // Road Width

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
            Line line;
            line.z = (float) i * SEGL;

            // Curves in the first half of the map
            if (i > fh1 && i < fh2) line.curve = c1;

            // Curves in the second half of the map
            if (i > sh1 && i < sh2) line.curve = c2;

            // Random objects
            if (dist(generator) > 0.875f) {
                if (distBool(generator) == 0) { // Left
                    line.spriteLeft.spriteNum = distObj(generator);
                    line.spriteLeft.offset = dist(generator);
                }
                else { // Right
                    line.spriteRight.spriteNum  = distObj(generator);
                    line.spriteRight.offset = dist(generator);
                }
            }

            // Elevation
            if (i >= ele && i <= maxLines - 180) // The last line will have the same Y as the first
                maxEle = i + 180;
            if (i >= ele && i <= maxEle)
                line.y = float(sin((i - ele) * M_PI / 180.0) * 1500.0);

            // Colors
            line.road = (i / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);
            if (i == 0)
                line.road = Color::Red;
            line.grass = (i / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);

            lines.push_back(line);
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

                            Line line;

                            // Colors
                            line.road = roadRGB[(lineIndex / 3) % 2];
                            line.grass = grassRGB[(lineIndex / 3) % 2];

                            // Indexes and positions
                            line.curve = curveCoeff;

                            line.z = (float) lineIndex * SEGL;
                            lineIndex++;

                            line.y = sqrt(abs(elevationIndex * elevationCoeff));
                            if (elevationIndex < 0.0f)
                                line.y *= 1.0f;

                            if (elevationCoeff > 0.0f)
                                elevationIndex++;
                            else if (elevationCoeff < 0.0f)
                                elevationIndex--;

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

                            lines.push_back(line);
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
        float z = lines[N - 1].z;
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
    shape.setPoint(0, Vector2f(x1 - w1, y1));
    shape.setPoint(1, Vector2f(x2 - w2, y2));
    shape.setPoint(2, Vector2f(x2 + w2, y2));
    shape.setPoint(3, Vector2f(x1 + w1, y1));
    w.draw(shape);
}

void Map::draw(Config &c) {
    Sprite sbg;
    sbg.setTexture(bg);
    sbg.setScale(Vector2f((float)c.w.getSize().x / bg.getSize().x, (float)c.w.getSize().y * BGS / bg.getSize().y));
    sbg.setPosition(0, 0);
    c.w.draw(sbg);

    int N = lines.size();
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

            Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : l.road;

            Line p = lines[(n - 1) % N]; //previous line

            drawQuad(c.w, l.grass, 0, int(p.Y), c.w.getSize().x, 0, int(l.Y), c.w.getSize().x);
            drawQuad(c.w, rumble, int(p.X), int(p.Y), int(p.W * 1.2f), int(l.X), int(l.Y), int(l.W * 1.2f));
            drawQuad(c.w, l.road, int(p.X), int(p.Y), int(p.W), int(l.X), int(l.Y), int(l.W));
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
}

void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW) {
    scale = camD / (z - camZ);
    X = (1.0f + scale * (x - camX)) * width / 2.0f;
    Y = (1.0f - scale * (y - camY)) * height / 2.0f;
    W = scale * rW  * width / 2.0f;
}

void Map::Line::drawSprite(RenderWindow &w, const vector<Texture> &objs, const vector<float> &coeff, SpriteInfo &object,
        bool left) {
    Sprite s(objs[object.spriteNum]);
    const int width = s.getTextureRect().width;
    const int h = s.getTextureRect().height;

    float destX = X + scale * ((float) w.getSize().x / (float) ROADW) * w.getSize().x / 2.0f; // Right road side
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
    if (left)
        destX -= s.getGlobalBounds().width + object.offset * s.getGlobalBounds().width / 2.0f;
    else
        destX += s.getGlobalBounds().width + object.offset * s.getGlobalBounds().width / 2.0f;
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
