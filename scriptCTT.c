#include <stdio.h>
#include <stdlib.h>
#define MAX_AGENT 8
#define MAX_J 4

typedef struct Agent Agent;
typedef struct Joueur Joueur;
typedef struct Partie Partie;
typedef struct InfosPub InfosPub;
typedef struct InfosPriv InfosPriv;



struct Agent
{
    int id;
    int coordoPrec[2];
    int coordoActu[2];
    int spe;
    int idJoueur;
};

void initAgent(Agent* a, int id, int J, int x, int y, int s){
    /*Toutes les infos ici sont des infos qui proviennes des scanf et sont en théorie toutes valides*/
    /*Initialiser un agent*/
    a->id = id;
    a->coordoActu[0] = x;
    a->coordoActu[1] = y;
    /*Quand on initialise un agent, au tour 1 les coordo précedentes et les coordo actuelles sont les mêmes*/
    a->coordoPrec[0] = x;
    a->coordoPrec[1] = y;
    a->spe = s;
    a->idJoueur = J;
}


struct Joueur
{
    int numero;
    int score;
    Agent* agents[2]; //je sais pas comment ca marche mais dans le doute je veux pas que ce soit une copie de l'agent original mais bien un pointeur ver sl'agent original
    int end;
};

struct InfosPub
{
    Joueur* infosJ[MAX_J];//les infos sur les ensembles des joueurs qui vont etres maj a chaque tour
    Agent* infosA[MAX_AGENT];//pareil pour les agents
    int infosVa[MAX_AGENT][2];//infos coordo des Voleurs attrapés
    int infosSDT[MAX_AGENT][2];//coordo des cases des scéllées au dernier tour
    int infosS[64][2]; //coordos de l'ensembles des cases scéllés(max 64 cases scelles) 
};

struct InfosPriv
{
    int nbSurv;
    int infosSurv[2][2];
    int nbNI;
    int infosNI[2][2];
};


struct Partie
{
    int tour;
    int nbJoueur;
    int nbAgent;
    int nbVoleursA;
    int nbVoleursADT;
    int nbCasesS;
    int nbCasesSDT;
    InfosPriv* priv;
    InfosPub* pub;
};


int CoordoEstValide(int x, int y){
    /* 
        Retourne 1 si la coordonnée (x,y) est valide, 0 sinon
        Une coordonée est valide si elle est entre (0,0) et (9,9)
    */
    return (x >= 0 && x <= 9) && (y >= 0 && y <= 9);
    
}

int CoordoEstUneChambre(int x, int y){
    /*
        Retourne 1 si la coordonée est valide et elle coorespond à une chambre
        les x représentent les étages, les chambres se retrouvent pour y entre 1 et 8
        y = 9 et y = 0 représente un escalier
        retourne 0 sinon
    */
   return CoordoEstValide(x, y) && (y >= 1 && y <= 8);

}

int CoordoEstUnEscalier(int x, int y){
    /*
        Retourne 1 si la coordo est valide et elle correspond à un escalier
        retourne 0 sinon
    */
   return CoordoEstValide(x, y) && !(CoordoEstUneChambre(x, y));

}

int CoordoIdentique(int x1, int y1, int x2, int y2){
    return (x1 == x2) && (y1 == y2);
}

void RecupererInit(int* nj, int* j){
    /*Récuperer les input de début de partie*/
    fscanf(stdin, "%d%d", nj, j);
}

void RecupererEntier(int *n){
    /*Récupérer un entier n en entrée standard*/
    fscanf(stdin, "%d", n);
}

void Recuperer3Infos(int nblignes, int infos[][3]){
    /*Récuperer 3 entiers en entrée standard nblignes fois*/
    for(int i = 0; i < nblignes; i++){
        fscanf(stdin, "%d %d %d", &infos[i][0], &infos[i][1], &infos[i][2]);
    }
}

void Recuperer5Infos(int nblignes, int infos[][5]){
    /*Récuperer 5 entiers en entrée standard nblignes fois*/
    for(int i = 0; i < nblignes; i++){
        fscanf(stdin, "%d %d %d %d %d", &infos[i][0], &infos[i][1], &infos[i][2], &infos[i][3], &infos[i][4]);
    }
}

void Recuperer2Infos(int nblignes, int infos[][2]){
    /*Récuperer 2 entiers en entrée standard nblignes fois*/
    for(int i = 0; i < nblignes; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
}


void RecupererInputTour(int nj, int* na, int* nva, int* ns, int* surv, int* ni, int infosJ[][3], int infosA[][5], int infosVA[][2], int infosS[][2], int infosSurv[][2], int infosSci[][2]){
    /*Récuperer l'ensemble des input d'un tour*/
    Recuperer3Infos(nj, infosJ); //Récuperer infos joueurs
    RecupererEntier(na); //Récuperer nombre d'agents
    Recuperer5Infos(*na, infosA); //Récuperer infos agents
    RecupererEntier(nva); //Récuperer nombre de voleurs attrapés
    Recuperer2Infos(*nva, infosVA); //Récuperer infos cases avec voleurs attrapés
    RecupererEntier(ns); //Récuperer nombre de cases scellées
    Recuperer2Infos(*ns, infosS); //Récuperer infos cases scellées
    RecupererEntier(surv); //Récuperer nombre d'agents de surveillance du joueurs dans un escalier
    Recuperer2Infos(*surv, infosSurv); //Récuperer infos nobres de voleurs situés à un étage d'agent de surv (qui est a l'escalier)
    RecupererEntier(ni); //Récuperer nombre d'infos envoyées par police scientifique du joueur
    Recuperer2Infos(*ni, infosSci); //Récuperer infos police scientifique
    /*
        pour gagner du temps on peut essayer de faire les différentes manipulation sur les données entre la réception des input
        Ca evite de le faire apres quand le prog attend la réponse (on a 1s pour répondre)
    */ 
    

}

int main(void) {
    int nj, j, na, nva, ns, surv, ni;
    int infosJ[MAX_J][3];
    int infosA[MAX_AGENT][5];
    int infosVA[MAX_AGENT][2];
    int infosS[MAX_AGENT][2];
    int infosSurv[MAX_AGENT][2];
    int infosSci[MAX_AGENT][2];
    
    //récupérer des entrées
    RecupererInit(&nj, &j);

    RecupererInputTour(nj, &na, &nva, &ns, &surv, &ni, infosJ, infosA, infosVA, infosS, infosSurv, infosSci);

    //ecrire dans un fichier
    FILE* fichier = NULL;
    fichier = fopen("test.txt", "w");
    if (fichier != NULL){
        fprintf(fichier, "entrée recup\n");
        fprintf(fichier, "initialisation : ");
        fprintf(fichier, " nj j = %d %d\n", nj, j);
        fprintf(fichier, "Pendant le tour : \n");
        fprintf(fichier, "infos joueurs : \n");
        for(int i = 0; i < nj; i++){
            fprintf(fichier, "J C END : %d %d %d\n", infosJ[i][0], infosJ[i][1], infosJ[i][2]);
        }
        fprintf(fichier, "nb agent : %d\n", na);
        fprintf(fichier, "infos agents : \n");
        for(int i = 0; i < na; i++){
            fprintf(fichier, "ID J F C S : %d %d %d %d %d\n", infosA[i][0], infosA[i][1], infosA[i][2], infosA[i][3], infosA[i][4]);
        }
        fprintf(fichier, "Nb voleurs attrapés tours préc : %d\n", nva);
        fprintf(fichier, "Infos voleurs attrapés : \n");
        for(int i = 0; i < nva; i++){
            fprintf(fichier, "coordonnées : %d,%d\n", infosVA[i][0], infosVA[i][1]);
        }
        fprintf(fichier, "Nb cases scellees dernier tour: %d\n", ns);
        fprintf(fichier, "Infos cases : \n");
        for(int i = 0; i < nva; i++){
            fprintf(fichier, "coordonnées : %d,%d\n", infosS[i][0], infosS[i][1]);
        }
        fprintf(fichier, "Nb agent de surv a un escalier : %d\n", surv);
        fprintf(fichier, "Infos envoyés : \n");
        for(int i = 0; i < nva; i++){
            fprintf(fichier, "etage nb voleurs a l'etage : %d,%d\n", infosSurv[i][0], infosSurv[i][1]);
        }
        fprintf(fichier, "Nb infos envoyés par agent scientifique : %d\n", ni);
        fprintf(fichier, "Infos chambre qui a ete fouillé si y a eu un voleur : \n");
        for(int i = 0; i < nva; i++){
            fprintf(fichier, "coordo : %d,%d\n", infosSci[i][0], infosSci[i][1]);
        }
        fclose(fichier);
    }
    else {
        fprintf(stderr, "impossible d'ouvrir le fichier souhaité\n");
    }
}