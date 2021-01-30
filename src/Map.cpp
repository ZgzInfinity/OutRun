/*
 * Copyright (c) 2020 Andres Gavin
 * Copyright (c) 2020 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */



/*
 * Module Map implementation file
 */

#include <random>
#include <fstream>
#include "../include/Map.hpp"

using namespace sf;

#define BGS 0.755F // Background size
#define SEGL 100 // Segment length
#define ROADW 3000 // Road Width / 2
#define RUMBLECOEFF 0.0666f // Ruble size = Road size * Rumble coeff
#define SCREEN_SCALE 280.0f
#define FORK_COEFF 0.032f
#define FORK_RECTANGLES 200 // Multiple of RECTANGLE
#define END_RECTANGLES 100 // Multiple of RECTANGLE



/**
 * Default constructor
 */
Map::SpriteInfo::SpriteInfo() {
    spriteNum = -1;
    offset = spriteMinX = spriteMaxX = spriteToSideX = 0.0f;
    repetitive = false;
    checkPoint = false;
}


/**
 * Default constructor
 */
Map::Line::Line() {
    curve = x = y = z = 0;
    mainColor = false;
    offsetX = yOffsetX = bgX = 0.0f;
}



/**
 * Add a new line of the landscape but not from the file
 * @param x is the actual coordinate 3d of the map in the axis x
 * @param y is the actual coordinate 3d of the map in the axis y
 * @param z is the actual coordinate 3d of the map in the axis z
 * @param prevY is the coordinate 3d of the map in the axis y of the last line added
 * @param curve represents the coefficient radius of the line to add to the map
 * @param mainColor shows in which color the line is going to be painted
 * @param spriteLeft is the map element located near and on the left of the road
 * @param spriteNearLeft is the map element located near and on the left of the road
 * @param spriteRight is the map element located near and on the right of the road
 * @param bgX is the background position of the map in axis x
 * @param offsetX is the controller offset to make the bifurcations
 * @param offsetInc is partial offset to increment the bifurcation direction
 */
void Map::addLine(float x, float y, float &z, float prevY, float curve, bool mainColor,
                  const Map::SpriteInfo &spriteFarLeft, const Map::SpriteInfo &spriteNearLeft,
                  const Map::SpriteInfo &spriteRight, float &bgX, float &offsetX,
                  const float offsetInc) {

    // RECTANGLE indicates the steps number will be added
    float yInc = (y - prevY) / float(RECTANGLE);

    // Assign the coordinates in axis x and y
    Line line, lineAux;
    line.x = x;
    line.y = prevY;

    // Assign the color
    line.mainColor = mainColor;

    // Assign the curve coefficient
    line.curve = curve;

    // Store the line
    lineAux = line;

    // Store the map elements
    line.spriteFarLeft = spriteFarLeft;
    line.spriteNearLeft = spriteNearLeft;
    line.spriteRight = spriteRight;

    // For each normal line, extra lines without objects for better visualization
    for (int i = 0; i < PRE_POS; i++) {

        // Increment the depth of the landscape in axis z
        lineAux.z = z;
        z += SEGL;

        // Check if a fork is being processed
        if (offsetX > 0.0f) {
            // Check if the possible fork is to the left or to the right
            if (offsetX >= xChange)
                // Possible fork to the left
                lineAux.yOffsetX = sqrt(FORK_RADIUS * FORK_RADIUS - pow(offsetX - aOffsetX, 2.0f)) + bOffsetX;
            else
                // Possible fork to the right
                lineAux.yOffsetX = -sqrt(FORK_RADIUS * FORK_RADIUS - offsetX * offsetX) + FORK_RADIUS;

            // Assign coordinate in axis x and increment it
            lineAux.offsetX = offsetX;
            if (offsetX + offsetInc <= 2.0f * xChange)
                offsetX += offsetInc;
        }

        // Increment the height in axis y
        lineAux.y += yInc;

        // Increment the direction curve
        bgX += lineAux.curve;
        lineAux.bgX = bgX;

        lines.push_back(lineAux);
        totalLines++;
    }

    // Increment the depth of the landscape in axis z
    line.z = z;
    z += SEGL;

    // Check if a fork is being processed
    if (offsetX > 0.0f) {
        // Check if the possible fork is to the left or to the right
        if (offsetX >= xChange)
            // Possible fork to the left
            line.yOffsetX = sqrt(FORK_RADIUS * FORK_RADIUS - pow(offsetX - aOffsetX, 2.0f)) + bOffsetX;
        else
            // Possible fork to the right
            line.yOffsetX = -sqrt(FORK_RADIUS * FORK_RADIUS - offsetX * offsetX) + FORK_RADIUS;

        // Assign coordinate in axis x and increment it
        line.offsetX = offsetX;
        if (offsetX + offsetInc <= 2.0f * xChange)
            offsetX += offsetInc;
    }

    // Increment the height in axis y
    lineAux.y += yInc;
    line.y = lineAux.y;

    // Increment the direction curve
    bgX += line.curve;
    line.bgX = bgX;
    lines.push_back(line);
    totalLines++;

    // Store the line of the checkPoint
    if (line.spriteFarLeft.checkPoint && line.spriteRight.checkPoint){
        checkPointLine = totalLines + END_RECTANGLES * RECTANGLE;
    }

    // For each normal line, extra lines without objects for better visualization
    for (int i = 0; i < PRE_POS; i++) {

        // Increment the depth of the landscape in axis z
        lineAux.z = z;
        z += SEGL;

        // Check if a fork is being processed
        if (offsetX > 0.0f) {
            // Check if the possible fork is to the left or to the right
            if (offsetX >= xChange)
                // Possible fork to the left
                lineAux.yOffsetX = sqrt(FORK_RADIUS * FORK_RADIUS - pow(offsetX - aOffsetX, 2.0f)) + bOffsetX;
            else
                // Possible fork to the right
                lineAux.yOffsetX = -sqrt(FORK_RADIUS * FORK_RADIUS - offsetX * offsetX) + FORK_RADIUS;

            // Assign coordinate in axis x and increment it
            lineAux.offsetX = offsetX;
            if (offsetX + offsetInc <= 2.0f * xChange)
                offsetX += offsetInc;
        }

        // Increment the height in axis y
        lineAux.y += yInc;

        // Increment the direction curve
        bgX += lineAux.curve;
        lineAux.bgX = bgX;
        lines.push_back(lineAux);
        totalLines++;
    }
}



/**
 * Return the line of the landscape with index n
 * @param n is the index of a landscape's line
 * @return
 */
Map::Line *Map::getLine(const int n) {
    if (n < (int)lines.size() || next == nullptr)
        return &lines[n % lines.size()];
    else
        return &next->lines[(n - lines.size()) % next->lines.size()];
}



/**
 * Return the line of the landscape with index n
 * @param n is the index of a landscape's line
 * @return
 */
Map::Line Map::getLine(const int n) const {
    if (n < (int)lines.size() || next == nullptr)
        return lines[n % lines.size()];
    else
        return next->lines[(n - lines.size()) % next->lines.size()];
}



/**
 * Returns the previous line of the landscape to the line with index n
 * @param is the index of a landscape's line
 * @return
 */
Map::Line *Map::getPreviousLine(const int n) {
    if ((n > 0 && n - 1 < (int)lines.size()) || next == nullptr)
        return &lines[(n - 1) % lines.size()];
    else
        return &next->lines[(n - 1 - lines.size()) % next->lines.size()];
}



/**
 * Returns the previous line of the landscape to the line with index n
 * @param is the index of a landscape's line
 * @return
 */
Map::Line Map::getPreviousLine(const int n) const {
    if ((n > 0 && n - 1 < (int)lines.size()) || next == nullptr)
        return lines[(n - 1) % lines.size()];
    else
        return next->lines[(n - 1 - lines.size()) % next->lines.size()];
}



/**
 * Throws an error in terminal when the map file format is not correct
 * @param error is the message to throw
 */
void fileError(const string &error = "") {
    cerr << "Error: Formato de fichero incorrecto." << endl;
    if (!error.empty())
        cerr << "\t" + error << endl;
    exit(1);
}



/**
 * Generates randomly a part of the map
 * @param numLines is the length of the map to be generated
 * @param objectIndexes is a vector with all the indexes of the map elements
 */
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

                uniform_int_distribution<int> distObj(0, static_cast<int>(objectIndexes.size() - 1));
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

                uniform_int_distribution<int> distObj(0, static_cast<int>(objectIndexes.size() - 1));
                inst.push_back(to_string(objectIndexes[distObj(generator)]));
                if (dist(generator) > 0.75f) {
                    // Offset
                    inst.push_back(to_string(dist(generator)));
                }
            }

            line++;
        } else if (prob < 0.92f) {
            // Curve
            inst.emplace_back("CURVE");
            inst.push_back(to_string(dist(generator) / 2.0f));
        } else if (prob < 0.94f) {
            // Curve
            inst.emplace_back("CURVE");
            inst.push_back(to_string(-dist(generator) / 2.0f));
        } else if (prob < 0.96f) {
            // Straight
            inst.emplace_back("STRAIGHT");
        } else if (prob < 0.98f) {
            // Climb
            inst.emplace_back("CLIMB");

            uniform_int_distribution<int> distLines(1 + line, numLines);
            untilLine = distLines(generator);

            inst.push_back(to_string(dist(generator) * float(untilLine - line) * 100.0f));
            inst.push_back(to_string(untilLine - line));
        } else {
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



/**
 * Reads a map from its configuration file and stores in memory
 * @param file is the path if the file
 */
vector<vector<string>> readMapFile(const std::string &file) {

    // Matrix with all the sentences an parameters
    vector<vector<string>> instructions;

    // Open the file of the map
    ifstream fin(file);
    if (fin.is_open()) {

        // Variables to controls the different parts read
        bool road = false, grass = false, rumbleAndDash = false, comment = false;
        bool terrain = false;
        int lines = 0, lastInclinationIndex = -1;

        // Buffer that stores temporarily what is been read
        vector<string> buffer;
        string s;

        // While still there is information to be read
        while (!fin.eof()) {
            fin >> s;

            // Control if the comments have been read
            if (s.size() >= 2 && s[0] == '/' && s[1] == '*')
                comment = true;
            if (comment) {
                // Ignore the comments
                if (s.size() >= 2 && s[s.size() - 1] == '/' && s[s.size() - 2] == '*')
                    comment = false;
            } else {
                buffer.push_back(s);

                // Read the terrain of the map
                if (!terrain){
                    if (buffer.size() == 1) {
                        instructions.push_back({buffer[0]});
                        buffer.clear();
                        // Terrain marked as read
                        terrain = true;
                    }
                }
                else if (!road) {
                    // Read the colors of the road
                    if (buffer.size() == 6) {
                        instructions.push_back({buffer[0], buffer[1], buffer[2]});
                        instructions.push_back({buffer[3], buffer[4], buffer[5]});
                        buffer.clear();
                        // Marked as read
                        road = true;
                    }
                } else if (!grass) {
                    // Read the colors of the grass
                    if (buffer.size() == 6) {
                        instructions.push_back({buffer[0], buffer[1], buffer[2]});
                        instructions.push_back({buffer[3], buffer[4], buffer[5]});
                        buffer.clear();
                        //Marked as read
                        grass = true;
                    }
                } else if (!rumbleAndDash) {
                    // Read the rumble and dash colors
                    if (buffer.size() == 6) {
                        instructions.push_back({buffer[0], buffer[1], buffer[2]});
                        instructions.push_back({buffer[3], buffer[4], buffer[5]});
                        buffer.clear();
                        // Marked as read
                        rumbleAndDash = true;
                    }
                }
                // Process the different parts of the route and store them
                else if (buffer.size() > 1 && (s == "ROAD" || s == "CURVE" || s == "STRAIGHT" || s == "CLIMB" ||
                                                 s == "FLAT" || s == "DROP" || s == "RANDOM" || s == "END")) {
                    if (buffer[0] == "CLIMB" || buffer[0] == "DROP" || buffer[0] == "FLAT") {
                        if (lastInclinationIndex > -1 && (instructions[lastInclinationIndex][0] == "CLIMB" ||
                                                          instructions[lastInclinationIndex][0] == "DROP")) {
                            instructions[lastInclinationIndex].push_back(to_string(lines));
                            lastInclinationIndex = -1;
                        }

                        if (buffer[0] == "CLIMB" || buffer[0] == "DROP") {
                            lines = 0;
                            lastInclinationIndex = static_cast<int>(instructions.size());

                            buffer.pop_back();
                            instructions.push_back(buffer);
                        }
                    } else if (buffer[0] == "RANDOM") {
                        if (buffer.size() < 3)
                            fileError(buffer[0] + " necessary argument.");

                        vector<int> objectIndexes;
                        for (int i = 2; i < (int)buffer.size() - 1; i++)
                            objectIndexes.push_back(stoi(buffer[i]));

                        const vector<vector<string>> randomInstructions = randomMap(stoi(buffer[1]), objectIndexes);
                        for (int i = 6; i < (int)randomInstructions.size(); i++) {
                            instructions.push_back(randomInstructions[i]);
                        }
                    } else {
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

        // Control if the file ends with the correct format or throws error
        if (buffer[0] != "END")
            fileError("El fichero debe terminar en END.");
        if (instructions.size() < 4)
            fileError();
    }
    return instructions;
}



/**
 * Add rectangles from the instructions to the map from (x, y, z). Update z for a new line.
 * @param x is the coordinate of the first rectangle in the axis x
 * @param y is the coordinate of the first rectangle in the axis y
 * @param z is the coordinate of the first rectangle in the axis z
 * @param bgX is the position in axis x of the background
 * @param instructions is a vector that contains all the information of the lines of the file that contains the map
 */
void Map::addLines(float x, float y, float &z, float &bgX, const vector<vector<string>> &instructions) {

    // Initialize the map
    float curveCoeff = 0.0f, elevation = 0.0f;
    int elevationIndex = 0, elevationLines = -1;
    bool mainColor = true;
    bool checkPointLeft = false, checkPointRight = false;

    // Colors
    try {

        // Stores the terrain of the map
        terrain = static_cast<int>(stoi(instructions[0][0]));

        // Store the colors of the road
        roadColor[0] = Color(static_cast<Uint8>(stoi(instructions[1][0])), static_cast<Uint8>(stoi(instructions[1][1])),
                             static_cast<Uint8>(stoi(instructions[1][2])));
        roadColor[1] = Color(static_cast<Uint8>(stoi(instructions[2][0])), static_cast<Uint8>(stoi(instructions[2][1])),
                             static_cast<Uint8>(stoi(instructions[2][2])));

        // Stores the colors of the grass
        grassColor[0] = Color(static_cast<Uint8>(stoi(instructions[3][0])),
                              static_cast<Uint8>(stoi(instructions[3][1])),
                              static_cast<Uint8>(stoi(instructions[3][2])));
        grassColor[1] = Color(static_cast<Uint8>(stoi(instructions[4][0])),
                              static_cast<Uint8>(stoi(instructions[4][1])),
                              static_cast<Uint8>(stoi(instructions[4][2])));

        // Store the color of the rumble of the road
        rumbleColor = Color(static_cast<Uint8>(stoi(instructions[5][0])), static_cast<Uint8>(stoi(instructions[5][1])),
                            static_cast<Uint8>(stoi(instructions[5][2])));

        // Store the color of the dash of the road
        dashColor = Color(static_cast<Uint8>(stoi(instructions[6][0])), static_cast<Uint8>(stoi(instructions[6][1])),
                          static_cast<Uint8>(stoi(instructions[6][2])));
    }
    catch (const exception &e) {
        // Error on the file format
        fileError("There are map colors not specified.");
    }

    // Iterate throughout the instructions matrix
    for (int i = 7; i < (int)instructions.size(); i++) {
        const vector<string> &inst = instructions[i];

        // Process the different parts of the road and their arguments
        if (inst[0] == "CURVE") {
            if (inst.size() < 2)
                fileError(inst[0] + " necessary arguments.");
            curveCoeff = stof(inst[1]);
        } else if (inst[0] == "STRAIGHT") {
            curveCoeff = 0.0f;
        } else if (inst[0] == "CLIMB") {
            if (inst.size() < 3)
                fileError(inst[0] + " necessary arguments.");
            elevation = stof(inst[1]);
            elevationLines = stoi(inst[2]);
            elevationIndex = 0;
        } else if (inst[0] == "DROP") {
            if (inst.size() < 3)
                fileError(inst[0] + " necessary arguments.");
            elevation = -stof(inst[1]);
            elevationLines = stoi(inst[2]);
            elevationIndex = 0;
        } else if (inst[0] == "ROAD") {
            SpriteInfo spriteLeft, spriteRight;

            // Compute the elevation of the terrain
            float yAux = y;
            if (elevationIndex < elevationLines) {
                yAux += elevation / 2.0f +
                        (elevation / 2.0f) * cosf(
                                static_cast<float>(M_PI + (M_PI / float(elevationLines)) * float(elevationIndex)));
                elevationIndex++;
            }
            // Stores the elevation of the line to draw the mountains
            if (!lines.empty() && elevationIndex == elevationLines) {
                y = lines[lines.size() - 1].y;
                yAux = y;
                elevationLines = -1;
            }

            // Objects
            int j = 1; // inst index
            if (inst[j] != "-") { // Left object
                if (inst[j] == "C"){
                    spriteLeft.checkPoint = true;
                    checkPointLeft = true;
                    j++;
                }
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
            if (j >= (int)inst.size() || inst[j] != "-") { // Checkpoint
                fileError(inst[0] + " has incorrect arguments.");
            }
            j++;
            if (j < (int)inst.size()) { // Right object
                if (inst[j] == "C"){
                    if (checkPointLeft){
                        spriteRight.checkPoint = true;
                        checkPointRight = true;
                        j++;
                    }
                    else {
                        fileError(" Checkpoint have to be left side too.");
                    }
                }
                if (inst[j] == "+") {
                    spriteRight.repetitive = true;
                    j++;
                }
                spriteRight.spriteNum = stoi(inst[j]) - 1;
                j++;
                if (j < (int)inst.size()) {
                    spriteRight.offset = stof(inst[j]);
                    j++;
                }
            }
            if (j != (int)inst.size()) { // Checkpoint
                fileError(inst[0] + " has incorrect arguments.");
            }

            // Add the line with all its attributes
            float offset = 0.0f;
            addLine(x, yAux, z, lines.empty() ? y : lines[lines.size() - 1].y, curveCoeff, mainColor,
                    spriteLeft, SpriteInfo(), spriteRight, bgX, offset);
            mainColor = !mainColor;
        }
    }

    if (checkPointLeft && !checkPointRight){
        fileError(" Checkpoint have to be right side too.");
    }
}



/**
 * Load all the textures of the map elements located in the landscape
 * @param path is the path of the file which contains the configuration of the landscape
 * @param objectNames is a vector which contains the identifiers of the map elements
 * @param objectIndexes is a vector which contains the code ids of the map elements
 */
void Map::loadObjects(const string &path, const vector<string> &objectNames, vector<int> &objectIndexes) {
    // Index of element to be processed
    int k = 0;
    objectIndexes.reserve(objectNames.size());
    objects.reserve(objectNames.size());
    hitCoeffs.reserve(objectNames.size());
    hitCoeffTypes.reserve(objectNames.size());
    for (const string &objName : objectNames) {
        // Load indexes
        objectIndexes.push_back(k);
        k++;

        // Load textures
        Texture t;
        t.loadFromFile(path + objName);
        objects.push_back(t);

        // Load hit percentage from center and scale coeff
        ifstream fin(path + objName + ".info");
        float hitC = 1.0f, scaleC = 1.0f;
        Line::HitCoeffType hitCoeffType = Line::HIT_CENTER;
        if (fin.is_open()) {
            while (!fin.eof()) {
                string s;
                fin >> s;
                if (s == "HIT:" && !fin.eof()) {
                    fin >> hitC;
                    hitCoeffType = Line::HIT_CENTER;
                } else if (s == "HIT_LEFT:" && !fin.eof()) {
                    fin >> hitC;
                    hitCoeffType = Line::HIT_LEFT;
                } else if (s == "HIT_RIGHT:" && !fin.eof()) {
                    fin >> hitC;
                    hitCoeffType = Line::HIT_RIGHT;
                } else if (s == "HIT_SIDES:" && !fin.eof()) {
                    fin >> hitC;
                    hitCoeffType = Line::HIT_SIDES;
                } else if (s == "SCALE:" && !fin.eof()) {
                    fin >> scaleC;
                } else if (!s.empty()) {
                    cerr << "WARNING: '" << s << "' at file " << path + objName + ".info" << endl;
                }
            }
            fin.close();
        }
        hitCoeffs.push_back(hitC);
        hitCoeffTypes.push_back(hitCoeffType);
        scaleCoeffs.push_back(scaleC);
    }
}



/**
 * Creates a landscape reading its configuration file
 * @param c is the configuration of the game
 * @param path is the path of the file that contains the configuration of the landscape
 * @param bgName is the path of the image that represents the background of the landscape
 * @param objectNames is a vector with the aliases of the different map elements
 * @param random controls if the map has to be generated from a file or randomly
 * @param time is the time available to complete the landscape
 */
Map::Map(Config &c, const std::string &path, const std::string &bgName,
         const std::vector<std::string> &objectNames, bool random, const int time) : posX(0), posY(0), next(nullptr),
                                                                                     nextRight(nullptr), initMap(false),
                                                                                     goalMap(false), maxTime(time) {
    // Load the background of the map
    bg.loadFromFile(path + bgName);

    Texture t;

    // Load all the backgrounds for the transition
    for (int i = 1; i <= 8; i++){
        t.loadFromFile(path + "bg/bg" + to_string(i) + ".png");
        backgroundMaps.push_back(t);
    }

    bg.setRepeated(true);

    // Load objects
    vector<int> objectIndexes;
    loadObjects(path, objectNames, objectIndexes);

    // Initial length of the map
    totalLines = 0, totalLinesWithoutFork = -1;

    // Default value of the checkpoint
    checkPointLine = 0;

    // Initialize colors
    this->grassColorPrev[0] = Color::Transparent;
    this->grassColorPrev[1] = Color::Transparent;
    this->roadColorPrev[0] = Color::Transparent;
    this->roadColorPrev[1] = Color::Transparent;
    this->rumbleColorPrev[0] = Color::Transparent;
    this->rumbleColorPrev[1] = Color::Transparent;
    this->dashColorPrev[0] = Color::Transparent;
    this->dashColorPrev[1] = Color::Transparent;

    // Line generation
    float z = 0; // Line position
    float bgX = 0; // Background position
    if (random) { // Random generation
        addLines(0, 0, z, bgX, randomMap(1000, objectIndexes));
    } else { // Predefined map
        addLines(0, 0, z, bgX, readMapFile(path + "map.info"));
    }

    // Throw possible error in file format
    if (lines.empty())
        fileError();
}



/**
 * Creates a straight flat map which represents the starting point of a map
 * @param map is the map to be displayed
 * @param flagger is the flagger position while is announcing the start
 * @param semaphore is the color of the semaphore in the starting
 */
Map::Map(const Map &map, int &flagger, int &semaphore) : bg(map.bg), posX(0), posY(0), next(nullptr),
                                                         nextRight(nullptr), initMap(true), goalMap(false), maxTime(0) {
    // Map size
    const int rectangles = 65;

    // Folder with common objects
    const string mapPath = "Resources/mapCommon/";
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
    leftSprites[3].offset = -1.87f;
    rightSprites[3].spriteNum = 29;
    rightSprites[3].offset = -1.80f;

    // Signals
    leftSprites[6].spriteNum = 22;
    leftSprites[6].offset = -1;
    rightSprites[6].spriteNum = 27;
    rightSprites[6].offset = -1;

    // Start
    rightSprites[5].spriteNum = 26;
    rightSprites[5].offset = -1.091f;

    // Trees
    leftSprites[4].spriteNum = 36;
    leftSprites[4].offset = -0.85f;
    rightSprites[4].spriteNum = 35;
    rightSprites[4].offset = -0.85f;

    // Fill
    for (int i = 7; i < rectangles; i++) {
        if (i % 4 == 3) {
            // Trees
            leftSprites[i].spriteNum = 36;
            leftSprites[i].offset = -0.15f;
            rightSprites[i].spriteNum = 35;
            rightSprites[i].offset = -0.15f;
        } else if (i % 2 == 0) {
            // People
            leftSprites[i].spriteNum = 31;
        }
    }

    // Left signal and flagger
    SpriteInfo spriteNearLeftFirst, spriteNearLeftSecond;

    // Flagger
    spriteNearLeftFirst.spriteNum = 17;
    spriteNearLeftFirst.offset = -4.2f;

     // Left signal
    spriteNearLeftSecond.spriteNum = 22;
    spriteNearLeftSecond.offset = -1;

    // Line generation
    bool mainColor = true;
    vector<vector<string>> instructions;
    instructions.reserve(rectangles);
    float z = 0; // Line position
    float bgX = 0; // Background position
    for (int i = 0; i < rectangles; i++) {
        float offset = 0.0f;
        if (i == 3){
            addLine(0, 0, z, 0, 0, mainColor, leftSprites[i], spriteNearLeftFirst, rightSprites[i], bgX, offset);
        }
        else if (i == 6){
            addLine(0, 0, z, 0, 0, mainColor, leftSprites[i], spriteNearLeftSecond, rightSprites[i], bgX, offset);
        }
        else {
            addLine(0, 0, z, 0, 0, mainColor, leftSprites[i], SpriteInfo(), rightSprites[i], bgX, offset);
        }
        mainColor = !mainColor;
    }

    flagger = RECTANGLE * 3 + PRE_POS;
    semaphore = RECTANGLE * 6 + PRE_POS;
}



/**
 * Creates a straight flat map which represents the goal point of a map
 * @param flagger is the flagger position while is announcing the goal
 * @param goalEnd is position of the goal in the final map
 */
Map::Map(int &flagger, int &goalEnd) : posX(0), posY(0), next(nullptr), nextRight(nullptr), initMap(false),
                                       goalMap(true), maxTime(0) {
    // Map size
    const int rectangles = 150;
    // Folder with common objects
    const string mapPath = "Resources/mapCommon/";
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
    rightSprites[1].offset = -0.98f;

    // Trees
    leftSprites[0].spriteNum = 37;
    leftSprites[0].offset = -0.9f;
    rightSprites[0].spriteNum = 37;
    rightSprites[0].offset = -0.9f;

    // People
    leftSprites[53].spriteNum = 28; // 29 - 1
    leftSprites[53].offset = -1.75f;
    rightSprites[53].spriteNum = 29;
    rightSprites[53].offset = -1.85f;

    // Flagger
    leftSprites[50].spriteNum = 16;
    leftSprites[50].offset = -4;

    // Second goal
    rightSprites[50].spriteNum = 32;
    rightSprites[50].offset = -0.98f;

    // Trees
    leftSprites[49].spriteNum = 37;
    leftSprites[49].offset = -0.9f;
    rightSprites[49].spriteNum = 37;
    rightSprites[49].offset = -0.9f;

    // Line generation
    bool mainColor = true;
    vector<vector<string>> instructions;
    instructions.reserve(rectangles);
    float z = 0; // Line position
    float bgX = 0; // Background position
    for (int i = 0; i < rectangles; i++) {
        float offset = 0.0f;
        addLine(0, 0, z, 0, 0, mainColor, leftSprites[i], SpriteInfo(), rightSprites[i], bgX, offset);
        mainColor = !mainColor;
    }

    flagger = RECTANGLE * 50 + PRE_POS;
    goalEnd = 45 * RECTANGLE;
}



/**
 * Initialize the colors of the map
 * @param map is the map whose colors are going to be initialized
 */
void Map::setColors(const Map &map) {

    // Colors
    bg = map.bg;
    roadColor[0] = map.roadColor[0];
    roadColor[1] = map.roadColor[1];
    grassColor[0] = map.grassColor[0];
    grassColor[1] = map.grassColor[1];
    rumbleColor = map.rumbleColor;
    dashColor = map.dashColor;
    terrain = map.terrain;
}



/**
 * Increase the code id of the map element of the step if it exits
 * @param step is the actual step of the map with the map elements
 * @param right controls if there are more map elements of the step
 * @param increment is how to code id has to be increased
 */
void Map::incrementSpriteIndex(int line, bool right, int increment) {
    if (line < (int)lines.size()) {
        SpriteInfo &sprite = right ? lines[line].spriteRight : lines[line].spriteNearLeft;
        if (sprite.spriteNum > -1)
            sprite.spriteNum += increment;
    }
}



/**
 * Add a map to follow the actual
 * @param map is the scenario to be added
 */
void Map::addNextMap(Map *map) {
    this->next = map;
    float yOffset = 0.0f;
    if (!lines.empty())
        yOffset = lines[lines.size() - 1].y;
    this->next->setOffset(yOffset);
}



/**
 * Create a fork between the actual map and its neighbors
 * @param left is the left map of the fork
 * @param right is the right map of the fork
 */
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

            // Store the length of the map without the fork extension
            totalLinesWithoutFork = totalLines;

            for (int i = 0; i < FORK_RECTANGLES + END_RECTANGLES; i++) { // 20 rectangles to fork
                addLine(x, y, z, y, 0.0f, mainColor, SpriteInfo(), SpriteInfo(), SpriteInfo(), bgX, xOffset, FORK_COEFF);
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
                left->addLine(x, y, z, y, 0.0f, false, SpriteInfo(), SpriteInfo(), SpriteInfo(), bgX, xOffset);
            mainColor = true;
            for (int i = 0; i < END_RECTANGLES; i++) {
                left->addLine(x, y, z, y, 0.0f, mainColor, SpriteInfo(), SpriteInfo(), SpriteInfo(), bgX, xOffset);
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
                    right->addLine(x, y, z, y, 0.0f, false, SpriteInfo(), SpriteInfo(), SpriteInfo(), bgX, xOffset);
                mainColor = true;
                for (int i = 0; i < END_RECTANGLES; i++) {
                    right->addLine(x, y, z, y, 0.0f, mainColor, SpriteInfo(), SpriteInfo(), SpriteInfo(), bgX, xOffset);
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



/**
 * Adds the offset to the map to be displayed correctly
 * @param yOffset is the offset to add
 */
void Map::setOffset(float yOffset) {
    for (Line &l : lines) {
        l.y += yOffset;
    }
}



/**
 * Updates the position of the camera in the map
 * @param pX is the new coordinate in the axis x of the camera
 * @param pY is the new coordinate in the axis y of the camera
 */
void Map::updateView(float pX, float pY) {
    if (pY < 0.0f)
        pY = 0.0f;
    this->posX = pX;
    this->posY = pY;
}



/**
 * Returns the coordinate x of the camera
 * @return
 */
float Map::getCamX() const {
    return this->posX;
}



/**
 * Returns the coordinate y of the camera
 * @return
 */
float Map::getCamY() const {
    return this->posY;
}



/**
 * Sets the coordinates on the screen that correspond to the rectangle and its scale. This function must be
 * call to update the rectangle if the position of the map has been changed and.
 * @param camX is the coordinate of the rectangle in axis X
 * @param camY is the coordinate of the rectangle in axis Y
 * @param camZ is the coordinate of the rectangle in axis Z
 * @param camD is the deep of the rectangle in the screen
 * @param width is the width dimension of the rectangle
 * @param height is the height dimension of the rectangle
 * @param rW is the with of the road
 * @param zOffset is the offset in the axis Z
 */
void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW, float zOffset) {
    scale = camD / (1.0f + z + zOffset - camZ);
    X = (1.0f + scale * (x - camX)) * width / 2.0f;
    Y = (1.0f - scale * (y - camY)) * height / 2.0f;
    W = scale * rW * width / 2.0f;
}




/**
 * Draw the map element in the screen of the game
 * @param w is the console window of the game
 * @param objs is a vector with all the map elements textures
 * @param hitCoeff is a vector with all the hits coefficients
 * @param hitCoeffType is the hit coefficient of the element to draw
 * @param scaleCoeff is the scaling factor of the element to draw
 * @param object is the element to draw in the screen
 * @param left control if the object has to be drawn on the left or on the right of the screen
 */
void Map::Line::drawSprite(RenderTexture &w, const vector<Texture> &objs, const vector<float> &hitCoeff,
                           const vector<HitCoeffType> &hitCoeffType, const vector<float> &scaleCoeff,
                           SpriteInfo &object, bool left) const {
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

        if (hitCoeffType[object.spriteNum] == HIT_CENTER) {
            object.spriteMinX =
                    destX + (s.getGlobalBounds().width - s.getGlobalBounds().width * hitCoeff[object.spriteNum]) / 2.0f;
        } else if (hitCoeffType[object.spriteNum] == HIT_RIGHT) {
            object.spriteMinX =
                    destX + s.getGlobalBounds().width - s.getGlobalBounds().width * hitCoeff[object.spriteNum];
        } else { // HIT_LEFT and HIT_SIDES
            object.spriteMinX = destX;
        }
        object.spriteMaxX = object.spriteMinX + s.getGlobalBounds().width * hitCoeff[object.spriteNum];

        if (hitCoeffType[object.spriteNum] != HIT_SIDES) {
            object.spriteToSideX = 0.0f;
        } else {
            object.spriteToSideX = s.getGlobalBounds().width - (object.spriteMaxX - object.spriteMinX);
        }

        // Repetitive drawing
        if (left && object.repetitive) {
            while (destX >= 0.0f) {
                destX -= s.getGlobalBounds().width;
                s.setPosition(destX, destY);
                w.draw(s);
            }
            if (hitCoeff[object.spriteNum] > 0.0f)
                object.spriteMinX = 0.0f;
        } else if (!left && object.repetitive) {
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



/**
 * Draw a rectangle with a width and height in the screen console
 * @param w is the console window of the game
 * @param c is the color with the rectangle is going to be painted
 * @param x1 is the lower bound vertex coordinate in axis x of the rectangle
 * @param y1 is the lower bound vertex coordinate in axis y of the rectangle
 * @param w1 is the width of the rectangle
 * @param x2 is the upper bound vertex coordinate in axis x of the rectangle
 * @param y2 is the upper bound vertex coordinate in axis y of the rectangle
 * @param w2 is the height of the rectangle
 */
void drawQuad(RenderTexture &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2) {
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1, y1));
    shape.setPoint(1, Vector2f(x2, y2));
    shape.setPoint(2, Vector2f(x2 + w2, y2));
    shape.setPoint(3, Vector2f(x1 + w1, y1));
    w.draw(shape);
}



/**
 * Returns true is the car v1 is lower than the car v2.
 * @param v1 is a car
 * @param v2 is another car
 * Otherwise returns false
 */
bool ascendingSort(const Enemy *v1, const Enemy *v2) {
    return v1->getPosY() < v2->getPosY();
}




void Map::draw(Config &c, vector<Enemy> &vehicles, float playerPosX, float playerPosY, bool inFork, Map* left, Map* right) {
    const int N = static_cast<const int>(lines.size());
    const int startPos = int(posY) % N;
    const int lastPos = startPos + c.renderLen - 1;
    Line *l = getLine(startPos), *p;

    if (l->offsetX >= xChange && nextRight != nullptr) {
        if (posX >= 0.0f)
            next = nextRight;
        nextRight = nullptr;
    }

    // Sort vehicles
    vector<Enemy *> sortedVehicles;
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
    sbg.setScale(
            Vector2f(2.0f * (float) c.w.getSize().x / bg.getSize().x, (float) c.w.getSize().y * BGS / bg.getSize().y));
    sbg.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg.getGlobalBounds().width), bg.getSize().y));
    sbg.setPosition(0, 0);
    sbg.move(-8.0f * c.w.getSize().x - l->bgX - posX, 0);
    c.w.draw(sbg);

    Sprite sbg2;

    if (inFork){
        if (playerPosY >= totalLines - 570){
             if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[0]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[0].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[0].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[0].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[0]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[0].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[0].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[0].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
        else if (playerPosY >= totalLines - 575){
             if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[1]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[1].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[1].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[1].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[1]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[1].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[1].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[1].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
        else if (playerPosY >= totalLines - 580){
             if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[2]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[2].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[2].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[2].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[2]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[2].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[2].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[2].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
        else if (playerPosY >= totalLines - 585){
             if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[3]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[3].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[3].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[1].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[3]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[3].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[3].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[3].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
        else if (playerPosY >= totalLines - 590){
            if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[4]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[4].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[4].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[4].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[4]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[4].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[4].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[4].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
        else if (playerPosY >= totalLines - 595){
             if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[5]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[5].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[5].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[5].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[5]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[5].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[5].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[5].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
        else if (playerPosY >= totalLines - 600){
            if (playerPosX < 0.f){
                sbg2.setTexture(left->backgroundMaps[6]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / left->backgroundMaps[6].getSize().x,
                                        (float) c.w.getSize().y * BGS / left->backgroundMaps[6].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), left->backgroundMaps[6].getSize().y));
             }
             else {
                sbg2.setTexture(right->backgroundMaps[6]);
                sbg2.setScale(
                        Vector2f(2.0f * (float) c.w.getSize().x / right->backgroundMaps[6].getSize().x,
                                        (float) c.w.getSize().y * BGS / right->backgroundMaps[6].getSize().y));
                sbg2.setTextureRect(IntRect(0, 0, static_cast<int>(80.0f * sbg2.getGlobalBounds().width), right->backgroundMaps[6].getSize().y));
             }
             sbg2.setPosition(0, 0);
             c.w.draw(sbg2);
        }
    }

    // Initialize lines
    float camH = l->y + 1500.0f;
    float maxy = c.w.getSize().y;
    float x = 0, dx = 0;
    vector<int> visibleLines;

    for (int n = startPos; n <= lastPos; n++) {
        l = getLine(n);
        l->spriteFarLeft.spriteMinX = 0;
        l->spriteFarLeft.spriteMaxX = 0;
        l->spriteFarLeft.spriteToSideX = 0;
        l->spriteNearLeft.spriteMinX = 0;
        l->spriteNearLeft.spriteMaxX = 0;
        l->spriteNearLeft.spriteToSideX = 0;
        l->spriteRight.spriteMinX = 0;
        l->spriteRight.spriteMaxX = 0;
        l->spriteRight.spriteToSideX = 0;

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

    // Update color in transition map
    int times = 0;
    int numLines = 5;

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
                grass = this->grassColor[l->mainColor];
                road = this->roadColor[l->mainColor];
                rumble = l->mainColor ? road : this->rumbleColor;
                dash = l->mainColor ? this->dashColor : road;
                if (nextRight != nullptr) {
                    grassRight = this->grassColor[l->mainColor];
                    roadRight = this->roadColor[l->mainColor];
                    rumbleRight = l->mainColor ? roadRight : this->rumbleColor;
                    dashRight = l->mainColor ? this->dashColor : roadRight;
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
                    drawQuad(c.w, rumbleRight, x1right + w1 - rw1, y1, rw1, x2right + w2 - rw2, y2,
                             rw2); // Right rumble
                    drawQuad(c.w, dashRight, x1right + w1 - rw1 - dw1, y1, dw1, x2right + w2 - rw2 - dw2, y2,
                             dw2); // First right dash
                    drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.75f), y1, dw1,
                             x2right + int(float(w2) * 0.75f), y2,
                             dw2); // Second right dash
                    drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.5f), y1, dw1, x2right + int(float(w2) * 0.5f),
                             y2,
                             dw2); // Third right dash
                    drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.25f), y1, dw1,
                             x2right + int(float(w2) * 0.25f), y2,
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

            // Opacity of the color
            Uint8 opacity;
            bool draw = false;

            if (inFork){
                if (playerPosY >= totalLines - 585){
                    opacity = 255;
                    draw = true;
                }
                else if (playerPosY >= totalLines - 590){
                    opacity = 205;
                    draw = true;
                }
                else if (playerPosY >= totalLines - 595){
                    opacity = 185;
                    draw = true;
                }
                else if (playerPosY >= totalLines - 600){
                    opacity = 135;
                    draw = true;
                }
                else if (playerPosY >= totalLines - 610){
                    opacity = 90;
                    draw = true;
                }
                else if (playerPosY >= totalLines - 615){
                    opacity = 45;
                    draw = true;
                }
                else if (playerPosY >= totalLines - 620){
                    opacity = 0;
                    draw = true;
                }

                if (draw){
                    Color grassRight, grass, roadRight, road, rumbleRight, rumble, dashRight, dash;
                    if ((initMap && n < N) || (!initMap && n < N - (END_RECTANGLES + FORK_RECTANGLES) * RECTANGLE) ||
                        next == nullptr) {
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
                        grass = Color(next->grassColor[l->mainColor].r, next->grassColor[l->mainColor].g, next->grassColor[l->mainColor].b, opacity);
                        road = Color(next->roadColor[l->mainColor].r, next->roadColor[l->mainColor].g, next->roadColor[l->mainColor].b, opacity);
                        rumble = l->mainColor ? road : this->rumbleColor;
                        dash = l->mainColor ? this->dashColor : road;
                        if (nextRight != nullptr) {
                            grassRight = Color(nextRight->grassColor[l->mainColor].r, nextRight->grassColor[l->mainColor].g, nextRight->grassColor[l->mainColor].b, opacity);
                            roadRight = Color(nextRight->roadColor[l->mainColor].r, nextRight->roadColor[l->mainColor].g, nextRight->roadColor[l->mainColor].b, opacity);
                            rumbleRight = l->mainColor ? roadRight : this->rumbleColor;
                            dashRight = l->mainColor ? this->dashColor : roadRight;
                        }
                        else {
                            grassRight = grass;
                            roadRight = road;
                            rumbleRight = rumble;
                            dashRight = dash;
                        }
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
                            drawQuad(c.w, rumbleRight, x1right + w1 - rw1, y1, rw1, x2right + w2 - rw2, y2,
                                     rw2); // Right rumble
                            drawQuad(c.w, dashRight, x1right + w1 - rw1 - dw1, y1, dw1, x2right + w2 - rw2 - dw2, y2,
                                     dw2); // First right dash
                            drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.75f), y1, dw1,
                                     x2right + int(float(w2) * 0.75f), y2,
                                     dw2); // Second right dash
                            drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.5f), y1, dw1, x2right + int(float(w2) * 0.5f),
                                     y2,
                                     dw2); // Third right dash
                            drawQuad(c.w, dashRight, x1right + int(float(w1) * 0.25f), y1, dw1,
                                     x2right + int(float(w2) * 0.25f), y2,
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
            }
        }

        // Draw object
        if (l->spriteFarLeft.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeffs, hitCoeffTypes, scaleCoeffs, l->spriteFarLeft, true);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeffs, next->hitCoeffTypes, next->scaleCoeffs,
                              l->spriteFarLeft, true);
        }
        // Draw object
        if (l->spriteNearLeft.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeffs, hitCoeffTypes, scaleCoeffs, l->spriteNearLeft, true);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeffs, next->hitCoeffTypes, next->scaleCoeffs,
                              l->spriteNearLeft, true);
        }
        if (l->spriteRight.spriteNum > -1) {
            if (n < N || next == nullptr)
                l->drawSprite(c.w, objects, hitCoeffs, hitCoeffTypes, scaleCoeffs, l->spriteRight, false);
            else
                l->drawSprite(c.w, next->objects, next->hitCoeffs, next->hitCoeffTypes, next->scaleCoeffs,
                              l->spriteRight, false);
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
        times++;
        numLines++;
    }
}



/**
 * Returns true if the vehicle has crashed with any element of the landscape or with its borders
 * @param c is the configuration of the player
 * @param prevY is the previous position of the player in the axis y of the landscape
 * @param currentY is the actual position of the player in the axis y of the landscape
 * @param currentX is the actual position of the player in the axis x of the landscape
 * @param minX is the minimum threshold to detect a collision
 * @param maxX is the maximum threshold to detect a collision
 * @param crashPos is the possible position of crash detected
 * @return
 */
bool Map::hasCrashed(const Config &c, float prevY, float currentY, float currentX, float minX, float maxX,
                     float &crashPos) const {
    if (!inFork(currentY) && abs(currentX) > 3.0f) { // has left the map
        crashPos = posY;
        return true;
    }

    Line l;
    for (int n = int(posY); n < int(posY) + c.renderLen; n++) {
        l = getLine(n);

        float spriteMinX, spriteMaxX;
        int spriteNum;
        for (int i = 0; i < 4; i++) {
            if (i == 0) {
                spriteNum = l.spriteFarLeft.spriteNum;
                spriteMinX = l.spriteFarLeft.spriteMinX;
                spriteMaxX = l.spriteFarLeft.spriteMaxX;
            } else if (i == 1) {
                spriteNum = l.spriteFarLeft.spriteNum;
                spriteMinX = l.spriteFarLeft.spriteMinX + l.spriteFarLeft.spriteToSideX;
                spriteMaxX = l.spriteFarLeft.spriteMaxX + l.spriteFarLeft.spriteToSideX;
            } else if (i == 2) {
                spriteNum = l.spriteRight.spriteNum;
                spriteMinX = l.spriteRight.spriteMinX;
                spriteMaxX = l.spriteRight.spriteMaxX;
            } else {
                spriteNum = l.spriteRight.spriteNum;
                spriteMinX = l.spriteRight.spriteMinX + l.spriteRight.spriteToSideX;
                spriteMaxX = l.spriteRight.spriteMaxX + l.spriteRight.spriteToSideX;
            }

            if (spriteNum != -1 && spriteMinX != spriteMaxX && // l has an object that can crash
                prevY <= float(n) && currentY >= float(n) && // y matches
                ((minX >= spriteMinX && minX <= spriteMaxX) || (maxX >= spriteMinX && maxX <= spriteMaxX) ||
                 (spriteMinX >= minX && spriteMinX <= maxX) ||
                 (spriteMaxX >= minX && spriteMaxX <= maxX))) { // x matches
                crashPos = float(n);
                return true;
            }
        }
    }
    return false;
}


/**
 * Returns true if the actual position in axis x is outside the road.
 * Otherwise returns false
 * @param currentX is the position on axis x
 * @param currentY is the position on axis y
 * @return
 */
bool Map::hasGotOut(float currentX, float currentY) const {
    float offset;
    if (posX >= 0.0f)
        offset = -lines[int(currentY) % lines.size()].yOffsetX;
    else
        offset = lines[int(currentY) % lines.size()].yOffsetX;

    return abs(currentX + offset) > 1.0f;
}



/**
 * Returns the curvature coefficient corresponding to the currentY rectangle. The coefficient is positive if the
 * curve is to the right, negative if it's to the left and 0 if it's a straight line
 * @param currentY currentY is the current of the landscape where is the player
 * @return
 */
float Map::getCurveCoefficient(float currentY) const {
    return getLine(int(currentY)).curve;
}



/**
 * Returns the elevation corresponding to the currentY rectangle based on the previous rectangle.
 * @param currentY is the current of the map where is the player
 * @return
 */
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



/**
 * Returns true if the map has been finished.
 * Otherwise returns false
 * @return
 */
bool Map::isOver() const {
    if (initMap || goalMap || (next != nullptr && next->goalMap))
        return posY >= lines.size();
    else
        return posY >= float(lines.size() - END_RECTANGLES * RECTANGLE);
}



/**
 * Returns the height of the map
 * @return
 */
float Map::getMaxY() const {
    if (initMap || goalMap || (next != nullptr && next->goalMap))
        return lines.size();
    else
        return float(lines.size() - END_RECTANGLES * RECTANGLE);
}


/**
 * Returns the offset of the road in the axis x.
 * @return
 */
float Map::getOffsetX() const {
    if (posX >= 0.0f)
        return -lines[lines.size() - 1].yOffsetX;
    else
        return lines[lines.size() - 1].yOffsetX;
}



/**
 * Returns true if the player is on a fork. Otherwise returns false
 * @param currentY is the coordinate in axis y which to be tested to know
 *        if the player is on a fork or not
 * @return
 */
bool Map::inFork(const float currentY) const {
    if (initMap || goalMap || (next != nullptr && next->goalMap))
        return false;
    else
        return currentY >= float(lines.size() - (FORK_RECTANGLES + END_RECTANGLES) * RECTANGLE);
}



/**
 * Returns the next map or null if it is the last one
 * @return
 */
Map *Map::getNext() const {
    return next;
}



/**
 * Returns true if is the init map. Otherwise returns false
 * @return
 */
bool Map::isInitMap() const {
    return initMap;
}



/**
 * Returns true if is the goal landscape. Otherwise returns false
 * @return
 */
bool Map::isGoalMap() const {
    return goalMap;
}



/**
 * Returns the time to complete the map
 * @return
 */
int Map::getTime() const {
    return maxTime;
}



/**
 * Returns the kind of terrain of the landscape
 * @return
 */
 int Map::getTerrain() const {
    return terrain;
 }



/**
 * Return the line of the possible checkpoint in the map
 * @return
 */
int Map::getCheckPointLine() const {
    return checkPointLine;
}



/**
 * Return number of lines of the map without fork
 * @return
 */
int Map::getLinesWithoutFork()const {
    return totalLinesWithoutFork;
}
