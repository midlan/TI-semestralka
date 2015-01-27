#ifndef BINARY_TREE_H
#define	BINARY_TREE_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct thebinary_node {
    unsigned int freq_sum;
    char c;
    struct thebinary_node* left;
    struct thebinary_node* right;
} binary_node;

binary_node* binary_node_create(unsigned int freq_sum, char c, binary_node* left, binary_node* right);

int binary_node_is_leaf(binary_node* node);

void binary_node_free(binary_node** node);

int binary_node_comp(binary_node** a, binary_node** b);

void binary_node_free_tree(binary_node** root);

#ifdef	__cplusplus
}
#endif

#endif	/* BINARY_TREE_H */

