#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <marche_pied.h>
#include <file.h>

Sommet * generer_graphe(Transport * t) { 
    int total_noeuds = t->nbre_provisions + t->nbre_commandes;
    Sommet * sommets = (Sommet*)malloc(sizeof(Sommet) * total_noeuds);
    if(sommets == NULL) {
        fprintf(stderr, "Erreur d'allocation des noeuds du graphe\n");
        return NULL;
    }

    // Initialisation des sommets fournisseurs
    for(int i = 0; i < t->nbre_provisions; i++) {
        sommets[i].type = FOURNISSEUR;
        sommets[i].index = i;
        sommets[i].degre = 0;
        sommets[i].voisins = NULL;
        for(int j = 0; j < t->nbre_commandes; j++) {
            if(t->tab[i][j] > 0){
                sommets[i].degre++;
                sommets[i].voisins = (int*)realloc(sommets[i].voisins, sizeof(int) * sommets[i].degre);
                if(sommets[i].voisins == NULL) {
                    fprintf(stderr, "Erreur d'allocation des voisins du sommet %d\n", i);
                    // Libération de la mémoire allouée avant l'erreur
                    for(int k = 0; k < i; k++) {
                        free(sommets[k].voisins);
                    }
                    free(sommets);
                    return NULL;
                }
                sommets[i].voisins[sommets[i].degre - 1] = t->nbre_provisions + j;
            }
        }
    }

    // Initialisation des sommets commandes
    for(int i = 0; i < t->nbre_commandes; i++) {
        sommets[t->nbre_provisions + i].type = COMMANDE;
        sommets[t->nbre_provisions + i].index = i;
        sommets[t->nbre_provisions + i].degre = 0;
        sommets[t->nbre_provisions + i].voisins = NULL;
        for(int j = 0; j < t->nbre_provisions; j++) {
            if(t->tab[j][i] > 0){
                sommets[t->nbre_provisions + i].degre++;
                sommets[t->nbre_provisions + i].voisins = (int*)realloc(sommets[t->nbre_provisions + i].voisins, sizeof(int) * sommets[t->nbre_provisions + i].degre);
                if(sommets[t->nbre_provisions + i].voisins == NULL) {
                    fprintf(stderr, "Erreur d'allocation des voisins du sommet %d\n", t->nbre_provisions + i);
                    // Libération de la mémoire allouée avant l'erreur
                    for(int k = 0; k < t->nbre_provisions + i; k++) {
                        free(sommets[k].voisins);
                    }
                    free(sommets);
                    return NULL;
                }
                sommets[t->nbre_provisions + i].voisins[sommets[t->nbre_provisions + i].degre - 1] = j;
            }
        }
    }

    return sommets;
}

void liberer_graphe(Sommet * sommets, int nbre_sommets) {
    for(int i = 0; i < nbre_sommets; i++) {
        free(sommets[i].voisins);
    }
    free(sommets);
}

void ajouter_arete(Sommet * sommets, int depart, int arrive) {
    sommets[depart].degre++;
    sommets[arrive].degre++;

    sommets[depart].voisins = (int*)realloc(sommets[depart].voisins, sizeof(int) * sommets[depart].degre);
    sommets[arrive].voisins = (int*)realloc(sommets[arrive].voisins, sizeof(int) * sommets[arrive].degre);
    
    sommets[depart].voisins[sommets[depart].degre - 1] = arrive;
    sommets[arrive].voisins[sommets[arrive].degre - 1] = depart;
}

int detecter_cycles(Sommet * sommets, int nbre_sommets, int ** cycle) {
    int visited[nbre_sommets];
    int parents[nbre_sommets];

    for(int i = 0; i < nbre_sommets; i++) {
        visited[i] = 0;
        parents[i] = -1;
    }

    File * queue = initialiser_file();

    enfiler(queue, 0);
    visited[0] = 1;
    while(!est_vide(queue)) {
        int curr = defiler(queue);
        for(int i = 0; i < sommets[curr].degre; i++) {
            int s = sommets[curr].voisins[i];
            if( visited[s] == 0 ) {
                enfiler(queue, s);
                parents[s] = curr;
                visited[s] = 1;
            } else if( parents[curr] != s && visited[s] == 1) { //  cycle detecte
                *cycle = (int*)malloc(sizeof(int)  *nbre_sommets);
                for(int i = 0; i < nbre_sommets; i++) {
                    (*cycle)[0] = 0;
                }

                int is_on_path_s[nbre_sommets];
                for(int i = 0; i < nbre_sommets; i++) is_on_path_s[i] = 0;

                int temp = s;
                while(temp != -1) {
                    is_on_path_s[temp] = 1;
                    temp = parents[temp];
                }
                
                int ancestor = -1;
                temp = curr;
                while(temp != -1) {
                    if (is_on_path_s[temp] == 1) {
                        ancestor = temp;
                        break;
                    }
                    temp = parents[temp];
                }
                
                int cycle_index = 0;
                temp = curr;
                while (temp != ancestor) {
                    (*cycle)[cycle_index++] = temp;
                    temp = parents[temp];
                }
                
                int path_s_to_ancestor[nbre_sommets];
                int path_s_index = 0;
                temp = s;
                while(temp != ancestor) {
                    path_s_to_ancestor[path_s_index++] = temp;
                    temp = parents[temp];
                }
                
                (*cycle)[cycle_index++] = ancestor; 
                
                for(int i = path_s_index - 1; i >= 0; i--) {
                    (*cycle)[cycle_index++] = path_s_to_ancestor[i];
                }
                
                (*cycle) = (int*)realloc(*cycle, sizeof(int) * cycle_index);

                libererFile(queue);
                return cycle_index;
            }
        }

    }
    
    libererFile(queue);
    return 0;
}

void afficherGraphe(Sommet * sommets, int nbre_sommets, int nbre_provisions) {
    for(int i = 0; i < nbre_sommets; i++) {
        if(sommets[i].type == FOURNISSEUR) {
            printf("P%d -> [", i+1);
        } else {
            printf("C%d -> [", i - nbre_provisions +1);
        }

        for(int j = 0; j < sommets[i].degre; j++) {
            int curr = sommets[i].voisins[j];
            if(sommets[curr].type == FOURNISSEUR) {
                printf("P%d, ", curr + 1);
            } else {
                 printf("C%d, ", curr - nbre_provisions + 1);
            }
        }
        printf("]\n");

    }
}

void afficherCycle(int *cycle, int n) {
    printf("Cycle detecte (longueur %d) : ", n);
    for(int i = 0; i < n; i++) {
        printf("%d", cycle[i]);
        if (i < n - 1) {
            printf(" -> ");
        }
    }

    printf(" -> %d", cycle[0]); 
    printf("\n");
}

