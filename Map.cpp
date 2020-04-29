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
#define ROADW 2000 // Road Width / 2
#define RUMBLECOEFF 0.1f // Ruble size = Road size * Rumble coeff
#define SCREEN_SCALE 266.0f

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
    float yInc = (y - prevY) / float(RECTANGLE); // RECTANGLE is total lines number will be added

    Line line, lineAux;
    line.x = x;
    line.y = prevY;
    line.mainColor = mainColor;
    line.curve = curve;

    lineAux = line; // without objects

    line.spriteLeft = spriteLeft;
    line.spriteRight = spriteRight;

    // For each normal line, extra lines without objects for better visualization
    for (int i = 0; i < PRE_POS; i++) {
        lineAux.z = z;
        z += SEGL;
        lineAux.y += yInc;
        lines.push_back(lineAux);
    }

    line.z = z;
    z += SEGL;
    lineAux.y += yInc;
    line.y = lineAux.y;
    lines.push_back(line);

    for (int i = 0; i < PRE_POS; i++) {
        lineAux.z = z;
        z += SEGL;
        lineAux.y += yInc;
        lines.push_back(lineAux);
    }
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

void fileError(const string &error="") {
    cerr << "Error: Formato de fichero incorrecto." << endl;
    if (!error.empty())
        cerr << "\t" + error << endl;
    exit(1);
}

vector<vector<string>> randomMap(const int numLines, const vector<int> &objectIndexes) {
    vector<vector<string>> instructions;

    // Random
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    uniform_int_distribution<int> distRB(0, 20), distG(0, 255);

    // Colors
    instructions.push_back({"107", "107", "107"});
    instructions.push_back({"105", "105", "105"});
    instructions.push_back({to_string(distRB(generator)), to_string(distG(generator)), to_string(distRB(generator))});
    instructions.push_back({to_string(distRB(generator)), to_string(distG(generator)), to_string(distRB(generator))});

    // Instructions
    float prob;
    int line = 0, untilLine = 0;
    while (line < numLines) {
        vector<string> inst;

        prob = dist(generator);
        if (line < untilLine || prob < 0.90f) {
            // Line
            inst.emplace_back("ROAD");

            if (!objectIndexes.empty() && dist(generator) > 0.75f) {
                // Left object
                if (dist(generator) > 0.75f) {
                    // Repetitive
                    inst.emplace_back("+");
                }

                uniform_int_distribution<int> distObj(0, objectIndexes.size() - 1);
                inst.push_back(to_string(objectIndexes[distObj(generator)]));
                if (dist(generator) > 0.75f) {
                    // Offset
                    inst.push_back(to_string(dist(generator)));
                }
            }

            inst.emplace_back("-");

            if (!objectIndexes.empty() && dist(generator) > 0.75f) {
                // Right object
                if (dist(generator) > 0.75f) {
                    // Repetitive
                    inst.emplace_back("+");
                }

                uniform_int_distribution<int> distObj(0, objectIndexes.size() - 1);
                inst.push_back(to_string(objectIndexes[distObj(generator)]));
                if (dist(generator) > 0.75f) {
                    // Offset
                    inst.push_back(to_string(dist(generator)));
                }
            }

            line++;
        }
        else if (prob < 0.92f) {
            // Curve
            inst.emplace_back("CURVE");
            inst.push_back(to_string(dist(generator) / 2.0f));
        }
        else if (prob < 0.94f) {
            // Curve
            inst.emplace_back("CURVE");
            inst.push_back(to_string(-dist(generator) / 2.0f));
        }
        else if (prob < 0.96f) {
            // Straight
            inst.emplace_back("STRAIGHT");
        }
        else if (prob < 0.98f) {
            // Climb
            inst.emplace_back("CLIMB");

            uniform_int_distribution<int> distLines(1 + line, numLines);
            untilLine = distLines(generator);

            inst.push_back(to_string(dist(generator) * float(untilLine - line) * 100.0f));
            inst.push_back(to_string(untilLine - line));
        }
        else {
            // Drop
            inst.emplace_back("DROP");

            uniform_int_distribution<int> distLines(1 + line, numLines);
            untilLine = distLines(generator);

            inst.push_back(to_string(dist(generator) * float(untilLine - line) * 100.0f));
            inst.push_back(to_string(untilLine - line));
        }

        instructions.push_back(inst);
    }

    return instructions;
}

vector<vector<string>> readMapFile(const std::string &file) {
    vector<vector<string>> instructions;

    ifstream fin(file);
    if (fin.is_open()) {
        bool road = false, grass = false, comment = false;
        int lines = 0, lastInclinationIndex = -1;

        vector<string> buffer;
        string s;
        while (!fin.eof()) {
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
                        instructions.push_back({buffer[0], buffer[1], buffer[2]});
                        instructions.push_back({buffer[3], buffer[4], buffer[5]});
                        buffer.clear();
                        road = true;
                    }
                }
                else if (!grass) {
                    if (buffer.size() == 6) {
                        instructions.push_back({buffer[0], buffer[1], buffer[2]});
                        instructions.push_back({buffer[3], buffer[4], buffer[5]});
                        buffer.clear();
                        grass = true;
                    }
                }
                else if (buffer.size() > 1 && (s == "ROAD" || s == "CURVE" || s == "STRAIGHT" || s == "CLIMB" ||
                                               s == "FLAT" || s == "DROP" || s == "RANDOM" || s  == "END")) {
                    if (buffer[0] == "CLIMB" || buffer[0] == "DROP" || buffer[0] == "FLAT") {
                        if (lastInclinationIndex > -1 && (instructions[lastInclinationIndex][0] == "CLIMB" ||
                                instructions[lastInclinationIndex][0] == "DROP")) {
                            instructions[lastInclinationIndex].push_back(to_string(lines));
                            lastInclinationIndex = -1;
                        }

                        if (buffer[0] == "CLIMB" || buffer[0] == "DROP") {
                            lines = 0;
                            lastInclinationIndex = instructions.size();

                            buffer.pop_back();
                            instructions.push_back(buffer);
                        }
                    }
                    else if (buffer[0] == "RANDOM") {
                        if (buffer.size() < 3)
                            fileError(buffer[0] + " necesita argumentos.");

                        vector<int> objectIndexes;
                        for (int i = 2; i < buffer.size() - 1; i++)
                            objectIndexes.push_back(stoi(buffer[i]));

                        const vector<vector<string>> randomInstructions = randomMap(stoi(buffer[1]), objectIndexes);
                        for (int i = 4; i < randomInstructions.size(); i++) {
                            instructions.push_back(randomInstructions[i]);
                        }
                    }
                    else {
                        if (s == "ROAD")
                            lines++;

                        buffer.pop_back();
                        instructions.push_back(buffer);
                    }

                    buffer.clear();
                    buffer.push_back(s);
                }
            }
        }
        fin.close();

        if (buffer[0] != "END")
            fileError("El fichero debe terminar en END.");
        if (instructions.size() < 4)
            fileError();
    }

    return instructions;
}

void Map::addLines(float x, float y, float &z, const vector<vector<string>> &instructions) {
    float curveCoeff = 0.0f, elevation = 0.0f;
    int elevationIndex = 0, elevationLines = -1;
    bool mainColor = true;

    // Colors
    roadColor[0] = Color(stoi(instructions[0][0]), stoi(instructions[0][1]), stoi(instructions[0][2]));
    roadColor[1] = Color(stoi(instructions[1][0]), stoi(instructions[1][1]), stoi(instructions[1][2]));
    grassColor[0] = Color(stoi(instructions[2][0]), stoi(instructions[2][1]), stoi(instructions[2][2]));
    grassColor[1] = Color(stoi(instructions[3][0]), stoi(instructions[3][1]), stoi(instructions[3][2]));

    for (int i = 4; i < instructions.size(); i++) {
        const vector<string> &inst = instructions[i];

        if (inst[0] == "CURVE") {
            if (inst.size() < 2)
                fileError(inst[0] + " necesita argumentos.");
            curveCoeff = stof(inst[1]);
        }
        else if (inst[0] == "STRAIGHT") {
            curveCoeff = 0.0f;
        }
        else if (inst[0] == "CLIMB") {
            if (inst.size() < 3)
                fileError(inst[0] + " necesita argumentos.");
            elevation = stof(inst[1]);
            elevationLines = stoi(inst[2]);
            elevationIndex = 0;
        }
        else if (inst[0] == "DROP") {
            if (inst.size() < 3)
                fileError(inst[0] + " necesita argumentos.");
            elevation = -stof(inst[1]);
            elevationLines = stoi(inst[2]);
            elevationIndex = 0;
        }
        else if (inst[0] == "ROAD") {
            SpriteInfo spriteLeft, spriteRight;

            // Elevation
            float yAux = y;
            if (elevationIndex < elevationLines) {
                yAux += float(elevation) / 2.0f +
                        (float(elevation) / 2.0f) * cosf(M_PI + (M_PI / float(elevationLines)) * float(elevationIndex));
                elevationIndex++;
            }
            if (!lines.empty() && elevationIndex == elevationLines) {
                y = lines[lines.size() - 1].y;
                yAux = y;
                elevationLines = -1;
            }

            // Objects
            int j = 1; // inst index
            if (inst[j] != "-") { // Left object
                if (inst[j] == "+") {
                    spriteLeft.repetitive = true;
                    j++;
                }
                spriteLeft.spriteNum = stoi(inst[j]) - 1;
                j++;
                if (inst[j] != "-") {
                    spriteLeft.offset = stof(inst[j]);
                    j++;
                }
            }
            if (j >= inst.size() || inst[j] != "-") { // Checkpoint
                fileError(inst[0] + " tiene argumentos incorrectos.");
            }
            j++;
            if (j < inst.size()) { // Right object
                if (inst[j] == "+") {
                    spriteRight.repetitive = true;
                    j++;
                }
                spriteRight.spriteNum = stoi(inst[j]) - 1;
                j++;
                if (j < inst.size()) {
                    spriteRight.offset = stof(inst[j]);
                    j++;
                }
            }
            if (j != inst.size()) { // Checkpoint
                fileError(inst[0] + " tiene argumentos incorrectos.");
            }

            addLine(x, yAux, z, lines.empty() ? y : lines[lines.size() - 1].y, curveCoeff, mainColor,
                    spriteLeft, spriteRight);
            mainColor = !mainColor;
        }
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
    hitCoeffs.reserve(objectNames.size());
    for (const string &objName : objectNames) {
        // Load indexes
        objectIndexes.push_back(k);
        k++;

        // Load textures
        Texture t;
        t.loadFromFile(path + objName);
        t.setSmooth(true);
        objects.push_back(t);

        // Load hit percentage from center and scale coeff
        ifstream fin(path + objName + ".info");
        float hitC = 1.0f, scaleC = 1.0f;
        if (fin.is_open()) {
            string s;
            while (!fin.eof()) {
                fin >> s;
                if (s == "HIT:" && !fin.eof()) {
                    fin >> hitC;
                }
                else if (s == "SCALE:" && !fin.eof()) {
                    fin >> scaleC;
                }
            }
            fin.close();
        }
        hitCoeffs.push_back(hitC);
        scaleCoeffs.push_back(scaleC);
    }

    // Colors
    roadColor[0] = Color(107, 107, 107);
    roadColor[1] = Color(105, 105, 105);
    grassColor[0] = Color(16, 200, 16);
    grassColor[1] = Color(0, 154, 0);

    // Line generation
    float z = 0; // Line position
    if (random) { // Random generation
        addLines(0, 0, z, randomMap(1000, objectIndexes));
    }
    else { // Predefined map
        addLines(0, 0, z, readMapFile(path + "map.info"));
    }

    if (lines.empty())
        fileError();
}

void Map::addNextMap(Map *map) {
    this->next = map;
    float yOffset = 0.0f;
    if (!lines.empty())
        yOffset = lines[lines.size() - 1].y;
    this->next->setOffset(yOffset);
}

void Map::setOffset(float yOffset) {
    for (Line &l : lines) {
        l.y += yOffset;
    }
}

void Map::updateView(float pX, float pY) {
    if (pY < 0.0f)
        pY = 0.0f;
    this->posX = pX;
    this->posY = pY;
}

float Map::getCamX() const {
    return this->posX;
}

void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW, float zOffset) {
    scale = camD / (1.0f + z + zOffset - camZ);
    X = (1.0f + scale * (x - camX)) * width / 2.0f;
    Y = (1.0f - scale * (y - camY)) * height / 2.0f;
    W = scale * rW  * width / 2.0f;
}

void Map::Line::drawSprite(RenderWindow &w, const vector<Texture> &objs, const vector<float> &hitCoeff,
        const vector<float> &scaleCoeff, SpriteInfo &object, bool left) {
    Sprite s(objs[object.spriteNum]);
    const float width = scaleCoeff[object.spriteNum] * s.getTextureRect().width;
    const float widthOri = s.getTextureRect().width;
    const float height = scaleCoeff[object.spriteNum] * s.getTextureRect().height;
    const float heightOri = s.getTextureRect().height;

    float destY = Y + 4.0f;
    float destW = width * W / SCREEN_SCALE;
    float destH = height * W / SCREEN_SCALE;

    destY += destH * (-1.0f); //offsetY

    float clipH = destY + destH - clip;
    if (clipH < 0)
        clipH = 0;

    if (clipH < destH) {
        s.setScale(destW / widthOri, destH / heightOri);

        float destX = X + W + object.offset * s.getGlobalBounds().width; // Right road side
        if (left)
            destX = X - W - s.getGlobalBounds().width - object.offset * s.getGlobalBounds().width; // Left road side
        s.setPosition(destX, destY);
        w.draw(s);

        object.spriteMinX =
                destX + (s.getGlobalBounds().width - s.getGlobalBounds().width * hitCoeff[object.spriteNum]) / 2.0f;
        object.spriteMaxX = object.spriteMinX + s.getGlobalBounds().width * hitCoeff[object.spriteNum];

        // Repetitive drawing
        if (left && object.repetitive) {
            while (destX >= 0.0f) {
                destX -= s.getGlobalBounds().width;
                s.setPosition(destX, destY);
                w.draw(s);
            }
            if (hitCoeff[object.spriteNum] > 0.0f)
                object.spriteMinX = 0.0f;
        }
        else if (!left && object.repetitive) {
            while (destX <= w.getSize().x) {
                destX += s.getGlobalBounds().width;
                s.setPosition(destX, destY);
                w.draw(s);
            }
            if (hitCoeff[object.spriteNum] > 0.0f)
                object.spriteMaxX = w.getSize().x;
        }
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

bool ascendingSort(const Enemy *v1, const Enemy *v2) {
    return v1->getPosY() < v2->getPosY();
}

void Map::draw(Config &c, vector<Enemy> &vehicles) {
    const int N = lines.size();
    const int startPos = int(posY) % N;
    const int lastPos = startPos + c.renderLen - 1;
    Line *l = getLine(startPos), *p;

    // Sort vehicles
    vector<Enemy*> sortedVehicles;
    sortedVehicles.reserve(vehicles.size());
    for (Enemy &v : vehicles)
        sortedVehicles.push_back(&v);
    sort(sortedVehicles.begin(), sortedVehicles.end(), ascendingSort); // vehicles are sorted in ascending order by posY

    // Discard out of range back vehicles
    while (!sortedVehicles.empty() && int(sortedVehicles.back()->getPosY()) % N < startPos &&
            int(sortedVehicles.back()->getPosY()) % N > lastPos % N)
        sortedVehicles.pop_back();

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

    // Initialize lines
    float camH = l->y + 1500.0f;
    float maxy = c.w.getSize().y;
    float x = 0, dx = 0;
    vector<int> visibleLines;

    l->spriteLeft.spriteMinX = 0;
    l->spriteLeft.spriteMaxX = 0;
    l->spriteRight.spriteMinX = 0;
    l->spriteRight.spriteMaxX = 0;
    for (int n = startPos; n <= lastPos; n++) {
        l = getLine(n);
        l->spriteLeft.spriteMinX = 0;
        l->spriteLeft.spriteMaxX = 0;
        l->spriteRight.spriteMinX = 0;
        l->spriteRight.spriteMaxX = 0;

        l->project(posX * ROADW - x, camH, float(startPos * SEGL), c.camD,
                   c.w.getSize().x, c.w.getSize().y, ROADW, n < N ? 0.0f : lines[lines.size() - 1].z);

        x += dx;
        dx += l->curve;

        l->clip = maxy;
        if (l->Y < maxy) { // This line is visible and will be drawn
            visibleLines.push_back(n);
            maxy = l->Y;
        }
    }

    // Draw road and objects
    for (int n = lastPos; n >= startPos; n--) {
        l = getLine(n);
        dx -= l->curve;
        x -= dx;

        // Prepare current line
        l->project(posX * ROADW - x, camH, float(startPos * SEGL), c.camD,
                   c.w.getSize().x, c.w.getSize().y, ROADW, n < N ? 0.0f : lines[lines.size() - 1].z);

        // Draw road
        if (visibleLines.back() == n) {
            visibleLines.pop_back();

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

            float prevX = l->X, prevY = c.w.getSize().y, prevW = l->W;
            if (n > 0) {
                p = getPreviousLine(n);
                prevX = p->X;
                prevY = p->Y;
                prevW = p->W;
            }

            // Draw grass
            drawQuad(c.w, grass, 0, int(prevY), c.w.getSize().x, 0, int(l->Y), c.w.getSize().x);

            // Draw road
            const int x1 = int(prevX - prevW), y1 = int(prevY), w1 = int(2.0f * prevW),
                    x2 = int(l->X - l->W), y2 = int(l->Y), w2 = int(2.0f * l->W),
                    rw1 = int(prevW * RUMBLECOEFF), rw2 = int(l->W * RUMBLECOEFF),
                    dw1 = rw1 / 2, dw2 = rw2 / 2;
            drawQuad(c.w, road, x1, y1, w1, x2, y2, w2);
            drawQuad(c.w, rumble, x1, y1, rw1, x2, y2, rw2); // Left rumble
            drawQuad(c.w, rumble, x1 + w1 - rw1, y1, rw1, x2 + w2 - rw2, y2, rw2); // Right rumble
            drawQuad(c.w, dash, x1 + rw1, y1, dw1, x2 + rw2, y2, dw2); // First left dash
            drawQuad(c.w, dash, x1 + w1 - rw1 - dw1, y1, dw1, x2 + w2 - rw2 - dw2, y2, dw2); // First right dash
            drawQuad(c.w, dash, x1 + int(float(w1) * 0.333f), y1, dw1, x2 + int(float(w2) * 0.333f), y2,
                     dw2); // Second left dash
            drawQuad(c.w, dash, x1 + int(float(w1) * 0.666f), y1, dw1, x2 + int(float(w2) * 0.666f), y2,
                     dw2); // Second right dash
        }

        // Draw object
        if (l->spriteLeft.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeffs, scaleCoeffs, l->spriteLeft, true);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeffs, next->scaleCoeffs, l->spriteLeft, true);
        }
        if (l->spriteRight.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeffs, scaleCoeffs, l->spriteRight, false);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeffs, next->scaleCoeffs, l->spriteRight, false);
        }

        // Draw vehicles
        while (!sortedVehicles.empty() && int(sortedVehicles.back()->getPosY()) % N == n % N) {
            Enemy *v = sortedVehicles.back();

            Sprite sv;
            sv.setTexture(*v->getCurrentTexture(), true);
            const float width = v->getScale() * sv.getTextureRect().width;
            const float widthOri = sv.getTextureRect().width;
            const float height = v->getScale() * sv.getTextureRect().height;
            const float heightOri = sv.getTextureRect().height;
            float destW = width * l->W / SCREEN_SCALE;
            float destH = height * l->W / SCREEN_SCALE;

            sv.setScale(destW / widthOri, destH / heightOri);
            v->setMinScreenX(l->X + l->W * v->getPosX() - sv.getGlobalBounds().width / 2.0f);
            v->setMaxScreenX(v->getMinScreenX() + sv.getGlobalBounds().width);
            sv.setPosition(v->getMinScreenX(), l->Y - destH);
            c.w.draw(sv);

            sortedVehicles.pop_back();
        }
    }
}

bool Map::hasCrashed(const Config &c, float prevY, float currentY, float minX, float maxX, int &crashPos) const {
    Line l;
    for (int n = int(posY); n < int(posY) + c.renderLen; n++) {
        l = getLine(n);

        if (l.spriteLeft.spriteNum != -1 && l.spriteLeft.spriteMinX != l.spriteLeft.spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= l.spriteLeft.spriteMinX && minX <= l.spriteLeft.spriteMaxX) ||
                 (maxX >= l.spriteLeft.spriteMinX && maxX <= l.spriteLeft.spriteMaxX) ||
                 (l.spriteLeft.spriteMinX >= minX && l.spriteLeft.spriteMinX <= maxX) ||
                 (l.spriteLeft.spriteMaxX >= minX && l.spriteLeft.spriteMaxX <= maxX))) { // x matches
            crashPos = n;
            return true;
        }
        if (l.spriteRight.spriteNum != -1 && l.spriteRight.spriteMinX != l.spriteRight.spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= l.spriteRight.spriteMinX && minX <= l.spriteRight.spriteMaxX) ||
                 (maxX >= l.spriteRight.spriteMinX && maxX <= l.spriteRight.spriteMaxX) ||
                 (l.spriteRight.spriteMinX >= minX && l.spriteRight.spriteMinX <= maxX) ||
                 (l.spriteRight.spriteMaxX >= minX && l.spriteRight.spriteMaxX <= maxX))) { // x matches
            crashPos = n;
            return true;
        }
    }
    return false;
}

bool Map::hasGotOut(float currentX) const {
    return abs(currentX) > 1.0f;
}

float Map::getCurveCoefficient(float currentY) const {
    return getLine(int(currentY)).curve;
}

Vehicle::Elevation Map::getElevation(float currentY) const {
    const int n = int(currentY);
    const Line currentLine = getLine(n);
    const Line prevLine = getPreviousLine(n);
    if (n != 0 && abs(currentLine.y) > 1000 && currentLine.y > prevLine.y + 1.0f)
        return Vehicle::Elevation::UP;
    else if (n != 0 && abs(currentLine.y) > 1000 && currentLine.y < prevLine.y - 1.0f)
        return Vehicle::Elevation::DOWN;
    else
        return Vehicle::Elevation::FLAT;
}

bool Map::isOver() const {
    return posY >= lines.size();
}

float Map::getMaxY() const {
    return lines.size();
}
