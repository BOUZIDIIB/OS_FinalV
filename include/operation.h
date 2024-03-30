#ifndef PARTITION_H
#define PARTITION_H

#include <stdbool.h>

#include "linkedList.h"

#define FAT_SIZE 100
#define BLOCK_SIZE 1024
#define PARTITION_SIZE 1000 


typedef struct {
    char name[100];
    int size;
    bool is_free;
} FileInfo;

typedef struct {
    FileInfo files[FAT_SIZE];
    bool fat[FAT_SIZE];
    int free_blocks;
    int next_free_block;
    char data[PARTITION_SIZE][BLOCK_SIZE];
} Partition;


Partition create_or_load_partition(char *partitionName);
void printPartitionData(Partition partition);
void initPartition(Partition *partition);
void saveInPartition(Partition partition, char *partitionName);
void loadPartition(Partition* partition, char *partitionName);
LinkedList readWholeFile(Partition *partition, LinkedList indexes); //
LinkedList readFile(Partition Partition, char* fileName); //
LinkedList findIndexesByName(Partition *partition, char *filename); 
int writeToFile(Partition *partition, char* partition_name ,char *filename, void *data, int size); 
void initFileInfo(FileInfo *fileInfo, const char *name, int size, bool is_free); 
Partition create_new_partition(char* partitionName);
int updateFileContent(Partition *partition, char* partition_name, char *filename, void *data, int size);
int deleteFile(Partition *partition, char* partitionName, char* filename); 

#endif