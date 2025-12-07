#include <stdio.h>
#include <stdlib.h> 
#include <balas_hammer.h>
#include <utils.h>


void balas_hammer(Transport * t) {
    ListeExclusion liste = NULL;
    int total = somme_tableau(t->provisions, t->nbre_provisions);
    int provisions[t->nbre_provisions];
    int commandes[t->nbre_commandes];

    copier_tableau(t->provisions, provisions, t->nbre_provisions);
    copier_tableau(t->commandes, commandes, t->nbre_commandes);     

    int remplir = 0;
    while(total > remplir) {
        ListePenalite * penalites = calcul_penalite(t, &liste);
        if (penalites == NULL) {
            fprintf(stderr, "Erreur: calcul_penalite a retourné NULL\n");
            break;
        }
        if (*penalites == NULL) { // pas de pénalités valides → sortie
            free(penalites);
            break;
        }
        penalites_max(penalites);
        Penalite p = choix_ligne_colonne(t, penalites);
        int allocation = 0;

        if(p.type == LIGNE) {
            allocation = min(commandes[p.min], provisions[p.pos]);
            commandes[p.min] -= allocation;
            provisions[p.pos] -= allocation;
            t->tab[p.pos][p.min] = allocation;
            // int commandes = t->commandes[p.min] - somme_colonne(t->tab, p.min, 0, t->nbre_provisions -1);
            // int provisions = t->provisions[p.pos] - somme_ligne(t->tab, p.pos, 0, t->nbre_commandes -1);
            if(provisions[p.pos] == 0) {
                ajouter_exclusion(&liste, p.pos, LIGNE);
            } 
            if(commandes[p.min] == 0) {
                ajouter_exclusion(&liste, p.min, COLONNE);
            }
        } else {
            allocation = min(commandes[p.pos], provisions[p.min]);
            commandes[p.pos] -= allocation;
            provisions[p.min] -= allocation;
            t->tab[p.min][p.pos] = allocation;
            // int commandes = t->commandes[p.pos] - somme_colonne(t->tab, p.pos, 0, t->nbre_provisions -1);
            // int provisions = t->provisions[p.min] - somme_ligne(t->tab, p.min, 0, t->nbre_commandes -1);
            if(commandes[p.pos] == 0) {
                ajouter_exclusion(&liste, p.pos, COLONNE);
            } 
            if(provisions[p.min] == 0) {
                ajouter_exclusion(&liste, p.min, LIGNE);
            }
        }
        libererPenalites(penalites);
        free(penalites);
        remplir  += allocation;
    }
    
    libererListeExclusion(&liste);
}

ListePenalite * calcul_penalite(Transport * t, ListeExclusion * liste) {
    ListePenalite * penalites = malloc(sizeof(ListePenalite));
    if(penalites == NULL) {
        fprintf(stderr, "Erreur d'allocation pour la liste de penalites\n");
        return NULL;
    }
    *penalites = NULL;

    

    // penalites sur les lignes
    
    for(int i = 0; i < t->nbre_provisions; i++) {
        int est_exclue = est_exclu(i, LIGNE, *liste);
        if(est_exclue == 0) {
            Penalite p;
            p.pos = i;
            p.type = LIGNE;
            p.val = penalite_ligne(t->constraintes, t->nbre_commandes, i, *liste, &p.min);
            ajouter_penalite(penalites, p);
        }
        
    }

    // penalites sur les colonnes
    for(int i = 0; i < t->nbre_commandes; i++) {
        int est_exclue = est_exclu(i, COLONNE, *liste);
        if(est_exclue == 0) {
            Penalite p;
            p.pos = i;
            p.type = COLONNE;
            p.val = penalite_colone(t->constraintes, t->nbre_provisions, i, *liste, &p.min);
            ajouter_penalite(penalites, p);
        }
    }

    return penalites;
}

void penalites_max(ListePenalite * penalites) { 
    ListePenalite current = *penalites;
    int max_val = -1;
    while(current != NULL) {
        if(current->val > max_val) {
            max_val = current->val;
        }
        current = current->suivant;
    }

    // supprimer les penalites qui ne sont pas max
    current = *penalites;
    ListePenalite prev = NULL;
    while(current != NULL) {
        if(current->val != max_val) {
            if(prev == NULL) {
                *penalites = current->suivant;
                free(current);
                current = *penalites;
            } else {
                prev->suivant = current->suivant;
                free(current);
                current = prev->suivant;
            }
        } else {
            prev = current;
            current = current->suivant;
        }
    }
}

int recuperer_cout(Transport * t, Penalite * p) {
    if (p->type == LIGNE) {
        return t->constraintes[p->pos][p->min];
    } else {
        return t->constraintes[p->min][p->pos];
    }
}

Penalite choix_ligne_colonne(Transport * t, ListePenalite * penalites) {
    ListePenalite current_node = *penalites;
    Penalite * best_candidate = current_node;

    int min_cost;
    if (best_candidate->type == LIGNE) {
        min_cost = t->constraintes[best_candidate->pos][best_candidate->min];
    } else {
        min_cost = t->constraintes[best_candidate->min][best_candidate->pos];
    }

    current_node = current_node->suivant;

    while (current_node != NULL) {
        int current_cost;
        if (current_node->type == LIGNE) {
            current_cost = t->constraintes[current_node->pos][current_node->min];
        } else {
            current_cost = t->constraintes[current_node->min][current_node->pos];
        }

        if (current_cost < min_cost) {
            min_cost = current_cost;
            best_candidate = current_node;
        }

        current_node = current_node->suivant;
    }
    return *best_candidate;
}

