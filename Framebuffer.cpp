#include "Framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
  fillColorTable();
  clearBuffers();
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
      if (id != 0){
        if (id != lastColorID) {
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

void Framebuffer::rgb2hsv(glm::vec4 rgb, glm::vec4 &hsv)
{
  // RGB are from 0..1, H is from 0..360, SV from 0..1
 /* double maxC = rgb.b;
  if (maxC < rgb.g) maxC = rgb.g;
  if (maxC < rgb.r) maxC = rgb.r;
  double minC = rgb.b;
  if (minC > rgb.g) minC = rgb.g;
  if (minC > rgb.r) minC = rgb.r;

  double delta = maxC - minC;

  double V = maxC;
  double S = 0;
  double H = 0;

  if (delta == 0)
  {
    H = 0;
    S = 0;
  }
  else
  {
    S = delta / maxC;
    double dR = 60*(maxC - rgb.r)/delta + 180;
    double dG = 60*(maxC - rgb.g)/delta + 180;
    double dB = 60*(maxC - rgb.b)/delta + 180;
    if (rgb.r == maxC)
      H = dB - dG;
    else if (rgb.g == maxC)
      H = 120 + dR - dB;
    else
      H = 240 + dG - dR;
  }

  if (H<0)
    H+=360;
  if (H>=360)
    H-=360;

  hsv = glm::vec4(float(H), float(S), float(V), 1.0f);
  */

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
  hsv.x = fabs(K + (rgb.g - rgb.b) / (6.f * chroma + 1e-20f)) * 360.0;
  hsv.y = chroma / (rgb.r + 1e-20f);
  hsv.z = rgb.r;

}

void Framebuffer::hsv2rgb(glm::vec4 hsv, glm::vec4 &rgb)
{
  float fC = hsv.z * hsv.y; // Chroma
  float fHPrime = fmod(hsv.x / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = hsv.z - fC;

  if(0 <= fHPrime && fHPrime < 1) rgb = glm::vec4(fC, fX, 0, 1.0);
  else if(1 <= fHPrime && fHPrime < 2) rgb = glm::vec4(fX, fC, 0, 1.0);
  else if(2 <= fHPrime && fHPrime < 3) rgb = glm::vec4(0, fC, fX, 1.0);
  else if(3 <= fHPrime && fHPrime < 4)  rgb = glm::vec4(0, fX, fC, 1.0);
  else if(4 <= fHPrime && fHPrime < 5)  rgb = glm::vec4(fX, 0, fC, 1.0);
  else if(5 <= fHPrime && fHPrime < 6)  rgb = glm::vec4(fC, 0, fX, 1.0);
  else rgb = glm::vec4(0, 0, 0, 1.0);
  rgb += glm::vec4(glm::vec3(fM), 0);
}

void Framebuffer::fillColorTable()
{
  //TO DO: call only once
  //if(colorTable.size() > 0) return;
  init_color(0,0,0,0); //Define 0 as black
  init_pair(0,0,0);
  const int hueSize = 32;
  const int satSize = 8;
  //attron(A_REVERSE);
  for(float r = 0.0f; r < 1.01f; r += 0.05)
  {
    for(float g = 0.0f; g < 1.01f; g += 0.05)
    {
      for(float b = 0.0f; b < 1.01f; b += 0.05)
      {
        std::pair<int,char> idPair = getColorID(glm::vec4(r,g,b,1));
        //int id = idPair.first;
        //if (id != 0)
        {
          glm::vec3 roundRGB = glm::vec3(r,g,b)*9.0f;
          roundRGB = glm::vec3(round(roundRGB.r),round(roundRGB.g),round(roundRGB.b));
          int id = int(roundRGB.r) + int(roundRGB.g * 10) + int(roundRGB.b * 100);
          roundRGB = roundRGB/9.0f * 1000.0f;
          init_color(id, int(roundRGB.r), int(roundRGB.g), int(roundRGB.b));
          //std::cout << "RGB: " << "(" << rgb.r << ", " << rgb.g << ", " << rgb.b << ")" << std::endl;
          //std::cout << "HSV: " << "(" << h << ", " << s << ")" << std::endl;
          //std::cout << id << std::endl;
          //attron(3);
          //mvprintw(0, 0, "%f,%f,%f %d", r, g, b, id);
          //refresh();
          //getch();
          //clear();
          init_pair(id,id,0);
        }
      }
    }
  }

  /*for(int hue = 0; hue < hueSize; ++hue)
  {
    for(int sat = 0; sat < satSize; ++sat)
    {
      float h = float(hue)/32;
      float s = float(sat)/8;
      glm::vec4 rgb;
      hsv2rgb(glm::vec4(h*360,s,1.0,1.0), rgb);

      std::pair<int,char> idPair = getColorID(rgb);
      int id = idPair.first;
      //mvprintw(hue,(sat*4)+50,"%d", id, hue, sat);

      if (id != 0)
      {
        init_color(id, int(round(rgb.r * 1000)), int(round(rgb.g * 1000)), int(round(rgb.b * 1000)));
        //std::cout << "RGB: " << "(" << rgb.r << ", " << rgb.g << ", " << rgb.b << ")" << std::endl;
        //std::cout << "HSV: " << "(" << h << ", " << s << ")" << std::endl;
        //std::cout << id << std::endl;
        init_pair(id,id,0);
        //if (sat == 0) {
          attron(COLOR_PAIR(id));
          //mvprintw(hue,sat*10, "%f,%f,%f", rgb.r,rgb.g,rgb.b);
   //       mvprintw(hue,(sat*4)+80,"%3d", id, hue, sat);
        //}
      }
    }
  }*/
}

const char render_chars[] = {' ', '.','-',':',';','i','c','x','%','#', '#'};

std::pair<int,char> Framebuffer::getColorID(glm::vec4 rgb)
{
  /*if (rgb.r == 0.0f) rgb.r = 0.05f;
  if (rgb.g == 0.0f) rgb.g = 0.05f;
  if (rgb.b == 0.0f) rgb.b = 0.05f;
  glm::vec4 hsv(0.0f);
  rgb2hsv(rgb, hsv);

  int hue = int(round(hsv.x / 360 * 32));
  int sat = int(round(hsv.y * 8));
  int id = hue*8  + sat; // - 1 - 8;

  if (id > 255) id = 255;

  char c = render_chars[int(round(hsv.z * 10))];

  //if (rgb.r == rgb.g && rgb.g == rgb.b) id = 32;
  if (sat == 0) return std::pair<int,char>(8,c);
  return std::pair<int,char>(id,c);*/

  //float m = glm::max(glm::max(rgb.r,rgb.g),max(m,rgb.b));
  //glm::vec3 maxRGB = rgb  * (1.0f/m);


  int id = int(round(rgb.r*9)) + int(round(rgb.g*9) + 10) + int(round(rgb.b*9)*100);
  return std::pair<int,char>(int(round(float(id)/999.0f * 255)),'#');
/*
  int irri = int(round(rgb.r*6));
  int gi = int(round(rgb.g*6));
  int bi = int(round(rgb.b*6));
  int id = irri*36 + gi*6  + bi;
*/
}


