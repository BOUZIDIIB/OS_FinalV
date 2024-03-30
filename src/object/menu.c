#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "libOS.h"

extern Partition partition; 
extern char* partitionName;
extern File currentFile;
extern bool isSet;

#define MAX_PARTITION_NAME_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_BUFFER_LENGTH 10000


#define CHARGER 1
#define FORMATER 2

void partitionOperation() {
    int choix;
    char userEntry[MAX_PARTITION_NAME_LENGTH];

    printf("Entrez le nom de la partition : ");
    scanf("%s", userEntry);
    userEntry[strcspn(userEntry, "\n")] = '\0'; // Supprimer le retour à la ligne si présent

    if (myFormatExists(userEntry)) {
        printf("Choisissez l'opération :\n");
        printf("%d. Charger\n", CHARGER);
        printf("%d. Formater\n", FORMATER);

        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        // Vider le tampon d'entrée standard
        while(getchar() != '\n');

        partitionName = strdup(userEntry); // Sauvegarder le nom de la partition

        switch (choix) {
            case CHARGER:
                printf("Vous avez choisi de charger la partition %s.\n", userEntry);
                loadPartition(&partition, partitionName);
                isSet = true;
                printf("Chargement réussi !\n");
                break;
            case FORMATER:
                partitionName = userEntry;
                int result = myFormat(&partition, partitionName);
                isSet = false;
                printf("Formatage de la partition : %d\n", result);
                break;
            default:
                printf("Choix invalide.\n");
        }
    } else {
        partitionName = userEntry;
        int result = myFormat(&partition, partitionName);
        printf("Création de la partition réussi : %d\n", result);
        
    }
}

void editFile() {
    char buffer[MAX_BUFFER_LENGTH];

    printf("Entrez le nouveau contenu du fichier (terminer avec Ctrl + D sur Unix ou Ctrl + Z sur Windows):\n");
    if (fgets(buffer, MAX_BUFFER_LENGTH, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne

        setFileContent(&currentFile, buffer);

        myWrite(&partition, partitionName, &currentFile);

        printf("Le nouveau contenu du fichier est :\n%s\n", buffer);
    } else {
        printf("Erreur lors de la lecture de l'entrée.\n");
    }
}

void fileOperation() {
    char userEntry[MAX_FILENAME_LENGTH];
    printf("Entrez le nom du fichier à ouvrir : ");
    scanf("%s", userEntry);
    userEntry[strcspn(userEntry, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne
    
    int res = exists(&partition, userEntry);
    if (res) {
        printf("Le fichier est ouvert\n");
        currentFile = myOpen(&partition, partitionName, userEntry);
        while (1) {
            int choix;
            printf("\nMenu :\n");
            printf("1. Information sur le fichier\n");
            printf("2. Supprimer le fichier\n");
            printf("3. Modifier le fichier\n");
            printf("4. Quitter\n");
            printf("Entrez votre choix : ");
            scanf("%d", &choix);
            switch (choix) {
                case 1:
                    printf("Informations sur le fichier :\n");
                    displayFile(&currentFile);
                    break;
                case 2:
                    printf("Supprimer le fichier\n");
                    myRemove(&partition, partitionName, currentFile.fileName);    
                    return;
                    break;
                case 3:
                    printf("Modifier le fichier\n");
                    displayFile(&currentFile);
                    editFile();                    
                    break;
                case 4:
                    printf("Quitter\n");
                    return;
                default:
                    printf("Choix invalide.\n");
            }
        }
    } else {
        printf("Création du fichier dans la partition.\n");
        currentFile = myOpen(&partition, partitionName, userEntry);
        printf("Création du fichier réussie!\n");    
    }


}


