enum {
    ND_NUM = 256,
};

typedef struct Node {
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
} Node;

typedef struct Vector {
    void **data;
    int capacity;
    int len;
} Vector;

typedef struct Token {
    int ty;
    int val;
    char *input;
} Token;

Node *add();
Node *mul();
Node *unary();
Node *term();
Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Vector *new_vector();
void vec_push(Vector *vec, void *elem);
Token *current_token(int pos);
void runtest();
void tokenize(char *p);
void gen(Node *nonde);
