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

// -----------------------------------------------------------------------------
// Maximisation du transport sur un cycle détecté (méthode du marche-pied)
// -----------------------------------------------------------------------------

/* Convertit une arête entre deux sommets du graphe biparti en coordonnée (i,j)
 * Retourne 0 si OK et remplit (i,j), ou -1 si l'arête n'est pas une F<->C valide.
 */
static int edge_to_ij(const Transport *t, int u, int v, int *i, int *j) {
    int nF = t->nbre_provisions;
    int nC = t->nbre_commandes;

    // u est fournisseur ?
    if (u >= 0 && u < nF) {
        // v doit être une commande
        if (v >= nF && v < nF + nC) {
            *i = u;
            *j = v - nF;
            return 0;
        }
        return -1;
    }
    // u est commande ?
    if (u >= nF && u < nF + nC) {
        // v doit être un fournisseur
        if (v >= 0 && v < nF) {
            *i = v;
            *j = u - nF;
            return 0;
        }
        return -1;
    }
    return -1;
}

/* Renvoie la quantité x[i][j] associée à l'arête (u,v) si c'est une arête F<->C,
 * sinon -1 en cas d'arête invalide.
 */
static int get_qty_on_edge(const Transport *t, int u, int v) {
    int i, j;
    if (edge_to_ij(t, u, v, &i, &j) != 0) return -1;
    return t->tab[i][j];
}

/* Ajoute (peut être négatif) delta à l'arête (u,v) si F<->C valide. */
static void add_qty_on_edge(Transport *t, int u, int v, int delta) {
    int i, j;
    if (edge_to_ij(t, u, v, &i, &j) != 0) return;
    t->tab[i][j] += delta;
    if (t->tab[i][j] < 0) t->tab[i][j] = 0; // sécurité
}

/* Applique la maximisation sur un cycle.
 * cycle : séquence de sommets (F,C,F,C,...) de longueur len ; on suppose cycle fermé :
 * les arêtes du cycle sont (cycle[k], cycle[(k+1)%len]) pour k=0..len-1
 * start_plus : 1 si l'arête k=0 est marquée '+', sinon 0 (elle serait '-').
 * Retourne Δ (valeur poussée).
 */
int maximiser_transport_sur_cycle(Transport *t, const int *cycle, int len, int start_plus) {
    if (!t || !cycle || len < 4 || (len % 2) != 0) {
        // Un cycle valide a au moins 4 sommets et longueur paire
        return 0;
    }

    // 1) Déterminer Δ = min des quantités sur les arêtes "−" du cycle
    int delta = -1;
    for (int k = 0; k < len; ++k) {
        int u = cycle[k];
        int v = cycle[(k + 1) % len];

        int is_plus = ((k % 2) == 0) ? start_plus : !start_plus;
        if (!is_plus) { // arête "−"
            int q = get_qty_on_edge(t, u, v);
            if (q < 0) {
                fprintf(stderr, "Cycle invalide: arête non basique F<->C introuvable pour k=%d\n", k);
                return 0;
            }
            if (delta == -1 || q < delta) delta = q;
        }
    }
    if (delta < 0) delta = 0; // si aucune arête "−" trouvée (ne devrait pas arriver si le cycle est correct)
    if (delta == 0) {
        // Cas dégénéré : on pousse 0 → rien ne change ; le pilote de marche-pied
        // décidera quelle arête 0 retirer si besoin.
        return 0;
    }

    // 2) Appliquer les +Δ / −Δ le long du cycle
    for (int k = 0; k < len; ++k) {
        int u = cycle[k];
        int v = cycle[(k + 1) % len];
        int is_plus = ((k % 2) == 0) ? start_plus : !start_plus;
        add_qty_on_edge(t, u, v, is_plus ? +delta : -delta);
    }

    // 3) Optionnel : retirer proprement de la base une des arêtes "−" devenues 0
    // (Ici on laisse à 0 ; si tu tiens une structure "base" booléenne, pense à la MAJ.)

    return delta;
}
