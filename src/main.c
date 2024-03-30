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
        char choix;
        printf("\nMenu:\n");
        printf("1. Chargement ou création de partition\n");
        if(isSet){
            printf("2. Opérations sur fichier\n");
            printf("3. Afficher l'état de la partition\n");
            printf("4. Quitter\n");
        }else{
            printf("2. Quitter\n");
        }
        printf("Choix: ");
        scanf(" %c", &choix);
        if(isSet){
            switch (choix) {
                case '1':
                    printf("*****************************************************");
                    printf("\nChargement ou création de partition\n");
                    partitionOperation();
                    printf("*****************************************************");

                    break;
                case '2':
                    printf("*****************************************************");
                    printf("\nOpérations sur fichier\n");
                    fileOperation();
                    printf("*****************************************************");


                    break;
                case '3':
                    printf("*****************************************************");
                    printf("\nEtat de la partition %s\n", partitionName);
                    // printPartitionState(&partition);
                    printPartitionData(partition);
                    printf("*****************************************************");

                    break;
                case '4':
                    end = true; 
                    printf("Au revoir !\n");
                    exit(0);
                default:
                    printf("Choix invalide. Veuillez choisir une option valide.\n");
            }
        }else{
            switch (choix) {
                case '1':
                    printf("*****************************************************");
                    printf("\nChargement ou création de partition\n");
                    partitionOperation();
                    printf("*****************************************************");
                    break;
                case '2':
                    end = true; 
                    printf("Au revoir !\n");
                    exit(0);
                default:
                    printf("Choix invalide. Veuillez choisir une option valide.\n");
            }
        }
    }

    return 0;
}

