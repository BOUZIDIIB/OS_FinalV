#ifndef MENU_H
#define MENU_H


/**
 * Partition Operation
*/
#define CHARGER 1
#define FORMATER 2
#define QUIT_FORMAT 3


/**
 * File Operations
*/
#define INFO_FILE 1
#define REMOVE_FILE 2
#define EDIT_FILE 3
#define QUIT_FILE 4


/**
 * Main  Options
*/
#define LOAD_FORMAT_PARTITION 1
#define FILE_OPERATION 2
#define PARTITION_STATE 3
#define QUIT 4



#define QUIT_UNOPENED 2




void partitionOperation();
void fileOperation();
void editFile();
bool openPartitionOperation(char choice);
bool unOpenedParitionOperation(char choice);
#endif