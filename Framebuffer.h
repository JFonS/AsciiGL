#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <ncurses.h>

#include <vector>
#include "glm/glm.hpp"

typedef std::pair<int, char> Fragment; //color_id, value_char

class Framebuffer
{
private:

    static char greyrampChars[];

    int width, height;
    std::vector< std::vector<glm::vec4> > colorBuffer;
    std::vector< std::vector<double> > zBuffer;

    static Fragment getColorID(const glm::vec4 &rgb);
    static void initializeColor();

public:

    Framebuffer(int width, int height);

    void setPixel(const glm::vec3 &pos, const glm::vec4 &color);
    void render() const;

    void clearColorBuffer();
    void clearZBuffer();
    void clearBuffers();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // FRAMEBUFFER_H
