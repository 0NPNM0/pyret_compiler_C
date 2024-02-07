#include "parser_pyr.h"


//<program> => (<stmt>)+
TreeNode * program(ParserInfo * info){
	return stmts(info);
}


//<stmts> => (<stmt>) *
TreeNode * stmts(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * leftEnd = NULL;
	TreeNode * rightEnd = NULL;
	TreeNode * it = NULL;
	
	int num = 0;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber,TYPE_ND);
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy(((top->attr).exprAttr).name, "program");
	
	while(true){
		
		it = stmt(info);
		
		top->child[num++] = it;
		it->parent = top;

		if(leftEnd == NULL){
			leftEnd = rightEnd = it;
		} else{
			it ->lSibling = rightEnd;
			rightEnd -> rSibling = it;
		}
		
		if(info->parseErrorFound == false) {
			return leftEnd;
		}
		
		if(need_token_move(info, EOP,"") == true){
			break;
		}
		
	}

	return top;
}


//<stmt> =>  <when-stmt> | <assign-stmt> | <exprs> | <decl-stmt> | <binding-stmt>
TreeNode * stmt(ParserInfo * info){
	
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	if(check_token(info->thisTokenNode, WHEN)){
	
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "when_stmt" );
		
		it = when_stmt(info);	
		it->parent = top;
		top->child[0] = it;
		
		return top;	
	}
	
	if(check_token(info->thisTokenNode, ID) && check_token(info->thisTokenNode->next, PMVB)){
		
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "assign_stmt" );
		
		it = assign_stmt(info);	
		it->parent = top;
		top->child[0] = it;
		
		return top;	
	}
	
	if(check_token(info->thisTokenNode, SHADOW) ||
			check_token(info->thisTokenNode, REC) ||
			check_token(info->thisTokenNode, VAR) ||
			check_token(info->thisTokenNode, FUN)){
				
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "decl_stmt" );
				
		it = decl_stmt(info);	
		it->parent = top;
		top->child[0] = it;
		
		return top;
	}
	
	if(check_token(info->thisTokenNode, ID) && check_token(info->thisTokenNode->next, ASSIGN)){
	
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "binding_stmt" );
		
		it = binding_stmt(info);	
		it->parent = top;
		top->child[0] = it;
		
		return top;	
	}
	
	return exprs(info);
	
}

//<exprs> => <expr>, | <expr>
TreeNode * exprs(ParserInfo * info){
	TreeNode * top = NULL;
    TreeNode * leftEnd = NULL; 
    TreeNode * rightEnd = NULL; 
    TreeNode * it = NULL;
    int num = 0;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "expression" );
	
	while( 5 == 5 ){
		
    	it = expr(info);
    	top->child[num++] = it;
		it->parent = top;
		if( leftEnd == NULL ) {
            leftEnd = rightEnd = it; 
        }else{
            it -> lSibling = rightEnd;
            rightEnd -> rSibling = it;
        }
    	if( info->parseErrorFound == FALSE ){
    		return top;
		}
		
    	if( need_token_move(info, COMMA, "") == false ){
			return top;
		}
	}
	
	return top;
	
}



//check whether the list contains operator
bool containsOp(ParserInfo * info){
	int num = 0;
	Node n = info->thisTokenNode;
	while(num<5){
		num++;
		if((n->token).type == PLUS || 
		   (n->token).type == MINUS || 
		   (n->token).type == STAR ||
		   (n->token).type == OVER ||
		   (n->token).type == MOD ||
		   (n->token).type == LT ||
		   (n->token).type == LTE ||
		   (n->token).type == GT ||
		   (n->token).type == GTE ||
		   (n->token).type == EQ){
			return true;
		}else{
			n = n->next; 
		}
	}
	return false;
}


//check whether current node type is operator or not 
bool isOp(ParserInfo * info){
	
	Node n = info->thisTokenNode;
	if((n->token).type == PLUS || 
		(n->token).type == MINUS || 
		(n->token).type == STAR ||
		(n->token).type == OVER ||
		(n->token).type == MOD ||
		(n->token).type == LT ||
		(n->token).type == LTE ||
		(n->token).type == GT ||
		(n->token).type == GTE ||
		(n->token).type == EQ){
		return true;
	}
	
	return false;
}



//<expr> =>  <non-op-expr>  | <op-expr>
TreeNode * expr(ParserInfo * info){
	
	if(check_token(info->thisTokenNode, IF) == true ||
	   check_token(info->thisTokenNode, BLOCK) == true ||
	   check_token(info->thisTokenNode, LPAR) == true ||
	   check_token(info->thisTokenNode, TRUE) == true || 
	   check_token(info->thisTokenNode, FALSE) == true ||
	   check_token(info->thisTokenNode, NOTHING) == true || 
	   (check_token(info->thisTokenNode, ID) == true && 
	   check_token(info->thisTokenNode->next, LPAR) == true)||
	   (check_token(info->thisTokenNode, ID) == true && 
	   check_token(info->thisTokenNode->next, COMMA) == true)||
	   check_token(info->thisTokenNode, LAM) == true ||
	   check_token(info->thisTokenNode, STRING) == true || 
	   check_token(info->thisTokenNode, CHARACTER) == true || 
	   check_token(info->thisTokenNode, NUMBER) == true ||
	   check_token(info->thisTokenNode, LBR) == true){
	   		return non_op_expr(info);
	}
	
	if(check_token(info->thisTokenNode, NOT) == true){
		return unop_expr(info);
	}
	
	if(containsOp(info) == true){
		return binop_expr(info);
	}
	
	if(check_token(info->thisTokenNode, ID) == true){
		return non_op_expr(info);
	}
	
	
}


////<unop> => not
TreeNode * unop(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = CONST_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy(((top->attr).exprAttr).name, "not");
	top->exact_type = "NOT";
	
	return top;
}


//unop-expr> => <unop> ( <non-op-expr> |  <paren-expr> )  
//<unop> => not
TreeNode * unop_expr(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy(((top->attr).exprAttr).name, "unop_expr");
	
	it = unop(info);
	top->child[0] = it;
	it->parent = top;
	
	move(info);
	it = non_op_expr(info);
	top->child[1] = it;
	it->parent = top;
	
	if(info->parseErrorFound == false){
		return top;
	}
	
	return top;
}


//<binop> => [+ , - , * , / , < , <= , > , >= ,  == ]
TreeNode * binop(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND );
	(top->kind).expr = OP_EXPR;
	
	if( check_token(info->thisTokenNode, PLUS) == true ){
		((top->attr).exprAttr).op = PLUS;
		top->exact_type = (char *)"+";
	}else if( check_token(info->thisTokenNode, MINUS) == true ){
		((top->attr).exprAttr).op = MINUS;
		top->exact_type = (char *)"-";
	}else if( check_token(info->thisTokenNode, STAR) == true ){
		((top->attr).exprAttr).op = STAR;
		top->exact_type = (char *)"*";
	}else if( check_token(info->thisTokenNode, OVER) == true ){
		((top->attr).exprAttr).op = OVER;
		top->exact_type = (char *)"/";
	}else if( check_token(info->thisTokenNode, MOD) == true ){
		((top->attr).exprAttr).op = MOD;
		top->exact_type = (char *)"%";
	}else if( check_token(info->thisTokenNode, LT) == true ){
		((top->attr).exprAttr).op = LT;
		top->exact_type = (char *)"<";
	}else if( check_token(info->thisTokenNode, LTE) == true ){
		((top->attr).exprAttr).op = LTE;
		top->exact_type = (char *)"<=";
	}else if( check_token(info->thisTokenNode, GT) == true ){
		((top->attr).exprAttr).op = GT;
		top->exact_type = (char *)">";
	}else if( check_token(info->thisTokenNode, GTE) == true ){
		((top->attr).exprAttr).op = GTE;
		top->exact_type = (char *)">=";
	}else if( check_token(info->thisTokenNode, EQ) == true ){
		((top->attr).exprAttr).op = EQ;
		top->exact_type = (char *)"==";
	}else{
		printf("the operation is not included in pyr.\n");
		info->parseErrorFound = false;
	}
	
	move(info);
	
	return top;
}


//<binop-expr> => <non-op-expr> <binop> ( <non-op-expr> | <paren-expr> )
TreeNode * binop_expr(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * leftEnd = NULL; 
    TreeNode * rightEnd = NULL; 
	TreeNode * it = NULL;
	int num = 1;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy(((top->attr).exprAttr).name, "binop_expr");
	
	it = non_op_expr(info);
	top->child[0] = it;
	it->parent = top;
	
	bool unfinish = true;
	while(unfinish){
		it = binop(info);
		top->child[num++] = it;
		it->parent = top;
		if( leftEnd == NULL ) {
            leftEnd = rightEnd = it; 
        }else{
            it -> lSibling = rightEnd;
            rightEnd -> rSibling = it;
        }
    	if( info->parseErrorFound == FALSE ){
    		return top;
		}	
		
		it = non_op_expr(info);
		top->child[num++] = it;
		it->parent = top;
		if( leftEnd == NULL ) {
            leftEnd = rightEnd = it; 
        }else{
            it -> lSibling = rightEnd;
            rightEnd -> rSibling = it;
        }
    	if( info->parseErrorFound == FALSE ){
    		return top;
		}
		
		
		if(isOp(info) == false ){
			unfinish = false;
			
		}
		
	}

	if(info->parseErrorFound == false){
		return top;
	}
	
	return top;
}



////<op-expr> => <binop-expr> | <unop-expr>
//TreeNode * op_expr(ParserInfo * info){
//	if(check_token(info->thisTokenNode, NOT) == true){
//		return unop_expr(info);
//	}else{
//		return binop_expr(info);
//	}
//}


//<id> => (<name>) - (<keyword>) 
TreeNode * id(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = ID_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, ((info->thisTokenNode)->token).string );
	top->exact_type = (char *)"id name";
	
	move(info);
	
	return top;
}


//<boolean> =>  true | false
TreeNode * l_boolean(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = CONST_EXPR;
	top->exact_type = (char *)"boolean";
	
	if(check_token(info->thisTokenNode, TRUE) == true){
		((top->attr).exprAttr).val = (char *)"true";
	}else if(check_token(info->thisTokenNode, FALSE) == true){
		((top->attr).exprAttr).val = (char *)"false";
	}else{
		printf("%s","The value is not belong to Boolean\n");
		info->parseErrorFound = false;
	}
	
	move(info);
	return top;
}


//<string> => <single_quote_str> | <double_quote_str> | <multi_line_str>
//<single_quote_str> => ' ([ASCII]-[' \n] | \')* '
//<double_quote_str> => " ([ASCII]-[" \n] | \")* "
//            <multi_line_str>   => ``` ([ASCII]* - ([ASCII]*```[ASCII]*) ) ```
TreeNode * l_string(ParserInfo * info){
	TreeNode * top = NULL;
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = CONST_EXPR;
	
	if(check_token(info->thisTokenNode, STRING) == true ){
		top->exact_type = (char *)"string";
		((top->attr).exprAttr).val = (char *)((info->thisTokenNode)->token).string;
	}else if(check_token(info->thisTokenNode, CHARACTER) == true){
		top->exact_type = (char *)"character";
		((top->attr).exprAttr).val = (char *)((info->thisTokenNode)->token).string;
	}else{
		printf("%s","The format of literal_string is not correct\n");
		info->parseErrorFound = false;
	}
	
	move(info);
	return top;
}




//<nothing> => 
TreeNode * l_nothing(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = CONST_EXPR;
	top->exact_type = (char *)"nothing";
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "" );
	move(info);
	return top;
}



//<number>  =>  <integer> | <float>| <fraction>
TreeNode * l_number(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = CONST_EXPR;


	if(check_token(info->thisTokenNode, NUMBER) == true){
		((top->attr).exprAttr).val = (char *)((info->thisTokenNode)->token).string;
		top->exact_type = (char *)"number";
	}else{
		printf("%s","The value is not belong to number\n");
		info->parseErrorFound = false;
	}
	
	move(info);
	return top;
}



//<literal> => <boolean> | <string> | <nothing> | <number>     
TreeNode * literal(ParserInfo * info){
	
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	if(check_token(info->thisTokenNode, TRUE) == true || check_token(info->thisTokenNode, FALSE) == true){
		
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy(((top->attr).exprAttr).name, "literal_boolean");
		
		it = l_boolean(info);
		it->parent = top;
		top->child[0] = it;
		if(info->parseErrorFound == false){
			return top;
		}
		
	}
	
	if(check_token(info->thisTokenNode, STRING) == true || check_token(info->thisTokenNode, CHARACTER) == true){
		
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy(((top->attr).exprAttr).name, "literal_string");
		
		it = l_string(info);
		it->parent = top;
		top->child[0] = it;
		if(info->parseErrorFound == false){
			return top;
		}
	}
	
	if(check_token(info->thisTokenNode, NOTHING) == true){
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy(((top->attr).exprAttr).name, "literal_nothing");
		
		it = l_nothing(info);
		it->parent = top;
		top->child[0] = it;
		if(info->parseErrorFound == false){
			return top;
		}
	}
	
	if(check_token(info->thisTokenNode, NUMBER) == true){
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy(((top->attr).exprAttr).name, "literal_number");
		
		it = l_number(info);
		it->parent = top;
		top->child[0] = it;
		
		if(info->parseErrorFound == false){
			return top;
		}
		
	}
	
	return top;
}



//<fun-call> => <id>  \(  (  kong | (<expr> ,)* <expr> )  \)
TreeNode * fun_call (ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * leftEnd = NULL; 
    TreeNode * rightEnd = NULL; 
    TreeNode * it = NULL;
    int num = 0;
    
    
	top = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND);
	(top->kind).expr = CALL_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, ((info->thisTokenNode)->token).string);

	move(info);
    if( need_token_move(info, LPAR, "The format of function call is missing (\n") == false  ){
    	return top;
	}
	
	if(check_token(info->thisTokenNode, RPAR) == true){
		top->exact_type = (char *)"no parameter";
		move(info);
		return top;
	}
	
	if(check_token(info->thisTokenNode, RPAR) == false){

		it = exprs(info);
		it->parent = top;
		top->child[num++] = it;
			

		if( need_token_move(info, RPAR, "The format of function call is missing )\n") == false ){
    		return top;
		}
		
	}

	return top;
}


//end
TreeNode * end(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "END" );
	
	it = create_tree_node((info->thisTokenNode)->lineNumber, EXPR_ND );
	(it->kind).expr = CONST_EXPR;
	((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((it->attr).exprAttr).name,  "end");
	 
	it->parent = top;
	top->child[0] = it;
	it->exact_type = "the end";
	
	
	return top;
}


//<block> => <stmt> | block :  <stmt>* end
TreeNode * block(ParserInfo * info){
	TreeNode * top = NULL;
    TreeNode * leftEnd = NULL; 
    TreeNode * rightEnd = NULL; 
    TreeNode * it = NULL;
    int num = 0;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "block" );
	
	if(check_token(info->thisTokenNode, BLOCK) == true){
		move(info);
		
		if( need_token_move(info, COLON, "The block is missing :\n") == false ){
			return top;
		}
		
		bool unFinish = true;
		while(unFinish){
		
			it = stmt(info); 
			top->child[num++] = it;
			it->parent = top;
		
			if( leftEnd == NULL ) {
            	leftEnd = rightEnd = it; 
        	}else{
            	it -> lSibling = rightEnd;
            	rightEnd -> rSibling = it;
       	 	}
        	if( info->parseErrorFound == FALSE ){
    			return top;
			}
		
			if( check_token(info->thisTokenNode, END) == true ){
				unFinish = false;
			}
		}
		
		it = end(info);
		top->child[num++] = it;
		it->parent = top;
		
		move(info);
		if( leftEnd == NULL ) {
        	leftEnd = rightEnd = it; 
    	}else{
        	it -> lSibling = rightEnd;
        	rightEnd -> rSibling = it;
    	}
    	if( info->parseErrorFound == false ){
    		return top;
		}
		
	}else{
		
		it = stmt(info); 
		top->child[0] = it;
		it->parent = top;
	
	}
	
	return top;
}



//‹else-if› =>  else if <expr> : <block>
TreeNode * else_if(ParserInfo * info){
	TreeNode * top = NULL;
    TreeNode * leftEnd = NULL; 
    TreeNode * rightEnd = NULL; 
    TreeNode * it = NULL;
    int num = 0;
    
    top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "<else-if>" );
	
	bool unfinish = true;
	
	while(unfinish){
		move(info);
		move(info);
		
		it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
		(it->kind).expr = TYPE_EXPR;
		((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((it->attr).exprAttr).name, "else if" );
		top->child[num++] = it;
		it->parent = top;
		
		it = expr(info);
		top->child[num++] = it;
		it->parent = top;

		if( need_token_move(info, COLON, "The if_expr's else_if is missing :\n") == false ){
			return top;
		}
		
		it = block(info);
		top->child[num++] = it;
		it->parent = top;
	
		if(check_token(info->thisTokenNode, ELSE) != true || 
		   (check_token(info->thisTokenNode, ELSE) != true && check_token(info->thisTokenNode, IF) != true)){
			unfinish = false;
		}else if(check_token(info->thisTokenNode, ELSE) == true && check_token(info->thisTokenNode->next, IF) == false){
			unfinish = false;
			move(info);
			
			it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
			(it->kind).expr = TYPE_EXPR;
			((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
			strcpy( ((it->attr).exprAttr).name, "else" );
			top->child[num++] = it;
			it->parent = top;
			
			if( need_token_move(info, COLON, "The if_expr is missing :\n") == false ){
				return top;
			}
			
			it = block(info);
			top->child[num++] = it;
			it->parent = top;
			
		}
	
	}
	
	
	if(info->parseErrorFound == false ){
		return top;
	}
	
	return top;
	
}



//<if-expr> =>
// if <expr> : <block> (<else-if>)+  (else: <block>)? end | if <expr> : <block> (<else-if>)*  else: <block> end
TreeNode * if_expr(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "if_expr" );
		
	move(info);
	it = expr(info);
	top->child[0] = it;
	it->parent = top;

	
	if( need_token_move(info, COLON, "The if_expr is missing :\n") == false ){
		return top;
	}
	
	it = block(info);
	top->child[1] = it;
	it->parent = top;
	
	if(check_token(info->thisTokenNode, ELSE) == true && check_token(info->thisTokenNode->next, IF) == true){
		
		it = else_if(info);
		top->child[2] = it;
		it->parent = top;
		
		
		if( need_token_move(info, END, "The if_expr is missing end\n") == false ){
			return top;
		}else{
			it = end(info);
			top->child[3] = it;
			it->parent = top;
		}
	}else if(check_token(info->thisTokenNode, ELSE) == true && check_token(info->thisTokenNode->next, IF) == false){
		move(info);
		
		it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
		(it->kind).expr = TYPE_EXPR;
		((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((it->attr).exprAttr).name, "else" );
		top->child[2] = it;
		it->parent = top;

		if( need_token_move(info, COLON, "The if_expr's else is missing :\n") == false ){
			return top;
		}
		
		it = block(info);
		top->child[3] = it;
		it->parent = top;
		
		if( need_token_move(info, END, "The if_expr is missing end\n") == false ){
			return top;
		}else{
			it = end(info);
			top->child[4] = it;
			it->parent = top;
		}
		
	}
	
	
	if(info->parseErrorFound == false ){
		return top;
	}
	
	move(info);
	
	return top;
}


//<array> => \[ array: (   | (<expr> ,)* <expr> ) \]
TreeNode * array(ParserInfo * info){
	TreeNode * top  = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "array, with member(s): " );
	
	move(info);
	
	if( need_token_move(info, COLON, "The construct_expr is missing :\n") == false ){
		return top;
	}
	
	if(check_token(info->thisTokenNode, RBR) == true) {
		move(info);
		top->exact_type = (char *)"none";
	}else{
		it = exprs(info);
		it->parent = top;
		top->child[0] = it;
		
		if(need_token_move(info, RBR, "The array is missing ]\n") == false){
			return top;
		}
	}
	return top;

}



//<list> => \[ list: (   | (<expr> ,)* <expr> ) \]
TreeNode * list(ParserInfo * info){
	TreeNode * top  = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "list, with member(s): " );
	
	move(info);
	
	if( need_token_move(info, COLON, "The construct_expr is missing :\n") == false ){
		return top;
	}
	
	if(check_token(info->thisTokenNode, RBR) == true) {
		move(info);
		top->exact_type = (char *)"none";
	}else{
		it = exprs(info);
		it->parent = top;
		top->child[0] = it;
		
		if(need_token_move(info, RBR, "The array is missing ]\n") == false){
			return top;
		}
	}
	return top;

}


//<construct-expr> => <array> | <list>
TreeNode * construct_expr(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	if( need_token_move(info, LBR, "The construct_expr is missing [\n") == false ){
		return top;
	}
	
	if(check_token(info->thisTokenNode, ARRAY) == true){
		return array(info); 
	}
	if(check_token(info->thisTokenNode, LIST) == true){
		return list(info); 
	}
	
	return top;
}


//<lambda-expr>  => lam <fun-header> : <block> end
TreeNode * lam_expr(ParserInfo * info){
	TreeNode * top  = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "lambda-expr" );
	
	move(info);
	
	it = fun_header(info);
	top->child[0] = it;
	it->parent = top;
	
	if(need_token_move(info, COLON, "The lam is missing :\n") == false){
		return top;
	}
		
	it = block(info);
	top->child[1] = it;
	it->parent = top;
	
	
	if(check_token(info->thisTokenNode, END) == true){
		it = end(info);
		top->child[2] = it;
		it->parent = top;
	}else{
		printf("%s","The lam_expr is missing end\n");
		info->parseErrorFound = false;
	}
	
	
	if(info->parseErrorFound == false ){
		return top;
	}
	
	move(info);
	
	return top;
}


bool isOperator(TokenType t){
	return t == PLUS || t == MINUS || t == STAR || t == OVER || t == MOD || t == LT || t == LTE || t == GT || t == GTE || t == EQ ;
}


//<non-op-expr> => <id> | <literal> | <fun-call> | <paren-expr> | <if-expr> | <block> | <construct-expr> | <lambda-expr>
TreeNode * non_op_expr(ParserInfo * info){
	
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	if(check_token(info->thisTokenNode, ID) == true && check_token(info->thisTokenNode->next, LPAR) == true){
		
		
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "function_call" );
			
		it = fun_call(info);
		it->parent = top;
		top->child[0] = it;
			
		if(info->parseErrorFound == false ){
			return top;
		}
		
		return top;
		
	}
	
	if(check_token(info->thisTokenNode, ID) == true){
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "id" );
			
		it = id(info);
		it->parent = top;
		top->child[0] = it;
		
		if(info->parseErrorFound == false ){
			return top;
		}
		
		return top;
	}
	
	if(check_token(info->thisTokenNode, TRUE) == true || 
	    check_token(info->thisTokenNode, FALSE) == true ||
	    check_token(info->thisTokenNode, STRING) == true || 
	    check_token(info->thisTokenNode, CHARACTER) == true || 
		check_token(info->thisTokenNode, NOTHING) == true || 
		check_token(info->thisTokenNode, NUMBER) == true) {
			
		return literal(info);
		
	}

	
	if(check_token(info->thisTokenNode, LPAR) == true){//for paren-expr
		
		top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
		(top->kind).expr = TYPE_EXPR;
		((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
		strcpy( ((top->attr).exprAttr).name, "parenthesis_expression" );
		
		move(info);
		it = expr(info);
		top->child[0] = it;
		it->parent = top;
		
		if( need_token_move(info, RPAR, "The format of function call is missing )\n") == false ){
    		return top;
		}
		
		return top;
	}
	
	
	
	if(check_token(info->thisTokenNode, IF) == true){
		return if_expr(info);
	}
	
	
	if(check_token(info->thisTokenNode, BLOCK) == true){
		
		return block(info);
	}
	
	
	if(check_token(info->thisTokenNode, LBR) == true){
	   	
		return construct_expr(info);
	}
	
	
	if(check_token(info->thisTokenNode, LAM) == true){
		
		return lam_expr(info);
	}

	
}




//<when-stmt> => when <expr> : <block> end
TreeNode * when_stmt(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, STMT_ND);
	(top->kind).stmt = WHEN_STMT;
	move(info);
	
	it = expr(info);
	top->child[0] = it;
	it->parent = top;
	if(info->parseErrorFound == false){
		return top;
	}
	
	if(need_token_move(info, COLON, "The when_stmt is missing -> :\n") == false){
		return top;
	} 
	
	it = block(info);
	top->child[1] = it;
	it->parent = top;
	
	if(check_token(info->thisTokenNode, END) == true){
	
		it = end(info);
		top->child[2] = it;
		it->parent = top;
	}else{
		printf("%s","The when_stmt is missing end \n");
		info->parseErrorFound = false;
	}
	
	if(info->parseErrorFound == false){
		return top;
 	}
	
	move(info);
	
	return top;
}


//<assign-stmt> => <id> := <expr> 
TreeNode * assign_stmt(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, STMT_ND);
	(top->kind).stmt = ASSIGN_STMT;
	
	it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(it->kind).expr = TYPE_EXPR;
	((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((it->attr).exprAttr).name, "id");
	it->parent = top;
	top->child[0] = it;
	
	it = id(info);
	it->parent = top;
	top->child[1] = it;
	

	if( need_token_move(info, PMVB, "The assign_stmt is missing :=\n") == false ){
    	return top;
	}
		
	it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(it->kind).expr = TYPE_EXPR;
	((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((it->attr).exprAttr).name, ":=" );
	it->exact_type = (char *)"PMVB";
	it->parent = top;
	top->child[2] = it;
	
	
	it = expr(info);
	it->parent = top;
	top->child[3] = it;
	
	return top;
}



//<binding-stmt> => <id> = <expr>
TreeNode * binding_stmt(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, STMT_ND);
	(top->kind).stmt = BINDING_STMT;
	
	it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(it->kind).expr = TYPE_EXPR;
	((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((it->attr).exprAttr).name, "id");
	it->parent = top;
	top->child[0] = it;
	
	it = id(info);
	it->parent = top;
	top->child[1] = it;
	

	if( need_token_move(info, ASSIGN, "The binding_stmt is missing =\n") == false ){
    	return top;
	}
		
	it = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(it->kind).expr = TYPE_EXPR;
	((it->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((it->attr).exprAttr).name, "=" );
	it->exact_type = (char *)"ASSIGN";
	it->parent = top;
	top->child[2] = it;
	
	
	it = expr(info);
	it->parent = top;
	top->child[3] = it;
	
	return top;
}


//shadow
TreeNode * shadow(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "shadow" );
	
	return top;
}



//rec
TreeNode * rec(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "rec" );
	
	return top;
}


//var
TreeNode * var(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "var" );
	
	return top;
}



//fun
TreeNode * fun(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "fun" );
	
	return top;
}



//<var-decl> => (shadow)? (rec)? var <id> = <expr>
TreeNode * var_decl(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, DECL_ND);
	(top->kind).decl = VAR_DECL;
	
	
	if(check_token(info->thisTokenNode, SHADOW) && check_token(info->thisTokenNode->next, VAR)){
		it = shadow(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		
		it = var(info);
		it->parent = top;
		top->child[1] = it;
		
		move(info);
		
		it = binding_stmt(info);
		it->parent = top;
		top->child[2] = it;
	}else if(check_token(info->thisTokenNode, REC) && check_token(info->thisTokenNode->next, VAR)){
		it = rec(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		
		it = var(info);
		it->parent = top;
		top->child[1] = it;
		
		move(info);
		
		it = binding_stmt(info);
		it->parent = top;
		top->child[2] = it;
	}else if (check_token(info->thisTokenNode, VAR)){
		it = var(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		it = binding_stmt(info);
		it->parent = top;
		top->child[1] = it;
	}
	
	if( info->parseErrorFound == false ){
    	return top;
	}
	
	return top;
}



//<fun-header> => \( < params>  \) (<return-part>)
//<return-part> =>  -> <type-name>
TreeNode * fun_header(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND);
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "fun-header" );
	
	
	if( need_token_move(info, LPAR, "The fun_header is missing (\n") == false ){
		return top;
	}
	
	if(check_token(info->thisTokenNode, RPAR) == true){
		top->exact_type = (char *)"no parameter";
		move(info);
		if( check_token(info->thisTokenNode, ARROW) == true ){
			move(info);
		
			it = type_name(info);
			it->parent = top;
			top->child[0] = it;
		
		}
	}else if(check_token(info->thisTokenNode, RPAR) == false){
		it = params(info);
	    it->parent = top;
	    top->child[0] = it;
	    
	    top->exact_type = (char *)"with parameter";
	    
	    if( need_token_move(info, RPAR, "The fun_header is missing )\n") == false ){
		    return top;
	    }
	
	 	if( check_token(info->thisTokenNode, ARROW) == true ){
			move(info);
		
			it = type_name(info);
			it->parent = top;
			top->child[1] = it;
		
		}
	}
	
	
	if( info->parseErrorFound == false ){
    	return top;
	}
	
	return top;
}




//<fun-decl> => (shadow)? (rec)? fun <id> <fun-header> : <stmts> end
TreeNode * fun_decl(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, DECL_ND);
	(top->kind).decl = FUN_DECL;
	
	
	if(check_token(info->thisTokenNode, SHADOW) && check_token(info->thisTokenNode->next, FUN)){
		it = shadow(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		
		it = fun(info);
		it->parent = top;
		top->child[1] = it;
		
		move(info);
		
		it = id(info);
		it->parent = top;
		top->child[2] = it;
		
		it = fun_header(info);
		it->parent = top;
		top->child[3] = it;
		
		if( need_token_move(info, COLON, "The fun_decl is missing :\n") == false ){
			return top;
		}
		
		it = stmts(info);
		it->parent = top;
		top->child[4] = it;
		
		it = end(info);
		it->parent = top;
		top->child[5] = it;
		
		
	}else if(check_token(info->thisTokenNode, REC) && check_token(info->thisTokenNode->next, FUN)){
		it = rec(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		it = fun(info);
		it->parent = top;
		top->child[1] = it;
		
		move(info);
		
		it = id(info);
		it->parent = top;
		top->child[2] = it;
		
		it = fun_header(info);
		it->parent = top;
		top->child[3] = it;
		
		if( need_token_move(info, COLON, "The fun_decl is missing :\n") == false ){
			return top;
		}
		
		it = stmts(info);
		it->parent = top;
		top->child[4] = it;
		
		it = end(info);
		it->parent = top;
		top->child[5] = it;
		
		
	}else if(check_token(info->thisTokenNode, FUN)){
		it = fun(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		it = id(info);
		it->parent = top;
		top->child[1] = it;
		
		it = fun_header(info);
		it->parent = top;
		top->child[2] = it;
		
		if( need_token_move(info, COLON, "The fun_decl is missing :\n") == false ){
			return top;
		}
		
		it = stmts(info);
		it->parent = top;
		top->child[3] = it;
		
		it = end(info);
		it->parent = top;
		top->child[4] = it;
	}
	
	if( info->parseErrorFound == false ){
    	return top;
	}
	
	move(info);
	
	return top;
}




//<decl-stmt> => <var-decl> | <fun-decl>
TreeNode * decl_stmt(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, STMT_ND);
	(top->kind).stmt = DECL_STMT;
	
	if( (check_token(info->thisTokenNode, SHADOW) && check_token(info->thisTokenNode->next, VAR)) ||
		(check_token(info->thisTokenNode, REC) && check_token(info->thisTokenNode->next, VAR)) ||
		(check_token(info->thisTokenNode, VAR)) ){
			
		it = var_decl(info);
		it->parent = top;
		top->child[0] = it;
		
	}
	
	if( (check_token(info->thisTokenNode, SHADOW) && check_token(info->thisTokenNode->next, FUN)) ||
		(check_token(info->thisTokenNode, REC) && check_token(info->thisTokenNode->next, FUN)) ||
		(check_token(info->thisTokenNode, FUN)) ){
			
		it = fun_decl(info);
		it->parent = top;
		top->child[0] = it;
		
	}
	
	if( info->parseErrorFound == false ){
    	return top;
	}
	
	return top;
}


//<type-name> => Any | Number | String | Boolean | Nothing | Function
TreeNode * type_name(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	
	if( check_token(info->thisTokenNode, Any) == true ){
		top->exact_type = (char *)"Any";
		strcpy( ((top->attr).exprAttr).name, "Any" );
		
	}else if( check_token(info->thisTokenNode, Number) == true ){
		top->exact_type = (char *)"Number";
		strcpy( ((top->attr).exprAttr).name, "Number" );
		
	}else if( check_token(info->thisTokenNode, String) == true ){
		top->exact_type = (char *)"String";
		strcpy( ((top->attr).exprAttr).name, "String" );
		
	}else if( check_token(info->thisTokenNode, Boolean) == true ){
		top->exact_type = (char *)"Boolean";
		strcpy( ((top->attr).exprAttr).name, "Boolean" );
		
	}else if( check_token(info->thisTokenNode, Nothing) == true ){
		top->exact_type = (char *)"Nothing";
		strcpy( ((top->attr).exprAttr).name, "Nothing" );
		
	}else if( check_token(info->thisTokenNode, Function) == true ){
		top->exact_type = (char *)"Function";
		strcpy( ((top->attr).exprAttr).name, "Function" );
		
	}else{
		printf("the type is not included in pyr.");
		info->parseErrorFound = false;
	}
	
	move(info);
	
	return top;
}



//<param> =>  (shadow)? <id> [:: <type-name>]
TreeNode * param(ParserInfo * info){
	TreeNode * top = NULL;
	TreeNode * it = NULL;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, PARAM_ND);
	(top->kind).param = VAR_PARAM;
	
	
	if(check_token(info->thisTokenNode, SHADOW) && check_token(info->thisTokenNode->next, ID) && check_token(info->thisTokenNode->next->next, DCOLON)){
		it = shadow(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		
		it = id(info);
		it->parent = top;
		top->child[1] = it;
		
		move(info);
		
		it = type_name(info);
		it->parent = top;
		top->child[2] = it;
		
		
	}else if(check_token(info->thisTokenNode, SHADOW) && check_token(info->thisTokenNode->next, ID)){
		it = shadow(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		
		it = id(info);
		it->parent = top;
		top->child[1] = it;
		
	}else if(check_token(info->thisTokenNode, ID) && check_token(info->thisTokenNode->next, DCOLON)){
		it = id(info);
		it->parent = top;
		top->child[0] = it;
		
		move(info);
		
		it = type_name(info);
		it->parent = top;
		top->child[1] = it;
		
	}else if(check_token(info->thisTokenNode, ID)){
		it = id(info);
		it->parent = top;
		top->child[0] = it;
		
	}
	
	if( info->parseErrorFound == false ){
    	return top;
	}
	
	return top;
}



//<param_list> => (<param> ,)* <param>
TreeNode * param_list(ParserInfo * info){
	TreeNode * top = NULL;
    TreeNode * leftEnd = NULL; 
    TreeNode * rightEnd = NULL; 
    TreeNode * it = NULL;
    int num = 0;
	
	top = create_tree_node((info->thisTokenNode)->lineNumber, TYPE_ND );
	(top->kind).expr = TYPE_EXPR;
	((top->attr).exprAttr).name = (char *)malloc(24*sizeof(char));
	strcpy( ((top->attr).exprAttr).name, "param_list" );
	
	while(true){
		
    	it = param(info);
    	top->child[num++] = it;
		it->parent = top;
		
		if( leftEnd == NULL ) {
            leftEnd = rightEnd = it; 
        }else{
            it -> lSibling = rightEnd;
            rightEnd -> rSibling = it;
        }
    	if( info->parseErrorFound == false ){
    		return top;
		}
		
    	if( need_token_move(info, COMMA, "") == false ){
    		return top;
		}
	}
	
	return top;
}


//<params> =>   | <param_list>
TreeNode * params(ParserInfo * info){
	TreeNode * top = NULL;
	
	top = param_list(info); 
	
	return top;
}






