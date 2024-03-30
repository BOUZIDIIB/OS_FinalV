#ifndef PROJECT_LIB_H
#define PROJECT_LIB_H
#include "linkedList.h"
#include "operation.h"

typedef struct File{
    char* fileName; 
    char* content;
    int size;
}File;


int myFormat(Partition * partition, char* partitionName); //
File myOpen(Partition *partition, char* partitionName, char *fileName);  ///
int myWrite(Partition *partition, char* partitionName,File* f); //
File myRead(Partition *partition, char* partitionName, char* fileName);
void mySeek(File* f, int offset, int base);
File createFile(char* fileName, char* content);
void displayFile(File* file);
int exists(Partition *partition,char *filename);


        
#endif