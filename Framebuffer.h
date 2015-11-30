#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <ncurses.h>

#include <vector>
#include "glm/glm.hpp"

class Framebuffer
{
private:
    int width, height;
    std::vector< std::vector<glm::vec4> > colorBuffer;
    std::vector< std::vector<double> > zBuffer;

    //0..31 hue tones, 0..7 saturation values.
    static void fillColorTable();

public:
    static std::pair<int,char> getColorID(glm::vec4 rgb);
    static void rgb2hsv(glm::vec4 rgb, glm::vec4 &hsv);
    static void hsv2rgb(glm::vec4 hsv, glm::vec4 &rgb);

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
