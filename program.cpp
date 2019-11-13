#include <windows.h>
#include <windowsx.h>
#include<iostream>
#include <list>
#include <vector>
#include<stdlib.h>
#include <time.h>
#include<cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>
#define MAX_INT 65535
#define max(x,y) x>y?x:y
#define min(x,y) x<y?x:y
using namespace std;
int i=0,j=0;

//==========================================================================//

struct Etape{
	int id;
	int coordX;
    int coordY;
    int dptot;
    int dptard;
    int marge;
};
struct Tache{
    string name;
    int dure;
    int rang;
    list <Tache>antecedant;
    list <Tache> successeur;
    Etape etapeDebut;
    Etape etapeFin;
    bool painted;
    
};
list<Tache> Projet;
Etape lastStep;
vector<Tache> criticalPath ; // declaration d'une liste des taches qui va representer le chemin critique.
list<Tache> loadedTasks; // la liste où on va mettre les taches chargées à partir du fichier.



//===============================

HWND btn1, btn2,btn3,lab1,lab2;

/* This is where all the input to the window goes to */

HDC hDC;
HWND hwnd;
PAINTSTRUCT Ps;
HBITMAP hBitmap01 = NULL;
bool test = false,crash = false;
//===============================
void Create_Form(HWND);
void chooseFile(HWND);
void Onpaint(HDC);
void Onerase(HDC);
void showDetails(HWND);
//===============================
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
   HDC hdcStatic = (HDC)wParam;
   HDC hdcButton = (HDC)wParam;
	switch(Message) {

		case WM_CREATE: {
					Create_Form(hwnd);

			break;
		}
		case WM_CTLCOLORSTATIC:
		SetTextColor((HDC)wParam, RGB(0, 0, 0));
		SetBkColor((HDC)wParam, RGB(225, 125, 125));
		return (INT_PTR)CreateSolidBrush(RGB(0, 140,140));
		break;
        /*case WM_CTLCOLOREDIT:
		SetTextColor( hdcButton, RGB(255, 255, 255));
		SetBkColor( hdcButton, RGB(0, 0, 0));
		return (INT_PTR)CreateSolidBrush(RGB(109, 194, 222));
		break;*/
		case WM_COMMAND: {
			if(wParam < 100){
  	            
				    
			}
			//LOWORD(wParam)
			switch(wParam){

				    case 100:	{
				    
			        InvalidateRect(hwnd,NULL,false);
			        
					break;
					}
					case 101:	{

				     chooseFile(hwnd);

					break;
					}
					case 102:	{
				    test = true;
					showDetails(hwnd);
					if(crash == false){
					InvalidateRect(hwnd,NULL,false);
					}
					break;
					}
					case 103:	{

                    test = false;
				    break;  
					}
					
		}
	}


   case WM_PAINT: {


	hDC = BeginPaint(hwnd, &Ps);

	if(test == true){
	Onpaint(hDC);
	}
	else{
	Onerase(hDC);	
	}
	break;
}

		/* Upon destruction, tell the main thread to stop */
 	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
		}

		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
    }

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */
    //wc.hbrBackground = CreatePatternBrush( LoadBitmap( hInstance, MAKEINTRESOURCE( "BACK0.BMP" ) ) );

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","PERT",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		1370, /* width */
		800, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/

	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate key codes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}

	return Msg.wParam;
}

void Create_Form(HWND hwnd){
                    CreateWindowW(L"static",L"---Reseau PERT---",WS_VISIBLE|WS_CHILD,450,10,120,20,hwnd,(HMENU) 102,NULL,NULL);
                    CreateWindowW(L"static",L"---Contenu du Fichier---",WS_VISIBLE|WS_CHILD,1050,10,275,20,hwnd,(HMENU) 102,NULL,NULL);
                    CreateWindowW(L"static",L"> Nom de la Tache\n> Dure de la Tache\n> Liste des Antecedants\n> Liste des Successeurs ",WS_VISIBLE|WS_CHILD,1050,30,275,100,hwnd,(HMENU) 102,NULL,NULL);
					btn1 =CreateWindow(TEXT("button"),TEXT("Ouvrir..."),WS_VISIBLE|WS_CHILD,1050,150,275,50,hwnd,(HMENU) 101,NULL,NULL);
				    btn2 =CreateWindow(TEXT("button"),TEXT("Dessiner le graphe"),WS_VISIBLE|WS_CHILD,1050,250,135,50,hwnd,(HMENU) 102,NULL,NULL);
					 ::EnableWindow(btn2,false);
                    btn3 =CreateWindow(TEXT("button"),TEXT("Effacer le graphe"),WS_VISIBLE|WS_CHILD,1190,250,135,50,hwnd,(HMENU) 104,NULL,NULL);
					::EnableWindow(btn3,false);
                    CreateWindowW(L"static",L"> Dure Minimale :",WS_VISIBLE|WS_CHILD,1050,350,275,20,hwnd,(HMENU) 102,NULL,NULL);
					lab1=CreateWindowW(L"static",L"==>:",WS_VISIBLE|WS_CHILD,1050,370,275,100,hwnd,(HMENU) 102,NULL,NULL);
					CreateWindowW(L"static",L"> Chemins Critiques  :",WS_VISIBLE|WS_CHILD,1050,470,275,20,hwnd,(HMENU) 102,NULL,NULL);
					lab2=CreateWindowW(L"static",L"==> :",WS_VISIBLE|WS_CHILD,1050,490,275,100,hwnd,(HMENU) 102,NULL,NULL);
					



}


//===============================
bool ifIssetAllAntecedantsLevel(list<Tache> antecedants)
        {
        for (std::list<Tache>::iterator it=antecedants.begin(); it!=antecedants.end(); ++it)
            
            {  
                if (it->rang == -1){ // si le rang = -1 (la valeur initial).
                    return false;
                }
            }
            return true;
        }
  bool ifIssetAllAntecedantsEarliestStart(list<Tache> antecedants)
        {
             for (std::list<Tache>::iterator it=antecedants.begin(); it!=antecedants.end(); ++it)
            {
                if (it->etapeDebut.dptot == -1)
                {
                    return false;
                }
            }
            return true;
        }

     // Si tous les dates de début au plutard des successeurs sont calculées.
  bool ifIssetAllSuccessorsLatestStart(list<Tache> successors)
        {
            for (std::list<Tache>::iterator it=successors.begin(); it!=successors.end(); ++it)
            {
                 if (it->etapeDebut.dptard == MAX_INT || it->etapeDebut.dptard < it->etapeDebut.dptot)
                {
                    return false;
                }
            }
            return true;
        }
void findLevels() // la méthode qui calcule les rangs des taches.
        {
            int nbTache= Projet.size(); // le nombre des taches dans ce projet .
            int nbAddL = 0; // le nombre des taches que nous avons déja calculé ses rangs.
            // recherche de taches qui n'ont pas d'antecédants.
            for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                if (it->antecedant.size() == 0)
                {
                    it->rang = 1; // attaché le rang '1' au taches qui n'ont pas d'antécédants.
                    nbAddL++; // incrementation de nombre des taches.
                    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                     for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
                     if(anteced1->name==it->name)
                      anteced1->rang=it->rang;
                     
                }
            }
            // mise en place les rang des autres taches.
            while(nbAddL<nbTache) // tantque le nombre des taches que nous avons déja calculé ses rangs est inférieur au nombre total des taches.
            {
               for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
               {
               // pour chaque taches dans la liste des taches.
                
                    if(it->antecedant.size()!=0) // si cette tache posséde des antecédants.
                    {
                        int rangmax = 0; // comme d'habitude, pour n'importe quelle tache, on prend le maximum des rangs des antecedants + 1
                         //if(ifIssetAllAntecedantsLevel(it->anticidant)) // si tous les rangs des antecedants sont calculés.
                        {
                          for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced) // on recupére le rang et on va trouver le maximum , en le compare avec maxLevel
                          
                                rangmax = max(rangmax,anteced->rang);
                            
                            it->rang = rangmax + 1; // on ajoute 1 au maximum trouvée et on l'attache au tache courante
                            //cout<<"Rmax="<< Projet[i].rang;
                             nbAddL++; 
                            for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                     for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
                     if(anteced1->name==it->name)
                      anteced1->rang=it->rang;
                        }
                   }
               }
               
            }
 }
 int getLevelMax()
        {
            int levelMax = 0;
           for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                if(it->rang > levelMax)
                {
                    levelMax = it->rang;
                }
            }
            return levelMax;
        }
        
 void findCoordByLevel(int maxWidth) 
        {
            
            //int random=rand()*(400-50)/RAND_MAX +50; // objet random : pour des entiers aléatoires.
            int coeff = 2*maxWidth / getLevelMax(); // distance calculée entre chaque 2 rangs, on divise la largeur total sur le (nombre ds rangs-2)
           for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it) // pour chaque tache.
            {
                 switch(it->rang) // on prend la valeur du rang.
                {
                    case 1: // si rang == 1
                       {it->etapeFin.coordX = coeff;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +100; // un entier aléatoire entre 100 et 700 pour coordonné Y
                        break;}
                    case 2:{
                        it->etapeFin.coordX = coeff*2; // pour rang 2 on multiplie le coeff par 2 et ainsi de suite ...
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
           
                        break;}
                    case 3:
                        it->etapeFin.coordX = coeff*3;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
                        break;
                    case 4:
                        it->etapeFin.coordX = coeff*4;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
                        
                        break;
                    case 5:
                        it->etapeFin.coordX = coeff*5;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
                         
                        break;
                    case 6:
                        it->etapeFin.coordX = coeff*6;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
                        break;
                    case 7:
                        it->etapeFin.coordX = coeff*7;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
                        break;
                    default:
                        it->etapeFin.coordX = 750;
                        it->etapeFin.coordY = rand()*(700-50)/RAND_MAX +50;
                        break;
                }
            }
        }
  // méthode retourne le maximum entier dans une listes des entiers.
 int findMaximum(vector<int> datesArray)
        {
            int maximum = datesArray[0];
            for (int i = 1; i < datesArray.size();i++)
            {
                if(maximum<datesArray[i])
                {
                    maximum = datesArray[i];
                }
            }
            return maximum;
        }

// méthode retourne le minimum entier dans une listes des entiers.
 int findMinimum(vector<int> datesArray)
        {
            int minimum = datesArray[0];
            for (int i = 1; i < datesArray.size();i++)
            {
                if(minimum>datesArray[i])
                {
                    minimum = datesArray[i];
                }
            }
            return minimum;
        }
void setEarliestStart()
        {
            int calculatedEarliestStartDates = 0; // variable pour compter les taches qu'on a déja calculée ses dates au plutot.
            /* l'idée est la même que la méthode findLevels, on parcoure les dates au plutot des antécédants de chaque tache
             * S'ils sont calculées , on prend le max + la durée de la tache sinon en re-boucle.
            */
            
           for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                if (it->antecedant.size()==0)
                {
                    it->etapeDebut.dptot = 0;
                    calculatedEarliestStartDates++;
                     for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                            for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
                             if(anteced1->name==it->name)
                               anteced1->etapeDebut.dptot=0;
                }
                
            }
           i=0;
           while(calculatedEarliestStartDates<=Projet.size())
            {  
                 for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
                {
                    if(it->antecedant.size()!=0)
                    {
                        //if(ifIssetAllAntecedantsEarliestStart(it->anticidant))
                        {
                            vector<int> earliestStartDates ;
                             for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
                            {
                                earliestStartDates.push_back(anteced->etapeDebut.dptot+anteced->dure);
                            }
                            it->etapeDebut.dptot= findMaximum(earliestStartDates); // en récupére le maximum
                             for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                             for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
                             if(anteced1->name==it->name)
                               anteced1->etapeDebut.dptot=it->etapeDebut.dptot;
                              
                            calculatedEarliestStartDates++;
                        }
                    }
                }
                
           }

           //apres la calcule de toutes les date au plutot , on va trouver la durée total du projet a l'aide des taches qui n'ont pas de successeurs.
            vector<int> findProjctDelay ;
           for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
            if(it->successeur.size()==0)
                {
                    findProjctDelay.push_back(it->etapeDebut.dptot+it->dure);
                }
            }
            //on fait appelle a la méthode findMaximum et en passe la liste des entiers pour trouver le maximum qui sera la durée total du projet.
            lastStep.dptard=findMaximum(findProjctDelay);
            lastStep.dptot=findMaximum(findProjctDelay); // en creer la derniere étape , avec la durée total du projet.
        }
// méthode pour numeroter les etapes.
   void countStep()
        {  
            int number=0;
            for (int i = 1; i <=getLevelMax() ; i++){
            for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
             if(it->etapeDebut.id==-1 && it->rang==i)
              {
                number++;
                it->etapeDebut.id=number;
                for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1)
                {   
                 if(it->name!=it1->name && it->etapeDebut.coordX ==it1->etapeDebut.coordX && it->etapeDebut.coordY ==it1->etapeDebut.coordY ) 
                   it1->etapeDebut.id =it->etapeDebut.id;
                
                }
              }
               //cout<<it->name<<"==>"<<"ID="<<it->etapeDebut.id<<" |"<<endl;
            } 
            
            }
            lastStep.id=number+1;
           // cout<<"X="<<lastStep.coordX<<"Y="<<lastStep.coordY<<"ID="<<lastStep.id;
          
        }
 bool ifIsACriticalPathTask(Tache task)
        {
              for (i = 0; i <criticalPath.size(); i++)
                if (criticalPath[i].name == task.name)
                {
                    return true;
                }
            
            return false;
        }


 // Pour chaque tache, cette méthode va rendre l'etapeDebut des successeurs de cette tache égale au etapeFin de cette tache 
 void switchSteps()
        {  
            for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                if(it->rang==1)
                {
                    it->etapeDebut.coordX =50; // on fixe les coordonnées de l'étape début des tache de rang 1
                    it->etapeDebut.coordY =350;
                  
                    
                }
                if(it->successeur.size()>1)
                {
                    Etape sfstep = it->etapeFin;
                     for (std::list<Tache>::iterator succes=it->successeur.begin(); succes!=it->successeur.end(); ++succes)
                    {
                        succes->etapeDebut.coordX = sfstep.coordX;
                        succes->etapeDebut.coordY = sfstep.coordY;
                          // incrementation de nombre des taches.
                    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                     if(it1->name==succes->name){
                      it1->etapeDebut.coordX=sfstep.coordX;
                      it1->etapeDebut.coordY=sfstep.coordY;}
                        
                    }
                }else{
                    if(it->successeur.size()==1)
                    {
                        Etape sfstep = it->etapeFin;
                        it->successeur.begin()->etapeDebut.coordX = sfstep.coordX;
                        it->successeur.begin()->etapeDebut.coordY = sfstep.coordY;
                        for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                          if(it1->name== it->successeur.begin()->name){
                            it1->etapeDebut.coordX=sfstep.coordX;
                            it1->etapeDebut.coordY=sfstep.coordY;}
                       
                    }else{ // <1 && !=1 : =0
                        it->etapeFin.coordX = 900; // on fixe les coordonnees de l'étape fin de la derniere tache.
                       it->etapeFin.coordY = 600;
                       lastStep.coordX=it->etapeFin.coordX;
                        lastStep.coordY=it->etapeFin.coordY;
                    }
                }
            }
        }
 // la même chose pour les taches qui ont plusieurs antécédants.
    void switchStepsAnteced()
        {
             for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                if(it->antecedant.size()>1)
                {
                    Etape  step = it->etapeDebut;
                    for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
                    {
                        anteced->etapeFin.coordX = step.coordX; // la fin de l'antécédant = au début du tache courante.
                        anteced->etapeFin.coordY = step.coordY;
                         for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                          if(it1->name== anteced->name){
                            it1->etapeFin.coordX=step.coordX;
                            it1->etapeFin.coordY=step.coordY;}
                       
                    }
                }
            }
        }
  // pour calculer les dates au plutard , le même principe de la méthode setEarliestStart().
   void setLatestStart()
        {
            int calculatedLatestStartDates = 0;
           for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                if (it->successeur.size() == 0)
                {
                    it->etapeDebut.dptard = lastStep.dptard- it->dure;
                    calculatedLatestStartDates++;
                   for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                             for (std::list<Tache>::iterator succes1=it1->successeur.begin(); succes1!=it1->successeur.end(); ++succes1)
                             if(succes1->name==it->name)
                               succes1->etapeDebut.dptard=lastStep.dptard- it->dure;
                             
                }
                
            
            }
           while(calculatedLatestStartDates<Projet.size())
            {
                for (std::list<Tache>::reverse_iterator it=Projet.rbegin(); it!=Projet.rend(); ++it)
                {
                   //if(it->etapeDebut.dptard==MAX_INT)
                    {
                        if (it->successeur.size() != 0)
                        {
                            if (ifIssetAllSuccessorsLatestStart(it->successeur))
                            {
                                vector<int> latestStarttDates ;
                               // cout<<it->name<<"==>";
                                 for (std::list<Tache>::iterator succes=it->successeur.begin(); succes!=it->successeur.end(); ++succes)
                                {   
                                    latestStarttDates.push_back(succes->etapeDebut.dptard-it->dure);
                                    //cout<<succes->name<<"|"<<succes->etapeDebut.dptard<<"|"<<it->dure<<"\t";
                                }
                                cout<<endl;
                                it->etapeDebut.dptard = findMinimum(latestStarttDates);
                                calculatedLatestStartDates++;
                                for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                                 for (std::list<Tache>::iterator succes1=it1->successeur.begin(); succes1!=it1->successeur.end(); ++succes1)
                                  if(succes1->name==it->name)
                                    succes1->etapeDebut.dptard=it->etapeDebut.dptard;
                           }
                        }
                    }
                }
            }
             for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
                   if(it->etapeDebut.dptard==MAX_INT)
                     if(it->antecedant.size()==0)
                     it->etapeDebut.dptard=0;
              for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
                {
                  vector<int> latestStarttDates ;
                  for (std::list<Tache>::iterator succes1=it->successeur.begin(); succes1!=it->successeur.end(); ++succes1)
                      latestStarttDates.push_back(succes1->etapeDebut.dptard);
                  int dpMin = findMinimum(latestStarttDates); 
                  for (std::list<Tache>::iterator succes1=it->successeur.begin(); succes1!=it->successeur.end(); ++succes1)
                     {
                       succes1->etapeDebut.dptard=dpMin;
                       for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1)
                       if(succes1->name==it1->name)
                       it1->etapeDebut.dptard=succes1->etapeDebut.dptard;
                     } 
                }   
           
        }
 // La méthode qui va calculer les marges totales pour chaque tache.
     void findMargins() 
        {
              for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {
                it->etapeDebut.marge = it->etapeDebut.dptard -  it->etapeDebut.dptot;
                for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
                                { for (std::list<Tache>::iterator succes1=it1->successeur.begin(); succes1!=it1->successeur.end(); ++succes1)
                                  if(succes1->name==it->name)
                                    succes1->etapeDebut.marge=it->etapeDebut.marge;
                                    for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
                                  if(anteced->name==it->name)
                                    anteced->etapeDebut.marge=it->etapeDebut.marge;}
                           
            }
        }
bool taskNotExistInCriticalPath(Tache task,vector<Tache> criticalPath)
{
      for (std::vector<Tache>::iterator it=criticalPath.begin(); it!=criticalPath.end(); ++it)
     if(it->name==task.name)return false;
     return true;
     
}
//la méthode qui cherche le chemin critique dans l'ensemble des taches.
    vector<Tache> findCriticalPath()
        {
            vector<Tache> criticalPath ;
            vector<Tache> tempPath ;
           
            bool add;
             for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
            {   
                add=false;
                if(it->etapeDebut.marge==0 )
                {  
                  
                  if (it->successeur.size() != 0){
                    for (std::list<Tache>::iterator succes1=it->successeur.begin(); succes1!=it->successeur.end(); ++succes1)
                     if(succes1->etapeDebut.marge==0){
                      add=true; 
                      
                      }}
                  else add=true; 
                }
                if(add){ 
                tempPath.push_back(*it); 
                //criticalPath.push_back(*it);
                }
                
            }
            for (i = 0; i <tempPath.size();i++)
            {
              for (j = 0; j < tempPath.size();j++)
               if(i!=j && tempPath[i].etapeDebut.coordX ==tempPath[j].etapeDebut.coordX && tempPath[i].etapeDebut.coordY ==tempPath[j].etapeDebut.coordY )
                {
                 if(tempPath[i].etapeDebut.dptot>tempPath[j].etapeDebut.dptot)
                 tempPath[i]=tempPath[j];
                 else
                 tempPath[j]=tempPath[i];
                }
              if(taskNotExistInCriticalPath(tempPath[i],criticalPath))
               criticalPath.push_back(tempPath[i]); 
              
            }
               
            // on ordonne les taches critique selon leurs date au plutot
            for (i = criticalPath.size()-1; i >= 0; i--)
            {
                for (j = 0; j < i;j++)
                {
                    if(criticalPath[j].etapeDebut.dptot>criticalPath[j+1].etapeDebut.dptot)
                    {
                        Tache tmp = criticalPath[j + 1];
                        criticalPath[j + 1] = criticalPath[j];
                        criticalPath[j] = tmp;
                    }
                }
            }
            
            return criticalPath;
        }
template <class Container>
void split2(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}
// Cette méthode declanche lorsqu'on clique sur le bouton de choi d'un fichier 'Ouvrir ...'
void chooseFile(HWND hwnd){
     map<string,Tache> tasks;
     map<string,vector<string> > antecedants;
     map<string,vector<string> > successors;
     string line;
     ifstream file("test2.txt",ios::in);
     if(file)
     { 
             
       while(getline(file,line))
       {
                                
                    // en utilise la méthode Split avec un point virgule 
                    // pour chaque point virgule elle met le bloc précédent dans le tableau blocs
                    //Split(line,";");
                    vector<string> blocs;
                    split2(line,blocs,';');
                    vector<string> taskBloc ;
                    taskBloc.push_back(blocs[0]);
                    // taskBloc contient le nom de la tache et sa durée.
                    vector<string>  antecedBloc;
                    antecedBloc.push_back(blocs[1]);
                    // antecedBloc contient les taches antecedantes.
                    vector<string>  succesBloc;
                    succesBloc.push_back(blocs[2]);
                    // succesBloc contient les taches successeurs.
                    // maintenant on utlise Split avec une virgule pour separer les informations complexes dans chaque bloc.
                    vector<string> NameAndDuration;
                    split2(taskBloc[0],NameAndDuration,',');
                    // NameAndDuration : ["nom_tache","durée"]
                    vector<string> antecedant;
                    split2(antecedBloc[0],antecedant,',');
                    // antecedant : ["tache anteced1","tache anteced2",...]
                    vector<string> successor;
                    split2( succesBloc[0],successor,',');
                    // successor : ["tache success1","tache success2",...]
                    // si la tache de la ligne courante posséde des antécédants .
                     tasks[NameAndDuration[0]].name = NameAndDuration[0];
                     tasks[NameAndDuration[0]].dure = atoi(NameAndDuration[1].c_str());
                     tasks[NameAndDuration[0]].rang=-1;
                      // on ajoute la tache au dictionnaire temporaire .
                    if(antecedant[1]!="rien")
                    {
                        vector<string> tmp;
                        for (int i = 1; i < antecedant.size();i++)
                        { 
                           tmp.push_back (antecedant[i]);
                        }
                        // on ajoute au dictionnaire des antecedants : "nom_tache" => liste des antécédants.
                        antecedants[NameAndDuration[0]]=tmp;
                    }
                    else
                    {
                        // sinon on ajoute une liste vide dans le dictionnaire pour la tache courante : "nom_tache" => liste vide.
                        vector<string> tmp;
                        tmp.clear();
                        antecedants[NameAndDuration[0]]=tmp;
                    }
                    // si la tache de la ligne courante posséde des successeurs .
                    if (successor[1] != "rien")
                    {
                        vector<string> tmp;
                        for (int i = 1; i < successor.size(); i++)
                        {
                            tmp.push_back (successor[i]);
                        }
                        // on ajoute au dictionnaire des antecedants : "nom_tache" => liste des antécédants.
                        successors[NameAndDuration[0]]=tmp;
                    }
                    else
                    {
                        // sinon on ajoute une liste vide dans le dictionnaire pour la tache courante : "nom_tache" => liste vide.
                        vector<string> tmp;
                        tmp.clear();
                        successors[NameAndDuration[0]]=tmp;
                    }        
       }
            for (std::map<string,Tache>::iterator it=tasks.begin(); it!=tasks.end(); ++it)
                {
                    list<Tache> anteced ;
                    // pour la tache courante on recupére la liste des antecedants.
                    vector<string> list = antecedants[it->first];
                     for (int i = 0; i < list.size(); i++)
                    {
                        anteced.push_back(tasks[list[i]]);
                    }
                    // on met a jour la liste des antecedants de la tache courante.
                    tasks[it->first].antecedant=anteced;
                }
           for (std::map<string,Tache>::iterator it=tasks.begin(); it!=tasks.end(); ++it)
                {
                    list<Tache> successor ;
                    // pour la tache courante on recupére la liste des antecedants.
                    vector<string> list = successors[it->first];
                     for (int i = 0; i < list.size(); i++)
                    {
                        successor.push_back(tasks[list[i]]);
                    }
                    // on met a jour la liste des antecedants de la tache courante.
                    tasks[it->first].successeur=successor;
                }
            // on initialise les étapes de chaque tache.
                 for (std::map<string,Tache>::iterator it=tasks.begin(); it!=tasks.end(); ++it)
                {   Etape e;
                    e.id=-1;
                    e.coordX=-1;
                    e.coordY=-1;
                    e.dptot=-1;
                    e.dptard=MAX_INT;;
                    e.marge=-1;
                    tasks[it->first].etapeDebut=e;
                    tasks[it->first].etapeFin=e;
                    loadedTasks.push_back(tasks[it->first]);
                }                   
     }
     file.close();
                Projet = loadedTasks; // on donne la liste des taches comme loadedTasks.
                findLevels(); // on trouve les rang des taches.
                setEarliestStart(); // on calcule les dd+tot.
                setLatestStart(); // on calcule les dd+tard.
                findMargins(); // on calcule des marges totales.
                findCoordByLevel(500); // on calcule les coordonnéees X et Y de chaque étape.
                switchSteps(); // mettre la fin de la tache comme debut des successeurs de cette tache. 
                switchStepsAnteced(); // mettre mettre la fin des antecedants comme le début de la tache courante .
                countStep();
                criticalPath = findCriticalPath(); // on trouve le chemin critique.
                ::EnableWindow(btn1,false);
                ::EnableWindow(btn2,true); // on rendre le bouton 'Dessiner le graphe' active.*/
 }
 // La méthode resposable de dessin graphique de graphe (etapes+taches)

void Onpaint(HDC hDC){
   	HPEN hPenOld;
    HPEN hLinePen,hLinePen1;
    COLORREF qLineColor,qLineColor1;
    qLineColor = RGB(255, 125,125);
    qLineColor1 = RGB(0, 125,125);
    hLinePen = CreatePen(PS_SOLID, 4, qLineColor);
    hLinePen1 = CreatePen(PS_SOLID, 4, qLineColor1);
    
    hPenOld = (HPEN)SelectObject(hDC, hLinePen);
    int WindowHeight = 700 ; // la hauteur de l'espace du dessin .
    int windowWidth = 900;// la largeur de l'espace du dessin .
    
  
   for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
    {
    // on commence par les taches de rang 1
       if(it->rang==1)  
           {
              it->painted = true; // pour dire qu'on a atteint la tache courante et on la dessine.
              MoveToEx(hDC, 50 , 350, NULL);// on deplace vers la 1ere étape
              hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
              Ellipse(hDC,30,350,80,400);
              MoveToEx(hDC, 40 , 360, NULL);
              LineTo(hDC, 70,390);
              MoveToEx(hDC, 40 , 390, NULL);
              LineTo(hDC, 70,360);
              std::stringstream wsst,wssd,wssr,wssm;
              wsst << it->etapeDebut.id;
              wssd << it->etapeDebut.dptot;
              wssr << it->etapeDebut.dptard;
              wssm << it->etapeDebut.marge;
              LPCSTR pcst=wsst.str().c_str();
              TextOut(hDC,35,365,pcst,1);
              pcst=wssd.str().c_str();
              TextOut(hDC,50,350,pcst,2);
              pcst=wssr.str().c_str();
              TextOut(hDC,50,380,pcst,2);
              pcst=wssm.str().c_str();
              TextOut(hDC,65,365,pcst,1);
              if(ifIsACriticalPathTask(*it))
                           hPenOld = (HPEN)SelectObject(hDC, hLinePen);
                        else
                           hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
                         
              LineTo(hDC, it->etapeFin.coordX,it->etapeFin.coordY);
              
              std::string a;
              std::stringstream wss;
          	  LPCSTR pcstrn;
	          a = it->name;
	          pcstrn = a.c_str();
	          TextOut(hDC,(50+it->etapeFin.coordX-30)/2,(350+it->etapeFin.coordY)/2,pcstrn,2);
              wss << it->dure ;
              LPCSTR pcstr=wss.str().c_str();
              TextOut(hDC,(50+it->etapeFin.coordX)/2,(350+it->etapeFin.coordY)/2,pcstr,2);
                hPenOld = (HPEN)SelectObject(hDC, hLinePen1);  
           }
         
      else// maintenant si la tache n'est pas de rang 1.
            {
                        it->painted = true; // on marque que le tache courante comme : dessinée.
                        hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
                        Ellipse(hDC,it->etapeDebut.coordX-25,it->etapeDebut.coordY-25,it->etapeDebut.coordX+25,it->etapeDebut.coordY+25);
                        MoveToEx(hDC, it->etapeDebut.coordX-15, it->etapeDebut.coordY-15, NULL);
                        LineTo(hDC, it->etapeDebut.coordX+15,it->etapeDebut.coordY+15);
                        MoveToEx(hDC, it->etapeDebut.coordX-15 , it->etapeDebut.coordY+15, NULL);
                        LineTo(hDC, it->etapeDebut.coordX+15,it->etapeDebut.coordY-15);
             
                        std::stringstream wssn,wssd,wssr,wssm;
                        wssn << it->etapeDebut.id;
                        wssd << it->etapeDebut.dptot;
                        wssr << it->etapeDebut.dptard;
                        wssm << it->etapeDebut.marge;
                        LPCSTR pcst=wssn.str().c_str();
                        TextOut(hDC,it->etapeDebut.coordX-20,it->etapeDebut.coordY-10,pcst,1);
                        pcst=wssd.str().c_str();
                        TextOut(hDC,it->etapeDebut.coordX-5,it->etapeDebut.coordY-25,pcst,2);
                        pcst=wssr.str().c_str();
                        TextOut(hDC,it->etapeDebut.coordX-5,it->etapeDebut.coordY+5,pcst,2);
                        pcst=wssm.str().c_str();
                        TextOut(hDC,it->etapeDebut.coordX+10,it->etapeDebut.coordY-10,pcst,2);
                        if(ifIsACriticalPathTask(*it))
                           hPenOld = (HPEN)SelectObject(hDC, hLinePen);
                        else
                           hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
                        MoveToEx(hDC,it->etapeDebut.coordX+15  ,it->etapeDebut.coordY+15, NULL);
                        LineTo(hDC, it->etapeFin.coordX-15,it->etapeFin.coordY-15);
                        std::string a;
                        std::stringstream wss;
          	            LPCSTR pcstrn;
	                    a = it->name;
	                    pcstrn = a.c_str();
	                    TextOut(hDC,(it->etapeDebut.coordX+it->etapeFin.coordX-30)/2,(it->etapeDebut.coordY+it->etapeFin.coordY)/2,pcstrn,2);
                        wss << it->dure ;
                        LPCSTR pcstr=wss.str().c_str();
                        TextOut(hDC,(it->etapeDebut.coordX+it->etapeFin.coordX)/2,(it->etapeDebut.coordY+it->etapeFin.coordY)/2,pcstr,2);
                        
                 
          } 
      }
                        hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
                        Ellipse(hDC,lastStep.coordX-25,lastStep.coordY-25,lastStep.coordX+25,lastStep.coordY+25);
                        MoveToEx(hDC, lastStep.coordX-15,lastStep.coordY-15, NULL);
                        LineTo(hDC, lastStep.coordX+15,lastStep.coordY+15);
                        MoveToEx(hDC, lastStep.coordX-15 , lastStep.coordY+15, NULL);
                        LineTo(hDC, lastStep.coordX+15,lastStep.coordY-15);
             
                        std::stringstream wssn,wssd,wssr,wssm;
                        wssn << lastStep.id;
                        wssd << lastStep.dptot;
                        wssr << lastStep.dptard;
                        wssm << lastStep.marge;
                        LPCSTR pcst=wssn.str().c_str();
                        TextOut(hDC,lastStep.coordX-20,lastStep.coordY-10,pcst,1);
                        pcst=wssd.str().c_str();
                        TextOut(hDC,lastStep.coordX-5,lastStep.coordY-25,pcst,2);
                        pcst=wssr.str().c_str();
                        TextOut(hDC,lastStep.coordX-5,lastStep.coordY+5,pcst,2);
                        pcst=wssm.str().c_str();
                        TextOut(hDC,lastStep.coordX+10,lastStep.coordY-10,pcst,1);
                  

    SelectObject(hDC, hPenOld);
    DeleteObject(hLinePen);
    ::EnableWindow(btn2,false);
    ::EnableWindow(btn3,true);
}
void showDetails(HWND hwnd){
LPCSTR idstr,idstr1;
string chemin;
std::stringstream dureMin;              
				for (int i = 0; i <criticalPath.size() ; i++)
				{
                  chemin=chemin+criticalPath[i].name;
                }
                idstr=chemin.c_str();
                SetWindowText(lab2,idstr);
		        dureMin << lastStep.dptot;
				idstr1 = dureMin.str().c_str();
                SetWindowText(lab1,idstr1);
                
				
}
void Onerase(HDC hDC){
     
     }
