#include <stdio.h>
#include <stdlib.h>
#include <nord_ouest.h>
#include <utils.h>

void nord_ouest(Transport * t) {
    int i = 0;
    int j = 0;
    int provisions[t->nbre_provisions];
    int commandes[t->nbre_commandes];

    copier_tableau(t->provisions, provisions, t->nbre_provisions);
    copier_tableau(t->commandes, commandes, t->nbre_commandes); 

    while(i < t->nbre_provisions && j < t->nbre_commandes) {
        int allocation = min(commandes[j], provisions[i]);
        t->tab[i][j] = allocation;
        commandes[j] -= allocation;
        provisions[i] -= allocation;

        if(provisions[i] == 0) {
            i = i+1;
        } 
        if (commandes[j] == 0) {
            j = j+1;
        }
    }
}