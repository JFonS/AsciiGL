#include "Framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
  clearBuffers();
  fillColorTable();
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
  //clear();
  int lastColorID = -1;
  for(int x = 0; x < width; ++x)
  {
    for(int y = 0; y < height; ++y)
    {
      glm::vec4 color = colorBuffer[x][y];
      std::pair<int,char> c = getColorID(color);
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
  //refresh();
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

void Framebuffer::fillColorTable()
{
  //attron(A_REVERSE);
    for(float b = 0.0f; b < 1.05f; b += 0.2f)
    {
        for(float g = 0.0f; g < 1.05f; g += 0.2f)
        {
              for(float r = 0.0f; r < 1.05f; r += 0.2f)
              {
                  std::pair<int,char> idPair = getColorID(glm::vec4(r,g,b,1));
                  int id = idPair.first;
                  //std::cout << "rgb ( " << r << ", " << g << ", " << b << ") = " << id  << std::endl;
                  init_color(id, int(r * 1000), int(g * 1000), int(b * 1000));
                  init_pair(id, id, COLOR_BLACK);
              }
        }
  }
  init_color(0,0,0,0); //Define 0 as black
  init_pair(0,0,0);
}

const char render_chars[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrft/\|()1{}[]?+!lI;:\"^`'.";
//const char render_chars[] = {'.','-',':',';','i','c','x','$','#'};

std::pair<int,char> Framebuffer::getColorID(glm::vec4 rgb)
{
  float maxVal = std::max(rgb.r, std::max(rgb.g, rgb.b));
  float mapVal = 1.0/(maxVal + 0.001);

  glm::vec3 newRGB = glm::vec3(rgb.r, rgb.g, rgb.b);
  newRGB *= mapVal;
  newRGB.r = round(newRGB.r*5)/5; //newRGB.r = [0.0, 0.1666, 0.333, ... , 0.83333, 1.0]
  newRGB.g = round(newRGB.g*5)/5;
  newRGB.b = round(newRGB.b*5)/5;

  int id = int(round(newRGB.r * 5)) + int(round(newRGB.g * 30)) + int(round(newRGB.b * 180))+1;

  maxVal = glm::length(rgb)  / (glm::length(glm::vec3(1,1,1)));
  char rchar = render_chars[ int(maxVal * sizeof(render_chars)) ];

  return std::pair<int,char>(id, rchar);
}


