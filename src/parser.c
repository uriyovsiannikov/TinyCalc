#include "calc.h"
#include <ctype.h>
#include <string.h>

static int op_precedence(TokenType op) {
    switch (op) {
        case TOK_OP_ADD: case TOK_OP_SUB: return 1;
        case TOK_OP_MUL: case TOK_OP_DIV: return 2;
        default: return 0;
    }
}

static TokenType char_to_op(char c) {
    switch (c) {
        case '+': return TOK_OP_ADD;
        case '-': return TOK_OP_SUB;
        case '*': return TOK_OP_MUL;
        case '/': return TOK_OP_DIV;
        default: return TOK_NUM;
    }
}

int parse_expr(const char *expr, Token *rpn, int max_tokens) {
    Token op_stack[STACK_SIZE];
    int op_top = -1;
    int rpn_idx = 0;
    
    const char *p = expr;
    
    while (*p && rpn_idx < max_tokens) {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }
        
        if (isdigit((unsigned char)*p) || *p == '.') {
            char *end;
            double val = strtod(p, &end);
            rpn[rpn_idx].type = TOK_NUM;
            rpn[rpn_idx].value = val;
            rpn_idx++;
            p = end;
            continue;
        }
        
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            TokenType cur_op = char_to_op(*p);
            int prec = op_precedence(cur_op);
            
            while (op_top >= 0 && op_precedence(op_stack[op_top].type) >= prec) {
                if (rpn_idx >= max_tokens) return -1;
                rpn[rpn_idx++] = op_stack[op_top--];
            }
            
            op_top++;
            op_stack[op_top].type = cur_op;
            p++;
            continue;
        }
        
        return -1;
    }
    
    while (op_top >= 0 && rpn_idx < max_tokens) {
        rpn[rpn_idx++] = op_stack[op_top--];
    }
    
    return rpn_idx;
}