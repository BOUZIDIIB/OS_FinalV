#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "linkedList.h"

#define PARTITION_NAME "partition"

int main() {
    printf("Start");
    Partition partition = create_or_load_partition(PARTITION_NAME);
    printPartitionData(partition); 
    char* data = "NewHelloWorld je suis en train d\'écrire un super lon text !"; 
    char readBuffer[100];
    char* fileName = "test1";
    updateFileContent(&partition, PARTITION_NAME, fileName, data, strlen(data)+1);
    printPartitionData(partition); 

    LinkedList indexOfFile = findIndexesByName(&partition, fileName); 
    printList(indexOfFile);
    printf("\nLecture du fichier %s", fileName);
    LinkedList res = readFile(partition, fileName);
    printList(res);
    printf("End"); 
    return EXIT_SUCCESS;
}
    