#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "g9cc.h"

enum {
    TK_NUM = 256,
    TK_EOF,
};

Vector *tokens;

int pos;

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

Node *new_node(int ty, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = ty;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = val;
    return node;
}

int consume(int ty) {
    if (current_token(pos)->ty != ty)
        return 0;
    pos++;
    return 1;
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume('+'))
            node = new_node('+', node, mul());
        else if (consume('-'))
            node = new_node('-', node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume('*'))
            node = new_node('*', node, unary());
        else if (consume('/'))
            node = new_node('/', node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume('+'))
        return term();
    if (consume('-'))
        return new_node('-', new_node_num(0), term());
    return term();
}

Node *term() {
    if (consume('(')) {
        Node *node = add();
        if (!consume(')'))
            error("開きカッコに対応する閉じカッコがありません: %s", current_token(pos)->input);
        return node;
    }

    if (current_token(pos)->ty == TK_NUM)
        return new_node_num(current_token(pos++)->val);

    error("数値でも開きカッコでもないトークンです: %s", current_token(pos)->input);
    return NULL;
}

Token *current_token(int pos) {
    return (Token *)tokens->data[pos];
}

void tokenize(char *p) {
    tokens = new_vector();

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
            Token *token = malloc(sizeof(Token));
            token->ty = *p;
            token->input = p;
            vec_push(tokens, token);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            Token *token = malloc(sizeof(Token));
            token->ty = TK_NUM;
            token->input = p;
            token->val = strtol(p, &p, 10);
            vec_push(tokens, token);
            continue;
        }

        fprintf(stderr, "Couldn't tokenize: %s\n", p);
        exit(1);
    }

    Token *token = malloc(sizeof(Token));
    token->ty = TK_EOF;
    token->input = p;
    vec_push(tokens, token);
}

