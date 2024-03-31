#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "libOS.h"
#include "operation.h"
#include "linkedList.h"




/**
 * La fonction `createFile` génère un fichier avec un nom spécifié et un contenu donné.
 * 
 * @param fileName Le nom du fichier à créer, représenté par un tableau de caractères.
 * @param content Le contenu à écrire dans le fichier, représenté par un tableau de caractères.
 * 
 * @return Une structure `File` est renvoyée, contenant le `fileName`, le `content` et la taille (`size`) du fichier créé.
 */
File createFile(char* fileName, char* content) {
    File file;
    file.fileName = strdup(fileName);
    file.content = strdup(content); 
    file.size = strlen(content)+1;
    return file;
}

/**
 * La fonction `myFormat` initialise une partition, enregistre les données dans la partition, puis renvoie
 * la valeur 1.
 * 
 * @param partition Un pointeur vers une structure `Partition`, pour représenter une partition dans un système de stockage.
 * @param partitionName Un pointeur vers un tableau de caractères représentant le nom de la partition où les données seront enregistrées.
 * 
 * @return La fonction `myFormat` renvoie un entier 1.
 */
int myFormat(Partition * partition, char* partitionName) {
    initPartition(partition);
    saveInPartition(*partition, partitionName);
    return 1;
}

/**
 * La fonction `myOpen` permet d'ouvrir un fichier depuis une partition, de lire son contenu s'il existe
 * et de créer le fichier avec un contenu vide s'il n'existe pas.
 * 
 * @param partition Un pointeur vers une structure `Partition`, contenant des informations sur une partition de stockage sur un périphérique.
 * @param partitionName Un pointeur vers un tableau de caractères représentant le nom de la partition où se trouve le fichier ou sera créé.
 * @param fileName Le nom du fichier que l'on souhaite ouvrir ou créer dans une partition spécifiée.
 * 
 * @return Un objet `File` est renvoyé par la fonction `myOpen`.
 */
File myOpen(Partition *partition, char* partitionName, char *fileName) {
    File file;
    file.fileName = fileName;
    if (exists(partition, fileName)) {
        char* content = readFile(*partition, fileName);
        file.size= (strlen(content)+1)*sizeof(char);
        file.content = (char*)malloc(file.size * sizeof(char));
        strcpy(file.content, content);
    } else {
        file.content = "";
        file.size = 0;
        writeToFile(partition, partitionName, file.fileName, file.content, file.size ); 
    }
    return file;
}


/**
 * La fonction `myWrite` met à jour le contenu d'un fichier dans une partition spécifiée.
 * 
 * @param partition Un pointeur vers une structure `Partition`, contenant des informations sur une partition de stockage.
 * @param partitionName Une chaîne de caractères représentant le nom de la partition où le contenu du fichier sera mis à jour.
 * @param f Une structure représentant un fichier, contenant le nom du fichier, son contenu et sa taille.
 * 
 * @return La fonction `myWrite` renvoie le résultat de la fonction `updateFileContent`, indiquant le succès ou 
 * l'échec de la mise à jour du contenu du fichier dans la partition spécifiée.
 */
int myWrite(Partition *partition, char* partitionName,File* f){
    return updateFileContent(partition, partitionName, f->fileName, f->content, f->size);
}   

/**
 * La fonction `setFileContent` définit le contenu d'un fichier avec les données fournies et gère
 * l'allocation de mémoire.
 * 
 * @param file Un pointeur vers une structure `File` contenant des informations sur un fichier, telles que sa taille et son contenu.
 * @param data Un pointeur vers un tableau de caractères contenant le contenu à définir dans la structure `File`. Si `data` est `NULL`, cela signifie que le contenu du fichier doit être vide (taille 0). Sinon, le contenu est défini en fonction des données fournies.
 * 
 * @return Si le paramètre `data` est `NULL`, la fonction définira la taille du fichier sur 0 puis
 * reviendra sans allouer de mémoire pour le contenu.
 */
void setFileContent(File* file, char* data) {
    if (data == NULL) {
        file->size = 0;
        return;
    }
    file->size = strlen(data);
    file->content = (char*)malloc((file->size + 1) * sizeof(char));
    if (file->content == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    strcpy(file->content, data);
}


/**
 * La fonction `displayFile` prend un pointeur de fichier en entrée et imprime des informations sur le
 * fichier, y compris son nom, sa taille et son contenu s'il n'est pas vide.
 * 
 * @param file Un pointeur vers une structure représentant un fichier. La structure contient des
 * informations telles que le nom du fichier, sa taille et son contenu.
 * 
 * @return La fonction `displayFile` ne renvoie explicitement aucune valeur. Il s’agit d’une fonction
 * vide, ce qui signifie qu’aucun type de retour n’est spécifié. La fonction effectue certaines actions
 * comme l'impression d'informations sur le fichier, mais elle ne renvoie aucune valeur spécifique.
 */
void displayFile(File* file) {
    if (file == NULL) {
        printf("Le fichier est vide.\n");
        return;
    }

    printf("Nom du fichier : %s\n", file->fileName);
    printf("Taille du fichier : %d octets\n", file->size);
    if(file->size > 1){
        printf("Contenu du fichier :\n%s\n", file->content);
    }else{
        printf("(Vide)\n");
    }
}



/**
 * La fonction `myRemove` appelle `deleteFile` pour supprimer un fichier d'une partition en fonction du
 * nom de la partition et du nom du fichier.
 * 
 * @param partition Pointeur vers une structure Partition, qui contient des informations
 * sur une partition de stockage sur un périphérique.
 * @param partitionName Le paramètre `partitionName` est une chaîne qui représente le nom de la
 * partition dont le fichier doit être supprimé.
 * @param fileName Le paramètre `fileName` dans la fonction `myRemove` représente le nom du fichier que
 * vous souhaitez supprimer de la partition spécifiée.
 */
void myRemove(Partition *partition, char* partitionName, char* fileName){
    deleteFile(partition, partitionName, fileName);
}

/**
 * La fonction `freeFile` libère de la mémoire pour une structure File en libérant ses champs fileName
 * et content.
 * 
 * @param file Le paramètre `file` est un pointeur vers une structure de type `File`. La structure `
 * Fichier ` contient  des informations sur un fichier, telles que son nom, son contenu et
 * sa taille. La fonction `freeFile` est chargée de libérer la mémoire allouée pour le nom et le
 * contenu du fichier, et
 * 
 * @return La fonction `freeFile` renvoie `void`, ce qui signifie qu'elle ne renvoie aucune valeur.
 */
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


/**
 * La fonction `myFormatExists` vérifie si un fichier ou un répertoire avec le `partitionName` donné
 * existe.
 * 
 * @param partitionName Le paramètre `partitionName` est un pointeur vers un tableau de caractères
 * qui représente le nom d'une partition.
 * 
 * @return La fonction `myFormatExists` renvoie une valeur entière. Il renvoie 1 si le nom de partition
 * spécifié existe et 0 s'il n'existe pas.
 */
int myFormatExists(char* partitionName){
    if (access(partitionName, F_OK) == 0) {
        return 1;
    } 
    return 0;
}