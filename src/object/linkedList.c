#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"



void initList( LinkedList* list) {
    *list = NULL;
}

/**
 * La fonction `createCell` alloue dynamiquement de la mémoire à une nouvelle cellule, initialise son
 * type et sa valeur en fonction des données d'entrée et renvoie un pointeur vers la nouvelle cellule.
 * 
 * @param data Le paramètre `data` dans la fonction `createCell` représente la valeur qui sera stockée
 * dans la cellule nouvellement créée. En fonction du paramètre `type`, cette valeur `data` sera
 * stockée sous forme d'entier ou de caractère dans le champ `value` de la cellule.
 * @param type Le paramètre `type` dans la fonction `createCell` est un type d'énumération `ListType`
 * qui spécifie le type de données que la cellule contiendra. Il peut s'agir de `INT_TYPE` ou
 * `CHAR_TYPE`, indiquant si la cellule stockera une valeur entière ou une valeur de caractère.
 * 
 * @return La fonction `createCell` renvoie un pointeur vers une structure `Cell`.
 */
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

/**
 * La fonction ` append » ajoute une nouvelle cellule avec les données et le type spécifiés à la fin
 * d'une liste chaînée.
 * 
 * @param list Un pointeur vers le début d’une liste chaînée.
 * @param data Le paramètre `data` dans la fonction `append` représente la valeur entière que vous
 * souhaitez ajouter à la liste chaînée.
 * @param type Le paramètre `type` dans la fonction `append` est utilisé pour spécifier le type de
 * données ajoutées à la liste chaînée. Il est de type ` ListType », qui est  une
 * énumération ou un type personnalisé défini ailleurs dans la base de code. Le paramètre `type` aide à
 * créer un nouveau
 */
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


/**
 * La fonction `printList` parcourt une liste chaînée et imprime les valeurs en fonction de leur type.
 * 
 * @param list La fonction `printList` que vous avez fournie prend un paramètre `list` de type
 * `LinkedList`, qui semble être une liste chaînée implémentée à l'aide d'une structure nommée `Cell`.
 * La structure `Cell` contient un champ `type` pour indiquer le type de données stockées dans le nœud
 * (`INT_TYPE` ou
 */
void printList(LinkedList list) {
    printf("Liste : ");
    Cell* current = list;
    while (current != NULL) {
        if (current->type == INT_TYPE) {
            printf("%d", current->value.intValue);
        } else if (current->type == CHAR_TYPE) {
            printf("%c", current->value.charValue);
        }else
        current = current->next;
    }
    printf("\n");
}


/**
 * La fonction pop supprime le premier élément d'une liste chaînée.
 * 
 * @param list Le paramètre `list` dans la fonction `pop` est un pointeur vers une structure
 * `LinkedList`. Cette structure contient  un pointeur vers le début de la liste chaînée.
 * La fonction est conçue pour supprimer le premier élément (tête) de la liste chaînée.
 */
void pop(LinkedList* list) {
    if (*list == NULL) {
        printf("Liste vide. Impossible de pop.\n");
    } else {
        Cell* temp = *list;
        *list = (*list)->next;
        free(temp);
    }
}


/**
 * La fonction clear supprime tous les éléments d'une liste chaînée.
 * 
 * @param list Le paramètre `list` est un pointeur vers une structure `LinkedList`.
 */
void clear(LinkedList* list) {
    while (*list != NULL) {
        pop(list);
    }
}

/**
 * La fonction `len` calcule la longueur d'une liste chaînée en parcourant ses éléments.
 * 
 * @param list Le paramètre `list` dans la fonction `len` est de type `LinkedList`, qui est une
 * structure de données de liste chaînée. La fonction calcule la longueur de la liste chaînée en
 * parcourant chaque élément de la liste à l'aide d'un pointeur ` current » de type ` Cell ».
 * 
 * @return La fonction `len` renvoie la longueur de la liste chaînée `list`.
 */
int len(LinkedList list) {
    int length = 0;
    Cell* current = list;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}


