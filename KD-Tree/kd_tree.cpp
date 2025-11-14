#include "kd_tree.h"


KDNode* insert(KDNode* root, vector<double> point, int depth) {
    if (root == nullptr) {
        return new KDNode(point);
    }
    int axis = depth % 2;
    
    if (point[axis] < root->point[axis]) {
        root->left = insert(root->left, point, depth + 1);
    } else {
        root->right = insert(root->right, point, depth + 1);
    }
    
    return root;
}


void deleteTree(KDNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}


double distance_square(vector<double>& a, vector<double>& b) {
    double diff_X = a[0] - b[0];
    double diff_Y = a[1] - b[1];
    return diff_X * diff_X + diff_Y * diff_Y;
}


void nearestPoint(KDNode* root, vector<double>& target, int depth, KDNode*& nearestNode, double& bestDist) {
    if (root == nullptr) return;
    
    int axis = depth % 2;
    double d = distance_square(root->point, target);
    
    if (d < bestDist) {
        bestDist = d;
        nearestNode = root;
    }
    
    KDNode* next = nullptr;
    KDNode* other = nullptr;
    
    if (target[axis] < root->point[axis]) {
        next = root->left;
        other = root->right;
    } else {
        next = root->right;
        other = root->left;
    }
    
    nearestPoint(next, target, depth + 1, nearestNode, bestDist);
    
    double diff = target[axis] - root->point[axis];
    if (diff * diff < bestDist) {
        nearestPoint(other, target, depth + 1, nearestNode, bestDist);
    }
}


KDNode* findNearest(KDNode* root, vector<double>& target_point, double& bestDist) {
    KDNode* nearestNode = nullptr;
    bestDist = numeric_limits<double>::max();
    nearestPoint(root, target_point, 0, nearestNode, bestDist);
    return nearestNode;
}


KDNode* findMin(KDNode* root, int axis, int depth) {
    if (root == nullptr) return nullptr;
    
    int currAxis = depth % 2;
    
    if (currAxis == axis) {
        if (root->left == nullptr) {
            return root;
        }
        return findMin(root->left, axis, depth + 1);
    }
    
    KDNode* left = findMin(root->left, axis, depth + 1);
    KDNode* right = findMin(root->right, axis, depth + 1);
    KDNode* minNode = root;
    
    if (left != nullptr && left->point[axis] < minNode->point[axis]) {
        minNode = left;
    }
    if (right != nullptr && right->point[axis] < minNode->point[axis]) {
        minNode = right;
    }
    
    return minNode;
}


KDNode* removeNode(KDNode* root, vector<double>& point_rmv, int depth) {
    if (root == nullptr) return nullptr;
    
    int axis = depth % 2;
    
    if (root->point == point_rmv) {
        if (root->right != nullptr) {
            KDNode* minNode = findMin(root->right, axis, depth + 1);
            root->point = minNode->point;
            root->right = removeNode(root->right, minNode->point, depth + 1);
        } else if (root->left != nullptr) {
            KDNode* minNode = findMin(root->left, axis, depth + 1);
            root->point = minNode->point;
            root->right = root->left;
            root->left = nullptr;
            root->right = removeNode(root->right, minNode->point, depth + 1);
        } else {
            delete root;
            return nullptr;
        }
        return root;
    }
    
    if (point_rmv[axis] < root->point[axis]) {
        root->left = removeNode(root->left, point_rmv, depth + 1);
    } else {
        root->right = removeNode(root->right, point_rmv, depth + 1);
    }
    
    return root;
}