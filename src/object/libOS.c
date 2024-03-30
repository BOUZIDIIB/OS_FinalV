#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "libOS.h"
#include "operation.h"
#include "linkedList.h"


/**
 * La fonction `createFile` crée un fichier avec un nom de fichier et un contenu donnés, initialisant
 * sa taille en fonction de la longueur du contenu.
 * 
 * @param fileName Le paramètre `fileName` est un tableau de caractères qui représente le nom du
 * fichier à créer.
 * @param content Le paramètre `content` dans la fonction `createFile` représente le contenu qui sera
 * stocké dans le fichier. Il s'agit d'un tableau de caractères (chaîne) qui contient les données à
 * écrire dans le fichier.
 * 
 * @return La fonction `createFile` renvoie une structure `File` avec les champs `fileName`, `content`
 * et `size` initialisés en fonction des paramètres d'entrée `fileName` et `content`.
 */
File createFile(char* fileName, char* content) {
    File file;
    file.fileName = strdup(fileName);
    file.content = strdup(content); 
    file.size = strlen(content)+1; // Initialisation de la taille du fichier à zéro
    return file;
}

/**
 * La fonction `myFormat` initialise une partition, y enregistre les données en utilisant un nom de
 * partition donné et renvoie un indicateur de réussite.
 * 
 * @param partitionName Le paramètre `partitionName` est un pointeur vers un tableau de caractères qui
 * représente le nom de la partition à formater.
 * 
 * @return La fonction `myFormat` renvoie une valeur entière de 1, ce qui indique que l'opération de
 * formatage a réussi.
 */
int myFormat(Partition * partition, char* partitionName) {
    initPartition(partition);
    saveInPartition(*partition, partitionName);
    return 1; // Indiquer que le formatage a été effectué avec succès
}

File myOpen(Partition *partition, char* partitionName, char *fileName) {
    File file;
    file.fileName = fileName;

    if (exists(partition, fileName)) {
        // Si le fichier existe, vous pouvez décider de le lire dans 'file.content'
        // Par exemple, vous pouvez utiliser la fonction 'readFile' pour obtenir le contenu du fichier

        LinkedList content = readFile(*partition, fileName);
        file.size = len(content);

        // Allouez de la mémoire pour 'file.content'
        file.content = (char*)malloc(file.size + 1); // +1 pour le caractère de fin de chaîne '\0'
        if (file.content == NULL) {
            perror("Erreur d'allocation de mémoire");
            exit(EXIT_FAILURE);
        }

        // Copiez le contenu du fichier dans 'file.content'
        int i = 0;
        while (content != NULL) {
            file.content[i] = content->value.charValue;
            content = content->next;
            i++;
        }
        file.content[i] = '\0'; // Ajoutez le caractère de fin de chaîne
    } else {
        // Si le fichier n'existe pas, initialiser 'file.content' à une chaîne vide
        file.content = "";
        file.size = 0;
        writeToFile(partition, partitionName, file.fileName, file.content, file.size ); 
    }
    return file;
}


int myWrite(Partition *partition, char* partitionName,File* f){
    return updateFileContent(partition, partitionName, f->fileName, f->content, f->size);
}   

void setFileContent(File* file, char* data) {
    // Vérifier si le contenu existe déjà
    // if (file->content != NULL) {
    //     // Si oui, libérer la mémoire
    //     free(file->content);
    //     file->content = NULL;
    // }

    // Vérifier si la donnée est vide
    if (data == NULL) {
        file->size = 0;
        return; // Sortir de la fonction si la donnée est vide
    }

    // Allouer de la mémoire pour la nouvelle donnée
    file->size = strlen(data);
    file->content = (char*)malloc((file->size + 1) * sizeof(char)); // +1 pour le caractère nul de fin de chaîne
    if (file->content == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Copier la nouvelle donnée dans le contenu du fichier
    strcpy(file->content, data);
}




File myRead(Partition *partition, char* partitionName, char* fileName) {
    File f;
    f.fileName = strdup(fileName);
    // Récupère le contenu entier du fichier
    LinkedList content = readFile(*partition, fileName);
    int contentLength = len(content);
    f.size = contentLength+1;

    f.content = (char*)malloc((contentLength + 1) * sizeof(char)); // +1 pour le caractère nul de fin de chaîne
    if (f.content == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Copier le contenu de la liste chaînée dans le tableau de contenu du fichier
    int i = 0;
    while (content != NULL) {
        f.content[i] = content->value.charValue;
        content = content->next;
        i++;
    }
    f.content[i] = '\0';
    return f;
}


/**
 * La fonction `displayFile` prend un pointeur vers une structure `File` en entrée et affiche des
 * informations sur le fichier, telles que son nom, sa taille et son contenu.
 * 
 * @param file Un pointeur vers une structure représentant un fichier. La structure contient des
 * informations telles que le nom du fichier, sa taille et son contenu.
 * 
 * @return La fonction `displayFile` renvoie `void`, ce qui signifie qu'elle ne renvoie aucune valeur.
 * Il affiche simplement des informations sur le fichier passé en paramètre.
 */
void displayFile(File* file) {
    if (file == NULL) {
        printf("Le fichier est vide.\n");
        return;
    }

    printf("Nom du fichier : %s\n", file->fileName);
    printf("Taille du fichier : %d octets\n", file->size);
    if(file->size > 0){
        printf("Contenu du fichier :\n%s\n", file->content);
    }else{
        printf("(Vide)\n");
    }
}



void myRemove(Partition *partition, char* partitionName, char* fileName){
    deleteFile(partition, partitionName, fileName);
}

// Fonction pour libérer la mémoire allouée pour un fichier
void freeFile(File* file) {
    if (file == NULL) {
        return;
    }

    if (file->fileName != NULL) {
        free(file->fileName);
        file->fileName = NULL;
    }

    if (file->content != NULL) {
        free(file->content);
        file->content = NULL;
    }

    file->size = 0;
}


int myFormatExists(char* partitionName){
    if (access(partitionName, F_OK) == 0) {
        return 1;
    } 
    return 0;
}