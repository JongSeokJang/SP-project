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
#define MAX_RECORD_SIZE	60

#define TRUE	1
#define FALSE	0

#define	A	0
#define X	1
#define	L	2
#define	B	3
#define	S	4
#define	T	5
#define	F	6

#define PC	8
#define SW	9

typedef struct _historyNode{
	char	historyCmd[32+1];
	int		historyIndex;
	struct _historyNode* next;
}historyNode;

typedef struct _hashNode{
	unsigned char opcode;
	char	mnemonic[8+1];
	char	format[4+1];
	struct _hashNode* nextNode;
}hashNode;

typedef struct _symbTableNode{
	char label[32+1];
	int location;
	struct _symbTableNode* next;
}symbTableNode;

int SystemPrompt(unsigned char*** memory);

//( 기능과 관련된 것
void ShowMemory(unsigned char*** memory, int start, int end);
void SetMemory(unsigned char*** memory, int start, int end, unsigned char value);
void InsertHistory(char* cmd, int index);
void ShowHistory(void);
void FreeHistory(void);
void ShowHelp(void);
void ShowDir(void);

//symbol Table;
void ShowSymbolTable(void);
int SearchSymbolTable(char* symbol);
int InsertSymbolTable(int locationCount, char* label);
int CalculateAddress(int locationCount, char* mnemonic, char* operand);

void AllocateMemory(unsigned char*** memory);
void FreeMemory(unsigned char*** memory);
void InitMemory(unsigned char*** memory);
void TrimAll(char* input, char* output);
void Trim(char* input);
void rtrim(char* input);
int	TransHex2Dec (char* Hex);

int MakeTokenArray(char (*tokenArray)[10], char* rowData);
int GetArrayLength(char (*tokenArray)[10]);

//HashTable 와 관련된 것
int MakeHashKey(char* key);
void MakeHashTable(void);
void ShowHashTable(void);
void FreeHashTable(void);

//make machine code;
int PassOne(char* fileName);
int PassTwo(char* fileName);
int IsIndicator(char* mnemonic);
int MakeObjectCode(char* objectCode,int format, int location, char* mnemonic, char* operand, int pointerCount, int basis);
int CalculateRelative(int pointerCounter, int basis, int operandAddress, char* relitiveAddress);

int SearchOpcode(char* mnemonic);
int SearchOpcodeNo(char* mnemonic);
int SearchOpcodeFormat(char* mnemonic);

int CheckInput(char* rowData);
int CheckParameter(char(*tokenArray)[10], int arrayCount);
int CheckEmpty(char* rowData);

void ShowSource(char* fileName);
void FreeSymbolTable(symbTableNode* symbNode);

hashNode* hashTable[HASHSIZE] = {NULL,};

historyNode* headNode = NULL;
historyNode* tailNode = NULL;

symbTableNode* symbTableNodeHead = NULL;
symbTableNode* symbTableNodeTemp = NULL;

int	historyCount = 1;
int dumpIndex = -1;
int sizeOfAssem = -1;
int baseFlag = 0;

int checkLabel(char* label);
void ExitPassTwo(char* line, char* input, char* fileName, FILE* readFP, FILE* writeFP1, FILE* writeFP2);
