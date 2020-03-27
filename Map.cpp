/******************************************************************************
 * @file    Map.hpp
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
#include "Map.hpp"

using namespace std;
using namespace sf;

#define BGS 0.525F // Background size
#define SEGL 200 // Segment length
#define ROADW 2000 // Road Width
#define MAXLINES 4000

Map::Map(sf::RenderWindow &w, const std::string &bgName, const std::vector<std::string> &objectNames) : posY(0) {
    bg.loadFromFile(bgName);
    bg.setRepeated(true);

    objects.reserve(objectNames.size());
    for (const string &s : objectNames) {
        Texture t;
        t.loadFromFile(s);
        t.setSmooth(true);
        objects.push_back(t);
    }

    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> dist(0.1f, 0.9f);
    uniform_int_distribution<unsigned int> distObj(0, objects.size() - 1);
    uniform_int_distribution<int> distBool(0, 1);

    // Random elements in the map
    float c1 = dist(generator), c2 = -dist(generator);
    unsigned int fh1, fh2, sh1, sh2, ele, maxEle = 0;
    fh1 = uniform_int_distribution<unsigned int>(0, MAXLINES / 2 - 1)(generator);
    fh2 = uniform_int_distribution<unsigned int>(fh1, MAXLINES / 2)(generator);
    sh1 = uniform_int_distribution<unsigned int>(MAXLINES / 2, MAXLINES - 1)(generator);
    sh2 = uniform_int_distribution<unsigned int>(sh1, MAXLINES)(generator);
    ele = uniform_int_distribution<unsigned int>(0, MAXLINES - 180)(generator);

    lines.reserve(MAXLINES);
    for (unsigned int i = 0; i < MAXLINES; i++) {
        Line line;
        line.z = (float) i * SEGL;

        // Curves in the first half of the map
        if (i > fh1 && i < fh2) line.curve = c1;

        // Curves in the second half of the map
        if (i > sh1 && i < sh2) line.curve = c2;

        // Random objects
        if (dist(generator) > 0.875f) {
            line.spriteNum = distObj(generator);
            line.offset = dist(generator);
            line.left = distBool(generator) == 0;
        }
        else {
            line.spriteNum = -1;
        }

        // Elevation
        if (i >= ele && i <= MAXLINES - 180) // The last line will have the same Y as the first
            maxEle = i + 180;
        if (i >= ele && i <= maxEle)
            line.y = float(sin((i - ele) * M_PI / 180.0) * 1500.0);

        lines.push_back(line);
    }
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

void Map::draw(Config &c, const float camD, float posX, float speed) {
    Sprite sbg;
    sbg.setTexture(bg);
    sbg.setScale(Vector2f((float)c.w.getSize().x / bg.getSize().x, (float)c.w.getSize().y * BGS / bg.getSize().y));
    sbg.setPosition(0, 0);
    c.w.draw(sbg);

    // Draw road
    /*Color road = (posY/3)%2?Color(107,107,107):Color(105,105,105);
    drawQuad(w, road, w.getSize().x/2 - posX, w.getSize().y, w.getSize().x/1.2, w.getSize().x/2 + posX * 0.2, w.getSize().y * BGS, w.getSize().x/20);*/

    posY += speed;
    int N = lines.size();
    float playerX = posX * 0.05f;
    int startPos = int(posY) % N;
    float camH = lines[startPos].y + 1500.0f;

    float maxy = c.w.getSize().y;
    float x=0, dx=0;

    ///////draw road////////
    for (int n = startPos; n<startPos + c.renderLen; n++) {
        Line &l = lines[n % N];
        l.project(playerX * ROADW - x, camH, float(startPos * SEGL - (n >= N ? N * SEGL : 0)), camD,
                c.w.getSize().x, c.w.getSize().y, ROADW);
        x += dx;
        dx += l.curve;

        l.clip = maxy;
        if (l.Y < maxy) {
            maxy = l.Y;

            Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
            Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);
            if (n % N == 0)
                road = Color::Red;
            Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : road;

            Line p = lines[(n - 1) % N]; //previous line

            drawQuad(c.w, grass, 0, int(p.Y), c.w.getSize().x, 0, int(l.Y), c.w.getSize().x);
            drawQuad(c.w, rumble, int(p.X), int(p.Y), int(p.W * 1.2f), int(l.X), int(l.Y), int(l.W * 1.2f));
            drawQuad(c.w, road, int(p.X), int(p.Y), int(p.W), int(l.X), int(l.Y), int(l.W));
        }
    }

    ////////draw objects////////
    for (int n = startPos + c.renderLen; n > startPos; n--)
        if (lines[n%N].spriteNum > -1)
            lines[n%N].drawSprite(c.w, objects);

}

Map::Line::Line() {
    spriteX = curve = x = y = z = 0;
}

void Map::Line::project(float camX, float camY, float camZ, float camD, float width, float height, float rW) {
    scale = camD / (z - camZ);
    X = (1.0f + scale * (x - camX)) * width / 2.0f;
    Y = (1.0f - scale * (y - camY)) * height / 2.0f;
    W = scale * rW  * width / 2.0f;
}

void Map::Line::drawSprite(RenderWindow &w, const vector<Texture> &objs) {
    Sprite s(objs[spriteNum]);
    if (left) // left
        spriteX = (float) w.getSize().x / (float) ROADW + offset + (float) s.getScale().x / 2.0f;
    else // right
        spriteX = -(float) w.getSize().x / (float) ROADW - offset - (float) s.getScale().x / 2.0f;

    int width = s.getTextureRect().width;
    int h = s.getTextureRect().height;

    float destX = X + scale * spriteX * w.getSize().x / 2.0f;
    float destY = Y + 4.0f;
    float destW = float(width) * W / 266.0f;
    float destH = float(h) * W / 266.0f;

    destX += destW * spriteX; //offsetX
    destY += destH * (-1.0f);    //offsetY

    float clipH = destY + destH - clip;
    if (clipH < 0)
        clipH = 0;

    if (clipH >= destH) return;
    s.setTextureRect(IntRect(0, 0, width, float(h) - float(h) * clipH / destH));
    s.setScale(destW / float(width), destH / float(h));
    s.setPosition(destX, destY);
    w.draw(s);
}
