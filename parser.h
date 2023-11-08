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
    void parse_program();
    void parse_varSection();
    void parse_idList();
    void parse_body();
    void parse_stmtList();
    void parse_stmt();
    void parse_assignStmt();
    void parse_expr();
    void parse_primary();
    void parse_op();
    void parse_outputStmt();
    void parse_inputStmt();
    void parse_whileStmt();
    void parse_ifStmt();
    void parse_condition();
    void parse_relop();
    void parse_switchStmt();
    void parse_forStmt();
    void parse_caseList();
    void parse_case();
    void parse_defaultCase();
    void parse_inputs();
    void parse_numList();


};

#endif
