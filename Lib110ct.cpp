#include <sstream>
#include <cstdlib>
#include <math.h>
#include "Lib110ct.h"




SDL_Surface * FormattedChar::getSurface()
{
    Surf = TTF_RenderText_Blended( font, c, cl);
    return Surf;
}

FormattedChar::FormattedChar():font(0)
{
    cl.r = cl.g = cl.b = 255;
    bg.r = bg.g = bg.b = 0;
    c[0] = ' ';
    c[1] = '\0';
    Surf = SDL_CreateRGBSurface(0, 10, 16, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
}

FormattedChar::FormattedChar(TTF_Font * ft)
{
    cl.r = cl.g = cl.b = 255;
    bg.r = bg.g = bg.b = 0;
    c[0] = ' ';
    c[1] = '\0';
    font = ft;
    TTF_SizeText(ft,"m",&width, &height);
    Surf = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
}

void FormattedChar::setFont(TTF_Font * ft)
{
     SDL_FreeSurface(Surf);
     font = ft;
     TTF_SizeText(ft,"m",&width, &height);
     Surf = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
}

Win110ct::Win110ct(int width, int height, int fontpitch)
{
    winRect.x = 0;
    winRect.y = 0;
    winRect.w = width;
    winRect.h = height;
    t = 0;

    SDL_Init( SDL_INIT_EVERYTHING );

    if(SDL_GetVideoInfo()->hw_available == 1)
        screen = SDL_SetVideoMode( width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    else
        screen = SDL_SetVideoMode( width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF );

    SDL_WM_SetCaption("110ct Console",NULL);

    if( TTF_Init() == -1 )
    {
        return;
    }
    font = TTF_OpenFont("C:\\110Fonts\\FreeMonoBold.ttf",fontpitch);
    TTF_SizeText(font,"m",&fontWidth, &fontHeight);
    ++fontHeight;
    ncols = winRect.w/fontWidth - 1;
    nlines = winRect.h/fontHeight;

    scrBuff = new FormattedChar*[nlines];
    for(int i=0; i<nlines; ++i)
    {
        scrBuff[i] = new FormattedChar[ncols];
        for(int j=0; j<ncols; ++j)
            scrBuff[i][j].setFont(font);
    }

    textColour.r = textColour.g = textColour.b = 255;
    backColour.r = backColour.g = backColour.b = 0;
    echo = true;
    waiting = got = turtling = false;
    SDL_EnableUNICODE(1);
    xpos = ypos = 0;

    backg = SDL_CreateRGBSurface( 0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    textSurf =  SDL_CreateRGBSurface( 0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

}

Win110ct::Win110ct(std::string fontFile, int width, int height, int fontpitch)
{
    winRect.x = 0;
    winRect.y = 0;
    winRect.w = width;
    winRect.h = height;
    t = 0;

    SDL_Init( SDL_INIT_EVERYTHING );

    if(SDL_GetVideoInfo()->hw_available == 1)
        screen = SDL_SetVideoMode( width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    else
        screen = SDL_SetVideoMode( width, height, 32, SDL_SWSURFACE | SDL_DOUBLEBUF );

    SDL_WM_SetCaption("110ct Console",NULL);

    if( TTF_Init() == -1 )
    {
        return;
    }
    font = TTF_OpenFont(fontFile.c_str() ,fontpitch);
    TTF_SizeText(font,"m",&fontWidth, &fontHeight);
    ++fontHeight;
    ncols = winRect.w/fontWidth - 1;
    nlines = winRect.h/fontHeight;

    scrBuff = new FormattedChar*[nlines];
    for(int i=0; i<nlines; ++i)
    {
        scrBuff[i] = new FormattedChar[ncols];
        for(int j=0; j<ncols; ++j)
            scrBuff[i][j].setFont(font);
    }

    textColour.r = textColour.g = textColour.b = 255;
    backColour.r = backColour.g = backColour.b = 0;
    echo = true;
    waiting = got = turtling = false;
    SDL_EnableUNICODE(1);
    xpos = ypos = 0;

    backg = SDL_CreateRGBSurface( 0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    textSurf =  SDL_CreateRGBSurface( 0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

}

Win110ct::~Win110ct()
{
    for(int i=0; i<nlines; ++i)
        delete [] scrBuff[i];
    delete [] scrBuff;
    if(t) delete t;
    SDL_FreeSurface(textSurf);
    SDL_FreeSurface(backg);
    SDL_FreeSurface(screen);
    SDL_EnableUNICODE(0);
    SDL_Quit();
}

void Win110ct::applySurface( int x, int y, SDL_Surface* src, SDL_Surface* dest, SDL_Rect* clip)
{
    SDL_Rect r;

    r.x = x;
    r.y = y;

    SDL_BlitSurface(src, clip, dest, &r);
}

bool Win110ct::checkInput()
{
        if(!waiting)
            waiting = true;
        else
        {
            SDL_Event theEvent;
            SDL_PumpEvents();
            if(SDL_PeepEvents(&theEvent,1,SDL_GETEVENT,SDL_KEYDOWNMASK) != 0)
            {
                got = true;
                last = (char) theEvent.key.keysym.unicode;
                waiting = false;
                return true;
            }
        }
        return false;
}

void Win110ct::moveCursor()
{
    if(++xpos >= ncols)
    {
        ypos++;
        xpos=0;
    }
}

char Win110ct::getchar()
{
    if(got == true)
    {
        got = false;
        return last;
    }
    bool quit = false;
    char input;
    while(!quit)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_RSHIFT ||
               event.key.keysym.sym == SDLK_LSHIFT ||
               event.key.keysym.sym == SDLK_CAPSLOCK) break;
            else if(event.key.keysym.sym == SDLK_BACKSPACE && ypos > 0)
            {
                if(xpos > startx || ypos > starty)
                {
                    if(xpos>0)
                        --xpos;
                    else if(ypos>0)
                    {
                        --ypos;
                        xpos = ncols-1;
                    }
                    if(ypos < nlines && xpos < ncols)
                    {
                        scrBuff[ypos][xpos] = ' ';
                    }
                    if(echo){
                        bufferChar();
                        render();
                    }
                    bufStr = bufStr.substr(0, bufStr.size()-1);
                }

            }
            else if(event.key.keysym.sym == SDLK_RETURN)
            {
                input = 13;
                if(echo) putchar(input);
                quit = true;
                break;
            }
            else
            {
                input = (char) event.key.keysym.unicode;
                if(echo){
                    putchar(input);
                }
            }
            quit = true;
        }
    }

    if(echo)
        render();
    return input;
}


void Win110ct::putchar(char c)
{
    addChar(c);
    if(c != 13 && c != 10 && c != '\t')
    {
        bufferChar();
        moveCursor();
    }
}

void Win110ct::bufferChar()
{
    if(ypos >= nlines || xpos >= ncols || ypos < 0  || xpos < 0) return;
    SDL_Rect charRect = {xpos*fontWidth, ypos*fontHeight, fontWidth+1, fontHeight};
    SDL_Color cl = scrBuff[ypos][xpos].getBack();
    Uint32 c = 0xDD000000 | cl.b << 16 | cl.g << 8 | cl.r;
    SDL_FillRect(textSurf, &charRect, c);

    applySurface(xpos*fontWidth, ypos*fontHeight,scrBuff[ypos][xpos].getSurface(), textSurf);
}

void Win110ct::clearChars(int nChars, int xoffset, int yoffset)
{
    int _x = xpos;
    int _y = ypos;
    SDL_Rect charRect = {xpos*fontWidth, ypos*fontHeight, fontWidth, fontHeight};

    for(int i=0; i<nChars; ++i)
    {
        charRect.x = _x*fontWidth+xoffset;
        charRect.y = _y*fontHeight+yoffset;
        SDL_FillRect(textSurf, &charRect,0xFF000000);

        if(++_x >= ncols)
        {
            ++_y;
            if(_y >= nlines)
                break;
            _x=0;
        }
    }
    render();
}

void Win110ct::addChar(char c)
{
        if(c == 10 || c == 13)
        {
            ypos++;
            xpos=0;
        }
        else if(c == '\t')
        {
            xpos = (xpos+tabWidth) - xpos%tabWidth;
            if(xpos >= ncols)
            {
                xpos=0;
                ++ypos;
            }
        }

        else if(xpos >= 0 && ypos >= 0 && xpos<ncols && ypos < nlines)
        {
            scrBuff[ypos][xpos] = c;
            scrBuff[ypos][xpos].setFore(textColour);
            scrBuff[ypos][xpos].setBack(backColour);
            bufStr.push_back(c);
        }
}

void Win110ct::render()
{
    SDL_FillRect(screen, &winRect, 0xFF000000);

    applySurface( 0, 0, backg , screen );
    applySurface( 0, 0, textSurf , screen );

    if(turtling)
    {
        SDL_Colour col = t->getColour();
        circleRGBA(screen, t->getx(), t->gety(), 8, col.r, col.g, col.b, 208);
        double x1 = t->getx() + sin(t->getDirection())*12;
        double y1 = t->gety() + cos(t->getDirection())*12;
        lineRGBA(screen, t->getx(), t->gety(), x1, y1, col.r, col.g, col.b, 208);
    }

    SDL_Flip(screen);
}

void Win110ct::renderRect(int x, int y, int w, int h)
{
    SDL_Rect r = {x,y,w,h};
    SDL_FillRect(screen, &r, 0xFF000000);
    applySurface( 0, 0, backg , screen, &r );

    applySurface( 0, 0, textSurf , screen, &r );

    SDL_Flip(screen);
}

Turtle * Win110ct::getTurtle()
{
    turtling = true;
    t = new Turtle(this);
    return t;
}

Win110ct& Win110ct::operator >> (char& c)
{
    c = getchar();
    return *this;
}

Win110ct& Win110ct::operator >> (std::string& s)
{
    startx = xpos;
    starty = ypos;
    bufStr.clear();
    char c;
    do
        c = getchar();
    while(c!=13 && c!=10);
    s = bufStr;
    return *this;
}

Win110ct& Win110ct::operator >> (int& n)
{
    std::string str;
    (*this) >> str;
    n = atoi(str.c_str());
    return *this;
}

Win110ct& Win110ct::operator >> (double& n)
{
    std::string str;
    (*this) >> str;
    n = atof(str.c_str());
    return *this;
}

Win110ct& Win110ct::operator << (std::string& out)
{
    for(std::size_t i=0; i<out.size(); ++i)
        putchar(out[i]);
    render();
    return *this;
}



Win110ct& Win110ct::operator << (char c)
{
    putchar(c);
    render();
    return *this;
}

Win110ct& Win110ct::operator << (char c[])
{
    for(int i=0; c[i]!='\0'; ++i)
        putchar(c[i]);
    render();
    return *this;
}

Win110ct& Win110ct::operator << (int n)
{
    std::string str;
    std::ostringstream sstream;
    sstream << n;
    str = sstream.str();
    (*this) << str;
    return *this;
}

Win110ct& Win110ct::operator << (double d)
{
    std::string str;
    std::ostringstream sstream;
    sstream << d;
    str = sstream.str();
    (*this) << str;
    return *this;
}

void Win110ct::clear()
{
    SDL_FillRect(textSurf, &winRect,0x00000000);
    render();
}

void Win110ct::clearBackRect(int x, int y, int width, int height)
{
    SDL_Rect r = {x,y,width,height};
    SDL_FillRect(backg, &r, 0x00000000);
    render();
}

void Win110ct::clearBack()
{
    clearBackRect(0,0,winRect.w,winRect.h);
}

void Win110ct::clearChar(int xoffset,int yoffset)
{
    clearChars(1,xoffset, yoffset);
}

Turtle::Turtle(Win110ct * host):direction(0),drawing(false)
{
    x = host->winRect.w/2;
    y = host->winRect.h/2;
    cl.r = cl.g = cl.b = 255;
    win = host;
    pauseSize = 0;
}

void Turtle::moveForward(double d)
{
    static Uint32 t = SDL_GetTicks();

    double x1 = x + d*sin(direction);
    double y1 = y + d*cos(direction);

    if(drawing)
        lineRGBA(win->backg, x, y, x1, y1, cl.r, cl.g, cl.b, 208);

    x = x1;
    y = y1;
    if(SDL_GetTicks() > t + 1000/60)// limit redraws to 60Hz (standard monitor refresh rate)
    {
        win->render();
        t = SDL_GetTicks();
    }
    SDL_Delay(pauseSize);
}

void Turtle::turn(double degrees)
{
    direction += degrees*M_PI/180;
    int periods = floor(direction/(M_PI*2));
    direction -= M_PI*2*periods;
}

Uint32 Stopwatch::stop()
{
    if(running)
    {
        elapsed += SDL_GetTicks() - startTime;
        running = false;
    }
    return elapsed;
}

Uint32 Stopwatch::read()
{
    if(running)
    {
        Uint32 now = SDL_GetTicks();
        elapsed += now - startTime;
        startTime = now;
    }
    return elapsed;
}
