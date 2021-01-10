#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_AGENT 8
#define MAX_J 4

/*
Explication du code ici 


*/

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
    for(int i = 0; i < nbJoueurs; i++){ 
        Joueur* j = (Joueur*)malloc(sizeof(Joueur));
        initJoueur(j);
        pub->infosJ[i] = j;//on pointe la case du tableau vers le joueurs qu'on a init (et biensur il a sa memoire alloué)
        Agent* a = (Agent*)malloc(sizeof(Agent));
        initAgent(a);
        pub->infosA[i] = a;//même principe que pour les joueurs mais comme y a max 2 agents par joueurs on le fait a chaque fois
        for(int y = 0; y < 2 * nbJoueurs; y ++){
            pub->infosVa[i][y] = -1;
            pub->infosSDT[i][y] = -1;
        }

    }//arrivé on a fait que la moitier des agents (max) mais tous les joueurs
    for(int i = 0; i < nbJoueurs; i ++){//donc on fait l'autre moitier des agents
        Agent* a = (Agent*)malloc(sizeof(Agent));
        initAgent(a);
        pub->infosA[i + nbJoueurs] = a;//même principe que pour les joueurs mais comme y a max 2 agents par joueurs on le fait a chaque fois
        for(int y = 0; y < 2 * nbJoueurs; y ++){
            pub->infosVa[i + nbJoueurs][y] = -1;
            pub->infosSDT[i + nbJoueurs][y] = -1;
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
void initP(Partie* p){
    p->tour = 1;
    p->nbJoueur = 0;
    p->numeroJoueur = 0;
    p->nbVoleursA = 0;
    p->nbVoleursADT = 0;
    p->nbCasesS = 0;
    p->nbCasesSDT = 0;
    p->priv = NULL;
    p->pub = NULL;
}

void initPartie(Partie* p, InfosPriv* priv, InfosPub* pub){
    p->tour = 1;
    //on n'initie pas nbJoueurs et numeroJoueurs car elles ont deja été récoltées
    p->nbAgent = 2 * p->nbJoueur;
    p->nbVoleursA = 0;
    p->nbVoleursADT = 0;
    p->nbCasesS = 0;
    p->nbCasesSDT = 0;
    p->priv = priv;
    p->pub = pub;
    initPriv(priv);
    initPub(pub, p->nbJoueur);
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
    /*
        Retourne 1 les coordo en paramètres sont identiques
        retourne 0 sinon
    */
    return (x1 == x2) && (y1 == y2);
}

int MemeEtage(Agent* a1, Agent* a2){
    /*
        Retourne 1 les agents a1 et a2 sans au même étage
        retourne 0 sinon
    */
    return a1->coordoActu[0] == a2->coordoActu[0];
}

int EstDansEscalier(Agent* a){
    /*
        Retourne 1 l'agent a est dans un escalier
        retourne 0 sinon
    */
    return (a->coordoActu[1] == 0) ||  (a->coordoActu[1] == 9);
}

int EtaitDansEscalier(Agent* a){
    /*
        Retourne 1 l'agent a etait dans un escalier au tour précédent
        retourne 0 sinon
    */
    return (a->coordoPrec[1] == 0) ||  (a->coordoPrec[1] == 9);
}

int SpeInProgress(Agent* a){
    /*
        Retourne 1 si l'agent a est en cours de spécialisation
        retourne 0 sinon
    */
    return a->spe == 0;
}

int VoleurAEtage(InfosPriv* priv){
    /*
        Retourne 1 s'il y a une voleur a notre étage
        Obtenue grave aux infos privé avec un agent de surveillance
        retourne 0 sinon
    */
    return (priv->infosSurv[0][NV] > 0);//dans notre strat il n'y a qu'un agent de surv donc pas besoin de faire attention pour ca quoi qu'il arrive sont on est pas a un tour ou on a pas encore spé
}

void RecupererAgents(Agent* agents[], Agent* agentsJoueur[2], int nbAgents, int idJ){
    int agentTrouv = 0;
    for(int i = 0; (i < nbAgents) && (agentTrouv < 2) ; i++){
        if (agents[i]->idJoueur == idJ)//on a trouvé un agent qui a notre id de joueur
        {
            fprintf(stderr, "trouvé agent %d %d %p\n", agents[i]->idJoueur, idJ, agents[i]);
            agentsJoueur[agentTrouv] = agents[i];//faire pointer notre pointeur de type agent vers notre agent
            agentTrouv ++;
        }
    }
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
        fscanf(stdin, "%d %d %d", (&joueurs[i]->numero), &(joueurs[i]->score), &(joueurs[i]->end));
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

void RecupererInfosPriv(int infos[][2], int nbInfos){
    /*Récupérer les informations privés (recuperer 2 entier dans un tableau nbInfos fois)*/
    for(int i = 0; i < nbInfos; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
}


void RecupererInputTour(Partie* p){
    /*Récuperer l'ensemble des input d'un tour*/
    RecupererInfosJoueurs(p->pub->infosJ, p->nbJoueur);

    RecupererEntier(&(p->nbAgent));

    RecupererInfosAgents(p->pub->infosA, p->nbAgent, p->tour);
    /*associer le joueur a ses agents*/
    //on le fait a chaque tour car des agents sont retirés du jeux quand un joueurs a perdu / fini donc on mettra les infos d'agents dans un autre agent
    RecupererAgents(p->pub->infosA, p->pub->infosJ[p->numeroJoueur]->agents, p->nbAgent, p->numeroJoueur);
    /**********************************/
    RecupererEntier(&(p->nbVoleursADT));

    RecupererInfosVoleursAttrapes(p->pub->infosVa, p->nbVoleursADT, p->nbVoleursA);

    /**traiter les infos**/
    p->nbVoleursA = p->nbVoleursA + p->nbVoleursADT;
    /*********************/

    RecupererEntier(&(p->nbCasesSDT));

    RecupererInfosCasesScellees(p->pub->infosSDT, p->nbCasesSDT, p->nbCasesS);

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


void afficherAgent(FILE* f, Agent* a){
    fprintf(f, "agent %p : %d | (%d,%d) | (%d, %d) | %d | %d\n", a, a->id, a->coordoPrec[0], a->coordoPrec[1], a->coordoActu[0], a->coordoActu[1], a->spe, a->idJoueur);
}

void afficherJoueur(FILE* f, Joueur* j){
    fprintf(f, "joueur %p : %d | %d | %p | %p | %d\n", j, j->numero, j->score, j->agents[0], j->agents[1], j->end);
}

void afficherTabDouble(FILE* f,int tab[][2], int dim){
    for(int i = 0; i < dim; i++){
        for(int y = 2; y < 2; y++){
            fprintf(f, "%d | ", tab[i][y]);
        }      
    }
    fprintf(f, "\n");
}

void afficherPriv(FILE* f, InfosPriv* priv){
    fprintf(f, "priv : %d | %d\n", priv->nbSurv, priv->nbNI);
    afficherTabDouble(f, priv->infosSurv, 2);
    afficherTabDouble(f, priv->infosNI, 2);
}

void afficherPub(FILE* f, InfosPub* pub, int nbJoueurs, int VA, int CSDT){
    fprintf(f, "pub : \n");
    afficherTabDouble(f, pub->infosVa, VA);
    afficherTabDouble(f, pub->infosSDT, CSDT);
    for(int i = 0; i < nbJoueurs; i++){
        afficherJoueur(f, pub->infosJ[i]);
    }
    for(int i = 0; i < 2 * nbJoueurs; i++){
        afficherAgent(f, pub->infosA[i]);
    }
}

void afficherPartie(FILE* f, Partie* p){
    fprintf(f, "p : %d | %d | %d | %d | %d | %d | %d | %d \n", p->tour, p->numeroJoueur, p->nbJoueur, p->nbAgent, p->nbVoleursA, p->nbVoleursADT, p->nbCasesS, p->nbCasesSDT);
    afficherPriv(f, p->priv);
    afficherPub(f, p->pub, p->nbJoueur, p->nbVoleursADT, p->nbCasesSDT);
}

int main(void) {
    Partie p;
    InfosPriv priv;
    InfosPub pub;
    int fin; //variable pour détecter la fin du jeux
    //récupérer des entrées
    char mvtHo1[12] = "reculer";
    char mvtHo2[12] = "avancer";
    char mvtVer1[12] = "descendre";
    char mvtVer1[12] = "descendre";

    
    initP(&p);
    RecupererInit(&(p.nbJoueur), &(p.numeroJoueur));
    initPartie(&p, &priv, &pub);
    afficherPartie(stderr, &p);

    /*A partir d'ici on a créer nbJoueur Joueur et 2*nbJoueur Agent
    *leurs pointeurs sont stockés dans les tableau correspondant dans pub
    */

    

    while(fin){
        RecupererInputTour(&p);
        if(p.tour == 1){
            //tour 1 on va aller en haut pour les 2 agents
            fprintf(stdout, "MOVE %d 9 0; MOVE %d 9 9 \n", p.pub->infosJ[p.numeroJoueur]->agents[0]->id, p.pub->infosJ[p.numeroJoueur]->agents[1]->id);
        }
        else if(p.tour == 2)
        {
            fprintf(stdout, "SPEC %d 2; PASS \n", p.pub->infosJ[p.numeroJoueur]->agents[0]->id);
            /* On spé le premier agent en Surve*/
        }
        else
        { 
            if(SpeInProgress(p.pub->infosJ[p.numeroJoueur]->agents[0]) || SpeInProgress(p.pub->infosJ[p.numeroJoueur]->agents[1])) //si un des 2 agents se spé on fait rien 
            {
                fprintf(stdout, "PASS \n");
            }
            else 
            {
                /* on attend que la spé soit fini puis on fouille étage par étage */
                //on verifie qu'il y ai un voleur a l'étage
                if(voleurAEtage())
            }
            
        }
        
        
        afficherPartie(stderr, &p);
        p.tour++;
        fin = fin;
        fflush(stdout);
        fflush(stderr);
    }

}