#include "Pipeline.h"

Pipeline::Pipeline()
{
}

void Pipeline::drawLine(const glm::vec3 &v0_3,const glm::vec3 &v1_3,
                        const std::vector<GenericMap> &lineVertexAttributes,
                        Framebuffer &framebuffer) const
{
  float length = glm::distance(v0_3, v1_3);

  int x0 = round(v0_3.x), y0 = round(v0_3.y);
  int x1 = round(v1_3.x), y1 = round(v1_3.y);

  int delta_x(x1 - x0);
  signed char const ix((delta_x > 0) - (delta_x < 0));
  delta_x = std::abs(delta_x) / 2;

  int delta_y(y1 - y0);
  signed char const iy((delta_y > 0) - (delta_y < 0));
  delta_y = std::abs(delta_y) / 2;

  glm::vec4 color = applyLineFragmentShader(lineVertexAttributes,1.0f,v0_3);
  framebuffer.setPixel(v0_3, color);

  if (delta_x >= delta_y)
  {
    int error(delta_y - (delta_x >> 1));
    while (x0 != x1)
    {
      if ((error >= 0) && (error || (ix > 0)))
      {
        error -= delta_x;
        y0 += iy;
      }
      error += delta_y;
      x0 += ix;
      float w = glm::clamp(glm::distance(glm::vec2(x0,y0), glm::vec2(v0_3.x,v0_3.y))/(length+1e-20f),0.0f,1.0f);

      //std::cout << x0 << "," << y0 << " -> " << v0_3.x << "," << v0_3.y << "//" << v1_3.x << "," << v1_3.y << "(" << length << ") => " << w << std::endl;
      float z = v0_3.z*(1.0f-w) + v1_3.z * w;
      glm::vec3 pos(x0, y0, z);
      glm::vec4 color = applyLineFragmentShader(lineVertexAttributes,w,pos);
      framebuffer.setPixel(pos, color);
    }
  }
  else
  {
    int error(delta_x - (delta_y >> 1));
    while (y0 != y1)
    {
      if ((error >= 0) && (error || (iy > 0)))
      {
        error -= delta_y;
        x0 += ix;
      }
      error += delta_x;
      y0 += iy;
      float w = glm::distance(glm::vec2(x0,y0), glm::vec2(v0_3))/length;
      float z = v0_3.z*(1.0f-w) + v1_3.z * w;
      glm::vec3 pos(x0, y0, z);
      glm::vec4 color = applyLineFragmentShader(lineVertexAttributes,w,pos);
      framebuffer.setPixel(pos, color);
    }
  }
}

void Pipeline::drawTriangle(const glm::vec3 &v0_3,const glm::vec3 &v1_3,const glm::vec3 &v2_3,
                            const std::vector<GenericMap> &triangleVertexAttributes,
                            Framebuffer &framebuffer) const
{
  glm::vec2 v0(v0_3.x, v0_3.y);
  glm::vec2 v1(v1_3.x, v1_3.y);
  glm::vec2 v2(v2_3.x, v2_3.y);

  float area = edgeFunction(v0, v1, v2);
  float minX = std::min(v0.x, std::min(v1.x, v2.x));
  float maxX = std::max(v0.x, std::max(v1.x, v2.x));
  float minY = std::min(v0.y, std::min(v1.y, v2.y));
  float maxY = std::max(v0.y, std::max(v1.y, v2.y));

  for (int x = minX; x <= maxX; x++)
  {
    for (int y = minY; y <= maxY; y++)
    {
      glm::vec2 p(x + 0.5, y + 0.5);
      float w0 = edgeFunction(v1, v2, p);
      float w1 = edgeFunction(v2, v0, p);
      float w2 = edgeFunction(v0, v1, p);
      if (w0 >= 0 && w1 >= 0 && w2 >= 0)
      {
        w0 /= area; w1 /= area; w2 /= area;
        float z = w0 * v0_3.z + w1 * v1_3.z + w2 * v2_3.z;
        glm::vec3 fragmentPos(x,y,z);


        glm::vec4 color = applyTriangleFragmentShader(triangleVertexAttributes, glm::vec3(w0, w1, w2),  fragmentPos);
        //char c = render_chars[ int((color.x/3+color.y/3+color.z/3) * 9 + 0.5) ];
        framebuffer.setPixel(fragmentPos, color);
      }
    }
  }
}

float Pipeline::edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{ return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); }



glm::vec4 Pipeline::applyVertexShader(const GenericMap &vertexAttributes, int vertex_index) const
{
  GenericMap fragmentAttributes_i; //Empty, to be filled in the vertexShader
  glm::vec4 vertex = program.vertexShader(vertexAttributes, program.uniforms, fragmentAttributes_i);
  fragmentAttributes[vertex_index] = fragmentAttributes_i;
  vertex.y *= -1.0;
  return vertex;
}

glm::vec4 Pipeline::applyTriangleFragmentShader(const std::vector<GenericMap> &vertexAttributes, const glm::vec3 &ws,  const glm::vec3 fragmentPos) const
{
  GenericMap fragmentAttributes;
  GenericMap::interpolateTriangle(vertexAttributes, ws, fragmentAttributes);

  fragmentAttributes.set("fragmentPos", fragmentPos);
  return program.fragmentShader(fragmentAttributes, program.uniforms);
}

glm::vec4 Pipeline::applyLineFragmentShader(const std::vector<GenericMap> &vertexAttributes, float w, const glm::vec3 fragmentPos) const
{
  GenericMap fragmentAttributes;
  GenericMap::interpolateLine(vertexAttributes, w, fragmentAttributes);

  fragmentAttributes.set("fragmentPos", fragmentPos);
  return program.fragmentShader(fragmentAttributes, program.uniforms);
}

void Pipeline::drawVAO(VAO &vao, Framebuffer &framebuffer) const
{
  fragmentAttributes = std::vector<GenericMap>(vao.vertexAttributes.size()); //clear it

  std::vector<glm::vec3> vertices;
  static float rotation = 0.0f;
  rotation += 0.15;
  for (unsigned int i = 0; i < vao.vertexAttributes.size(); ++i)
  {
    glm::vec4 vertex = applyVertexShader(vao.vertexAttributes[i], i);
    //Clip it
    float w = vertex.w;
    if (vertex.x >= -w && vertex.x <= w &&
        vertex.y >= -w && vertex.y <= w &&
        vertex.z >= -w && vertex.z <= w)
    {
      //Perspective division it
      vertex /= vertex.w;

      //Map it
      vertex *= 0.5f;
      vertex += 0.5f;
      vertex.x *= framebuffer.getWidth();
      vertex.y *= framebuffer.getHeight();

      vertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
    }
    else //Provisional clip treatment
    {
      if (vertices.size() > 0) vertices.push_back(vertices[vertices.size()-1]);
    }
  }

  if (vertices.size() > 1)
  {
    for (unsigned int i = 0; i < vertices.size(); i += 3)
    {
      /*/
      std::vector<GenericMap> lineVertexAttributes;
      lineVertexAttributes = { fragmentAttributes[i],
                               fragmentAttributes[i+1],
                             };
      drawLine(vertices[i],vertices[i+1], lineVertexAttributes, framebuffer);

      lineVertexAttributes = { fragmentAttributes[i+1],
                               fragmentAttributes[i+2],
                             };
      drawLine(vertices[i+1],vertices[i+2], lineVertexAttributes, framebuffer);

      lineVertexAttributes = { fragmentAttributes[i],
                               fragmentAttributes[i+2],
                             };
      drawLine(vertices[i],vertices[i+2], lineVertexAttributes, framebuffer);


      //drawLine(glm::vec2(vertices[i+1]),glm::vec2(vertices[i+2]), framebuffer);
      //drawLine(glm::vec2(vertices[i]),glm::vec2(vertices[i+2]), framebuffer);
      /*/
      std::vector<GenericMap> triangleVertexAttributes;
      triangleVertexAttributes = { fragmentAttributes[i],
                                   fragmentAttributes[i+1],
                                   fragmentAttributes[i+2]
                                 };

      drawTriangle(vertices[i], vertices[i+1], vertices[i+2], triangleVertexAttributes, framebuffer);
      //*/
    }
  }
}


