#include <stdio.h>
#include "connexite.h"

int test_connexite(int nF, int nC,
                   int base[nF][nC],
                   int compSommet[nF + nC])
{
    int N = nF + nC;
    int marque[N];
    int queue[N];
    int nbComp = 0;

    for (int v = 0; v < N; ++v) {
        marque[v] = 0;
        compSommet[v] = 0;
    }

    for (int v = 0; v < N; ++v) {
        if (!marque[v]) {
            nbComp++;

            int head = 0, tail = 0;
            queue[tail++] = v;
            marque[v] = 1;
            compSommet[v] = nbComp;

            while (head < tail) {
                int u = queue[head++];

                if (u < nF) {
                    /* u = fournisseur Pi */
                    int i = u;
                    for (int j = 0; j < nC; ++j) {
                        if (base[i][j]) {
                            int w = nF + j;  /* client Cj */
                            if (!marque[w]) {
                                marque[w] = 1;
                                compSommet[w] = nbComp;
                                queue[tail++] = w;
                            }
                        }
                    }
                } else {
                    /* u = client Cj */
                    int j = u - nF;
                    for (int i = 0; i < nF; ++i) {
                        if (base[i][j]) {
                            int w = i;       /* fournisseur Pi */
                            if (!marque[w]) {
                                marque[w] = 1;
                                compSommet[w] = nbComp;
                                queue[tail++] = w;
                            }
                        }
                    }
                }
            }
        }
    }

    if (nbComp == 1) {
        printf("La proposition est connexe.\n");
    } else {
        printf("La proposition n'est PAS connexe (%d composantes).\n", nbComp);

        for (int k = 1; k <= nbComp; ++k) {
            printf("Composante %d : ", k);
            for (int v = 0; v < N; ++v) {
                if (compSommet[v] == k) {
                    if (v < nF)
                        printf("P%d ", v + 1);
                    else
                        printf("C%d ", v - nF + 1);
                }
            }
            printf("\n");
        }
    }

    return nbComp;
}

void rendre_connexe(int nF, int nC,
                    int base[nF][nC],
                    int cout[nF][nC])
{
    int N = nF + nC;
    int compSommet[N];

    int nbComp = test_connexite(nF, nC, base, compSommet);

    while (nbComp > 1) {
        int bestI = -1;
        int bestJ = -1;
        int bestCout = 0;
        int first = 1;

        /* Chercher l'arête non basique de plus faible coût
           reliant deux composantes différentes. */
        for (int i = 0; i < nF; ++i) {
            for (int j = 0; j < nC; ++j) {
                if (!base[i][j]) {
                    int u = i;       /* sommet Pi */
                    int v = nF + j;  /* sommet Cj */

                    if (compSommet[u] != compSommet[v]) {
                        int c = cout[i][j];

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

        if (bestI == -1) {
            /* Normalement impossible pour un tableau de coûts complet */
            printf("Impossible de rendre la proposition connexe : aucune arête disponible.\n");
            return;
        }

        /* Ajout de l'arête à la base (quantité 0 si tu gères les x[i][j]) */
        base[bestI][bestJ] = 1;
        /* Si tu as une matrice x[][] :
           x[bestI][bestJ] = 0; */

        printf("Ajout de l'arête (P%d, C%d) de coût %d pour connecter deux composantes.\n",
               bestI + 1, bestJ + 1, bestCout);

        /* Recalcul des composantes après ajout */
        nbComp = test_connexite(nF, nC, base, compSommet);
    }

    printf("La proposition est maintenant connexe.\n");
}
