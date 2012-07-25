#include "Lib110ct.h"

int main(int argc, char** argv)
{
    Win110ct win;
    int x = 40;
    int y = 20;
    char input = ' ';
    win.setEcho(false);
    win.setTextColour({64,128,192});
    win.setPosition(10,4);
    win << "Press w, a, s, or d to move the character around the screen (or x to exit)";

    while(input != 'x')
    {
        win >> input;
        win.setPosition(x,y);
        win.clearChar(0,0);

        switch(input)
        {
            case 'w': --y; break;
            case 'a': --x; break;
            case 's': ++y; break;
            case 'd': ++x; break;
        }
        win.setPosition(x,y);
        win << '#';
    }
    return 0;
}


