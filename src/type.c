#include <stdio.h>
#include <stdlib.h>
#include <type.h>

Transport * creerTransport(int nbre_provisions, int nbre_commandes) {
    Transport * t = (Transport*)malloc(sizeof(Transport));

    t->nbre_commandes = nbre_commandes;
    t->nbre_provisions = nbre_provisions;


    t->commandes = (int*)malloc(sizeof(int) * t->nbre_commandes);
    if(t->commandes == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau de commandes\n");
        free(t);
        return NULL;
    }

    t->provisions = (int*)malloc(sizeof(int) * t->nbre_provisions);
    if(t->provisions == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau de provisions\n");
        free(t->commandes);
        free(t);
        return NULL;
    }

    t->constraintes = (int**)malloc(sizeof(int*) * t->nbre_provisions);
    t->tab = (int**)malloc(sizeof(int*) * t->nbre_provisions);
    if(t->constraintes == NULL || t->tab == NULL) {
        fprintf(stderr, "Erreur d'allocation du tableau de commandes (en lignes)\n");
        free(t->commandes);
        free(t->provisions);
        free(t);
        return NULL;
    }
    for(int i = 0; i < t->nbre_provisions; i++) {
        t->tab[i] = (int*)malloc(sizeof(int) * t->nbre_commandes);
        t->constraintes[i] = (int*)malloc(sizeof(int) * t->nbre_commandes);
        if(t->tab[i] == NULL || t->constraintes[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation du tableau de commandes (en colonnes %d)\n", i);
            for(int j = 0; j < i; j++) {
                free(t->tab[j]);
                free(t->constraintes[j]);
            }
            free(t->tab);
            free(t->constraintes);
            free(t->commandes);
            free(t->provisions);
            free(t);
            return NULL;
        }
    }

    // initialisation du tab avec des zeros
    for(int i = 0; i < t->nbre_provisions; i++) {
        for(int j = 0; j < t->nbre_commandes; j++){
            t->tab[i][j] = 0;
        }
    }

    return t;
}

void libererTransport(Transport * t) {
    for(int i = 0; i < t->nbre_provisions; i++) {
        free(t->constraintes[i]);
        free(t->tab[i]);
    }
    free(t->constraintes);
    free(t->tab);
    free(t->commandes);
    free(t->provisions);
    free(t);
    t = NULL;
}

void afficher_cout(Transport * t) {
    printf("+");
    for(int j = 0; j < t->nbre_commandes+2; j++) {
        printf("------+");
    }
    printf("\n");
    printf("|  C   |");
    for(int i = 0; i < t->nbre_commandes; i++) {
        printf("%3s%2d |", "C", i+1);
    }
    printf(" Pro  |\n");

    printf("+");
    for(int i = 0; i < t->nbre_commandes+2; i++) {
        printf("------+");
    }
    printf("\n");

    for(int i = 0; i < t->nbre_provisions; i++) {
        printf("|%3s%2d |", "P", i+1);
        for(int j = 0; j < t->nbre_commandes; j++) {
            printf("%5d |", t->constraintes[i][j]);
        }
        printf("%5d |", t->provisions[i]);
        printf("\n");
    }

    printf("+");
    for(int i = 0; i < t->nbre_commandes+2; i++) {
        printf("------+");
    }
    printf("\n");

    printf("| Com  |");    
    for(int i = 0; i < t->nbre_commandes; i++) {
        printf("%5d |", t->commandes[i]);
    }
    printf("      |\n");
     printf("+");
    for(int i = 0; i < t->nbre_commandes+2; i++) {
        printf("------+");
    }
    printf("\n");
}

void afficher_proposition_transport(Transport * t) {
    printf("+");
    for(int j = 0; j < t->nbre_commandes+2; j++) {
        printf("------+");
    }
    printf("\n");
    printf("|  P   |");
    for(int i = 0; i < t->nbre_commandes; i++) {
        printf("%3s%2d |", "C", i+1);
    }
    printf(" Pro  |\n");

    printf("+");
    for(int i = 0; i < t->nbre_commandes+2; i++) {
        printf("------+");
    }
    printf("\n");

    for(int i = 0; i < t->nbre_provisions; i++) {
        printf("|%3s%2d |", "P", i+1);
        for(int j = 0; j < t->nbre_commandes; j++) {
            printf("%5d |", t->tab[i][j]);
        }
        printf("%5d |", t->provisions[i]);
        printf("\n");
    }

    printf("+");
    for(int i = 0; i < t->nbre_commandes+2; i++) {
        printf("------+");
    }
    printf("\n");

    printf("| Com  |");    
    for(int i = 0; i < t->nbre_commandes; i++) {
        printf("%5d |", t->commandes[i]);
    }
    printf("      |\n");
     printf("+");
    for(int i = 0; i < t->nbre_commandes+2; i++) {
        printf("------+");
    }
    printf("\n");
}


int cout_transport(Transport * t) {
    int cout = 0;
    for(int i = 0; i < t->nbre_provisions; i++) {
        for(int j = 0; j < t->nbre_commandes; j++) {
            cout += t->tab[i][j] * t->constraintes[i][j];
        }
    }
    return cout;
}

void reiniatitialiser_transport(Transport * t) {
    for(int i = 0; i < t->nbre_provisions; i++) {
        for(int j = 0; j < t->nbre_commandes; j++){
            t->tab[i][j] = 0;
        }
    }
}