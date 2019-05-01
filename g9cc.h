typedef struct Node {
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
} Node;

Node *add();
Node *term();
Node *mul();
Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
