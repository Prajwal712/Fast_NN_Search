#include "kd_tree.h"

KDNode* insert(KDNode * root , vector<double> point , int depth){
    if(root == NULL){
        KDNode * newNode = new KDNode(point);
        return newNode;
    }
    int axis = depth % 2;
    
    if(point[axis] < root->point[axis]){
        root->left = insert(root->left , point , depth+1);
    }
    else{
        root->right = insert(root->right , point , depth + 1);
    }
    
    return root;
}     