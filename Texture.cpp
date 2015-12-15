#include "Texture.h"

using namespace agl;

Texture::Texture() : components(0), width(0), height(0)
{}

Texture::Texture(int w, int h, int c, float initialValue) : components(c), width(w), height(h)
{data = std::vector<float>(height*width * components, initialValue);}

void Texture::set(int x, int y, int component, float value) //set a specific component
{
    data[(x + y*width)*components  + component] = value;
}

void Texture::set(int x, int y, float value) //For 1 component
{
    set(x,y,0,value);
}

void Texture::set(int x, int y, const glm::vec2& value) //For 2 components
{
    set(x,y,0,value[0]); set(x,y,1,value[1]);
}

void Texture::set(int x, int y, const glm::vec3& value) //For 3 components
{
    set(x,y,0,value[0]); set(x,y,1,value[1]);
    set(x,y,2,value[2]);
}

void Texture::set(int x, int y, const glm::vec4& value) //For 4 components
{
    set(x,y,0,value[0]); set(x,y,1,value[1]);
    set(x,y,2,value[2]); set(x,y,3,value[3]);
}

float Texture::sample(int x, int y, int component) const
{
    return data[(x + y*width)*components  + component];
}

glm::vec4 Texture::sample(float s, float r) const
{
  int x = int(round(s*width)), y = int(round(r*height));
  return sample(x,y);
}

glm::vec4 Texture::sample(int x, int y) const
{
  int start = (width*y+x) * components;
  return glm::vec4(data[start], data[start+1], data[start+2], data[start+3]);
}
