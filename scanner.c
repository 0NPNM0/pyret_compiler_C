#include "scanner.h"
#include "libs.h"



long getFileSize(const char * fileName){
	FILE * file = fopen(fileName, "rb");       //"rb" : Open the file in binary mode
	if(file == NULL){
		fputs("file error", stderr); //fputs() : Writes the string str to the file specified in stream
		                                       //stderr :  A standard stream in C used to output error messages
		exit(1);                               //exit(1) : The program terminates immediately and returns the status code 1 to the operating system
	}	
	
	fseek(file, 0, SEEK_END);                  //fseek(..) : Moves the file pointer to the end of the file without offset(0)
	long fileSize = ftell(file);               //ftell(..) : The initial position is 0, Get the position (offset) of the file pointer.
	
	fclose(file);
	
	return fileSize;
}



void getFileString(const char * fileName, char * str, long fileSize){
	FILE * file = fopen(fileName, "rb");
	size_t result = fread(str, 1, fileSize, file); //fread() : read data from a file
	                                               //str : A pointer to the memory buffer where the read data is to be stored
	                                               //1 : The block size of each read
	
	if(file == NULL){
		fputs("file error", stderr);
		exit(1);
	} 
	
	if(result != fileSize){
		fputs("reading error", stderr);
		exit(2);
	}
	
	fclose(file);
}



void insertNode(Token T, Tail * tail, int lineNumber){
	Node node = (Node)malloc(sizeof(TokenNode));
	if(node == NULL){
		fputs("memory error", stderr);
		exit(3);
	}
	
	node->lineNumber = lineNumber;
	node->token = T;
	(*tail)->next = node;
	node->prev = *tail;
	node->next = NULL;
	*tail = node;
}



TokenList scan(const char * fileName){
	List list = (List)malloc(sizeof(struct TokenNode));

	if(list == NULL){
		fputs("memory error", stderr);
		exit(3);	
	}
	
	TokenList tokenList = {list, list};
	
	long fileSize = getFileSize(fileName);
	
	char * str = (char*)malloc(sizeof(char)*fileSize);
	if(str == NULL){
		fputs("memory error", stderr);
		exit(3);
	}
	
	getFileString(fileName, str, fileSize);
	
	int currentPosition = 0;
	Token token;
	int lineNumber = 1;
	while(currentPosition < fileSize && strlen(str) != 0){
		token = dfa(str, &currentPosition, &lineNumber, fileSize);
		
//		if(token.type != COMMENT){
			insertNode(token, &tokenList.tail, lineNumber);
			if(token.type == ERROR){
				return tokenList;	
			}
//		}
	}
	
	token = (Token){EOP, "\0"};
	insertNode(token, &(tokenList.tail)->prev, lineNumber+1);
	
	free(str);
	
	return tokenList;
}



int digit(char c){
	if(c >= '0' && c <= '9'){
		return 1;
	}
	return 0;
}



int letter(char c){
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
		return 1;
	}else{
		return 0;
	}
}



Token dfa(const char *str, int *currentPosition, int *lineNumber, long fileSize ){
	
	Token token;
	TokenType tokenType;
	char tokenString[500] = "\0";
	bool isTokenString = false;
	State currentState = START;
	int i = 0;
	
	while(*currentPosition < strlen(str) && strlen(str) != 0){
		char c = str[*currentPosition];
		*currentPosition = *currentPosition + 1;
		
		if(currentState == START){
			switch(c){
				case '+':
					currentState = DONE;
					tokenType = PLUS;
					break;
				case '/':
					currentState = DONE;
					tokenType = OVER;
					break;
				case '*':
					currentState = DONE;
					tokenType = STAR;
					break;
				case ',':
					currentState = DONE;
					tokenType = COMMA;
					break;
				case '(':
					currentState = DONE;
					tokenType = LPAR;
					break;
				case ')':
					currentState = DONE;
					tokenType = RPAR;
					break;
				case '[':
					currentState = DONE;
					tokenType = LBR;
					break;
				case ']':
					currentState = DONE;
					tokenType = RBR;
					break;
				case '{':
					currentState = DONE;
					tokenType = LCUR;
					break;
				case '}':
					currentState = DONE;
					tokenType = RCUR;
					break;
				case '%':
					currentState = DONE;
					tokenType = MOD;
					break;
				case '.':
					currentState = DONE;
					tokenType = MAO;
					break;
				
				case '-':
					currentState = READ_MINUS;
					break;
				case ':':
					currentState = READ_COLON;
					break;
				case '<':
					currentState = READ_LT;
					break;
				case '>':
					currentState = READ_GT;
					break;
				case '=':
					currentState = READ_EQ;
					break;
				case '#':
					currentState = READ_COMMENT;
					tokenString[i++] = c; 
					break;
				case '\'':
					currentState = READ_CHARACTER;
					tokenString[i++] = c;
					break;
				case '\"':
					currentState = READ_STRING;
					tokenString[i++] = c;
					break;
				
				default:
					if(letter(c)){
						currentState = READ_ID;
						tokenString[i++] = c;
					}else if(digit(c)){
						currentState = READ_NUM;
						tokenString[i++] = c;
					}else if(isspace(c) != 0){//judge whether c is nothing
						if(c == 10){
							*lineNumber = *lineNumber + 1;
						}
						continue;
					}else if(isspace(c) == 0){//judge whether c is nothing
						continue;
					}else {
						currentState = WRONG;
						tokenType = ERROR;
						strcpy(tokenString, "found unknown character");
					}
			}
		}
		else if(currentState == READ_MINUS){
			if(c == '>'){
				currentState = DONE;
				tokenType = ARROW;
			}else if(digit(c)){
				currentState = READ_NUM;
				tokenType = Number; 
				tokenString[i++] = c;
			}else {
				currentState = ROLLBACK;
				tokenType = MINUS;
			}	
		}
		else if(currentState == READ_COLON){
			if(c == ':'){
				currentState = DONE;
				tokenType = DCOLON;
			}else if(c == '='){
				currentState = DONE;
				tokenType = PMVB;
			}else {
				currentState = ROLLBACK;
				tokenType = COLON;
			}
		}
		else if(currentState == READ_LT){
			if(c == '='){
				currentState = DONE;
				tokenType = LTE;
			}else {
				currentState = ROLLBACK;
				tokenType = LT;
			}
		}
		else if(currentState == READ_GT){
			if(c == '='){
				currentState = DONE;
				tokenType = GTE;
			}else {
				currentState = ROLLBACK;
				tokenType = GT;
			}
		}
		else if(currentState == READ_EQ){
			if(c == '='){
				currentState = DONE;
				tokenType = EQ;
			}else {
				currentState = ROLLBACK;
				tokenType = ASSIGN;
			}
		}
		
		else if(currentState == READ_ID){
			if(letter(c) || c == '-' || digit(c)){
				currentState = READ_ID;
				tokenType = ID;
				tokenString[i++] = c;
			}else{
				currentState = ROLLBACK;
				if(!strcmp(tokenString, "Any")){
					tokenType = Any;
				}else if(!strcmp(tokenString, "Number")){
					tokenType = Number;
				}else if(!strcmp(tokenString, "String")){
					tokenType = String;
				}else if(!strcmp(tokenString, "Boolean")){
					tokenType = Boolean;
				}else if(!strcmp(tokenString, "Nothing")){
					tokenType = Nothing;
				}else if(!strcmp(tokenString, "Function")){
					tokenType = Function;
				}else if(!strcmp(tokenString, "true")){
					tokenType = TRUE;
				}else if(!strcmp(tokenString, "false")){
					tokenType = FALSE;
				}else if(!strcmp(tokenString, "fun")){
					tokenType = FUN;
				}else if(!strcmp(tokenString, "end")){
					tokenType = END;
				}else if(!strcmp(tokenString, "if")){
					tokenType = IF;
				}else if(!strcmp(tokenString, "else")){
					tokenType = ELSE;
				}else if(!strcmp(tokenString, "var")){
					tokenType = VAR;
				}else if(!strcmp(tokenString, "block")){
					tokenType = BLOCK;
				}else if(!strcmp(tokenString, "nothing")){
					tokenType = NOTHING;
				}else if(!strcmp(tokenString, "when")){
					tokenType = WHEN;
				}else if(!strcmp(tokenString, "lam")){
					tokenType = LAM;
				}else if(!strcmp(tokenString, "shadow")){
					tokenType = SHADOW;
				}else if(!strcmp(tokenString, "list")){
					tokenType = LIST;
				}else if(!strcmp(tokenString, "array")){
					tokenType = ARRAY;
				}else if(!strcmp(tokenString, "empty")){
					tokenType = EMPTY;
				}else if(!strcmp(tokenString, "rec")){
					tokenType = REC;
				}else if(!strcmp(tokenString, "for")){
					tokenType = FOR;
				}else if(!strcmp(tokenString, "each")){
					tokenType = EACH;
				}else if(!strcmp(tokenString, "is")){
					tokenType = IS;
				}else if(!strcmp(tokenString, "not")){
					tokenType = NOT;
				}else if(!strcmp(tokenString, "and")){
					tokenType = AND;
				}else if(!strcmp(tokenString, "or")){
					tokenType = OR;
				}else if(!strcmp(tokenString, "include")){
					tokenType = INCLUDE;
				}
				else{
					tokenType = ID;
					isTokenString = true;
				}
			}
		}else if(currentState == READ_NUM){
			if(digit(c)){
				currentState = READ_NUM;
				tokenType = NUMBER;
				tokenString[i++] = c;
			}else if(c == 'e'){
				currentState = READ_NUM1;
				tokenString[i++] = c;
			}else{
				currentState = ROLLBACK;
				tokenType = NUMBER;
				isTokenString = true;
			}
		}
		else if(currentState == READ_NUM1){
			if(digit(c)){
				currentState = READ_NUM1;
				tokenString[i++] = c; 
			}else{
				currentState = ROLLBACK;
				tokenType = NUMBER;
				isTokenString = true; 
			}
		}else if(currentState == READ_CHARACTER){
			if(c == '\''){
				currentState = ROLLBACK;
				tokenType = CHARACTER;
				isTokenString = true; 
				tokenString[i++] = c; 
			}else{
				currentState = READ_CHARACTER1;
				tokenString[i++] = c; 
			}
		}else if(currentState == READ_CHARACTER1){
			if(c == '\''){
				currentState = DONE;
				tokenType = CHARACTER;
				isTokenString = true; 
				tokenString[i++] = c; 
			}else{
				currentState = WRONG;
				tokenType = ERROR;
				strcpy(tokenString, "The character type is not correct!");
			}
		}else if(currentState == READ_STRING){
			if(c == '\"'){
				currentState = DONE;
				tokenType = STRING;
				isTokenString = true; 
				tokenString[i++] = c; 
			}else{
				currentState = READ_STRING1;
				tokenString[i++] = c; 
			}
		}else if(currentState == READ_STRING1){
			if(c == 10){
				currentState = WRONG;
				tokenType = ERROR;
				strcpy(tokenString, "The string type is not correct!");
			}else if(c == '\"'){
				currentState = DONE;
				tokenType = STRING;
				isTokenString = true; 
				tokenString[i++] = c; 
			}else{
				currentState = READ_STRING1;
				tokenString[i++] = c; 
			}
		}
		
		else if(currentState == READ_COMMENT){// #
			if(c == 10){
				currentState = ROLLBACK;
				tokenType = COMMENT;
				tokenString[i++] = c;
				isTokenString = true;
			}else if(c == '|'){
				currentState = READ_COMMENT1;
				tokenString[i++] = c;
			}else{
				currentState = READ_COMMENT;
				tokenString[i++] = c; 
			}
		}
		else if(currentState == READ_COMMENT1){// #|
			if(*currentPosition >= fileSize){
				currentState = WRONG;
				tokenType = ERROR;
				strcpy(tokenString,"no matching problem");
			}else if(c == '|'){
				currentState = READ_COMMENT2;
				tokenString[i++] = c;
			}else {
				currentState = READ_COMMENT1;
				tokenString[i++] = c;
				if(c == 10){
					*lineNumber = *lineNumber + 1;
				}
			}
		}
		else if(currentState == READ_COMMENT2){// #|...|
			if(c == '#'){
				currentState = DONE;
				tokenString[i++] = c;
				isTokenString = true;
				tokenType = COMMENT;
			}else if(c == '|'){
				currentState = READ_COMMENT2;
				tokenString[i++] = c;
			}else{
				currentState = READ_COMMENT1;
				tokenString[i++] = c;
				if(c == 10){
					*lineNumber = *lineNumber + 1;
				}
			}
		}
		
		
		
		
		if(currentState == DONE){
			if(isTokenString){
				char * tstr = (char*)malloc(sizeof(tokenString) * sizeof(char));
				if(tstr == NULL){
					printf("memory error");
					exit(3); 
				}
				strcpy(tstr,tokenString);
				token = (Token){tokenType,tstr};
			}else{
				token = (Token){tokenType,"\0"};
			}
			break;
		}else if(currentState == WRONG){
			char * tstr = (char*)malloc(sizeof(tokenString) * sizeof(char));
				if(tstr == NULL){
					printf("memory error");
					exit(3); 
				}
			strcpy(tstr,tokenString);
			token = (Token){tokenType,tstr};
			break;
		}else if(currentState == ROLLBACK){
			if(isTokenString){
				char * tstr = (char*)malloc(sizeof(tokenString) * sizeof(char));
				if(tstr == NULL){
					printf("memory error");
					exit(3); 
				}
				strcpy(tstr,tokenString);
				token = (Token){tokenType,tstr};
			}else{
				token = (Token){tokenType,"\0"};
			}
			*currentPosition = *currentPosition - 1;
			break;
		}
	}
	
	return token;
}



int getFileLine(const char * fileName){
	char a[100];
	int fileLine = 0;
	FILE * l = fopen(fileName, "r");
	if(l == NULL){
		fputs("file error", stderr);
		exit(1);
	}
	
	while(!feof(l)){ //feof:file end of file, check whether the point has reach the end after file reading operation 
		fgets(a, 100, l); //a:save the read  100:maximum read number   l:point at the file to be read
		fileLine++;
		if(feof(l)){
			break;
		}
	}
	fclose(l);
	
	return fileLine;	
}



void printTokenType(int num){
	switch (num) {
		case 0:
			printf("TYPE NAME : Any");
			break;
		case 1:
			printf("TYPE NAME : Number");
			break;
		case 2:
			printf("TYPE NAME : String");
			break;
		case 3:
			printf("TYPE NAME : Boolean");
			break;
		case 4:
			printf("TYPE NAME : Nothing");
			break;
		case 5:
			printf("TYPE NAME : Function");
			break;
		case 6:
			printf("KEYWORD : TURE");
			break;
		case 7:
			printf("KEYWORD : FALSE");
			break;
		case 8:
			printf("KEYWORD : FUN");
			break;
		case 9:
			printf("KEYWORD : END");
			break;
		case 10:
			printf("KEYWORD : IF");
			break;
		case 11:
			printf("KEYWORD : ELSE");
			break;
		case 12:
			printf("KEYWORD : VAR");
			break;
		case 13:
			printf("KEYWORD : BLOCK");
			break;
		case 14:
			printf("KEYWORD : NOTHING");
			break;
		case 15:
			printf("KEYWORD : WHEN");
			break;
		case 16:
			printf("KEYWORD : LAM");
			break;
		case 17:
			printf("KEYWORD : SHADOW");
			break;
		case 18:
			printf("KEYWORD : LIST");
			break;
		case 19:
			printf("KEYWORD : ARRAY");
			break;
		case 20:
			printf("KEYWORD : EMPTY");
			break;
		case 21:
			printf("KEYWORD : REC");
			break;
		case 22:
			printf("KEYWORD : FOR");
			break;
		case 23:
			printf("KEYWORD : EACH");
			break;
		case 24:
			printf("KEYWORD : IS");
			break;
		case 25:
			printf("KEYWORD : NOT");
			break;
		case 26:
			printf("KEYWORD : AND");
			break;
		case 27:
			printf("KEYWORD : OR");
			break;
		case 28:
			printf("KEYWORD : INCLUDE");
			break;
		case 29:
			printf("ID , NAME =");
			break;
		case 30:
			printf("COMMENT =");
			break;
		case 31:
			printf("CHARACTER =");
			break;
		case 32:
			printf("STRING =");
			break;
		case 33:
			printf("NUMBER , value =");
			break;
		case 34:
			printf("+");
			break;
		case 35:
			printf("-");
			break;
		case 36:
			printf("*");
			break;
		case 37:
			printf("/");
			break;
		case 38:
			printf("<"); 
			break;
		case 39:
			printf("<=");
			break;
		case 40:
			printf(">");
			break;
		case 41:
			printf(">=");
			break;
		case 42:
			printf("==");
			break;
		case 43:
			printf("=");
			break;
		case 44:
			printf(",");
			break;
		case 45:
			printf(":");
			break;
		case 46:
			printf("::");
			break;
		case 47:
			printf("(");
			break;
		case 48:
			printf(")");
			break;
		case 49:
			printf("[");
			break;
		case 50:
			printf("]");
			break;
		case 51:
			printf("{");
			break;
		case 52:
			printf("}");
			break;
		case 53:
			printf("#");
			break;
		case 54:
			printf("|");
			break;
		case 55:
			printf("->");
			break;
		case 56:
			printf("%%");
			break;
		case 57:
			printf(".");
			break;
		case 58:
			printf(":=");
			break;
		case 59:
			printf("ERROR :");
			break;
		case 60:
			printf("EOP");
			break;
		case 61:
			printf("BLINDING");
			break;
		
	}
		
}



void printTokenList(TokenList list, char * fileName){//for scanner test
    int num = 1;
    int type = 0;
	Node position = (list.head)->next;
	
	while(position != (list.tail)->next){
		printf("%d",num); 
		printf(" : ");
		type = (position->token).type;
		printTokenType(type);
		if(type == ID || type == NUMBER || type == COMMENT || type == ERROR || type == CHARACTER || type == STRING){
			printf(" %s ",(position->token).string);
		}
		printf("\n");
		
		num++;
		position = position->next;
	}
	
}









