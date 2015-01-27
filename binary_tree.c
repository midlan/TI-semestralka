
#include <stdlib.h>

#include "binary_tree.h"
#include "safe_malloc.h"

binary_node* binary_node_create(unsigned int freq_sum, char c, binary_node* left, binary_node* right) {
    
    binary_node *node = safe_malloc(sizeof(binary_node));
    
    node->freq_sum = freq_sum;
    node->c = c;
    node->left = left;
    node->right = right;
    
    return node;
}

int binary_node_is_leaf(binary_node* node) {
    
    if(node != NULL) {
        return node->right == NULL && node->left == NULL;
    }
    
    return 0;
}

void binary_node_free(binary_node** node) {
    
    if(node != NULL) {
        
        if(*node != NULL) {
            safe_free(*node);
        }
        
        *node = NULL;
    }
}

int binary_node_comp(binary_node** a, binary_node** b) {
    
    if(a != NULL && b != NULL) {
        return (int)((*a)->freq_sum - (*b)->freq_sum);
    }
    
    return 0;
}

void binary_node_free_tree(binary_node** root) {
    
    if(root != NULL) {
        
        if(*root != NULL) {
            binary_node_free_tree(&((*root)->left));
            binary_node_free_tree(&((*root)->right));
        }
        
        binary_node_free(root);
    }
}