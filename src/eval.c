#include "calc.h"
#include <math.h>

double eval_rpn(const Token *rpn, int token_count, int *error) {
    double stack[STACK_SIZE];
    int top = -1;
    *error = 0;
    
    for (int i = 0; i < token_count; i++) {
        const Token *t = &rpn[i];
        
        if (t->type == TOK_NUM) {
            if (top >= STACK_SIZE - 1) { *error = 1; return 0; }
            stack[++top] = t->value;
        } else {
            if (top < 1) { *error = 1; return 0; }
            double b = stack[top--];
            double a = stack[top--];
            double res = 0;
            
            switch (t->type) {
                case TOK_OP_ADD: res = a + b; break;
                case TOK_OP_SUB: res = a - b; break;
                case TOK_OP_MUL: res = a * b; break;
                case TOK_OP_DIV: 
                    if (b == 0) { *error = 2; return 0; }  // /0 (err)
                    res = a / b; 
                    break;
                default: *error = 1; return 0;
            }
            stack[++top] = res;
        }
    }
    
    if (top != 0) { *error = 1; return 0; }
    return stack[0];
}