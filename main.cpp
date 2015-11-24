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
    vao.vertices = cube;

    std::string a = "colors";
    vao.addVBO(a, cubeColors);
    float color = vao.getFloat(a);

    while (true)
    {
        clear();
        fb.clearChars();
        fb.clearZBuffer();

        pl.drawVAO(vao, fb);

        fb.render();

        refresh();
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    getch();
    endwin();
}

