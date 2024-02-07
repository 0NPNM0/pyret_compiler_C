#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "libs.h"

typedef enum
{	
    /* type-name tokens*/
    Any,          /*any type of value*/
    Number,       /*number type of value*/
	String,       /*string type value*/
	Boolean,      /*boolean type value*/
	Nothing,      /*void type value for void or missing value*/
	Function,     /*function type value*/
	
	/*keyword tokens*/
	TRUE,         /*boolean true*/
	FALSE,        /*boolean false*/
	FUN,          /*define function*/
	END,          /*end blocks*/
	IF,           /*condition statement*/
	ELSE,         /*condition statement is false*/
	VAR,          /*declare variable*/
	BLOCK,        /*blocks with more than one expression*/
	NOTHING,      /*(nothing)null*/
	WHEN,         /*when*/
	LAM,          /*unnamed funtion*/
	SHADOW,       /*using the already defined name inside the shadow with new value*/
	LIST,         /*list*/
	ARRAY,        /*array*/
	EMPTY,        /*empty set*/
	REC,          /*recursive function*/
	FOR,          /*for*/
	EACH,         /*for each*/
	IS,           /*is*/
	NOT,          /*not*/
	AND,          /*and*/
	OR,           /*or*/
	INCLUDE,      /*include*/

	/*not keyword tokens*/
    ID,           /*name*/
    COMMENT,      /*comment*/
    CHARACTER,    /* 'A' */
    STRING,       /* "SSS" */
    NUMBER,       /*number*/
    
    /* operator tokens */
    PLUS,         /*  +   */
 	MINUS,        /*  -   */
	STAR,         /*  *   */
	OVER,         /*  /   */
	LT,           /*  <   */
	LTE,          /*  <=  */
	GT,           /*  >   */
	GTE,          /*  >=  */
	EQ,           /*  ==  */
	ASSIGN,       /*  =   */ 
	COMMA,        /*  ,   */
	COLON,        /*  :   */
	DCOLON,       /*  ::  */ 
	LPAR,         /*  (   */
	RPAR,         /*  )   */
	LBR,          /*  [   */
	RBR,          /*  ]   */
	LCUR,         /*  {   */
	RCUR,         /*  }   */
	SCOM,         /*  #   */
	TCOM,         /*  |   */
	ARROW,        /*  -> */  
	MOD,          /*  %  */ 
	MAO,          /*  . (member access operator) */
	PMVB,         /*  :=(pattern matching and variable binding)  */
	
	/* tokens for non-final states in the DFA graph.*/ 
	ERROR,
	EOP,          /* end of program */   
	
	/* Supplement */
	Binding,      /*binding*/
      
} TokenType;

typedef struct{
	TokenType type;
	char * string;
}Token;

typedef struct TokenNode{
	Token token;
	int lineNumber;
	struct TokenNode * prev;
	struct TokenNode * next;
}TokenNode;

typedef struct{
	TokenNode * head;
	TokenNode * tail;
}TokenList;

typedef TokenNode * Node;
typedef TokenNode * Tail;
typedef TokenNode * List;

typedef enum{
	START,
	DONE,
	WRONG,
	ROLLBACK,
	READ_MINUS,
	READ_LT,
	READ_GT,
	READ_EQ,
	READ_COLON,
	READ_COMMENT,
	READ_COMMENT1,
	READ_COMMENT2,
	READ_NUM,
	READ_NUM1,
	READ_ID,
	READ_CHARACTER,
	READ_CHARACTER1,
	READ_STRING,
	READ_STRING1,
	READ_FOR,
	READ_IS,
	READ_EACH,
	READ_NOT
}State;


long getFileSize(const char * fileName);


void getFileString(const char * fileName, char * str, long fileSize);


void insertNode(Token T, Tail * tail, int lineNumber);


TokenList scan(const char * fileName);


Token dfa(const char *str, int *currentPosition, int *lineNumber, long fileSize);


int digit(char c);


int letter(char c);


int getFileLine(const char * fileName);


void printTokenType(int num);


void printTokenList(TokenList list, char * fileName);


#endif










          







