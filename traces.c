#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nord_ouest.h>
#include <balas_hammer.h>
#include <marche_pied.h>
#include <connexite.h>
#include <utils.h>

typedef struct {
    int i;
    int j;
} AreteAjoutee;

typedef struct {
    AreteAjoutee * aretes;
    int count;
    int capacite;
} ListeAretesAjoutees;

void init_liste_aretes(ListeAretesAjoutees * liste) {
    liste->aretes = NULL;
    liste->count = 0;
    liste->capacite = 0;
}

void ajouter_arete_a_liste(ListeAretesAjoutees * liste, int i, int j) {
    if (liste->count >= liste->capacite) {
        liste->capacite = (liste->capacite == 0) ? 4 : liste->capacite * 2;
        liste->aretes = (AreteAjoutee*)realloc(liste->aretes, sizeof(AreteAjoutee) * liste->capacite);
    }
    liste->aretes[liste->count].i = i;
    liste->aretes[liste->count].j = j;
    liste->count++;
}

void liberer_liste_aretes(ListeAretesAjoutees * liste) {
    free(liste->aretes);
    liste->aretes = NULL;
    liste->count = 0;
    liste->capacite = 0;
}

void resoudre_transport_nord_ouest(const char * filepath) {
    printf("\n================================================\n");
    printf("Fichier : %s\n", filepath);
    printf("================================================\n");
    
    Transport * t = lireTransport(filepath);
    
    if (t == NULL) {
        printf("ERREUR : Impossible de lire le fichier\n");
        return;
    }
    
    printf("Probleme : %d fournisseurs x %d commandes\n", 
           t->nbre_provisions, t->nbre_commandes);
    
    // Solution initiale Nord-Ouest
    printf("\n--- Solution initiale (Nord-Ouest) ---\n");
    nord_ouest(t);
    afficher_proposition_transport(t);
    printf("Cout initial : %d\n", cout_transport(t));
    
    // Optimisation par marche-pied
    int iteration = 0;
    int max_iterations = 100;
    int amelioration = 1;
    ListeAretesAjoutees aretes_connexite;
    init_liste_aretes(&aretes_connexite);
    
    while (amelioration && iteration < max_iterations) {
        iteration++;
        amelioration = 0;
        
        printf("\n========== ITERATION %d ==========\n", iteration);
        
        int N = t->nbre_provisions + t->nbre_commandes;
        
        // Afficher la matrice tab actuelle
        printf("\nMatrice tab actuelle :\n");
        afficher_proposition_transport(t);
        printf("Cout : %d\n", cout_transport(t));
        
        // Test de dégénérescence
        int nbre_aretes_basiques = 0;
        for (int i = 0; i < t->nbre_provisions; i++) {
            for (int j = 0; j < t->nbre_commandes; j++) {
                if (t->tab[i][j] > 0) {
                    nbre_aretes_basiques++;
                }
            }
        }
        int nbre_aretes_requises = t->nbre_provisions + t->nbre_commandes - 1;
        printf("Aretes basiques : %d / %d requis", nbre_aretes_basiques, nbre_aretes_requises);
        if (nbre_aretes_basiques < nbre_aretes_requises) {
            printf(" - DEGENEREE\n");
        } else {
            printf("\n");
        }
        
        // Élimination des cycles
        printf("\nElimination des cycles :\n");
        int cycles_iteration = 0;
        int max_cycles_iterations = 50;
        int cycles_detectes = 0;
        
        while (cycles_iteration < max_cycles_iterations) {
            cycles_iteration++;
            
            Sommet * sommets = generer_graphe(t);
            int * cycle = NULL;
            int longueur = detecter_cycles(sommets, N, &cycle);
            
            if (longueur > 0) {
                cycles_detectes++;
                printf("  Cycle #%d : ", cycles_detectes);
                for (int k = 0; k < longueur; k++) {
                    printf("%d", cycle[k]);
                    if (k < longueur - 1) printf(" -> ");
                }
                printf(" -> %d\n", cycle[0]);
                
                int delta = maximiser_transport_sur_cycle(t, cycle, longueur, 1);
                printf("  Delta = %d", delta);
                
                if (delta == 0) {
                    printf(" - CAS DEGENERE : retrait aretes connexite\n");
                    for (int k = 0; k < aretes_connexite.count; k++) {
                        int i = aretes_connexite.aretes[k].i;
                        int j = aretes_connexite.aretes[k].j;
                        printf("    Retrait (P%d, C%d)\n", i + 1, j + 1);
                        t->tab[i][j] = 0;
                    }
                    aretes_connexite.count = 0;
                } else {
                    printf("\n");
                }
                
                free(cycle);
                liberer_graphe(sommets, N);
            } else {
                if (cycles_detectes == 0) {
                    printf("  Aucun cycle detecte\n");
                } else {
                    printf("  Graphe acyclique apres %d cycle(s)\n", cycles_detectes);
                }
                liberer_graphe(sommets, N);
                break;
            }
        }
        
        // Test de connexité
        printf("\nTest de connexite :\n");
        int * compSommet = (int*)malloc(sizeof(int) * N);
        if (!compSommet) break;
        
        int nbComp = test_connexite(t, compSommet);
        
        if (nbComp > 1) {
            printf("  Graphe NON CONNEXE : %d composantes\n", nbComp);
            
            // Afficher les composantes
            for (int comp = 0; comp < nbComp; comp++) {
                printf("  Composante %d : ", comp);
                int first = 1;
                for (int s = 0; s < N; s++) {
                    if (compSommet[s] == comp) {
                        if (!first) printf(", ");
                        printf("%d", s);
                        first = 0;
                    }
                }
                printf("\n");
            }
            
            printf("  Ajout d'aretes pour rendre connexe :\n");
            aretes_connexite.count = 0;
            int nF = t->nbre_provisions;
            int nC = t->nbre_commandes;
            
            int nbComp_actuel = nbComp;
            while (nbComp_actuel > 1) {
                int bestI = -1, bestJ = -1, bestCout = 0, first = 1;
                
                for (int i = 0; i < nF; ++i) {
                    for (int j = 0; j < nC; ++j) {
                        if (t->tab[i][j] == 0) {
                            int u = i, v = nF + j;
                            if (compSommet[u] != compSommet[v]) {
                                int c = t->constraintes[i][j];
                                if (first || c < bestCout) {
                                    first = 0;
                                    bestCout = c;
                                    bestI = i;
                                    bestJ = j;
                                }
                            }
                        }
                    }
                }
                
                if (bestI != -1) {
                    t->tab[bestI][bestJ] = 1;
                    ajouter_arete_a_liste(&aretes_connexite, bestI, bestJ);
                    printf("    Ajout (P%d, C%d) cout %d\n", bestI + 1, bestJ + 1, bestCout);
                    nbComp_actuel = test_connexite(t, compSommet);
                } else {
                    break;
                }
            }
        } else {
            printf("  Graphe CONNEXE\n");
        }
        
        free(compSommet);
        
        // Calcul des potentiels
        printf("\nCalcul des potentiels :\n");
        int * potentiels = (int*)calloc(N, sizeof(int));
        if (!potentiels) {
            fprintf(stderr, "Erreur allocation potentiels\n");
            break;
        }
        
        if (calculer_potentiels(t, potentiels) != 0) {
            printf("  ERREUR : calcul impossible\n");
            free(potentiels);
            break;
        }
        
        afficher_potentiels(t, potentiels);
        
        printf("\nTable des couts potentiels (u_i + v_j) :\n");
        afficher_table_couts_potentiels(t, potentiels);
        
        printf("\nTable des couts marginaux (c_ij - u_i - v_j) :\n");
        int best_i = -1, best_j = -1;
        int existe_ameliorante = afficher_table_couts_marginaux(t, potentiels, &best_i, &best_j);
        
        if (existe_ameliorante && best_i != -1 && best_j != -1) {
            printf("\n=> Arete ameliorante trouvee : (P%d, C%d)\n", best_i + 1, best_j + 1);
            t->tab[best_i][best_j] = 1;
            amelioration = 1;
        } else {
            printf("\n=> Aucune arete ameliorante : SOLUTION OPTIMALE\n");
        }
        
        free(potentiels);
    }
    
    // Solution finale
    printf("\n--- Solution optimale ---\n");
    afficher_proposition_transport(t);
    printf("Cout optimal : %d\n", cout_transport(t));
    printf("Iterations : %d\n", iteration);
    
    liberer_liste_aretes(&aretes_connexite);
    libererTransport(t);
}

int main() {
    char filepath[256];
    
    while (scanf("%255s", filepath) == 1) {
        resoudre_transport_nord_ouest(filepath);
    }
    
    return 0;
}
