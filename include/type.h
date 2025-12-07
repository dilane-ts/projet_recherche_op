#ifndef __TYPE_H__
#define __TYPE_H__

typedef struct {
    int ** constraintes;
    int ** tab;
    int * provisions;
    int * commandes;
    int nbre_provisions;
    int nbre_commandes;
} Transport;

Transport * creerTransport(int nbre_provisions, int nbre_commandes);
void libererTransport(Transport * t);

void afficher_cout(Transport * t);
void afficher_proposition_transport(Transport * t);

void reiniatitialiser_transport(Transport * t);
int cout_transport(Transport * t);

#endif