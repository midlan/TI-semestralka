
#include <stdlib.h>

#include "binary_tree.h"

binary_node *binary_node_create(int freq_sum, char c, binary_node *left, binary_node *right) {
    
    binary_node *node = malloc(sizeof(binary_node));
    
    if(node != NULL) {
        node->freq_sum = freq_sum;
        node->c = c;
        node->left = left;
        node->right = right;
    }
    
    return node;
}

int binary_node_is_leaf(binary_node *node) {
    
    if(node != NULL) {
        return node->right == NULL && node->left == NULL;
    }
    
    return 0;
}

void bindary_node_free(binary_node **node) {
    
    if(node != NULL) {
        
        if(*node != NULL) {
            free(*node);
        }
        
        *node = NULL;
    }
}

int binary_node_comp(binary_node *a, binary_node *b) {
    
    if(a != NULL && b != NULL) {
        return a->freq_sum - b->freq_sum;
    }
    
    return 0;
}

void destroy_binary_tree(binary_node **root) {
    
    if(root != NULL) {
        
        if(*root != NULL) {
            destroy_binary_tree(&((*root)->left));
            destroy_binary_tree(&((*root)->right));
        }
        
        bindary_node_free(root);
    }
}