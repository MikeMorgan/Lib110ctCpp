#ifndef LIB110CT_H_INCLUDED
#define LIB110CT_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include <time.h>

class FormattedChar
{
    SDL_Color cl,bg;
    SDL_Surface * Surf;
    TTF_Font * font;
    int width, height;
    char c[2];
public:
    FormattedChar();
    FormattedChar(TTF_Font * ft);
    void setFont(TTF_Font * ft);
    void setFore(SDL_Color col){cl=col;}
    void setBack(SDL_Color col){bg=col;}
    void operator = (char ch) {c[0]=ch;}
    char get(){return c[0];}
    SDL_Surface * getSurface();
    ~FormattedChar(){SDL_FreeSurface(Surf);}
};


class Win110ct
{
private:
    static const int tabWidth = 8;
    int nlines;
    int ncols;
    int fontHeight;
    int fontWidth;
    SDL_Surface * screen;
    SDL_Surface * backg;
    SDL_Surface * textSurf;
    SDL_Rect winRect;
    SDL_Event event;
    TTF_Font * font;
    SDL_Color textColour, backColour;
    FormattedChar **scrBuff;
    bool echo, waiting, got;
    void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
    void putchar(char);
    void bufferChar();
    void addChar(char c);
    void moveCursor();
    std::string bufStr;

    char last;
    int xpos, ypos, startx, starty;

 public:

    Win110ct(int width=1024, int height=768, int fontPitch=14);
    Win110ct& operator >>(char& input);
    Win110ct& operator >> (std::string& input);
    Win110ct& operator >>(int& input);
    Win110ct& operator >> (double& input);
    Win110ct& operator <<(char output);
    Win110ct& operator << (char c[]);
    Win110ct& operator << (std::string& output);
    Win110ct& operator << (int n);
    Win110ct& operator << (double d);

    friend class Turtle;

    char getchar();
    bool checkInput();
    void setTextColour(SDL_Color c){textColour = c;}
    void setBackColour(SDL_Color c){backColour = c;};
    void setPosition(int x, int y){xpos = x % ncols; ypos = y % ncols;};
    void setEcho(bool b){echo = b;}
    void getPosition(int& x, int& y){x = xpos; y = ypos;}
    int getx(){return xpos;}
    int gety(){return ypos;}
    SDL_Surface * getGraphics(){return backg;}
    void render();
    void renderRect(int x, int y, int w, int h);
    void clear();
    void clearChars(int nChars,int xoffset = 0,int yoffset = 0);
    void clearChar(int xoffset = 0,int yoffset = 0);
    void clearBackRect(int x, int y, int width, int height);
    void clearBack();
    ~Win110ct();
} ;

class Turtle
{
    Win110ct * win;
    double x,y,penWidth;
    double direction;
    SDL_Color cl;
    bool drawing;
    //bool drawWhileMoving;
public:
    Turtle(Win110ct * host);
    void moveForward(double x);
    void turn(double degrees);
    void setColour(SDL_Color c){cl = c;}
    void setWidth(int w){penWidth = w;}
    void setPosition(int xpos, int ypos){x = xpos; y = ypos;}
    void penDown(){drawing = true;}
    void penUp(){drawing = false;}

};

class Stopwatch
{
    Uint32 startTime, elapsed;
    bool running;
    public:
    Stopwatch():startTime(0), elapsed(0), running(false){}
    void start(){if(running) return; startTime = SDL_GetTicks(); running = true;}
    Uint32 stop();
    Uint32 read();
    void reset(){startTime = SDL_GetTicks(); elapsed = 0;}
};


#endif // LIB110CT_H_INCLUDED
