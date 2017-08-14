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
	char* fileName;
	char inputCmd[64+1];
	char tempCmd[64+1];
	int  ii;
	char tempToken[64+1];
	char tokenArray[10][10];
	char loaderList[10][20];
	int loaderListCount;
	int arrayLength;
	int start, end, value;
	int result;
	int _isExist = -1;
	struct stat fileInfo;

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

		FreeSymbolTable(symbTableNodeHead);
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

		TrimAll(&inputCmd[11], tempToken);

		// in exsit parameter case
		if( strlen(tempToken)!= 0 ){
			printf("Invalid input!!\n");
			return -1;
		}

		InsertHistory(inputCmd, historyCount++);

		ShowHashTable();
	}
	else if( !strcmp("type", token) ){
		
		TrimAll(&inputCmd[5], tempToken);

		arrayLength = MakeTokenArray(tokenArray, tempToken);
		if( arrayLength == 1 ){
			ShowSource(tokenArray[0]);	
			InsertHistory(inputCmd, historyCount++);
		}
		else{
			printf("Invalid parameter!\n");
			return -1;
		}

	}
	else if( !strcmp("assemble", token) ){
		
		
		TrimAll(&inputCmd[9], tempToken);
		arrayLength = MakeTokenArray(tokenArray, tempToken);

		if( arrayLength == 1 ){
			// Is  tokenArray[0] file?
			_isExist = stat(tokenArray[0], &fileInfo);
			if( _isExist == -1 ){
				printf(" [%s] does not exist !!\n", tokenArray[0]);
				return -1;
			}
		
			baseFlag = 0;
			symbTableNodeHead = NULL;
			symbTableNodeTemp = NULL;
			result = PassOne(tokenArray[0]);
			fileName = strtok(tokenArray[0],".");	
			if( result == 0 ){
				result = PassTwo(fileName);
				if( result == 0 ){
					InsertHistory(inputCmd, historyCount++);
					//symbTableNodeHead = symbTableNodeTemp;
				}
				else{
					printf("Fail to assemble\n");	
					symbTableNodeTemp = symbTableNodeHead;
					FreeSymbolTable(symbTableNodeTemp);
					symbTableNodeHead = NULL;
				}
			}
			else{
				printf("Fail to assemble\n");	
				symbTableNodeTemp = symbTableNodeHead;
				FreeSymbolTable(symbTableNodeTemp);
				symbTableNodeHead = NULL;
			}
		}   
		else{
			printf("Invalid parameter!\n");
			return -1;
		} 
	}
	else if( !strcmp("symbol", token) ){

		TrimAll(&inputCmd[7], tempToken);
		arrayLength = MakeTokenArray(tokenArray, tempToken);

		if( arrayLength == 0 ){
			ShowSymbolTable();
			printf("\n");
			InsertHistory(inputCmd, historyCount++);
		}
		else{
			printf("Invalid parameter!\n");
			return -1;
		}
	}
	else if( !strcmp("progaddr", token) ){

		TrimAll(&inputCmd[8], tempToken);
		arrayLength = MakeTokenArray(tokenArray, tempToken);

		if( arrayLength == 1 ){

			if(TransHex2Dec(tokenArray[0]) < 0){
				printf("input proaddr is wrong!\n");
				return -1;
			}
			else if(TransHex2Dec(tokenArray[0]) >= TransHex2Dec("FFFFFF")){
				printf("input proaddr is wrong!\n");
				return -1;
			}
			progAddr = TransHex2Dec(tokenArray[0]);
		}
		else{
			printf("Invalid parameter!\n");
			return -1;
		}
	}
	else if( !strcmp("loader", token) ){
		strcpy(tempToken, &inputCmd[7]);
		arrayLength = MakeTokenArray(tokenArray, &inputCmd[7]);

		if( arrayLength == 1 ){

			loaderListCount = MakeLoaderList(loaderList, tempToken);

			// confirm *.obj file exist.
			for( ii = 0; ii < loaderListCount; ii++){

				_isExist = stat(loaderList[ii], &fileInfo);

				if( _isExist == -1 ){
					printf(" [%s] does not exist !!\n", loaderList[ii]);
					return -1;
				}
			}

			//clear ESYMTAB
			FreeEsymTable();
			// Loader PASS 1
			result = LoaderPass1(loaderList, loaderListCount);
			if( result == -1 ){
				printf("loader pass1 error!\n");	
				return -1;
			}
			// Loader PASS 2
			
			result = LoaderPass2(memory, loaderList, loaderListCount);
			if( result == -1 ){
				printf("loader pass2 error!\n");	
				return -1;
			}
			InsertHistory(inputCmd, historyCount++);
			
		}
		else{

			printf("Invalid parameter!\n");
			return -1;

		}
	}
	else if( !strcmp("run", token) ){
		TrimAll(&inputCmd[3], tempToken);
		arrayLength = MakeTokenArray(tokenArray, tempToken);

		if( arrayLength == 0 ){
			//printf("[%d]\n", progSize);
			if(progSize == 0){
				printf("progarm is not load!\n");
				return -1;
			}
			result = RunMemory(memory);
			InsertHistory(inputCmd, historyCount++);
			if( result == 2 ){

			}
			else{
				printf("[End program]\n");
			}
			
		}
		else{
			printf("Invalid parameter!\n");
			return -1;
		}
	}
	else if( !strcmp("bp", token) ){
		TrimAll(&inputCmd[2], tempToken);
		arrayLength = MakeTokenArray(tokenArray, tempToken);

		if( arrayLength == 0 ){
			// Show break pointer List
			ShowBreakPoint();
		}
		else if( arrayLength == 1 ){

			if( !strcmp("clear", tokenArray[0]) ){
				// clear breakPoint
				result = FreeBreakPoint();
				if( result == -1 ){
					printf("fila to clear break point list\n");
					return -1;
				}
				printf("[ok] clear all breakpoints\n");
				InsertHistory(inputCmd, historyCount++);
			}
			else{
				result = SearchBreakPoint(TransHex2Dec(tokenArray[0]));
				if( result == -1 ){
					printf("[%s] is already in breakpoint List\n", tokenArray[0]);
					return -1;
				}
				else{
					
					if(TransHex2Dec(tokenArray[0]) < 0){
						printf("input proaddr is wrong!\n");
						return -1;
					}
					else if(TransHex2Dec(tokenArray[0]) >= TransHex2Dec("FFFFFF")){
						printf("input proaddr is wrong!\n");
						return -1;
					}

					InsertBreakPoint(TransHex2Dec(tokenArray[0]));
					printf("[ok] create breakpoint %X\n", TransHex2Dec(tokenArray[0]));
					InsertHistory(inputCmd, historyCount++);
				}
			}
		}
		else{
			printf("Invalid parameter!\n");
			return -1;
		}
	}
	else
	{
		printf("Cmd is wrong!!!\n");
		return -1;
	}
	return 0;
}
// Show file's inner content
void ShowSource(char* fileName)
{
	char temp[1024];
	struct stat fileInfo;
	FILE* readFP;
	int _isExist = -1;

	memset(temp, 0x00, sizeof(temp));
	_isExist = stat(fileName, &fileInfo);

	if( !_isExist ){
		if( S_ISREG(fileInfo.st_mode) ){
			readFP = fopen(fileName, "r");

			while( !feof(readFP) ){
				memset(temp, 0x00, sizeof(temp));
				fgets(temp, sizeof(temp),readFP);
				printf("%s", temp);
			}
		}
		else{
			printf("[%s] isn't file \n", fileName);
		}
	}
	else{
		printf("there is not exist [%s]\n", fileName);
	}

}
// show cmdlist
void ShowHelp(void)
{
	int ii;
	char *helpCmdList[] =  {"h[elp]", "d[ir]", "q[uit]", "hi[story]",
							"du[mp] [start, end]","e[dit] address, value",
							"f[ill] start, end, value", "reset",
							"opcode mnemonic", "opcodelist",
							"assemble filename", "type filename", "symbol",
							"progaddr [address]", "loader [object filename1] [...]",
							"run", "bp [address]"
							};

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

// get Memory value
int GetMemory(unsigned char*** memory, int start)
{
	int ii, jj;
	char value[2+1];
	int row	= start/COL_MEMORY_SIZE; //show memory's left side memory's start row
	int col	= start%COL_MEMORY_SIZE; //show memory's left side memory's end row

	return (*memory)[row][col];
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
void FreeSymbolTable(symbTableNode* symbNode)
{
	symbTableNode* tmpNode;

	while(symbNode){
		tmpNode = symbNode;
		symbNode = symbNode->next;
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

// remove all empty space
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

void Trim(char * input){
	int ii=0, jj=0;

	for( ii = 0; ii < strlen(input); ii++ ){
		if(input[ii] ==' ')
			continue;
		if(input[ii] =='\t')
			continue;
		input[jj++] = input[ii];
	}
	input[jj]='\0';
}

//remove right empty space
//remove right empty space
void rtrim(char* input)
{
	int ii;
	for( ii = strlen(input)-1; ii >= 0; ii-- ){
		if( input[ii] != ' ' ){
			input[ii+1] = '\0';
			break;
		}
	}

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
int MakeLoaderList(char (*LoaderList)[20], char* rowData)
{
	char* token;
	int ii = 0;
	
	token = strtok(rowData," \t");
	while(token){
		strcpy(LoaderList[ii++],token);
		token = strtok(NULL," \t");
	}
	return ii;
}

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
		strcpy(newNode->format, format);
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
//Search Opcode All
char* SearchOpcodeAll(int opcode)
{
	int ii;
	char temp[2+1];

	hashNode* ptrNode;

	for( ii = 0; ii < HASHSIZE; ii++){
		for( ptrNode = hashTable[ii]; ptrNode; ptrNode = ptrNode->nextNode ){
			sprintf(temp,"%0X", ptrNode->opcode);
			if( opcode == TransHex2Dec(temp))
				return ptrNode->mnemonic;
		}
	}
	return "-1";
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
// search Opcode 
int SearchOpcodeNo(char* mnemonic)
{
	hashNode* pNode;
	int hashKey;
	int flag = -1;

	hashKey = MakeHashKey(mnemonic);
	
	for( pNode = hashTable[hashKey]; pNode; pNode = pNode->nextNode){
		if(!strcmp(pNode->mnemonic, mnemonic) ){
				flag = pNode->opcode;
				break;
		}
	}
	return flag;
	/*
	if( flag ==0 )
		return -1;
	else
		return flag;
		*/
}
// search Opcode 
int SearchOpcodeFormat(char* mnemonic)
{
	hashNode* pNode;
	int hashKey;
	int flag = 0;

	hashKey = MakeHashKey(mnemonic);

	for( pNode = hashTable[hashKey]; pNode; pNode = pNode->nextNode){
		if(!strcmp(pNode->mnemonic, mnemonic) ){
			if ( !strcmp(pNode->format, "1") )
				flag = 1;
			else if( !strcmp(pNode->format, "2") )
				flag = 2;
			else
				flag = 3;
			break;
		}
	}
	if( flag ==0 )
		return -1;
	else
		return flag;
}
// check input
int CheckInput(char *rowData)
{
	int ii = 0;
	for( ii = 0; ii < strlen(rowData); ii++){
		if( rowData[ii] == 44 ) // '.'
			continue;
		//else if (rowData[ii] == '#')
		//	continue;
		//else if (rowData[ii] == '@')
		//	continue;
		//else if (rowData[ii] == '\n' )
		//	continue;
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
// Show Symbole Table
void ShowSymbolTable(void)
{
	symbTableNode* ptrNode;
	for( ptrNode = symbTableNodeHead; ptrNode; ptrNode = ptrNode->next){
		if(ptrNode->next == NULL){
			printf("\t%s\t%04X",ptrNode->label ,ptrNode->location);
		}
		else{
			printf("\t%s\t%04X\n",ptrNode->label ,ptrNode->location);
		}

	}
}
// Insert Symbol DESC
int InsertSymbolTable(int locationCount, char *label)
{
	symbTableNode* newNode;
	symbTableNode* ptrNode;

	newNode = (symbTableNode*)malloc(sizeof(symbTableNode));
	strcpy(newNode->label, label);
	newNode->location	= locationCount;
	newNode->next =NULL;

	if( symbTableNodeHead == NULL ){
		symbTableNodeHead = newNode;
	}
	else if( strcmp(label,symbTableNodeHead->label) > 0 ){
		newNode->next = symbTableNodeHead;
		symbTableNodeHead = newNode;
	}
	else{
		ptrNode = symbTableNodeHead;
		while(1){
			if( !ptrNode->next ){
				ptrNode->next =newNode;
				break;
			}
			else if( strcmp(label, ptrNode->next->label) > 0 ){
				newNode->next = ptrNode->next;
				ptrNode->next = newNode;
				break;
			}
			else{
				ptrNode = ptrNode->next;
			}

		}
	}
}
// SearchSymbol Table
int SearchSymbolTable(char *label)
{   
	symbTableNode* ptrNode;
	int flag = -1;

	for( ptrNode = symbTableNodeHead; ptrNode; ptrNode = ptrNode->next ){
		if( !strcmp(ptrNode->label, label) ){
			flag = ptrNode->location;
			//printf("In function: [%s]\n", label);
			break;
		}
	}
	if( flag == -1 )
		return -1;
	else
		return flag;
}
// check mnemonic is START, BASE, END ...
int IsIndicator(char* mnemonic)
{
	int result;
	if( !strcmp(mnemonic, "WORD") || !strcmp(mnemonic,"BYTE") || !strcmp(mnemonic, "RESW") || !strcmp(mnemonic, "RESB") )
		return -1;
	else{
		if( mnemonic[0] == '+')
			result = SearchOpcodeFormat(&mnemonic[1]);
		else
			result = SearchOpcodeFormat(mnemonic);//format1 = 1, 2,3

		if( result != -1 )
			return -1;
		else
			return 0;
	}

}
// Calculate Address
int CalculateAddress(int locationCount, char* mnemonic, char* operand)
{
	int result;
	// 지시어 인지, opcode 인지 구분
	if ( !strcmp(mnemonic, "START") ){
		locationCount;
	}
	else if( !strcmp(mnemonic, "BASE") ){
		locationCount;
	}
	else if( !strcmp(mnemonic, "WORD") ){
		locationCount += 3;
	}
	else if( !strcmp(mnemonic, "BYTE") ){
		if( operand[0] == 'C' )
			locationCount += ((int)strlen(operand)-3);
		else if( operand[0] == 'X' )
			locationCount += ((int)strlen(operand)-3)/2;
	}
	else if( !strcmp(mnemonic, "RESW") ){
		locationCount += atoi(operand)*3;
	}
	else if( !strcmp(mnemonic, "RESB") ){
		locationCount += atoi(operand)*1;;
	}
	else{
		result = SearchOpcodeFormat(mnemonic);
		if( mnemonic[0] == '+' ){
			result = SearchOpcodeFormat( &mnemonic[1]);
			if( result == 3 ){
				locationCount += 4;
			}
			else{
				return -1;
			}
		}
		else if( result == 3 ){
			locationCount += 3;
		}
		else if( result == 2 ){
			locationCount += 2;
		}
		else if( result == 1 ){
			locationCount += 1;
		}
		else{
			return -1;
		}

	}

	return locationCount;

}
// pass one algorithm
int PassOne(char* fileName)
{
	char label[16+1];
	char mnemonic[16+1];
	char operand[64+1];
	char input[100];
	char temp[100];
	int ii;
	int result;
	int locationCount;
	int startAddress;
	int line = 5;
	FILE* readFP = fopen(fileName, "r");
	FILE* writeFP = fopen("./pass1.asm", "w");
	//symbTableNodeHead = NULL;


	while( !feof(readFP) ){
		memset( temp,		0x00, sizeof(temp)		);
		memset( input,      0x00, sizeof(input)     );
		memset( label,      0x00, sizeof(label)     );
		memset( mnemonic,   0x00, sizeof(mnemonic)  );
		memset( operand,    0x00, sizeof(operand)   );

		fgets(input, sizeof(input), readFP);
		input[strlen(input)-1] = '\0';

		// divide input
		for( ii = 0; ii < 7; ii++ ){
			if( input[0] == ' ' )
				break;
			label[ii] = input[ii];
		}

		for ( ii = 7; ii < 14; ii++){
			if( input[7] == ' ' )
				break;
			mnemonic[ii-7] = input[ii];
		}

		for ( ii = 14; ii < strlen(input); ii++) {
			if( input[14] == ' ' )
				break;
			operand[ii-14] = input[ii];
		}
		rtrim(label);
		rtrim(mnemonic);
		rtrim(operand);

		result = checkLabel(label);
		if( result == -1 ){
			printf("label is wrong!![%d][%s]\n", line,input);
			return -1;
		}
		
		if( !strcmp(mnemonic,"START") ){
			locationCount = TransHex2Dec(operand);
			startAddress = TransHex2Dec(operand);
			fprintf(writeFP, "%3d   %04X %-7s %-7s %-10s\n",line, locationCount, label, mnemonic,operand);
			line += 5;
		}
		else{
			if( label[0] != '.' ){
				// this is not a comment line
				if( !strcmp(mnemonic, "END") ){
					fprintf(writeFP, "%3d    %-11s %-7s %-10s\n",line, label, mnemonic,operand);

					sizeOfAssem = locationCount - startAddress;
					fclose(readFP);
					fclose(writeFP);
					return 0;
				}

				if( strlen(label) != 0 ){
					// there is a symbol in the LABEL field
					if( SearchSymbolTable(label) != -1 ){
						// set error flag (duplicate symbol);
						printf("duplicate label!![%d][%s]\n", line,input);
						return -1;
					}
					else{
						InsertSymbolTable(locationCount, label); // insert locationCount, label
					}
				}
				//search OPTAB for OPCODE

				if( IsIndicator(mnemonic) == 0 ) // this is directive EX) START, END, BASE
					fprintf(writeFP, "%3d    %11s %-7s %-10s\n",line, label, mnemonic,operand);
				else
					fprintf(writeFP, "%3d   %04X %-7s %-7s %-10s\n",line, locationCount, label, mnemonic,operand);
				locationCount = CalculateAddress(locationCount, mnemonic, operand);
				if( locationCount < 0 ){
					printf("this line is wrong!![%d][%s]\n", line, input);
					return -1;
				}
				line += 5;	
			}
			else{
				strcat(temp, mnemonic);
				strcat(temp, operand);
					
				fprintf(writeFP, "%3d   %-11s  %-30s \n",line,label, temp);
				line += 5;
			}
		}
	}
	return 0;
}
// passtwo algorithm
int PassTwo(char* fileName)
{
	char line[4+1];
	char location[4+1];
	char label[16+1];
	char mnemonic[16+1];
	char operand[64+1];
	char objectCode[32+1];
	char input[100];
	char temp[100];
	char tBodyBuffer[200];
	char listFileName[32];
	char objFileName[32];
	int modificationAddress[10] = {0};
	int modificationCount = 0;
	int currentLocation;
	int ii,jj;
	int result;
	int opcode;
	int startAddress;
	int pointerCounter;
	int registerB;
	int registerL;
	int tRecordLength = 0;
	FILE* readFP;
	FILE* writeLstFP;
	FILE* writeObjFP;
	
	sprintf(listFileName,	"%s.lst", fileName);
	sprintf(objFileName,	"%s.obj", fileName);

	readFP		= fopen("pass1.asm",	"r"); // read intermediate file
	writeLstFP	= fopen(listFileName,	"w");
	writeObjFP	= fopen(objFileName,	"w");


	memset(tBodyBuffer, 0x00, sizeof(tBodyBuffer) );
	while( !feof(readFP) ){

		memset( temp,		0x00, sizeof(temp)		);
		memset( input,      0x00, sizeof(input)     );
		memset( line,		0x00, sizeof(line)		);
		memset( location,	0x00, sizeof(location)  );
		memset( label,      0x00, sizeof(label)     );
		memset( mnemonic,   0x00, sizeof(mnemonic)  );
		memset( operand,    0x00, sizeof(operand)   );
		memset( objectCode, 0x00, sizeof(objectCode));

		fgets(input, sizeof(input), readFP);
		input[strlen(input)-1] = '\0';

		if(strlen(input) == 0)
			continue;

		// divide input
		for( ii = 0, jj=0 ; ii< 3; ii++){
			if(input[ii] == ' ')
				continue;
			line[jj++] = input[ii];
		}
		for( ii = 6, jj=0; ii< 11; ii++){
			location[jj++] = input[ii];
		}
		for( ii = 11, jj=0; ii< 19; ii++){
			label[jj++] = input[ii];
		}
		for( ii = 19, jj=0; ii< 27; ii++){
			mnemonic[jj++] = input[ii];
		}
		for( ii = 27, jj=0; ii< strlen(input); ii++){
			operand[jj++] = input[ii];
		}

		rtrim(line);
		rtrim(location);
		rtrim(label);
		rtrim(mnemonic);
		rtrim(operand);
		currentLocation = TransHex2Dec(location);;

		if( !strcmp(mnemonic, "START") ){
			startAddress  = TransHex2Dec(operand);
			fprintf(writeLstFP, "%3s %04X %-8s %-7s %-10s \n",line, currentLocation, label, mnemonic,operand);
			fprintf(writeObjFP, "%c%-6s%06X%06X\n",'H',label,startAddress, sizeOfAssem);
		}
		else{
			if( location[0]!= '.' ){// not comment
				if( !strcmp(mnemonic,"BASE") ){
					// set registerB and baseFlag
					registerB = SearchSymbolTable(operand);
					baseFlag = 1;
				}
				if( !strcmp(mnemonic,"+JSUB") || !strcmp(mnemonic,"JSUB") ){
					// set registerL
					if( mnemonic[0] == '+' )
						registerL = currentLocation + 4;
					else
						registerL = currentLocation + 3;
				}


				// Make Object Code
				if( !strcmp(mnemonic, "BYTE") ){
					if( operand[0] == 'C' )
						pointerCounter = currentLocation + ((int)strlen(operand)-3);
					else if( operand[0] == 'X' )
						pointerCounter = currentLocation + ((int)strlen(operand)-3)/2;
					else{
						printf("Error[1]\n");
						ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
						return -1;
					}
					result = MakeObjectCode(objectCode,0, currentLocation, mnemonic, operand, pointerCounter, registerB);
					if( result == -1 ){
						printf("Error[2]\n");
						ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
						return -1;
					}
				}
				else if( !strcmp(mnemonic, "WORD") ){
					pointerCounter = currentLocation + 3;
					result = MakeObjectCode(objectCode,0, currentLocation, mnemonic, operand, pointerCounter, registerB);
					if( result == -1 ){
						printf("Error[3]\n");
						ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
						return -1;
					}
				}
				else if( !strcmp(mnemonic, "RESB") ){
					pointerCounter = currentLocation + atoi(operand)*1;
				}
				else if( !strcmp(mnemonic, "RESW") ){
					pointerCounter = currentLocation + atoi(operand)*3;
				}
				else{
					if( mnemonic[0] == '+' ){ // format 4
						opcode = SearchOpcodeNo(&mnemonic[1]);	
						result = SearchOpcodeFormat(&mnemonic[1]);

						if( result == 3 ){
							pointerCounter = currentLocation + 4;
							result = MakeObjectCode(objectCode,4, currentLocation, mnemonic, operand, pointerCounter, registerB);
							if( result == -1 ){
								printf("Error[4]\n");
								ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
								return -1;
							}
							// save modificaion record value 
							if( operand[0] != '#')
								modificationAddress[modificationCount++] = currentLocation+1;
						}
						else{
							printf("Error[5]\n");
							ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
							return -1;
						}
					}
					else{
						opcode = SearchOpcodeNo(mnemonic);	
						result = SearchOpcodeFormat(mnemonic);
				
						if( result == 3 ){ // format 3
							pointerCounter = currentLocation + 3;
							result = MakeObjectCode(objectCode,3, currentLocation, mnemonic, operand, pointerCounter, registerB);
							if( result == -1 ){
								printf("Error[6]\n");
								ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
								return -1;
							}
						}
						else if( result == 2 ){ // format 2
							pointerCounter = currentLocation + 2;
							result = MakeObjectCode(objectCode,2, currentLocation, mnemonic, operand, pointerCounter, registerB);
							if( result == -1 ){
								printf("Error[7]\n");
								ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
								return -1;
							}
						}
						else if( result == 1 ){ // format 1
							pointerCounter = currentLocation + 1;
							result = MakeObjectCode(objectCode,1, currentLocation, mnemonic, operand, pointerCounter, registerB);
							if( result == -1 ){
								printf("Error[8]\n");
								ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
								return -1;
							}
						}
						else
						{
							if( !strcmp(mnemonic,"BASE") ){ 
								fprintf(writeLstFP, "%3s %-13s %-7s %-10s \n",line, label, mnemonic,operand);
								continue;
							}
							else if( !strcmp(mnemonic,"END") ){
								fprintf(writeLstFP, "%3s %-13s %-7s %-10s \n",line, label, mnemonic,operand);

								if( tRecordLength != 0 ) // write Obj File
									fprintf(writeObjFP, "%02X%s\n", tRecordLength/2, tBodyBuffer);

								for( ii = 0; ii < modificationCount; ii++)
									fprintf(writeObjFP, "%c%06X%02X\n",'M', modificationAddress[ii],5);

								fprintf(writeObjFP, "%c%06X\n",'E',startAddress);
								printf("output file : [%s], [%s]\n", listFileName, objFileName);

								fclose(writeObjFP);
								fclose(readFP);
								fclose(writeLstFP);
								return 0;

							}
							else{
								printf("Error[9]\n");
								ExitPassTwo(line, input, fileName, readFP, writeLstFP, writeObjFP);
								return -1;
							}
						}
					}
				}
				// write Lst File 
				fprintf(writeLstFP, "%3s %04X %-8s %-7s %-10s %-10s\n",line, currentLocation, label, mnemonic,operand,objectCode);

				// write Obj File
				if( !strcmp(mnemonic,"RESW") || !strcmp(mnemonic,"RESB") ){
					if( tRecordLength == 0 ){
						tRecordLength = 0;
						memset(tBodyBuffer, 0x00, sizeof(tBodyBuffer) );
					}
					else{
						fprintf(writeObjFP, "%02X%s\n", tRecordLength/2, tBodyBuffer);	
						tRecordLength = 0;
						memset(tBodyBuffer, 0x00, sizeof(tBodyBuffer) );
					}
				}	
				else{
					if( tRecordLength == 0 ){
						fprintf(writeObjFP, "%c%06X", 'T', currentLocation);
					}

					if( tRecordLength + strlen(objectCode) > MAX_RECORD_SIZE ){
						fprintf(writeObjFP, "%02X%s\n", tRecordLength/2, tBodyBuffer);

						// save before data;
						fprintf(writeObjFP, "%c%06X", 'T', currentLocation);
						tRecordLength = strlen(objectCode);
						memset(tBodyBuffer, 0x00, sizeof(tBodyBuffer));
						strcat(tBodyBuffer, objectCode);
					}
					else if( tRecordLength + strlen(objectCode) == 0){
						strcat(tBodyBuffer, objectCode);
						tRecordLength += strlen(objectCode);
						fprintf(writeObjFP, "%02X%s\n", tRecordLength/2, tBodyBuffer);
						// save before+ current data

						tRecordLength = 0;
						memset(tBodyBuffer, 0x00, sizeof(tBodyBuffer));
					}
					else{
						strcat(tBodyBuffer, objectCode);
						tRecordLength += strlen(objectCode);
					}
				}
			}
			else{ // comment
				strcat(temp, label);
				strcat(temp, mnemonic);
				strcat(temp, operand);
				fprintf(writeLstFP, "%3s %-5s %-30s \n",line,location, temp);
			}
		}
	}
}
// MakeObjectCode
int MakeObjectCode(char* objectCode,int format,  int location, char* mnemonic, char* operand, int pointerCounter, int registerB)
{
    char tokenArray[3][10];
	char tempString[8];
	char tempOperand[16+1];
	char relative[10];
	int registerNo[5];
	int	tempLength;
	int operandAddress;
	int arrayLength;
	int ii;
	int result;
	
	if( format == 4 ){

		sprintf(tempString, "%X", SearchOpcodeNo(&mnemonic[1]));
		if(operand[0] == '#' ){ 
			// immediate operand 
			// (n,i) = (0,1) (x,b,p,e) = (0,0,0,1)
			sprintf(objectCode, "%02X%X%05X", TransHex2Dec(tempString)+1,1,atoi(&operand[1]));
		}
		else{ 
			// simply address
			// (n,i) = (1,1) (x,b,p,e) = (0,0,0,1)
			//sprintf(objectCode, "%02X%X%05X", TransHex2Dec(tempString)+3,1,SearchSymbolTable(operand));

			operandAddress = SearchSymbolTable(operand);
			if( operandAddress == -1 )
				return -1;

			sprintf(objectCode, "%02X%X%05X", TransHex2Dec(tempString)+3,1,operandAddress);
		}
	}
	else if( format == 3 ){

		if(operand[0] == '#' ){
			// immediate operand 
			// (n,i) = (0,1) (x,b,p,e) = (0,1/0,1/0,0)
			operandAddress = SearchSymbolTable(&operand[1]);
			if( operandAddress == -1 ){ // ex) COMP #0

				if( atoi(&operand[1]) >= 4096)
					return -1;

				if( operand[1] >= '0' && operand[1] <='9' ){
					sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
					sprintf(objectCode, "%02X%X%03X", TransHex2Dec(tempString)+1,0,atoi(&operand[1]));
				}
				else{
					printf("error[1-1]");
					return -1;
				}
			}
			else{ // ex) LDB #LENGTH

				sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
				result = CalculateRelative(pointerCounter, registerB, operandAddress, relative);

				if( result == 1 ){ //pc
					//(x,b,p,e) = (0,0,1,0)
					sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+1,2,relative);
				}
				else if (result == 2){
					//(x,b,p,e) = (0,1,0,0)
					sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+1,4,relative);
				}
				else{
					printf("error[1-2]");
					return -1;
				}
			}
		}
		else if(operand[0] == '@'){
			// indirect addressing 
			// (n,i) = (1,0) (x,b,p,e) = (0,1/0,1/0,0)
			operandAddress = SearchSymbolTable(&operand[1]);
			if( operandAddress == -1 ){
				return -1;
			}
			else{

				sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
				result = CalculateRelative(pointerCounter, registerB, operandAddress, relative);
				
				if( result == 1 ){ // pc
					//(x,b,p,e) = (0,0,1,0)
					sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+2,2,relative);
				}
				else if( result == 2 ){ // basis
					//(x,b,p,e) = (0,1,0,0)
					sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+2,4,relative);
				}
				else{
					printf("error[1-3]");
					return -1;
				}
			}

		}
		else{
			// simple addressing 
			// (n,i) = (1,1) (x,b,p,e) = (1/0,1/0,1/0,0)
			if( !strcmp(mnemonic,"RSUB") ){

				sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
				sprintf(objectCode, "%02X%04X", TransHex2Dec(tempString)+3,0);
			}
			else{

				operandAddress = SearchSymbolTable(operand);
				if( operandAddress == -1 ){ // BUFFER,X case
					// (n,i) = (1,1) (x,b,p,e) = (1,1/0,1/0,0)

					//result = CheckEmpty(operand);
					Trim(operand);
					result = CheckInput(operand);
					if( result == -1 )
						return -1;

					strcpy(tempOperand, operand);
					arrayLength = MakeTokenArray(tokenArray, tempOperand);
					
					if( arrayLength == 2 ){
						operandAddress = SearchSymbolTable(tokenArray[0]);
						result = CalculateRelative(pointerCounter, registerB, operandAddress, relative);
						sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
		
						if( result == 1 ){ // pc
							// (n,i) = (1,1) (x,b,p,e) = (1,0,1,0)
							sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+3,8+2,relative);
						}
						else if( result == 2 ){ // basis
							// (n,i) = (1,1) (x,b,p,e) = (1,1,0,0)
							sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+3,8+4,relative);
						}
						else{
							printf("error[1-4]");
							return -1;
						}
					}
					else{
						printf("error[1-5]");
						return -1;
					}
				}
				else{
					// (n,i) = (1,1) (x,b,p,e) = (0,1/0,1/0,0)
					sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
					result = CalculateRelative(pointerCounter, registerB, operandAddress, relative);

					if( result == 1 ){// pc
						// (n,i) = (1,1) (x,b,p,e) = (0,0,1,0)
						sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+3,2,relative);
					}
					else if( result == 2){ //basis
						// (n,i) = (1,1) (x,b,p,e) = (0,1,0,0)
						sprintf(objectCode, "%02X%X%s", TransHex2Dec(tempString)+3,4,relative);
					}	
					else{
						printf("error[1-6]");
						return -1;
					}
				}
			}
		}
	}
	else if( format == 2 ){


		Trim(operand);
		if(strlen(operand) == 0)
			return -1;

		result = CheckInput(operand);
		if(result == -1)
			return -1;

		arrayLength = MakeTokenArray(tokenArray, operand);

		for( ii = 0; ii < arrayLength; ii++){
			Trim(tokenArray[ii]);

			if( tokenArray[ii][0] == 'A' )
				registerNo[ii] = 0;
			else if( tokenArray[ii][0] == 'X' )
				registerNo[ii] = 1;
			else if( tokenArray[ii][0] == 'L' )
				registerNo[ii] = 2;
			else if( tokenArray[ii][0] == 'B' )
				registerNo[ii] = 3;
			else if( tokenArray[ii][0] == 'S' )
				registerNo[ii] = 4;
			else if( tokenArray[ii][0] == 'T' )
				registerNo[ii] = 5;
			else if( tokenArray[ii][0] == 'F' )
				registerNo[ii] = 6;
			else if( !strcmp(tokenArray[ii],"PC") )
				registerNo[ii] = 8;
			else if( !strcmp(tokenArray[ii],"SW") )
				registerNo[ii] = 9;
			else {
				return -1;
			}
		}

		sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));

		if( arrayLength == 1){
			// format2's operand count is just one(ex: A)
			sprintf(objectCode,"%02X%X%X",TransHex2Dec(tempString), registerNo[0], 0 );
		}
		else if( arrayLength == 2){
			// format2's operand count is two (ex: A,S)
			sprintf(objectCode,"%02X%X%X",TransHex2Dec(tempString), registerNo[0], registerNo[1]);
		}
		else{
			return -1;
		}
	}
	else if (format == 1){

		sprintf(tempString, "%X", SearchOpcodeNo(mnemonic));
		sprintf(objectCode,"%02X",TransHex2Dec(tempString));
	}
	else{
		if( !strcmp(mnemonic, "BYTE") ){

			if( operand[0] == 'C' ){
				tempLength = strlen(operand) - 3; // sub length => C'' 
				for( ii = 0 ; ii< tempLength; ii++){
					memset(tempString, 0x00, sizeof(tempString));
					sprintf(tempString,"%2X", operand[ii+2]);
					strcat(objectCode, tempString);
				}
			}
			else if( operand[0] == 'X' ){
				tempLength = strlen(operand)-3;
				strncpy(objectCode, &operand[2], tempLength);
			}
		}
		else if( !strcmp(mnemonic, "WORD") ){
			sprintf(objectCode, "%06X", atoi(operand));
		}
		else{
			return -1;
		}
	}
	return 0;
}
// Calculate Relative address
int CalculateRelative(int pointerCounter, int registerB, int operandAddress, char* relative){

	char tempString[10];
	int result;
	int flag = 0;
	
	memset(tempString, 0x00, sizeof(tempString));

	if( pointerCounter-2048 <= operandAddress && operandAddress <= pointerCounter+2048){
		// calculate PC relative
		result = operandAddress-pointerCounter;	
		if( result < 0 ){
			// negative number
			sprintf(tempString,"%X", operandAddress-pointerCounter);
			strncpy(relative,&tempString[5],3);
			relative[3]='\0';
		}
		else{
			// posstive number
			sprintf(relative,"%03X", operandAddress-pointerCounter);
		}
		flag = 1; // pc relative
	}
	else if( baseFlag == 1 && operandAddress <= registerB+4095 ){
		//printf("[%d]\n", baseFlag);
		sprintf(relative,"%03X", operandAddress-registerB);
		flag = 2; // basis relative
	}
	else{
		printf("can't calculate address using pc relative or base relative\n");
		return -1;
	}
	return flag;
}
// ExitPassTwo When error is occured
void ExitPassTwo(char* line, char* input, char* fileName, FILE* readFP, FILE* writeFP1, FILE* writeFP2)
{
	char listFileName[32];
	char objFileName[32];

	sprintf(listFileName,   "./%s.lst", fileName);
	sprintf(objFileName,    "./%s.obj", fileName);

	printf("assemble code is worng[%s][%s]\n",line, input);
	fclose(readFP);
	fclose(writeFP1);
	fclose(writeFP2);

	remove(listFileName);
	remove(objFileName);
}
int checkLabel(char* label)
{
	int ii;
	int flag = 0;
	if( label[0] >= '0' && label[0] <='9')
		return -1;

	for( ii = 0; ii < strlen(label); ii++ ){
		if( label[ii] >= '0' && label[ii] <= '9' )
			continue;
		else if( label[ii] >='A' && label[ii] <= 'Z')
			continue;
		else if( label[ii] >='a' && label[ii] <= 'z')
			continue;
		else
			flag = 1;
	}

	if( label[0] == '.' )
		flag = 0;

	if( flag == 1 )
		return -1;
	else
		return 0;
}

int LoaderPass1(char (*objList)[20], int objCount)
{
	FILE* readFP;

	char objLineData[100];
	char dataType;

	char progName[6+1];
	char controlLength[6+1];
	char esymName[6+1];
	char esymAddr[6+1];

	int hashKey;
	int lineLength;
	
	int ii, jj, kk=1;
	int result = 0;

	int endflag = 0;

	esymNode* newNode;
	esymNode* ptrNode; 

	memset(progName, 0x00, sizeof(progName));
	memset(controlLength, 0x00, sizeof(controlLength));
	
	for ( ii = 0; ii < objCount; ii++ ){

		readFP = fopen(objList[ii],"r");

		if( ii == 0 ){
			controlAddr = progAddr;
		}
		else{
			controlAddr = controlAddr + TransHex2Dec(controlLength);
		}
		endflag = 0;
		kk = 1;

		while( !feof(readFP) ){

			dataType = '0';
			memset( objLineData, 0x00, sizeof(objLineData) );	

			fgets( objLineData, sizeof(objLineData), readFP );
			if (strlen(objLineData) == 1)
				objLineData[strlen(objLineData)] = '\0';
			else	
				objLineData[strlen(objLineData)-1] = '\0';
			dataType = objLineData[0];

			if(dataType == 'H'){

				strncpy(progName, &objLineData[1], 6);
				strncpy(controlLength, &objLineData[13],6);

				Trim(progName);
				// search essyNode
				result = SearchEsymTable(progName);
				if( result == -1 ){
					printf("duplicate external symbol\n");
					return -1;
				}
				else{
					// insert external symbol table;
					newNode = (esymNode*)malloc(sizeof(esymNode));

					strcpy(newNode->symbolName,		progName);
					newNode->symbolAddr = controlAddr;
					newNode->progSize	= TransHex2Dec(controlLength); 
					newNode->type		= 0;
					newNode->next		= NULL;

					hashKey = MakeHashKey(newNode->symbolName);
					if( esymbTable[hashKey] == NULL )
						esymbTable[hashKey] = newNode;
					else{
						for( ptrNode = esymbTable[hashKey]; ptrNode->next; ptrNode = ptrNode->next);
						ptrNode->next = newNode;
					}
				}
			}
			else if(dataType == 'D'){
				
				lineLength = strlen(objLineData);
				for( jj = 0; jj < ((lineLength-1)/6)/2 ; jj++ ){

					if( jj != 0)
						kk += 12;
					//printf("[%s][%d]/[%d]\n", objLineData, kk,lineLength);
					strncpy(esymName, &objLineData[kk], 6);
					strncpy(esymAddr, &objLineData[kk+6],6);

					// insert external symbol table;
					Trim(esymName);
					newNode = (esymNode*)malloc(sizeof(esymNode));
					newNode->type = 1;
					strcpy(newNode->symbolName,		esymName);
					newNode->symbolAddr = controlAddr + TransHex2Dec(esymAddr);
					newNode->next = NULL;

					hashKey = MakeHashKey(newNode->symbolName);
					if( esymbTable[hashKey] == NULL )
						esymbTable[hashKey] = newNode;
					else{
						for( ptrNode = esymbTable[hashKey]; ptrNode->next; ptrNode = ptrNode->next);
						ptrNode->next = newNode;
					}
				}
			}
			else if(dataType == 'E'){
				endflag = 1;
				break;
			}
			else{
				continue;
			}

		}
		if( endflag = 1 ){
			fclose(readFP);
			continue;
		}
		else{
			fclose(readFP);
			printf("loaderpass1 occur error\n");
			return -1;
		}
	}
	
	// printf esymtabl map
	//ShowEsymTable();
	return 0;
}

int LoaderPass2(unsigned char ***memory, char (*objList)[20], int objCount)
{
	FILE* readFP;
	char objLineData[100];
	char lineDataStart[6+1];
	char lineDataLength[2+1];
	char lineDataTemp[2+1];

	struct _referenceMap{
		char referenceNum[2+1];
		char referenceVal[6+1];
	}referenceMap[10];

	char progName[6+1];

	char modifyType;
	char modifyNum[2+1];
	char modifyVal[6+1];
	char dataType;
	char controlLength[6+1];
	char esymName[6+1];
	char esymAddr[6+1];
	char tempBefore[10+1]; 
	char tempAfter[10+1]; 

	char tempSixteen[10+1];
	char tempSixteenPart[2+1];

	int ii, jj, kk, ll;
	int tempSum = 0;

	int result = 0;
	int sizeofReference = 0;

	int endflag = 0;

	for ( ii = 0; ii < objCount; ii++ ){
		memset( referenceMap,	0x00, sizeof(referenceMap)	);

		readFP = fopen(objList[ii],"r");

		if( ii == 0 ){
			controlAddr = progAddr;
		}
		else{
			controlAddr = controlAddr + TransHex2Dec(controlLength);
		}

		endflag = 0;

		while( !feof(readFP) ){

			dataType = '0';
			tempSum = 0;
			sizeofReference = 0;
			memset( objLineData,	0x00, sizeof(objLineData)	);	
			memset( lineDataStart,	0x00, sizeof(lineDataStart)	);
			memset( lineDataLength, 0x00, sizeof(lineDataLength));
			memset( modifyVal,		0x00, sizeof(modifyVal)		);
			memset( modifyNum,		0x00, sizeof(modifyNum)		);
			memset( tempBefore,		0x00, sizeof(tempBefore)	); 
			memset( tempAfter,		0x00, sizeof(tempAfter)		); 
			memset( tempSixteen,	0x00, sizeof(tempSixteen)	);


			fgets( objLineData, sizeof(objLineData), readFP );
			if (strlen(objLineData) == 1)
				objLineData[strlen(objLineData)] = '\0';
			else	
				objLineData[strlen(objLineData)-1] = '\0';
			dataType = objLineData[0];

			if( dataType == 'H'){
				strncpy(progName, &objLineData[1], 6);
				strncpy(controlLength, &objLineData[13],6);
			}
			else if( dataType == 'D'){
				continue;
			}
			else if( dataType == 'R'){
				// make reference num, external feference map.
				if( (strlen(objLineData)-1)%8 != 0 )
					sizeofReference = (strlen(objLineData)-1)/8 + 1;
				else
					sizeofReference = (strlen(objLineData)-1)/8;

				kk = 1;
				strcpy(referenceMap[0].referenceNum, "01");
				strcpy(referenceMap[0].referenceVal, progName);
				for( jj = 1; jj < sizeofReference+1; jj++){
					strncpy(referenceMap[jj].referenceNum, &objLineData[kk],	2);
					strncpy(referenceMap[jj].referenceVal, &objLineData[kk+2],	6);
					kk += 8;
				}

			}
			else if( dataType == 'T'){
				strncpy( lineDataStart,		&objLineData[1], 6);
				strncpy( lineDataLength,	&objLineData[7], 2);
				
				kk = 9;
				ll = controlAddr + TransHex2Dec(lineDataStart);
				for( jj = 0; jj < TransHex2Dec(lineDataLength); jj++){
					memset( lineDataTemp,	0x00, sizeof(lineDataTemp));
					strncpy(lineDataTemp, &objLineData[kk], 2);
					kk += 2;

					SetMemory(memory, ll, ll, TransHex2Dec(lineDataTemp));
					ll++;
				}	
			}
			else if( dataType == 'M'){
				strncpy( lineDataStart,		&objLineData[1], 6);
				strncpy( lineDataLength,	&objLineData[7], 2);
				strncpy( modifyNum,			&objLineData[10],2);
				modifyType = objLineData[9];

				ll = controlAddr + TransHex2Dec(lineDataStart);

				// before value;
				for( jj = ll; jj < ll+3 ; jj++){
					sprintf(tempSixteen, "%02X", GetMemory(memory,jj));
					strncat(tempBefore, tempSixteen, 2);
				}

				for( jj = 0; jj < 10; jj++){
					if( TransHex2Dec(referenceMap[jj].referenceNum) == TransHex2Dec(modifyNum)){
						strncpy(modifyVal, referenceMap[jj].referenceVal, 6);		
						break;
					}
				}

				// find reference addr using reference velue
				Trim(modifyVal);
				result = SearchEsymAddr(modifyVal);

				// exception handling
				if( result == -1 ){
					printf("duplicate external symbol\n");
					fclose(readFP);
					return -1;
				}

				if( modifyType == '+' ){
					sprintf(tempAfter,"%06X", TransHex2Dec(tempBefore) + result);
				}
				else{
					sprintf(tempAfter,"%06X", TransHex2Dec(tempBefore) - result);
				}

				for( jj = 5, kk= strlen(tempAfter)-1; jj>=0; jj--){
					tempSixteen[jj] = tempAfter[kk--];
				}

				for( jj = 0; jj <3; jj++){
					memset(tempSixteenPart, 0x00, sizeof(tempSixteenPart));
					strncpy(tempSixteenPart, &tempSixteen[2*jj], 2);
					//printf("[%x]: [%s]\n", ll+jj, tempSixteenPart);
					SetMemory(memory, ll+jj, ll+jj, TransHex2Dec(tempSixteenPart));
				}
			}
			else if( dataType == 'E'){
				fclose(readFP);
				endflag = 1;
				break;
			}
			else if( dataType == '.'){
				// comment
				continue;
			}
			else{
				endflag = 2;
				break;
			}
		}
		if(endflag == 1){
			continue;
		}
		else{
			fclose(readFP);
			printf("loaderpass2 occur error[%d]\n", endflag);
			return -1;
		}
	}
	ShowEsymTable();
	return 0;
}

int SearchEsymAddr(char* symbolName)
{
	esymNode* ptrNode;
	int hashKey;

	hashKey = MakeHashKey(symbolName);

	for( ptrNode = esymbTable[hashKey]; ptrNode; ptrNode = ptrNode->next){
		if(!strcmp(ptrNode->symbolName, symbolName) ){
			return ptrNode->symbolAddr;
		}
	}
	return -1;
}


int SearchEsymTable(char* symbolName)
{
	esymNode* ptrNode;
	int hashKey;
	int flag = 0;

	hashKey = MakeHashKey(symbolName);

	for( ptrNode = esymbTable[hashKey]; ptrNode; ptrNode = ptrNode->next){
		if(!strcmp(ptrNode->symbolName, symbolName) ){
			return -1;
		}
	}
	return 0;
}

int SearchEsymTableAll(char* controlSect)
{
	int ii;
	esymNode* ptrNode;

	for( ii = 0; ii < HASHSIZE; ii++){
		for( ptrNode = esymbTable[ii]; ptrNode; ptrNode = ptrNode->next ){
			if( !strcmp(ptrNode->symbolName, controlSect) )
				return -1;
		}
	}
	return 0;
}

void ShowEsymTable(void)
{
	esymNode* ptrNode;
	int ii, jj =0;
	int kk;
	int total = 0;
	int minimum = 0;
	esymNode temp;
	esymNode sortTable[HASHSIZE];
	// 
	for( ii = 0; ii < HASHSIZE; ii++){
		for( ptrNode = esymbTable[ii]; ptrNode; ptrNode = ptrNode->next ){
			if(ptrNode->type == 0){
				total += ptrNode->progSize;
			}

			sortTable[jj].type			= ptrNode->type;
			sortTable[jj].symbolAddr	= ptrNode->symbolAddr;
			sortTable[jj].progSize		= ptrNode->progSize;
			sortTable[jj].next			= NULL;
			strcpy(sortTable[jj].controlSect,	ptrNode->controlSect);
			strcpy(sortTable[jj].symbolName,	ptrNode->symbolName);
			jj++;
		}
	}
	for( ii = 0; ii < jj; ii++){
		for (kk = ii; kk< jj -1; kk++){
			if(sortTable[kk].symbolAddr > sortTable[kk+1].symbolAddr){
				temp = sortTable[kk];
				sortTable[kk] = sortTable[kk+1];
				sortTable[kk+1] = temp;
			}
		}
	}


	printf("Control		Symbol		Address		Length\n");
	printf("section		Name\n");
	printf("---------------------------------------------------\n");
	for( ii = 0 ; ii < jj; ii++){
		if(sortTable[ii].type == 0){
			printf("%s\t\t\t\t%04X\t\t%04X\n", sortTable[ii].symbolName, sortTable[ii].symbolAddr, sortTable[ii].progSize);
		}
		else{
			printf("\t\t%s\t\t%04X\t\n", sortTable[ii].symbolName, sortTable[ii].symbolAddr);
		}
	}

	printf("---------------------------------------------------\n");
	printf("\t\t%s\t\t\t%04X\n", "total length", total);
	progSize = total;
}
void FreeEsymTable(void)
{
	int ii=0;
	esymNode* tmpNode = NULL;
	esymNode* headNode= NULL;

	for( ii = 0; ii < HASHSIZE; ii++){
		headNode = esymbTable[ii];
		while( headNode ){
			tmpNode = headNode;
			headNode = headNode->next;
			free(tmpNode);
		}
		esymbTable[ii] = NULL;
	}
}
int RunMemory(unsigned char ***memory)
{
	int target;
	int ii;
	int opcode;
	int r1, r2;
	int disp = 0;
	char *mnemonic;
	char flags[6+1];
	breakPoint* ptrNode;

	registerList[PC] = progAddr;
	registerList[L]	 = progAddr + progSize;


	while(registerList[PC] <= progAddr+progSize){

		for(ptrNode = breakHead; ptrNode; ptrNode = ptrNode->next){
			if( registerList[PC] <= ptrNode->breakPoint && ptrNode->breakPoint <= registerList[PC]+2){
				if(ptrNode->flag == 1){
					ptrNode->flag = 0;
					// print
					PrintRegister();
					printf("Stop at checkpoint[%04X]\n", ptrNode->breakPoint);
					return 2;
				}
				else{
					break;
				}
			}
		}
		
		memset(flags, 0x00, sizeof(flags));

		ii = registerList[PC];
		disp = 0;

		opcode	 = GetMemory(memory,ii)		& 0xFC;
		flags[0] = (GetMemory(memory,ii)	& 0x02) ? '1':'0';	// n 
		flags[1] = (GetMemory(memory,ii)	& 0x01) ? '1':'0';	// i
		flags[2] = (GetMemory(memory,ii+1)	& 0x80) ? '1':'0';	// x
		flags[3] = (GetMemory(memory,ii+1)	& 0x40) ? '1':'0';	// b
		flags[4] = (GetMemory(memory,ii+1)	& 0x20) ? '1':'0';	// p
		flags[5] = (GetMemory(memory,ii+1)	& 0x10) ? '1':'0';	// e

	
		mnemonic = SearchOpcodeAll(opcode);

		if( !strcmp(mnemonic, "ADDR") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;
			registerList[r2] += registerList[r1];
		}
		else if( !strcmp(mnemonic, "CLEAR") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;
			registerList[r1] = 0;

		}
		else if( !strcmp(mnemonic, "DIVR") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;
			registerList[r2] /= registerList[r1];

		}
		else if( !strcmp(mnemonic, "RMO") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;
			registerList[r2] = registerList[r1];

		}
		else if( !strcmp(mnemonic, "SHIFTL") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;

		}
		else if( !strcmp(mnemonic, "SHIFTR") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;

		}
		else if( !strcmp(mnemonic, "SUBR") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;
			registerList[r2] -= registerList[r1];

		}
		else if( !strcmp(mnemonic, "SVC") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;

		}
		else if( !strcmp(mnemonic, "TIXR") ){
			r1 = GetMemory(memory, ii+1) & 0x0C;
			r1 = GetMemory(memory, ii+1) & 0x03;
			registerList[X]++;
			if( registerList[X] > registerList[r1])
				registerList[SW] = 1;
			else if (registerList[X] < registerList[r1])
				registerList[SW] = -1;
			else
				registerList[SW] = 0;
			registerList[T] = 3;
		}
		
		else if(progAddr + progSize - registerList[PC] <= 2){
			registerList[PC] += (progAddr + progSize - registerList[PC]);
			break;

		}
		
		
		else if( !strcmp(mnemonic, "ADD")){
			registerList[A] += ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "ADDF")){
			//registerList[PC] +=3;
			registerList[F] += ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "AND")){
			registerList[A] &= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "COMP")){

		}
		else if( !strcmp(mnemonic, "COMPF")){

		}
		else if( !strcmp(mnemonic, "DIV")){
			if(flags[5] == '1')
				registerList[PC] += 4;
			else
				registerList[PC] += 3;
		}
		else if( !strcmp(mnemonic, "DIVF")){
			registerList[F] /= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "FIX")){
			registerList[PC] += 1;
			registerList[A] = registerList[F];
		}
		else if( !strcmp(mnemonic, "FLOAT")){
			registerList[PC] += 1;
			registerList[F] = registerList[A];
		}
		else if( !strcmp(mnemonic, "HIO")){
			registerList[PC] += 1;
		}
		else if( !strcmp(mnemonic, "J")){
			ProcessFormat3(memory,flags,&target);
			registerList[PC] = target;
		}
		else if( !strcmp(mnemonic, "JEQ")){
			ProcessFormat3(memory,flags,&target);
			if( registerList[SW] == 0)
				registerList[PC] = target;
		}
		else if( !strcmp(mnemonic, "JGL")){
			ProcessFormat3(memory,flags,&target);
			if( registerList[SW] == 1)
				registerList[PC] = target;
			
		}
		else if( !strcmp(mnemonic, "JLT")){
			ProcessFormat3(memory,flags,&target);
			if( registerList[SW] == -1)
				registerList[PC] = target;

		}
		else if( !strcmp(mnemonic, "JSUB")){
			ProcessFormat3(memory,flags,&target);
			registerList[L] = registerList[PC];
			registerList[PC] = target;
		}
		else if( !strcmp(mnemonic, "LDA")){
			registerList[A] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LDB")){
			registerList[B] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LDCH")){
			registerList[A] &= 0xFFFFFF00;	
			ProcessFormat3(memory, flags, &target);
			registerList[A] += GetMemory(memory, target);
		}
		else if( !strcmp(mnemonic, "LDF")){
			registerList[F] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LDL")){
			registerList[L] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LDS")){
			registerList[S] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LDT")){
			registerList[T] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LDX")){
			registerList[X] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "LPS")){
			registerList[S] = ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "MUL")){
			registerList[A] *= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "MULF")){
			registerList[F] *= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "NORM")){
			registerList[PC] += 1;
		}
		else if( !strcmp(mnemonic, "OR")){
			registerList[A] |= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "RD")){
			ProcessFormat3(memory,flags,&target);
			registerList[SW] = -1;	
		}
		else if( !strcmp(mnemonic, "RSUB")){
			ProcessFormat3(memory,flags,&target);
			registerList[PC] = registerList[L];	
			registerList[X] = 3;
			registerList[A] = 0x46;
		}
		else if( !strcmp(mnemonic, "SIO")){
			registerList[PC] +=1;

		}
		else if( !strcmp(mnemonic, "SSK")){
			ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "STA")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target,	  target, (registerList[A] & 0x00FF0000)>>16);
			SetMemory(memory, target+1, target+1, (registerList[A] & 0x0000FF00)>>8);
			SetMemory(memory, target+2, target+2, (registerList[A] & 0x000000FF));
		}
		else if( !strcmp(mnemonic, "STB")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target,	  target, (registerList[B] & 0x00FF0000)>>16);
			SetMemory(memory, target+1, target+1, (registerList[B] & 0x0000FF00)>>8);
			SetMemory(memory, target+2, target+2, (registerList[B] & 0x000000FF));

		}
		else if( !strcmp(mnemonic, "STCH")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target, target, registerList[A]);

		}
		else if( !strcmp(mnemonic, "STF")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target, target, registerList[F]);
		}
		else if( !strcmp(mnemonic, "STL")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target,	  target, (registerList[L] & 0x00FF0000)>>16);
			SetMemory(memory, target+1, target+1, (registerList[L] & 0x0000FF00)>>8);
			SetMemory(memory, target+2, target+2, (registerList[L] & 0x000000FF));
		}
		else if( !strcmp(mnemonic, "STS")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target,	  target, (registerList[S] & 0x00FF0000)>>16);
			SetMemory(memory, target+1, target+1, (registerList[S] & 0x0000FF00)>>8);
			SetMemory(memory, target+2, target+2, (registerList[S] & 0x000000FF));

		}
		else if( !strcmp(mnemonic, "STSW")){
			ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "STT")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target,	  target, (registerList[T] & 0x00FF0000)>>16);
			SetMemory(memory, target+1, target+1, (registerList[T] & 0x0000FF00)>>8);
			SetMemory(memory, target+2, target+2, (registerList[T] & 0x000000FF));
		}
		else if( !strcmp(mnemonic, "STX")){
			ProcessFormat3(memory,flags,&target);
			SetMemory(memory, target,	  target, (registerList[X] & 0x00FF0000)>>16);
			SetMemory(memory, target+1, target+1, (registerList[X] & 0x0000FF00)>>8);
			SetMemory(memory, target+2, target+2, (registerList[X] & 0x000000FF));

		}
		else if( !strcmp(mnemonic, "SUB")){
			registerList[A] -= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "SUBF")){
			registerList[F] -= ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "TD")){
			ProcessFormat3(memory,flags,&target);
		}
		else if( !strcmp(mnemonic, "TIO")){
			registerList[PC] += 1;
		}
		else if( !strcmp(mnemonic, "TIX")){
			registerList[X] +=1;
			ProcessFormat3(memory,flags,&target);
			if( registerList[X] > registerList[r1]) 
				registerList[SW] = 1;
			else if( registerList[X] < registerList[r1]) 
				registerList[SW] = -1;
			else
				registerList[SW] = 0;
		}
		else if( !strcmp(mnemonic, "WD")){
			ProcessFormat3(memory,flags,&target);
			registerList[SW] = -1;
		}

		else if(progAddr + progSize - registerList[PC] <= 6){
			registerList[PC] += (progAddr + progSize - registerList[PC]);
			break;
		}
		else{
			registerList[PC] += 1;
		}
	
	}
	PrintRegister();
	
}
void ShowBreakPoint(void)
{
	breakPoint *ptrNode;

	printf("breakPoint\n");
	printf("---------------\n");
	for(ptrNode = breakHead; ptrNode; ptrNode = ptrNode->next)
		printf("%04X\n", ptrNode->breakPoint);
}
int SearchBreakPoint(int point)
{
	breakPoint *ptrNode;

	for(ptrNode = breakHead; ptrNode; ptrNode = ptrNode->next){
		if( ptrNode->breakPoint == point )
			return -1;
	}
	return 0;
}
void InsertBreakPoint(int point)
{
	breakPoint *newNode;
	breakPoint *ptrNode;

	newNode = (breakPoint*)malloc(sizeof(breakPoint));

	newNode->breakPoint = point;
	newNode->flag = 1;
	newNode->next	= NULL;

	if( breakHead == NULL ){
		breakHead = newNode;
	}
	else if( breakHead->breakPoint > point ){
		newNode->next = breakHead;
		breakHead = newNode;
	}
	else{
		ptrNode = breakHead;
		while(1){
			if( !ptrNode->next ){
				ptrNode->next = newNode;
				break;
			}
			else if( ptrNode->breakPoint < point && ptrNode->next->breakPoint > point){
				newNode->next = ptrNode->next;
				ptrNode->next = newNode;
				break;
			}
			else{
				ptrNode = ptrNode->next;
			}
		}
	}

}

int FreeBreakPoint(void)
{
	breakPoint* tmpNode;

	while(breakHead){
		tmpNode = breakHead;
		breakHead = breakHead->next;
		free(tmpNode);
	}
}

int ProcessFormat3(unsigned char ***memory,char* flags, int* target){
	int disp;
	int result = 0;
	int temp;

	if(flags[5] == '1'){ // e = 1;
		disp = (GetMemory(memory, registerList[PC]+1) & 0x0F) << 16;
		disp+= (GetMemory(memory, registerList[PC]+2) & 0xFF) << 8;
		disp+= (GetMemory(memory, registerList[PC]+3) & 0xFF) ;

		registerList[PC] += 4;
	}
	else{
		disp = (GetMemory(memory, registerList[PC]+1) & 0x0F) << 8;
		disp+= (GetMemory(memory, registerList[PC]+2) & 0xFF) ;

		registerList[PC] += 3;
	}

	*target = disp;
	// index addressing mode
	if( flags[2] == '1') 
		*target += registerList[X];

	// sic mode
	if( flags[0] == '0' && flags[1] == '0'){
		temp = *target;
		return CalculateData(memory, temp);
	}
	else{	

		if( flags[3] == '1') // b flag;
			*target = registerList[B];
		if( flags[4] == '1')
			*target = registerList[PC];

		if( flags[0] == '1' && flags[1] == '1'){
			temp = *target;
			return CalculateData(memory, temp);
		}
		else if( flags[0] == '0' && flags[1] == '1'){
			return *target;
		}
		else if( flags[0] == '1' && flags[1] == '0'){
			temp = *target;
			*target = CalculateData(memory, temp);
			temp = *target;
			return CalculateData(memory, temp);
		}
	}
}
void PrintRegister(void)
{
	printf("A : %012X \n",	registerList[A]);
	printf("X : %012X \n",	registerList[X]);
	printf("L : %012X \n",	registerList[L]);
	printf("PC: %012X \n",	registerList[PC]);
	printf("B : %012X \n",	registerList[B]);
	printf("S : %012X \n",	registerList[S]);
	printf("T : %012X \n",	registerList[T]);
}
int CalculateData(unsigned char*** memory, int target)
{
	int result;
	result = (GetMemory(memory, target)    << 16);
	result+= (GetMemory(memory, target +1) << 8);
	result+= (GetMemory(memory, target +2));

	return result;
}
