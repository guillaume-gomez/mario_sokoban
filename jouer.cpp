#include <iostream>
#include <SDL/SDL.h>
#include "GerenatePicture.h"

Jouer::Jouer():Case(0),xMario(0),yMario(0),NextMario(0),NextBlock(0),EmptyBox(0),level(0),nbCase(155)
{
    Case = new Image [nbCase]; //+1 pour mettre la case white en dessous de mario
}

Jouer::~Jouer()
{
   delete ifile;
   delete [] Case ;
}

void Jouer::BlitterEcran(const Window& screen)
{
    for(int i=0;i<nbCase;i++)
        {
        Case[i].BlitSurfaces(  screen,Case[i]);
        }
}

SDL_Surface* Jouer::GetCase(int i) const
{
  return Case[i].getBackground();

}

void Jouer::ChangerFond(int i,std::string filename)
{
    Case[i].changeFond(filename);
}

void Jouer::ReadFile() // change to setNiveau  and it is a little big
{
    int x,y,i,j;
    char c;
    x=y=0;//init on cascade
    i=1;// because 0 modulo 12 = 0
    j=nbCase-10;
    ifile.open("niveaux.txt");
    if(ifile==NULL)
    {
        cerr<<"Erreur de lecture du fichier,"<<endl;
        cerr << "Avez vous supprimer le fichier niveau.txt?"<<endl;
        cerr <<"Ce programme devient infonctionnelle sans celui ci"<<endl;
        exit(1);
    }

if(ifile)
{
   ifile.seekg (level*146, ios::beg);
 while(ifile.get(c) && i<=nbCase-10) // i would prefer stop to read the file when it find the first \n
 {

        switch(c)   //  define the picture
        {          //NB it could be better if a charged only picture of each link of bmp.And after use a copy constructor
        case '0':
            EmptyBox=i-1;
            Case[i-1].changeFond("white");  //c'est juste pour le voir sur la console ,il n'y a pas d'image pareil
            Case[i-1].SetX(800); //pour etre hors de  l'ecran
            Case[i-1].SetY(800);
                        goto escape;  //permet de sauter l'affectation des coordonnées  normale
        break;
        case '1':
        Case[i-1].changeFond("mur.bmp");
        break;
        case '2':
        Case[i-1].changeFond("caisse.bmp");
        break;
        case '3':
        Case[i-1].changeFond("objectif.png");
        break;
        case '4':
        Case[i-1].changeFond("mario_bas.bmp");
        Mario=i-1; //permet de situer mario rien qu'avec son indice
        xMario=x;
        yMario=y;
        break;
        case '5':
        Case[i-1].changeFond("objectif.png");
        Case[j].changeFond("caisse_ok.bmp");
        Case[j].SetX(x);
        Case[j].SetY(y);
        cout<<"indice   "<<i-1<<"   indice"<<j<<endl;
        j++;
        break;
        }

            Case[i-1].SetX(x); // definie les coordonnés pour l'instance en cours de transformation
            Case[i-1].SetY(y);

  escape:
  //label pour sauter les lignes plus haut quand la suface est vide
            if(i%12 == 0)// permet de mettre les bonnes coordonnés
            {
                y+=34;
                x=0;
            }
            else
            {
                x+=34;
            }
     i++;
    }
}
ifile.close();
}

void Jouer::Input( Window& g)  //maybe... Sure too long
{
         SDL_Event event;


      if(SDL_WaitEvent(&event))
      {
          if( event.type == SDL_QUIT )
           {
              g.Quit();
           }
           else if ( event.type == SDL_KEYDOWN )
           {
              switch( event.key.keysym.sym )
              {
                   case SDLK_UP:
                        FindAround( 'h');
                        MoveMario('h');
                        GoalReach();
                   break;

                   case SDLK_DOWN:
                        FindAround( 'b');
                        MoveMario('b');
                        GoalReach();
                   break;

                    case SDLK_LEFT:
                        FindAround( 'g');
                        MoveMario('g');
                        GoalReach();
                   break;

                   case SDLK_RIGHT:
                        FindAround( 'd');
                        MoveMario('d');
                        GoalReach();
                   break;

                   case SDLK_r:
                        ViderSurface();
                        ReadFile(); // a changer il faut pointer sur l'instance en cours
                        ChangeIndice();
                   break;
                   default:
                   break;

              }
        }
    }
}


void Jouer::ChangeIndice()
{
    int xtemp,ytemp;
    string tmp,tmp2;
    Case[nbCase-1].changeFond("mario_bas.bmp");  //on lui applique les caracté. de Mario a la derniere case cree
    Case[nbCase-1].SetX(xMario);
    Case[nbCase-1].SetY(yMario);
    Case[Mario].changeFond(""); // et on applique les caracté de la derniere instance la ou se trouver Mario
    Mario=nbCase-1;  //permet de pouvoir deplacer Mario et non une caisse,un mur ou autre
    int n=1;

    for(int i=0;i<(nbCase-2-n);i++)
    {

        if(Case[i].GetName() == "caisse.bmp" || Case[i].GetName() == "caisse_ok.bmp" )
        {
            while(Case[nbCase-2-n].GetName()== "caisse.bmp" || Case[nbCase-2-n].GetName()=="caisse_ok.bmp")
                {
                    n++;
                }

            xtemp=Case[i].GetX();                           //en gros on inverse les 2 surfaces pour que les caisses au meme titre que mario puissent passer sur tout le monde
            ytemp=Case[i].GetY();
            tmp2=Case[i].GetName();
            tmp=Case[nbCase-2-n].GetName();
            Case[i].changeFond(tmp);
            Case[i].SetX(Case[nbCase-2-n].GetX());
            Case[i].SetY(Case[nbCase-2-n].GetY());
            cout<<tmp2<<endl<<tmp<<endl;

            Case[nbCase-2-n].changeFond(tmp2);
            Case[nbCase-2-n].SetX(xtemp);
            Case[nbCase-2-n].SetY(ytemp);
            n++;
            cout<<i<<endl;
            cout<<tmp2<<nbCase-2-n<<endl;
        }

    }

}

void Jouer::FindAround(char direction)
{
    NextMario=EmptyBox;
    NextBlock=EmptyBox;

    for(int i=0;i<nbCase-1;i++)
    {
        if(direction == 'h')  // h haut
        {
            if( (Case[i].GetX() == Case[Mario].GetX()) && (Case[i].GetY()+34 == Case[Mario].GetY()) ) //cherche la case au dessus de mario
            {  NextMario=i;

            }
            if((Case[i].GetX() == Case[Mario].GetX()) && (Case[i].GetY()+(2*34) == Case[Mario].GetY())) //cherche la case 2 fois au dessus de mario
            {
                NextBlock=i;

            }
        }

        if(direction =='g') // gauche
        {
            if( (Case[i].GetX()+34 == Case[Mario].GetX()) && (Case[i].GetY() == Case[Mario].GetY()) ) //idem que plus haut mais pour gauche
            {NextMario=i;

            }
            if((Case[i].GetX()+(2*34) == Case[Mario].GetX()) && (Case[i].GetY() == Case[Mario].GetY()) ) //idem
            {
                NextBlock=i;

            }
        }

        if(direction == 'd') //droite
        {
            if( (Case[i].GetX() -34 == Case[Mario].GetX()) && (Case[i].GetY() == Case[Mario].GetY()) ) //idem
            { NextMario=i;


            }
            if( (Case[i].GetX() - (34*2) == Case[Mario].GetX()) && (Case[i].GetY() == Case[Mario].GetY()) ) //idem
            {
            NextBlock=i;

            }
        }

        if(direction == 'b') //bas
        {

          if( (Case[i].GetX()== Case[Mario].GetX()) && (Case[i].GetY()- 34 == Case[Mario].GetY()) ) //idem
            {NextMario=i;

             }
             if((Case[i].GetX()== Case[Mario].GetX()) && (Case[i].GetY()- 68 == Case[Mario].GetY())) //idem
             {
                 NextBlock=i;

             }
        }
    }

}

bool Jouer::TestWin()  //teste si nous avons gagner;le probleme de cette fonction,c'est sa rapidité algorithmique
{
    int cptGoal=0,cptRedBox=0;

    for(int i=0;i<nbCase-1;i++)
    {
        if(Case[i].GetName()  == "objectif.png")
        {
            cptGoal++;
        }
        else if(Case[i].GetName() == "caisse_ok.bmp")
        {
            cptRedBox++;
        }
    }
    return cptRedBox == cptGoal ;

}

void Jouer::GoalReach()  //change la surface de la case bien placée
{

             if(Case[NextMario].GetX() == Case[NextBlock].GetX()  && Case[NextMario].GetY() == Case[NextBlock].GetY() && Case[NextMario].GetName()=="caisse.bmp" && Case[NextBlock].GetName()=="objectif.png")
            {
                Case[NextMario].changeFond("caisse_ok.bmp");
                cout<<"je suis rouge"<<endl;

            }
           else if(Case[NextMario].GetName()== "caisse_ok.bmp" && Case[NextMario].GetX() != Case[NextBlock].GetX()  && Case[NextMario].GetY() != Case[NextBlock].GetY())
            {
                 Case[NextMario].changeFond("caisse.bmp");
                 cout<<"et voila je suis plus rouge"<<endl;

            }
}

void Jouer::MoveMario(char direction) //trop long trop long...
{
    int x,y;
    switch(direction)
    {
        case 'h':
        cout<<"NextBlock  "<<Case[NextBlock].GetName()<<endl;
        cout<<"MarioNext  "<<Case[NextMario].GetName()<<endl;
         if(Case[Mario].GetY() > 0)
                   {

                        if(Case[NextMario].GetName() != "mur.bmp") //teste si il a y un mur
                        {


                            if(Case[NextBlock].GetName()!= "caisse_ok.bmp" && Case[NextBlock].GetName()!="caisse.bmp" &&  Case[NextBlock].GetName()!= "mur.bmp") //teste si il y un quelque chose a 2 case de Mario
                            {


                                    if(Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!= "caisse_ok.bmp") //teste on pourra de deplacer car si on a une caisse on pourra la deplacer
                                    {
                                        Case[Mario].SetY(yMario-=34);
                                        Case[Mario].changeFond("mario_haut.bmp") ;

                                            if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //deplacement ou non des caisses
                                            {
                                                y=Case[NextMario].GetY();
                                                Case[NextMario].SetY(y-=34);
                                            }
                                    }
                                    else if(Case[Mario].GetY()-34 > 0) //verifie si  la caisse n'est pas au bord de la fenetre
                                    {
                                        Case[Mario].SetY(yMario-=34);
                                        Case[Mario].changeFond("mario_haut.bmp") ;

                                        if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //deplacement ou non des caisses
                                            {
                                                y=Case[NextMario].GetY();
                                                Case[NextMario].SetY(y-=34);
                                            }
                                    }
                            }
                            else
                                    {

                                        if (Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!=  "caisse_ok.bmp" && Case[NextMario].GetName() != "mur.bmp"
                                            && Case[Mario].GetY() -34 >0)
                                        {


                                                Case[Mario].SetY(yMario-=34);
                                                Case[Mario].changeFond("mario_haut.bmp") ;

                                                if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //deplacement ou non des caisses
                                            {
                                                y=Case[NextMario].GetY();
                                                Case[NextMario].SetY(y-=34);
                                            }


                                        }
                                    }
                            }
                        }

    break;
    case 'b':
 cout<<"NextBlock  "<<Case[NextBlock].GetName()<<endl;
 cout<<"MarioNext  "<<Case[NextMario].GetName()<<endl;
         if(Case[Mario].GetY() < (12*34)-34)
                   {
            if(Case[NextMario].GetName() != "mur.bmp")
                        {


                            if(Case[NextBlock].GetName()!= "caisse_ok.bmp" && Case[NextBlock].GetName()!="caisse.bmp" &&  Case[NextBlock].GetName()!= "mur.bmp")
                            {


                                    if(Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!= "caisse_ok.bmp") //teste on pourra de deplacer car si on a une caisse on pourra la deplacer
                                    {
                                        Case[Mario].SetY(yMario+=34);
                                        Case[Mario].changeFond("mario_bas.bmp") ;
                                        if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //test la deplacement des caisses
                                        {
                                        y=Case[NextMario].GetY();
                                        Case[NextMario].SetY(y+=34);
                                        }

                                    }
                                    else if(Case[Mario].GetY()+34  <(12*34)-34) //verifie si  la caisse n'est pas au bord de la fenetre
                                    {
                                        Case[Mario].SetY(yMario+=34);
                                        Case[Mario].changeFond("mario_bas.bmp") ;

                                        if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //test la deplacement des caisses
                                        {
                                        y=Case[NextMario].GetY();
                                        Case[NextMario].SetY(y+=34);
                                        }

                                    }
                            }
                            else
                                    {

                                        if (Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!=  "caisse_ok.bmp" && Case[NextMario].GetName() != "mur.bmp"
                                            && Case[Mario].GetY() +34 <(12*34)-34)
                                        {
                                                Case[Mario].SetY(yMario+=34);
                                                Case[Mario].changeFond("mario_bas.bmp") ;

                                                if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //test la deplacement des caisses
                                                {
                                                y=Case[NextMario].GetY();
                                                Case[NextMario].SetY(y+=34);
                                                }


                                        }
                                    }
                            }

                    }

    break;
        case 'g':
         cout<<"NextBlock  "<<Case[NextBlock].GetName()<<endl;
        cout<<"MarioNext  "<<Case[NextMario].GetName()<<endl;
        if(Case[Mario].GetX() > 0)
        {
        if(Case[NextMario].GetName() != "mur.bmp")
                        {


                            if(Case[NextBlock].GetName()!= "caisse_ok.bmp" && Case[NextBlock].GetName()!="caisse.bmp" &&  Case[NextBlock].GetName()!= "mur.bmp")
                            {


                                    if(Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!= "caisse_ok.bmp") //teste si on pourra se deplacer car si on a une caisse on pourra la deplacer
                                    {
                                        Case[Mario].SetX(xMario-=34);
                                        Case[Mario].changeFond("mario_gauche.bmp") ;

                                         if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //test sur les caisses
                                        {
                                            x=Case[NextMario].GetX();
                                            Case[NextMario].SetX(x-=34);
                                       }

                                    }
                                    else if(Case[Mario].GetX()-34 > 0) //verifie si  la caisse n'est pas au bord de la fenetre
                                    {
                                        Case[Mario].SetX(xMario-=34);
                                        Case[Mario].changeFond("mario_gauche.bmp") ;

                                         if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //test sur les caisses
                                        {
                                            x=Case[NextMario].GetX();
                                            Case[NextMario].SetX(x-=34);
                                       }

                                    }
                            }
                            else
                                    {

                                        if (Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!=  "caisse_ok.bmp" && Case[NextMario].GetName() != "mur.bmp"
                                            && Case[Mario].GetX() -34 >0)
                                        {


                                                Case[Mario].SetX(xMario-=34);
                                                Case[Mario].changeFond("mario_gauche.bmp") ;

                                                 if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp") //test sur les caisses
                                                {
                                                    x=Case[NextMario].GetX();
                                                    Case[NextMario].SetX(x-=34);
                                                    }



                                        }
                                    }
                            }

        }
        break;

        case 'd':
         cout<<"NextBlock  "<<Case[NextBlock].GetName()<<endl;
        cout<<"MarioNext  "<<Case[NextMario].GetName()<<endl;
         if(Case[Mario].GetX() < (12 *34) -34)
                    {
                        if(Case[NextMario].GetName() != "mur.bmp")
                        {


                            if(Case[NextBlock].GetName()!= "caisse_ok.bmp" && Case[NextBlock].GetName()!="caisse.bmp" &&  Case[NextBlock].GetName()!= "mur.bmp")
                            {


                                    if(Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!= "caisse_ok.bmp") //teste on pourra de deplacer car si on a une caisse on pourra la deplacer
                                    {
                                        Case[Mario].SetX(xMario+=34);
                                        Case[Mario].changeFond("mario_droite.bmp") ;

                                                if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp")
                                                {
                                                x=Case[NextMario].GetX();
                                                Case[NextMario].SetX(x+=34);
                                                }

                                    }
                                    else if(Case[Mario].GetX()+34 < (12 *34) -34) //verifie si  la caisse n'est pas au bord de la fenetre
                                    {
                                        Case[Mario].SetX(xMario+=34);
                                        Case[Mario].changeFond("mario_droite.bmp") ;

                                         if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp")
                                                {
                                                x=Case[NextMario].GetX();
                                                Case[NextMario].SetX(x+=34);
                                                }
                                    }

                            }
                            else
                                    {


                                        if (Case[NextMario].GetName() != "caisse.bmp" && Case[NextMario].GetName()!=  "caisse_ok.bmp" && Case[NextMario].GetName() != "mur.bmp"
                                            && Case[Mario].GetX() +34 <(12*34)-34)
                                        {


                                                Case[Mario].SetX(xMario+=34);
                                                Case[Mario].changeFond("mario_droite.bmp") ;

                                                 if(Case[NextMario].GetName() == "caisse.bmp" || Case[NextMario].GetName()== "caisse_ok.bmp")
                                                {
                                                x=Case[NextMario].GetX();
                                                Case[NextMario].SetX(x+=34);
                                                }


                                        }
                                    }
                            }
                    }




    }

}
void Jouer::ChangerNiveau(const Window& g)// a revoir
{
    if(TestWin())
    {
        level++;
        Image Bravo("bravo.bmp");
        Bravo.BlitSurfaces(g,Bravo);

    }
}

void Jouer::ViderSurface()
{
     for(int i=nbCase-10;i<nbCase;i++) //on vide les 10 dernieres surface qui pourrait contenir quelques choses
                {
                    Case[i].changeFond("");
                    Case[i].SetX(800);
                    Case[i].SetY(800);//pour etre sur qu'ils soient en dehors de l'ecran
                }
}
