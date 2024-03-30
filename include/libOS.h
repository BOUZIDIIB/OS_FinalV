#ifndef PROJECT_LIB_H
#define PROJECT_LIB_H
#include "linkedList.h"
#include "operation.h"

typedef struct File{
    char* fileName; 
    char* content;
    int size;
}File;


int myFormat(Partition * partition, char* partitionName); 
File myOpen(Partition *partition, char* partitionName, char *fileName);  ///
int myWrite(Partition *partition, char* partitionName, File* f); //
void myRemove(Partition *partition, char* partitionName, char* fileName);
File createFile(char* fileName, char* content);
void displayFile(File* file);
void setFileContent(File* file, char* data);
void freeFile(File* file); 

int myFormatExists(char* partitionName);
File myRead(Partition *partition, char* partitionName, char* fileName);         
#endif