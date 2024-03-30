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


    // if (myFormat(&partition, partitionName)){
    //     printPartitionData(partition); 
    // }
    loadPartition(&partition, partitionName);
    int res = exists(&partition, fileName);
    if(res){
        File file = myRead(&partition, partitionName, fileName);
        displayFile(&file);
    }
    printPartitionData(partition);


}
