#include "libs.h"
#include "parser_pyr.c"
#include "scanner.c"
#include "parser_print.c"
#include "parser.c"

int main(){
	
	char fileName[] = "test.txt";
	TokenList list = scan(fileName);
	
	if(((list.tail)->token).type == ERROR){
		printf("ERROR: %s",((list.tail)->token).string);
		return 0;
	}
	
	printTokenList(list, fileName);

	TokenNode * thisTokenNode = (list.head)->next;
	TreeNode * parseTree = NULL;
	bool parseErrorFound = true;
	
	ParserInfo it = {
		list,
		thisTokenNode,
		parseTree,
		parseErrorFound
	};
	
	ParserInfo * info = &it;
	TreeNode * tree = program(info);
	
	print_tree(tree);
	
	return 0;
}
