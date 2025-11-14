#include "kd_tree.h"
#include <limits>

void nearestPoint(KDNode* root, const vector<double>& target, int depth, KDNode*& nearestNode, double& bestDist) {
    if (root == nullptr) return;

    int axis = depth % 2;

    double dx = root->point[0] - target[0];
    double dy = root->point[1] - target[1];
    double d = dx * dx + dy * dy;
    
    if (d < bestDist) {
        bestDist = d;
        nearestNode = root;
    }

    bool goLeft = target[axis] < root->point[axis];
    KDNode* primary = goLeft ? root->left : root->right;
    KDNode* secondary = goLeft ? root->right : root->left;

    if (primary != nullptr) {
        nearestPoint(primary, target, depth + 1, nearestNode, bestDist);
    }

    double axisDiff = target[axis] - root->point[axis];
    double axisDistSq = axisDiff * axisDiff;
    if (secondary != nullptr && axisDistSq < bestDist) {
        nearestPoint(secondary, target, depth + 1, nearestNode, bestDist);
    }
}

KDNode* findNearest(KDNode* root, vector<double>& target_point) {
    if (!root) return nullptr;
    
    KDNode* nearestNode = nullptr;
    double bestDist = numeric_limits<double>::max();

    nearestPoint(root, target_point, 0, nearestNode, bestDist);

    return nearestNode;
}
