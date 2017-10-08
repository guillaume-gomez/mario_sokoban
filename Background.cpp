#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "GerenatePicture.h"

Image::Image():background(0),name("\0")  // des doutes sur l'inialisation  de name
{
     positionFond.x=0;
    positionFond.y=0;
}

Image::Image(std::string filename):background(0),name(filename)
{
    positionFond.x=0;
    positionFond.y=0;
    background=IMG_Load( filename.c_str());
}
Image::~Image()
{
    SDL_FreeSurface(background);

}
SDL_Surface* Image::getBackground() const
{
    return background;
}

void Image::SetX(int _X)
{
    positionFond.x=_X;
}
void Image::SetY(int _Y)
{
     positionFond.y=_Y;
}

int Image::GetX() const
{
    return positionFond.x;
}

int Image::GetY() const
{
    return positionFond.y;
}

void Image::BlitSurfaces( const Window& g,Image& i)
{
    SDL_BlitSurface( i.background, NULL, g.screen,&i.positionFond);
}

void Image::changeFond(std::string filename)
{
    name=filename; //il a plus propre mais pour l'instant on cherche a que tout fonctionne ^^
   background=IMG_Load( filename.c_str());
}

string Image::GetName()const
{
    return name;
}

