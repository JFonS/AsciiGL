#include <ncurses.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

vector <char> chars;

void plot(int x, int y, char c) {
    mvaddch(y,x,c);
}

void ascii_line(glm::vec2 p1, glm::vec2 p2) {

    int x1 = int(p1.x), y1 = int(p1.y);
    int x2 = int(p2.x), y2 = int(p2.y);

    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    plot(x1, y1,'*');

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

            plot(x1, y1, '*');
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

            plot(x1, y1, '*');
        }
    }
}


int main()
{
    initscr();
    int WINDOW_WIDTH = getmaxx(stdscr), WINDOW_HEIGHT = getmaxy(stdscr);

    float rotation = 0.0f;
    glm::mat4 P = glm::perspective(M_PI/2.0, double(WINDOW_WIDTH)/double(WINDOW_HEIGHT), 2.0, 20.0);
    while (true)
    {
        double height = 2.5, size = 2.5;
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

        vector<glm::vec2> lines;
        rotation += 0.15;
        glm::mat4 M(1.0f);
        M = glm::translate(M, glm::vec3(0,2,-7));
        M = glm::rotate(M,rotation,glm::vec3(0,1,0));

        for (int i = 0; i < triangle.size(); ++i)
        {
            glm::vec4 vertex = triangle[i];
            vertex.y *= -1.0;
            vertex = P*M*vertex;
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

                lines.push_back(glm::vec2(vertex.x,vertex.y));
                //sfmlTriangle.append(sf::Vertex(sf::Vector2f(vertex.x,vertex.y)));
            } else {
                //cout << "clipped" << endl;
            }
        }

        clear();
        if (lines.size() > 1) {
            for (int i = 0; i < lines.size(); i+=3)
            {
                ascii_line(lines[i],  lines[i+1]);
                ascii_line(lines[i+1],lines[i+2]);
                ascii_line(lines[i+2],  lines[i]);
                //mvprintw(i,0,"%d ->  %d, %d,    %d,%d", i, p0.x,p0.y, p1.x,p1.y);
            }
        }
        move(0,0);
        refresh();
        std::this_thread::sleep_for (std::chrono::milliseconds(30));
    }
    getch();			/* Wait for user input */
    endwin();			/* End curses mode		  */
}

