#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>
#include <ctype.h>
#include <math.h>

#include "tokenizator.h"
#include "tree_expression.h"

const size_t SIZE_OF_OPER = 16;

operation_t get_op (const char *oper_str, int shift);
int get_num (const char *str, int *index);
int is_new_var(Token *expr, char *oper, int shift);

Node *token (Token *expr)
{
    assert(expr);

    int index = 0;
    int jndex = 0;
    size_t size = strlen(expr->string_with_expr);

    Node *array = (Node *)calloc(size, sizeof(Node));
    if (!array)
        return NULL;

    while (expr->string_with_expr[index] != '$')
    {
        if (isdigit(expr->string_with_expr[index]))
        {
            array[jndex].value.data.number = get_num(expr->string_with_expr, &index);
            array[jndex++].value.type = NODE_TYPE_NUM;
        }
        else if (isalpha(expr->string_with_expr[index]))
        {
            int count_shift = 0;
            while (isalpha(expr->string_with_expr[index + count_shift]))
                count_shift++;

            operation_t oper = get_op(expr->string_with_expr + index, count_shift);
            if (oper != OPERATION_UNKNOWN)
            {
                array[jndex].value.data.operation = oper;
                array[jndex++].value.type = NODE_TYPE_OPER;
            }
            else
            {

                array[jndex].value.type = NODE_TYPE_VAR;
                int position = is_new_var(expr, expr->string_with_expr + index, count_shift);
                if (position == -1)
                {
                    array[jndex++].value.data.id = expr->amount_of_id;
                    expr->array_of_id[expr->amount_of_id].name = expr->string_with_expr + index;
                    expr->array_of_id[expr->amount_of_id++].size = count_shift;
                }
                else
                    array[jndex++].value.data.id = position;

            }
            index += count_shift;
        }
        else if ((expr->string_with_expr[index]) == ' ')
        {
            while ((expr->string_with_expr[index]) == ' ')
            {
                (index)++;
            }
        }
        else
        {
            operation_t oper = get_op(expr->string_with_expr + index, 1);

            if (oper != OPERATION_UNKNOWN)
            {
                if (oper == OPERATION_IMP)
                    index++;
                index += 1;
                array[jndex].value.data.operation = oper;
                array[jndex++].value.type = NODE_TYPE_OPER;
            }
            else
            {
                assert(0 && "ERROR, not correct oper");
            }
        }

    }
    array[jndex].value.data.operation = OPERATION_END;
    array[jndex++].value.type = NODE_TYPE_OPER;

    return array;
}

operation_t get_op (const char *oper_str, int shift)
{
    assert(oper_str);

    operation_t name_of_oper = OPERATION_UNKNOWN;
    for (size_t i = 1; i < OPER_ARRAY_SIZE; i++)
    {
        if (strncmp(oper_str, array_of_oper[i].str_operation, shift) == 0)
        {
           name_of_oper = array_of_oper[i].operation;
            break;
        }
    }
    return name_of_oper;
}
int get_num (const char *str, int *index)
{
    assert(str);
    assert(index);

    int num = 0;

    while (isdigit(str[*index]))
    {
        num += num*10 + str[*index] -'0';
        (*index)++;

    }

    return num;
}

int is_new_var(Token *expr, char *oper, int shift)
{
    for (int i = 0; i < expr->amount_of_id; i++)
    {
        if (strncmp(expr->array_of_id[i].name, oper, shift) == 0)
            return i;
    }
    return -1;
}
void print_token (Node *node, Token *expr, size_t size)
{
    for (size_t i = 0; i < size-1; i++)
    {
        if (node[i].value.type == NODE_TYPE_NUM)
            printf ("%d", node[i].value.data.number);
        else if (node[i].value.type == NODE_TYPE_VAR)
            printf("%.*s",expr->array_of_id[node[i].value.data.id].size, expr->array_of_id[node[i].value.data.id].name);
        else if (node[i].value.data.operation == OPERATION_END)
            break;
        else
        {
            for (size_t j = 1; j < OPER_ARRAY_SIZE; j++)
            {
                if (node[i].value.data.operation == array_of_oper[j].operation)
                {
                    printf("%s", array_of_oper[j].str_operation);
                }
            }
        }
    }
    printf("\n");
}


