#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "tree_expression.h"
#include "tree_dif_dump.h"
#include "descent_read.h"
#include "tokenizator.h"
#include "truth_table.h"

int main()
{
    struct Token expr = {};
    expr.capacity = 15;
    expr.amount_of_id = 0;
    //expr.string_with_expr = "x&y$";

    printf("Print expression (oper = '|' '&' '->' '!' ):\n");
    scanf("%s", expr.string_with_expr);
    expr.string_with_expr[strlen(expr.string_with_expr)] = '$';
    printf ("%s\n", expr.string_with_expr);
    expr.array_of_id = (Var*) calloc(expr.capacity, sizeof(Var));
    expr.root = token (&expr);
    print_token(expr.root, &expr, strlen(expr.string_with_expr));
    int index = 0;
    expr.root = GetG(expr.root, &index);

    draw_and_show(&expr);

}


