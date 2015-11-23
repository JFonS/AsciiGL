#include <ncurses.h>
#include <stdlib.h>

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

vector <char> chars;

int main()
{

    /*initscr();			/* Start curses mode 		  *
    chars.push_back(' ');
    chars.push_back('.');
    chars.push_back(':');
    chars.push_back('*');
    chars.push_back('%');
    chars.push_back('#');
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 100; ++j) {
            int r = rand()%chars.size();
            //mvaddch(i,j, chars[r]);
            //mvaddch(i,10-i,'#');
        }
    }

    int mx,my;
    getmaxyx(stdscr, mx, my);

    move(mx/2,my/2);
    addch('#');
    //printw("Hola Bictor!");	/* Print Hello World		  *
    refresh();			/* Print it on to the real screen *
    getch();			/* Wait for user input *
    endwin();			/* End curses mode		  *
    */
    // create the window
        const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mscratchpad");


//        // define the position of the triangle's points
//        triangle[0].position = sf::Vector2f(10, 10);
//        triangle[1].position = sf::Vector2f(100, 10);
//        triangle[2].position = sf::Vector2f(100, 100);

//        // define the color of the triangle's points
//        triangle[0].color = sf::Color::Red;
//        triangle[1].color = sf::Color::Blue;
//        triangle[2].color = sf::Color::Green;

        float rotation = 0.0f;

        // run the program as long as the window is open
        while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed )
                    window.close();
            }

            sf::VertexArray sfmlTriangle(sf::Triangles, 3);

            vector <glm::vec4> triangle(3);
            triangle[0] = glm::vec4( 0.0, 0.0, -10.0, 1.0);
            triangle[1] = glm::vec4( 2.0, 0.0, -10.0, 1.0);
            triangle[2] = glm::vec4( 0.0, 2.5, -10.0, 1.0);


            glm::mat4 P = glm::perspective(M_PI/2.0, double(WINDOW_WIDTH)/double(WINDOW_HEIGHT), 2.0, 20.0);

            rotation += 0.002;
            glm::mat4 M = glm::rotate(glm::mat4(1.0),rotation,glm::vec3(0,1,0));
            for (auto vertex : triangle) {
                vertex.y *= -1.0;
                vertex = P*M*vertex;
                float w = vertex.w;


                cout << vertex.x << " " << vertex.y << " " << vertex.z << " " << w  << endl;
                if (vertex.x >= -w && vertex.x <= w &&
                        vertex.y >= -w && vertex.y <= w &&
                        vertex.z >= -w && vertex.z <= w) {
                    vertex /= vertex.w;
                    vertex *= 0.5f;
                    vertex += 0.5f;


                    vertex.x *= WINDOW_WIDTH;
                    vertex.y *= WINDOW_HEIGHT;




                    sfmlTriangle.append(sf::Vertex(sf::Vector2f(vertex.x,vertex.y)));
                } else {
                    cout << "clipped" << endl;
                }
            }

            // clear the window with black color
            window.clear(sf::Color::Black);

            // draw everything here...
            window.draw(sfmlTriangle);
            // end the current frame
            window.display();
        }

        return 0;
}

