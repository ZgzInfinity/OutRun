/******************************************************************************
 * @file    Map.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Marzo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include <cmath>
#include <random>
#include "Map.hpp"

using namespace std;
using namespace sf;

#define BGS 0.666f // Background size
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
    for(unsigned int i = 0; i < MAXLINES; i++) {
        Line line;
        line.z = i * SEGL;

        // Curves in the first half of the map
        if (i > fh1 && i < fh2) line.curve = c1;

        // Curves in the second half of the map
        if (i > sh1 && i < sh2) line.curve = c2;

        // Random objects
        if (dist(generator) > 0.875f) {
            line.sprite.setTexture(objects[distObj(generator)]);
            if (distBool(generator) == 0) // left
                line.spriteX = (float) w.getSize().x / (float) ROADW + dist(generator) + (float) line.sprite.getScale().x / 2.0f;
            else // right
                line.spriteX = -(float) w.getSize().x / (float) ROADW - dist(generator) - (float) line.sprite.getScale().x / 2.0f;
        }

        // Elevation
        if (i >= ele && i <= MAXLINES - 180) // The last line will have the same Y as the first
            maxEle = i + 180;
        if (i >= ele && i <= maxEle)
            line.y = sin((i - ele) * M_PI / 180) * 1500;

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

void Map::draw(Config &c, const float camD, int posX, float speed) {
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
    float playerX = posX * 0.1f;
    int startPos = posY % N;
    int camH = lines[startPos].y + 1500;

    int maxy = c.w.getSize().y;
    float x=0, dx=0;

    ///////draw road////////
    for(int n = startPos; n<startPos + c.renderLen; n++)
    {
        Line &l = lines[n%N];
        l.project(playerX*ROADW-x, camH, startPos*SEGL - (n>=N?N*SEGL:0), camD, c.w.getSize().x, c.w.getSize().y, ROADW);
        x += dx;
        dx += l.curve;

        l.clip = maxy;
        if (l.Y >= maxy) continue;
        maxy = l.Y;

        Color grass = (n/3)%2?Color(16,200,16):Color(0,154,0);
        Color road = (n/3)%2?Color(107,107,107):Color(105,105,105);
        if (n%N == 0)
            road = Color::Red;
        Color rumble = (n/3)%2?Color(255,255,255):road;

        Line p = lines[(n-1)%N]; //previous line

        drawQuad(c.w, grass, 0, p.Y, c.w.getSize().x, 0, l.Y, c.w.getSize().x);
        drawQuad(c.w, rumble,p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
        drawQuad(c.w, road,  p.X, p.Y, p.W, l.X, l.Y, l.W);
    }

    ////////draw objects////////
    for(int n = startPos + c.renderLen; n > startPos; n--)
        lines[n%N].drawSprite(c.w);

}

Map::Line::Line() {
    spriteX = curve = x = y = z = 0;
}

void Map::Line::project(int camX,int camY,int camZ, float camD, int width, int height, int rW) {
    scale = camD / (z-camZ);
    X = (1 + scale * (x - camX)) * width / 2;
    Y = (1 - scale * (y - camY)) * height / 2;
    W = scale * rW  * width / 2;
}

void Map::Line::drawSprite(RenderWindow &app) {
    Sprite s = sprite;
    int w = s.getTextureRect().width;
    int h = s.getTextureRect().height;

    float destX = X + scale * spriteX * app.getSize().x/2;
    float destY = Y + 4;
    float destW = w * W / 266;
    float destH = h * W / 266;

    destX += destW * spriteX; //offsetX
    destY += destH * (-1);    //offsetY

    float clipH = destY+destH-clip;
    if (clipH < 0) clipH = 0;

    if (clipH >= destH) return;
    s.setTextureRect(IntRect(0,0,w,h-h*clipH/destH));
    s.setScale(destW/w,destH/h);
    s.setPosition(destX, destY);
    app.draw(s);
}