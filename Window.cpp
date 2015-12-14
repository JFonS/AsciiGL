#include "Window.h"

using namespace agl;

int Window::getMaxWidth() { initscr(); return getmaxx(stdscr); }
int Window::getMaxHeight() { initscr(); return getmaxy(stdscr); }

Window::Window(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
{
    initscr();
    start_color();
    curs_set(0);
    idcok(stdscr,true);
    nodelay(stdscr, true);
    //scrollok(stdscr, TRUE);

    drawBox = false;

    window = newwin(height, width, y, x);
    wresize(window, height, width);
    wrefresh(window);
}

Window::~Window()
{
    delwin(window);
}

void Window::attrOn(int attr)
{
    wattron(window, attr);
}

void Window::display()
{
    if(drawBox) box(window, 0, 0);
    wrefresh(window);
}

void Window::render(const Framebuffer &fb) const
{
    fb.render(window);
}

void Window::erase() const
{
    werase(window);
}

void Window::setPos(const glm::vec2& pos)
{
    mvwin(window, pos.y, pos.x);
}

void Window::setPos(int x, int y)
{
    mvwin(window, y, x);
}

void Window::setSize(const glm::vec2& size)
{
    wresize(window, size.y, size.x);
}

void Window::setSize(int width, int height)
{
     wresize(window, height, width);
}

void Window::printf(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    wattron(window, COLOR_PAIR(70));
    if(drawBox) wmove(window, y+1, x+1);
    else wmove(window, y, x);
    vwprintw(window, format, args);
    va_end(args);
}
