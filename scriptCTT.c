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
    int id;//id de l'agent
    int coordoPrec[2];//coordo de l'agent au tour actuel
    int coordoActu[2];//coordo de l'agent au tour precedent
    int spe;//specialisation de l'agent
    int idJoueur;//id du joueur de l'agent
};

struct Joueur
{
    int numero;//numero/id du joueur
    int score;//score du joueur
    Agent* agents[2]; //Agents du joueurs
    int end;//etat du joueur
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
    int nbSurv;//nombre d'agent de surveillence à un escalier d'etage 
    int infosSurv[2][2];//etage et nombre de voleur à l'etage pour chaque agent de surveillance à un escalier
    int nbNI;//nombre d'infos envoyés par les agents scientifique
    int infosNI[2][2];// coordo des voleurs pour les infos envoyés par les agents scientifique
};

/*La structure partie représente des informations globales sur l'état de la partie*/
struct Partie
{
    int tour;//tour actuelle de la partie
    int numeroJoueur;//numero de mon joueur
    int nbJoueur;//nombre de joueurs dans la partie
    int nbAgent;//nombre d'agents dans la partie
    int nbVoleursA;//nombre total de voleurs attrapées
    int nbVoleursADT;//nombre de voleurs attrapées au dernier tour
    int nbCasesS;//nombre de cases scellées total
    int nbCasesSDT;//nombre de cases scellées au dernier tour
    InfosPriv* priv;//informations données à tous les joueurs
    InfosPub* pub;//informations personnelles à mon joueur
};

void initAgent(Agent* a){
    /*Toutes les infos ici sont des infos qui proviennes des scanf et sont en théorie toutes valides*/
    /*Initialiser un agent*/
    a->id = -1;
    a->coordoActu[0] = -1;
    a->coordoActu[1] = -1;
    /*Quand on initialise un agent, au tour 1 les coordo précedentes et les coordo actuelles sont les mêmes*/
    a->coordoPrec[0] = -1;
    a->coordoPrec[1] = -1;
    a->spe = -1;
    a->idJoueur = -1;
}

void initJoueur(Joueur* j){
    j->numero = -1;
    j->score = -2;
    j->agents[0] = NULL;
    j->agents[0] = NULL;
    j->end = -1;
}

void initPub(InfosPub* pub, int nbJoueurs){
    for(int i = 0; i < 2 * nbJoueurs; i++){
        if((i%2) == 0){//une fois sur 2 on init un Joueur
            Joueur* j = (Joueur)malloc(sizeof(Joueur));
            initJoueur(j);
            pub->infosJ[i] = j;//on pointe la case du tableau vers le joueurs qu'on a init (et biensur il a sa memoire alloué)
        }
        Agent* a = (Agent)malloc(sizeof(Agent));
        init(a);
        pub->infosA[i] = a;//même principe que pour les joueurs mais comme y a max 2 agents par joueurs on le fait a chaque fois
        for(int y = 0; y < 2 * nbJoueurs; y ++){
            pub->infosA[i][y] = -1;
            pub->infosSDT[i][y] = -1;
        }
    }
}

void initPriv(InfosPriv* priv){
    priv->nbSurv = -1;
    priv->nbNI = -1;
    for(int i = 0; i < 2; i++){
        for(int y = 0; y < 2; y++){
            priv->infosSurv[i][y] = -1;
            priv->infosNI[i][y] = -1;
        }
    }
}

void initPartie(Partie* p, InfosPriv* priv, InfosPub* pub){
    p->tour = 1;
    //on n'initie pas nbJoueurs et numeroJoueurs car elles ont deja été récoltées
    p->nbVoleursA = 0;
    p->nbVoleursADT = 0;
    p->nbCasesS = 0;
    p->nbCasesSDT = 0;
    p->priv = priv;
    p->pub = pub;
    initInfosPriv(priv);
    initInfosPub(pub, nbJoueurs);
}





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

void RecupererInit(int *nbJoueur, int *numJoueur){
    /*Récuperer les input de début de partie et les stock dans la structur "Partie*/
    fscanf(stdin, "%d %d", nbJoueur, numJoueur);
}

void RecupererEntier(int *n){
    /*Récupérer un entier n en entrée standard*/
    fscanf(stdin, "%d", n);
}

void RecupererInfosJoueurs(Joueur* joueurs[], int nbJoueurs){
    /*Récuperer les informations relatives aux joueurs de la partie*/
    for(int i = 0; i < nbJoueurs; i++){
        fscanf(stdin, "%d %d %d", &(joueurs[i]->numero), &(joueurs[i]->score), &(joueurs[i]->end));
    }
}

void RecupererInfosAgents(Agent* agents[], int nbAgents, int tour){
    /*Récuperer les informations relatives aux agents de la partie*/
    if(tour == 1){//le premier tour les coordo precedentes sont les coordo du premier tour
        for(int i = 0; i < nbAgents; i++){
        fscanf(stdin, "%d %d %d %d %d", &(agents[i]->id), &(agents[i]->idJoueur), &(agents[i]->coordoActu[0]), &(agents[i]->coordoActu[1]), &(agents[i]->spe));
        agents[i]->coordoPrec[0] = agents[i]->coordoActu[0];
        agents[i]->coordoPrec[1] = agents[i]->coordoActu[1];
    }
    }
    else {
        for(int i = 0; i < nbAgents; i++){
        agents[i]->coordoPrec[0] = agents[i]->coordoActu[0];
        agents[i]->coordoPrec[1] = agents[i]->coordoActu[1];
        fscanf(stdin, "%d %d %d %d %d", &(agents[i]->id), &(agents[i]->idJoueur), &(agents[i]->coordoActu[0]), &(agents[i]->coordoActu[1]), &(agents[i]->spe));
    }
    }
    
}

void RecupererInfosVoleursAttrapes(int voleursAttrapes[][2] , int nbVoleursAttrapesTour, int nbVoleursAttrapesTot){
    /*Récuperer les informations des voleurs attrapés*/
    for(int i = 0; i < nbVoleursAttrapesTour; i++){
        fscanf(stdin, "%d %d", &voleursAttrapes[nbVoleursAttrapesTot + i][0], &voleursAttrapes[nbVoleursAttrapesTot + i][1]);
    }
}

void RecupererInfosCasesScellees(int casesScellees[][2] , int nbCasesScelleesTour, int nbCasesScelleesTot){
    /*Récuperer les informations des cases scellées*/
    for(int i = 0; i < nbCasesScelleesTour; i++){
        fscanf(stdin, "%d %d", &casesScellees[nbCasesScelleesTot + i][0], &casesScellees[nbCasesScelleesTot + i][1]);
    }
}

void RecupererInfosPriv(int infos[][], int nbInfos){
    /*Récupérer les informations privés (recuperer 2 entier dans un tableau nbInfos fois)*/
    for(int i = 0; i < nbInfos; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
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


void RecupererInputTour(Partie* p){
    /*Récuperer l'ensemble des input d'un tour*/
    RecupererInfosJoueurs(p->pub->infosJ, p->nbJoueur);

    RecupererEntier(&(p->nbAgent));
    RecupererInfosAgents(p->pub->infosA, p->nbAgent);

    /*associer le joueurs a ces agents*/

    RecupererEntier(&(p->nbVoleursADT));
    RecupererInfosVoleursAttrapes(p->pub->infosVa, p->nbVoleursADT);
    /**traiter les infos**/
    p->nbVoleursA = p->nbVoleursA + p->nbVoleursADT;
    /*********************/

    RecupererEntier(&(p->nbCasesSDT));
    RecupererInfosCasesScellees(p->pub->infosSDT, p->nbCasesSDT);
    /**traiter les infos**/
    p->nbCasesS = p->nbCasesS + p->nbCasesSDT;
    /*********************/

    RecupererEntier(&(p->priv->nbSurv));
    RecupererInfosPriv(p->priv->infosSurv, p->priv->nbSurv);
    /**traiter les infos**/
    /*********************/
    RecupererEntier(&(p->priv->nbNI));
    RecupererInfosPriv(p->priv->infosNI, p->priv->nbNI);
    /**traiter les infos**/
    /*********************/
}

/*void RecupererInputTour(Partie* p, int infos[][5], int nj, int* na, int* nva, int* ns, int* surv, int* ni, int infosJ[][3], int infosA[][5], int infosVA[][2], int infosS[][2], int infosSurv[][2], int infosSci[][2]){
    /*Récuperer l'ensemble des input d'un tour
    Recuperer3Infos(p->nbJoueur, infos); //Récuperer infos joueurs
    /*Traitement des infos sur les joueurs
    /**************************************


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
        Ca evite de le faire apres quand le prog attend la réponse (on a 1 s pour répondre)
    
    

}*/

int main(void) {
    Partie p;
    InfosPriv priv;
    InfosPub pub;
    int fin; //variable pour détecter la fin du jeux
    //récupérer des entrées
    RecupererInit(&(p->nbJoueur), &(p->numeroJoueur));
    initPartie(&p, &priv, &pub);

    /*A partir d'ici on a créer nbJoueur Joueur et 2*nbJoueur Agent
    *leurs pointeurs sont stockés dans les tableau correspondant dans pub
    */

    while(fin){
       p.tour++;
       fin = ;
   }
    RecupererInputTour(&p);

    //ecrire dans un fichier
    FILE* fichier = NULL;
    fichier = fopen("test.txt", "w");
    /*if (fichier != NULL){
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
    */
}