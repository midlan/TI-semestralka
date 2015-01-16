#ifndef BINARY_TREE_H
#define	BINARY_TREE_H

typedef struct thebinary_node {
    int freq_sum;
    char c;
    struct thebinary_node *left;
    struct thebinary_node *right;
} binary_node;

binary_node *binary_node_create(int freq_sum, char c, binary_node *left, binary_node *right);

int binary_node_is_leaf(binary_node *node);

void bindary_node_free(binary_node **node);

int binary_node_comp(binary_node *a, binary_node *b);

void destroy_binary_tree(binary_node **root);

#endif