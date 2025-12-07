#include <stdio.h>
#include <stdlib.h>
#include <nord_ouest.h>
#include <balas_hammer.h>
#include <marche_pied.h>
#include <utils.h>


int main() {
    Transport * t = lireTransport("data/test1.txt");
    afficher_cout(t);
    nord_ouest(t);
    // balas_hammer(t);
    // printf("Proposition de transport finale (BALAS-HAMMER):\n");
    afficher_proposition_transport(t);
    // printf("Coût total du transport (BALAS-HAMMER): %d\n", cout_transport(t));

    Sommet * sommets = generer_graphe(t);
    int * cycles;
    afficherGraphe(sommets, t->nbre_commandes + t->nbre_provisions, t->nbre_provisions);
    int longueur = detecter_cycles(sommets, t->nbre_commandes + t->nbre_provisions, &cycles);
    if(longueur)
        afficherCycle(cycles, longueur);
        

    libererTransport(t);
    return 0;
}