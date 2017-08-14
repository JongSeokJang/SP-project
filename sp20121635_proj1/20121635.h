#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<error.h>

#define ROW_MEMORY_SIZE 65536
#define COL_MEMORY_SIZE 16 

#define HASHSIZE		20

#define TRUE	1
#define FALSE	0

typedef struct _historyNode{
	char	historyCmd[32+1];
	int		historyIndex;
	struct _historyNode* next;
}historyNode;

typedef struct _hashNode{
	unsigned char opcode;
	char	mnemonic[8+1];
	struct _hashNode* nextNode;
}hashNode;

int SystemPrompt(unsigned char*** memory);

//( 기능과 관련된 것
void ShowMemory(unsigned char*** memory, int start, int end);
void SetMemory(unsigned char*** memory, int start, int end, unsigned char value);
void InsertHistory(char* cmd, int index);
void ShowHistory(void);
void FreeHistory(void);
void ShowHelp(void);
void ShowDir(void);

void AllocateMemory(unsigned char*** memory);
void FreeMemory(unsigned char*** memory);
void InitMemory(unsigned char*** memory);
void TrimAll(char* input, char* output);
int	TransHex2Dec (char* Hex);

int MakeTokenArray(char (*tokenArray)[10], char* rowData);
int GetArrayLength(char (*tokenArray)[10]);

//HashTable 와 관련된 것
int MakeHashKey(char* key);
void MakeHashTable(void);
void ShowHashTable(void);
void FreeHashTable(void);

int SearchOpcode(char* mnemonic);

int CheckInput(char* rowData);
int CheckParameter(char(*tokenArray)[10], int arrayCount);
int CheckEmpty(char* rowData);

hashNode* hashTable[HASHSIZE] = {NULL,};

historyNode* headNode = NULL;
historyNode* tailNode = NULL;

int	historyCount = 1;
int dumpIndex = -1;

