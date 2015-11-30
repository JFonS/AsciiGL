#include <ncurses.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Framebuffer.h"
#include "Pipeline.h"
#include "VAO.h"

using namespace std;

char render_chars[] = {'.','-',':',';','i','c','x','%','#'};

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

glm::vec4 vshader(const GenericMap &vertexAttributes, const GenericMap &uniforms, GenericMap &fragmentAttributes)
{
  glm::vec3 v = vertexAttributes.getVec3("position");
  glm::vec4 pos = glm::scale(glm::mat4(1.0f), glm::vec3(1,0.6,1))  * uniforms.getMat4("P") * uniforms.getMat4("M") * glm::vec4(v, 1.0f);
  //glm::vec4 pos(v,1.0f);
  fragmentAttributes.set("color", vertexAttributes.getVec3("color"));
  //fragmentAttributes.set("pos", pos);

  return  pos;
}

glm::vec4 fshader(const GenericMap &fragmentAttributes, const GenericMap &uniforms)
{

  glm::vec3 pos = fragmentAttributes.getVec3("fragmentPos");
  glm::vec2 uv = glm::vec2(pos.x/uniforms.getInt("screenWidth"),pos.y/uniforms.getInt("screenHeight"));

  glm::vec4 color = glm::vec4(fragmentAttributes.getVec3("color"), 1.0f);
  return color;
  if (fmod(pos.x + 2.0f*sin(pos.y*0.5f), 4.0f) <= 2.0f) return color;
  else return glm::vec4(uv,1,1);
}

int main()
{
  initscr();
  start_color();
  idcok(stdscr,true);

  //*/
  Framebuffer fb(getmaxx(stdscr), getmaxy(stdscr));
  //*/
  //attron(A_REVERSE);
  for(float r = 0.0f; r <= 1.05f; r += 0.2)
  {
    for(float g = 0.0f; g <= 1.05f; g += 0.2)
    {
      for(float b = 0.0f; b <= 1.05f; b += 0.2)
      {
        glm::vec4 color(r,g,b,1.0f);
        std::pair<int,char> idPair = Framebuffer::getColorID(color);
        int id = idPair.first;
        attron(COLOR_PAIR(id));

        //init_color(250, 1000, 1000, 1000); //per escriure en blanc
        //init_pair(250, 250, 0); //per escriure en blanc
        //attron(COLOR_PAIR(250)); //per escriure en blanc

        mvprintw(0, 0, "rgb (%.1f,  %.1f,  %.1f) = %d", r, g, b, id);
        refresh();
        getch();
        erase();
      }
    }
  }
 // refresh();
  //attroff(A_REVERSE);
  //*/
 // getch();
/*
  Pipeline pl2;
  pl2.program.vertexShader = [](const GenericMap &vertexAttributes, const GenericMap &uniforms, GenericMap &fragmentAttributes) {
    fragmentAttributes.set("color",vertexAttributes.getVec3("color"));
    return glm::vec4(vertexAttributes.getVec3("position"), 1.0f);
  };

  pl2.program.fragmentShader = [](const GenericMap &fragmentAttributes, const GenericMap &uniforms) {
    return glm::vec4(fragmentAttributes.getVec3("color"),1.0);
  };

  VAO vao2;
  vector<glm::vec3> triangle =
  {
    glm::vec3(-0.5f,0.5f,-0.5f),
    glm::vec3(0.5f,-0.5f,-0.5f),
    glm::vec3(0.5f,0.5f,-0.5f),

    glm::vec3(-0.5f,-0.5f,-0.5f),
    glm::vec3(0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f,0.5f,-0.5f)

  };

  vector<glm::vec3> triangleColors =
  {
    glm::vec3(0.5f,0.0f,0.0f),
    glm::vec3(0.3f,0.3f,0.0f),
    glm::vec3(0.2f,0.0f,0.2f),

    glm::vec3(0.6f,0.3f,0.9f),
    glm::vec3(0.3f,0.3f,0.0f),
    glm::vec3(0.5f,0.0f,0.0f)
  };

  vao2.addVBO("position", triangle);
  vao2.addVBO("color", triangleColors);

  erase();
  fb.clearBuffers();

  pl2.drawVAO(vao2,fb);

  fb.render();

  refresh();
  getch();

  Pipeline pl;
  pl.program.fragmentShader = fshader;
  pl.program.vertexShader = vshader;

  VAO vao;
  vao.addVBO("position", cube);
  vao.addVBO("color", cubeColors);

  float rotation = 0.0f;

  glm::mat4 P = glm::perspective(M_PI/3.0, double(fb.getWidth()) / fb.getHeight(), 0.5, 40.0);
  pl.program.uniforms.set("P", P);
  pl.program.uniforms.set("screenWidth", fb.getWidth());
  pl.program.uniforms.set("screenHeight", fb.getHeight());

  while (true)
  {
    erase();
    fb.clearBuffers();

    rotation += 0.005f;
    glm::mat4 M;
    M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(-6,0,-13));
    M = glm::rotate(M,rotation,glm::vec3(1,1,0.3));
    M = glm::rotate(M,rotation*1.5f,glm::vec3(0.5,0,1));
    M = glm::scale(M,glm::vec3(2.5));

    pl.program.uniforms.set("M", M);
    pl.drawVAO(vao, fb);

    M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(6,0,-13));
    M = glm::rotate(M,rotation,glm::vec3(1,1,0.3));
    M = glm::rotate(M,rotation*1.5f,glm::vec3(0,1,0));
    M = glm::scale(M,glm::vec3(2.5));

    pl.program.uniforms.set("M", M);
    pl.drawVAO(vao, fb);

    fb.render();

    refresh();
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
  }

  getch();
  */
  endwin();
}
