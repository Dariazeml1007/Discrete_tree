#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "descent_read.h"
#include "tree_expression.h"

Node *GetG(Node *root, int *index)
{
    Node *val = GetIMP(root, index);
    if (root[*index].value.data.operation != OPERATION_END)
        SyntaxError(index);
    (*index)++;
    return val;
}

Node *GetIMP (Node *root, int *index)
{
    Node *val = GetOR(root, index);
    while (root[*index].value.data.operation == OPERATION_IMP)
    {
        Node *old = val;
        val = root + *index;

        val->left = old;
        val->left->parent = val;
        (*index)++;

        val->right = GetOR(root, index);
        val->right->parent = val;
    }
    return val;
}

Node *GetOR (Node *root, int *index)
{
    Node *val = GetAND(root, index);
    while (root[*index].value.data.operation == OPERATION_OR)
    {
        Node *old = val;
        val = root + *index;

        val->left = old;
        val->left->parent = val;
        (*index)++;

        val->right = GetAND(root, index);
        val->right->parent = val;
    }
    return val;
}

Node * GetAND(Node *root, int *index)
{
    Node *val = GetP(root, index);
    while (root[*index].value.data.operation == OPERATION_AND )
    {
        Node *old = val;
        val = root + *index;

        old->parent = val;
        val->left = old;
        (*index)++;

        val->right = GetP(root, index);
        val->right->parent = val;
    }
    return val;
}

Node *GetP (Node *root, int *index)
{
    if (root[*index].value.data.operation == OPERATION_OPEN_BRACKET)
    {
        (*index)++;
        Node * val = GetIMP(root, index);
        if (root[*index].value.data.operation != OPERATION_CLOSE_BRACKET)
            SyntaxError(index);
        (*index)++;
        return val;
    }
    if (root[*index].value.type == NODE_TYPE_NUM)
        return GetN(root, index);
    if (root[*index].value.type == NODE_TYPE_VAR)
        return GetVar(root, index);
    if (root[*index].value.type == NODE_TYPE_OPER)
        return GetOp(root, index);

    SyntaxError(index);

    return NULL;
}

Node *GetOp (Node *root, int *index)
{

    if (root[*index].value.type != NODE_TYPE_OPER)
        SyntaxError(index);

    Node *res = root + *index;
    (*index)++;

    if (root[*index].value.data.operation == OPERATION_OPEN_BRACKET)
    {
        (*index)++;

        res->right = GetIMP(root, index);
        res->right->parent= res;

        if (root[*index].value.data.operation != OPERATION_CLOSE_BRACKET)
            SyntaxError(index);
        (*index)++;
        return res;
    }
    else
    {
        res->right = GetP(root, index);
        res->right->parent= res;
        return res;
    }
    return NULL;
}

Node * GetVar(Node *root, int *index)
{
    if (root[*index].value.type != NODE_TYPE_VAR)
        SyntaxError(index);

    Node *res = root + *index;
    (*index)++;
    return res;
}



Node * GetN(Node *root, int *index)
{

    if (root[*index].value.type != NODE_TYPE_NUM)
        SyntaxError(index);
    Node *res = root + *index;
    (*index)++;
    printf ("getN %d, index = %d\n", res->value.data.number, *index);
    return res;
}


void SyntaxError(int *p)
{
    printf ("ERROR %d", *p);
    exit(0);
}
