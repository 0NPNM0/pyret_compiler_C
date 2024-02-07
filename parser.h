#ifndef _PARSE_H_
#define _PARSE_H_

#include "scanner.h"
#include "libs.h"

#define MAX_CHILDREN 100

typedef enum {
	BLOCK_ND,     //block node
	DECL_ND,       //declaration node
	PARAM_ND,     //parameter node
	STMT_ND,      //statement node
	EXPR_ND,      //expression node
	TYPE_ND       //show node type name 
}NodeKind;//node kind



typedef enum {
	DECL_BLOCK, //declaration block
}BlockKind;//block kind



typedef enum {
	VAR_DECL,      //variable declaration
	FUN_DECL       //function declaration
}DeclKind;//declaration kind



typedef enum {
	VAR_PARAM,    //variable parameter
}ParamKind;//parameter kind



typedef enum {
	WHEN_STMT,       //when loop statement
	ASSIGN_STMT,     //assign statement
	BINDING_STMT,       //expression statement
	DECL_STMT,       //compound statement(block)
}StmtKind;//statement kind



typedef enum {
	OP_EXPR,      //operator expression
	CONST_EXPR,   //constant expression
	ID_EXPR,      //identifier expression
	CALL_EXPR,    //function call expression
	TYPE_EXPR     //show node type name
}ExprKind;//expression kind



//typedef enum {
//	NOTHING_TYPE,     //void type
//	INT_TYPE,      //int type
//	ADDR_TYPE      //address type
//}ExprType;//expression data type



//typedef enum {
//  FALSE,
//  TRUE
//}boolean;



/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/



typedef struct treeNode {
  struct treeNode * child[MAX_CHILDREN];   //down
  struct treeNode * lSibling;              //left
  struct treeNode * rSibling;              //right
  struct treeNode * parent;                //up


  int lineNum;   //line number of the token being handled 
  NodeKind nodeKind;
  
                 //(share memory)(Only one type of value can be stored)
  union {        //union : same memory space can store different types of value
    BlockKind block;
  	DeclKind decl; 
	ParamKind param; 
	StmtKind stmt; 
	ExprKind expr;
  }kind;
  
  
  
  union {
  	
    union {
      TokenType op;       // (tokentype)used by Op_EXPR
      char * val;            // (constant value)used by Const_EXPR,
      char * name;  // (string)used by ID_EXPR, Call_EXPR, Array_EXPR
    }exprAttr;//expression attribute
    
    
    struct { 
//      ExprType type;      // (expression type)used by all dcl and param
      const char * name;  // (string)used by all dcl and param(dcl names and param names)
      int size;           // (int)used by array declaration(declare size of array)
    }dclAttr; // declaration attribute and parameters attribute.
    
  }attr;//attribute
  
  
  
  char * exact_type;//for type checking and semantic analyzer
  void * something; //something possibly useful for other tasks of compiling
}TreeNode;



typedef struct parser Parser;



//Each function has a parameter p, that is a pointer to the parser itself, in order to use the resources belong to the parser
typedef struct parser {
	TreeNode * (* parse)(Parser * p); //returning a parse tree, based on the tokenList that the parser knows
	void (* set_token_list)(Parser * p, TokenList tokenList); //remember some tokenList
	void (* print_tree)(Parser * p,  TreeNode * tree); //can print some parser tree
	void (* free_tree)(Parser *p, TreeNode * tree); // free the space of a parse tree
	void * info; //Some data belonging to this parser object. It can contain the tokenList that the parser knows. 
}Parser;



typedef struct parserInfo {
	TokenList theTokenList;
	TokenNode * thisTokenNode;
	TreeNode * parseTree;
	bool parseErrorFound;
}ParserInfo;



bool check_token(TokenNode * tknode, TokenType tktype);



bool need_token(ParserInfo * info, TokenType tktype, const char * errorMsg);



TokenNode * move(ParserInfo * info);



TokenNode * move_back(ParserInfo * info);



bool need_token_move(ParserInfo * info, TokenType tktype, const char * errorMag);



TreeNode * create_tree_node(int lineNumber, NodeKind nodekind);



void printParserTree(TreeNode * root);



void end_tree(TreeNode * root);



#endif
