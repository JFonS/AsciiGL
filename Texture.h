#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <string>
#include "FileReader.h"

class Texture
{
private:
  unsigned char* data;
  int components, width, height;
public:
  Texture();
  Texture(int w, int h, int components);
  void loadFromFile(const std::string &filepath);

  int getWidth() { return width;}
  int getHeight() { return height;}
  int getComponents() { return components; }
  void sample(float s, float r, glm::vec4 &color);
  void sample(int x, int y, glm::vec4 &color);
};

#endif // TEXTURE_H
