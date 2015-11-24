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

static const vector<float> cubeColors = {
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
    vao.vertices = cube;

    vao.vertices = { glm::vec3(0.1,0.1,0.5), glm::vec3(0.1,0.9,0.5), glm::vec3(0.9, 0.1,0.5) };

    while (true)
    {
        fb.clearChars();
        fb.clearZBuffer();

        //pl.drawTriangle(glm::vec3(0,0,6), glm::vec3(0,fb.getHeight(),6), glm::vec3(fb.getWidth(), 0, 6), fb);
        pl.drawVAO(vao, fb);
        fb.render();

        move(0,0);
        std::this_thread::sleep_for (std::chrono::milliseconds(2000));
    }

    getch();
    endwin();
}

