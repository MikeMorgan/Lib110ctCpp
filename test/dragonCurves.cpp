#include "Lib110ct.h"
#include <math.h>

void dragonSecond(Turtle*,int,double);

void dragonFirst(Turtle * t, int depth, double length)
{
    t->setColour({192,192,255});
    if(depth == 0) t->moveForward(length);
    else{
        t->turn(-45);
        dragonFirst(t, depth-1, length*sin(M_PI_4));
        t->turn(90);
        dragonSecond(t, depth-1, length*sin(M_PI_4));
        t->turn(-45);
    }
}

void dragonSecond(Turtle * t, int depth, double length)
{
    t->setColour({192,255,192});
    if(depth == 0) t->moveForward(length);
    else{
        t->turn(45);
        dragonFirst(t, depth-1, length*sin(M_PI_4));
        t->turn(-90);
        dragonSecond(t, depth-1, length*sin(M_PI_4));
        t->turn(45);
    }
}

void dragon(Turtle * t, int depth, double length)
{
    dragonFirst(t,depth,length);
}

int main(int argc, char** argv)
{
    Win110ct win;
    Turtle * t = win.getTurtle();
    t->penDown();
    t->setPosition(512,200);
    t->setColour({128,128,255});

    for(int i=0; i<24; ++i)
    {
        t->setPosition(512,200);
        dragon(t, i, 400);
        win.render();
        SDL_Delay(2000);
        win.clearBack();
    }

    win.getchar();
    return 0;
}
