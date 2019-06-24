#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "g9cc.h"

enum {
    TK_NUM = 256,
    TK_IDENT,
    TK_RETURN,
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

int is_alnum(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           (c == '_');
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

    if (current_token(pos)->ty == TK_IDENT) {
        char varname = current_token(pos++)->input[0];

        Node *node = malloc(sizeof(Node));
        node->ty = ND_LVAR;
        node->offset = (varname - 'a' + 1) * 8;
        return node;
    }

    error("数値でも開きカッコでもないトークンです: %s", current_token(pos)->input);
    return NULL;
}

Node *unary() {
    if (consume('+'))
        return term();
    if (consume('-'))
        return new_node('-', new_node_num(0), term());
    return term();
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

Node *assign() {
    Node *node = add();
    if (consume('='))
        node = new_node('=', node, assign());
    return node;
}

Node *expr() {
    return assign();
}

Node *stmt() {
    Node *node;

    if (consume(TK_RETURN)) {
        node = malloc(sizeof(Node));
        node->ty = ND_RETURN;
        node->lhs = expr();
    } else {
        node = expr();
    }

    if (!consume(';'))
        error("';'ではないトークンです %s\n", current_token(pos)->input);
    return node;
}

void program() {
    int i = 0;
    while(current_token(pos)->ty != TK_EOF)
        code[i++] = stmt();
    code[i] = NULL;
}

Token *current_token(int pos) {
    return (Token *)tokens->data[pos];
}

void tokenize(char *p) {
    tokens = new_vector();

    while (*p) {
        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            Token *token = malloc(sizeof(Token));
            token->ty = TK_RETURN;
            token->input = p;
            p += 6;
            continue;
        }

        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == ';' || *p  ==  '=') {
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

        if ('a' <= *p && *p <= 'z') {
            Token *token = malloc(sizeof(Token));
            token->ty = TK_IDENT;
            token->input = p;
            vec_push(tokens, token);
            p++;
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
