#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <utils.h>

Transport * lireTransport(char * filepath) {
    FILE * file = fopen(filepath, "r");
    if(file == NULL) {
        fprintf(stderr, "Erreur de lecture du fichier %s\n", filepath);
        return NULL;
    }
    int nbre_provisions, nbre_commandes;
    fscanf(file, "%d %d", &nbre_provisions, &nbre_commandes);

    Transport * t = creerTransport(nbre_provisions, nbre_commandes);
    if(t == NULL) {
        return NULL;
    }
    

    for(int i = 0; i < nbre_provisions; i++) {
        for(int j = 0; j < nbre_commandes; j++) {
            fscanf(file, "%d", &t->constraintes[i][j]);
        }
        fscanf(file, "%d", &t->provisions[i]);
    }
    for(int j = 0; j < nbre_commandes; j++) {
        fscanf(file, "%d", &t->commandes[j]);
    }

    return t;
}

int somme_ligne(int ** M, int i, int start, int end) {
    int sum = 0;
    for (int j = start; j <= end; j++) {
        sum += M[i][j];
    }
    return sum;
}

int somme_colonne(int ** M, int j, int start, int end) {
    int sum = 0;
    for (int i = start; i <= end; i++) {
        sum += M[i][j];
    }
    return sum;
}


int  penalite_ligne(int **M, int m, int i, ListeExclusion liste, int *min_pos) {
    int C_min1 = -1;
    int C_min2 = -1;
    int j_min1 = -1;

    for(int j = 0; j < m; j++) {
        if( est_exclu(j, COLONNE, liste) == 0) {
            int current_cost = M[i][j];

            if(C_min1 == -1 || current_cost < C_min1) {
                C_min2 = C_min1;
                C_min1 = current_cost;
                j_min1 = j;
            } else if (C_min2 == -1 || current_cost < C_min2) {
                C_min2 = current_cost;
            }
        }
    }

    *min_pos = j_min1;

    if(C_min1 == -1 || C_min1 == -1) {
        return 0;
    }

    return abs(C_min2 - C_min1);
}

int penalite_colone(int **M, int n, int j, ListeExclusion liste, int *min_pos) {
    int C_min1 = -1;
    int C_min2 = -1;
    int i_min1 = -1;

    for(int i = 0; i < n; i++) {
        if( est_exclu(i, LIGNE, liste) == 0) {
            int current_cost = M[i][j];

            if(C_min1 == -1 || current_cost < C_min1) {
                C_min2 = C_min1;
                C_min1 = current_cost;
                i_min1 = i;
            } else if (C_min2 == -1 || current_cost < C_min2) {
                C_min2 = current_cost;
            }
        }
    }

    *min_pos = i_min1;

    if(C_min1 == -1 || C_min2 == -1) {
        return 0;
    }

    return abs(C_min2 - C_min1);
}

int somme_matrice(int **M, int n, int m) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            sum += M[i][j];
        }
    }
    return sum;
}

int somme_tableau(int *T, int size) {
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += T[i];
    }
    return sum;
}


int min(int a, int b) {
    return a > b ? b : a;
}


int copier_tableau(int * A, int * B, int n) {
    for(int i = 0; i < n; i++) {
        B[i] = A[i];
    }
}