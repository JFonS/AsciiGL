#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <ncurses.h>

#include <vector>
#include "glm/glm.hpp"

class Framebuffer
{
private:
    int width, height;
    std::vector< std::vector<char> >   charBuffer;
    std::vector< std::vector<glm::vec4> > colorBuffer;
    std::vector< std::vector<double> > zBuffer;
    static void rgb2hsv(glm::vec4 rgb, glm::vec4 &hsv);

public:
    Framebuffer(int width, int height);

    void drawChar(const glm::vec3 &pos, char c, const glm::vec4 &color);
    void render() const;

    void clearCharBuffer();
    void clearColorBuffer();
    void clearZBuffer();
    void clearBuffers();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // FRAMEBUFFER_H
