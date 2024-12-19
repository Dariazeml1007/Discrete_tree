#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>
#include <stdarg.h>

#include "tree_expression.h"
#include "tree_dif_dump.h"

Node *new_node (Node *left, Node *right, Node_value val)
{
    struct Node *node = (Node *) calloc (1, sizeof(Node));
    if (!node)
        return NULL;

    node->value = val;
    node->right = right;
    node->left = left;

    if (left)
        left->parent = node;

    if (right)
        right->parent = node;

    return node;
}

void print_tree (Node *node, Token *expr)
{

    if (node == NULL)
    {
        printf ("NULL pointer\n");
        return;
    }

    if (node->left)
    {
        print_tree(node->left, expr);
    }

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_NUM)
        {
                printf ("%d", node->value.data.number);
        }
        else if (node->value.type == NODE_TYPE_VAR)
            printf ("%.*s", expr->array_of_id[node->value.data.id].size, expr->array_of_id[node->value.data.id].name);
    }
    else
    {
        printf ("%s", array_of_oper[node->value.data.operation].str_operation);

    }

    if (node->right)
    {
        print_tree(node->right, expr);


    }

}


bool is_leaf(Node *node)
{
    assert(node);

    return !node->left && !node->right;
}

void node_dtor (Node *node)
{
    assert(node);

    if (node->right)
    {
        if (!is_leaf(node->right))
            node_dtor(node->right);

        else
            free(node->right);
    }

    if (node->left)
    {
        if (!is_leaf(node->left))
            node_dtor(node->left);

        else
            free(node->left);

    }
    free(node);
    return;
}

void draw_and_show (Token *expr)
{
    assert(expr);

    printf("the tree :");
    print_tree(expr->root, expr);
    printf("\n");

    // dump(expr, "dump.dot", false);
    // system("dot dump.dot -Tpng -o tree.png");

    dump(expr, "dump_for_use.dot", true);
    system("dot dump_for_use.dot -Tpng -o tree_for_user.png");
}
