#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "libOS.h"
#include "menu.h"

extern Partition partition; 
extern char* partitionName;
extern File currentFile;
extern bool isSet;

#define MAX_PARTITION_NAME_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_BUFFER_LENGTH 10000




/**
 * La fonction `partitionOperation` permet à l'utilisateur de choisir entre charger, formater une partition ou de
 * quitter le menu d'opérations sur partition.
 */
void partitionOperation() {
    int choix;
    char userEntry[MAX_PARTITION_NAME_LENGTH];

    printf("\nEntrez le nom de la partition : ");

    scanf("%s", userEntry);
    userEntry[strcspn(userEntry, "\n")] = '\0';
    if (myFormatExists(userEntry)) {
        bool end = false; 
        while(!end){
            printf("\n\n???????????????????????????????????????\n");
            printf("\n\tChoisissez l'opération :\n\n");
            printf("%d. Charger\n", CHARGER);
            printf("%d. Formater\n", FORMATER);
            printf("%d. Quitter\n", QUIT_FORMAT);
            printf("\n\n???????????????????????????????????????\n");


            printf("Entrez votre choix : ");
            if (scanf("%d", &choix) != 1) {
                printf("Erreur : Veuillez saisir un entier valide.\n");
                while (getchar() != '\n');
                continue;        
            }
            partitionName = strdup(userEntry); 
            printf("***************************************\n\n");
            switch (choix) {
                case CHARGER:
                    printf("Vous avez choisi de charger la partition %s.\n", userEntry);
                    loadPartition(&partition, partitionName);
                    isSet = true;
                    end = true;
                    printf("Chargement réussi !\n");
                    break;
                case FORMATER:
                    partitionName = userEntry;
                    int result = myFormat(&partition, partitionName);
                    isSet = false;
                    end= true;
                    printf("Formatage de la partition : %d\n", result);
                    break;
                case QUIT_FORMAT:
                    end = true;
                default:
                    printf("Choix invalide.\n");
            }
            printf("\n\n***************************************\n");
        }
    } else {
        partitionName = userEntry;
        int result = myFormat(&partition, partitionName);
        printf("Création de la partition réussi : %d\n", result);
        
    }
}

/**
 * La fonction `editFile` lit les entrées de l'utilisateur pour mettre à jour le contenu d'un fichier
 * et écrit les modifications sur une partition.
 */
void editFile() {
    char buffer[MAX_BUFFER_LENGTH];

    printf("Entrez le nouveau contenu du fichier (terminer avec une ligne vide):\n");
    if (scanf(" %[^\n]", buffer) == 1) {
        printf("Le nouveau contenu du fichier est :\n%s\n", buffer);
        setFileContent(&currentFile, buffer);
        myWrite(&partition, partitionName, &currentFile);
    } else {
        printf("Erreur lors de la lecture de l'entrée.\n");
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * La fonction ` fileOperation ` permet à l'utilisateur d'interagir avec les fichiers d'une partition,
 * en fournissant des options pour ouvrir, afficher des informations, modifier ou supprimer un fichier.
 * 
 * @return La fonction `fileOperation()` renvoie void.
 */
void fileOperation() {
    char userEntry[MAX_FILENAME_LENGTH];
    printPartitionData(partition);
    printf("\n\nEntrez le nom du fichier à ouvrir : ");
    scanf("%s", userEntry);
    userEntry[strcspn(userEntry, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne
    
    int res = exists(&partition, userEntry);
    if (res) {
        
        printf("Le fichier est ouvert\n");
        currentFile = myOpen(&partition, partitionName, userEntry);
        while (1) {
            int choix;
            printf("\n\n???????????????????????????????????????\n");

            printf("\n\tMenu Fichier %s :\n\n", userEntry);
            printf("%d. Information sur le fichier\n", INFO_FILE);
            printf("%d. Supprimer le fichier\n", REMOVE_FILE);
            printf("%d. Modifier le fichier\n", EDIT_FILE);
            printf("%d. Quitter\n", QUIT_FILE);
            printf("\n\n???????????????????????????????????????\n");
            printf("Entrez votre choix : ");
            if (scanf("%d", &choix) != 1) {
                printf("Erreur : Veuillez saisir un entier valide.\n");
                while (getchar() != '\n');
                continue;        
            }
            printf("\n***************************************n\n");
            switch (choix) {
                case INFO_FILE:
                    printf("Informations sur le fichier :\n");
                    displayFile(&currentFile);
                    break;
                case REMOVE_FILE:
                    printf("Supprimer le fichier\n");
                    myRemove(&partition, partitionName, currentFile.fileName);    
                    return;
                    break;
                case EDIT_FILE:
                    printf("Modifier le fichier\n");
                    displayFile(&currentFile);
                    editFile();                    
                    break;
                case QUIT_FILE:
                    printf("Quitter\n");
                    return;
                default:
                    printf("Choix invalide.\n");
            }
            printf("\n\n***************************************\n");
        }
    } else {
        printf("Création du fichier dans la partition.\n");
        currentFile = myOpen(&partition, partitionName, userEntry);
        printf("Création du fichier réussie!\n");  
        fileOperation();  
    }


}



/**
 * La fonction `openPartitionOperation` prend une entrée de choix et effectue différentes opérations en
 * fonction du choix sélectionné, comme le chargement ou la création d'une partition, les opérations
 * sur les fichiers, l'affichage de l'état de la partition ou la fermeture du programme.
 * 
 * @param choice Le paramètre `choice` dans la fonction `openPartitionOperation` est de type `char`. Il
 * est utilisé pour déterminer quelle opération effectuer en fonction de la saisie de l'utilisateur. La
 * fonction utilise une instruction switch pour exécuter différentes actions en fonction de la valeur
 * de ` choix `.
 * 
 * @return La fonction `openPartitionOperation` renvoie une valeur booléenne indiquant si le programme
 * doit se terminer ou non. Si l'utilisateur choisit de quitter (option QUITTER), la fonction définit
 * la variable `end` sur true et la renvoie. Sinon, il renvoie faux.
 */
bool openPartitionOperation(char choice){
    bool end = false; 
    printf("\n***************************************\n\n");
    switch (choice) {
        case LOAD_FORMAT_PARTITION:
            printf("\nChargement ou création de partition\n");
            partitionOperation();
            break;
        case FILE_OPERATION:
            printf("\nOpérations sur fichier\n");
            fileOperation();
            break;
        case PARTITION_STATE:
            printPartitionState(&partition);
            printPartitionData(partition);
            break;
        case QUIT:
            end = true; 
            printf("Au revoir !\n");
            exit(0);
        default:
            printf("Choix invalide. Veuillez choisir une option valide.\n");
    }
    printf("\n\n***************************************\n");
    return end;
}


/**
 * La fonction `unOpenedParitionOperation` gère différentes opérations en fonction du choix de
 * l'utilisateur, comme charger ou créer une partition, quitter ou afficher un message d'erreur pour
 * des choix non valides.
 * 
 * @param choice Le paramètre ` choix ` dans la fonction ` unOpenedParitionOperation ` est un caractère
 * qui représente la sélection de l'utilisateur pour une opération spécifique. La fonction vérifie la
 * valeur de ` choix ` par rapport à des constantes prédéfinies telles que ` LOAD_FORMAT_PARTITION ` et
 * ` QUIT_UNOPENED ` pour déterminer l'action à effectuer.
 * 
 * @return La fonction `unOpenedParitionOperation` renvoie une valeur booléenne indiquant si le
 * programme doit se terminer ou non. Si le paramètre `choice` correspond à `QUIT_UNOPENED`, la
 * fonction définit `end` sur true et le renvoie. Sinon, il renvoie la valeur initiale de ` end `, qui
 * est fausse.
 */
bool unOpenedParitionOperation(char choice){
    bool end = false;
    printf("***************************************\n\n");
    switch (choice) {
        case LOAD_FORMAT_PARTITION:
            printf("Chargement ou création de partition\n");
            partitionOperation();
            break;
        case QUIT_UNOPENED:
            end = true; 
            printf("Au revoir !\n");
            exit(0);
        default:
            printf("Choix invalide. Veuillez choisir une option valide.\n");
    }
    printf("\n\n***************************************");
    return end;
}