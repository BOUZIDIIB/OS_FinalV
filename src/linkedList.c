#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"



void initList( LinkedList* list) {
    *list = NULL;
}

Cell* createCell(int data, ListType type) {
    Cell* newNode = (Cell*)malloc(sizeof(Cell));
    if (!newNode) {
        fprintf(stderr, "Erreur: Impossible d'allouer de la mémoire\n");
        exit(EXIT_FAILURE);
    }
    newNode->type = type;
    if (type == INT_TYPE) {
        newNode->value.intValue = data;
    } else if (type == CHAR_TYPE) {
        newNode->value.charValue = (char)data;
    }
    newNode->next = NULL;
    return newNode;
}

void append(LinkedList* list, int data, ListType type) {
    Cell* newNode = createCell(data, type);
    if (*list == NULL) {
        *list = newNode;
    } else {
        Cell* current = *list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


void printList(LinkedList list) {
    printf("Liste : ");
    Cell* current = list;
    while (current != NULL) {
        if (current->type == INT_TYPE) {
            printf("%d", current->value.intValue);
        } else if (current->type == CHAR_TYPE) {
            printf("%c", current->value.charValue);
        }
        current = current->next;
    }
    printf("\n");
}


void pop(LinkedList* list) {
    if (*list == NULL) {
        printf("Liste vide. Impossible de pop.\n");
    } else {
        Cell* temp = *list;
        *list = (*list)->next;
        free(temp);
    }
}


void clear(LinkedList* list) {
    while (*list != NULL) {
        pop(list);
    }
}


int len(LinkedList list) {
    int length = 0;
    Cell* current = list;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}


