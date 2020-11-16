#include <stdio.h>
#include <stdlib.h>
#define MAX_AGENT 10
#define MAX_J 5

void RecupererInit(int* nj, int* j){
    fscanf(stdin, "%d%d", nj, j);
}

void RecupererEntier(int *na){
    fscanf(stdin, "%d", na);
}

void RecupererInfosJoueurs(int nj, int infos[][3]){
    for(int i = 0; i < nj; i++){
        fscanf(stdin, "%d %d %d", &infos[i][0], &infos[i][1], &infos[i][2]);
    }
}

void RecupererInfosAgents(int na, int infos[][5]){
    for(int i = 0; i < na; i++){
        fscanf(stdin, "%d %d %d %d %d", &infos[i][0], &infos[i][1], &infos[i][2], &infos[i][3], &infos[i][4]);
    }
}

void RecupererInfosVoleursA(int nva, int infos[][2]){
    for(int i = 0; i < nva; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
}

void RecupererInfosCasesS(int ns, int infos[][2]){
    for(int i = 0; i < ns; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
}

void RecupererInfosSurv(int surv, int infos[][2]){
    for(int i = 0; i < surv; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
}

void RecupererInfosScientifique(int ni, int infos[][2]){
    for(int i = 0; i < ni; i++){
        fscanf(stdin, "%d %d", &infos[i][0], &infos[i][1]);
    }
}

void RecupererInputTour(int nj, int* na, int* nva, int* ns, int* surv, int* ni, int infosJ[][3], int infosA[][5], int infosVA[][2], int infosS[][2], int infosSurv[][2], int infosSci[][2]){
    RecupererInfosJoueurs(nj, infosJ);
    RecupererEntier(na);
    RecupererInfosAgents(*na, infosA);
    RecupererEntier(nva);
    RecupererInfosVoleursA(*nva, infosVA);
    RecupererEntier(ns);
    RecupererInfosCasesS(*ns, infosS);
    RecupererEntier(surv);
    RecupererInfosSurv(*surv, infosSurv);
    RecupererEntier(ni);
    RecupererInfosScientifique(*ni, infosSci);
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