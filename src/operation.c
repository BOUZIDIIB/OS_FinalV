#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "linkedList.h"




void saveInPartition(Partition partition, char *partitionName) {
    FILE *partition_file = fopen(partitionName, "rb+");
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

void initPartition(Partition *partition) {
    memset(partition->fat, 0, sizeof(bool) * FAT_SIZE);    
    for (int i = 0; i < FAT_SIZE; ++i) {
        initFileInfo(&partition->files[i], " ", 0, true);
    }
    partition->free_blocks = FAT_SIZE;
    partition->next_free_block = 0;
}


void printPartitionData(Partition partition) {
    printf("**********************************\n");
    printf("PARTITION INFO\n");
    for (int i = 0; i < FAT_SIZE; i++) {
        if(!partition.files[i].is_free){
        printf("File %d - Name: %s, Size: %d, is_free: %d\n", i + 1,
                   partition.files[i].name, partition.files[i].size,  partition.files[i].is_free);

        }
    }
    printf("**********************************\n");
}


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
 */
int writeToFile(Partition *partition, char* partition_name ,char *filename, void *data, int size) {
    // Vérifie si un fichier avec le même nom existe déjà dans la partition
    for (int i = 0; i < FAT_SIZE; i++) {
        if (strcmp(partition->files[i].name, filename) == 0) {
            printf("Erreur: Un fichier avec le même nom existe déjà dans la partition.\n");
            return -1;
        }
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
    int blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // Recherche d'emplacements libres dans la FAT
    int start_block = -1;
    int blocks_allocated = 0;
    for (int i = 0; i < FAT_SIZE && blocks_allocated < blocks_needed; i++) {
        if (!partition->fat[i]) {
            if (start_block == -1) {
                start_block = i;
            }
            partition->fat[i] = true;
            blocks_allocated++;
        }
    }

    // Vérifie si suffisamment de blocs libres ont été trouvés
    if (blocks_allocated < blocks_needed) {
        printf("Erreur: Pas assez de blocs libres disponibles.\n");
        return -1;
    }

    // Copie des données dans les blocs alloués
    int current_block = start_block;
    int remaining_size = size;
    int offset = 0;
    while (remaining_size > 0) {
        int bytes_to_copy = remaining_size < BLOCK_SIZE ? remaining_size : BLOCK_SIZE;
        memcpy(partition->data[current_block], (char *)data + offset, bytes_to_copy);
        remaining_size -= bytes_to_copy;
        offset += bytes_to_copy;
        current_block++;
    }

    // Mise à jour des informations sur le fichier dans la partition
    strncpy(partition->files[file_index].name, filename, sizeof(partition->files[file_index].name));
    partition->files[file_index].size = size;
    partition->files[file_index].is_free = false; // Marquer le fichier comme occupé

    // Mise à jour des informations sur la partition
    partition->free_blocks -= blocks_needed;
    partition->next_free_block += blocks_needed;

    saveInPartition(*partition, partition_name); 
    return 0; // Écriture du fichier réussie
}


int updateFileContent(Partition *partition, char* partition_name, char *filename, void *data, int size) {
    // Vérifier si un fichier avec le même nom existe déjà dans la partition
    LinkedList fileIndexes = findIndexesByName(partition, filename);
    if (fileIndexes == NULL) {
        printf("Aucun fichier enregistré à ce nom, veuillez créer un fichier.\n");
        return -1;
    }

    // Copier le nouveau contenu dans les blocs correspondants
    int offset = 0;
    Cell *current = fileIndexes;
    while (current != NULL && offset < size) {
        memcpy(partition->data[current->value.intValue], (char *)data + offset, BLOCK_SIZE);
        offset += BLOCK_SIZE;
        current = current->next;
    }

    // Mettre à jour la taille du fichier
    int fileSize = strlen(data)+1; // Calculer le nombre de blocs nécessaires
    strncpy(partition->files[fileIndexes->value.intValue].name, filename, sizeof(partition->files[fileIndexes->value.intValue].name));
    partition->files[fileIndexes->value.intValue].size = fileSize;

    // Libérer la mémoire utilisée par la liste des index des blocs du fichier
    clear(&fileIndexes);

    // Mise à jour des informations sur la partition
    saveInPartition(*partition, partition_name);

    return 0; // Écriture du fichier réussie
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
 * La fonction `readWholeFile` lit les données d'une liste d'index de bloc dans une partition et les
 * concatène dans une liste chaînée représentant le contenu d'un fichier.
 * 
 * @param partition Un pointeur vers une structure `Partition` contenant les blocs de données à lire.
 * @param indexes Une liste chaînée contenant les index des blocs à lire à partir de la partition.
 * Chaque nœud de la liste contient l'index d'un bloc dans le tableau de données de la partition.
 * 
 * @return Une `LinkedList` contenant le contenu du fichier lu à partir de la partition spécifiée,
 * en fonction des index fournis. Chaque élément de la liste contient les données d'un bloc de fichier.
 */

LinkedList readWholeFile(Partition *partition, LinkedList indexes) {
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

    // Lecture des données de chaque bloc et concaténation dans le contenu du fichier
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

    // Transformation du contenu du fichier en une liste chaînée de caractères
    for (int i = 0;  fileContent[i] != '\0'; i++) {
        append(&result, fileContent[i], CHAR_TYPE);
    }
    free(fileContent);

    return result;
}

/**
 * La fonction `readFile` lit le contenu d'un fichier à partir d'une partition donnée en fonction du
 * nom du fichier.
 * 
 * @param partition Un pointeur vers une structure `Partition` représentant la partition à partir
 * de laquelle le fichier doit être lu.
 * @param filename Le nom du fichier à lire.
 * 
 * @return Une `LinkedList` contenant les index des blocs de données correspondant au fichier spécifié
 * par `filename` dans la partition. Chaque élément de la liste contient l'index d'un bloc de données.
 */
LinkedList readFile(Partition partition, char* fileName){
    LinkedList fileIndex = findIndexesByName(&partition, fileName);
    return readWholeFile(&partition, fileIndex); 
}
