#include "kd_tree.h"

KDNode * findMin(KDNode * root , int axis ,int depth){
    if(root == NULL) return nullptr;

    int currAxis = depth % 2;

    if(currAxis == axis){
        if(root->left ==  nullptr) {
            return root;
        }

        return findMin(root->left ,axis , depth + 1);
    }

    KDNode * left = findMin(root->left , axis ,depth + 1);
    KDNode * right = findMin(root->right , axis , depth + 1);

    KDNode * minNode = root;
    if(left != NULL && left->point[axis] < minNode->point[axis]){
        minNode = left;
    }
    if(right != NULL && right->point[axis] < minNode->point[axis]){
        minNode = right;
    }

    return minNode;
}

KDNode* removeNode(KDNode * root , vector<double> &point_rmv, int depth){
    if(root == nullptr) return NULL;

    int axis = depth % 2;

    if(root->point == point_rmv){
        
        if(root->right != NULL){
            KDNode* minNode = findMin(root->right , axis , depth+1);
            root->point = minNode->point;
            root->right = removeNode(root->right , minNode->point , depth + 1);
        }
        else if( root->left != NULL){
            KDNode* minNode = findMin(root->left , axis , depth + 1);
            root->point = minNode->point;
            root->left = removeNode(root->left , minNode->point ,depth + 1 );
            
        }
        else{
            delete root;
            return nullptr;
        }

        return root;
    }

    if(point_rmv[axis] < root->point[axis]){
        root->left = removeNode(root->left , point_rmv , depth + 1);
    }
    else{
        root->right = removeNode(root->right , point_rmv , depth+1);
    }

    return root;
}
