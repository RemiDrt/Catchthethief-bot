#include <stdio.h>
#include <stdlib.h>
#define MAX_AGENT 10
#define MAX_J 5

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
    Recuperer2InfosVoleursA(*nva, infosVA); //Récuperer infos cases avec voleurs attrapés
    RecupererEntier(ns); //Récuperer nombre de cases scellées
    Recuperer2InfosCasesS(*ns, infosS); //Récuperer infos cases scellées
    RecupererEntier(surv); //Récuperer nombre d'agents de surveillance du joueurs dans un escalier
    Recuperer2InfosSurv(*surv, infosSurv); //Récuperer infos nobres de voleurs situés à un étage d'agent de surv (qui est a l'escalier)
    RecupererEntier(ni); //Récuperer nombre d'infos envoyées par police scientifique
    Recuperer2InfosScientifique(*ni, infosSci); //Récuperer infos police scientifique
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
        fprintf(fichier, "%d %d\n", nj, j);
        fclose(fichier);
    }
    else {
        fprintf(stderr, "impossible d'ouvrir le fichier souhaité\n");
    }
}