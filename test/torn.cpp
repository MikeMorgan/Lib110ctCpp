#include "Lib110ct.h"

void torn(Turtle* t, int depth, double length)
{
    if(depth == 0) t->moveForward(length);
    else{
        torn(t,depth-1,length/2 * 0.9);
        t->turn(-85);
        torn(t,depth-1,length/2*0.9);
        t->turn(170);
        torn(t,depth-1,length/2*0.9);
        t->turn(-85);
        torn(t,depth-1,length/2 * 0.9);

    }
}

void tornSquare(Turtle* t, int depth, double length)
{
    for(int i=0; i<4; ++i)
    {
        torn(t,depth,length);
        t->turn(-90);
    }
}


int main(int argc, char** argv)
{
    Win110ct win;
    win.setPosition(15,14);
    win.setTextColour({128,192,192});
    win.setBackColour({192,64,64});
    SDL_Rect r = {0,0,1024,768};
    int maxOrder;
    win << "Enter the maximimum order for the torn squares (0-10): ";
    win >> maxOrder;
    Turtle * t = win.getTurtle();
    t->penDown();
    t->setPosition(512,200);
    t->setColour({192,192,128});

    for(int i=0; i<maxOrder; ++i)
    {
        win.clearBack();
        t->setPosition(812,200);
        tornSquare(t, i, 400);
        win.render();
        SDL_Delay(2000);
    }

    win.getchar();
    return 0;
}
