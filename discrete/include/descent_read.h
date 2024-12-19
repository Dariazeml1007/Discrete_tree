#ifndef READ_DES
#define READ_DES

#include "tree_expression.h"
void SyntaxError(int *p);
Node *GetG   (Node *root, int *index);
Node *GetNOT (Node *root, int *index);
Node *GetIMP (Node *root, int *index);
Node *GetAND (Node *root, int *index);
Node *GetOR (Node *root, int *index);
Node *GetP   (Node *root, int *index);
Node *GetN   (Node *root, int *index);
Node *GetVar (Node *root, int *index);
Node *GetOp  (Node *root, int *index);


#endif
