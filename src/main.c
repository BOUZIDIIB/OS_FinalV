#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h" 
#include "libOS.h"
#include "menu.h"


Partition partition;
char* partitionName;
File currentFile;
bool isSet;



int main() {
    isSet = false;
    int result;
    bool end= false;
    while (!end) {
        int choix;
        printf("\n\n???????????????????????????????????????\n");

        if(isSet){
            printf("\n\tMenu Partition %s :\n\n", partitionName);
            printf("%d. Chargement ou création de partition\n", LOAD_FORMAT_PARTITION);
            printf("%d. Opérations sur fichier\n", FILE_OPERATION);
            printf("%d. Afficher l'état de la partition\n", PARTITION_STATE);
            printf("%d. Quitter\n", QUIT);
        }else{
            printf("\n\tMenu :\n\n");
            printf("%d. Chargement ou création de partition\n", LOAD_FORMAT_PARTITION);
            printf("%d. Quitter\n", QUIT_UNOPENED);
        }
        printf("\n\n???????????????????????????????????????\n");
        printf("Choix: ");
        if (scanf("%d", &choix) != 1) {
            printf("Erreur : Veuillez saisir un entier valide.\n");
            while (getchar() != '\n');
            continue;        
        }
        if(isSet){
            end = openPartitionOperation(choix);
        }else{
            end = unOpenedParitionOperation(choix);
        }
    }

    return 0;
}

