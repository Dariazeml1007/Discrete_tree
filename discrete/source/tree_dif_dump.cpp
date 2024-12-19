#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "tree_expression.h"
#include "tree_dif_dump.h"

static void print_node_in_file (Node *node, Token *expr, FILE * pointer_file);
static void print_edge_in_file (Node *node, FILE * pointer_file);
static void print_node_in_file_for_use (Node *node, Token *expr, FILE * pointer_file);
int dump (Token *expr, const char *name_of_dump_file, bool for_use)
{
    assert(expr);
    assert(name_of_dump_file);

    FILE *pointer_file = fopen (name_of_dump_file, "w");
    if (!pointer_file)
        return DIF_FILE_NOT_OPENED;

    fprintf (pointer_file,  "digraph structs\n"
                            "{\n"
                            "\trankdir=HR;\n"
                            "\tnode[shape = circle, color= \"#003A8C\",style=\"filled\", fillcolor=\"#B4FBFF\", fontsize=14];\n"
                            "\tedge[color=\"black\", weight = 9,fontsize=14];"
                            "\n");
    if (for_use)
        print_node_in_file_for_use(expr->root, expr, pointer_file);
    else
        print_node_in_file(expr->root, expr, pointer_file);
    print_edge_in_file(expr->root, pointer_file);
    fprintf (pointer_file, "\n}");

    if (fclose(pointer_file) != 0)
        return DIF_FILE_NOT_CLOSED;

    return DIF_SUCCESS;

}


void print_node_in_file_for_use (Node *node, Token *expr, FILE * pointer_file)
{

    if (node == NULL)
    {
        printf ("NULL pointer\n");
        return;
    }

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_NUM)
             fprintf (pointer_file, "\tel_%p [shape=circle, label= \""
                                    "%d\"];\n", node, node->value.data.number);

        else if (node->value.type == NODE_TYPE_VAR)
            fprintf (pointer_file, "\tel_%p [shape=circle, label= \""
                                   "%.*s\"];\n", node,expr->array_of_id[node->value.data.id].size,  expr->array_of_id[node->value.data.id].name);
    }

    else
    {
        for (size_t i = 1; i < OPER_ARRAY_SIZE; i++)
        {
            if (node->value.data.operation == array_of_oper[i].operation)
            {
                if (node->value.data.operation == OPERATION_IMP)
                    fprintf (pointer_file, "\tel_%p [shape=circle, label= \""
                                   "IMP\"];\n", node);

                else
                fprintf (pointer_file, "\tel_%p [shape=circle, label= \""
                                   "%s\"];\n",node, array_of_oper[i].str_operation);
            }
        }

    }

    if (node->left)
        print_node_in_file_for_use(node->left, expr, pointer_file);

    if (node->right)
        print_node_in_file_for_use(node->right, expr,  pointer_file);
}

void print_node_in_file (Node *node, Token *expr, FILE * pointer_file)
{

    if (node == NULL)
    {
        printf ("NULL pointer\n");
        return;
    }

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_NUM)
             fprintf (pointer_file, "\tel_%p [shape=record, label= \"address = %p|"
                                    "%d| addr_parent = %p\"];\n", node, node, node->value.data.number, node->parent);

        else if (node->value.type == NODE_TYPE_VAR)
            fprintf (pointer_file, "\tel_%p [shape=record, label= \"address = %p|"
                                   "%.*s| addr_parent = %p\"];\n", node, node, expr->array_of_id[node->value.data.id].size,  expr->array_of_id[node->value.data.id].name, node->parent);

    }

    else
    {
        for (size_t i = 1; i < OPER_ARRAY_SIZE; i++)
        {
            if (node->value.data.operation == array_of_oper[i].operation)
            {
                if (node->value.data.operation == OPERATION_IMP)
                    fprintf (pointer_file, "\tel_%p [shape=record, label= \"addr_left = %p | address = %p|"
                                   "IMP| addr_parent = %p | addr_right = %p\"];\n", node, node->left, node, node->parent, node->right);

                else
                fprintf (pointer_file, "\tel_%p [shape=record, label= \"addr_left = %p | address = %p|"
                                   "%s| addr_parent = %p | addr_right = %p\"];\n", node, node->left, node, array_of_oper[i].str_operation, node->parent, node->right);
            }
        }

    }

    if (node->left)
        print_node_in_file(node->left, expr, pointer_file);

    if (node->right)
        print_node_in_file(node->right, expr,  pointer_file);
}

void print_edge_in_file (Node *node, FILE * pointer_file)
{
    if (node == NULL)
    {
        printf ("NULL pointer\n");
        return;
    }

    if (node->left)
    {
        fprintf (pointer_file, "\tel_%p", node);
        fprintf (pointer_file, "->");
        fprintf (pointer_file, "el_%p;\n", node->left);
        print_edge_in_file(node->left, pointer_file);
    }
    if (node->right)
    {
        fprintf (pointer_file, "\tel_%p", node);
        fprintf (pointer_file, "->");
        fprintf (pointer_file, "el_%p;\n", node->right);
        print_edge_in_file(node->right, pointer_file);
    }
}
