#include "Window.h"

using namespace agl;

int Window::getMaxWidth() { initscr(); return getmaxx(stdscr); }  //TODO: redundant initscr()
int Window::getMaxHeight() { initscr(); return getmaxy(stdscr); } //TODO: redundant initscr()

Window::Window(int x, int y, int width, int height) : x(x), y(y), width(width), height(height), title("")
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

void Window::setTitle(const std::string &newTitle)
{
    title = " " + newTitle + " ";
}

void Window::display()
{
    if(drawBox) 
    {
        box(window, 0, 0);
        if (title != "") write(2,0, title);
    }

    //Clip and show the window :S
    int cx = getClippedX(), cy = getClippedY();
    int cw = getClippedWidth(), ch = getClippedHeight();

    if(cw > 0 && ch > 0)
    {
        mvwin(window, cy, cx);
        wresize(window, ch, cw);

        wrefresh(window);
    }
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
    setPos(pos.x, pos.y);
}

void Window::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Window::setSize(const glm::vec2& size)
{
    setSize(size.x, size.y);
}

void Window::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
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

void Window::write(int x, int y, const std::string &str)
{
    printf(x, y, str.c_str());
}

int Window::getClippedX()
{
    return glm::clamp(x, 0, Window::getMaxWidth());
}

int Window::getClippedY()
{
    return glm::clamp(y, 0, Window::getMaxHeight());
}

int Window::getClippedWidth()
{
    int rx = getClippedX();
    return glm::clamp(width, 0, Window::getMaxWidth() - rx);
}

int Window::getClippedHeight()
{
    int ry = getClippedY();
    return glm::clamp(height, 0, Window::getMaxHeight() - ry);
}
