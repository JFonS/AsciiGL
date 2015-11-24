#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
    clearChars();
    clearZBuffer();
}

void Framebuffer::drawChar(const glm::vec3 &pos, char c)
{
    if(pos.z < zBuffer[pos.x][pos.y])
    {
        chars[pos.x][pos.y] = c;
        zBuffer[pos.x][pos.y] = pos.z;
    }
}

void Framebuffer::render() const
{
    //clear();
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            if(chars[x][y] != ' ') mvaddch(y, x, chars[x][y]);
        }
    }
    //refresh();
}

void Framebuffer::clearChars()
{
    chars   = std::vector< std::vector<char> >(width, std::vector<char>(height, ' '));
}

void Framebuffer::clearZBuffer()
{
    zBuffer = std::vector< std::vector<double> >(width, std::vector<double>(height, 999999.9));
}
