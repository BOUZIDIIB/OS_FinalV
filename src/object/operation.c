#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "linkedList.h"

#define ROW_SIZE 10



/**
 * La fonction `saveInPartition` enregistre une structure de partition, un tableau FAT et des données
 * de bloc dans un fichier spécifié par `partitionName`.
 * 
 * @param partition Le paramètre `partition` est une structure représentant une partition dans un
 * système de fichiers. Il contient des informations telles que le prochain bloc libre, le nombre de
 * blocs libres, la table d'allocation de fichiers (FAT) et les blocs de données. La fonction
 * `saveInPartition` est chargée de sauvegarder cette partition dans un fichier
 * @param partitionName Le paramètre `partitionName` est un pointeur vers un tableau de caractères qui
 * représente le nom du fichier de partition dans lequel les données de partition seront enregistrées.
 */
void saveInPartition(Partition partition, char *partitionName) {
    FILE *partition_file = fopen(partitionName, "rb+");

    partition.next_free_block = findFirstFreeBlock(&partition);
    partition.free_blocks = calculateEmptyBlocks(&partition);
    if (!partition_file) {
        perror("Création de la nouvelle partition");
        partition_file = fopen(partitionName, "wb+");;
    }

    // Écrit la partition dans le fichier
    if (fwrite(&partition, sizeof(Partition), 1, partition_file) != 1) {
        perror("Erreur: Impossible d'écrire la partition dans le fichier");
        fclose(partition_file);
        exit(EXIT_FAILURE);
    }

    // Écrit la FAT dans le fichier
    fseek(partition_file, sizeof(Partition), SEEK_SET); // Déplace le curseur après la partition
    if (fwrite(partition.fat, sizeof(bool), FAT_SIZE, partition_file) != FAT_SIZE) {
        perror("Erreur: Impossible d'écrire la FAT dans le fichier");
        fclose(partition_file);
        exit(EXIT_FAILURE);
    }

    // Écrit les données des blocs dans le fichier
    for (int i = 0; i < PARTITION_SIZE; i++) {
        if (fwrite(partition.data[i], sizeof(char), BLOCK_SIZE, partition_file) != BLOCK_SIZE) {
            perror("Erreur: Impossible d'écrire les données des blocs dans le fichier");
            fclose(partition_file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(partition_file);
}

/**
 * La fonction `loadPartition` lit les informations de partition, FAT et les données de bloc d'un
 * fichier dans une structure `Partition`.
 * 
 * @param partition Le paramètre `partition` est un pointeur vers une structure `Partition`. Cette
 * structure contient  des informations sur une partition de disque, telles que des
 * métadonnées, une table d'allocation de fichiers (FAT) et des blocs de données. La fonction
 * `loadPartition` lit ces informations à partir d'un fichier spécifié par `partitionName` et pop
 * @param partitionName Le paramètre `partitionName` est un pointeur vers un tableau de caractères qui
 * représente le nom du fichier contenant les données de partition qui doivent être chargées.
 */
void loadPartition(Partition *partition, char *partitionName) {
    FILE *partition_file = fopen(partitionName, "rb");
    if (!partition_file) {
        perror("Erreur: Impossible d'ouvrir le fichier de partition en mode lecture");
        exit(EXIT_FAILURE);
    }

    // Le fichier de partition existe, on le lit
    if (fread(partition, sizeof(Partition), 1, partition_file) != 1) {
        perror("Erreur lors de la lecture de la partition depuis le fichier");
        fclose(partition_file);
        exit(EXIT_FAILURE);
    }

    fseek(partition_file, sizeof(Partition), SEEK_SET); // Déplace le curseur après la partition
    if (fread(partition->fat, sizeof(bool), FAT_SIZE, partition_file) != FAT_SIZE) {
        perror("Erreur lors de la lecture de la FAT depuis le fichier");
        fclose(partition_file);
        exit(EXIT_FAILURE);
    }

    // Lit les données des blocs depuis le fichier
    for (int i = 0; i < PARTITION_SIZE; i++) {
        if (fread(partition->data[i], sizeof(char), BLOCK_SIZE, partition_file) != BLOCK_SIZE) {
            perror("Erreur lors de la lecture des données des blocs depuis le fichier");
            fclose(partition_file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(partition_file);
}




/**
 * La fonction `create_or_load_partition` vérifie si un fichier de partition existe, crée une nouvelle
 * partition si ce n'est pas le cas, ou charge la partition existante si c'est le cas.
 * 
 * @param partitionName Le paramètre `partitionName` est un pointeur vers un tableau de caractères qui
 * représente le nom du fichier de partition à créer ou à charger.
 * 
 * @return La fonction `create_or_load_partition` renvoie une structure `Partition`, soit une partition
 * nouvellement créée, soit une partition chargée à partir d'un fichier existant.
 */
Partition create_or_load_partition(char *partitionName) {
    FILE *partition_file = fopen(partitionName, "rb+");
    bool exists = partition_file != NULL; // Modifier la vérification de l'existence du fichier
    if (partition_file != NULL) fclose(partition_file); // Fermer le fichier s'il existe

    Partition partition;

    if (!exists) {
        // Le fichier de partition n'existe pas, on crée une nouvelle partition
        printf("CREATION DU FICHIER DE PARTITION\n");
        partition = create_new_partition(partitionName);
    } else {
        printf("LECTURE À PARTIR DU FICHIER DE PARTITION\n");
        loadPartition(&partition, partitionName);
    }

    return partition;
}

/**
 * La fonction crée une nouvelle partition en l'initialisant, en l'enregistrant dans un fichier et en
 * renvoyant la partition.
 * 
 * @param partitionName Le paramètre `partitionName` est un pointeur vers un tableau de caractères
 * (chaîne) qui représente le nom de la partition à créer.
 * 
 * @return La fonction `create_new_partition` renvoie une structure `Partition` après avoir créé un
 * nouveau fichier de partition, initialisé la partition, enregistré les données dans la partition et
 * fermé le fichier.
 */
Partition create_new_partition(char* partitionName){
    Partition partition;
    FILE* partition_file = fopen(partitionName, "wb");
    if (!partition_file) {
        perror("Erreur: Impossible de créer la partition");
        exit(EXIT_FAILURE);
    }
    initPartition(&partition);
    saveInPartition(partition, partitionName);
    fclose(partition_file); 
    return partition; 
}

/**
 * La fonction `initPartition` initialise une structure de partition en définissant les entrées FAT sur
 * 0, en initialisant les informations de fichier pour chaque fichier et en définissant le nombre de
 * blocs libres et l'index du prochain bloc libre.
 * 
 * @param partition La fonction `initPartition` initialise une structure de partition avec les
 * paramètres suivants :
 */
void initPartition(Partition *partition) {
    memset(partition->fat, 0, sizeof(bool) * FAT_SIZE);    
    for (int i = 0; i < FAT_SIZE; ++i) {
        initFileInfo(&partition->files[i], " ", 0, true);
    }
    partition->free_blocks = FAT_SIZE;
    partition->next_free_block = 0;
}


/**
 * La fonction `printPartitionData` imprime des informations sur les fichiers d'une partition.
 * 
 * @param partition La fonction `printPartitionData` prend une structure `Partition` comme paramètre.
 * La structure ` Partition ` contient un tableau de structures ` Fichier ` représentant
 * les fichiers de la partition. La fonction parcourt les fichiers de la partition et imprime des
 * informations sur chaque fichier telles que son nom, sa taille,
 */
void printPartitionData(Partition partition) {
    printf("**********************************\n");
    printf("\n\tDonnées de la parition\n\n");
    for (int i = 0; i < FAT_SIZE; i++){
        if(!partition.files[i].is_free){
        printf("Bloc %d - File Name: %s, Total File Size: %d\n", i + 1,
                   partition.files[i].name, partition.files[i].size);
        }
    }
    printf("**********************************\n");
}


/**
 * La fonction `initFileInfo` initialise une structure `FileInfo` avec le nom, la taille et l'état
 * libre fournis.
 * 
 * @param fileInfo `fileInfo` est un pointeur vers une structure `FileInfo`.
 * @param name Une chaîne représentant le nom du fichier.
 * @param size Le paramètre `size` dans la fonction `initFileInfo` représente la taille du fichier en
 * octets. Il s'agit d'une valeur entière qui indique la taille du fichier en cours d'initialisation
 * dans la structure `FileInfo`.
 * @param is_free Le paramètre `is_free` dans la fonction `initFileInfo` est une valeur booléenne qui
 * indique si le fichier est libre ou non. Si ` is_free ` est vrai, cela signifie que le fichier est
 * disponible ou n'est pas utilisé. Si `is_free` est faux, cela signifie que le fichier est
 * actuellement
 */
void initFileInfo(FileInfo *fileInfo, const char *name, int size, bool is_free) {
    strncpy(fileInfo->name, name, sizeof(fileInfo->name));
    fileInfo->size = size;
    fileInfo->is_free = is_free;
}



/**
 * La fonction `writeToFile` permet d'écrire des données dans un fichier d'une partition, gérant
 * l'allocation des fichiers et mettant à jour les structures du système de fichiers en conséquence.
 * 
 * @param partition Un pointeur vers une structure `Partition` contenant des informations sur une
 *                  partition de stockage, telles que la table d'allocation de fichiers (FAT),
 *                  les blocs de données de fichier et les métadonnées de fichier.
 * @param partition_name Un pointeur vers un tableau de caractères représentant le nom de la
 *                       partition sur laquelle le fichier sera écrit. Il est utilisé pour identifier
 *                       la partition spécifique dans laquelle le fichier sera stocké.
 * @param filename Le nom du fichier à écrire sur la partition, spécifié en tant que chaîne de
 *                 caractères.
 * @param data Un pointeur vers les données que vous souhaitez écrire dans le fichier de la partition.
 *             Ces données seront copiées dans le fichier.
 * @param size La taille des données à écrire dans le fichier, en octets.
 * 
 * @return Retourne 0 si l'opération d'écriture du fichier réussit. Retourne -1 en cas d'erreur
 *         lors du processus, telles qu'un fichier du même nom déjà existant, un manque d'espace
 *         libre pour le fichier, des blocs libres insuffisants, etc.
 */int writeToFile(Partition *partition, char* partition_name ,char *filename, void *data, int size) {
    // Vérifie si un fichier avec le même nom existe déjà dans la partition
    if(exists(partition, filename)){
        printf("Le fichier existe déjà");
        return 1; 
    }

    // Si aucun fichier avec le même nom n'existe, procéder à l'écriture du fichier
    int file_index = -1;
    for (int i = 0; i < FAT_SIZE; i++) {
        if (partition->files[i].size == 0) {
            file_index = i;
            break;
        }
    }
    if (file_index == -1) {
        printf("Erreur: Aucun emplacement libre trouvé pour le fichier.\n");
        return -1;
    }

    // Calcul du nombre de blocs nécessaires pour stocker les données du fichier
    int blocks_needed = (int)size / BLOCK_SIZE;
    blocks_needed = blocks_needed>0 ?blocks_needed :1;
    int blocks_allocated = 0; 
    // Recherche des blocs libres
    LinkedList list; 
    initList(&list); 
    for (int i = 0; i < FAT_SIZE && blocks_allocated < blocks_needed; i++) {
        if (!partition->fat[i]) {
            partition->fat[i] = true;
            blocks_allocated++;
            append(&list, i, INT_TYPE);
        }
    }
    
    // Vérifie si suffisamment de blocs libres ont été trouvés
    if (blocks_allocated < blocks_needed) {
        printf("Erreur: Pas assez de blocs libres disponibles.\n");
        return -1;
    }

    // Copie des données dans les blocs alloués
    int remaining_size = size;
    int index;
    int offset = 0;
    while (list !=NULL) {
        index = list->value.intValue;
        int bytes_to_copy = remaining_size < BLOCK_SIZE ? remaining_size : BLOCK_SIZE;
        memcpy(partition->data[index], (char *)data + offset, bytes_to_copy);
        memcpy(partition->files[index].name, filename, sizeof(partition->files[index].name));
        partition->files[index].size = (strlen(data)+1)*sizeof(char);
        partition->files[index].is_free = false;
        remaining_size -= bytes_to_copy;
        offset += bytes_to_copy;
        list = list->next;
    }

    saveInPartition(*partition, partition_name);
    loadPartition(partition, partition_name); 

    return 0; 
}


/**
 * La fonction calcule le nombre de blocs vides dans une partition donnée.
 * 
 * @param partition La fonction `calculateEmptyBlocks` prend un pointeur vers une structure `Partition`
 * comme paramètre. La structure ` Partition ` contient  des informations sur une partition
 * de disque, telles que la table d'allocation de fichiers (FAT) et d'autres données pertinentes. La
 * fonction parcourt les entrées FAT et compte le nombre
 * 
 * @return La fonction `calculateEmptyBlocks` renvoie le nombre de blocs vides dans la FAT (File
 * Allocation Table) de la structure `Partition` donnée.
 */
int calculateEmptyBlocks(Partition *partition) {
    int empty_blocks = 0;
    for (int i = 0; i < FAT_SIZE; ++i) {
        if (!partition->fat[i]) {
            empty_blocks++;
        }
    }
    return empty_blocks;
}



/**
 * La fonction `findIndexesByName` recherche les fichiers avec un nom spécifique dans une partition et
 * renvoie une liste chaînée de leurs index.
 * 
 * @param partition Un pointeur vers une structure `Partition` contenant des informations sur une
 * partition du système de fichiers, telles que des entrées de fichier et leurs blocs de données associés.
 * @param filename Un tableau de caractères représentant le nom du fichier recherché dans la partition.
 * 
 * @return Une `LinkedList` contenant les index des blocs dans la FAT (File Allocation Table) qui
 * correspondent à un fichier avec le nom spécifié (`filename`) dans la partition donnée. Chaque élément
 * de la liste représente l'index d'un bloc de fichier.
 */
LinkedList findIndexesByName(Partition *partition, char *filename) {
    LinkedList indexes = NULL;
    for (int i = 0; i < FAT_SIZE; ++i) {
          if (strcmp(partition->files[i].name, filename) == 0) {
              // Le fichier avec le même nom est trouvé, ajout des index des blocs dans la liste
                  append(&indexes, i, INT_TYPE);
          }
    }
    return indexes;
}

/**
 * La fonction `updateFileContent` met à jour le contenu d'un fichier dans une partition en supprimant
 * le fichier existant et en y écrivant de nouvelles données.
 * 
 * @param partition Le paramètre ` partition ` est un pointeur vers une structure ` Partition `, qui
 * contient  des informations sur une partition de stockage sur un système. Cette structure
 * peut inclure des détails tels que la taille de la partition, l'espace utilisé, l'espace disponible,
 * le type de système de fichiers, etc.
 * @param partition_name Le paramètre `partition_name` est une chaîne qui représente le nom de la
 * partition où se trouve le fichier ou où le contenu du fichier sera mis à jour.
 * @param filename Le paramètre `filename` dans la fonction `updateFileContent` fait référence au nom
 * du fichier dont vous souhaitez mettre à jour le contenu dans la partition spécifiée.
 * @param data Le paramètre `data` dans la fonction `updateFileContent` représente le contenu que vous
 * souhaitez écrire dans le fichier spécifié. Il s'agit d'un pointeur vers l'emplacement mémoire où les
 * données sont stockées, et ` taille ` indique la taille des données en octets que vous souhaitez
 * écrire dans le fichier.
 * @param size Le paramètre `size` dans la fonction `updateFileContent` représente la taille des
 * données que vous souhaitez écrire dans le fichier. Il indique le nombre d'octets dans les données
 * que vous transmettez à la fonction. Ce paramètre est important pour écrire correctement les données
 * dans le fichier sans aucune mémoire
 * 
 * @return La fonction `updateFileContent` renvoie une valeur entière de 0.
 */
int updateFileContent(Partition *partition, char *partition_name, char *filename, void *data, int size) {
    deleteFile(partition, partition_name, filename);    
    writeToFile(partition, partition_name, filename, data, strlen(data));
    return 0; 

}


/**
 * La fonction `findFirstFreeBlock` recherche le premier bloc disponible dans la table d'allocation de
 * fichiers d'une partition.
 * 
 * @param partition La partition est une structure de données représentant une section d'un disque qui
 * a été divisée à des fins de stockage. Elle contient la table d'allocation de fichiers (FAT) et 
 * des blocs de données.
 * 
 * @return La fonction `findFirstFreeBlock` renvoie l'index du premier bloc libre dans la FAT (File
 * Allocation Table) de la structure `Partition` donnée. Si un bloc libre est trouvé, son index est
 * renvoyé. Si aucun bloc libre n'est trouvé, la fonction renvoie -1.
 */
int findFirstFreeBlock(Partition *partition) {
    for (int i = 0; i < FAT_SIZE; ++i) {
        if (!partition->fat[i]) {
            return i;
        }
    }
    return -1;
}


/**
 * La fonction `readWholeFile` lit le contenu d'un fichier stocké en blocs sur une partition en
 * fonction des index fournis et renvoie le contenu concaténé sous forme de chaîne.
 * 
 * @param partition Une structure `Partition` contenant des blocs de données.
 * @param indexes Le paramètre `indexes` dans la fonction `readWholeFile` est une liste chaînée
 * contenant les index des blocs qui stockent le contenu d'un fichier dans une partition. Chaque nœud
 * de la liste chaînée contient une valeur entière représentant l'index d'un bloc dans le tableau de
 * données de la partition.
 * 
 * @return La fonction `readWholeFile` renvoie un pointeur vers un tableau de caractères qui contient
 * le contenu concaténé de tous les blocs référencés par les index dans la partition donnée.
 */

char* readWholeFile(Partition *partition, LinkedList indexes) {
    LinkedList result = NULL;
    LinkedList current = NULL;

    // Calcul du nombre total de blocs dans la liste d'index
    int totalBlocks = len(indexes);

    // Allocation de mémoire pour stocker le contenu de tous les blocs
    char *fileContent = (char *)malloc((totalBlocks * BLOCK_SIZE + 1) * sizeof(char));
    if (!fileContent) {
        perror("Erreur: Impossible d'allouer de la mémoire pour le contenu du fichier");
        exit(EXIT_FAILURE);
    }

    char *contentBuffer = (char *)malloc(BLOCK_SIZE * sizeof(char));
    if (!contentBuffer) {
        perror("Erreur: Impossible d'allouer de la mémoire pour le tampon de contenu");
        free(fileContent);
        exit(EXIT_FAILURE);
    }

    current = indexes;
    int offset = 0;
    while (current != NULL) {
        int index = current->value.intValue;
        memcpy(contentBuffer, partition->data[index], BLOCK_SIZE);
        strncpy(fileContent + offset, contentBuffer, BLOCK_SIZE);
        offset += BLOCK_SIZE;
        current = current->next;
    }
    free(contentBuffer);

    return fileContent;
}

/**
 * La fonction `readFile` lit le contenu d'un fichier à partir d'une partition donnée en recherchant
 * les index des fichiers puis en lisant l'intégralité du fichier.
 * 
 * @param partition Le paramètre ` partition ` fait  référence à une structure de données
 * représentant une partition sur un périphérique de stockage, tel qu'un disque dur. Il peut contenir
 * des informations sur les fichiers et répertoires stockés dans cette partition.
 * @param fileName Le paramètre `fileName` est un pointeur vers un tableau de caractères qui représente
 * le nom du fichier que vous souhaitez lire à partir de la `Partition` spécifiée.
 * 
 * @return La fonction `readFile` renvoie un pointeur vers un tableau de caractères (char*) qui
 * contient le contenu du fichier spécifié par le paramètre `fileName` dans la `Partition` spécifiée
 * par le paramètre `partition`.
 */
char* readFile(Partition partition, char* fileName){
    LinkedList fileIndex = findIndexesByName(&partition, fileName);
    return readWholeFile(&partition, fileIndex); 
}


/**
 * La fonction `deleteFile` supprime un fichier d'une partition en marquant ses blocs comme libres dans
 * la table d'allocation de fichiers (FAT) et en réinitialisant les informations sur le fichier dans la
 * partition.
 * 
 * @param partition Le paramètre ` partition ` est un pointeur vers une structure ` Partition `, qui
 * contient  des informations sur une partition de disque, telles que la table d'allocation
 * de fichiers (FAT), des blocs de données et des informations sur les fichiers.
 * @param partitionName Le paramètre `partitionName` est une chaîne qui représente le nom de la
 * partition à partir de laquelle le fichier doit être supprimé. Il est utilisé pour identifier la
 * partition spécifique dans laquelle le fichier existe.
 * @param filename Le paramètre `filename` dans la fonction `deleteFile` est le nom du fichier que vous
 * souhaitez supprimer de la partition spécifiée.
 * 
 * @return La fonction `deleteFile` renvoie une valeur entière. Si la suppression du fichier réussit,
 * elle renvoie 0. S'il n'y a aucun fichier portant le nom spécifié dans la partition, elle renvoie -1
 * pour indiquer que le fichier n'existe pas.
 */
int deleteFile(Partition *partition, char* partitionName, char* filename) {
    // Rechercher les index des blocs appartenant au fichier
    LinkedList fileIndexes = findIndexesByName(partition, filename);
    if (fileIndexes == NULL) {
        printf("Aucun fichier enregistré à ce nom.\n");
        return -1; // Le fichier n'existe pas
    }

    // Parcourir la liste des index des blocs et les marquer comme libres
    while (fileIndexes != NULL) {
        int blockIndex = fileIndexes->value.intValue;
        partition->fat[blockIndex] = false; // Marquer le bloc comme libre dans la FAT
        // Effacer le contenu du bloc si nécessaire
        memset(partition->data[blockIndex], 0, BLOCK_SIZE); // Remplir le bloc avec des zéros
        initFileInfo(&(partition->files[blockIndex]), "", 0, true);
        partition->fat[blockIndex] = false;
        fileIndexes = fileIndexes->next;
    }

    // Mise à jour des informations sur la partition
    saveInPartition(*partition, partitionName);

    // Libérer la mémoire utilisée par la liste des index des blocs du fichier
    clear(&fileIndexes);

    return 0; // Suppression du fichier réussie
}

/**
 * La fonction `fileExists` vérifie si un fichier avec un nom donné existe dans une partition en recherchant
 * dans la table d'allocation de fichiers (FAT).
 * 
 * @param partition Un pointeur vers une structure `Partition` contenant des informations sur une partition
 *                  du système de fichiers, telles que la table d'allocation de fichiers (FAT), des
 *                  informations sur les fichiers et leur données.
    * @param filename Le nom du fichier à rechercher, spécifié en tant que tableau de caractères.
 * 
 * @return Retourne une valeur entière : 1 (vrai) si le fichier avec le nom donné est trouvé dans la
 *         partition, 0 (faux) sinon.
 */
int exists(Partition *partition,char *filename) {
    return findIndexesByName(partition, filename) != NULL;
}

/**
 * La fonction `printPartitionState` imprime l'état d'une partition incluant le nombre de blocs libres,
 * le prochain bloc libre et l'état de chaque bloc dans la table d'allocation de fichiers.
 * 
 * @param partition La fonction `printPartitionState` est conçue pour imprimer l'état d'une partition,
 * y compris le nombre de blocs libres, l'index du prochain bloc libre et l'état de chaque bloc dans la
 * table d'allocation de fichiers (FAT) de la partition.
 */
void printPartitionState(Partition* partition) {
    printf("État de la partition :\n");
    printf("----------------------\n");
    printf("Blocs libres : %d\n", partition->free_blocks);
    printf("Bloc suivant libre : %d\n", partition->next_free_block);

    for(int i=0; i<FAT_SIZE; i++){
        if(i%10==0){
            printf("\n");
            if(!partition->files[i].is_free){
                printf("[Taken] ");
            } else {
                printf("[-----] ");
            }
        }else{
            if(!partition->files[i].is_free){
                printf("[Taken] ");
            } else {
                printf("[-----] ");
            }
        }

    }
    printf("\n");
}