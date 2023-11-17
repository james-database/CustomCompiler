/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>
#include <vector>

extern int mem[1000];
extern int next_available;

extern std::vector<int> inputs;
extern int next_input;

enum ArithmeticOperatorType {
    OPERATOR_NONE = 123,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULT,
    OPERATOR_DIV
};

enum ConditionalOperatorType {
    CONDITION_GREATER = 345,
    CONDITION_LESS,
    CONDITION_NOTEQUAL
};

enum InstructionType
{
    NOOP = 1000,
    IN,
    OUT,
    ASSIGN,
    CJMP,
    JMP
};

struct InstructionNode
{
    //TYPES:   NOOP, ASSIGN, JMP, CJMP (conditional jump), IN, OUT
    InstructionType type;

    union
    {
        struct
        {
            int left_hand_side_index;
            int opernd1_index;
            int opernd2_index;
            
            /*
             * If op == OPERATOR_NONE then only opernd1 is meaningful.
             * Otherwise both opernds are meaningful
             */
            ArithmeticOperatorType op;

        } assign_inst;
        
        struct
        {
            int var_index;
        } input_inst;
        
        struct
        {
            int var_index;
        } output_inst;
        
        struct {

            ConditionalOperatorType condition_op;
            int opernd1_index;
            int opernd2_index;

            //instruction to jump to
            struct InstructionNode * target;

        } cjmp_inst;
        
        struct {
            //instruction to jump to
            struct InstructionNode * target;
        } jmp_inst;
  
    };

    struct InstructionNode * next; // next statement in the list or NULL
};

void debug(const char* format, ...);

//---------------------------------------------------------
// You should write the following function:

//locates a variable name within the mem integer array
int location(std::string varName);

struct InstructionNode * parse_Generate_Intermediate_Representation();



#endif /* _COMPILER_H_ */
