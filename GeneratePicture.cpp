#include <iostream>
#include <SDL/SDL.h>
#include "GerenatePicture.h"


Window ::Window():SCREEN_WIDTH(640),SCREEN_HEIGHT(480),SCREEN_BPP(32),running(true),blue(0),green(0),red(0)
{
    screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_HWSURFACE | SDL_DOUBLEBUF);
    printf("it is better if you don't use this constructor\n");
}

Window::Window(int _SCREEN_WIDTH,int _SCREEN_HEIGHT):
SCREEN_WIDTH(_SCREEN_WIDTH),SCREEN_HEIGHT (_SCREEN_HEIGHT),SCREEN_BPP(32),screen(0),running(true),blue(0),green(0),red(0)
{
screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_HWSURFACE | SDL_DOUBLEBUF);
}

void Window::Load()
{

}

void Window::Draw()
{

}
void Window::Update()
{
    SDL_Flip(screen);
}

bool Window::runningState()
{
return running;
}

void Window::Quit()
{
    running= false;
}

Window::~Window()
{
 SDL_Quit();
 exit(0);
}

SDL_Surface* Window::getScreen() const
{
    return screen;
}

void  Window::SetColor(int _red ,int _green,int _blue)
{
red=_red;
green=_green;
blue=_blue;


}
void Window::FillRect()
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, red, green, blue));
}

void Window::HeaderTitle(std::string filename)
{
   SDL_WM_SetCaption( filename.c_str(),NULL);
}
