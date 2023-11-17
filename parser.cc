/*
 * Author: James Zanetti
 * Professor: Rida Bazzi
 * SID: 1222771693
 * Course: CSE 340
 *
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include "parser.h"
#include "execute.h"
#include <unordered_map>
#include <set>
#include <stack>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// vector to store the values assigned in the input
vector<int> inputs;

// initialize lexer object
Parser parser;

// syntax error function
void Parser::syntax_error()
{
	cout << "SYNTAX ERROR !!!\n";
	exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
	// get token
	Token t = lexer.GetToken();

	if (t.token_type != expected_type)
	{
		syntax_error();
	}

	return t;
}

// program -> var_section body inputs
void Parser::parse_program()
{
	parse_varSection();

	parse_body();

	parse_inputs();

	// returns but ignore since SYNTAX ERRORS will crash the program
	expect(END_OF_FILE);
}

// var_section -> id_list SEMICOLON
void Parser::parse_varSection()
{
	parse_idList();

	expect(SEMICOLON);
}

// id_list -> ID COMMA id_list | ID
void Parser::parse_idList()
{
	expect(ID);

	// peek at the next token
	Token t = lexer.peek(1);

	if (t.token_type == COMMA)
	{
		expect(COMMA);

		parse_idList();
	}
}

// body -> LBRACE stmt_list RBRACE
struct InstructionNode* Parser::parse_body()
{
	expect(LBRACE);

	parse_stmtList();

	expect(RBRACE);
}

// stmt_list -> stmt stmt_list stmt
struct InstructionNode* Parser::parse_stmtList()
{
	//instruction for one statement
	struct InstructionNode* inst; 

	//instruction list for statement list
	struct InstructionNode* instList;

	inst = parse_stmt();

	// peek at the next token
	Token t = lexer.peek(1);

	// check if there is a statement list
	if (t.token_type == ID ||
		t.token_type == WHILE ||
		t.token_type == IF ||
		t.token_type == INPUT ||
		t.token_type == SWITCH ||
		t.token_type == OUTPUT ||
		t.token_type == FOR)
	{
		parse_stmtList();
	}
}

// stmt -> assign_stmt | while_stmt | if_stmt | for_statemnt
// stmt -> output_stmt | input_stmt
struct InstructionNode* Parser::parse_stmt()
{
	//initialize new instruction
	InstructionNode * inst = nullptr;

	Token t = lexer.peek(1);

	// if ID then parse assign_stmt
	if (t.token_type == ID)
	{
		//initialize the next instruction for an assignment statement
		inst = parse_assignStmt();
	}
	// if WHILE parse while statement
	else if (t.token_type == WHILE)
	{
		parse_whileStmt();
	}
	// if IF parse if statement
	else if (t.token_type == IF)
	{
		parse_ifStmt();
	}
	// if INPUT parse input statement
	else if (t.token_type == INPUT)
	{
		parse_inputStmt();
	}
	// if SWITCH parse swtich statement
	else if (t.token_type == SWITCH)
	{
		parse_switchStmt();
	}
	// if OUTPUT then parse output statement
	else if (t.token_type == OUTPUT)
	{
		parse_outputStmt();
	}
	// if FOR parse for statement
	else if (t.token_type == FOR)
	{
		parse_forStmt();
	}

	return inst;
}

// assign_stmt -> ID EQUAL primary SEMICOLON
// assign_stmt -> ID EQUAL expr SEMICOLON
struct InstructionNode* Parser::parse_assignStmt()
{

	//initialize new instruction
	InstructionNode * inst = new InstructionNode;

	Token tempToken = expect(ID);

	//initialize type for instruction
	inst->type = ASSIGN;

	//initialize struct
	inst->assign_inst.left_hand_side_index;
    inst->assign_inst.opernd1_index; 
    inst->assign_inst.opernd2_index;    
    inst->assign_inst.op;

	expect(EQUAL);

	// always parse a primary
	parse_primary();

	// peek to see if we need to parse an expression
	Token t = lexer.peek(1);

	// check if there is an operator (meaning you are parsing an expression)
	if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
	{
		// parse the rest of the expr
		parse_op();

		parse_primary();
	}

	expect(SEMICOLON);
}

// expr -> primary op primary
void Parser::parse_expr()
{
	parse_primary();

	parse_op();

	parse_primary();
}

// primary -> ID | NUM
void Parser::parse_primary()
{
	Token t = lexer.peek(1);

	if (t.token_type == ID)
	{
		expect(ID);
	}
	else
	{
		// otherwise expect a num and if its not it will syntax error
		expect(NUM);
	}
}

// op -> PLUS | MINUS | MULT | DIV
void Parser::parse_op()
{
	Token t = lexer.peek(1);

	// check if there is an operator (meaning you are parsing an expression)
	if (t.token_type == MINUS)
	{
		expect(MINUS);
	}
	else if (t.token_type == PLUS)
	{
		expect(PLUS);
	}
	else if (t.token_type == MULT)
	{
		expect(MULT);
	}
	else
	{
		expect(DIV);
	}
}

// output_stmt -> output ID SEMICOLON
struct InstructionNode* Parser::parse_outputStmt()
{
	expect(OUTPUT);

	expect(ID);

	expect(SEMICOLON);
}

// input_stmt -> input ID SEMICOLON
struct InstructionNode* Parser::parse_inputStmt()
{
	expect(INPUT);

	expect(ID);

	expect(SEMICOLON);
}

// while_stmt -> while condition body
struct InstructionNode* Parser::parse_whileStmt()
{
	expect(WHILE);

	parse_condition();

	parse_body();
}

// if_stmt -> IF condition body
struct InstructionNode* Parser::parse_ifStmt()
{
	expect(IF);

	parse_condition();

	parse_body();
}

// condition -> primary relop primary
void Parser::parse_condition()
{
	parse_primary();

	parse_relop();

	parse_primary();
}

// relop -> GREATER | LESS | NOTEQUAL
void Parser::parse_relop()
{
	Token t = lexer.peek(1);

	if (t.token_type == GREATER)
	{
		expect(GREATER);
	}
	else if (t.token_type == LESS)
	{
		expect(LESS);
	}
	else
	{
		expect(NOTEQUAL);
	}
}

// switch_stmt -> SWITCH ID LBRACE case_list RBRACE
// switch_stmt -> SWITCH ID LBRACE case_list default_case RBRACE
struct InstructionNode* Parser::parse_switchStmt()
{
	expect(SWITCH);

	expect(ID);

	expect(LBRACE);

	parse_caseList();

	Token t = lexer.peek(1);

	if (t.token_type == DEFAULT)
	{
		parse_defaultCase();
	}

	expect(RBRACE);
}

// for_stmt -> FOR LPAREN assign_stmt condition SEMICOLON assign_stmt RPAREN body
struct InstructionNode* Parser::parse_forStmt()
{
	expect(FOR);

	expect(LPAREN);

	parse_assignStmt();

	parse_condition();

	expect(SEMICOLON);

	parse_assignStmt();

	expect(RPAREN);

	parse_body();
}

// case_list -> case case_list | case
void Parser::parse_caseList()
{
	parse_case();

	Token t = lexer.peek(1);

	// check if there is a case_list
	if (t.token_type == CASE)
	{
		parse_caseList();
	}
}

// case -> CASE NUM COLON body
void Parser::parse_case()
{
	expect(CASE);

	expect(NUM);

	expect(COLON);

	parse_body();
}

// default_case -> DEFAULT COLON body
void Parser::parse_defaultCase()
{
	expect(DEFAULT);

	expect(COLON);

	parse_body();
}

// inputs -> num_list
void Parser::parse_inputs()
{
	parse_numList();
}

// num_list -> NUM
// num_list -> NUM num_list
void Parser::parse_numList()
{
	expect(NUM);

	Token t = lexer.peek(1);

	// check if there is a num_list
	if (t.token_type == NUM)
	{
		parse_numList();
	}
}

int main(int argc, char *argv[])
{
	return 0;
}
