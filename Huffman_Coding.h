#ifndef HAFFMANCODING_HUFFMANCODING_H
#define HAFFMANCODING_HUFFMANCODING_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <limits.h>

//-- My Variables --//

#define N 10000 //File buffer limit
static int numberOfValues,liveNodeListLength;
static TRE selectedNode1 = NULL,selectedNode2 = NULL;
static TRE selectedFreq1 = NULL,selectedFreq2 = NULL;
static char codeString[100] = "";
static char completeCode[1000] = "";
static bool textMode; //The encoding mode that the user chooses


//-- My Functions --//

int getAnswer();
void printInstructions(int input);
void insertCodeToList(int value,char* code);
void printEncodedMessage();
void read_ColourTableValues_File();
void read_TextMessage_File();
void findAllValuesFrequency(CV FHead);
void findColourFrequency(int colourValue, bool createHead);
void insert_to_VFList(int colourValue, int colourFrequency, bool createHead);
void insert_to_File_List(int word, bool createHead);
void insert_to_Unique_List(int value, bool createHead);
void insert_List_to_Array(TRE array[],VF head);
void createTree();
void initialize(TRE firstnode, TRE freqArray[], int loopIndex);
void remove_From_LiveNodes_List(TRE oldLiveNode);
void add_To_LiveNodes_List(TRE newLiveNode);
void selectionSortArray(TRE array[],int length);
void tree_Traversal(TRE t);
void printCodes(TRE array[]);
void printOriginalMessage(bool textMode);
int findListLength(VF head);
int minValuesGenerator(TRE array[], int loopIndex, int listlength);
TRE create_New_TreeNode();

#endif //HAFFMANCODING_HUFFMANCODING_H
