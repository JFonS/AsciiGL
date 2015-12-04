#include <ncurses.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"

#include "Framebuffer.h"
#include "FileReader.h"
#include "Pipeline.h"
#include "Texture.h"
#include "VAO.h"

using namespace std;

static const vector<glm::vec3> cube = {
  glm::vec3(-1.0f,-1.0f,-1.0f),//A
  glm::vec3(-1.0f,-1.0f, 1.0f),
  glm::vec3(-1.0f, 1.0f, 1.0f),

  glm::vec3(-1.0f,-1.0f,-1.0f),//A
  glm::vec3(-1.0f, 1.0f, 1.0f),
  glm::vec3(-1.0f, 1.0f,-1.0f),

  glm::vec3(1.0f, 1.0f,-1.0f),//B
  glm::vec3(-1.0f,-1.0f,-1.0f),
  glm::vec3(-1.0f, 1.0f,-1.0f),

  glm::vec3(1.0f, 1.0f,-1.0f),//B
  glm::vec3(1.0f,-1.0f,-1.0f),
  glm::vec3(-1.0f,-1.0f,-1.0f),

  glm::vec3(1.0f,-1.0f, 1.0f),//C
  glm::vec3(-1.0f,-1.0f,-1.0f),
  glm::vec3(1.0f,-1.0f,-1.0f),

  glm::vec3(1.0f,-1.0f, 1.0f),//C
  glm::vec3(-1.0f,-1.0f, 1.0f),
  glm::vec3(-1.0f,-1.0f,-1.0f),

  glm::vec3(-1.0f, 1.0f, 1.0f),//D
  glm::vec3(-1.0f,-1.0f, 1.0f),
  glm::vec3(1.0f,-1.0f, 1.0f),

  glm::vec3(1.0f, 1.0f, 1.0f),//D
  glm::vec3(-1.0f, 1.0f, 1.0f),
  glm::vec3(1.0f,-1.0f, 1.0f),

  glm::vec3(1.0f, 1.0f, 1.0f),//E
  glm::vec3(1.0f,-1.0f,-1.0f),
  glm::vec3(1.0f, 1.0f,-1.0f),

  glm::vec3(1.0f,-1.0f,-1.0f),//E
  glm::vec3(1.0f, 1.0f, 1.0f),
  glm::vec3(1.0f,-1.0f, 1.0f),

  glm::vec3(1.0f, 1.0f, 1.0f),//F
  glm::vec3(1.0f, 1.0f,-1.0f),
  glm::vec3(-1.0f, 1.0f,-1.0f),

  glm::vec3(1.0f, 1.0f, 1.0f),//F
  glm::vec3(-1.0f, 1.0f,-1.0f),
  glm::vec3(-1.0f, 1.0f, 1.0f)
};

std::vector<glm::vec3> cubeColors = {
  glm::vec3(0.5f,0.0f,0.5f),//A
  glm::vec3(0.5f,0.0f,0.0f),
  glm::vec3(0.5f,0.0f,0.0f),

  glm::vec3(0.5f,0.0f,0.5f),//A
  glm::vec3(0.5f,0.0f,0.0f),
  glm::vec3(0.5f,0.0f,0.5f),


  glm::vec3(1.0f,0.0f,0.0f),//B
  glm::vec3(1.0f,1.0f,0.0f),
  glm::vec3(1.0f,0.0f,0.0f),

  glm::vec3(1.0f,0.0f,0.0f),//B
  glm::vec3(1.0f,1.0f,0.0f),
  glm::vec3(1.0f,1.0f,0.0f),

  glm::vec3(0.0f,1.0f,0.0f),//C
  glm::vec3(0.0f,1.0f,0.0f),
  glm::vec3(0.0f,1.0f,0.0f),
  
  glm::vec3(0.0f,1.0f,0.0f),//C
  glm::vec3(0.0f,1.0f,0.0f),
  glm::vec3(0.0f,1.0f,0.0f),

  glm::vec3(0.2f,0.6f,1.0f),//D
  glm::vec3(0.2f,0.6f,1.0f),
  glm::vec3(0.2f,0.6f,1.0f),

  glm::vec3(0.2f,0.6f,1.0f),//D
  glm::vec3(0.2f,0.6f,1.0f),
  glm::vec3(0.2f,0.6f,1.0f),

  glm::vec3(0.0f,1.0f,1.0f),//E
  glm::vec3(0.0f,1.0f,1.0f),
  glm::vec3(0.0f,1.0f,1.0f),

  glm::vec3(0.0f,1.0f,1.0f),//E
  glm::vec3(0.0f,1.0f,1.0f),
  glm::vec3(0.0f,1.0f,1.0f),

  glm::vec3(0.7f,0.0f,0.2f),//F
  glm::vec3(0.7f,0.0f,0.2f),
  glm::vec3(0.2f,0.2f,0.0f),

  glm::vec3(0.7f,0.0f,0.2f),//F
  glm::vec3(0.2f,0.2f,0.0f),
  glm::vec3(0.2f,0.2f,0.0f)
};

glm::mat4 M, P;
glm::vec4 vshader(const GenericMap &vertexAttributes, const GenericMap &uniforms, GenericMap &fragmentAttributes)
{
  glm::mat4 M, P;
  uniforms.getMat4("M", M);
  uniforms.getMat4("P", P);

  glm::vec3 pos, normal;
  vertexAttributes.getVec3("position", pos);
  vertexAttributes.getVec3("normals", normal);

  glm::vec4 tPos, tNormal;
  tPos = (M * glm::vec4(pos, 1));
  tNormal = (M * glm::vec4(normal, 0));

  glm::vec2 uv; vertexAttributes.getVec2("uvs", uv);
  fragmentAttributes.set("uv", uv);
  fragmentAttributes.set("normal", tNormal.xyz());
  fragmentAttributes.set("position", tPos.xyz());
  return P * tPos;
}

Texture texture;

glm::vec4 fshader(const GenericMap &fragmentAttributes, const GenericMap &uniforms)
{
  glm::vec3 normal;
  fragmentAttributes.getVec3("normal", normal);
  normal = glm::normalize(normal);

  glm::vec2 uv;
  fragmentAttributes.getVec2("uv", uv);
  uv.y = 1.0f - uv.y;

  glm::vec3 lightPos(0, 1, 1);
  float att = glm::clamp(glm::dot(normal, glm::normalize(lightPos)), 0.0f, 1.0f);

  glm::vec4 texColor;
  texture.sample(uv.x, uv.y, texColor);
  return glm::vec4(att * texColor.xyz(), 1);
}

int main()
{
  initscr();
  start_color();
  idcok(stdscr,true);

  Framebuffer fb(getmaxx(stdscr), getmaxy(stdscr));
  fb.clearBuffers();

  Pipeline pl;
  pl.program.fragmentShader = fshader;
  pl.program.vertexShader = vshader;

  std::vector<glm::vec3> pos, normals;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> colors;
  bool triangles;
  FileReader::ReadOBJ("./luigi.obj", pos, uvs, normals, triangles);
  float minx = 99999, maxx = -99999, miny = 99999, maxy = -99999, minz = 99999, maxz = -99999;

  for(int i = 0; i < pos.size(); ++i)
  {
    minx = std::min(minx, pos[i].x);
    maxx = std::max(maxx, pos[i].x);
    miny = std::min(miny, pos[i].y);
    maxy = std::max(maxy, pos[i].y);
    minz = std::min(minz, pos[i].z);
    maxz = std::max(maxz, pos[i].z);
  }

  for(int i = 0; i < pos.size(); ++i)
  {
    glm::vec3 color = pos[i];
    color.x = (color.x - minx) / (maxx - minx);
    color.y = (color.y - miny) / (maxy - miny);
    color.z = (color.z - minz) / (maxz - minz);
    colors.push_back(color);
  }

  VAO vao;
  vao.addVBO("position", pos);
  vao.addVBO("color", colors);
  vao.addVBO("normals", normals);
  vao.addVBO("uvs", uvs);

  float rotation = 0.0f;

  glm::mat4 P = glm::perspective(M_PI/3.0, double(fb.getWidth()) / fb.getHeight(), 0.5, 40.0);
  P = glm::scale(glm::mat4(1.0f), glm::vec3(1,0.6,1)) * P;
  pl.program.uniforms.set("P", P);
  pl.program.uniforms.set("screenWidth", fb.getWidth());
  pl.program.uniforms.set("screenHeight", fb.getHeight());


  static float trans = 0.0f;

  texture.loadFromFile("luigiD.jpg");
  /*
  int fbW = fb.getWidth();
  int fbH = fb.getHeight();
  cout << fbW << " " << fbH << endl;
  fb.clearBuffers();
  for (int i = 0; i < fb.getWidth(); ++i) {
    for (int j = 0; j < fb.getHeight(); ++j) {
      glm::vec4 color = t.sample(float(i)/fbW,float(j)/fbH);
      cout << glm::to_string(glm::vec2(float(i)/fbW,float(j)/fbH)) << " / " << glm::to_string(color) << endl;
      fb.setPixel(glm::vec3(i,j,1),color);
    }
  }
  fb.render();
  refresh();
  getch();
  getch();
  */

  for (int i = 0; i < 100; ++i) //while (true)
  {
    erase();
    fb.clearBuffers();

    trans += 0.05;
    rotation += 0.005f;
    glm::mat4 M(1.0f);
    M = glm::translate(M, glm::vec3(-12,-8, ((sin(trans)*0.5+0.5f)*-37)-1));
    M = glm::rotate(M, rotation*5, glm::vec3(0,1,0));
    //M = glm::rotate(M,3.141592f/2.0f,glm::vec3(-1,0,0));
    //M = glm::rotate(M,rotation,glm::vec3(1,1,0.3));
    //M = glm::rotate(M,rotation*1.5f,glm::vec3(0.5,0,1));
    M = glm::scale(M,glm::vec3(2.5));
    M = glm::translate(M,glm::vec3(-(maxx+minx)/2.0f,-miny,-(maxz+minz)/2.0f));

    pl.program.uniforms.set("color", glm::vec3(1,0.2,0.2));
    pl.program.uniforms.set("M", M);
    //pl.drawVAO(vao, fb);

    M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(((sin(trans*0.5f))*3),-15,-20));
    M = glm::rotate(M, rotation*9, glm::vec3(0,1,0));
    //M = glm::rotate(M,3.141592f/2.0f,glm::vec3(-1,0,0));
    M = glm::scale(M,glm::vec3(0.2));
    //M = glm::translate(M,glm::vec3(-(maxx+minx)/2.0f,-miny,-(maxz+minz)/2.0f));

    pl.program.uniforms.set("color", glm::vec3(0.2,1.0,0.2));
    pl.program.uniforms.set("M", M);
    pl.drawVAO(vao, fb);

    M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(12,-8,-13));
    M = glm::rotate(M, rotation*14, glm::vec3(0,1,0));
    //M = glm::rotate(M,3.141592f/2.0f,glm::vec3(-1,0,0));
    M = glm::scale(M,glm::vec3(2.5));
    M = glm::translate(M,glm::vec3(-(maxx+minx)/2.0f,-miny,-(maxz+minz)/2.0f));

    pl.program.uniforms.set("color", glm::vec3(0.4,0.8,1));
    pl.program.uniforms.set("M", M);
    //pl.drawVAO(vao, fb);

    fb.render();

    refresh();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }

  getch();
  endwin();
}
