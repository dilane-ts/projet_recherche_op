#include <stdio.h>
#include <stdlib.h>
#include <file.h>

File * initialiser_file() {
    File * file  = (File*)malloc(sizeof(File));
    if(file == NULL) {
        fprintf(stderr, "Erreur d'allocation de la file.\n");
        return NULL;
    }
    file->queue = NULL;
    file->tete = NULL;
    return file;
}

void enfiler(File * file, int val) {
    ListeMaillon * nouveau = (ListeMaillon*)malloc(sizeof(ListeMaillon));
    if(nouveau == NULL) {
        fprintf(stderr, "Erreur d'allocation d'un maillon d'une file.\n");
        return;
    }

    nouveau->val = val;
    nouveau->succ = NULL;

    if(file->queue == NULL && file->tete == NULL) {
        file->queue = nouveau;
        file->tete = file->queue;
    } else {
        file->queue->succ = nouveau;
        file->queue = nouveau;
    }
}


int defiler(File * file) {
    if (file->tete == NULL) {
        fprintf(stderr, "Erreur : Tentative de défiler sur une file vide.\n");
        return -1;
    }
    ListeMaillon * temp = file->tete;

    if(file->tete == file->queue) {
        file->queue = NULL;
        file->tete = NULL;
    } else {
        file->tete = temp->succ;
    }

    int val = temp->val;
    free(temp);
    return val;
    
}

int est_vide(File * file) {
    return file->tete == NULL;
}

int exister(File * file, int val) {
    ListeMaillon * temp = file->tete;
    while(temp != NULL) {
        if(temp->val == val) return 1;
        temp = temp->succ;
    }

    return 0;
}

void afficher_file(File * file) {
    ListeMaillon * temp = file->tete;
    printf("[");
    while(temp != NULL) {
        printf("%d, ", temp->val);
        temp = temp->succ;
    }
    printf("]\n");
}

void libererFile(File * file) {
    while(!est_vide(file)) 
        defiler(file);
    
    
}