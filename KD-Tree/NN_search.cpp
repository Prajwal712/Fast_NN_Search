#include "kd_tree.h"

double distance_square(vector<double> &a , vector<double> &b){
    double diff_X = a[0] - b[0];
    double diff_Y = a[1] - b[1];
    return diff_X * diff_X + diff_Y * diff_Y; 
}

void nearestPoint(KDNode * root , vector<double> &target , int depth ,KDNode* &nearestNode , double &bestDist ){
    if(root == nullptr) {
        return ;
    }

    int axis = depth % 2;

    double d = distance_square(root->point , target);

    if(d < bestDist){
        bestDist = d;
        nearestNode = root;
    }

    KDNode* next = nullptr;
    KDNode* other = nullptr;

    if(target[axis] < root->point[axis]){
        next = root->left;
        other = root->right;
    }
    else{
        next = root->right;
        other = root->left;
    }

    nearestPoint(next , target , depth + 1 , nearestNode , bestDist);

    double diff = target[axis] - root->point[axis];
    if(diff * diff < bestDist){
        nearestPoint(other , target , depth +1 , nearestNode , bestDist);
    }

}

KDNode * findNearest(KDNode* root , vector<double> &target_point){
    KDNode * nearestNode = nullptr;

    double bestDist = numeric_limits<double>::max();

    nearestPoint(root , target_point ,0 , nearestNode , bestDist);

    return nearestNode;
}
