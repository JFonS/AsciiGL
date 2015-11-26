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
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),

};

std::vector<float> cubeColors = {
    1.0f,//A
    1.0f,
    1.0f,

    0.3f,//B
    0.3f,
    0.3f,

    0.7f,//C
    0.7f,
    0.7f,

    0.3f,//B
    0.3f,
    0.3f,

    1.0f,//A
    1.0f,
    1.0f,

    0.7f,//C
    0.7f,
    0.7f,

    0.2f,//D
    0.2f,
    0.2f,

    0.8f,//E
    0.8f,
    0.8f,

    0.8f,//E
    0.8f,
    0.8f,

    0.4f,//F
    0.4f,
    0.4f,

    0.4f,//F
    0.4f,
    0.4f,

    0.2f,//D
    0.2f,
    0.2f
};

glm::vec4 vshader(const GenericMap &vertexAttributes, const GenericMap &uniforms, GenericMap &fragmentAttributes)
{
    glm::vec3 v = vertexAttributes.getVec3("position");

    fragmentAttributes.set("color", vertexAttributes.getFloat("color"));

    return  glm::scale(glm::mat4(1.0f), glm::vec3(1,0.6,1))  * uniforms.getMat4("P") * uniforms.getMat4("M") * glm::vec4(v, 1.0f);
}

glm::vec4 fshader(const GenericMap &fragmentAttributes, const GenericMap &uniforms)
{
    return glm::vec4(glm::vec3(fragmentAttributes.getFloat("color")), 1.0f);
}

int main()
{
    initscr();

    Framebuffer fb(getmaxx(stdscr), getmaxy(stdscr));
    Pipeline pl;
    pl.program.fragmentShader = fshader;
    pl.program.vertexShader = vshader;

    VAO vao;
    vao.addVBO("position", cube);
    vao.addVBO("color", cubeColors);

    float rotation = 0.0f;

    glm::mat4 P = glm::perspective(M_PI/2.0, double(fb.getWidth()) / fb.getHeight(), 2.0, 20.0);
    pl.program.uniforms.set("P", P);

    while (true)
    {
        clear();

        fb.clearChars();
        fb.clearZBuffer();

        rotation += 0.2f;
        glm::mat4 M(1.0f);
        M = glm::translate(M, glm::vec3(0,2,-13));
        M = glm::rotate(M,rotation,glm::vec3(1,1,0.3));
        M = glm::rotate(M,rotation*1.5f,glm::vec3(0.5,0,1));
        M = glm::scale(M,glm::vec3(4.0));

        pl.program.uniforms.set("M", M);

        pl.drawVAO(vao, fb);
        fb.render();

        refresh();
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    getch();
    endwin();
}

