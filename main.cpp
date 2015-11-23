#include <ncurses.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

vector <char> chars;
vector< vector<double> > zBuffer;

void plot(int x, int y, char c) {
    mvaddch(y,x,c);
}

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
    0.0f,
    0.0f,
    0.0f,

    0.0f,
    0.0f,
    1.0f,

    0.0f,
    1.0f,
    0.0f,

    0.0f,
    1.0f,
    1.0f,

    1.0f,
    0.0f,
    0.0f,

    1.0f,
    0.0f,
    1.0f,

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

const char render_char = '*';

void ascii_line(glm::vec2 p1, glm::vec2 p2) {

    int x1 = round(p1.x), y1 = round(p1.y);
    int x2 = round(p2.x), y2 = round(p2.y);

    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    plot(x1, y1,render_char);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;

            plot(x1, y1, render_char);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;

            plot(x1, y1, render_char);
        }
    }
}


inline float sign (glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

inline bool pointInTriangle (glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    bool b1 = sign(pt, v1, v2) < 0.0f;
    bool b2 = sign(pt, v2, v3) < 0.0f;
    bool b3 = sign(pt, v3, v1) < 0.0f;
    return ((b1 == b2) && (b2 == b3));
}

float edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{ return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); }

char render_chars[] = {'.','-',':',';','i','c','x','%','#'};

void ascii_triangle(const glm::vec3 &v0_3,const glm::vec3 &v1_3,const glm::vec3 &v2_3, float c0, float c1, float c2)
{
    static int i = 0;
    glm::vec2 v0(v0_3.x, v0_3.y);
    glm::vec2 v1(v1_3.x, v1_3.y);
    glm::vec2 v2(v2_3.x, v2_3.y);

    float area = edgeFunction(v0, v1, v2);

    float minX = min(v0.x, min(v1.x, v2.x));
    float maxX = max(v0.x, max(v1.x, v2.x));
    float minY = min(v0.y, min(v1.y, v2.y));
    float maxY = max(v0.y, max(v1.y, v2.y));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            glm::vec2 p(x + 0.5, y + 0.5);
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float z = w0 * v0_3.z + w1 * v1_3.z + w2 * v2_3.z;
                float color = w0 * c0 + w1 * c1 + w2 * c2;
                mvprintw(0,0, "%f,   %f", z, zBuffer[x][y]);
                if(z < zBuffer[x][y])
                {
                    plot(x, y, render_chars[int(round(color*sizeof(render_chars)))]);//(render_chars[((i) % sizeof(render_chars))]));
                    zBuffer[x][y] = z;
                }
            }
        }
    }
    ++i;
}

/*void ascii_triangle(glm::vec3 v1_3, glm::vec3 v2_3, glm::vec3 v3_3)
{
    glm::vec2 v1(v1_3.x, v1_3.y);
    glm::vec2 v2(v2_3.x, v2_3.y);
    glm::vec2 v3(v3_3.x, v3_3.y);

    static int i = 0;
    float minX = min(v1.x, min(v2.x, v3.x));
    float maxX = max(v1.x, max(v2.x, v3.x));
    float minY = min(v1.y, min(v2.y, v3.y));
    float maxY = max(v1.y, max(v2.y, v3.y));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            if ( pointInTriangle(glm::vec2(x,y), v1, v2, v3) )
            {
                //CAL DETERMINAR LA Z AMB ALGUNA INTERPOLACIO XUNGA, aquesta me la he tret dels ous, me voy
                double z = (double(x)/abs(v1.x - v2.x) + double(y)/abs(v1.y - v2.y))/2 * (v1_3.z-v2_3.z) + v1_3.z;
                if(z < zBuffer[x][y])
                {
                    plot(x, y, (render_chars[((i) % sizeof(render_chars))]));
                    zBuffer[x][y] = z;
                }
            }
        }
    }
    ++i;
}*/

int main()
{
    initscr();
    int WINDOW_WIDTH = getmaxx(stdscr), WINDOW_HEIGHT = getmaxy(stdscr);

    float rotation = 0.0f;
    glm::mat4 P = glm::perspective(M_PI/2.0, double(WINDOW_WIDTH)/double(WINDOW_HEIGHT), 2.0, 20.0);
    while (true)
    {
        double height = 7.5, size = 3.5;
        vector <glm::vec4> triangle(12);
        triangle[0] = glm::vec4( -size, 0, -size, 1.0);
        triangle[1] = glm::vec4(  size, 0, -size, 1.0);
        triangle[2] = glm::vec4( 0.0, height, 0, 1.0); //tip

        triangle[3] = glm::vec4( -size, 0, size, 1.0);
        triangle[4] = glm::vec4(  size, 0, size, 1.0);
        triangle[5] = triangle[2];

        triangle[6] = glm::vec4( size, 0, -size, 1.0);
        triangle[7] = glm::vec4( size, 0,  size, 1.0);
        triangle[8] = triangle[2];

        triangle[9]  = glm::vec4( -size, 0, -size, 1.0);
        triangle[10] = glm::vec4( -size, 0,  size, 1.0);
        triangle[11] = triangle[2];

        vector<glm::vec3> lines;
        vector<float> colors;
        rotation += 0.15;
        glm::mat4 M(1.0f);
        M = glm::translate(M, glm::vec3(0,2,-13));
        M = glm::rotate(M,rotation,glm::vec3(0,1,0));
        M = glm::scale(M,glm::vec3(4.0));

        for (int i = 0; i < cube.size(); ++i)
        {
            glm::vec4 vertex = glm::vec4(cube[i],1.0);
            vertex.y *= -1.0;
            vertex = glm::scale(glm::mat4(1.0),glm::vec3(1,0.6,1))*P*M*vertex;
            float w = vertex.w;

            if (vertex.x >= -w && vertex.x <= w &&
                    vertex.y >= -w && vertex.y <= w &&
                    vertex.z >= -w && vertex.z <= w)
            {
                vertex /= vertex.w;
                vertex *= 0.5f;
                vertex += 0.5f;


                vertex.x *= WINDOW_WIDTH;
                vertex.y *= WINDOW_HEIGHT;

                lines.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
                colors.push_back(cubeColors[i]);
                //sfmlTriangle.append(sf::Vertex(sf::Vector2f(vertex.x,vertex.y)));
            } else {
                if (lines.size() > 0)
                    lines.push_back(lines[lines.size()-1]);

            }
        }

        clear();
        zBuffer = vector< vector<double> >(WINDOW_WIDTH, vector<double>(WINDOW_HEIGHT, 9999999.9) ); //clear zBuffer
        if (lines.size() > 1) {
            for (int i = 0; i < lines.size(); i+=3)
            {
                ascii_triangle(lines[i], lines[i+1], lines[i+2], colors[i], colors[i+1], colors[i+2]);
                /*
                ascii_line(lines[i],  lines[i+1]);
                ascii_line(lines[i+1],lines[i+2]);
                ascii_line(lines[i+2],  lines[i]);
                */
                //mvprintw(i,0,"%d ->  %d, %d,    %d,%d", i, p0.x,p0.y, p1.x,p1.y);
            }
        }
        move(0,0);
        refresh();
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
    getch();			/* Wait for user input */
    endwin();			/* End curses mode		  */
}

