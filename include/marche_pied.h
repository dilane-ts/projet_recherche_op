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
void ajouter_arete(Sommet * sommets, int depart, int arrive);

/**
 * Applique la maximisation sur un cycle (méthode du marche-pied).
 * @param t           Matrice de transport (x[i][j] dans t->tab)
 * @param cycle       Tableau des indices de sommets (F puis C en alternance), longueur = len
 * @param len         Nombre de sommets dans le cycle
 * @param start_plus  1 si l'arête (cycle[0] -> cycle[1]) est marquée '+', 0 sinon
 *                    (en général 1 : on démarre par l'arête non basique qu'on veut ajouter)
 * @return            Δ utilisé (la valeur poussée sur le cycle)
 */
int maximiser_transport_sur_cycle(Transport *t, const int *cycle, int len, int start_plus);

#endif