#include "parser.h"

bool check_token(TokenNode * tknode, TokenType tktype){
	return tktype == (tknode->token).type;
}



bool need_token(ParserInfo * info, TokenType tktype, const char * errorMsg){
	if(tktype == ((info->thisTokenNode)->token).type){
		return true;
	}else{
		printf("%s\n",errorMsg);
		info->parseErrorFound = false;//FALSE : has error
		return FALSE;
	}
}



TokenNode * move(ParserInfo * info){
	info->thisTokenNode = (info->thisTokenNode)->next;
	return info->thisTokenNode;
}



TokenNode * move_back(ParserInfo * info){
	info->thisTokenNode = (info->thisTokenNode)->prev;
	return info->thisTokenNode;
}



bool need_token_move(ParserInfo * info, TokenType tktype, const char * errorMsg){
	if(tktype == ((info->thisTokenNode)->token).type){
		move(info);
		return true;
	}else{
		printf("%s\n",errorMsg);
		info->parseErrorFound = false;
		return false;
	}
}



TreeNode * create_tree_node(int lineNumber, NodeKind nodekind){
	TreeNode * treeNode = (TreeNode *)malloc(sizeof(struct treeNode));
	if(treeNode == NULL){
		printf("out of space!");
		exit(1);
	}
	
	treeNode->child[0] = NULL;
	treeNode->child[1] = NULL;
	treeNode->child[2] = NULL;
	treeNode->child[3] = NULL;
	treeNode->lSibling = NULL;
	treeNode->rSibling = NULL;
	treeNode->parent = NULL;
	treeNode->lineNum = lineNumber;
	treeNode->nodeKind = nodekind;
	
	return treeNode;
}




void end_tree(TreeNode * root){
	TreeNode * treeNode = root;
	int num = 0;
	
	if(treeNode->child[num] != NULL){
		end_tree(treeNode->child[num++]);
	}
	
	while(num < MAX_CHILDREN){
		if(treeNode->child[num++] == NULL){
			break;
		}
		
		if(treeNode->nodeKind == EXPR_ND && (treeNode->kind).expr == CONST_EXPR){
			free(((treeNode->attr).exprAttr).val);
		}
		
		if(treeNode->nodeKind == EXPR_ND && (treeNode->kind).expr == ID_EXPR){
			free(((treeNode->attr).exprAttr).name);
		}
	}
}














