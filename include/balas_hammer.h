#ifndef __BALAS_HAMMER__
#define __BALAS_HAMMER__

#include <type.h>
#include <liste.h>

// fonctions principales de l'algorithme Balas-Hammer

/**
 * Algorithme de Balas-Hammer
 * @param t le transport
 */
void balas_hammer(Transport * t);

/**
 * Calcul des penalités pour les lignes et colonnes
 * @param t le transport
 * @param liste liste des ligne/colonnes exclues
 * @return liste de penalités
 */
ListePenalite * calcul_penalite(Transport * t, ListeExclusion * liste);

/**
 * Recherche des penalités max 
 * @param penalites liste de penalités
 * @return le nombre de penalité max trouvée
 */
void penalites_max(ListePenalite * penalites);

/**
 * Choix entre ligne et colonne parmi les penalités max
 * @param t le transport
 * @param penalites liste de penalités
 * @return la penalité_max du choix
 */
Penalite choix_ligne_colonne(Transport * t, ListePenalite * penalites);

#endif