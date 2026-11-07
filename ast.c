#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


#define MAX_VARS 64

typedef struct {
    char name[64];
    double value;
} Variable;

static Variable vars[MAX_VARS];
static int var_count = 0;

static double get_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }
    fprintf(stderr, "Невідома змінна: %s\n", name);
    return 0.0;
}

static void set_var(const char *name, double value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strncpy(vars[var_count].name, name, 63);
        vars[var_count].value = value;
        var_count++;
    }
}

// ─────────────────────────────────────────────
// Створення вузлів
// ─────────────────────────────────────────────

ASTNode *make_number(double value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type  = NODE_NUMBER;
    node->value = value;
    node->name  = NULL;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

ASTNode *make_var(const char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type  = NODE_VAR;
    node->value = 0;
    node->name  = strdup(name);
    node->left  = NULL;
    node->right = NULL;
    return node;
}

ASTNode *make_assign(const char *name, ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type  = NODE_ASSIGN;
    node->value = 0;
    node->name  = strdup(name);
    node->left  = NULL;
    node->right = expr;
    return node;
}

ASTNode *make_binop(NodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type  = type;
    node->value = 0;
    node->name  = NULL;
    node->left  = left;
    node->right = right;
    return node;
}

ASTNode *make_neg(ASTNode *operand) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type  = NODE_NEG;
    node->value = 0;
    node->name  = NULL;
    node->left  = operand;
    node->right = NULL;
    return node;
}

// ─────────────────────────────────────────────
// Обчислення виразу (рекурсивне)
// ─────────────────────────────────────────────

double eval(ASTNode *node) {
    if (!node) return 0.0;

    switch (node->type) {
        case NODE_NUMBER:
            return node->value;

        case NODE_VAR:
            return get_var(node->name);

        case NODE_ASSIGN: {
            double val = eval(node->right);
            set_var(node->name, val);
            return val;
        }

        case NODE_ADD:
            return eval(node->left) + eval(node->right);

        case NODE_SUB:
            return eval(node->left) - eval(node->right);

        case NODE_MUL:
            return eval(node->left) * eval(node->right);

        case NODE_DIV: {
            double divisor = eval(node->right);
            if (divisor == 0.0) {
                fprintf(stderr, "Помилка: ділення на нуль\n");
                return 0.0;
            }
            return eval(node->left) / divisor;
        }

        case NODE_NEG:
            return -eval(node->left);
    }
    return 0.0;
}

// ─────────────────────────────────────────────
// Виведення дерева в термінал
// ─────────────────────────────────────────────

void print_ast(ASTNode *node, int depth) {
    if (!node) return;

    // Відступ
    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case NODE_NUMBER:
            printf("NUMBER(%.6g)\n", node->value);
            break;
        case NODE_VAR:
            printf("VAR(%s)\n", node->name);
            break;
        case NODE_ASSIGN:
            printf("ASSIGN(%s)\n", node->name);
            break;
        case NODE_ADD:  printf("ADD\n");  break;
        case NODE_SUB:  printf("SUB\n");  break;
        case NODE_MUL:  printf("MUL\n");  break;
        case NODE_DIV:  printf("DIV\n");  break;
        case NODE_NEG:  printf("NEG\n");  break;
    }

    print_ast(node->left,  depth + 1);
    print_ast(node->right, depth + 1);
}

// ─────────────────────────────────────────────
// Конвертація в JSON для d3.js
// ─────────────────────────────────────────────

static void ast_to_json_inner(ASTNode *node) {
    if (!node) return;

    printf("{");

    switch (node->type) {
        case NODE_NUMBER:
            printf("\"name\": \"%.6g\"", node->value);
            break;
        case NODE_VAR:
            printf("\"name\": \"%s\"", node->name);
            break;
        case NODE_ASSIGN:
            printf("\"name\": \"= (%s)\"", node->name);
            break;
        case NODE_ADD:  printf("\"name\": \"+\"");  break;
        case NODE_SUB:  printf("\"name\": \"-\"");  break;
        case NODE_MUL:  printf("\"name\": \"*\"");  break;
        case NODE_DIV:  printf("\"name\": \"/\"");  break;
        case NODE_NEG:  printf("\"name\": \"neg\""); break;
    }

    // Дочірні вузли
    if (node->left || node->right) {
        printf(", \"children\": [");
        int first = 1;
        if (node->left) {
            ast_to_json_inner(node->left);
            first = 0;
        }
        if (node->right) {
            if (!first) printf(", ");
            ast_to_json_inner(node->right);
        }
        printf("]");
    }

    printf("}");
}

void ast_to_json(ASTNode *node) {
    ast_to_json_inner(node);
    printf("\n");
}

// ─────────────────────────────────────────────
// Звільнення пам'яті
// ─────────────────────────────────────────────

void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->name) free(node->name);
    free(node);
}
