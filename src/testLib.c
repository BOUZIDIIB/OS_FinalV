#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libOS.h"
#include "operation.h"

#define BUFFER_SIZE 50


int main() {
    char partitionName[] = "test.bin";
    char fileName[] = "example.txt";
    char data[] = "Hello, world!";
    char bufferReader[BUFFER_SIZE];
    Partition partition; 

    int partitionExists = myFormatExists(partitionName);
    printf("myFormatExist: %d\n", partitionExists);
    if(partitionExists){
        loadPartition(&partition, partitionName);
    }else{
        myFormat(&partition, partitionName);
    }
    int res = exists(&partition, fileName);
    printf("Le fichier existe "); 
    File file;
    if(res){
        printf(": oui\n");
        file = myOpen(&partition, partitionName, fileName); 
        displayFile(&file);
        myRemove(&partition, partitionName, fileName);
        printf("Fichier supprimer de la partition\n");
    }else{
        printf(": non\n");
        file = myOpen(&partition, partitionName, fileName);
        displayFile(&file);
        setFileContent(&file, data);
        myWrite(&partition, partitionName, &file);
        printf("Fichier écris dans la partition\n");

    }
    printPartitionData(partition);

}
