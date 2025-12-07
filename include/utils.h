#ifndef __UTILS_H__
#define __UTILS_H__

#include <type.h>
#include <liste.h>

Transport * lireTransport(char * filepath);

// fonctions sur les matrices et tableaux
/**
 * Somme des element de la ligne i allant de start (inclus) a end(inclus)
 * @param M la matrice
 * @param i le numero de la ligne
 * @param start indice de debut
 * @param end indice de fin
 * @return int
 */
int somme_ligne(int ** M, int i, int start, int end);

/**
 * Somme des element de la colonne j allant de start (inclus) a end(inclus)
 * @param M la matrice
 * @param j le numero de la colonne
 * @param start indice de debut
 * @param end indice de fin
 * @return int
 */
int somme_colonne(int ** M, int j, int start, int end);


/**
 * Calcul de penalite sur une ligne
 * @param M matrice des couts
 * @param m nombre de colonne
 * @param i la ligne de calcul de penalite
 * @param min_pos la position du plus petit 
 */
int penalite_ligne(int **M, int m, int i, ListeExclusion liste, int *min_pos);


/**
 * Calcul de penalite sur une colonne
 * @param M matrice des couts
 * @param n nombre de ligne
 * @param j la colonne de calcul de penalite
 */
int penalite_colone(int **M, int n, int j, ListeExclusion liste, int *min_pos);


int somme_matrice(int **M, int n, int m);
int somme_tableau(int *T, int size);
int min(int a, int b);

int copier_tableau(int * A, int * B, int n);

#endif