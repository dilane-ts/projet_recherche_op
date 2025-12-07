#ifndef __MARCHE_PIED_H__
#define __MARCHE_PIED_H__

#include <type.h>

typedef enum {
    FOURNISSEUR,
    COMMANDE
} TypeSommet;

typedef struct sommet {
    TypeSommet type;
    int index;
    int * voisins;
    int degre;
} Sommet;


Sommet * generer_graphe(Transport * t);
void liberer_graphe(Sommet * sommets, int nbre_sommets);

/**
 * Détecte la présence de cycles dans le graphe.
 * @param sommets  tableau des sommets du graphe
 * @param nbre_sommets  nombre total de sommets dans le graphe  
 * @param cycle  tableau pour stocker le cycle détecté (si trouvé)
 * @return  longueur du cycle détecté, ou 0 s'il n'y a pas de cycle.
 */
int detecter_cycles(Sommet * sommets, int nbre_sommets, int ** cycle);

void afficherGraphe(Sommet * sommets, int nbre_sommets, int nbre_provisions);
void afficherCycle(int *cycle, int n);
void ajouter_aret(Sommet * sommets, int depart, int arrive);

#endif