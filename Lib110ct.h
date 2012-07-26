#ifndef LIB110CT_H_INCLUDED
#define LIB110CT_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include <time.h>

class Turtle;

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
    SDL_Color getBack(){return bg;}
    void operator = (char ch) {c[0]=ch;}
    char get(){return c[0];}
    SDL_Surface * getSurface();
    ~FormattedChar(){SDL_FreeSurface(Surf);}
};

/**
*   The console window class. Combines console behaviour with turtle graphics and enables rendering
*   to an SDL background surface.
*/
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
    bool echo, waiting, got, turtling;
    void applySurface( int x, int y, SDL_Surface* src, SDL_Surface* dest, SDL_Rect* clip = NULL );
    void putchar(char);
    void bufferChar();
    void addChar(char c);
    void moveCursor();
    std::string bufStr;

    char last;
    int xpos, ypos, startx, starty;

    friend class Turtle;
    Turtle * t;

 public:

    /**
    *   Builds a new console window. Width and height should represent an appropriate video mode if specified
    *   i.e. 1024 x 768, 800 x 600, 640 x 480 etc
    *
    *   @param  width       The width of the console in pixels, default 1024
    *   @param  height      The height of the console in pixels, default 768
    *   @param  fontPitch   The pitch of the console font, 16
    */
    Win110ct(int width=1024, int height=768, int fontPitch=16);

    /**
    *   Builds a new console window which uses the specified fontFile for text output
    *   (must be a truetype ".ttf" font, preferably monospaced).
    *   Width and height should represent an appropriate video mode if specified
    *   i.e. 1024 x 768, 800 x 600, 640 x 480 etc
    *
    *   @param  fontFile    The path to the fontfile, including filename and extension
    *   @param  width       The width of the console in pixels, default 1024
    *   @param  height      The height of the console in pixels, default 768
    *   @param  fontPitch   The pitch of the console font, default 16
    */
    Win110ct(std::string fontFile, int width=1024, int height=768, int fontPitch=16);

    /**
    *   Overloaded input methods for ints, doubles, chars and strings.
    *   Input is always unbuffered for chars and always buffered for the others
    *   @param  input   The variable to contain the input
    */
    Win110ct& operator >>(char& input);
    Win110ct& operator >> (std::string& input);
    Win110ct& operator >>(int& input);
    Win110ct& operator >> (double& input);

    /**
    *   Overloaded output methods for ints, doubles, chars, strings and string literals
    *   Input is always unbuffered for chars and always buffered for the others
    *   @param  output   The variable to be output
    */
    Win110ct& operator <<(char output);
    Win110ct& operator << (char output[]);
    Win110ct& operator << (std::string& output);
    Win110ct& operator << (int output);
    Win110ct& operator << (double output);

    /**
    *   Reads a single character from the keyboard.
    *   Main usage in waiting for user keypress since return variable may be ignored
    *   @return The first key pressed by the user
    */
    char getchar();

    /**
    *   Calls to this will query whether the user has pressed a key yet
    *   This enables the calling program to continue with other tasks while waiting for input
    *   @return TRUE if the user has pressed a key, FALSE otherwise
    */
    bool checkInput();

    /**
    *   Sets the text colour for the console
    *   @param  The text colour
    */
    void setTextColour(SDL_Color c){textColour = c;}

    /**
    *   Sets the background colour for the console text
    *   @param  The background colour
    */
    void setBackColour(SDL_Color c){backColour = c;};

    /**
    *   Sets the cursor position
    *   @param  x   the horizontal (x) position (in character spaces)
    *   @param  y   the vertical (y) position (in character spaces)
    */
    void setPosition(int x, int y){xpos = x % ncols; ypos = y % ncols;};

    /**
    *   Controls whether or not keystrokes are displayed in the console window while typing
    *   @param  b   True to switch echo on, false otherwise
    */
    void setEcho(bool b){echo = b;}

    /**
    *   Obtain the current position of the cursor (in character spaces)
    *   @param  x   stores the horizontal (x) position
    *   @param  y   stores the vertical (y) position
    */
    void getPosition(int& x, int& y){x = xpos; y = ypos;}

    /**
    *   Gets the current x position of the cursor (in character spaces)
    *   @return the x position
    */
    int getx(){return xpos;}

    /**
    *   Gets the current y position of the cursor (in character spaces)
    *   @return the y position
    */
    int gety(){return ypos;}

    /**
    *   Returns the console background as an SDL_Surface. Enables fun with graphics!
    *   @return A pointer to the background surface
    */
    SDL_Surface * getGraphics(){return backg;}

    /**
    *   Returns a pointer to an MSWLogo stye turtle which draws on the window's background
    *   @return pointer to the turtle
    */
    Turtle * getTurtle();

    /**
    *   Hides the turtle from view
    */
    void hideTurtle(){turtling = false;}

    /**
    *   Shows the turtle
    */
    void showTurtle(){turtling = true;}

    /**
    *   Redraws the console window (background and text)
    */
    void render();

    /**
    *   Redraws the specified rectangular portion of the console window (coordinates in pixels).
    *   Does not redraw turtle!
    *   @param  x   the x coordinate of the top left hand corner of the rectangle
    *   @param  y   the y coordinate of the top left hand corner of the rectangle
    *   @param  w   the width of the rectangle
    *   @param  h   the height of the rectangle
    */
    void renderRect(int x, int y, int w, int h);

    /**
    *   Clears all text from the console, without affecting the background
    */
    void clear();

    /**
    *   Clears the character space at the current cursor position.
    *   Offsets can be used for correction if it misses a bit!
    *   @param  xoffset adjusts the x position of the clearing rectangle (in pixels)
    *   @param  yoffset adjusts the y position of the clearing rectangle (in pixels)
    */
    void clearChar(int xoffset = 0,int yoffset = 0);

    /**
    *   Clears several character spaces, beginning at the current cursor position.
    *   Offsets can be used for correction if it misses a bit!
    *   @param  nChars  the number of character spaces to be cleared
    *   @param  xoffset adjusts the x position of the clearing rectangle (in pixels)
    *   @param  yoffset adjusts the y position of the clearing rectangle (in pixels)
    */
    void clearChars(int nChars,int xoffset = 0,int yoffset = 0);

    /**
    *   Clears the background image from the console, without affecting the text
    */
    void clearBack();

    /**
    *   Clears a rectangular portion of the background image from the console, without affecting the text.
    *   Coordinates are in pixels
    *   @param  x   the x coordinate of the top left hand corner of the rectangle
    *   @param  y   the y coordinate of the top left hand corner of the rectangle
    *   @param  w   the width of the rectangle
    *   @param  h   the height of the rectangle
    */
    void clearBackRect(int x, int y, int width, int height);

    virtual ~Win110ct();
} ;

/**
*   An MSW logo style 'Turtle' class for the Win110ct console
*/
class Turtle
{
    Win110ct * win;
    double x,y,penWidth;
    double direction;
    SDL_Color cl;
    bool drawing;
    int pauseSize;

public:

    /**
    *   Constructs a turtle (called automatically when using getTurtle() from Win110ct)
    *   @param  host    the host window for the turtle
    */
    Turtle(Win110ct * host);

    /**
    *   Moves the turtle forward
    *   @param  x    the distance to move
    */
    void moveForward(double x);

    /**
    *   Rotates the turtle
    *   @param  degrees the number of degrees to turn, positive for left, negative for right
    */
    void turn(double degrees);

    /**
    *   Sets the colour of the line drawn by the turtle
    *   @param  c   the line colour
    */
    void setColour(SDL_Color c){cl = c;}

    /**
    *   Sets the position of the turtle (pixel coordinates)
    *   @param  x   the horizontal (x) position
    *   @param  y   the vertical (y) position
    */
    void setPosition(int xpos, int ypos){x = xpos; y = ypos;}

    /**
    *   Sets the speed of the turtle (0-100) default value 100
    *   @param  speed   the speed of the turtle (100 is as fast as possible)
    */
    void setSpeed(int speed){pauseSize = 100-speed;}

    /**
    *   Starts the turtle drawing when it moves
    */
    void penDown(){drawing = true;}

    /**
    *   Stops the turtle drawing when it moves
    */
    void penUp(){drawing = false;}

    /**
    *   Returns the current direction of the turtle in radians
    *   @return the current direction
    */
    double getDirection(){return direction;}

    /**
    *   Fetches the current x and y position of the turtle
    *   @param  xpos    the horizontal (x) position
    *   @param  ypos    the vertical (y) position
    */
    void getPosition(double& xpos, double& ypos){xpos = x; ypos = y;}

    /**
    *   Returns the current colour of the turtle
    *   @return the turtle's color
    */
    SDL_Color getColour(){return cl;}

    /**
    *   Returns the current x position of the turtle in pixels
    *   @return the current x position
    */
    double getx(){return x;}

    /**
    *   Returns the current y position of the turtle in pixels
    *   @return the current y position
    */
    double gety(){return y;}
};

class Stopwatch
{
    Uint32 startTime, elapsed;
    bool running;
    public:
    /**
    *   Creates a stopwatch
    */
    Stopwatch():startTime(0), elapsed(0), running(false){}

    /**
    *   Starts the watch (adds to current time if watch has not been reset
    */
    void start(){if(running) return; startTime = SDL_GetTicks(); running = true;}

    /**
    *   Stops the watch (does not reset the time)
    *   @return the time on the watch
    */
    Uint32 stop();

    /**
    *   Reads the watch (does not stop)
    *   @return the current time on the watch
    */
    Uint32 read();

    /**
    *   Resets time to zero
    */
    void reset(){startTime = SDL_GetTicks(); elapsed = 0;}
};

#endif // LIB110CT_H_INCLUDED
