#ifndef _PARSER_PYR_H_
#define _PARSER_PYR_H_

#include "libs.h"
#include "scanner.h"
#include "parser.h"


TreeNode * program(ParserInfo * info);


TreeNode * stmts(ParserInfo * info);


TreeNode * stmt(ParserInfo * info);


TreeNode * exprs(ParserInfo * info);


TreeNode * expr(ParserInfo * info);


TreeNode * non_op_expr(ParserInfo * info);


TreeNode * literal(ParserInfo * info);


TreeNode * when_stmt(ParserInfo * info);


TreeNode * op_expr(ParserInfo * info);


TreeNode * assign_stmt(ParserInfo * info);


TreeNode * binding_stmt(ParserInfo * info);


TreeNode * decl_stmt(ParserInfo * info);


TreeNode * type_name(ParserInfo * info);


TreeNode * params(ParserInfo * info);


TreeNode * fun_header(ParserInfo * info);


TreeNode * unop_expr(ParserInfo * info);


TreeNode * binop_expr(ParserInfo * info);


//Start





















#endif
