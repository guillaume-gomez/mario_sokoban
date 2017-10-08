#include <iostream>
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#include "GerenatePicture.h"

int main(int argc, char *argv[])
{

freopen("CON", "w", stdout);
freopen("CON", "r", stdin);
freopen("CON", "w", stderr);
    Jouer jeu;
    Window g(12*34,12*34);  //
    g.SetColor(255,255,255);
    g.HeaderTitle("Mario Sokoban by Gomez Guillaume");
// Activation de la répétition des touches
SDL_Event ev_menu;
        Image im_menu("menu.jpg");
        bool continuer=true;

 while (continuer)
{
    SDL_WaitEvent(&ev_menu);
    switch(ev_menu.type)
    {
        case SDL_QUIT:
            continuer = false;
            break;
        case SDL_KEYDOWN: /* Si appui d'une touche */
            continuer = false;
            break;
    }
    im_menu.BlitSurfaces(g,im_menu);
g.Update();
}


SDL_ShowCursor(SDL_DISABLE);
SDL_EnableKeyRepeat(100, 100);
  while( g.runningState()) //while the user don't closed the window
   {
    jeu.ReadFile();

    jeu.ChangeIndice();


         do
           {
            jeu.Input(g);
            g.FillRect();
            jeu.BlitterEcran(g);
            //Update
            g.Update();
           }  while( g.runningState()==true && jeu.TestWin()==false);
           jeu.ChangerNiveau(g);
            jeu.ViderSurface();  //permet d' "effacer" les surfaces qui contiendrait les caisses_ok
   }

    //destruction de l'instance window par le destructeur

    return 0;

}
