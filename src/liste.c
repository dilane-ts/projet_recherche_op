#include <stdio.h>
#include <stdlib.h>
#include <liste.h>

// fonctions de manipulation des listes (exclusion, penalites)
void ajouter_exclusion(ListeExclusion * liste, int index, TypePenalite type) {
    Maillon * nouveau = (Maillon*)malloc(sizeof(Maillon));
    if(nouveau == NULL) {
        fprintf(stderr, "Erreur d'allocation pour l'ajout dans la liste d'exclusion.\n");
        return;
    }
    nouveau->index = index;
    nouveau->type = type;
    nouveau->suivant = *liste;
    *liste = nouveau;
}

void ajouter_penalite(ListePenalite * penalites, Penalite p) {
    Penalite * nouveau = (Penalite*)malloc(sizeof(Penalite));
    if(nouveau == NULL) {
        fprintf(stderr, "Erreur d'allocation pour l'ajout dans la liste de penalites.\n");
        return;
    }
    *nouveau = p;
    nouveau->suivant = *penalites;
    *penalites = nouveau;
}

void libererListeExclusion(ListeExclusion * liste) {
    Maillon * current = *liste;
    Maillon * suivant;
    while(current != NULL) {
        suivant = current->suivant;
        free(current);
        current = suivant;
    }
    *liste = NULL;
}

void libererPenalites(ListePenalite * penalites) {
    Penalite * current = *penalites;
    Penalite * suivant;
    while(current != NULL) {
        suivant = current->suivant;
        free(current);
        current = suivant;
    }
    *penalites = NULL;
}

int est_exclu(int index, TypePenalite type, ListeExclusion liste) {
    ListeExclusion current = liste;

    while(current != NULL) {
        if(current->index == index && current->type == type) {
            return 1;
        }
        current = current->suivant;
    }
    return 0;
}
