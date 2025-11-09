#include "kd_tree.h"

void deleteTree(KDNode* root) {
    if (!root) return;
    
    deleteTree(root->left);
    
    deleteTree(root->right);

    delete root;
}

