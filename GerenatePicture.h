#ifndef GERENATEPICTURE_H_INCLUDED
#define GERENATEPICTURE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class Window
{
  private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int SCREEN_BPP;
    SDL_Surface *screen;
    bool running;
    int green,red,blue;
  public:
    friend class Image;
    Window(int _SCREEN_WIDTH,int _SCREEN_HEIGHT);
    Window();
    void Load();
    void Init();
    void Draw();
    void Quit();
    void Update();
    bool runningState();
    ~Window();
    SDL_Surface* getScreen() const;
    void SetColor(int _red,int _green,int _blue);
    void  FillRect();
    void HeaderTitle(std::string filename);
};

class Image
{
  private:
    SDL_Surface *background;
    SDL_Rect positionFond;
    string name; //the name of the picture
  public:
    Image();
    Image(std::string filename);
    ~Image();
    SDL_Surface* getBackground() const;
    int   GetX()const;
    int   GetY()const;
    void SetY(int _Y);
    void SetX(int _X);
    void BlitSurfaces(const Window& g,  Image& i);// a revoir
    void changeFond(std::string filename);
    string GetName()const;
};


class Jouer
{
  private :
    int nbCase;
    Image *Case;
    int level;//permet de lancer le niveau
    int Mario; //l'indice de Mario
    int xMario,yMario;  //define the coord of Mario in the beginning of the level
    int NextMario,NextBlock,EmptyBox; // les variables qui vont jouer le role de yeux de mario puisque ces variables contiendra les case autour de mario
    std::ifstream ifile;
  public:
    Jouer();
    ~Jouer();
    SDL_Surface* GetCase(int i) const;
    void ChangerFond(int i,std::string filename);
    void  BlitterEcran(const Window& screen);
    void ReadFile();
    void Input( Window& g);
    void FindAround(char dir);
    bool TestWin();
    void GoalReach();
    void MoveMario(char direction);
    void ChangeIndice();
    void ChangerNiveau(const Window& g);
    void ViderSurface();
};
#endif // GERENATEPICTURE_H_INCLUDED

