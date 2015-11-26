#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
  clearBuffers();
}

void Framebuffer::drawChar(const glm::vec3 &pos, char c, const glm::vec4 &color)
{
  if(pos.z < zBuffer[pos.x][pos.y])
  {
    charBuffer[pos.x][pos.y] = c;
    colorBuffer[pos.x][pos.y] = color;
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
      if(charBuffer[x][y] != ' ') {
        glm::vec4 color = colorBuffer[x][y] * 1000.0f;
//        init_color(1,int(color.x),int(color.y),int(color.z));
//        init_pair(1,1, COLOR_BLACK);
//        wattron(stdscr,COLOR_PAIR(1));
        mvaddch(y, x, charBuffer[x][y]);
      }
    }
  }
  //refresh();
}

void Framebuffer::clearBuffers()
{
  clearCharBuffer();
  clearColorBuffer();
  clearZBuffer();
}

void Framebuffer::clearCharBuffer()
{
  charBuffer   = std::vector< std::vector<char> >(width, std::vector<char>(height, ' '));
}

void Framebuffer::clearColorBuffer()
{
  colorBuffer   = std::vector< std::vector<glm::vec4> >(width, std::vector<glm::vec4>(height, glm::vec4(1.0f)));
}

void Framebuffer::clearZBuffer()
{
  zBuffer = std::vector< std::vector<double> >(width, std::vector<double>(height, 999999.9));
}

void Framebuffer::rgb2hsv(glm::vec4 rgb, glm::vec4 &hsv)
{
  float K = 0.f;

  if (rgb.g < rgb.b)
  {
    std::swap(rgb.g, rgb.b);
    K = -1.f;
  }

  if (rgb.r < rgb.g)
  {
    std::swap(rgb.r, rgb.g);
    K = -2.f / 6.f - K;
  }

  float chroma = rgb.r - std::min(rgb.g, rgb.b);
  hsv.x = fabs(K + (rgb.g - rgb.b) / (6.f * chroma + 1e-20f));
  hsv.y = chroma / (rgb.r + 1e-20f);
  hsv.z = rgb.r;
}

/*void hsv2rgb(glm::vec4 hsv, glm::vec4 &rgb)
{
  int h = hsv.x;
  int s =
  float saturation = 255 - S;

  byte r = V * constrain(0.5+cos(radians(H)),0,1);

  byte g = V * constrain(0.5+cos(radians(H-120)),0,1);

  byte b = V * constrain(0.5+cos(radians(H+120)),0,1);

  byte white = 0.3*r + 0.59*g + 0.11*b;

  R = r + saturation/255 * (white - r);

  G = g + saturation/255 * (white - g);

  B = b + saturation/255 * (white - b);

}*/
