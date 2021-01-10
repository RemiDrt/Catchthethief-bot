#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_AGENT 8
#define MAX_J 4

/* Explication du code */
/*
    La stratégie mise en place est la suivante : 1 agent se spécialise en service de surveillance et l'autre reste en brigade rapide
    tant qu'à un etage il y a un voleur détécté par le service de surveillance, l'agent en brigade rapide fait des aller retour de fouille chambre par chambre.
    Quand il n'y a plus / pas de voleur a l'étage, l'agent de surveillance va monter ou descendre d'étage en fonction de sa situation. S'il est parti du bas il va jusqu'en 
    haut étage par étage s'il n'y a pas de voleur à l'étage. Et arrivé en haut il redescend étage par étage en s'arrêtant s'il y a un ou plusieurs voleurs à l'étage.
    Pour l'agent rapide. Quand il n'y a plus de voleur a l'étage, il retourne à l'escalier opposé à celui de l'autre agent et monte les étages comme l'autre agent (il le rejoint
    à son étage). Et s'il y a un voleur à un étage il va fouiller l'étage jusqu'à ce qu'il n'y ai plus de voleur.
 
    code :
        Structure de données :
            Les données sont organisés de la manière suivant (ma méthode est totalement inspiré de la programmation objet je m'en excuse) :

            -Une structure partie stock l'ensemble des données de la partie. Elle contient des variables simples comme nombre de joueur, le nombre d'agents, le tour actuelle,
            le numéro de mon joueur, le nombre de voleurs attrapé au total, le nombre de voleurs attrapés au dernier tour, le nombre de cases scellées au totale et le 
            nombre de cases scellées au dernier tour. Elle contient de plus un pointeur vers une structure appellé InfosPub et un pointeur vers une structure
            InfosPriv.

            -InfosPub : Cette structure sert a receuillir les informations publique fournis chaque tour par le jeu. On retrouve donc dedans une tableau de pointeur vers des Joueur
            (structure représentant un joueur), un tableau de pointeur vers des Agent (structure représentant des agents), un tableau d'entiers à 2 dimensions afin de stocker toutes 
            les informations sur les voleurs, un tableau d'entier à 2 dimensions afin de stocker les informations sur les cases scellés mais aussi un tableau à double entrée qui doit servir 
            à stocker l'ensemble des cases scellées.

            -InfosPriv : Cette structure sert à receuillir les informations privés des joueurs. Celles qu'ils sont les seules à recevoir. Dans la structure on retrouve donc le nombre
            d'agent de surveillance à un escalier d'étage et un tableau à 2 dimension d'entier qui va receuillir les informations transmises par la brigade de surveillance. Et on retrouve
            le même principe pour la brigade scientifique.

            -Joueur : Cette structure représente un joueur. Son score, son id, ces points et aussi un tableau de 2 pointeurs vers des Agents qui sont censés être les siens

            -Agent : Structure représentant un agent avec ces coordonnées actuelles, celles du tour précédent, sa spécialisation, son id et l'id du joueur auquel il appartient.

            Donc une partie contient un pointeur vers une instance d'InfosPub et un pointeur vers une instance d'InfosPriv. InfosPub contient un tableau de <nombre de joueur> pointeur vers des instances de Joueur
            et un tableau de <2 * nombre de joueurs> pointeurs vers des instances d'agents.

            Au début de la partie on crée une instance de Partie une instance de InfosPub et une instance de InfosPriv, on récupère ensuite la première entrée qui donne le nombre de joueur et notre numéros.
            Avec cela on va pouvoir faire l'initialisation de la structure Partie qui va pointer vers InfosPub et InfosPriv. Puis on va initialiser InfosPub qui va créer tous les agents et joueurs necessaire et pointer vers eux

        Algorithmique : L'algorithmique est assez simple. Pour l'aspect de stratégie de déplacement il n'y a pas de boucle. Ce ne sont que des accès simples, et des structures if else
            (le tout dans une boucle pour chaque tour bien sûr)
            C'est dans la récolte et le traitement des données au début des tours de jeux que les algorithmes sont un peu plus complexe.
            Commencons par l'initialisation des structure de données pour initialiser la partie il faut initialiser les infos publiques et privées. Pour initialiser les infos publiques
            il faut faire des boucles pour instancier les joueurs et les agents. Leur allouer de la mémoire et faire pointer vers autant d'instances que necessaire (en fonction du nombre de joueurs de la partie)
            pour instancier tous les joueurs il faut une boucle de <nb joueurs>. Pour les agents il faut une boucle de <2 * nbJoueurs> (max 2 agents par joueurs). Mais il faut aussi initialiser les tableaux
            pour les voleurs attrapés et les cases scellees donc on va se servir de la même boucle et on se retrouve avec un algorithme de complexité O(4n^2) (2x * 2x) mais sur un gros volume de données on
            peut neglier les 4 * donc juste O(n^2).
            Pour recuperer les données tous les tours on a des boucles simples. on connait par exemple a chaque tour le nombre d'agents. Et pour chaque agents on va récuperer les informations
            et les stocker dans la bonne structure.
            Un algorithme interressant selon moi est celui qui permet de retrouver les agents d'un joueurs. (Algorithme de recherche dans une liste). C'est la fonction RecupererAgents
            En connaissant l'id d'un joueurs on parcours tous les agents a la recherches des deux ayant l'id de joueur que nous cherchons et on fait pointer les pointeurs de la structure joueurs vers les agents concernés.
            C'est une fonction qu'on repète tous les tours car le nombre d'agent peut changer avec la fin d'un joueur (qui aurait perdu) donc les agents ne sont pas stocker.
            forcément au même indice dans le tableau des agents, il est donc important  de les retrouver chaque tour (comme les informations sont mises à jour) à de repointer vers l'endroit le bon agent.(recherche brut, element par element du premier jusqu'à ce qu'on ai trouvé les 2 agents)

            Pour le déplacement des agents chaque tour l'algorithmique suis une certaine logique les 2 premiers tours sont prédéfinis. On spécialise notre agent et l'autre commence la fouille de l'étage du bas.
            A partir du 3eme tour on suit les différentes condition. 
            Savoir si les 2 agents sont au même étage et si l'étage contient des voleurs. Si oui le rapide commence la fouille de l'étage jusqu'à ce qu'il n'y ai plus de voleurs à l'étage.
            Si l'étage contient des voleur mais le rapide n'est pas présent il y a 2 cas : - le rapide est dans l'escalier dans ce cas il rejoindre le bon etage. -il est dans un etage et doit donc rejiondre l'escalier.
            Si l'étage ne contient pas de voleur le serveillance va monter/descendre. Si le rapide n'est pas au meme etage il y a les 2 mêmes cas que précédement. S'il était au même étage il va aussi aller au prochain étage comme le surveillance.
            A recommencer tant que la partie n'est pas fini. Si le rapide est arrivé au bout d'un étage on change son mode d'action pour le faire faire demie tour. Même principe pour le surveillance s'il arrive au 9 étage ou au 0, on le fait faire demie tour


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
            priv->infosNI[i][y] = -1;//comme je ne joue pas scientifique je n'ai pas vraiment besoin de ca
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
    return (priv->infosSurv[0][1] > 0);//dans notre strat il n'y a qu'un agent de surv donc pas besoin de faire attention pour ca quoi qu'il arrive sont on est pas a un tour ou on a pas encore spé
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

    /**traiter les infos (on augmente le nombre total de voleurs attrapés**/
    p->nbVoleursA = p->nbVoleursA + p->nbVoleursADT;
    /*********************/

    RecupererEntier(&(p->nbCasesSDT));

    RecupererInfosCasesScellees(p->pub->infosSDT, p->nbCasesSDT, p->nbCasesS);

    /**traiter les infos* (on augmente le nombre total de cases scellées)*/
    p->nbCasesS = p->nbCasesS + p->nbCasesSDT;

    /*********************/

    RecupererEntier(&(p->priv->nbSurv));

    RecupererInfosPriv(p->priv->infosSurv, p->priv->nbSurv);


    RecupererEntier(&(p->priv->nbNI));

    RecupererInfosPriv(p->priv->infosNI, p->priv->nbNI);


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
    /*Variable pour connaitre l'état des mouvements qu'on fait, si on va vers le haut, le bas, la gauche, la droite*/
    char mvtHo2[12] = "reculer";
    char mvtVer1[12] = "descendre";
    char action2[12] = "move";

    /*Entiers qui serviront pour anticiper si la cases du prochain coup calculé sera valide*/
    int x1Suiv=0;
    int x2Suiv=0;
    int y1Suiv=0;
    int y2Suiv=9;

    //récupérer des entrées

    initP(&p);
    RecupererInit(&(p.nbJoueur), &(p.numeroJoueur));
    initPartie(&p, &priv, &pub);
    //afficherPartie(stderr, &p);

    

    while(1){
        RecupererInputTour(&p);
        if(p.tour == 1){
            //tour 1 on specialise le premier agent et le 2eme commence a fouiller l'étage
            fprintf(stdout, "SPEC %d 3; MOVE %d 0 8 \n",  p.pub->infosJ[p.numeroJoueur]->agents[0]->id, p.pub->infosJ[p.numeroJoueur]->agents[1]->id);
        }
        else if(p.tour == 2)
        {
            //on continue la fouille et on attend la fin de la spé
            fprintf(stdout, "SEARCH %d \n", p.pub->infosJ[p.numeroJoueur]->agents[1]->id);
        }
        else
        { 
            if(SpeInProgress(p.pub->infosJ[p.numeroJoueur]->agents[0]) || SpeInProgress(p.pub->infosJ[p.numeroJoueur]->agents[1])) //si un des 2 agents se spé on fait rien 
            {
                fprintf(stderr, "spec in progress \n");
                fprintf(stdout, "PASS \n");
            }
            else 
            {
                /* on attend que la spé soit fini puis on fouille étage par étage */
                //on verifie qu'il y ai un voleur a l'étage
                if(VoleurAEtage(p.priv) && MemeEtage(p.pub->infosJ[p.numeroJoueur]->agents[0], p.pub->infosJ[p.numeroJoueur]->agents[1]))
                {// à l'étage il y a un voleur et le 2eme agent est au même etage
                    if (EstDansEscalier(p.pub->infosJ[p.numeroJoueur]->agents[1]))//s'il l'agent etait dans l'escalier il recule (x = x - 1)
                    {
                        if(strcmp(mvtHo2, "reculer") == 0){//si on recule
                            y2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1] - 1;
                            x2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0];
                            x1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0];
                            y1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1];
                        }
                        else{//si on avance (je le mets au cas ou mais je pense pas que j'arrivera dans ce cas de figure)
                            y2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1] + 1;
                            x2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0];
                            x1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0];
                            y1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1];
                        }
                        if(CoordoEstValide(x2Suiv, y2Suiv)){
                            fprintf(stdout, "MOVE %d %d %d\n", p.pub->infosJ[p.numeroJoueur]->agents[1]->id, x2Suiv, y2Suiv);
                        }
                        else{
                            fprintf(stdout, "PASS\n");
                            fprintf(stderr, "mauvaises coordo %d %d\n", x2Suiv, y2Suiv);    
                        }
                        
                    }
                    else//si on est au même étage et y a un voleur mais a2 n'est pas dans l'escalier : deux cas de figure : fouiller ou aller a l'autre chambre
                    {
                        if(strcmp(action2, "move") == 0){//au dernier tour on a move, il faut fouiller
                            //on verifie au cas ou qu'on est pas dans un escalier et que la coordo est valide (donc on est dans un chambre)
                            if (CoordoEstValide(p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0], p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1]) && !EstDansEscalier(p.pub->infosJ[p.numeroJoueur]->agents[1]))
                            {
                                fprintf(stdout, "SEARCH %d \n", p.pub->infosJ[p.numeroJoueur]->agents[1]->id);// on fouille la chambre
                                strcpy(action2, "search"); //pour savoir la dernière action qu'on a fait
                            }
                            else
                            {
                                fprintf(stdout, "PASS\n");
                                fprintf(stderr, "mauvaises coordo/escalier %d %d\n", x2Suiv, y2Suiv);    
                            }

                        }
                        else//donc au dernier tour on a fouiller il faut move (si on est au bout et que la prochaine coordo est un escalier : il faut faire demi tour)
                        {
                            if((p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1] == 1) && (strcmp(mvtHo2, "reculer") == 0)){
                                strcpy(mvtHo2, "avancer");//si on est au bout a gauche demi tour
                            }
                            else if ((p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1] == 8) && (strcmp(mvtHo2, "avancer") == 0))
                            {
                                strcpy(mvtHo2, "reculer");//si on est au bout a droite demi tour
                            }
                            
                            if(strcmp(mvtHo2, "reculer") == 0){//si on recule
                                y2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1] - 1;
                                x2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0];
                                x1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0];
                                y1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1];
                            }
                            else{//si on avance (je le mets au cas ou mais je pense pas que j'arrivera dans ce cas de figure)
                                y2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1] + 1;
                                x2Suiv = p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0];
                                x1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0];
                                y1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1];
                            }
                            if(CoordoEstValide(x2Suiv, y2Suiv)){
                                fprintf(stdout, "MOVE %d %d %d\n", p.pub->infosJ[p.numeroJoueur]->agents[1]->id, x2Suiv, y2Suiv);
                                strcpy(action2, "move");
                            }
                            else{
                                fprintf(stdout, "PASS\n");
                                fprintf(stderr, "mauvaises coordo %d %d\n", x2Suiv, y2Suiv);    
                            }
                        }
                    }
                }
                else
                {   //soit y a un voleur mais l'autre est pas au même étage => 2 cas : est a l'escalier donc doit rejoindre le bon etage ou est pas a l'escalier donc doit rejoindre l'escalier puis l'étage
                    //soit y a pas de voleur dans ce cas il faut changer d'etage : avancer ou reculer mais si on est au bout faut faire demi tour
                    if(VoleurAEtage(p.priv))//donc ce cas = il faut rejoindre à l'étage (ils sont pas au meme étage)
                    {
                        if(EstDansEscalier(p.pub->infosJ[p.numeroJoueur]->agents[1]))//si l'agent 1 est dans l'escalier il doit juste le rejoindre
                        {
                            //les coordos mises seront forcéments valides car elles proviennents de coordo d'agents
                            fprintf(stdout, "MOVE %d %d %d \n", p.pub->infosJ[p.numeroJoueur]->agents[1]->id, p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0], p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[1]); // je bouge en x (floor) donc je met la coordo de l'autre pour le rejoindre et en y ca bouge pas
                        }
                        else //on est pas à l'escalier et pas a l'etage de surv donc je dois rejoidre l'étage de surv pour ca je vais a l'escalier
                        {
                            //je vais au y de l'escalier et je bouge pas en x
                            fprintf(stdout, "MOVE %d %d 9 \n", p.pub->infosJ[p.numeroJoueur]->agents[1]->id, p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0]);
                        }
                    }
                    else// donc on est dans le cas ou on a pas de voleur a l'étage il faut descendre / monter ET le rapide doit soit rejoindre l'escalier, soit rejoindre l'étage
                    {
                        if( (p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0] == 9) && (strcmp(mvtVer1, "monter") == 0))//on monte mais on est déja au dernier étage 
                        {
                            strcpy(mvtVer1, "descendre");
                        }
                        else if( (p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0] == 0) && (strcmp(mvtVer1, "descendre") == 0))//on descend mais on est en bas
                        {
                            strcpy(mvtVer1, "monter");
                        }

                        if(strcmp(mvtVer1, "descendre") == 0)
                        {
                            x1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0] - 1;
                            y1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1];
                        }
                        else
                        {
                            x1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[0] + 1;
                            y1Suiv = p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1];
                        }
                        //l'agent surv va juste changer d'étage
                        //pour l'agent rapide il y a plusieurs solution : soit il est a l'escalier et il doit rejoindre le même étage, soit il doit rejoindre l'escalier
                        if (EstDansEscalier(p.pub->infosJ[p.numeroJoueur]->agents[1]))//cas ou il faut juste rejoindre le même étage
                        {
                            fprintf(stdout, "MOVE %d %d %d; ", p.pub->infosJ[p.numeroJoueur]->agents[1]->id, x1Suiv, p.pub->infosJ[p.numeroJoueur]->agents[0]->coordoActu[1]); 
                        }
                        else
                        {
                            fprintf(stdout, "MOVE %d %d 9; ", p.pub->infosJ[p.numeroJoueur]->agents[1]->id, p.pub->infosJ[p.numeroJoueur]->agents[1]->coordoActu[0]);
                        }
                        fprintf(stdout, "MOVE %d %d %d \n", p.pub->infosJ[p.numeroJoueur]->agents[0]->id, x1Suiv, y1Suiv);
                    }                    
                }
                
            }
            
        }
        
        //afficherPartie(stderr, &p);
        p.tour++;
        fflush(stdout);
        fflush(stderr);
    }

}