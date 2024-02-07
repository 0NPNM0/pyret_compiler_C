
#include "parser.h"

#define INDENT_GAP 2

/* printSpaces indents by printing spaces */
void print_spaces(int indentNum) {
	int i;
	for (i=0;i<indentNum;i++)
		printf(" ");
}


void print_tree( TreeNode * tree ){
	
	int i;

	static int indentNum = 0;//intent space number

	indentNum += INDENT_GAP;
	if (tree != NULL) {
		
		print_spaces(indentNum); 
		

		if (tree->nodeKind == DECL_ND){
			
			
			switch(tree->kind.decl){

			case FUN_DECL:
				printf( "fun_decl:\n");
				break;
			case VAR_DECL:
				printf( "var_decl:\n");
				break;
			default:
				printf("Unknown DclNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind==PARAM_ND){
			printf( "Parameter: ");
			switch (tree->kind.param){
				case VAR_PARAM:
				printf("variable \n");
				break;
			}
		}
		else if(tree->nodeKind==STMT_ND) {
			switch (tree->kind.stmt) {
			case WHEN_STMT:
				printf("when: \n");
				break;
			case ASSIGN_STMT:
				printf("assign: \n");
				break;
			case BINDING_STMT:
				printf("binding: \n");
				break;
			case DECL_STMT:
				printf("decl:\n");
				break;
			default:
				printf("Unknown StmtNode kind\n");
				break;
			}
		}
		else if(tree->nodeKind==EXPR_ND) {
			switch (tree->kind.expr) {
			case OP_EXPR:
				printf("Operator: %s\n",tree->exact_type);
				break;
			case CONST_EXPR:
				printf("%s: %s\n",tree->exact_type, tree->attr.exprAttr.val);
				break;
			case ID_EXPR:
				printf("%s: %s\n",tree->exact_type, tree->attr.exprAttr.name);
				break;
			case CALL_EXPR:
				if(tree->exact_type == "no parameter"){
					printf("Call function: %s, with arguments below:%s\n", tree->attr.exprAttr.name, tree->exact_type);
				}else{
					printf("Call function: %s, with arguments below:\n", tree->attr.exprAttr.name);
				}
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
			
		}
		else if(tree->nodeKind==TYPE_ND) {
			switch (tree->kind.expr) {
			case TYPE_EXPR:
				if(tree->exact_type == "none" || tree->exact_type == "no parameter" || 
				tree->exact_type == "with parameter"){
					printf("Type: %s %s\n",tree->attr.exprAttr.name, tree->exact_type);
				}else if(tree->exact_type == "PMVB"){
					printf("Type: %s (%s)\n",tree->exact_type,tree->attr.exprAttr.name);
				}else if(tree->exact_type == "ASSIGN"){
					printf("Type: %s (%s)\n",tree->exact_type,tree->attr.exprAttr.name);
				}else if(tree->exact_type == "Any" || 
				tree->exact_type == "Number" || tree->exact_type == "String" || 
				tree->exact_type == "Boolean" || tree->exact_type == "Nothing" || 
				tree->exact_type == "Function"){
					printf("Type name: %s \n",tree->exact_type);
				}else{
					printf("Type: %s\n",tree->attr.exprAttr.name);
				}
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
			
		}
		else {
			printf("Unknown node kind\n");
		}

	    for (i=0;i<MAX_CHILDREN;i++){
	    	
	    	if(tree->child[i] == NULL){
	    		i = MAX_CHILDREN;
			}else{
				print_tree(tree->child[i]);
			}
		}
		tree = tree->rSibling;
		
	}
	
	indentNum -= INDENT_GAP;
	
}


