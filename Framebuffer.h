#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <ncurses.h>

#include <vector>
#include "glm/glm.hpp"

class Framebuffer
{
private:
    int width, height;
    std::vector< std::vector<char> >   chars;
    std::vector< std::vector<double> > zBuffer;

public:
    Framebuffer(int width, int height);

    void drawChar(const glm::vec3 &pos, char c);
    void render() const;

    void clearChars();
    void clearZBuffer();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // FRAMEBUFFER_H
