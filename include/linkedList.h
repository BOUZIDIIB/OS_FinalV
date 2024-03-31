#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * @brief Fonctions et structures de _support_ pour les listes chaînées 
*/

typedef enum {
    INT_TYPE,
    CHAR_TYPE
} ListType;

typedef union {
    int intValue;
    char charValue;
} DataValue;

typedef struct Cell {
    ListType type;
    DataValue value;
    struct Cell* next;
} Cell;

typedef Cell* LinkedList;


void initList( LinkedList* list); 
Cell* createCell(int data, ListType type); 
void append(LinkedList* list, int data, ListType type); 
void printList(LinkedList list); 
void pop(LinkedList* list); 
void clear(LinkedList* list); 
int len(LinkedList list); 
    

#endif
