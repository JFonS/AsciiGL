#include "Framebuffer.h"
#include <iostream>

char Framebuffer::greyrampChars[] = " .:xh@#"; //;"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
  clearBuffers();
  initializeColor();
}

void Framebuffer::setPixel(const glm::vec3 &pos, const glm::vec4 &color)
{
  if(pos.z < zBuffer[pos.x][pos.y])
  {
    colorBuffer[pos.x][pos.y] = color;
    zBuffer[pos.x][pos.y] = pos.z;
  }
}

void Framebuffer::render() const
{
  int lastColorID = -1;
  for(int x = 0; x < width; ++x)
  {
    for(int y = 0; y < height; ++y)
    {
      glm::vec4 color = colorBuffer[x][y];
      Fragment c = getColorID(color);
      int id = c.first;
      if (id != 0)
      {
        if (id != lastColorID)
        {
          attron(COLOR_PAIR(id));
          lastColorID = id;
        }
        mvaddch(y, x, c.second);
      }
    }
  }
}

void Framebuffer::clearBuffers()
{
  clearColorBuffer();
  clearZBuffer();
}

void Framebuffer::clearColorBuffer()
{
  colorBuffer   = std::vector< std::vector<glm::vec4> >(width, std::vector<glm::vec4>(height, glm::vec4(0.0f)));
}

void Framebuffer::clearZBuffer()
{
  zBuffer = std::vector< std::vector<double> >(width, std::vector<double>(height, 999999.9));
}

void Framebuffer::initializeColor()
{
    for(float b = 0.0f; b < 1.05f; b += 0.2f)
    {
        for(float g = 0.0f; g < 1.05f; g += 0.2f)
        {
              for(float r = 0.0f; r < 1.05f; r += 0.2f)
              {
                  Fragment idPair = getColorID(glm::vec4(r,g,b,1));
                  int id = idPair.first;
                  init_color(id, int(r * 1000), int(g * 1000), int(b * 1000));
                  init_pair(id, id, COLOR_BLACK);
              }
        }
  }
  init_color(0,0,0,0); //Define 0 as black
  init_pair(0,0,0);
}

Fragment Framebuffer::getColorID(const glm::vec4 &rgb) //RGB in 0..1
{
  float maxVal = std::max(rgb.r, std::max(rgb.g, rgb.b));
  float mapVal = 1.0/(maxVal + 0.001); //The mapped colors must have the max value

  //std::cout << maxVal << std::endl;
  glm::vec3 newRGB = glm::vec3(rgb.r, rgb.g, rgb.b);
  newRGB *= mapVal;
  newRGB.r = round(newRGB.r*5)/5; //newRGB.r = [0.0, 0.2, 0.4, ... , 0.8, 1.0]
  newRGB.g = round(newRGB.g*5)/5;
  newRGB.b = round(newRGB.b*5)/5;

  int id = int(round(newRGB.r * 5)) + int(round(newRGB.g * 30)) + int(round(newRGB.b * 180))+1;
  char rchar = greyrampChars[ int(maxVal * (sizeof(greyrampChars)-1.1)) ];

  return Fragment(id, rchar);
}


