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
      //std::cout << "id->" << colorID << std::endl;

      std::pair<int,char> c = getColorID(color);
      //std::cout << "id->" << colorID << std::endl;
      int id = c.first;
      if (id != 0)
      {
        if (id != lastColorID)
        {
          wattron(stdscr,COLOR_PAIR(id));
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

const char render_chars[] = {' ', '.','-',':',';','i','c','x','%','#', '#'};

std::pair<int,char> Framebuffer::getColorID(glm::vec4 rgb)
{
  rgb.r = round(rgb.r*5)/5; //rgb.r = [0.0, 0.1666, 0.333, ... , 0.83333, 1.0]
  rgb.g = round(rgb.g*5)/5;
  rgb.b = round(rgb.b*5)/5;
  int id = int(round(rgb.r * 5)) + int(round(rgb.g * 30)) + int(round(rgb.b * 180))+1;
  return std::pair<int,char>(id,'#');
}


