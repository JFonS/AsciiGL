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
    1.0f,
    1.0f,
    1.0f,

    1.0f,
    1.0f,
    1.0f,

    1.0f,
    1.0f,
    1.0f,

    0.2f,
    0.2f,
    0.2f,

    0.8f,
    0.8f,
    0.8f,

    0.8f,
    0.8f,
    0.8f,

    1.0f,
    1.0f,
    0.0f,

    1.0f,
    1.0f,
    1.0f,

    1.0f,
    0.0f,
    1.0f,

    1.0f,
    0.0f,
    0.0f,

    0.0f,
    1.0f,
    1.0f,

    0.0f,
    1.0f,
    0.0f
};

int main()
{
    initscr();

    Framebuffer fb(getmaxx(stdscr), getmaxy(stdscr));
    Pipeline pl;
    VAO vao;
    vao.addVBO("position", cube);

    std::string a = "colors";
    vao.addVBO(a, cubeColors);

    float rotation = 0.0f;

    glm::mat4 P = glm::perspective(M_PI/2.0, double(fb.getWidth()) / fb.getHeight(), 2.0, 20.0);

    vao.setMat4("project", P);

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

        vao.setMat4("model", M);

        pl.drawVAO(vao, fb);
        fb.render();

        refresh();
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    getch();
    endwin();
}

