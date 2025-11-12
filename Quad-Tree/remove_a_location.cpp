#include "quadtree.h"


bool samePoint(Point a, Point b, double eps = 1e-9) {
    return (fabs(a.x - b.x) < eps && fabs(a.y - b.y) < eps);
}


QuadNode* removeNode(QuadNode* node, Point target) {
    if (!node) return nullptr;


    if (target.x < node->x_min || target.x > node->x_max ||
        target.y < node->y_min || target.y > node->y_max)
        return node;


    for (auto it = node->points.begin(); it != node->points.end(); ++it) {
        if (samePoint(*it, target)) {
            node->points.erase(it);
            return node;
        }
    }


    if (node->divided) {
        node->nw = removeNode(node->nw, target);
        node->ne = removeNode(node->ne, target);
        node->sw = removeNode(node->sw, target);
        node->se = removeNode(node->se, target);
    }

    return node;
}
