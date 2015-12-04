#include "Texture.h"

Texture::Texture() : components(0), width(0), height(0) {}

Texture::Texture(int w, int h, int c) : components(c), width(w), height(h) {}

void Texture::loadFromFile(const std::string &filepath)
{
  this->data = FileReader::ReadImage(filepath.c_str(), this->components, this->width, this->height);
}

void Texture::sample(float s, float r, glm::vec4 &color)
{
  int x = int(round(s*width)), y = int(round(r*height));
  sample(x,y,color);
}

void Texture::sample(int x, int y, glm::vec4 &color)
{
  int start = (width*y+x) * components;
  color = glm::clamp(glm::vec4(float(data[start])/255, float(data[start+1])/255, float(data[start+2])/255, float(data[start+3])/255),0.0f,1.0f);
}
