#include <stdio.h>
#include "expression.h"
#include <stdlib.h>

int eval_expr(struct my_expr *expr)
{
    if (expr->type == EXPR_ADDITION)
    {
        return eval_expr(expr->data.children.left)
            + eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_SUBTRACTION)
    {
        return eval_expr(expr->data.children.left)
            - eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_MULTIPLICATION)
    {
        return eval_expr(expr->data.children.left)
            * eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_DIVISION)
    {
        if (expr->data.children.right->data.value == 0)
            exit(1);
        return eval_expr(expr->data.children.left)
            / eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_NEGATION)
    {
        return -1 * eval_expr(expr->data.children.left);
    }
    else if (expr->type == EXPR_NUMBER)
    {
        return expr->data.value;
    }

    return 0;
}
