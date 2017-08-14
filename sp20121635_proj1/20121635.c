#include "20121635.h"

int main(){
	// 메모리를 초기화 시키는데 가급적 함수로 하자.
	unsigned char **memory;

	AllocateMemory(&memory);
	InitMemory(&memory);
	MakeHashTable();

	
	while(TRUE){
		SystemPrompt(&memory);
	}

	return 0;
}

int SystemPrompt(unsigned char*** memory)
{
	char* token;
	char inputCmd[32+1];
	char tempCmd[32+1];
	int  ii;
	char tempToken[32+1];
	char tokenArray[10][10];
	int arrayLength;
	int start, end, value;
	int result;

	//using string init
	memset(inputCmd,	0x00, sizeof(inputCmd));
	memset(tempCmd,		0x00, sizeof(tempCmd));
	memset(tempToken,	0x00, sizeof(tempToken));

	//using string's Array init
	for( ii = 0; ii < 10; ii++)
		memset(tokenArray[ii], 0, sizeof(tokenArray[0]));

	printf("sicsim> ");

	fgets(inputCmd, sizeof(inputCmd), stdin);

	if(inputCmd[0] == '\n')
		return -1;
	if(inputCmd[0] == ' ' || inputCmd[0] =='\t'){
		return -1;
	}
	
	
		

	inputCmd[strlen(inputCmd)-1] = '\0';
	strcpy(tempCmd, inputCmd);

	token = strtok(tempCmd, " ");
	//token = strtok(tempCmd, " \t");

	if( !strcmp("h", token) || !strcmp("help", token) ){

		// remove empty space(" ") using TrimAll()
		if( !strcmp("h", token) )
			TrimAll(&inputCmd[2], tempToken);
		else
			TrimAll(&inputCmd[5], tempToken);

		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		InsertHistory(inputCmd, historyCount++);

		ShowHelp();
	}
	else if( !strcmp("d", token) || !strcmp("dir", token) ){
		
		// remove empty space(" ") using TrimAll()
		if( !strcmp("d", token) )
			TrimAll(&inputCmd[2], tempToken);
		else
			TrimAll(&inputCmd[4], tempToken);

		// in exsit parameter case
		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		InsertHistory(inputCmd, historyCount++);

		ShowDir();
	}
	else if( !strcmp("q", token) || !strcmp("quit", token) ){

		// remove empty space(" ") using TrimAll()
		if( !strcmp("q", token) )
			TrimAll(&inputCmd[2], tempToken);
		else
			TrimAll(&inputCmd[5], tempToken);

		// in exsit parameter case
		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		FreeMemory(memory);
		FreeHistory();
		FreeHashTable();
		exit(1);
	}
	else if( !strcmp("hi", token) || !strcmp("history", token) ){

		// remove empty space(" ") using TrimAll()
		if( !strcmp("hi", token) )
			TrimAll(&inputCmd[3], tempToken);
		else
			TrimAll(&inputCmd[9], tempToken);

		// in exsit parameter case
		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		InsertHistory(inputCmd, historyCount++);

		ShowHistory();
	}
	else if( !strcmp("du", token) || !strcmp("dump", token) ){

		
		// remove empty space(" ") using TrimAll()
		if( !strcmp("du", token) ){
			TrimAll(&inputCmd[3], tempToken);
			result = CheckEmpty(&inputCmd[3]);
		}
		else{
			TrimAll(&inputCmd[5], tempToken);
			result = CheckEmpty(&inputCmd[5]);
		}
		if(result == -1){
			printf("please check empty\n");
			return -1;
		}

		if(strlen(tempToken)!= 0 && CheckInput(tempToken) == -1 ){
			printf("Invalid input!!\n");
			return -1;
		}
		// make parameter to Array
		arrayLength = MakeTokenArray(tokenArray, tempToken);

		if( CheckParameter(tokenArray, arrayLength) == -1 ){
			printf("Invalid parameter!!\n");
			return -1;
		}

		for( ii = 0; ii < arrayLength; ii++ ){
			if( strlen(tokenArray[ii]) > 5 ){
				printf("Invalid parameter!!\n");
				return -1;
			}
		}
		
		if( arrayLength == 0 ){
			start	= dumpIndex+1;
			end		= dumpIndex+160;

			if( start >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE ){
				start	= 0;
				end		= 159;	
			}

			if( end >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE){
				end = ROW_MEMORY_SIZE*COL_MEMORY_SIZE-1;
			}
			InsertHistory(inputCmd, historyCount++);
			ShowMemory(memory,start, end);
			dumpIndex = end;
		}
		else if( arrayLength == 1 ){
			start	= TransHex2Dec(tokenArray[0]);
			end		= start + 159;

			if( start >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE ){
				printf("[%d]start is bigger then Memory!!!\n",start);
				return -1;
			}
			if( end >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE){
				end = ROW_MEMORY_SIZE*COL_MEMORY_SIZE-1;
			}

			InsertHistory(inputCmd, historyCount++);
			ShowMemory(memory,start, end);
			dumpIndex = end;
		}
		else if( arrayLength == 2 ){
			start	= TransHex2Dec(tokenArray[0]);
			end		= TransHex2Dec(tokenArray[1]);

			// first is bigger than second
			if( start > end ){
				printf("start is bigger then end!!!\n");
				return -1;
			}
			if( start >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE ){
				printf("[%d]start is bigger then Memory!!!\n",start);
				return -1;
			}
			if( end >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE){
				end = ROW_MEMORY_SIZE*COL_MEMORY_SIZE-1;
			}

			InsertHistory(inputCmd, historyCount++);
			ShowMemory(memory,start, end);
			dumpIndex = end;
		}
		else{
			printf("invalid parameter!!\n");
		}
	}
	else if( !strcmp("e", token) || !strcmp("edit", token) ){


		// remove empty space(" ") using TrimAll()
		if( !strcmp("e", token) ){
			TrimAll(&inputCmd[2], tempToken);
			result = CheckEmpty(&inputCmd[2]);
		}
		else{
			TrimAll(&inputCmd[5], tempToken);
			result = CheckEmpty(&inputCmd[5]);
		}

		if( result == -1 ){
			printf("please check empty\n");
			return -1;
		}

		if(strlen(tempToken)!= 0 && CheckInput(tempToken) == -1){
			printf("Invalid input!!\n");
			return -1;
		}

		// make parameter to Array
		arrayLength = MakeTokenArray(tokenArray, tempToken);
		
		for( ii = 0; ii < arrayLength; ii++ ){
			if( strlen(tokenArray[ii]) > 5 ){
				printf("Invalid parameter!!\n");
				return -1;
			}
		}
		
		if( CheckParameter(tokenArray, arrayLength) == -1 ){
			printf("Invalid parameter!!\n");
			return -1;
		}

		if( arrayLength == 0 ){
			printf("invalid parameter!!\n");
		}
		else if( arrayLength == 1 ){
			printf("invalid parameter!!\n");
		}
		else if( arrayLength == 2 ){

			start	= TransHex2Dec(tokenArray[0]);
			end		= TransHex2Dec(tokenArray[0]);
			value	= TransHex2Dec(tokenArray[1]);

			if(strlen(tokenArray[1]) > 2){
				printf("invalid parameter!!\n");
				return -1;
			}
			
			if( start >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE ){
				printf("[%d]start is bigger then Memory!!!\n",start);
				return -1;
			}

			SetMemory(memory, start, end, value);
			InsertHistory(inputCmd, historyCount++);
		}
		else{
			printf("invalid parameter!!\n");
		}
	}
	else if( !strcmp("f", token) || !strcmp("fill", token) ){
		
		// remove empty space(" ") using TrimAll()
		if( !strcmp("f", token) ){
			TrimAll(&inputCmd[2], tempToken);
			result = CheckEmpty(&inputCmd[2]);
		}
		else{
			TrimAll(&inputCmd[5], tempToken);
			result = CheckEmpty(&inputCmd[5]);
		}

		if( result == -1 ){
			printf("please check empty\n");
			return -1;
		}

		if(strlen(tempToken)!= 0 && CheckInput(tempToken) == -1){
			printf("Invalid input!!\n");
			return -1;
		}


		// make parameter to Array
		arrayLength = MakeTokenArray(tokenArray, tempToken);
		
		for( ii = 0; ii < arrayLength; ii++ ){
			if( strlen(tokenArray[ii]) > 5 ){
				printf("Invalid parameter!!\n");
				return -1;
			}
		}

		if( CheckParameter(tokenArray, arrayLength) == -1 ){
			printf("Invalid parameter!!\n");
			return -1;
		}
		
		if( arrayLength == 0 ){
			printf("invalid parameter!!\n");
		}
		else if( arrayLength == 1 ){
			printf("invalid parameter!!\n");
		}
		else if( arrayLength == 2 ){
			printf("invalid parameter!!\n");
		}
		else if( arrayLength == 3){
			start	= TransHex2Dec(tokenArray[0]);
			end		= TransHex2Dec(tokenArray[1]);
			value	= TransHex2Dec(tokenArray[2]);
			
			if(strlen(tokenArray[2]) > 2){
				printf("invalid parameter!!\n");
				return -1;
			}
			// first is bigger than second
			if( start > end ){
				printf("start is bigger then end!!!\n");
				return -1;
			}
			if( start >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE ){
				printf("[%d]start is bigger then Memory!!!\n",start);
				return -1;
			}
			if( end >= ROW_MEMORY_SIZE*COL_MEMORY_SIZE){
				end = ROW_MEMORY_SIZE*COL_MEMORY_SIZE-1;
			}

			InsertHistory(inputCmd, historyCount++);
			SetMemory(memory, start, end, value);
		}
		else{
			printf("invalid parameter!!\n");
		}
	}
	else if( !strcmp("reset", token) ){

		// remove empty space(" ") using TrimAll()
		if( !strcmp("reset", token) )
			TrimAll(&inputCmd[6], tempToken);

		// in exsit parameter case
		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		InsertHistory(inputCmd, historyCount++);

		SetMemory(memory, 0, ROW_MEMORY_SIZE * COL_MEMORY_SIZE -1, 0);
	}
	else if( !strcmp("opcode", token) ){

		// remove empty space(" ") using TrimAll()
		TrimAll(&inputCmd[6], tempToken);

		if(strlen(tempToken)!= 0 && CheckInput(tempToken) == -1 ){
			printf("Invalid input!!\n");
			return -1;
		}

		arrayLength = MakeTokenArray(tokenArray, tempToken);

		for( ii = 0; ii < arrayLength; ii++ ){
			if( strlen(tokenArray[ii]) > 5 ){
				printf("Invalid parameter!!\n");
				return -1;
			}
		}

		if( arrayLength == 0 ){
			printf("invalid parameter!!\n");
		}
		else if( arrayLength == 1){
			if( SearchOpcode(tokenArray[0]) == -1 ){
				printf("invalid parameter!\n");
				return -1;
			}
				

			InsertHistory(inputCmd, historyCount++);
		}
		else if( arrayLength == 2){
			printf("invalid parameter!!\n");
		}
		else{
		}
	}
	else if( !strcmp("opcodelist", token) ){

		if( !strcmp("opcodelist", token) )
			TrimAll(&inputCmd[11], tempToken);

		// in exsit parameter case
		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		InsertHistory(inputCmd, historyCount++);

		ShowHashTable();
	}
	else
	{
		printf("Cmd is wrong!!!\n");
		return -1;
	}
	return 0;
}

// show cmdlist
void ShowHelp(void)
{
	int ii;
	char *helpCmdList[] =  {"h[elp]", "d[ir]", "q[uit]", "hi[story]",
							"du[mp] [start, end]","e[dit] address, value",
							"f[ill] start, end, value", "reset",
							"opcode mnemonic", "opcodelist"};

	for( ii = 0; ii < (int)(sizeof(helpCmdList)/sizeof(helpCmdList[0])); ii++){
		printf("%s\n", helpCmdList[ii]);
	}

}

// allocate memory
void AllocateMemory(unsigned char*** memory)
{
	int ii;
	*memory = (unsigned char**)malloc(ROW_MEMORY_SIZE * sizeof(unsigned char*));
	for( ii = 0; ii < ROW_MEMORY_SIZE; ii++){
		(*memory)[ii] = (unsigned char*)malloc(COL_MEMORY_SIZE * sizeof(unsigned char));
	}
}

// initMemory (all memory = 0x00
void InitMemory(unsigned char *** memory)
{
	int ii, jj;
	for( ii = 0; ii < ROW_MEMORY_SIZE; ii++){
		for( jj = 0; jj < COL_MEMORY_SIZE; jj++){
			(*memory)[ii][jj] = '\x0';
		}
	}

}

// freememory
void FreeMemory(unsigned char*** memory)
{
	int ii;
	for( ii = 0; ii < ROW_MEMORY_SIZE;ii ++)
		free((*memory)[ii]);
	free(*memory);
}

// show memory
void ShowMemory(unsigned char*** memory, int start, int end)
{
	int ii, jj;
	int startRow	= start/COL_MEMORY_SIZE; //show memory's left side memory's start row
	int endRow		= end/COL_MEMORY_SIZE;	 //show memory's left side memory's end row
	int currentSpot;

	for( ii = startRow; ii <= endRow; ii++){
		// show row
		printf("%05X   ", ii*16);

		// show memory detail
		for( jj = 0; jj < COL_MEMORY_SIZE; jj++){
			currentSpot	= ii*COL_MEMORY_SIZE + jj; // calculate current pointer
			if( currentSpot >= start && currentSpot <= end ){
				printf("%02X ", (*memory)[ii][jj]);
			}
			else{
				printf("   ");
			}
		}
		printf("; ");
		//show memory -> ASCII code
		for( jj = 0; jj < COL_MEMORY_SIZE; jj++){
			currentSpot	= ii*COL_MEMORY_SIZE + jj;
			if( currentSpot >= start && currentSpot <= end)
			{
				//jus show ASCII CODE from 0x20 to ox7E
				if( (*memory)[ii][jj] >= '\x20' && (*memory)[ii][jj] <= '\x7E') 
				{
					printf("%c", (*memory)[ii][jj]);
				}
				else
				{
					printf(".");
				}
			}
			else
			{
				printf(".");
			}
		}
		printf("\n");
	}

}

// set memory.
void SetMemory(unsigned char*** memory, int start, int end, unsigned char value)
{
	int ii, jj;
	int startRow    = start/COL_MEMORY_SIZE;
	int endRow      = end/COL_MEMORY_SIZE;
	int currentSpot;

	for( ii = startRow; ii <= endRow; ii++){
		for( jj = 0; jj < COL_MEMORY_SIZE; jj++){
			currentSpot = ii*COL_MEMORY_SIZE + jj; // calculate current pointer

			if( currentSpot >= start && currentSpot <= end ){
				(*memory)[ii][jj] = value;
			}
		}
	}

}

// insert history using linked list
void InsertHistory(char* cmd, int index)
{
	historyNode* newNode;
	newNode = (historyNode*)malloc(sizeof(historyNode)*1);

	strcpy( newNode->historyCmd, cmd);
	newNode->historyIndex = index;
	newNode->next = NULL;

	if( headNode == NULL ){
		headNode = tailNode = newNode;
	}
	else{
		tailNode->next = newNode;
		tailNode = newNode;
	}

}

// show history list
void ShowHistory(void)
{
	historyNode* tmpNode;
	
	for( tmpNode = headNode; tmpNode; tmpNode = tmpNode->next){
		printf("%-4d %s\n",tmpNode->historyIndex ,tmpNode->historyCmd);
	}
}

// free history linked list
void FreeHistory(void)
{
	historyNode* tmpNode;

	while(headNode){
		tmpNode = headNode;
		headNode = headNode->next;
		free(tmpNode);
	}
}

// show directory
void ShowDir(void)
{
	DIR *dir;
	struct dirent *ent;
	struct stat fileInfo;

	dir = opendir ("./");

	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			printf ("%s", ent->d_name);
			stat(ent->d_name, &fileInfo);

			if( S_ISDIR(fileInfo.st_mode) ) // if directory
				printf("/\n");
			if( S_ISREG(fileInfo.st_mode) ){ // if file
				if( fileInfo.st_mode & S_IEXEC ) //if excutable file
					printf("*\n");
				else
					printf("\n");
			}
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
	}
}

// remove empty space
void TrimAll(char * input, char * output){
	int ii=0, jj=0;

	for( ii = 0; ii < strlen(input); ii++ ){
		if(input[ii] ==' ')
			continue;
		if(input[ii] =='\t')
			continue;
		output[jj++] = input[ii];
	}
	output[jj]='\0';
}
// make tokenArray
// [ex : dump 5a, 5b => rowData: "5a,5b" => tokenArray[0]=5a, tokenArray[1]=5b]
int MakeTokenArray(char (*tokenArray)[10], char* rowData)
{
	char* token;
	int ii = 0;
	
	token = strtok(rowData,",");
	while(token){
		strcpy(tokenArray[ii++],token);
		token = strtok(NULL,",");
	}
	return ii;
}
/*
int GetArrayLength(char (*tokenArray)[10])
{
	int ii;
	int arrayLength = 0;
	for( ii = 0; ii < 10; ii++){
		if( tokenArray[ii][0] != '\0' )
			arrayLength++;
		else
			break;
	}
	return arrayLength;
}
*/

//translate hexdicimal to decimal
int TransHex2Dec (char* Hex)
{
	unsigned char digit; // single digit
	int temp; // store integer temporary
	int ii, jj, sum = 0;
	int length = strlen(Hex); // token length

	for (ii = length - 1; ii >= 0; ii--) {
		// get last digit from token
		digit = Hex[ii];

		// change hexadecimal character to integer form
		// save the value in temporary variable
		if (digit >= 'A' && digit <= 'F')
			digit = 10 + digit - 'A';
		else if (digit >= 'a' && digit <= 'f')
			digit = 10 + digit - 'a';
		else if (digit >= '0' && digit <= '9')
			digit -= '0';
		else
			return -1;

		temp = (int)digit;
		// multiple decimal 16 in proportion to the digit location
		// accumulate sum
		for (jj = 0; jj < length - ii - 1; jj++)
			temp *= 16;
		sum += temp;
	}

	return sum;

}

// make hashkey
int MakeHashKey(char* key)
{
	int ii;
	int hash=0;
	int keyLength = strlen(key);

	for( ii = 0; ii < keyLength; ii++)
		hash = hash*29 + key[ii];

	return hash % HASHSIZE;
}

// make hashtable
void MakeHashTable(void)
{
	FILE *readFP = fopen("./opcode.txt","r");
	char opcode[8+1];
	char format[8+1];
	char mnemonic[8+1];
	int hashKey;
	hashNode* newNode;
	hashNode* ptrNode;
	if( readFP == NULL ){
		printf("opcode.txt doesn't exist!!\n");
		return;
	}

	while( !feof(readFP) ){
		memset(opcode, 0x00, sizeof(opcode));
		memset(format, 0x00, sizeof(format));
		memset(mnemonic, 0x00, sizeof(mnemonic));

		//read file data.
		fscanf(readFP, "%s\t\t%s\t\t%s\n", opcode, mnemonic, format); 
		hashKey = MakeHashKey(mnemonic);

		newNode = (hashNode*)malloc(sizeof(hashNode));
		strcpy(newNode->mnemonic, mnemonic);
		newNode->opcode = TransHex2Dec(opcode);
		newNode->nextNode = NULL;
		
		//insert to linkedlist
		if(hashTable[hashKey] == NULL){
			hashTable[hashKey] = newNode;
		}
		else{
			for( ptrNode = hashTable[hashKey]; ptrNode->nextNode; ptrNode = ptrNode->nextNode );
			ptrNode->nextNode = newNode;
		}
	}
}
// free HashTable
void FreeHashTable(void)
{
	int ii=0;
	hashNode* tmpNode = NULL;
	hashNode* headNode= NULL;

	for( ii = 0; ii < HASHSIZE; ii++){
		headNode = hashTable[ii];
		while( headNode ){
			tmpNode = headNode;
			headNode = headNode->nextNode;
			free(tmpNode);
		}
	}
}
// show HashTable
void ShowHashTable(void)
{
	int ii, jj=0;
	hashNode* ptrNode;

	for( ii = 0; ii < HASHSIZE; ii++){
		printf("%2d : ", ii);
		for( ptrNode = hashTable[ii], jj=0; ptrNode; ptrNode = ptrNode->nextNode ){
			if( jj++ == 0 )
				printf("[%s,%2X]", ptrNode->mnemonic, ptrNode->opcode);
			else
				printf(" -> [%s,%2X]", ptrNode->mnemonic, ptrNode->opcode);
		}
		printf("\n");
	}
}
// search Opcode 
int SearchOpcode(char* mnemonic)
{
	hashNode* pNode;
	int hashKey;
	int flag = 0;

	hashKey = MakeHashKey(mnemonic);
	
	for( pNode = hashTable[hashKey]; pNode; pNode = pNode->nextNode){
		if(!strcmp(pNode->mnemonic, mnemonic) ){
				printf("opcode is %2X\n", pNode->opcode);
				flag = 1;
				break;
		}
	}
	if( flag ==0 )
		return -1;
	else
		return 0;
}
// check input
int CheckInput(char *rowData)
{
	int ii = 0;
	for( ii = 0; ii < strlen(rowData); ii++){
		if( rowData[ii] == 44 ) // '.'
			continue;
		else if (rowData[ii] == '\n' )
			continue;
		else if( rowData[ii] >=48 && rowData[ii] <= 57) // 0~9
			continue;
		else if( rowData[ii] >=65 && rowData[ii] <= 90) // A~Z
			continue;
		else if( rowData[ii] >=97 && rowData[ii] <= 122) // a~z
			continue;
		else{
			return -1;
		}
	}
	// bad case [ex: 5,,7]
	for( ii = 0; ii < strlen(rowData)-1; ii++){
		if( rowData[ii] == ',' && rowData[ii+1] == ',')
			return -1;
	}
	// bad case [ex: ,5]
	if(rowData[0] == ',')
		return -1;
	// bad case [ex: 5,]
	if(rowData[strlen(rowData)-1] == ',')
		return -1;

	return 0;
}

// check parameter is valid!!
int CheckParameter(char(*tokenArray)[10], int arrayCount)
{
	int ii, jj;

	for( ii = 0; ii < arrayCount; ii++){
		for( jj = 0; jj< strlen(tokenArray[ii]); jj++){
			if( tokenArray[ii][jj] >= 48 && tokenArray[ii][jj] <= 57 ) // 0~9
				continue;
			else if( tokenArray[ii][jj] >= 65 && tokenArray[ii][jj] <= 70 ) // A~F
				continue;
			else if( tokenArray[ii][jj] >= 97 && tokenArray[ii][jj] <= 102) // a~f
				continue;
			else{
				return -1; // invalid parameter
			}
		}
	}
	return 0;
}
// check parameter is AA   BB(doesn't exist comma)
int CheckEmpty(char* rowData)
{
	int ii = 0;
	int charFlag = 0;
	int commaFlag = 0;
	int emptyFlag = 0;
	
	for( ii = 0; ii < strlen(rowData); ii++){
		if(charFlag == 1 && emptyFlag == 1 && commaFlag == 0){
			if( rowData[ii] >=48 && rowData[ii] <= 57) // 0~9
				return -1;
			else if( rowData[ii] >=65 && rowData[ii] <= 90) // A~Z
				return -1;
			else if( rowData[ii] >=97 && rowData[ii] <= 122) // a~z
				return -1;

			if( rowData[ii] == ','){
				charFlag = 0;
				emptyFlag = 0;
				commaFlag = 0;
			}
		}
		else{
			if( charFlag == 1 ){
				if( rowData[ii] == ',')
					commaFlag = 1;
				if( rowData[ii] == ' ')
					emptyFlag = 1;
			}
			if( rowData[ii] >=48 && rowData[ii] <= 57) // 0~9
				charFlag = 1;
			else if( rowData[ii] >=65 && rowData[ii] <= 90) // A~Z
				charFlag = 1;
			else if( rowData[ii] >=97 && rowData[ii] <= 122) // a~z
				charFlag = 1;
		}
	}
	return 0;

}
