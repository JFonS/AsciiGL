#include "Texture.h"

Texture::Texture() : components(0), width(0), height(0)
{}

Texture::Texture(int w, int h, int c) : components(c), width(w), height(h)
{data = std::vector<float>(height*width * components, 0.0f);}

void Texture::loadFromFile(const std::string &filepath)
{
  unsigned char *cdata = FileReader::ReadImage(filepath.c_str(), this->components, this->width, this->height);

  //Store in data as 0..1 instead of 0..255
  data = std::vector<float>(height*width * components, 0.0f);
  for(int i = 0; i < width; ++i) //Convert from 0..255 to 0..1
  {
      for(int j = 0; j < height; ++j)
      {
         data[(j*width+i) * components    ] = float(cdata[(j*width+i) * components    ]) / 255;
         data[(j*width+i) * components + 1] = float(cdata[(j*width+i) * components + 1]) / 255;
         data[(j*width+i) * components + 2] = float(cdata[(j*width+i) * components + 2]) / 255;
         data[(j*width+i) * components + 3] = float(cdata[(j*width+i) * components + 3]) / 255;
      }
  }
}

void Texture::sample(float s, float r, glm::vec4 &color)
{
  int x = int(round(s*width)), y = int(round(r*height));
  sample(x,y,color);
}

void Texture::sample(int x, int y, glm::vec4 &color)
{
  int start = (width*y+x) * components;
  color = glm::clamp(glm::vec4(data[start], data[start+1], data[start+2], data[start+3]), 0.0f, 1.0f);
}
