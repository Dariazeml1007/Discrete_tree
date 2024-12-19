#ifndef DIF_TREE
#define DIF_TREE

#include <stdint.h>

#define NUM(a) new_node(NULL, NULL, (struct Node_value){.type = NODE_TYPE_NUM, .data = {.number = a}})
#define ADD(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_ADD}})
#define SUB(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_SUB}})
#define MUL(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_MUL}})
#define DIV(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_DIV}})
#define POW(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_POWER}})
#define SIN(a) new_node(NULL, a, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_SIN}})
#define COS(a) new_node(NULL, a, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_COS}})
const size_t size_of_var = 16;

typedef enum
{
    DIF_SUCCESS = 0,
    DIF_ALLOCATION_MEMORY_ERROR = 1,
    DIF_FILE_NOT_OPENED = 2,
    DIF_FILE_NOT_CLOSED = 3,
    DIF_STAT_ERROR = 4,
    DIF_NO_FREE_ELEM = 5,
    NO_SUCH_ITEM_ERROR = 6,
    DIF_READ_ERROR = 7,
    DIF_INSERT_ERROR = 8

}Dif_errors;

enum Info_t
{
    NODE_TYPE_NUM = 1,
    NODE_TYPE_VAR = 2,
    NODE_TYPE_OPER = 3

};

enum operation_t
{
    OPERATION_UNKNOWN = 0,
    OPERATION_AND = 1,
    OPERATION_OR = 2,
    OPERATION_NOT = 3,
    OPERATION_IMP = 4,
    OPERATION_OPEN_BRACKET = 5,
    OPERATION_CLOSE_BRACKET = 6,
    OPERATION_END = 7


};

struct Node_value
{
    Info_t type;
    union
    {
        int number;
        operation_t operation;
        int id;

    }data;
};

struct Node
{

    Node *left;
    Node *right;
    Node *parent;
    Node_value value;
};

struct oper_prototype
{
    operation_t operation;
    const char *str_operation;
};

static const oper_prototype array_of_oper[] =
{
    {OPERATION_UNKNOWN, ""/*EMPTY SPACE FOR ERRORS CHECKING*/},
    {OPERATION_AND, "&"},
    {OPERATION_OR, "|"},
    {OPERATION_NOT, "!"},
    {OPERATION_IMP, "->"},
    {OPERATION_OPEN_BRACKET, "("},
    {OPERATION_CLOSE_BRACKET, ")"},
    {OPERATION_END, "$"}
};
static const size_t OPER_ARRAY_SIZE = sizeof(array_of_oper)/sizeof(array_of_oper[0]);

struct Var
{
    char *name;
    int size;
};
struct Token
{
    Node *root;
    int capacity;
    int amount_of_id;
    char string_with_expr[40];
    Var *array_of_id;
};

Node * new_node (Node *left, Node *right, Node_value val);
void print_tree (Node *node, Token *expr);
bool is_leaf(Node *node);
void node_dtor (Node *node);
void draw_and_show (Token *expr);


#endif
