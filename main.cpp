#include <ncurses.h>
#include <vector>
#include <stdlib.h>

using namespace std;

vector <char> chars;

int main()
{
    initscr();			/* Start curses mode 		  */
    chars.push_back(' ');
    chars.push_back('.');
    chars.push_back(':');
    chars.push_back('*');
    chars.push_back('%');
    chars.push_back('#');
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 100; ++j) {
            int r = rand()%chars.size();
            mvaddch(i,j, chars[r]);
            //mvaddch(i,10-i,'#');
        }
    }
    //printw("Hola Bictor!");	/* Print Hello World		  */
    refresh();			/* Print it on to the real screen */
    getch();			/* Wait for user input */
    endwin();			/* End curses mode		  */

    return 0;
}

