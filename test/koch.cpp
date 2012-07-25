#include "Lib110ct.h"


void koch(Turtle * t, int depth, double length)
{
    if(depth == 0) t->moveForward(length);
    else
    {
        koch(t, depth-1, length/3);
        t->turn(60);
        koch(t, depth-1, length/3);
        t->turn(-120);
        koch(t, depth-1, length/3);
        t->turn(60);
        koch(t, depth-1, length/3);
    }
}

void kochFlake(Turtle * t, int depth, double length)
{
    t->penDown();
    t->setColour({255,128,128});
    koch(t, depth, length);
    t->setColour({128,255,128});
    t->turn(-120);
    koch(t, depth, length);
    t->setColour({128,128,255});
    t->turn(-120);
    koch(t, depth, length);
}

int main( int argc, char* args[] )
{
    Win110ct win;
    int d;

    win.setTextColour({64,255,64});
    win.setPosition(5,10);
    win << "Enter max snowflake depth: ";

    win >> d;

    win.setTextColour({128,128,255});

    Stopwatch s;

    Turtle * t = win.getTurtle();
    win.render();
    win.getchar();

    t->setPosition(800, 100);
    t->setColour({255,64,64});
    win.hideTurtle();

    for(int i=1; i<=d; ++i)
    {
        win.clearBack();
        kochFlake(t,i, 600);
        win.render();
        SDL_Delay(1000);
        t->setPosition(800, 100);
        t->turn(-120);
    }

    win.clear();
    win.setPosition(10,20);
    win << "Press any key to exit program";
    win.getchar();

    return 0;
}
