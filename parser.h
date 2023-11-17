/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 * 
 * AUTHOR: JAMES ZANETTI
 * COURSE: CSE340
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <map>
#include "lexer.h"
#include <set>

class Parser {
  public:
    LexicalAnalyzer lexer;
    Token expect(TokenType expected_type);
    void syntax_error();
  private:
    struct InstructionNode* parse_body();
    struct InstructionNode* parse_stmtList();
    struct InstructionNode* parse_stmt();
    void parse_program();
    void parse_varSection();
    void parse_idList();
    struct InstructionNode* parse_assignStmt();
    void parse_expr();
    void parse_primary();
    void parse_op();
    struct InstructionNode* parse_outputStmt();
    struct InstructionNode* parse_inputStmt();
    struct InstructionNode* parse_whileStmt();
    struct InstructionNode* parse_ifStmt();
    void parse_condition();
    void parse_relop();
    struct InstructionNode* parse_switchStmt();
    struct InstructionNode* parse_forStmt();
    void parse_caseList();
    void parse_case();
    void parse_defaultCase();
    void parse_inputs();
    void parse_numList();
};

#endif
