#ifndef AST_H
#define AST_H

// Типи вузлів дерева
typedef enum {
    NODE_NUMBER,    // числове значення (3.14, 42)
    NODE_VAR,       // змінна (x, y)
    NODE_ASSIGN,    // присвоєння (x = 5)
    NODE_ADD,       // +
    NODE_SUB,       // -
    NODE_MUL,       // *
    NODE_DIV,       // /
    NODE_NEG,       // унарний мінус (-x)
} NodeType;

// Вузол дерева AST
typedef struct ASTNode {
    NodeType type;          // тип вузла
    double value;           // числове значення (для NODE_NUMBER)
    char *name;             // ім'я змінної (для NODE_VAR, NODE_ASSIGN)
    struct ASTNode *left;   // лівий дочірній вузол
    struct ASTNode *right;  // правий дочірній вузол
} ASTNode;

// Створення вузлів
ASTNode *make_number(double value);
ASTNode *make_var(const char *name);
ASTNode *make_assign(const char *name, ASTNode *expr);
ASTNode *make_binop(NodeType type, ASTNode *left, ASTNode *right);
ASTNode *make_neg(ASTNode *operand);

// Обчислення і виведення
double eval(ASTNode *node);
void print_ast(ASTNode *node, int depth);
void ast_to_json(ASTNode *node);
void free_ast(ASTNode *node);

#endif
