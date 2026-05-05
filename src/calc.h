#ifndef CALC_H
#define CALC_H

#include <gtk/gtk.h>

#define MAX_EXPR_LEN 256

#define STACK_SIZE 64

typedef enum {
    TOK_NUM,    // num
    TOK_OP_ADD, // +
    TOK_OP_SUB, // -
    TOK_OP_MUL, // *
    TOK_OP_DIV  // /
} TokenType;

typedef struct {
    TokenType type;
    double    value;
} Token;

int  parse_expr(const char *expr, Token *rpn, int max_tokens);
double eval_rpn(const Token *rpn, int token_count, int *error);

extern GtkWidget *display;
GtkWidget* create_ui(void);
#endif