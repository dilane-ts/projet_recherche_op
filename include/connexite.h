#ifndef CONNEXITE_H
#define CONNEXITE_H


int test_connexite(int nF, int nC,
                   int base[nF][nC],
                   int compSommet[nF + nC]);

void rendre_connexe(int nF, int nC,
                    int base[nF][nC],
                    int cout[nF][nC]);

#endif 
