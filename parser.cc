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
#include <unordered_map>
#include <set>
#include <stack>
#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

//initialize lexer object
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
    //get token
    Token t = lexer.GetToken();

    if (t.token_type != expected_type)
        {
            syntax_error();
        }
    return t;
}

//function to parse a provided rule_list
//Rule-list -> Rule Rule-list | Rule
void Parser::parse_rule_list()
{
	parse_rule();

	//store next token in list
    Token t = lexer.peek(1);

    //check if token type is ID since Rules always start with ID
    if(t.token_type == ID)
    {
        parse_rule_list();
    }

}

//function to parse a provided rule
//Rule → ID ARROW Right-hand-side STAR
void Parser::parse_rule()
{
	//store next token in list
    Token t = lexer.peek(1);

	//store LHS grammar rule into the string
	Rule.LHS = t.lexeme;

	//add the parsed symbol to the order list
    Rule.parsedOrder.push_back(Rule.LHS);

	if (Rule.T.find(t.lexeme) != Rule.T.end()) 
	{
        // Add it to NT set
        Rule.NT.insert(t.lexeme);

        // Remove it from T set
        Rule.T.erase(t.lexeme);

    } 
	else 
	{
        // If the LHS token is not a terminal, add it to both NT and RHS sets
        Rule.NT.insert(t.lexeme);

    }
	
	//consume ID, maybe its because im calling a parser object declared at the top of the file?
    parser.expect(ID);

	//consume ARROW
	parser.expect(ARROW);

	//clear RHS vector to start fresh
    Rule.RHS.clear();

	//parse RHS
	parse_rhs();

	//CONSUME STAR
	parser.expect(STAR);

	//add the parsed rule to the ruleMap
    Rule.ruleMap[Rule.LHS].push_back(Rule.RHS);
}

//function to parse the right hand side of a given grammar
//Right-hand-side → Id-list | epsilon
//Id-list → ID Id-list | ID
void Parser::parse_rhs()
{
	Token t = lexer.peek(1);

	//check if we need to parse id list
	if(t.token_type == ID)
	{
		parse_id_list();
	}
	//if epsilon
	else
	{
		return;
	}
}

//function to parse a given id list
//Id-list → ID Id-list | ID
void Parser::parse_id_list()
{
	//peek for token
	Token t = lexer.peek(1);

	//initialize a vector to store the RHS for the current rule
    vector<Token> tokenVector;
    
    //store the first ID in the token vector
    //tokenVector.push_back(t);

	//after RHS and LHS is initialized for a given rule initialize it to rule map
	Rule.RHS.push_back(t);

	//add the parsed symbol to the order list
    Rule.parsedOrder.push_back(t.lexeme);

	//check if the rule exists in the non terminal list
	// Check if the token is in NT
    if (Rule.NT.find(t.lexeme) != Rule.NT.end()) 
	{
		//remove it from the set of terminals
        Rule.T.erase(t.lexeme);  
    } 
	else 
	{
		//add to the set of terminals
        Rule.T.insert(t.lexeme);
    }
	
	//consume ID
	expect(ID);

	// Add the token vector to the RHS
    //Rule.RHS.push_back(tokenVector);

	// Check if the rule exists in the non-terminal list
    if (Rule.NT.find(t.lexeme) == Rule.NT.end()) {
        // Add to the set of terminals
        Rule.T.insert(t.lexeme);
    }

	//store next token in list
    t = lexer.peek(1);

    //check if next token in list is an ID list
    if(t.token_type == ID)
    {	
		parse_id_list();
    }
    else
    {
        return;
    }

}

//program -> var_section body inputs
void Parser::parse_program()
{
	parse_varSection();

	parse_body();

	parse_inputs();

	expect(END_OF_FILE);
}

//var_section -> id_list SEMICOLON
void Parser::parse_varSection()
{
	parse_idList();

	expect(SEMICOLON);
}

//id_list -> ID COMMA id_list | ID
void Parser::parse_idList()
{
	expect(ID);

	//peek at the next token
	Token t = lexer.peek(1);

	if(t.token_type == COMMA)
	{
		expect(COMMA);

		parse_idList();
	}
	
}

//body -> LBRACE stmt_list RBRACE
void Parser::parse_body()
{
	expect(LBRACE);

	parse_stmtList();

	expect(RBRACE);
}

//stmt_list -> stmt stmt_list stmt
void Parser::parse_stmtList()
{
	parse_stmt();

	//peek at the next token
	Token t = lexer.peek(1);

	//check if there is a statement list
	if(t.token_type == ID ||
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

//stmt -> assign_stmt | while_stmt | if_stmt | for_statemnt
//stmt -> output_stmt | input_stmt
void Parser::parse_stmt()
{
	Token t = lexer.peek(1);

	//if ID then parse assign_stmt
	if(t.token_type == ID)
	{
		parse_assignStmt();
	}
	//if WHILE parse while statement
	else if(t.token_type == WHILE)
	{
		parse_whileStmt();
	}
	//if IF parse if statement
	else if(t.token_type == IF)
	{
		parse_ifStmt();
	}
	//if INPUT parse input statement
	else if(t.token_type == INPUT)
	{
		parse_inputStmt();
	}
	//if SWITCH parse swtich statement
	else if(t.token_type == SWITCH)
	{
		parse_switchStmt();
	}
	//if OUTPUT then parse output statement
	else if(t.token_type == OUTPUT)
	{
		parse_outputStmt();
	}
	//if FOR parse for statement
	else if(t.token_type == FOR)
	{
		parse_forStmt();
	}
}

//assign_stmt -> ID EQUAL primary SEMICOLON
//assign_stmt -> ID EQUAL expr SEMICOLON
void Parser::parse_assignStmt()
{

}

int main (int argc, char* argv[])
{
	return 0;
}

