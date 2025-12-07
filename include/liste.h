#ifndef __LISTE_H__
#define __LISTE_H__

typedef enum {
    LIGNE,
    COLONNE
} TypePenalite;



typedef struct penalite {
    int val;
    int pos; // le numero de ligne ou colone
    int min; // la position du plus petit de la ligne ou colone
    TypePenalite type; // 0 pour ligne et 1 pour colonne;
    struct penalite * suivant;
} Penalite;

typedef struct maillon {
    int index;
    TypePenalite type;
    struct maillon * suivant;
} Maillon;

typedef Maillon * ListeExclusion;

typedef Penalite * ListePenalite;


// fonctions de manipulation des listes (exclusion, penalites)
/**
 * Ajout d'un element a la liste d'exclusion
 * @param liste la liste d'exclusion
 * @param index l'index de la ligne ou colonne
 * @param type le type (LIGNE ou COLONNE)
 * @return la nouvelle liste d'exclusion
 */ 
void ajouter_exclusion(ListeExclusion * liste, int index, TypePenalite type);

void ajouter_penalite(ListePenalite * penalites, Penalite p);


void libererListeExclusion(ListeExclusion * liste);

void libererPenalites(ListePenalite * penalites);

int est_exclu(int index, TypePenalite type, ListeExclusion liste);



#endif