/******************************************************************************
 * @file    Map.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include <random>
#include <fstream>
#include "Map.hpp"

using namespace std;
using namespace sf;

#define BGS 0.525F // Background size
#define SEGL 100 // Segment length
#define ROADW 3000 // Road Width / 2
#define RUMBLECOEFF 0.0666f // Ruble size = Road size * Rumble coeff
#define SCREEN_SCALE 266.0f
#define FORK_COEFF 0.03f
#define FORK_RECTANGLES 100 // Multiple of RECTANGLE
#define END_RECTANGLES 60 // Multiple of RECTANGLE

Map::SpriteInfo::SpriteInfo() {
    spriteNum = -1;
    offset = spriteMinX = spriteMaxX = 0.0f;
    repetitive = false;
}

Map::Line::Line() {
    curve = x = y = z = 0;
    mainColor = false;
    offsetX = yOffsetX = 0.0f;
}

void Map::addLine(float x, float y, float &z, float prevY, float curve, bool mainColor,
        const Map::SpriteInfo &spriteLeft, const Map::SpriteInfo &spriteRight, float &bgX, float &offsetX,
        const float offsetInc) {
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
        if (offsetX > 0.0f) {
            if (offsetX >= xChange)
                lineAux.yOffsetX = sqrt(FORK_RADIUS * FORK_RADIUS - pow(offsetX - aOffsetX, 2.0f)) + bOffsetX;
            else
                lineAux.yOffsetX = -sqrt(FORK_RADIUS * FORK_RADIUS - offsetX * offsetX) + FORK_RADIUS;
            lineAux.offsetX = offsetX;
            if (offsetX + offsetInc <= 2.0f * xChange)
                offsetX += offsetInc;
        }
        lineAux.y += yInc;
        bgX += lineAux.curve;
        lineAux.bgX = bgX;
        lines.push_back(lineAux);
    }

    line.z = z;
    z += SEGL;
    if (offsetX > 0.0f) {
        if (offsetX >= xChange)
            line.yOffsetX = sqrt(FORK_RADIUS * FORK_RADIUS - pow(offsetX - aOffsetX, 2.0f)) + bOffsetX;
        else
            line.yOffsetX = -sqrt(FORK_RADIUS * FORK_RADIUS - offsetX * offsetX) + FORK_RADIUS;
        line.offsetX = offsetX;
        if (offsetX + offsetInc <= 2.0f * xChange)
            offsetX += offsetInc;
    }
    lineAux.y += yInc;
    line.y = lineAux.y;
    bgX += line.curve;
    line.bgX = bgX;
    lines.push_back(line);

    for (int i = 0; i < PRE_POS; i++) {
        lineAux.z = z;
        z += SEGL;
        if (offsetX > 0.0f) {
            if (offsetX >= xChange)
                lineAux.yOffsetX = sqrt(FORK_RADIUS * FORK_RADIUS - pow(offsetX - aOffsetX, 2.0f)) + bOffsetX;
            else
                lineAux.yOffsetX = -sqrt(FORK_RADIUS * FORK_RADIUS - offsetX * offsetX) + FORK_RADIUS;
            lineAux.offsetX = offsetX;
            if (offsetX + offsetInc <= 2.0f * xChange)
                offsetX += offsetInc;
        }
        lineAux.y += yInc;
        bgX += lineAux.curve;
        lineAux.bgX = bgX;
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
    // Road
    instructions.push_back({"107", "107", "107"});
    instructions.push_back({"105", "105", "105"});
    // Grass
    instructions.push_back({to_string(distRB(generator)), to_string(distG(generator)), to_string(distRB(generator))});
    instructions.push_back({to_string(distRB(generator)), to_string(distG(generator)), to_string(distRB(generator))});
    // Rumble
    instructions.push_back({"255", "255", "255"});
    // Dash
    instructions.push_back({"255", "255", "255"});

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
        bool road = false, grass = false, rumbleAndDash = false, comment = false;
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
                else if (!rumbleAndDash) {
                    if (buffer.size() == 6) {
                        instructions.push_back({buffer[0], buffer[1], buffer[2]});
                        instructions.push_back({buffer[3], buffer[4], buffer[5]});
                        buffer.clear();
                        rumbleAndDash = true;
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

void Map::addLines(float x, float y, float &z, float &bgX, const vector<vector<string>> &instructions) {
    float curveCoeff = 0.0f, elevation = 0.0f;
    int elevationIndex = 0, elevationLines = -1;
    bool mainColor = true;

    // Colors
    try {
        roadColor[0] = Color(stoi(instructions[0][0]), stoi(instructions[0][1]), stoi(instructions[0][2]));
        roadColor[1] = Color(stoi(instructions[1][0]), stoi(instructions[1][1]), stoi(instructions[1][2]));
        grassColor[0] = Color(stoi(instructions[2][0]), stoi(instructions[2][1]), stoi(instructions[2][2]));
        grassColor[1] = Color(stoi(instructions[3][0]), stoi(instructions[3][1]), stoi(instructions[3][2]));
        rumbleColor = Color(stoi(instructions[4][0]), stoi(instructions[4][1]), stoi(instructions[4][2]));
        dashColor = Color(stoi(instructions[5][0]), stoi(instructions[5][1]), stoi(instructions[5][2]));
    }
    catch (const exception &e) {
        fileError("Faltan colores al declarar el mapa.");
    }

    for (int i = 6; i < instructions.size(); i++) {
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

            float offset = 0.0f;
            addLine(x, yAux, z, lines.empty() ? y : lines[lines.size() - 1].y, curveCoeff, mainColor,
                    spriteLeft, spriteRight, bgX, offset);
            mainColor = !mainColor;
        }
    }
}

void Map::loadObjects(const string &path, const vector<string> &objectNames, vector<int> &objectIndexes) {
    int k = 0;
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
}

Map::Map(Config &c, const std::string &path, const std::string &bgName,
        const std::vector<std::string> &objectNames, bool random, const int time) : posX(0), posY(0), next(nullptr),
        nextRight(nullptr), initMap(false), goalMap(false), maxTime(time) {
    bg.loadFromFile(path + bgName);
    bg.setRepeated(true);

    // Load objects
    vector<int> objectIndexes;
    loadObjects(path, objectNames, objectIndexes);

    // Line generation
    float z = 0; // Line position
    float bgX = 0; // Background position
    if (random) { // Random generation
        addLines(0, 0, z, bgX, randomMap(1000, objectIndexes));
    }
    else { // Predefined map
        addLines(0, 0, z, bgX, readMapFile(path + "map.info"));
    }

    if (lines.empty())
        fileError();
}

Map::Map(const Map &map, int &flagger, int &semaphore) : bg(map.bg), posX(0), posY(0), next(nullptr),
                                                         nextRight(nullptr), initMap(true), goalMap(false), maxTime(0) {
    const int rectangles = 50; // Map size
    const string mapPath = "resources/mapCommon/"; // Folder with common objects
    const int nobjects = 38;

    // Colors
    roadColor[0] = map.roadColor[0];
    roadColor[1] = map.roadColor[1];
    grassColor[0] = map.grassColor[0];
    grassColor[1] = map.grassColor[1];
    rumbleColor = map.rumbleColor;
    dashColor = map.dashColor;

    // Load objects
    vector<string> objectNames;
    objectNames.reserve(nobjects);
    for (int no = 1; no <= nobjects; no++)
        objectNames.push_back(to_string(no) + ".png");
    vector<int> objectIndexes;
    loadObjects(mapPath, objectNames, objectIndexes);

    // Hardcoded sprites
    vector<SpriteInfo> leftSprites, rightSprites;
    leftSprites.reserve(rectangles);
    rightSprites.reserve(rectangles);
    for (int i = 0; i < rectangles; i++) {
        leftSprites.emplace_back();
        rightSprites.emplace_back();
    }
    // People
    leftSprites[3].spriteNum = 28; // 29 - 1
    leftSprites[3].offset = -1.75f;
    rightSprites[3].spriteNum = 29;
    rightSprites[3].offset = -1.75f;
    // Signals
    leftSprites[6].spriteNum = 22;
    leftSprites[6].offset = -1;
    rightSprites[6].spriteNum = 27;
    rightSprites[6].offset = -1;
    // Flagger
    leftSprites[5].spriteNum = 17;
    leftSprites[5].offset = -4;
    // Start
    rightSprites[5].spriteNum = 26;
    rightSprites[5].offset = -0.95f;
    // Trees
    leftSprites[4].spriteNum = 36;
    leftSprites[4].offset = -1.15f;
    rightSprites[4].spriteNum = 35;
    rightSprites[4].offset = -1.15f;
    // Fill
    for (int i = 7; i < rectangles; i++) {
        if (i % 4 == 3) {
            // Trees
            leftSprites[i].spriteNum = 36;
            leftSprites[i].offset = -0.15f;
            rightSprites[i].spriteNum = 35;
            rightSprites[i].offset = -0.15f;
        }
        else if (i % 2 == 0) {
            // People
            leftSprites[i].spriteNum = 31;
        }
    }

    // Line generation
    bool mainColor = true;
    vector<vector<string>> instructions;
    instructions.reserve(rectangles);
    float z = 0; // Line position
    float bgX = 0; // Background position
    for (int i = 0; i < rectangles; i++) {
        float offset = 0.0f;
        addLine(0, 0, z, 0, 0, mainColor, leftSprites[i], rightSprites[i], bgX, offset);
        mainColor = !mainColor;
    }

    flagger = RECTANGLE * 5 + PRE_POS;
    semaphore = RECTANGLE * 6 + PRE_POS;
}

Map::Map(int &flagger, int &goalEnd) : posX(0), posY(0), next(nullptr), nextRight(nullptr), initMap(false),
                                       goalMap(true), maxTime(0) {
    const int rectangles = 150; // Map size
    const string mapPath = "resources/mapCommon/"; // Folder with common objects
    const int nobjects = 38;

    // Load objects
    vector<string> objectNames;
    objectNames.reserve(nobjects);
    for (int no = 1; no <= nobjects; no++)
        objectNames.push_back(to_string(no) + ".png");
    vector<int> objectIndexes;
    loadObjects(mapPath, objectNames, objectIndexes);

    // Hardcoded sprites
    vector<SpriteInfo> leftSprites, rightSprites;
    leftSprites.reserve(rectangles);
    rightSprites.reserve(rectangles);
    for (int i = 0; i < rectangles; i++) {
        leftSprites.emplace_back();
        rightSprites.emplace_back();
    }
    // First goal
    rightSprites[1].spriteNum = 32;
    rightSprites[1].offset = -0.95f;
    // Trees
    leftSprites[0].spriteNum = 37;
    leftSprites[0].offset = -1.0f;
    rightSprites[0].spriteNum = 37;
    rightSprites[0].offset = -1.0f;
    // People
    leftSprites[53].spriteNum = 28; // 29 - 1
    leftSprites[53].offset = -1.75f;
    rightSprites[53].spriteNum = 29;
    rightSprites[53].offset = -1.75f;
    // Flagger
    leftSprites[50].spriteNum = 16;
    leftSprites[50].offset = -4;
    // Second goal
    rightSprites[50].spriteNum = 32;
    rightSprites[50].offset = -0.95f;
    // Trees
    leftSprites[49].spriteNum = 37;
    leftSprites[49].offset = -1.0f;
    rightSprites[49].spriteNum = 37;
    rightSprites[49].offset = -1.0f;

    // Line generation
    bool mainColor = true;
    vector<vector<string>> instructions;
    instructions.reserve(rectangles);
    float z = 0; // Line position
    float bgX = 0; // Background position
    for (int i = 0; i < rectangles; i++) {
        float offset = 0.0f;
        addLine(0, 0, z, 0, 0, mainColor, leftSprites[i], rightSprites[i], bgX, offset);
        mainColor = !mainColor;
    }

    flagger = RECTANGLE * 50 + PRE_POS;
    goalEnd = 45 * RECTANGLE;
}

void Map::setColors(const Map &map) {
    // Colors
    bg = map.bg;
    roadColor[0] = map.roadColor[0];
    roadColor[1] = map.roadColor[1];
    grassColor[0] = map.grassColor[0];
    grassColor[1] = map.grassColor[1];
    rumbleColor = map.rumbleColor;
    dashColor = map.dashColor;
}

void Map::incrementSpriteIndex(int line, bool right, int increment) {
    if (line < lines.size()) {
        SpriteInfo &sprite = right ? lines[line].spriteRight : lines[line].spriteLeft;
        if (sprite.spriteNum > -1)
            sprite.spriteNum += increment;
    }
}

void Map::addNextMap(Map *map) {
    this->next = map;
    float yOffset = 0.0f;
    if (!lines.empty())
        yOffset = lines[lines.size() - 1].y;
    this->next->setOffset(yOffset);
}

void Map::addFork(Map *left, Map *right) {
    if (this->nextRight == nullptr) {
        this->next = left;
        this->nextRight = right;
        float yOffset = 0.0f;
        if (!lines.empty())
            yOffset = lines[lines.size() - 1].y;
        this->next->setOffset(yOffset);
        if (this->next != this->nextRight)
            this->nextRight->setOffset(yOffset);

        // Add fork to current map
        if (!lines.empty()) {
            float x = lines[lines.size() - 1].x;
            float y = lines[lines.size() - 1].y;
            float z = lines[lines.size() - 1].z + SEGL;
            float bgX = lines[lines.size() - 1].bgX;
            bool mainColor = !lines[lines.size() - 1].mainColor;

            float xOffset = lines[lines.size() - 1].offsetX + FORK_COEFF;
            for (int i = 0; i < FORK_RECTANGLES + END_RECTANGLES; i++) { // 20 rectangles to fork
                addLine(x, y, z, y, 0.0f, mainColor, SpriteInfo(), SpriteInfo(), bgX, xOffset, FORK_COEFF);
                mainColor = !mainColor;
            }

            xOffset = 0.0f;
            vector<Line> auxLines;
            swap(left->lines, auxLines);
            left->lines.clear();
            left->lines.reserve(auxLines.size() + END_RECTANGLES * RECTANGLE);
            z = 0.0f;
            bgX = 0.0f;
            if (!lines[lines.size() - 1].mainColor)
                left->addLine(x, y, z, y, 0.0f, false, SpriteInfo(), SpriteInfo(), bgX, xOffset);
            mainColor = true;
            for (int i = 0; i < END_RECTANGLES; i++) {
                left->addLine(x, y, z, y, 0.0f, mainColor, SpriteInfo(), SpriteInfo(), bgX, xOffset);
                mainColor = !mainColor;
            }
            for (const Line &l : auxLines) {
                left->lines.push_back(l);
                left->lines.back().z += z;
                left->lines.back().bgX += bgX;
            }
            auxLines.clear();

            if (right != left) {
                swap(right->lines, auxLines);
                right->lines.clear();
                right->lines.reserve(auxLines.size() + END_RECTANGLES * RECTANGLE);
                z = 0.0f;
                if (!lines[lines.size() - 1].mainColor)
                    right->addLine(x, y, z, y, 0.0f, false, SpriteInfo(), SpriteInfo(), bgX, xOffset);
                mainColor = true;
                for (int i = 0; i < END_RECTANGLES; i++) {
                    right->addLine(x, y, z, y, 0.0f, mainColor, SpriteInfo(), SpriteInfo(), bgX, xOffset);
                    mainColor = !mainColor;
                }
                for (const Line &l : auxLines) {
                    right->lines.push_back(l);
                    right->lines.back().z += z;
                    right->lines.back().bgX += bgX;
                }
                auxLines.clear();
            }
        }
    }
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

float Map::getCamY() const {
    return this->posY;
}

void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW, float zOffset) {
    scale = camD / (1.0f + z + zOffset - camZ);
    X = (1.0f + scale * (x - camX)) * width / 2.0f;
    Y = (1.0f - scale * (y - camY)) * height / 2.0f;
    W = scale * rW  * width / 2.0f;
}

void Map::Line::drawSprite(RenderWindow &w, const vector<Texture> &objs, const vector<float> &hitCoeff,
        const vector<float> &scaleCoeff, SpriteInfo &object, bool left) const {
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

    if (l->offsetX >= xChange && nextRight != nullptr) {
        if (posX >= 0.0f)
            next = nextRight;
        nextRight = nullptr;
    }

    // Sort vehicles
    vector<Enemy*> sortedVehicles;
    sortedVehicles.reserve(vehicles.size());
    for (Enemy &v : vehicles)
        sortedVehicles.push_back(&v);
    sort(sortedVehicles.begin(), sortedVehicles.end(), ascendingSort); // vehicles are sorted in ascending order by posY

    // Discard out of range back vehicles
    while (!sortedVehicles.empty() && (int(sortedVehicles.back()->getPosY()) < int(posY) ||
            int(sortedVehicles.back()->getPosY()) > int(posY) + c.renderLen - 1))
        sortedVehicles.pop_back();

    // Background
    drawQuad(c.w, grassColor[0], 0, 0, c.w.getSize().x, 0, c.w.getSize().y, c.w.getSize().x);
    Sprite sbg;
    sbg.setTexture(bg);
    sbg.setScale(Vector2f(2.0f * (float)c.w.getSize().x / bg.getSize().x, (float)c.w.getSize().y * BGS / bg.getSize().y));
    sbg.setTextureRect(IntRect(0, 0, 5.0f * sbg.getGlobalBounds().width, bg.getSize().y));
    sbg.setPosition(0, 0);
    sbg.move(-4.0f * c.w.getSize().x - l->bgX - posX, 0);
    c.w.draw(sbg);

    // Initialize lines
    float camH = l->y + 1500.0f;
    float maxy = c.w.getSize().y;
    float x = 0, dx = 0;
    vector<int> visibleLines;

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

            Color grassRight, grass, roadRight, road, rumbleRight, rumble, dashRight, dash;
            if ((initMap && n < N) || (!initMap && n < N - (END_RECTANGLES + FORK_RECTANGLES) * RECTANGLE) || next == nullptr) {
                grassRight = grassColor[l->mainColor];
                roadRight = roadColor[l->mainColor];
                rumbleRight = l->mainColor ? roadRight : rumbleColor;
                dashRight = l->mainColor ? dashColor : roadRight;

                grass = grassRight;
                road = roadRight;
                rumble = rumbleRight;
                dash = dashRight;
            }
            else {
                grass = next->grassColor[l->mainColor];
                road = next->roadColor[l->mainColor];
                rumble = l->mainColor ? road : next->rumbleColor;
                dash = l->mainColor ? next->dashColor : road;
                if (nextRight != nullptr) {
                    grassRight = nextRight->grassColor[l->mainColor];
                    roadRight = nextRight->roadColor[l->mainColor];
                    rumbleRight = l->mainColor ? roadRight : nextRight->rumbleColor;
                    dashRight = l->mainColor ? nextRight->dashColor : roadRight;
                }
                else {
                    grassRight = grass;
                    roadRight = road;
                    rumbleRight = rumble;
                    dashRight = dash;
                }
            }

            float prevX = l->X, prevY = c.w.getSize().y, prevW = l->W;
            p = l;
            if (n > 0) {
                p = getPreviousLine(n);
                prevX = p->X;
                prevY = p->Y;
                prevW = p->W;
            }

            // Draw grass
            drawQuad(c.w, grassRight, 0, int(prevY), c.w.getSize().x, 0, int(l->Y), c.w.getSize().x);

            // Draw road
            int x1 = int(prevX - prevW - p->scale * (p->x - p->yOffsetX * ROADW) * c.w.getSize().x / 2.0f),
                    x2 = int(l->X - l->W - l->scale * (l->x - l->yOffsetX * ROADW) * c.w.getSize().x / 2.0f);
            const int y1 = int(prevY), w1 = int(2.0f * prevW), y2 = int(l->Y), w2 = int(2.0f * l->W),
                    rw1 = int(prevW * RUMBLECOEFF), rw2 = int(l->W * RUMBLECOEFF), dw1 = rw1 / 2, dw2 = rw2 / 2;
            drawQuad(c.w, roadRight, x1, y1, w1, x2, y2, w2);
            drawQuad(c.w, rumbleRight, x1, y1, rw1, x2, y2, rw2); // Left rumble
            drawQuad(c.w, rumbleRight, x1 + w1 - rw1, y1, rw1, x2 + w2 - rw2, y2, rw2); // Right rumble
            drawQuad(c.w, dashRight, x1 + rw1, y1, dw1, x2 + rw2, y2, dw2); // First left dash
            drawQuad(c.w, dashRight, x1 + w1 - rw1 - dw1, y1, dw1, x2 + w2 - rw2 - dw2, y2, dw2); // First right dash
            drawQuad(c.w, dashRight, x1 + int(float(w1) * 0.75f), y1, dw1, x2 + int(float(w2) * 0.75f), y2,
                     dw2); // Second right dash
            drawQuad(c.w, dashRight, x1 + int(float(w1) * 0.5f), y1, dw1, x2 + int(float(w2) * 0.5f), y2,
                     dw2); // Third right dash
            drawQuad(c.w, dashRight, x1 + int(float(w1) * 0.25f), y1, dw1, x2 + int(float(w2) * 0.25f), y2,
                     dw2); // Fourth right dash

            if (l->yOffsetX > 0.0f) {
                const int x1right = x1, x2right = x2;
                x1 = int(prevX - prevW + p->scale * (p->x - p->yOffsetX * ROADW) * c.w.getSize().x / 2.0f);
                x2 = int(l->X - l->W + l->scale * (l->x - l->yOffsetX * ROADW) * c.w.getSize().x / 2.0f);
                if ((x1 + w1) >= x1right) { // Intersection
                    grassRight = grassColor[l->mainColor];
                    roadRight = roadColor[l->mainColor];
                    rumbleRight = l->mainColor ? roadRight : rumbleColor;
                    dashRight = l->mainColor ? dashColor : roadRight;

                    grass = grassRight;
                    road = roadRight;
                    rumble = rumbleRight;
                    dash = dashRight;

                    // Draw grass
                    drawQuad(c.w, grassRight, 0, int(prevY), c.w.getSize().x, 0, int(l->Y), c.w.getSize().x);

                    // Left road
                    drawQuad(c.w, road, x1, y1, w1, x2, y2, w2);
                    drawQuad(c.w, rumble, x1, y1, rw1, x2, y2, rw2); // Left rumble

                    // Right road
                    drawQuad(c.w, roadRight, x1right, y1, w1, x2right, y2, w2);
                    drawQuad(c.w, rumbleRight, x1right + w1 - rw1, y1, rw1, x2right + w2 - rw2, y2, rw2); // Right rumble
                    drawQuad(c.w, dashRight, x1right + w1 - rw1 - dw1, y1, dw1, x2right + w2 - rw2 - dw2, y2, dw2); // First right dash
                    drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.75f), y1, dw1, x2right + int(float(w2) * 0.75f), y2,
                             dw2); // Second right dash
                    drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.5f), y1, dw1, x2right + int(float(w2) * 0.5f), y2,
                             dw2); // Third right dash
                    drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.25f), y1, dw1, x2right + int(float(w2) * 0.25f), y2,
                             dw2); // Fourth right dash
                }
                else {
                    // Draw grass
                    drawQuad(c.w, grass, 0, int(prevY), int(prevX), 0, int(l->Y), int(l->X));

                    drawQuad(c.w, road, x1, y1, w1, x2, y2, w2);
                    drawQuad(c.w, rumble, x1, y1, rw1, x2, y2, rw2); // Left rumble
                    drawQuad(c.w, rumble, x1 + w1 - rw1, y1, rw1, x2 + w2 - rw2, y2, rw2); // Right rumble
                    drawQuad(c.w, dash, x1 + w1 - rw1 - dw1, y1, dw1, x2 + w2 - rw2 - dw2, y2, dw2); // First right dash
                }

                drawQuad(c.w, dash, x1 + rw1, y1, dw1, x2 + rw2, y2, dw2); // First left dash
                drawQuad(c.w, dash, x1 + int(float(w1) * 0.75f), y1, dw1, x2 + int(float(w2) * 0.75f), y2,
                         dw2); // Second right dash
                drawQuad(c.w, dash, x1 + int(float(w1) * 0.5f), y1, dw1, x2 + int(float(w2) * 0.5f), y2,
                         dw2); // Third right dash
                drawQuad(c.w, dash, x1 + int(float(w1) * 0.25f), y1, dw1, x2 + int(float(w2) * 0.25f), y2,
                         dw2); // Fourth right dash
            }
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
        while (!sortedVehicles.empty() && int(sortedVehicles.back()->getPosY()) == n - startPos + int(posY)) {
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

bool Map::hasCrashed(const Config &c, float prevY, float currentY, float minX, float maxX, float &crashPos) const {
    Line l;
    for (int n = int(posY); n < int(posY) + c.renderLen; n++) {
        l = getLine(n);

        if (l.spriteLeft.spriteNum != -1 && l.spriteLeft.spriteMinX != l.spriteLeft.spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= l.spriteLeft.spriteMinX && minX <= l.spriteLeft.spriteMaxX) ||
                 (maxX >= l.spriteLeft.spriteMinX && maxX <= l.spriteLeft.spriteMaxX) ||
                 (l.spriteLeft.spriteMinX >= minX && l.spriteLeft.spriteMinX <= maxX) ||
                 (l.spriteLeft.spriteMaxX >= minX && l.spriteLeft.spriteMaxX <= maxX))) { // x matches
            crashPos = float(n);
            return true;
        }
        if (l.spriteRight.spriteNum != -1 && l.spriteRight.spriteMinX != l.spriteRight.spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= l.spriteRight.spriteMinX && minX <= l.spriteRight.spriteMaxX) ||
                 (maxX >= l.spriteRight.spriteMinX && maxX <= l.spriteRight.spriteMaxX) ||
                 (l.spriteRight.spriteMinX >= minX && l.spriteRight.spriteMinX <= maxX) ||
                 (l.spriteRight.spriteMaxX >= minX && l.spriteRight.spriteMaxX <= maxX))) { // x matches
            crashPos = float(n);
            return true;
        }
    }
    return false;
}

bool Map::hasGotOut(float currentX, float currentY) const {
    float offset;
    if (posX >= 0.0f)
        offset = -lines[int(currentY) % lines.size()].yOffsetX;
    else
        offset = lines[int(currentY) % lines.size()].yOffsetX;

    return abs(currentX + offset) > 1.0f;
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
    if (initMap || goalMap || (next != nullptr && next->goalMap))
        return posY >= lines.size();
    else
        return posY >= float(lines.size() - END_RECTANGLES * RECTANGLE);
}

float Map::getMaxY() const {
    if (initMap || goalMap || (next != nullptr && next->goalMap))
        return lines.size();
    else
        return float(lines.size() - END_RECTANGLES * RECTANGLE);
}

float Map::getOffsetX() const {
    if (posX >= 0.0f)
        return -lines[lines.size() - 1].yOffsetX;
    else
        return lines[lines.size() - 1].yOffsetX;
}

bool Map::inFork(const float currentY) const {
    if (initMap || goalMap || (next != nullptr && next->goalMap))
        return false;
    else
        return currentY >= float(lines.size() - (FORK_RECTANGLES + END_RECTANGLES) * RECTANGLE);
}

Map *Map::getNext() const {
    return next;
}

bool Map::isInitMap() const {
    return initMap;
}

bool Map::isGoalMap() const {
    return goalMap;
}

int Map::getTime() const {
    return maxTime;
}
