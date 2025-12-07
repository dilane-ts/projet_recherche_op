#ifndef __FILE_H__
#define __FILE_H__

typedef struct list_maillon {
    int val;
    struct list_maillon * succ;
} ListeMaillon;

typedef struct {
    ListeMaillon * tete;
    ListeMaillon * queue;
} File;

File * initialiser_file();
void enfiler(File * file, int val);
int defiler(File * file);
int est_vide(File * file);
int exister(File * file, int val);
void afficher_file(File * file);
void libererFile(File * file);

#endif